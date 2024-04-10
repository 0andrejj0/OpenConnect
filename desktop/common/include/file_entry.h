#include <filesystem>
#include <vector>

namespace openconnect {
    struct FileEntry {
        std::filesystem::path name;
        std::vector<uint8_t> content;
    };
}