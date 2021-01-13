cmake_minimum_required(VERSION 3.13.0)


set(SOURCES
        tree.cpp
        tree_renderer.cpp
        tree_union.cpp
        vertex.cpp)

target_sources(streamlet PRIVATE ${SOURCES})
target_sources(tests PRIVATE ${SOURCES})
