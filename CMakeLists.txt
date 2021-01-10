cmake_minimum_required(VERSION 3.10)

project(streamlet)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SOURCES
        ${PROJECT_SOURCE_DIR}/src/dummy_node.cpp
        ${PROJECT_SOURCE_DIR}/src/message.cpp
        ${PROJECT_SOURCE_DIR}/src/streamlet.cpp
        ${PROJECT_SOURCE_DIR}/src/node/block.cpp
        ${PROJECT_SOURCE_DIR}/src/node/dag/dag.cpp
        ${PROJECT_SOURCE_DIR}/src/node/dag/vertex.cpp)

add_executable(streamlet main.cpp)
target_include_directories(streamlet PRIVATE include)
target_sources(streamlet PRIVATE ${SOURCES})

add_library(Catch INTERFACE)
target_include_directories(Catch INTERFACE tests)


set(TEST_SOURCES
        ${PROJECT_SOURCE_DIR}/src/streamlet.cpp
        ${PROJECT_SOURCE_DIR}/src/node/block.cpp
        ${PROJECT_SOURCE_DIR}/src/node/dag/dag.cpp
        ${PROJECT_SOURCE_DIR}/src/node/dag/vertex.cpp
        tests/main.cpp
        tests/example.cpp
        )

add_executable(tests ${TEST_SOURCES})
target_link_libraries(tests PUBLIC Catch)
target_include_directories(tests PRIVATE ${PROJECT_SOURCE_DIR}/tests)
target_include_directories(tests PRIVATE ${PROJECT_SOURCE_DIR}/include)