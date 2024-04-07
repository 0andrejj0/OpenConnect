#include <exception>
#include <server.h>

#include <spdlog/spdlog.h>

int main(int argc, char** argv) {
    SPDLOG_INFO("Starting...");

    try {
        openconnect::Server().Run();
    } catch (const std::exception& e) {
        spdlog::critical("Fail: {}", e.what());
        return 1;
    }

}