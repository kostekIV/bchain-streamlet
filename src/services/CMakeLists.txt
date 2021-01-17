cmake_minimum_required(VERSION 3.13.0)


set(SOURCES
        abstract_service.cpp
        repeat_service.cpp
        synchronizer.cpp)

target_sources(streamlet PRIVATE ${SOURCES})
target_sources(tests PRIVATE ${SOURCES})
