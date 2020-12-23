cmake_minimum_required(VERSION 3.17)
project(webserv)

set(CMAKE_CXX_STANDARD 98)

include_directories(conf_parse)
include_directories(includes)
include_directories(request_parse)
include_directories(server_engine)

add_executable(webserv
        conf_parse/ConfParser.cpp
        conf_parse/ConfParser.hpp
        conf_parse/Location.cpp
        conf_parse/Location.hpp
        conf_parse/VirtServer.cpp
        conf_parse/VirtServer.hpp
        includes/includes.hpp
        request_parse/Request.cpp
        request_parse/Request.hpp
        server_engine/Server.cpp
        server_engine/Server.hpp
        server_engine/Response.cpp
        server_engine/Client.cpp
        utils/time.cpp
        utils/utils.cpp
        utils/ft_strjoin.cpp
        utils/ft_strnstr.cpp
        server_engine/Bytes.cpp
        main.cpp)
