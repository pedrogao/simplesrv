file(GLOB_RECURSE nebula_sources 
    ${PROJECT_SOURCE_DIR}/src/*.cpp
    )
add_library(nebula_shared SHARED ${nebula_sources})
