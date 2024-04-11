#include "config.h"

#include "udp_server.h"
#include "grpc_server.h"
#include "fs_processor.h"

#include <atomic>
#include <filesystem>
#include <list>
#include <optional>
#include <thread>

namespace openconnect {

class Server {
public:
    Server();

    void Run();
    void Stop();

    ~Server();

private: // udp_server
    std::optional<UDPServer> m_UDPServer;
    std::string UDPServerCallback(const std::string& req);

private: // grpc server
    std::optional<GRPCServer> m_GRPCServer;

private: // config
    const ServerConfig m_config;

private:
    FSProcessor m_FSProcessor;

private: // common
    std::atomic_flag m_stop{false};
    std::list<std::thread> m_threads;

};

} //namespace openconnect