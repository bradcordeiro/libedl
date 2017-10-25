cmake_minimum_required(VERSION 3.8)
project(untitled4)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp src/edl.cpp src/event.cpp include/edl.h include/event.h src/timecode.cpp include/timecode.h)

add_executable(edlthing ${SOURCE_FILES})