#ifndef SERVER_H
# define SERVER_H
#include <iostream>
#include "vector"
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/select.h>
#include "zconf.h"
#include "unistd.h"

class Server{
public:
	std::string response;
	std::string body;
	std::string date;


};

void response_prepare(Server & serv);

#endif