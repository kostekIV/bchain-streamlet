cmake_minimum_required(VERSION 3.13.0)


set(SOURCES
        node.cpp
        base_node.cpp
        dummy_node.cpp
        honest_node.cpp
        dishonest_node.cpp)

target_sources(streamlet PRIVATE ${SOURCES})
target_sources(tests PRIVATE ${SOURCES})
