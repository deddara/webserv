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
#include "VirtServer.hpp"

class Server{

private:
	std::vector<Client*> client_session;
	std::vector<VirtServer> & virt_serv;
	fd_set readset, writeset;
	int				max_fd;
	int				is_closed;
	size_t			serversNum;


public:
	typedef std::map<std::string, std::vector<std::string> >    map_type;
	Server(std::vector<VirtServer> & vec) : max_fd(0), is_closed(0), virt_serv(vec) {
		serversNum = virt_serv.size();
	}
	~Server(){};
	int launch();
	int setup();
	void set_prepare();
	void recv_msg(std::vector<Client*>::iterator it);
	void closeConnection(std::vector<Client*>::iterator it);
	int newSession();
	int clientSessionHandler();
	int createSocket(std::string const & host, int const port, int const & i);
	int postPutHandler(map_type const & data, std::vector<Client*>::iterator it, int &);
	void getLocation(std::vector<Client*>::iterator it, map_type const &);
	int nameCompare(std::string const &, std::vector<VirtServer>::iterator & it);
};


#endif