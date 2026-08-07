cmake_minimum_required(VERSION 3.10)

project(RedisX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(RedisX
    src/main.cpp
    src/server.cpp
    src/database.cpp
    src/client.cpp
    src/parser.cpp
    src/command.cpp
)