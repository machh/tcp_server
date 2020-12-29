cmake_minimum_required(VERSION 2.8)
project(server)

add_compile_options(-std=c++11)
add_definitions("-ggdb3")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-Wall")
#设置工程源码根目录
set(SRC_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

aux_source_directory(. SRC_LIST)
#list(APPEND SRC_LIST ${READER}/reader.cpp)
include_directories(.)

file(GLOB SOURCE_FILES
        ${SRC_ROOT}/*.cpp
        ${SRC_ROOT}/*.h
        "*/*.h"
        "*/*/*.h"
        "*/*/*/*.h"
        )

add_executable(server ${SOURCE_FILES})
target_link_libraries(${CMAKE_PROJECT_NAME}
        -lpthread
        )