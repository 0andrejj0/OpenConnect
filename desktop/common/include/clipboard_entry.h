#pragma once

#include <filesystem>
#include <vector>
#include <string>
#include <variant>
#include <optional>

#include "file_entry.h"

namespace openconnect {
    struct ClipboardEntryCpp {
        std::variant<std::string, FileEntry> content;
    };
    using OptionalClipboardEntryCpp = std::optional<ClipboardEntryCpp>;
}