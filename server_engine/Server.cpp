/* *************************************************************************************/
/*                                                                                     */
/*                                                             |\---/|                 */
/*  Server.cpp                                                 | o_o |                 */
/*                                                             ‾‾‾‾‾‾‾                 */
/*  By: deddara <deddara@student-21.school.ru>                 ┌┬┐┌─┐┌┬┐┌┬┐┌─┐┬─┐┌─┐   */
/*                                                             _││├┤  ││ ││├─┤├┬┘├─┤   */
/*  created: 12/24/20 20:36:43 by deddara                      ─┴┘└─┘─┴┘─┴┘┴ ┴┴└─┴ ┴   */
/*  updated: 12/26/20 19:35:34 by deddara                      +-++-++-++-++-++-++-+   */
/*                                                             |)[-|)|)/-\|2/-\        */
/*                                                                                     */
/* **********************************************************²**************************/
#include "Server.hpp"
#include "includes.hpp"
#include "Request.hpp"
#include "sys/time.h"
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
	if ((listen(listen_sock, 2048)) < 0){
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

int Server::set_prepare()
{
	FD_ZERO(&readset);
	FD_ZERO(&writeset);
	for (std::vector<VirtServer>::iterator it = virt_serv.begin(); it != virt_serv.end(); ++it)
		FD_SET((*it).getFd(), &readset);
	for (std::vector<Client*>::iterator it = client_session.begin(); it !=  client_session.end(); ++it){
		gettimeofday(&t, NULL);
		if (t.tv_sec - (*it)->getLastMsg().tv_sec > 500)
		{
			closeConnection(it);
			return (1);
		}
		FD_SET((*it)->getFd(), &readset);
		if ((*it)->getResponse() && strlen((*it)->getResponse()->getStr())){
			FD_SET((*it)->getFd(), &writeset);
		}
		if ((*it)->getFd() > max_fd)
			max_fd = (*it)->getFd();
	}
	return (0);
}

int Server::postPutHandler(map_type const & data, std::vector<Client*>::iterator it)
{
	map_type::const_iterator map_it;
	map_it = data.find("content-length");
	unsigned long content_length = atoi(map_it->second[0].c_str());
	unsigned long should_read_len = content_length + (*it)->getRequest()->get_body_pos();


	if ((*it)->getBytes().getBytes() == should_read_len)  //общее колво сколько должны считать
		(*it)->setStatus(1);
	else if ((*it)->getBytes().getBytes() > should_read_len)
	{
		if ((*it)->buffCut(should_read_len))
			(*it)->getResponse()->setErrcode(500);
		(*it)->setStatus(1);
	}
	return (0);
}

int Server::error_headers(Request const &req) {
	std::string methods[4] = {"GET", "HEAD", "POST", "PUT"};
	std::string method;
	bool is_allowed_method = false;
	if (req.is_valid_value("head")) {
		method = req.find("head").front();
		for (int i = 0; i <= methods->size(); ++i)
			if (method == methods[i])
				is_allowed_method = true;
		if (!is_allowed_method)
			return 405;
	}
	map_type const & map_data = req.getData();
	map_type::const_iterator cont_it = map_data.find("content-length");
	map_type::const_iterator chunk_it = map_data.find("transfer-encoding");
	if ((method == "POST" || method == "PUT")) {
		if (cont_it == map_data.end() && chunk_it == map_data.end())
			return 411;
	}
	return 0;
}

void Server::chunkHandler(std::vector<Client*>::iterator & it) {

	Chunk & chunk = (*it)->getChunk();
	Bytes & bytes = (*it)->getBytes();
	const int & body_pos = (*it)->getRequest()->get_body_pos();
	const char * read_buff = (*it)->getBuff();
	read_buff += body_pos;

	while (bytes.getBytes() > body_pos + chunk.getLenSum()){
		if (chunk.getCount() % 2)
		{
			(*it)->bodyAppend(read_buff + chunk.getLenSum(), chunk.getLen());
			chunk.setBuffSum(chunk.getBuffSum() + chunk.getLen());
			chunk.setCount(chunk.getCount() + 1);
			chunk.setLenSum(chunk.getLenSum() + chunk.getLen() + 2);
		}
		else
		{
			int res = chunk.takeNum(read_buff + chunk.getLenSum(), bytes.getBytes() - body_pos);
			if (res == -1)
			{
				(*it)->getResponse()->setErrcode(400);
				(*it)->setStatus(1);
				return;
			}
			else if (!res)
			{
				if (chunk.getLen() == 0)
				{
					(*it)->getResponse()->setErrcode(200);
					(*it)->setStatus(1);
					chunk.setZero();
					return;
				}
				chunk.setLenSum(chunk.getLenSum() + chunk.getHexLen());
				chunk.setCount(chunk.getCount() + 1);
			}
		}
	}
}


void Server::recv_msg(std::vector<Client*>::iterator it){
	int n;
	map_type::const_iterator map_it;
	char buff[300];
	bzero(&buff, 300);
	int err = 400;

	(*it)->setLastMsg();

	if((n = recv((*it)->getFd(), buff, sizeof(buff), MSG_TRUNC)) <= 0)
	{
		(*it)->setStatus(3);
		return;
	}

	if((*it)->buffAppend(buff, n)) {
		(*it)->getResponse()->setErrcode(500);
	}
	(*it)->getBytes().bytesCount(n);
	if (ft_strnstr((*it)->getBuff(), "\r\n\r\n", (*it)->getBytes().getBytes())) {
		//parse and chech parse errror codes
		(*it)->getRequest()->req_init(((*it)->getBuff()));
		if ((*it)->getRequest()->error() || (err = error_headers(*(*it)->getRequest()))) {
			(*it)->setStatus(1);
			(*it)->getResponse()->setErrcode(err);
			return;
		}
		map_type const & map_data = (*it)->getRequest()->getData();
		map_it = map_data.find("head");
		if (map_it->second[0] == "POST" || map_it->second[0] == "PUT") {
			map_it = map_data.find("transfer-encoding");
			if (map_it != map_data.end() && map_it->second[0] == "chunked")
				chunkHandler(it);
			else
				postPutHandler((*it)->getRequest()->getData(), it);

		}
		else
			(*it)->setStatus(1);

	}
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

int Server::nameCompare(const std::string &name, std::vector<VirtServer>::iterator &  it) {
	for(std::vector<std::string>::const_iterator name_it = it->getServerName().begin(); name_it != it->getServerName().end(); ++name_it)
	{
		if (name == (*name_it))
			return (0);
	}
	return (1);
}

void Server::getLocation(std::vector<Client *>::iterator it, const map_type &data) {
	std::string host;
	map_type::const_iterator map_it = data.find("host");
	host = map_it->second[0];
	for (std::vector<VirtServer>::iterator serv_it = virt_serv.begin(); serv_it != virt_serv.end(); ++serv_it) {
		if ((*it)->getServPort() == (*serv_it).getPort() && (*it)->getServHost() == (*serv_it).getHost() && \
			(host == (*serv_it).getHost() || !(nameCompare(host, serv_it)))){
			(*it)->getResponse()->setLocation((*serv_it).getLocation());
			return;
		}
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
					std::cout << (*it)->getBody();
					if ((*it)->getStatus() != 3)
					{
						(*it)->clearBuff();
						if (!(*it)->getRequest()->error())
							this->getLocation(it, data);
						if ((*it)->getResponse()->response_prepare((*it)->getStatus(), &data))
							return 1;
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
			(*it)->setStatus(0);
		}
	}
	return (0);
}

int Server::launch() {
	//главный цикл жизни сервера (Желательно потом разбить на еще доп методы, это я сделаю сам)
	for (;;){
		int select_res;
		max_fd = virt_serv.back().getFd();
		if (this->set_prepare())
			continue;
		t.tv_sec = 500;
		t.tv_usec = 0;

		if ((select_res = select(max_fd + 1, &readset, &writeset, NULL, &t)) < 0) {
			return (1);
		}
		if (select_res == 0)
			continue;
		if (this->newSession())
			return (1);
		if (this->clientSessionHandler())
			return (1);
	}
}
