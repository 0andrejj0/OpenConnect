#include "grpc_server.h"

#include "file_entry.h"
#include "service.pb.h"
#include "service.grpc.pb.h"

#include <format>
#include <functional>
#include <grpcpp/support/status.h>
#include <spdlog/common.h>
#include <spdlog/spdlog.h>

#include "types.pb.h"

namespace {

class Service
    : public openconnect::Server::Service {
public:
    Service(std::function<int(openconnect::FileEntry&& entry)>&& pushFileCallback)
        : m_pushFileCallback(pushFileCallback)
    {}
public:
    virtual ::grpc::Status PostNotifications(::grpc::ServerContext* context, const ::openconnect::NotificationAggregate* request, ::openconnect::Retcode* response) {
        return ::grpc::Status::OK;
    }

    virtual ::grpc::Status PostKey(::grpc::ServerContext* context, const ::openconnect::KeyboardAggregate* request, ::openconnect::Retcode* response) {
        return ::grpc::Status::OK;
    }

    virtual ::grpc::Status PostClipboard(::grpc::ServerContext* context, const ::openconnect::ClipboardEntry* request, ::openconnect::Retcode* response) {
        return ::grpc::Status::OK;
    }

    virtual ::grpc::Status ReadClipboard(::grpc::ServerContext* context, const ::openconnect::Void* request, ::openconnect::OptionalClipboardEntry* response) {
        return ::grpc::Status::OK;
    }

    virtual ::grpc::Status PostFile(::grpc::ServerContext* context, const ::openconnect::File* request, ::openconnect::Retcode* response) {
        openconnect::FileEntry file;

        file.name = request->has_name() ? request->name() : std::format("{}.{}", "", request->extension());
        file.content = request->file();

        try {
            auto rc = m_pushFileCallback(std::move(file));
            if (rc) {
                SPDLOG_ERROR("Faailed to push file. ErrorCode: {}", rc);
            }
        } catch (const std::exception& e) {
            SPDLOG_ERROR("Faailed to push file. Error: {}", e.what());
        }

        return ::grpc::Status::OK;
    }

private:

    std::function<int(openconnect::FileEntry&& entry)> m_pushFileCallback;
};

}

namespace openconnect {

    
GRPCServer::GRPCServer(
        int port, 
        std::function<int(openconnect::FileEntry&&)>&& pushFileCallback)
    : m_port(port)
    , m_pushFileCallback(pushFileCallback) {
    SPDLOG_INFO("Grpc server initialization...");

}

void GRPCServer::Run() {
    SPDLOG_INFO("Grpc server running...");

    grpc::ServerBuilder builder;
    builder.AddListeningPort(std::format("0.0.0.0:{}", m_port), grpc::InsecureServerCredentials());


    Service service(std::move(m_pushFileCallback));

    builder.RegisterService(&service);

    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    
    server->Wait();
}

void GRPCServer::Stop() {
    SPDLOG_INFO("Grpc server stopping...");
    // TODO
}

}