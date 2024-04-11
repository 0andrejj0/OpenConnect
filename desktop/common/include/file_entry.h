#pragma once

#include <filesystem>
#include <vector>
#include <string>

namespace openconnect {
    struct FileEntry {
        std::filesystem::path name;
        std::string content;
    };
}