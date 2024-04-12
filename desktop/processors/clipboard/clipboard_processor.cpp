#include "clipboard_processor.h"
#include "clipboard_entry.h"
#include "file_entry.h"
#include "spdlog/common.h"

#include <cstdio>
#include <cstdlib>
#include <exception>
#include <filesystem>
#include <fmt/core.h>
#include <string>
#include <string_view>
#include <sys/types.h>
#include <thread>
#include <unistd.h>
#include <variant>
#include <fstream>

#include <spdlog/spdlog.h>

namespace {

openconnect::OptionalClipboardEntryCpp readClipboardLinux() noexcept {
    static constexpr std::string_view command = "wl-paste -n";
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command.data(), "r"), pclose);
    if (!pipe) {
        SPDLOG_ERROR("Failed to create pipe to read clipboard");
        return {};
    }

    static std::array<char, 20'000'000> buffer;
    auto rc = fread(buffer.data(), 1, buffer.size(), pipe.get());
    if (rc <= 0) {
        SPDLOG_ERROR("Failed to read clipboard from pipe, res: {}", rc);
    }
    openconnect::OptionalClipboardEntryCpp res;
    res.emplace(std::string(buffer.begin(), buffer.begin() + rc));
    return res;
}

void workerFuncLinux(const std::function<void(openconnect::OptionalClipboardEntryCpp)>& calback, std::atomic<int64_t>& needSkip) {
    static constexpr std::string_view scriptPath = "/tmp/openconnect_notify_new_clipboard.sh";
    try {
        {
            std::ofstream out(scriptPath.data());
            out << "echo -n \"f\"\n";
        }
        {
            std::filesystem::permissions(scriptPath.data(), std::filesystem::perms::all);
        }
    } catch(const std::exception& e) {
        SPDLOG_ERROR("filed to write notification script: {}", e.what());
        return;
    }
     
    static constexpr std::string_view command = "wl-paste -w /tmp/openconnect_notify_new_clipboard.sh"; 
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command.data(), "r"), pclose);

    if (!pipe) {
        SPDLOG_ERROR("Failed to create pipe");
    }

    while (true) try {
        static std::array<char, 100> buffer;
        auto rc = fread(buffer.data(), 1, 1, pipe.get());
        if (rc > 0) {
            if (needSkip.fetch_sub(1) > 0) {
                SPDLOG_INFO("Read buffer skipped, this is self-own change");
                continue;
            } else {
                needSkip.fetch_add(1);
            }
            auto res = readClipboardLinux();
            SPDLOG_INFO("Clipboard changed notification");
            try {
                calback(res);
            } catch (const std::exception& e) {
                SPDLOG_ERROR("Exception from clipboard change callback: {}", e.what());
            }
        } else {
            using namespace std::chrono_literals;
            std::this_thread::sleep_for(50ms);
            continue;
        }

    } catch (const std::exception& e) {
        SPDLOG_INFO("Clipboard changed notification, but fail: {}", e.what());
    }
}

#if defined(__linux)
void setClipboardLinux(std::string&& content) noexcept {
    static constexpr std::string_view command = "wl-copy";
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command.data(), "w"), pclose);
    if (!pipe) {
        SPDLOG_ERROR("Failed to create pipe");
        return;
    }
    auto rc = fwrite(content.data(), 1, content.size(), pipe.get());
    if (rc == content.length()) {
        SPDLOG_INFO("Writing to clipboard OK");
    } else {
        SPDLOG_INFO("Writing to clipboard ERROR: content: {}, writed: {}", content.length(), rc);
    }
}
#endif

} // namespace

namespace openconnect {

ClipboardProcessor::ClipboardProcessor(std::function<void(openconnect::OptionalClipboardEntryCpp&&)>&& clipboardChangeCallback)
    : m_clipboardChangeCallback(clipboardChangeCallback) {

    m_thread.emplace(workerFuncLinux, clipboardChangeCallback, std::reference_wrapper<std::atomic<int64_t>>(m_need_skip));
}

int ClipboardProcessor::setClipboard(ClipboardEntryCpp&& entry) noexcept {
    m_need_skip.fetch_add(1);
    #if defined(__linux)
        if (std::holds_alternative<std::string>(entry.content)) {
            setClipboardLinux(std::move(std::get<std::string>(entry.content)));
        } else
        if (std::holds_alternative<openconnect::FileEntry>(entry.content)) {
            setClipboardLinux(std::move(std::get<openconnect::FileEntry>(entry.content).content));
        }
    return 0;
    #endif
}

} // namespace openconnect