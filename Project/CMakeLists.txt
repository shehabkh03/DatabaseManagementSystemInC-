cmake_minimum_required(VERSION 3.27)
project(PROJECT)

set(CMAKE_CXX_STANDARD 20)
include(FetchContent)

FetchContent_Declare(
        fmt
        GIT_REPOSITORY  https://github.com/fmtlib/fmt
        GIT_TAG         10.2.1
)

FetchContent_MakeAvailable(fmt)

add_executable(PROJECT main.cpp
        Classes_And_Functions.cpp
        Classes_And_Functions.h)
target_link_libraries(PROJECT fmt)
