#include "server.hpp"

int server_setup(){
	struct sockaddr_in addr;
	int listen_sock;

	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(3031);
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
	return (listen_sock);
}

void set_prepare(fd_set & readset, fd_set & writeset, const int & listen_sock, \
std::vector<int> & client_fd, int & max_fd, const Server & serv)
{
	FD_ZERO(&readset);
	FD_ZERO(&writeset);
	FD_SET(listen_sock, &readset);
	for (std::vector<int>::iterator it = client_fd.begin(); it != client_fd.end(); ++it){
		FD_SET(*it, &readset);
		if (serv.response.length() != 0){
			FD_SET(*it, &writeset);
		}
		if (*it > max_fd)
			max_fd = *it;
	}
}

int serv_recv_msg(std::vector<int> & client_fd, std::vector<int>::iterator & it){
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

int main()
{
	fd_set readset, writeset;
	int accept_sock, listen_sock, max_fd;
	std::vector<int> client_fd;
	Server	serv;

	if ((listen_sock = server_setup()) == 500)
		return (1);

	for (;;){
		max_fd = listen_sock;
		set_prepare(readset, writeset, listen_sock, client_fd, max_fd, serv);
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
				if (serv_recv_msg(client_fd, it))
					break;
				// тут должен быть парс реквеста
				response_prepare(serv);
			}
			if (FD_ISSET(*it, &writeset)){
				if ((send(*it, serv.response.c_str(), serv.response.length(), 0)) < 0)
				{
					perror("send");
					return 1;
				}
				serv.response.clear();
			}
		}
	}
	
}