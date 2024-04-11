
set(SERVER_LIBRARY_NAME server_lib)

# build server

add_library(${SERVER_LIBRARY_NAME}
    STATIC
        "src/server.cpp"
)
target_include_directories(${SERVER_LIBRARY_NAME}
    PUBLIC
        "${COMMON_INCLUDE_DIR}"
        "include"
        "${UDP_SERVER_INCLUDE}"
        "${GRPC_SERVER_INCLUDE}"
        "${FS_PROCESSOR_INCLUDE}"
        "${NOTIFICATION_PROCESSOR_INCLUDE}"
        "${CLIPBOARD_PROCESSOR_INCLUDE}"
        "${CMAKE_CURRENT_BINARY_DIR}"
        "${GRPC_SERVER_INCLUDE_ADD}"
)
message(WARNING "${CMAKE_CURRENT_BINARY_DIR}")
target_link_libraries(${SERVER_LIBRARY_NAME}
    PRIVATE
        spdlog::spdlog
        udp_server_lib
        fs_processor_lib
        grpc_server_lib
        notification_processor_lib
        clipboard_processor_lib
)

# build executable

add_executable(${PROJECT_NAME}
    main.cpp
)
target_include_directories(${PROJECT_NAME}
    PRIVATE
        "${COMMON_INCLUDE_DIR}"
)
target_link_libraries(${PROJECT_NAME} 
    PRIVATE 
        spdlog::spdlog
        ${SERVER_LIBRARY_NAME}
) 

# install

install(TARGETS ${PROJECT_NAME})

