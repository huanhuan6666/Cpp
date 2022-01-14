cmake_minimum_required(VERSION 3.21)
project(socket_C__)

set(CMAKE_CXX_STANDARD 11)

add_executable(socket_C__ test.cpp CSocketProtocol.h CEncDesProtocol.h ciscoEncdes.cpp ciscoEncdes.h CEncDesProtocol.cpp)
