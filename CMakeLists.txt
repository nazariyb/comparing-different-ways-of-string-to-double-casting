cmake_minimum_required(VERSION 3.10)
project(lab1 CXX)

set(CMAKE_CXX_STANDARD 14)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/dependencies)

add_executable(lab1
        main.cpp
        )
