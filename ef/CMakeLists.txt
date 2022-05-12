cmake_minimum_required(VERSION 3.10)
set(CMAKE_C_STANDARD 11)
project(fiber CXX C ASM)
enable_language(CXX C ASM)

set (SOURCE
    fiber.c
    coroutine.c
    framework.c
)

include_directories(include)

# OS
if (CMAKE_SYSTEM_NAME MATCHES "Darwin")
    message(STATUS "Platform: macOS")
    set(SOURCE ${SOURCE} kqueue.c)
    set(SOURCE ${SOURCE} fiber_amd64_darwin.s)
elseif (CMAKE_SYSTEM_NAME MATCHES "Linux")
    message(STATUS "Platform: Linux")
    set(SOURCE ${SOURCE} epoll.c)
    set(SOURCE ${SOURCE} fiber_amd64_linux.s)
endif()

add_library(fiber_shared SHARED ${SOURCE})

add_executable(ef_srv main.c)
target_link_libraries(ef_srv fiber_shared)