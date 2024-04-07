#include <filesystem>

namespace openconnect {
    struct ServerConfig {
        std::filesystem::path imagesSavePath;
        std::filesystem::path videosSavePath;
        std::filesystem::path filesSavePath;
    };
} // namespace openconnect