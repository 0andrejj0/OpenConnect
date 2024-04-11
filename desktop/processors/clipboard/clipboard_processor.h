
#include "clipboard_entry.h"
#include "file_entry.h"

#include <optional>
#include <chrono>
#include <functional>

namespace openconnect {


class ClipboardProcessor {
public:
    ClipboardProcessor(std::function<void(openconnect::OptionalClipboardEntryCpp)>&& clipboardChangeCallback);

public:
    int setClipboard(ClipboardEntryCpp&& entry) noexcept;

private:
    std::function<void(openconnect::OptionalClipboardEntryCpp)> m_clipboardChangeCallback;
};


}