#include "Server.hpp"
#include "includes.hpp"
#include "Request.hpp"

void Server::response_prepare()
{
	char buff[2048];
	bzero(buff, sizeof(buff));
	body.clear();
	date.clear();
	date = my_localtime();
	int fd = open("../html_files/index.html", O_RDONLY);
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

int Server::setup(std::string const & host, int const port){
	struct sockaddr_in addr;
	int listen_sock;

	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = ft_htons(port);
	addr.sin_addr.s_addr = inet_addr(host.c_str());

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
	server_socks.push_back(listen_sock);
	return (0);
}

void Server::set_prepare()
{
	FD_ZERO(&readset);
	FD_ZERO(&writeset);
	for (std::vector<int>::iterator it = server_socks.begin(); it != server_socks.end(); ++it)
		FD_SET(*it, &readset);
	for (std::vector<Client*>::iterator it = client_session.begin(); it !=  client_session.end(); ++it){
		FD_SET((*it)->getFd(), &readset);
		if (this->response.length() != 0){
			FD_SET((*it)->getFd(), &writeset);
		}
		if ((*it)->getFd() > max_fd)
			max_fd = (*it)->getFd();
	}
}

void Server::recv_msg(std::vector<Client*>::iterator it){
	int n;
	char buff[2048];
	bzero(&buff, 2048);
	n = recv((*it)->getFd(), buff, sizeof(buff), MSG_TRUNC);
	if (n <= 0)
	{
		(*it)->setStatus(3);
		return;
	}
	(*it)->buffAppend(buff);
	if ((*it)->getBuff().rfind("\r\n\r\n") != std::string::npos)
		(*it)->setStatus(1);
}

void Server::response_prepare_2(){
	char buff[2048];
	bzero(buff, sizeof(buff));
	body.clear();
	date.clear();
	date = my_localtime();
	int fd = open("../html_files/50x.html", O_RDONLY);
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

void Server::closeConnection(std::vector<Client*>::iterator it){
	close((*it)->getFd());
	client_session.erase(it);
}

int Server::launch() {
	for (;;){
		max_fd = 0;
		for (std::vector<int>::iterator it = server_socks.begin(); it != server_socks.end(); ++it) {
			if (*it > max_fd)
				max_fd = *it;
		}
		set_prepare();
		if (select(max_fd + 1, &readset, &writeset, NULL, NULL) < 0)
			return (1);
		for (std::vector<int>::iterator it = server_socks.begin(); it != server_socks.end(); ++it) {
			if (FD_ISSET(*it, &readset)) {
				int accept_sock;

				if ((accept_sock = accept(*it, NULL, NULL)) < 0) {
					perror("accept");
					return (1);
				}
				fcntl(accept_sock, F_SETFL, O_NONBLOCK);
				client_session.push_back(new Client(accept_sock));
			}
		}
		for (std::vector<Client*>::iterator it = client_session.begin(); it !=  client_session.end(); ++it) {
			if (FD_ISSET((*it)->getFd(), &readset))
			{
				switch ((*it)->getStatus())
				{
					case rdy_recv:
						recv_msg(it);
					case rdy_parse:
						(*it)->getRequest()->req_init(((*it)->getBuff()));
						if ((*it)->getRequest()->error()) {
							response_prepare_2();
						}
						else
							response_prepare();
						(*it)->setStatus(2);
						break;
					case finish:
						closeConnection(it);
						is_closed = 1;
						break;
				}

				if (is_closed)
				{
					is_closed = 0;
					break;
				}
//				if (recv_msg(it))
//					break;
//				Request req(static_cast<std::string>(buff));
//				if (req.error())
//				{
//					response_prepare_2();
//				}
//				else
//					response_prepare();
			}
			if (FD_ISSET((*it)->getFd(), &writeset)){
				if ((send((*it)->getFd(), response.c_str(), response.length(), 0)) < 0)
				{
					perror("send");
					return 1;
				}
				response.clear();
			}
		}
	}
}
