cmake_minimum_required(VERSION 3.13.0)


set(SOURCES round_service.cpp)

target_sources(streamlet PRIVATE ${SOURCES})
target_sources(tests PRIVATE ${SOURCES})

add_subdirectory(logging)
add_subdirectory(message)
add_subdirectory(node)
add_subdirectory(scenarios)
add_subdirectory(schedulers)
add_subdirectory(state)
