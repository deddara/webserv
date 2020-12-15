#include "server.hpp"

void response_prepare(Server & serv)
{
	char buff[1000];
	bzero(buff, sizeof(buff));
	serv.body = "\0";

	int fd = open("../html_files/index.html", O_RDONLY);
	if (fd < 0){
		perror("open");
		return ;
	}
	while (read(fd, buff, sizeof(buff))) {
		serv.body += buff;
	}
	serv.response = "HTTP/1.1 200 OK\r\n"
					"Date: Mon, 27 Jul 2009 12:28:53 GMT\r\n"
					"Server: webserv0.0\r\n";
	serv.response += std::to_string(serv.body.length()); serv.response += "\r\n";
	serv.response += "Content-Type: text/html; charset=UTF-8\r\n"
					 "Connection: Close\r\n\r\n";
	serv.response += serv.body;
	serv.response += "\r\n\r\n";
}