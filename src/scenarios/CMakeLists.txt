cmake_minimum_required(VERSION 3.13.0)


set(SOURCES
        runner.cpp
        config.cpp)

target_sources(streamlet PRIVATE ${SOURCES})
target_sources(tests PRIVATE ${SOURCES})
