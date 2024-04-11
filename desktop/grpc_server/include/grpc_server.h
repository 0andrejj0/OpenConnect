#include "file_entry.h"

#include <functional>
#include <spdlog/spdlog.h>

#include <grpc/grpc.h>
#include <grpcpp/server_builder.h>

#include <atomic>

namespace openconnect {

class GRPCServer {
public:
    GRPCServer(int port, std::function<int(openconnect::FileEntry&&)>&& pushFileCallback);
    void Run();
    void Stop();

private:
    // rpc PostNotifications (NotificationAggregate) returns (Retcode)                {}

    // rpc PostKey           (KeyboardAggregate)     returns (Retcode)                {}

    // rpc PostClipboard     (ClipboardEntry)        returns (Retcode)                {}
    // rpc ReadClipboard     (Void)                  returns (OptionalClipboardEntry) {}

    // rpc PostFile          (File)                  returns (Retcode)                {}

    std::atomic<bool> m_stop{false};
    std::atomic<bool> m_stopped{false};
    int m_port;

    std::function<int(openconnect::FileEntry&&)> m_pushFileCallback;

    std::function<void()> StopCallback;
};

} // namespace openconnect