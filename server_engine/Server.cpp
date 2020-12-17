#include "Server.hpp"
#include "includes.hpp"

void Server::response_prepare()
{
	char buff[1000];
	bzero(buff, sizeof(buff));
	body = "\0";
	date = my_localtime();
	int fd = open("../html_files/index.html", O_RDONLY);
	if (fd < 0){
		perror("open");
		return ;
	}
	while (read(fd, buff, sizeof(buff))) {
		body += buff;
	}
	response = "HTTP/1.1 200 OK\r\nDate: ";
	response.append(date);
	response.append("Server: webserv0.0\r\nContent-Length: ");
	response.append(std::to_string(body.length())); response.append("\r\n");
	response += "Content-Type: text/html; charset=UTF-8\r\n"
					 "Connection: Keep-Alive\r\n\r\n";
	response += body;
	response += "\r\n\r\n";
	std::cout << response;
}

int Server::setup(){
	struct sockaddr_in addr;

	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = ft_htons(3032);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	if ((listen_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		perror("listen_sock");
		return (500);
	}
	fcntl(listen_sock, F_SETFL, O_NONBLOCK);
	int opt = 1;
	setsockopt(listen_sock, SOL_SOCKET,  SO_REUSEADDR, &opt, sizeof(opt));
	if ((bind(listen_sock, (struct sockaddr*)& addr, sizeof(addr))) < 0)
	{
		perror("bind");
		return (500);
	}
	if ((listen(listen_sock, 10)) < 0){
		perror("listen_sock");
		return (500);
	}
	return (0);
}

void Server::set_prepare()
{
	FD_ZERO(&readset);
	FD_ZERO(&writeset);
	FD_SET(listen_sock, &readset);
	for (std::vector<int>::iterator it = client_fd.begin(); it != client_fd.end(); ++it){
		FD_SET(*it, &readset);
		if (this->response.length() != 0){
			FD_SET(*it, &writeset);
		}
		if (*it > max_fd)
			max_fd = *it;
	}
}

int Server::recv_msg(std::vector<int>::iterator it){
	int n;
	char buff[1024];
	bzero(&buff, 1024);

	while ((n = recv(*it, buff, sizeof(buff), 0)) > 0)
	{
		std::cout << buff;
		if (buff[n - 1] == '\n')
			break;
	}
	if (n <= 0)
	{
		close(*it);
		client_fd.erase(it);
		return (1);
	}
	return (0);
}

int Server::launch() {
	for (;;){
		max_fd = listen_sock;
		set_prepare();
		if (select(max_fd + 1, &readset, &writeset, NULL, NULL) < 0)
			return (1);
		if (FD_ISSET(listen_sock, &readset))
		{
			struct sockaddr_storage ss;
			socklen_t slen = sizeof(ss);
			if ((accept_sock = accept(listen_sock, (struct sockaddr*) & ss, &slen)) < 0)
			{
				perror("accept");
				return(1);
			}
			fcntl(accept_sock, F_SETFL, O_NONBLOCK);
			client_fd.push_back(accept_sock);
		}
		for (std::vector<int>::iterator it = client_fd.begin(); it != client_fd.end(); ++it) {
			if (FD_ISSET(*it, &readset))
			{
				if (recv_msg(it))
					break;
				// тут должен быть парс реквеста
				response_prepare();
			}
			if (FD_ISSET(*it, &writeset)){
				if ((send(*it, response.c_str(), response.length(), 0)) < 0)
				{
					perror("send");
					return 1;
				}
				response.clear();
			}
		}
	}
}
