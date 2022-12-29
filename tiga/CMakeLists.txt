cmake_minimum_required(VERSION 3.10)
project(tiga)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -O0 -ggdb -std=c++11 -Wall -Wno-deprecated -Werror -Wno-unused-function")

include_directories(.)

set(LIB_SRC
    lib/log.cc
    lib/util.cc
    )

add_library(tiga SHARED ${LIB_SRC})

add_executable(test tests/test.cc)
add_dependencies(test tiga)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)