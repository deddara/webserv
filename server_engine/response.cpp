#include "server.hpp"

void response_prepare(Server & serv)
{
	char buff[1000];
	bzero(buff, sizeof(buff));
	serv.body = "\0";
	serv.date = my_localtime();
	int fd = open("../html_files/index.html", O_RDONLY);
	if (fd < 0){
		perror("open");
		return ;
	}
	while (read(fd, buff, sizeof(buff))) {
		serv.body += buff;
	}
	serv.response = "HTTP/1.1 200 OK\r\nDate: ";
	serv.response.append(serv.date);
	serv.response.append("Server: webserv0.0\r\nContent-Length: ");
	serv.response += std::to_string(serv.body.length()); serv.response += "\r\n";
	serv.response += "Content-Type: text/html; charset=UTF-8\r\n"
					 "Connection: Keep-Alive\r\n\r\n";
	serv.response += serv.body;
	serv.response += "\r\n\r\n";
	std::cout << serv.response;
}