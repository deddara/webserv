cmake_minimum_required(VERSION 3.17)
project(webserv)

set(CMAKE_CXX_STANDARD 98)

include_directories(includes)

add_executable(webserv
        ./server_engine/stol_server.cpp
        ./server_engine/response.cpp)
