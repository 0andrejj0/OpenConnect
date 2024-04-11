
#include "clipboard_entry.h"
#include "file_entry.h"

#include <optional>
#include <chrono>

namespace openconnect {


class ClipboardProcessor {
public:
    ClipboardProcessor() = default;

public:
    int setClipboard(ClipboardEntry&& entry) noexcept;
    OptionalClipboardEntry getClipboard() noexcept;
};


}