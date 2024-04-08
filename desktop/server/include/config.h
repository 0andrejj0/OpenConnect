#include <filesystem>

namespace openconnect {
    struct ServerConfig {
        std::filesystem::path imagesSavePath {"~/.openconnect/images"};
        std::filesystem::path videosSavePath {"~/.openconnect/videos"};
        std::filesystem::path filesSavePath  {"~/.openconnect/files"};

        int udp_port{8080};
        int grpc_port{8081};
    };
} // namespace openconnect