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

int sender_recv(int fd)
{
	int n;
	char buff[1024];
	memset(buff, 0, sizeof(buff));

	while ((n = recv(fd, buff, sizeof(buff), 0)) > 0)
	{
		std::cout << buff;
		if (buff[n-1] == '\n')
			break;
		memset(buff, 0, sizeof(buff));
	}
	if (n < 0)
	{
		perror("n");
		return (1);
	}
	char *resp = "HTTP/1.1 200 OK\r\n";
	if ((send(fd, resp, strlen(resp), 0)) < 0)
	{
		perror("send");
		return 1;
	}
	close(fd);
	return (0);
}

int main()
{
	lst_adress	adr;
	get_ip_from_file(adr);
	fd_set cur_socks, rdy_socks;
	int listener, accepter;
	struct sockaddr_in servaddr;
	char	buff[1024];
	memset(buff, 0, sizeof(buff));

	if ((listener = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("listen");
		return (1);
	}
	fcntl(listener, F_SETFL, O_NONBLOCK);

	FD_ZERO(&cur_socks);
	FD_SET(listener, &cur_socks);


	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(adr.port);
	servaddr.sin_addr.s_addr = inet_addr(adr.ip);

	if (bind(listener, (struct sockaddr *)& servaddr, sizeof(servaddr)) < 0)
	{
		perror("bind");
		return (1);
	}
	if (listen(listener, 10) < 0)
	{
		perror("listen");
		return (1);
	}
	int n;
	int max_fd = listener;
	for ( ; ; )
	{
		rdy_socks = cur_socks;

		if (select(max_fd + 1, &rdy_socks, NULL, NULL, NULL) < 0){
			perror("select");
			return (1);
		}

		for (int i = 0; i < max_fd + 1; i++)
		{
			if (FD_ISSET(i, &rdy_socks))
			{
				if (i == listener)
				{
					fd_set fd_;
					struct sockaddr_storage ss;
					socklen_t slen = sizeof(ss);

					if ((accepter = accept(listener, (struct sockaddr*)&ss, &slen)) < 0)
					{
						perror("accept");
						return (1);
					}
					fcntl(accepter, F_SETFL, O_NONBLOCK);
					FD_ZERO(&fd_);
					FD_SET(accepter, &fd_);
					if ((select(accepter + 1, &fd_, &fd_, 0, 0)) < 0)
					{
						perror("select");
						return (1);
					}
					FD_SET(accepter, &cur_socks);
					if (accepter > max_fd)
						max_fd = accepter;
				}
				else
				{
					sender_recv(i);
					FD_CLR(i, &cur_socks);
				}
			}
		}
	}

}
