

#include <atomic>
#include <functional>
#include <string>

namespace openconnect {

class UDPServer {
public:
    UDPServer(std::function<std::string(std::string)>&&, int port);

    void Run();
    void Stop();

private:
    std::atomic<bool> m_stop{false};
    std::atomic<bool> m_stopped{false};
    std::function<std::string(std::string)> m_callback;
    int m_port{0};
};


} // namespace openconnect