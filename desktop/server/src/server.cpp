#include "server.h"

#include <spdlog/spdlog.h>

namespace openconnect {

    Server::Server() {
        SPDLOG_INFO("Server initialization...");
    }

    void Server::Run() {
        SPDLOG_INFO("Server starting...");



        SPDLOG_INFO("Server stopped");
    }

}