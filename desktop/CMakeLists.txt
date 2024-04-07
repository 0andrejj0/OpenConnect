cmake_minimum_required (VERSION 3.28)

project (OpenConnectServer CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

if (NOT COMMON_DIR)
    message(FATAL_ERROR "You must specify COMMON_DIR var")
endif()

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

if (NOT USE_SYSTEM_SPDLOG)
    message("Use spdlog from github")
    include(FetchContent)

    FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog
        GIT_TAG        v1.x
    )
    set(FETCHCONTENT_QUIET OFF)
    FetchContent_MakeAvailable(spdlog)
else()
    message("Use local spdlog")
    find_package(spdlog REQUIRED)
endif()

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(grpc_server)
add_subdirectory(server)

project (OpenConnect CXX)