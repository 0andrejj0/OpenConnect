#include "config.h"

#include <atomic>
#include <filesystem>
#include <list>
#include <thread>

namespace openconnect {

class Server {
public:
    Server();
    void Run();

private: // udp_server

private: // grpc server

private: // config
    const ServerConfig m_config;
    ServerConfig ReadConfig(std::filesystem::path);

private: //
    std::atomic_flag m_stop{false};
    std::list<std::thread> m_threads;

};

} //namespace openconnect