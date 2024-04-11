#include "fs_processor.h"

#include "spdlog/spdlog.h"
#include <exception>
#include <filesystem>
#include <fstream>
#include <ios>
#include <mutex>
#include <stdexcept>
#include <map>

namespace {
    enum class FileType {
        image = 0,
        video = 1,
        other = 2
    };

    FileType getFileType(const std::filesystem::path& name) {
        static const std::map<std::string, FileType> types = {
            {".png", FileType::image},
            {".jpeg", FileType::image},
            {".gif", FileType::image},
            {".jpg", FileType::image},
            {".tiff", FileType::image},
            {".hiec", FileType::image},

            {".mp4", FileType::video},
            {".mpeg-4", FileType::video},
            {".m4v", FileType::video},
            {".mov", FileType::video},
        };

        auto it = types.find(name.extension());
        if (it != types.end())
            return it->second;
        return FileType::other;
    }
}

namespace openconnect {
    FSProcessor::FSProcessor(
        std::filesystem::path imageDir,
        std::filesystem::path videosDir,
        std::filesystem::path filesDir
    )
        : m_imageDir(imageDir)
        , m_videosDir(videosDir)
        , m_filesDir(filesDir)
    {
        SPDLOG_INFO("Directories: {} {} {}", m_imageDir.c_str(), m_videosDir.c_str(), m_filesDir.c_str());
        if (!std::filesystem::is_directory(m_imageDir) ||
            !std::filesystem::is_directory(m_videosDir) ||
            !std::filesystem::is_directory(m_filesDir)
        ) throw std::invalid_argument("directory for file saving must exist");
    }


    std::filesystem::path FSProcessor::getFullPath(const std::filesystem::path& name) noexcept {
        auto type = getFileType(name);
        switch (type) {
            case FileType::image:
                return m_imageDir / name;
            case FileType::video:
                return m_videosDir / name;
            case FileType::other:
                return m_filesDir / name;
            default:
                SPDLOG_ERROR("unknown file type: {}", name.c_str());
                return m_filesDir / name;
        }
    }

    int FSProcessor::PushFile(FileEntry&& entry) noexcept try {
        SPDLOG_DEBUG("Processing file: {}", entry.name);
        auto fullPath = getFullPath(entry.name);

        SPDLOG_INFO("Write file: {}", fullPath.c_str());

        std::lock_guard lock(m_mutex);
        std::ofstream out(fullPath, std::ios::binary);
        out.write(entry.content.data(), entry.content.size());
        return 0;
    } catch (const std::exception& e) {
        SPDLOG_ERROR("Failed to process recieved file");
        return 1;
    }
}