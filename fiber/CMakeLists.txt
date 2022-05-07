cmake_minimum_required(VERSION 3.10)

set(CMAKE_C_STANDARD 11)

project(fiber C ASM)

enable_language(C ASM)

set (SOURCE
    fiber.c
)

# OS
if (CMAKE_SYSTEM_NAME MATCHES "Darwin")
    message(STATUS "Platform: macOS")
    set(SOURCE ${SOURCE} fiber_amd64_darwin.s)
elseif (CMAKE_SYSTEM_NAME MATCHES "Linux")
    message(STATUS "Platform: Linux")
    set(SOURCE ${SOURCE} fiber_amd64_linux.s)
endif()

# set_property(SOURCE fiber_amd64.s APPEND PROPERTY COMPILE_OPTIONS "-x" "assembler-with-cpp")
# set_source_files_properties(fiber_amd64.s PROPERTIES COMPILE_FLAGS "-x assembler-with-c")

add_library(fiber_shared SHARED  ${SOURCE})
# add_executable(hello foo.s bar.c)