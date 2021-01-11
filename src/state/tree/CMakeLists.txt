cmake_minimum_required(VERSION 3.13.0)


set(SOURCES
        tree.cpp
        vertex.cpp)

target_sources(streamlet PRIVATE ${SOURCES})
target_sources(tests PRIVATE ${SOURCES})
