#include "udp_server.h"

#include <spdlog/spdlog.h>

#include <MinimalSocket/udp/UdpSocket.h>

#include <atomic>
#include <chrono>
#include <stdexcept>
#include <thread>


namespace openconnect {

UDPServer::UDPServer(std::function<std::string(std::string)>&& callback, int port) 
    : m_callback(callback)
    , m_port(port)
{
    SPDLOG_INFO("UDP Server initialize...");
}

void UDPServer::Run() {
    using namespace std::chrono_literals;
    MinimalSocket::Port socket_port = m_port;
    MinimalSocket::udp::Udp<true> udp_socket(socket_port,
                                        MinimalSocket::AddressFamily::IP_V4);
    bool success = udp_socket.open();
    if (!success) {
        throw std::runtime_error("failed to bind socket");
    }

    while (!m_stop.load(std::memory_order_relaxed)) {
        try {
            // get request
            constexpr size_t maxMessageLen = 40;
            auto message = udp_socket.receive(maxMessageLen, 500ms);
            if (!message.has_value()) {
                std::chrono::duration sleepTime{50ms};
                std::this_thread::sleep_for(sleepTime);
                continue;
            }

            // call callback
            auto response = m_callback(message.value().received_message);

            // send response
            udp_socket.sendTo(response, message->sender);

        } catch(const std::exception& e) {
            SPDLOG_ERROR("UDP Server loop failed: {}", e.what());
        }
    }
    m_stopped.store(true);
}

void UDPServer::Stop() {
    m_stop.store(true);
    m_stopped.wait(false);
}

}