
set(LIBRARY_NAME notification_processor_lib)

set(PLATFORM_INCLUDE)
set(PLATFORM_LIB)

if (UNIX)
        FIND_PATH( LIBNOTIFY_INCLUDE_DIR "notify.h"
        PATHS /usr/include
            PATH_SUFFIXES "libnotify" 
            REQUIRED
        )
        set (LIBNOTIFY_INCLUDE_DIRS ${LIBNOTIFY_INCLUDE_DIR})
        set (PLATFORM_INCLUDE ${LIBNOTIFY_INCLUDE_DIR})

        FIND_LIBRARY( LIBNOTIFY_LIBRARY
            NAMES "libnotify" "libnotify.so"
            PATHS /usr/lib 
            REQUIRED
        )
        set (LIBNOTIFY_LIBRARIES ${LIBNOTIFY_LIBRARY})
        set(PLATFORM_LIB ${LIBNOTIFY_LIBRARY})

        # set()

endif (UNIX)

add_library(${LIBRARY_NAME}
    notification_processor.cpp
    notification_processor.h
)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
if (UNIX)
target_include_directories(${LIBRARY_NAME}
    PRIVATE
        "/usr/include/glib-2.0/"
        "/usr/lib/glib-2.0/include/"
        "/usr/include/gdk-pixbuf-2.0/"
        "${COMMON_INCLUDE_DIR}"
        "${CMAKE_CURRENT_LIST_DIR}"
        "${PLATFORM_INCLUDE}"
)
else (UNIX)
target_include_directories(${LIBRARY_NAME}
    PRIVATE
        "${COMMON_INCLUDE_DIR}"
        "${CMAKE_CURRENT_LIST_DIR}"
        "${PLATFORM_INCLUDE}"
)
endif(UNIX)
target_link_libraries(${LIBRARY_NAME}
    PRIVATE
        spdlog::spdlog
        "${PLATFORM_LIB}"
)