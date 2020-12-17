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

private:
	std::string response;
	std::string body;
	std::string date;
	std::vector<int> client_fd;
	fd_set readset, writeset;
	int accept_sock, listen_sock, max_fd;

public:
	Server(){};
	~Server(){};
	int launch();
	int setup();
	void set_prepare();
	int recv_msg(std::vector<int>::iterator it);
	void response_prepare();

};


#endif