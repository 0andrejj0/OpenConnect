#include "server.h"

#include <spdlog/spdlog.h>

namespace openconnect {

    Server::Server() 
    {
        SPDLOG_INFO("Server initialization...");
        m_UDPServer.emplace(
            [this](const std::string& s) {
                return UDPServerCallback(s);
            },
            m_config.udp_port
        );
    }

    void Server::Run() {
        SPDLOG_INFO("Server starting...");

        if (m_UDPServer.has_value()) {
            m_threads.emplace_back([this]{
                m_UDPServer->Run();
            });
        }

        SPDLOG_INFO("Server stopped");
    }

    void Server::Stop() {
        SPDLOG_INFO("Server stoppng...");

        if (m_UDPServer.has_value()) {
            m_UDPServer->Stop();
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