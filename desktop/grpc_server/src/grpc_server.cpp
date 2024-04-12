#include "grpc_server.h"

#include "clipboard_entry.h"
#include "file_entry.h"
#include "service.pb.h"
#include "service.grpc.pb.h"

#include <exception>
#include <format>
#include <functional>
#include <grpcpp/support/status.h>
#include <spdlog/common.h>
#include <spdlog/spdlog.h>
#include <string>
#include <variant>

#include "spdlog/fmt/bundled/core.h"
#include "types.pb.h"

namespace {

class Service
    : public openconnect::Server::Service {
public:
    Service(
            std::function<int(openconnect::FileEntry&& entry)>&& pushFileCallback,
            std::function<int(openconnect::NotificationAggregateCpp&&)>&& pushNotificationCallback,
            std::function<void(openconnect::ClipboardEntryCpp&&)>&& pushClipboardEntryCallback,
            std::function<openconnect::OptionalClipboardEntryCpp()>&& readNotificationsFunc)
        : m_pushFileCallback(pushFileCallback)
        , m_pushNotificationCallback(pushNotificationCallback)
        , m_pushClipboardEntryCallback(pushClipboardEntryCallback)
        , m_readNotificationsFunc(readNotificationsFunc)
    {}
public:
    virtual ::grpc::Status PostNotifications(::grpc::ServerContext* context, const ::openconnect::NotificationAggregate* request, ::openconnect::Retcode* response) {
        try {
            SPDLOG_INFO("Recieved notifications");
            openconnect::NotificationAggregateCpp notifications;
            for (auto& notification: request->notifications())
                notifications.push_back({notification.from(), notification.data()});
            auto rc = m_pushNotificationCallback(std::move(notifications));
        } catch(const std::exception& e) {
            SPDLOG_ERROR("Failed to process recieved torifications: {}", e.what());
        }
        return ::grpc::Status::OK;
    }

    virtual ::grpc::Status PostKey(::grpc::ServerContext* context, const ::openconnect::KeyboardAggregate* request, ::openconnect::Retcode* response) {
        SPDLOG_INFO("Enter text req");
        for (auto key : request->events()) {
                auto command = std::format(("wtype \"{}\""), char(key.key()));
                std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command.c_str(), "r"), pclose);
                if (!pipe) {
                    SPDLOG_ERROR("Failed to enter key");
                    return {};
                }
        }
        return ::grpc::Status::OK;
    }

    virtual ::grpc::Status PostClipboard(::grpc::ServerContext* context, const ::openconnect::ClipboardEntry* request, ::openconnect::Retcode* response) {
        SPDLOG_INFO("Recieved set clipboard request");

        openconnect::ClipboardEntryCpp entry;
        if (request->has_clipboardtext()) {
            entry.content = request->clipboardtext();
        } else if (request->has_file()) {
            entry.content = openconnect::FileEntry{.name=request->file().name(), .content = request->file().file()};
        }

        m_pushClipboardEntryCallback(std::move(entry));

        return ::grpc::Status::OK;
    }

    virtual ::grpc::Status ReadClipboard(::grpc::ServerContext* context, const ::openconnect::Void* request, ::openconnect::OptionalClipboardEntry* response) {
        try {
            auto res = m_readNotificationsFunc();
            if (res.has_value()) {
                ::openconnect::ClipboardEntry value;
                if (std::holds_alternative<std::string>(res->content)) {
                    value.set_clipboardtext(std::get<std::string>(res->content));
                } else {
                    SPDLOG_ERROR("currently can't reach this code");
                }
                *response->mutable_clipboardentry() = value;
            }
        } catch(const std::exception& e) {
            SPDLOG_ERROR("Failed to read clipboard: {}", e.what());
        }
        return ::grpc::Status::OK;
    }

    virtual ::grpc::Status PostFile(::grpc::ServerContext* context, const ::openconnect::File* request, ::openconnect::Retcode* response) {
        openconnect::FileEntry file;

        file.name = request->has_name() ? request->name() : std::format("{}.{}", "", request->extension());
        file.content = request->file();

        try {
            auto rc = m_pushFileCallback(std::move(file));
            if (rc) {
                SPDLOG_ERROR("Push file callback failed. ErrorCode: {}", rc);
            }
        } catch (const std::exception& e) {
            SPDLOG_ERROR("Exception from push file callback. Error: {}", e.what());
        }

        return ::grpc::Status::OK;
    }

private:

    std::function<openconnect::OptionalClipboardEntryCpp()> m_readNotificationsFunc;
    std::function<int(openconnect::FileEntry&& entry)> m_pushFileCallback;
    std::function<void(openconnect::ClipboardEntryCpp&&)> m_pushClipboardEntryCallback;
    std::function<int(openconnect::NotificationAggregateCpp&&)> m_pushNotificationCallback;
};

}

namespace openconnect {

    
GRPCServer::GRPCServer(
        int port, 
        std::function<int(openconnect::FileEntry&&)>&& pushFileCallback,
        std::function<int(openconnect::NotificationAggregateCpp&&)>&& notificationCallback,
        std::function<void(openconnect::ClipboardEntryCpp&&)>&& pushClipboardEntryCallback,
        std::function<OptionalClipboardEntryCpp()>&& readNotificationsFunc)
    : m_port(port)
    , m_pushFileCallback(pushFileCallback)
    , m_pushClipboardEntryCallback(pushClipboardEntryCallback)
    , m_readNotificationsFunc(readNotificationsFunc)
    , m_pushNotificationCallback(notificationCallback)
{
    SPDLOG_INFO("Grpc server initialization...");
}

void GRPCServer::Run() {
    SPDLOG_INFO("Grpc server running...");

    grpc::ServerBuilder builder;
    builder.AddListeningPort(std::format("0.0.0.0:{}", m_port), grpc::InsecureServerCredentials());


    Service service(
        std::move(m_pushFileCallback),
        std::move(m_pushNotificationCallback),
        std::move(m_pushClipboardEntryCallback),
        std::move(m_readNotificationsFunc)
    );

    builder.RegisterService(&service);

    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    
    server->Wait();
}

void GRPCServer::Stop() {
    SPDLOG_INFO("Grpc server stopping...");
    // TODO
}

}