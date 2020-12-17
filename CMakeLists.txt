cmake_minimum_required(VERSION 3.17)
project(webserv)

set(CMAKE_CXX_STANDARD 98)

include_directories(includes server_engine)

add_executable(webserv
        ./server_engine/Server.cpp
        utils/time.cpp
        utils/utils.cpp
        main.cpp)

#add_executable(webserv
#        ./server_engine/time.cpp
#        )