#include <spdlog/spdlog.h>

#include <filesystem>

#include "file_entry.h"

namespace openconnect {

class FSProcessor {
public:
    explicit FSProcessor(
        std::filesystem::path imageDir,
        std::filesystem::path videosDir,
        std::filesystem::path filesDir
    );

public:
    int PushFile(FileEntry&& f) noexcept;

private:
    std::filesystem::path getFullPath(const std::filesystem::path& name) noexcept;

private:
    std::filesystem::path m_imageDir;
    std::filesystem::path m_videosDir;
    std::filesystem::path m_filesDir;

    std::mutex            m_mutex;
};

}