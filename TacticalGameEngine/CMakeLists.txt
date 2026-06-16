cmake_minimum_required(VERSION 3.20)
project(TacticalGameEngine CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
    stduuid
    GIT_REPOSITORY https://github.com/mariusbancila/stduuid.git
    GIT_TAG        v1.2.3  
)

FetchContent_MakeAvailable(stduuid)

add_subdirectory(src/core)

add_executable(TacticalGameEngine 
    "TacticalGameEngine.cpp" 
    "TacticalGameEngine.h"
)

target_link_libraries(TacticalGameEngine PRIVATE engine_core)