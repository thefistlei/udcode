cmake_minimum_required(VERSION 3.10)
project(MyProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set UTF-8 flag only for MSVC
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /utf-8")
endif()

# 添加头文件搜索路径
include_directories(include) # 包含项目自身的头文件
include_directories(include/curl) # 包含 curl 的头文件

# 设置库文件搜索路径
link_directories(lib)

# 查找 curl 库
find_library(CURL_LIBRARY NAMES curl PATHS lib NO_DEFAULT_PATH)

if(CURL_LIBRARY)
    message(STATUS "Found curl library: ${CURL_LIBRARY}")
else()
    message(FATAL_ERROR "curl library not found!")
endif()

# 创建可执行目标
add_executable(MyProject main.cpp read-file.cpp http.cpp)

# 链接 curl 库
target_link_libraries(MyProject ${CURL_LIBRARY})
