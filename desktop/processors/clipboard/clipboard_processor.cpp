#include "clipboard_processor.h"
#include "file_entry.h"
#include "spdlog/common.h"

#include <string>
#include <unistd.h>
#include <variant>

#include <spdlog/spdlog.h>

namespace {


// std::string execSimple(const char* cmd) {
//     std::array<char, 128> buffer;
//     std::string result;
//     std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "rw"), pclose);
//     if (!pipe) {
//         throw std::runtime_error("popen() failed!");
//     }
//     while (fgets(buffer.data(), static_cast<int>(buffer.size()), pipe.get()) != nullptr) {
//         result += buffer.data();
//     }
//     return result;
// }

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

} // namespace

namespace openconnect {

ClipboardProcessor::ClipboardProcessor(std::function<void(openconnect::OptionalClipboardEntryCpp)>&& clipboardChangeCallback)
    : m_clipboardChangeCallback(clipboardChangeCallback) {
}

int ClipboardProcessor::setClipboard(ClipboardEntryCpp&& entry) noexcept {
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

// OptionalClipboardEntryCpp ClipboardProcessor::getClipboard() noexcept {
//     using namespace std::chrono_literals;
//     auto timeout = 500ms;

//     return {};
// }

} // namespace openconnect