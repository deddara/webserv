#ifndef RESPONSE_HPP
# define RESPONSE_HPP
# include "iostream"
# include "includes.hpp"

class Response{
private:
	std::string response;
	std::string body;
	std::string date;
public:
	Response(){};
	~Response(){};

	void response_prepare(int num){
		int fd;
		char buff[2048];
		bzero(buff, sizeof(buff));
		body.clear();
		date.clear();
		date = my_localtime();
		if (num == 0)
			fd = open("../html_files/index.html", O_RDONLY);
		else
			fd = open("../html_files/50x.html", O_RDONLY);
		if (fd < 0){
			perror("open");
			return ;
		}
		while (read(fd, buff, sizeof(buff))) {
			body.append(buff);
		}
		response = "HTTP/1.1 200 OK\r\nDate: ";
		response.append(date);
		response.append("Server: webserv0.0\r\nContent-Length: ");
		response.append(std::to_string(body.length())); response.append("\r\n");
		response.append("Content-Type: text/html; charset=UTF-8\r\nConnection: Keep-Alive\r\n\r\n");
		response.append(body);
		response.append("\r\n\r\n");
		std::cout << response;
	}

	void clearStr(){
		response.clear();
	}

	std::string & getStr() {return response; }

};

#endif