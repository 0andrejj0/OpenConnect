

set(LIBRARY_NAME udp_server_lib)

add_library(${LIBRARY_NAME}
    src/udp_server.cpp
)
target_include_directories(${LIBRARY_NAME}
    PRIVATE
        include
)
target_link_libraries(${LIBRARY_NAME}
    PRIVATE
        MinimalSocket
        spdlog::spdlog
)

set(UDP_SERVER_INCLUDE "${CMAKE_CURRENT_LIST_DIR}/include" CACHE INTERNAL "")