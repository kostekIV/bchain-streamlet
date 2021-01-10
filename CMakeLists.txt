cmake_minimum_required(VERSION 3.10)

project(streamlet)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SOURCES
    ${PROJECT_SOURCE_DIR}/src/streamlet.cpp
)

add_executable(streamlet main.cpp)
target_include_directories(streamlet PRIVATE include)
target_sources(streamlet PRIVATE ${SOURCES})
