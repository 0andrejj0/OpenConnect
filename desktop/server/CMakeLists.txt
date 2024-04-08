
set(SERVER_LIBRARY_NAME server_lib)

# build server

add_library(${SERVER_LIBRARY_NAME}
    STATIC
        "src/server.cpp"
)
target_include_directories(${SERVER_LIBRARY_NAME}
    PUBLIC
        "include"
        "${UDP_SERVER_INCLUDE}"
)
target_link_libraries(${SERVER_LIBRARY_NAME}
    PRIVATE
        spdlog::spdlog
        udp_server
)

# build executable

add_executable(${PROJECT_NAME}
    main.cpp
)

target_link_libraries(${PROJECT_NAME} 
    PRIVATE 
        spdlog::spdlog
        ${SERVER_LIBRARY_NAME}
) 

# install

install(TARGETS ${PROJECT_NAME})

