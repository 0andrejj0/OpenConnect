cmake_minimum_required (VERSION 3.28)

project (OpenCOnnectServer CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(CMAKE_CXX_STANDARD 23)

if (NOT USE_SYSTEM_GRPC)
    message("Use gRPC from github")
    include(FetchContent)

    set(ABSL_ENABLE_INSTALL ON)
    set(ABSL_PROPAGATE_CXX_STD ON)

    FetchContent_Declare(
        gRPC
        GIT_REPOSITORY https://github.com/grpc/grpc
        GIT_TAG        v1.62.0
    )
    set(FETCHCONTENT_QUIET OFF)
    FetchContent_MakeAvailable(gRPC)
else()
    message("Use local gRPC")
    find_package(gRPC REQUIRED)
endif()




project (OpenConnect CXX)