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
struct lst_adress{
	const char *ip;
	int 		port;
};

void 	get_ip_from_file(lst_adress & adr)
{
	std::string			file_data;
	std::stringstream	buffer;
	std::ifstream		ifs("conf.d");
	std::vector<std::string>	vec;

	if (!ifs.is_open())
		return;
	buffer << ifs.rdbuf(); //reading whole file
	file_data = buffer.str();	//converting to string

	std::string delimiter = ":";
	size_t pos = 0;
	std::string token;
	while ((pos = file_data.find(delimiter)) != std::string::npos) {
		token = file_data.substr(0, pos);
		vec.push_back(token);
		file_data.erase(0, pos + delimiter.length());
	}
	vec.push_back(file_data);
	adr.ip = vec.front().c_str();
	adr.port = std::stoi(vec.back());
}

int main()
{
	lst_adress	adr;
	get_ip_from_file(adr);

	adr.port = 1339;
	adr.ip = "127.0.0.1";
	fd_set readset, writeset, exset;

	int sockfd;
	struct sockaddr_in	my_addr;

	sockfd = socket(PF_INET, SOCK_STREAM, 0);
	fcntl(sockfd, F_SETFL, O_NONBLOCK);

	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(adr.port);
	my_addr.sin_addr.s_addr = inet_addr(adr.ip);
	memset(my_addr.sin_zero, 0, sizeof my_addr.sin_zero);
	if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(my_addr)) < 0){
		perror("bind");
		return 1;
	}
	if (listen(sockfd, 16)<0) {
		perror("listen");
		return 1;
	}
	FD_ZERO(&writeset);
	FD_ZERO(&exset);
	FD_ZERO(&readset);
	int yes = 1;
	setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int));
	int max_fd;
	while (1) {
		char buf[1024];
		bzero(buf, 1024);
		max_fd = sockfd;
		FD_ZERO(&readset);
		FD_ZERO(&writeset);
		FD_ZERO(&exset);

		FD_SET(sockfd, &readset);
		if (select(max_fd+1, &readset, &writeset, &exset, NULL) < 0) {
			perror("select");
			return 1;
		}

		if (FD_ISSET(sockfd, &readset))
		{
			struct sockaddr_storage ss;
			fd_set new_fd;

			socklen_t slen = sizeof(ss);
			int fd = accept(sockfd, (struct sockaddr*)&ss, &slen);
			fcntl(fd, F_SETFL, O_NONBLOCK);

//			FD_ZERO(&new_fd);
//			FD_SET(fd, &new_fd);
//			select(fd + 1, &new_fd, &new_fd, 0, 0);
			sleep(1);
			if (fd < 0) {
				perror("accept");
			}
			else
			{
				while (1) {
					ssize_t res = recv(fd, buf, sizeof(buf), 0);
					std::cout << res <<  " "<< buf << std::endl;
					if (res <= 0) {
						break;
					}
				}
			}
		}
	}
}
