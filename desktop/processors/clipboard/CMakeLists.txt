

set(LIBRARY_NAME clipboard_processor_lib)

add_library(${LIBRARY_NAME}
    STATIC
        clipboard_processor.cpp
        clipboard_processor.h
)
target_include_directories(${LIBRARY_NAME}
    PUBLIC
        "${COMMON_INCLUDE_DIR}"
        "${CMAKE_CURRENT_LIST_DIR}"
)
target_link_libraries(${LIBRARY_NAME}
    PRIVATE
        spdlog::spdlog
)

set(CLIPBOARD_PROCESSOR_INCLUDE "${CMAKE_CURRENT_LIST_DIR}" CACHE INTERNAL "")