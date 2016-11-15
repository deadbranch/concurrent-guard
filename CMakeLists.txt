cmake_minimum_required(VERSION 3.6)
project(sw_shared_ptr)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -O0")

set(SOURCE_FILES main.cpp cg_shared_ptr.h concurrent_guard.h)
add_executable(sw_shared_ptr ${SOURCE_FILES})