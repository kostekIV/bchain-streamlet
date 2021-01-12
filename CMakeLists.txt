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
        tests/tree.cpp
        tests/config-test.cpp
        tests/runner-test.cpp)

add_executable(tests ${TEST_SOURCES})
target_link_libraries(tests PUBLIC Catch)
target_include_directories(tests PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_include_directories(tests PRIVATE ${PROJECT_SOURCE_DIR}/tests)


include(FetchContent)

FetchContent_Declare(yaml-cpp
  GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
  GIT_TAG yaml-cpp-0.6.3)
FetchContent_GetProperties(yaml-cpp)
if(NOT yaml-cpp_POPULATED)
  message(STATUS "Fetching yaml-cpp...")
  FetchContent_Populate(yaml-cpp)
  add_subdirectory(${yaml-cpp_SOURCE_DIR} ${yaml-cpp_BINARY_DIR})
endif()


target_link_libraries(streamlet PUBLIC yaml-cpp)
target_link_libraries(tests PUBLIC yaml-cpp)

add_custom_command(TARGET tests PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                   ${CMAKE_SOURCE_DIR}/res/ $<TARGET_FILE_DIR:tests>)

add_subdirectory(${PROJECT_SOURCE_DIR}/src)
