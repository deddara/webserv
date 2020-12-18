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
#include "Client.hpp"

class Server{

private:
	std::vector<Client*> client_session;
	std::vector<int> server_socks;
	fd_set readset, writeset;
	int max_fd;
	int is_closed;

public:
	Server() : max_fd(0), is_closed(0) { server_socks.reserve(100); };
	~Server(){};
	int launch();
	int setup(std::string const & host, int const port);
	void set_prepare();
	void recv_msg(std::vector<Client*>::iterator it);
	void closeConnection(std::vector<Client*>::iterator it);
	int newSession();
	int clientSessionHandler();

};


#endif