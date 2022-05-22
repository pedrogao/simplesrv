cmake_minimum_required(VERSION 3.10)
project(co CXX C ASM)
enable_language(CXX C ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(./)

set (SOURCE
    Channel.cpp
    Coroutine.cpp
    EventLoop.cpp
    fiber.c
    Poller.cpp
    Socket.cpp
    util.cpp
)

# OS
if (CMAKE_SYSTEM_NAME MATCHES "Darwin")
    message(STATUS "Platform: macOS")
    set(SOURCE ${SOURCE} fiber_amd64_darwin.s)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DOS_MACOS")
elseif (CMAKE_SYSTEM_NAME MATCHES "Linux")
    message(STATUS "Platform: Linux")
    set(SOURCE ${SOURCE} fiber_amd64_linux.s)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DOS_LINUX")
endif()

add_library(co_shared SHARED ${SOURCE})

add_executable(co_srv server.cpp)
target_link_libraries(co_srv co_shared)