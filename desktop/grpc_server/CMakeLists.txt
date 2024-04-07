
# protos

find_package(protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)
find_package(Threads)

set(PROTO_FILES
    "${COMMON_DIR}/types.proto"
    "${COMMON_DIR}/service.proto"
)

add_library(protolib 
    ${PROTO_FILES}
)

target_link_libraries(protolib
    PUBLIC
        protobuf::libprotobuf
        gRPC::grpc
        gRPC::grpc++
)

target_include_directories(protolib PUBLIC ${COMMON_DIR})

protobuf_generate(TARGET protolib 
    LANGUAGE cpp
    IMPORT_DIRS ${COMMON_DIR}
)

# server

set (LIBRARY_NAME grpc_server)

add_library(${LIBRARY_NAME}
    STATIC
        src/server.cpp
)
target_include_directories(${LIBRARY_NAME}
    PUBLIC
        "include"
)