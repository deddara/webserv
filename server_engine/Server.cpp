#include "Server.hpp"
#include "includes.hpp"
#include "Request.hpp"

void Server::closeConnection(std::vector<Client*>::iterator it){
	close((*it)->getFd());
	client_session.erase(it);
}

int Server::createSocket(const std::string &host, const int port, int const & i) {
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
	virt_serv[i].setFd(listen_sock);
	return (0);
}

int Server::setup(){
	for (int i = 0; i < serversNum; ++i)
	{
		for (int j = 0; j < serversNum; ++j){
			if (virt_serv[i].getHost() == virt_serv[j].getHost() && virt_serv[i].getPort() == virt_serv[j].getPort() && j != i) {
				virt_serv[i].setFd(virt_serv[j].getFd());
				break;
			}
		}
		if (!virt_serv[i].getFd())
			this->createSocket(virt_serv[i].getHost(), virt_serv[i].getPort(), i);
	}
	return (0);
}

void Server::set_prepare()
{
	FD_ZERO(&readset);
	FD_ZERO(&writeset);
	for (std::vector<VirtServer>::iterator it = virt_serv.begin(); it != virt_serv.end(); ++it)
		FD_SET((*it).getFd(), &readset);
	for (std::vector<Client*>::iterator it = client_session.begin(); it !=  client_session.end(); ++it){
		FD_SET((*it)->getFd(), &readset);
		if ((*it)->getResponse() && strlen((*it)->getResponse()->getStr())){
			FD_SET((*it)->getFd(), &writeset);
		}
		if ((*it)->getFd() > max_fd)
			max_fd = (*it)->getFd();
	}
}

void Server::postPutHandler(map_type const & data)
{

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

//	(*it)->getRequest()->req_init(((*it)->getBuff()));
//	map_type const & data = (*it)->getRequest()->getData();
//	map_type::const_iterator data_it = data.find("head");

	if ((*it)->getBuff().rfind("\r\n\r\n") != std::string::npos)
		(*it)->setStatus(1);
}

int Server::newSession() {
	for (std::vector<VirtServer>::iterator it = virt_serv.begin(); it != virt_serv.end(); ++it) {
		if (FD_ISSET((*it).getFd(), &readset)) {
			int accept_sock;
			struct sockaddr_storage ss;
			socklen_t slen = sizeof(ss);
			if ((accept_sock = accept((*it).getFd(), (struct sockaddr *) &ss, &slen)) < 0) {
				perror("accept");
				return (1);
			}
			fcntl(accept_sock, F_SETFL, O_NONBLOCK);
			client_session.push_back(new Client(accept_sock, (*it).getHost(), (*it).getPort()));
		}
	}
	return (0);
}

void Server::getLocation(std::vector<Client *>::iterator it, const map_type &data) {
	std::string host;
	map_type::const_iterator map_it = data.find("host");
	host = map_it->second[0];
	for (std::vector<VirtServer>::iterator serv_it = virt_serv.begin(); serv_it != virt_serv.end(); ++serv_it) {
		if ((*it)->getServPort() == (*serv_it).getPort() && (*it)->getServHost() == (*serv_it).getHost() && \
			(host == (*serv_it).getHost())){
			(*it)->getResponse()->setLocation((*serv_it).getLocation());
			return;
		} // ||servername
	}
	(*it)->getResponse()->setErrcode(400);
}

int Server::clientSessionHandler() {
	for (std::vector<Client*>::iterator it = client_session.begin(); it !=  client_session.end(); ++it) {
		if (FD_ISSET((*it)->getFd(), &readset))
		{
			map_type const & data = (*it)->getRequest()->getData();

			switch ((*it)->getStatus()) {
				case rdy_recv:
					recv_msg(it);
					if ((*it)->getStatus() == 0)
						break ;
				case rdy_parse:
					if ((*it)->getStatus() != 3)
					{
						(*it)->getRequest()->req_init(((*it)->getBuff()));
						if ((*it)->getRequest()->error())
							(*it)->getResponse()->setErrcode(400);
						else
							this->getLocation(it, data);
						if ((*it)->getResponse()->response_prepare((*it)->getStatus(), &data))
							return 1;
						(*it)->clearBuff();
						break;
					}
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
		}
		if (FD_ISSET((*it)->getFd(), &writeset)){
			if ((send((*it)->getFd(), (*it)->getResponse()->getStr(),  strlen((*it)->getResponse()->getStr()), 0)) < 0)
			{
				perror("send");
				return 1;
			}
			(*it)->getResponse()->clearStr();
			if ((*it)->getStatus() == 3) {
				this->closeConnection(it);
				break;
			}
		}
	}
	return (0);
}

int Server::launch() {
	//главный цикл жизни сервера (Желательно потом разбить на еще доп методы, это я сделаю сам)
	for (;;){
		max_fd = virt_serv.back().getFd();
		this->set_prepare();
		if (select(max_fd + 1, &readset, &writeset, NULL, NULL) < 0)
			return (1);
		if (this->newSession())
			return (1);
		if (this->clientSessionHandler())
			return (1);
	}
}
