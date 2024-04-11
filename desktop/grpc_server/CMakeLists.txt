
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

target_include_directories(protolib 
    PUBLIC 
        "${CMAKE_CURRENT_BINARY_DIR}"
)

get_target_property(grpc_cpp_plugin_location gRPC::grpc_cpp_plugin LOCATION)
protobuf_generate(TARGET protolib 
    LANGUAGE cpp
    IMPORT_DIRS ${COMMON_DIR}
)
protobuf_generate(
    TARGET 
        protolib 
    LANGUAGE
        grpc 
    GENERATE_EXTENSIONS
        .grpc.pb.h
        .grpc.pb.cc
    PLUGIN
        "protoc-gen-grpc=${grpc_cpp_plugin_location}"
    IMPORT_DIRS ${COMMON_DIR}
)

# server

set (LIBRARY_NAME grpc_server_lib)

add_library(${LIBRARY_NAME}
    STATIC
        src/grpc_server.cpp
)
target_include_directories(${LIBRARY_NAME}
    PUBLIC
        "include"
        "${COMMON_INCLUDE_DIR}"
        "${CMAKE_CURRENT_BINARY_DIR}"
)
target_link_libraries(${LIBRARY_NAME}
    PRIVATE
        spdlog::spdlog
        protolib
)

set (GRPC_SERVER_INCLUDE ${CMAKE_CURRENT_LIST_DIR}/include CACHE INTERNAL "")
set (GRPC_SERVER_INCLUDE_ADD ${CMAKE_CURRENT_BINARY_DIR} CACHE INTERNAL "")