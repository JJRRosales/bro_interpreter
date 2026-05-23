cmake_minimum_required(VERSION 3.16)
project(bro VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    src/*.cpp
)

add_executable(bro ${SOURCES})

target_include_directories(bro PRIVATE ${CMAKE_SOURCE_DIR})

# Friendly build output
message(STATUS "Building Bro Language interpreter v${PROJECT_VERSION} -- trust the process, bro.")
