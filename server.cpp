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

	int listener, accepter;
	struct sockaddr_in servaddr;
	char	buff[1024];
	memset(buff, 0, sizeof(buff));

	if ((listener = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("listen");
		return (1);
	}

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

	for ( ; ; )
	{
		accepter = accept(listener, (struct sockaddr*) NULL, NULL);
		while ((n = recv(accepter, buff, sizeof(buff), 0)) > 0)
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
		if ((send(accepter, resp, strlen(resp), 0)) < 0)
		{
			perror("send");
			return 1;
		}
		close(accepter);
	}

}
