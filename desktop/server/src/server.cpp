#include "server.h"
#include "file_entry.h"
#include "types.pb.h"

#include <cstdlib>
#include <exception>
#include <filesystem>
#include <spdlog/spdlog.h>

namespace {

std::filesystem::path FixPath(const std::filesystem::path& p) noexcept {
    if (!p.empty() && p.c_str()[0] == '~') {
        auto home = std::getenv("HOME");
        if (home) {
            SPDLOG_INFO("Home path is {}", home);

            static constexpr std::string delim = 
            #ifdef _WIN32
                "\\";
            # else
                "/";
            #endif

            return std::string(home) + delim + p.generic_string().substr(1);
        }
    }
    return p;
}

}

namespace openconnect {

    Server::Server()
        : m_FSProcessor(
            FixPath(m_config.imagesSavePath),
            FixPath(m_config.videosSavePath),
            FixPath(m_config.filesSavePath))
        , m_ClipboardProcessor({})
    {
        SPDLOG_INFO("Server initialization...");

        m_NotificationProcessor.PushNotification({NotificationEntryCpp{.senderName="from mee", .text="MESSSAGE"}});
        m_ClipboardProcessor.setClipboard({"KEK_CLIPBOARD"});

        m_UDPServer.emplace(
            [this](const std::string& s) {
                return UDPServerCallback(s);
            },
            m_config.udp_port
        );

        m_GRPCServer.emplace(
            m_config.grpc_port,
            [this](openconnect::FileEntry&& entry) -> int {
                try {
                    m_FSProcessor.PushFile(std::move(entry));
                } catch (const std::exception& e) {
                    SPDLOG_ERROR("Failed to save file");
                    return 1;
                }
                return 0;
            },
            [this](openconnect::NotificationAggregateCpp&& entry) -> int {
                try {
                    m_NotificationProcessor.PushNotification(std::move(entry));
                } catch (const std::exception& e) {
                    SPDLOG_ERROR("Failed to display notifications");
                    return 1;
                }
                return 0;
            }
        );
    }

    void Server::Run() {
        SPDLOG_INFO("Server starting...");

        if (m_UDPServer.has_value()) {
            m_threads.emplace_back([this]{
                m_UDPServer->Run();
            });
        }

        if (m_GRPCServer.has_value()) {
            m_threads.emplace_back([this]{
                m_GRPCServer->Run();
            });
        }

        for (auto& t : m_threads)
            t.join();

        SPDLOG_INFO("Server stopped");
    }

    void Server::Stop() {
        SPDLOG_INFO("Server stoppng...");

        if (m_UDPServer.has_value()) {
            m_UDPServer->Stop();
        }

        if (m_GRPCServer.has_value()) {
            m_GRPCServer->Stop();
        }

        for (auto& t: m_threads)
            t.join();

        SPDLOG_INFO("Server stopped");
    }

    Server::~Server() {
        Stop();
    }

    std::string Server::UDPServerCallback(const std::string& req) {
        if (req == "/connection")
            return "OK";
        return "";
    }

}