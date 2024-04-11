#include "clipboard_processor.h"

namespace openconnect {

int ClipboardProcessor::setClipboard(ClipboardEntry&& entry) noexcept {
    return 0;
}

OptionalClipboardEntry ClipboardProcessor::getClipboard() noexcept {
    using namespace std::chrono_literals;
    auto timeout = 500ms;

    return {};
}

} // namespace openconnect