
#include "clipboard_entry.h"
#include "file_entry.h"

#include <condition_variable>
#include <mutex>
#include <optional>
#include <chrono>
#include <functional>
#include <thread>
#include <type_traits>
#include <atomic>

namespace openconnect {


class ClipboardProcessor {
public:
    ClipboardProcessor(std::function<void(openconnect::OptionalClipboardEntryCpp&&)>&& clipboardChangeCallback);

public:
    int setClipboard(ClipboardEntryCpp&& entry) noexcept;

private:

    std::function<void(openconnect::OptionalClipboardEntryCpp)> m_clipboardChangeCallback;
    std::optional<std::thread> m_thread;

    std::atomic<int64_t> m_need_skip{1};
};


}