cmake_minimum_required(VERSION 3.10)
project(udcode)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set UTF-8 flag only for MSVC
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /utf-8")
endif()

# add the curl library
include_directories(third-party) # include the curl header files
include_directories(third-party/include) # include the curl header files
include_directories(third-party/include/curl) # include the curl header files
include_directories(third-party/nlohmannjson) # include the json header files

# set the path to the curl library
link_directories(third-party/lib)

# find the curl library
find_library(CURL_LIBRARY NAMES curl PATHS third-party/lib NO_DEFAULT_PATH)

if(CURL_LIBRARY)
    message(STATUS "Found curl library: ${CURL_LIBRARY}")
else()
    message(FATAL_ERROR "curl library not found!")
endif()

add_executable(udcode src/main.cpp src/read-file.cpp src/curl-http.cpp src/util.cpp src/settings.cpp)

target_link_libraries(udcode ${CURL_LIBRARY})

# copy the dll to the build directory
if(WIN32)
    if(EXISTS "${CMAKE_SOURCE_DIR}/third-party/lib/libcurl-d.dll")
        add_custom_command(TARGET udcode POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_SOURCE_DIR}/third-party/lib/zlibd1.dll"
            $<TARGET_FILE_DIR:udcode>/zlibd1.dll
        )
        add_custom_command(TARGET udcode POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_SOURCE_DIR}/third-party/lib/libcurl-d.dll"
            $<TARGET_FILE_DIR:udcode>/libcurl-d.dll
        )
        message(STATUS "dll copied to the build directory.")
    else()
        message(WARNING "dll not found, skipping copy.")
    endif()

    add_custom_command(TARGET udcode POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_SOURCE_DIR}/config.json"
    $<TARGET_FILE_DIR:udcode>/config.json
    )
endif()
