#include "file_entry.h"
#include "clipboard_entry.h"
#include "notification_entry.h"

#include <functional>
#include <spdlog/spdlog.h>

#include <grpc/grpc.h>
#include <grpcpp/server_builder.h>

#include <atomic>

namespace openconnect {

class GRPCServer {
public:
    GRPCServer(
        int port,
        std::function<int(openconnect::FileEntry&&)>&& pushFileCallback,
        std::function<int(openconnect::NotificationAggregateCpp&&)>&& pushNotificationCallback,
        std::function<void(openconnect::ClipboardEntryCpp&&)>&&,
        std::function<OptionalClipboardEntryCpp()>&& readNotificationsFunc
    );
    void Run();
    void Stop();

private:
    // rpc PostNotifications (NotificationAggregate) returns (Retcode)                {}

    // rpc PostKey           (KeyboardAggregate)     returns (Retcode)                {}

    // rpc PostClipboard     (ClipboardEntry)        returns (Retcode)                {}
    // rpc ReadClipboard     (Void)                  returns (OptionalClipboardEntry) {}

    // rpc PostFile          (File)                  returns (Retcode)                {}

    int m_port;

    std::function<int(openconnect::FileEntry&&)> m_pushFileCallback;
    std::function<int(openconnect::NotificationAggregateCpp&&)> m_pushNotificationCallback;
    std::function<void(openconnect::ClipboardEntryCpp&&)> m_pushClipboardEntryCallback;
    std::function<OptionalClipboardEntryCpp()> m_readNotificationsFunc;
};

} // namespace openconnect