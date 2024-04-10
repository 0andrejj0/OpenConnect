set(FS_PROCESSOR_LIBRARY_NAME fs_processor_lib CACHE INTERNAL "")

set(FS_PROCESSOR_INCLUDE "${CMAKE_CURRENT_LIST_DIR}/include" CACHE INTERNAL "")

add_library(${FS_PROCESSOR_LIBRARY_NAME}
    STATIC
        "src/fs_processor.cpp"
)

target_include_directories(${FS_PROCESSOR_LIBRARY_NAME}
    PUBLIC
        "include"
        "${FS_PROCESSOR_INCLUDE}"
        "${COMMON_INCLUDE_DIR}"
)
target_link_libraries(${FS_PROCESSOR_LIBRARY_NAME}
    PRIVATE
        spdlog::spdlog
)