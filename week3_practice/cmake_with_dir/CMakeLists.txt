cmake_minimum_required(VERSION 2.8.9)
project(cmake_with_dir)

include_directories(include)

#set(SOURCES src/main.cpp src/add.cpp)

file(GLOB SOURCES "src/*.cpp")

add_executable(mainTest ${SOURCES})
