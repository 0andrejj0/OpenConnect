

add_executable(${PROJECT_NAME}
    main.cpp
)

target_link_libraries(${PROJECT_NAME} 
    PRIVATE 
        spdlog::spdlog) 

install(TARGETS ${PROJECT_NAME}
        DESTINATION "kek")

