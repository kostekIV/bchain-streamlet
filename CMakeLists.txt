cmake_minimum_required(VERSION 3.13.0)

project(streamlet)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


add_executable(streamlet main.cpp)
target_include_directories(streamlet PRIVATE include)

add_library(Catch INTERFACE)
target_include_directories(Catch INTERFACE tests)


set(TEST_SOURCES
        tests/main.cpp
        tests/scheduler-test.cpp
        tests/tree.cpp)

add_executable(tests ${TEST_SOURCES})
target_link_libraries(tests PUBLIC Catch)
target_include_directories(tests PRIVATE ${PROJECT_SOURCE_DIR}/tests)
target_include_directories(tests PRIVATE ${PROJECT_SOURCE_DIR}/include)

add_subdirectory(${PROJECT_SOURCE_DIR}/src)
