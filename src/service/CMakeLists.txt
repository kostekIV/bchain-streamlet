cmake_minimum_required(VERSION 3.13.0)


set(SOURCES
        round_service.cpp
        twice_service.cpp)

target_sources(streamlet PRIVATE ${SOURCES})
target_sources(tests PRIVATE ${SOURCES})
