#include <iostream>
#include "sstream"
#include "fstream"
#include "vector"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/select.h>
#include "zconf.h"

class Server{
	std::string response;

};

int main()
{
	fd_set readset, writeset;
	int listen_sock, accept_sock;
	struct sockaddr_in addr;
	std::vector<int> client_fd;
	Server	serv;

	addr.sin_family = AF_INET;
	addr.sin_port = htons(3030);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	if ((listen_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		perror("listen_sock");
		return (1);
	}
	fcntl(listen_sock, F_SETFL, O_NONBLOCK);

	if ((bind(listen_sock, (struct sockaddr*)& addr, sizeof(addr))) < 0)
	{
		perror("bind");
		return (1);
	}

	if ((listen(listen_sock, 10)) < 0){
		perror("listen_sock");
		return (1);
	}
	int max_fd;
	for (;;){
		max_fd = listen_sock;
		FD_ZERO(&readset);
		FD_ZERO(&writeset);
		FD_SET(listen_sock, &readset);
		for (std::vector<int>::iterator it = client_fd.begin(); it != client_fd.end(), ++it){
			FD_SET(*it, &readset);
			if (serv.response.length() != 0){
				FD_SET(*it, &writeset);
			}
			if (*it > max_fd)
				max_fd = *it;
		}
		select(max_fd, &readset, &writeset, NULL, NULL);
		if (FD_SET())
	}
	
}