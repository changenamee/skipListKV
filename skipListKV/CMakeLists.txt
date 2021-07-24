cmake_minimum_required(VERSION 3.19)
project(skipListKV)

set(CMAKE_CXX_STANDARD 14)

add_executable(skipListKV main.cpp Node.cpp Node.h skipList.cpp skipList.h)