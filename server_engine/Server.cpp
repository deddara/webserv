/* *************************************************************************************/
/*                                                                                     */
/*                                                             |\---/|                 */
/*  Server.cpp                                                 | o_o |                 */
/*                                                             ‾‾‾‾‾‾‾                 */
/*  By: deddara <deddara@student-21.school.ru>                 ┌┬┐┌─┐┌┬┐┌┬┐┌─┐┬─┐┌─┐   */
/*                                                             _││├┤  ││ ││├─┤├┬┘├─┤   */
/*  created: 12/24/20 20:36:43 by deddara                      ─┴┘└─┘─┴┘─┴┘┴ ┴┴└─┴ ┴   */
/*  updated: 12/26/20 20:44:29 by deddara                      +-++-++-++-++-++-++-+   */
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
	for (size_t i = 0; i < serversNum; ++i)
	{
		for (size_t j = 0; j < serversNum; ++j){
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
		if ((*it)->getResponse() && ((*it)->getResponse()->getResponseStruct().length)){
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
	char *body;
	map_it = data.find("content-length");
	unsigned long content_length = atoi(map_it->second[0].c_str());
	unsigned long should_read_len = content_length + (*it)->getRequest()->get_body_pos();


	if ((*it)->getBytes().getBytes() == should_read_len)  //общее колво сколько должны считать
	{
		if (!(body = (char*)malloc(sizeof(char) * content_length)))
			(*it)->getResponse()->setErrcode(500);
		else {
			ft_memcpy(body, (*it)->getBuff() + (*it)->getRequest()->get_body_pos(), content_length);
			(*it)->setBody(body);
			(*it)->setBodyLen(content_length);
		}
		(*it)->setStatus(1);
	}
	else if ((*it)->getBytes().getBytes() > should_read_len)
	{
		if ((*it)->buffCut(should_read_len))
			(*it)->getResponse()->setErrcode(500);
		if (!(body = (char*)malloc(sizeof(char) * content_length)))
			(*it)->getResponse()->setErrcode(500);
		else {
			ft_memcpy(body, (*it)->getBuff() + (*it)->getRequest()->get_body_pos(), content_length);
			(*it)->setBody(body);
			(*it)->setBodyLen(content_length);
		}
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
		for (size_t i = 0; i <= methods->size(); ++i)
			if (method == methods[i])
				is_allowed_method = true;
		if (!is_allowed_method)
			return 501;
	}
	map_type const & map_data = req.getData();
	map_type::const_iterator cont_it = map_data.find("content-length");
	map_type::const_iterator chunk_it = map_data.find("transfer-encoding");
	if ((method == "POST")) {
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

	while (bytes.getBytes() >
			(static_cast<unsigned long>(body_pos + chunk.getLenSum()) + chunk.getLen())){
		if (chunk.getCount() % 2)
		{
			if (chunk.getLen() == 0)
			{
				if (!ft_memcmp(read_buff + chunk.getLenSum(), "\r\n", 2))
				{
					(*it)->setBodyLen(chunk.getBuffSum());
					(*it)->setStatus(1);
					chunk.setZero();
				}
				else if ((bytes.getBytes() - (body_pos + chunk.getLenSum())) == 1) {
					break;
				}
				else
				{
					(*it)->getResponse()->setErrcode(400);
					(*it)->setStatus(1);
				}
				return;
			}
			if((*it)->bodyAppend(read_buff + chunk.getLenSum(), chunk.getLen()))
			{
				(*it)->getResponse()->setErrcode(500);
				(*it)->setStatus(1);
				return;
			}
			chunk.setBuffSum(chunk.getBuffSum() + chunk.getLen());
			chunk.setCount(chunk.getCount() + 1);
			chunk.setLenSum(chunk.getLenSum() + chunk.getLen());
			chunk.setLen(0);
			chunk.setLenSum(chunk.getLenSum() + 2);
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
				chunk.setLenSum(chunk.getLenSum() + chunk.getHexLen());
				chunk.setCount(chunk.getCount() + 1);
			}
			else {
				break;
			}
		}
	}
}


void Server::recv_msg(std::vector<Client*>::iterator it){
	int n;
	map_type::const_iterator map_it;
	char buff[1000000];
	bzero(&buff, 1000000);
	int err = 400;

	(*it)->setLastMsg();

#ifdef LINUX
	if((n = recv((*it)->getFd(), buff, sizeof(buff), 0)) <= 0)
#else
	if((n = recv((*it)->getFd(), buff, sizeof(buff), MSG_TRUNC)) <= 0)
#endif
	{
		(*it)->setStatus(3);
		return;
	}
	write(1, "1", 1);
	if((*it)->buffAppend(buff, n)) {
		(*it)->getResponse()->setErrcode(500);
	}
	(*it)->getBytes().bytesCount(n);
	if (ft_strnstr((*it)->getBuff(), "\r\n\r\n", (*it)->getBytes().getBytes())) {
		//parse and chech parse errror codes
		(*it)->getRequest()->req_init(((*it)->getBuff()));
		if ((*it)->getRequest()->error() || (err = error_headers(*(*it)->getRequest()))) {
			(*it)->setStatus(1);
			for (size_t i = 0; i < virt_serv.size(); ++i) {
				if (virt_serv[i].getHost() == (*it)->getServHost() &&
						virt_serv[i].getPort() == (*it)->getServPort()) {
					(*it)->getResponse()->setServerData(virt_serv[i]);
				}
			}
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
			struct sockaddr_in addr;
			bzero(&addr, sizeof(addr));
			int accept_sock;
			socklen_t slen = sizeof(addr);
			if ((accept_sock = accept((*it).getFd(), (struct sockaddr *) &addr, &slen)) < 0) {
				perror("accept");
				return (1);
			}
			fcntl(accept_sock, F_SETFL, O_NONBLOCK);
			client_session.push_back(new Client(accept_sock, (*it).getHost(), (*it).getPort(), addr));
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
			(*it)->getResponse()->setServerData(*serv_it);
			std::multimap<std::string, std::vector<std::string> > serv_data;
			return;
		}
	}
	(*it)->getResponse()->setErrcode(400);
}

int Server::clientSessionHandler(ErrorPages const & errPageMap) {
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
						(*it)->setCgiData();
						if (!(*it)->getRequest()->error()) // check for 400
							this->getLocation(it, data);
						(*it)->getResponse()->setReqBody((*it)->getBody());
						(*it)->getResponse()->setErrorPageTempl(&errPageMap.getErrorPageTemplates());
						(*it)->getResponse()->responsePrepare((*it)->getStatus(), &data, (*it)->getCgiData());
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
			if ((send((*it)->getFd(),
					(*it)->getResponse()->getResponseStruct().data,
					(*it)->getResponse()->getResponseStruct().length, 0)) < 0)
			{
				perror("send");
				return 1;
			}
			write(1, (*it)->getResponse()->getResponseStruct().data, (*it)->getResponse()->getResponseStruct().length);
			delete (*it)->getResponse();
			Response		*resp = new Response;
			(*it)->setResponse(resp);
			delete (*it)->getRequest();
			Request			*reqst = new Request;
			(*it)->setRequest(reqst);
//			if ((*it)->getStatus() == 3) {
//				this->closeConnection(it);
//				break;
//			}
			(*it)->setStatus(0);
		}
	}
	return (0);
}

int Server::launch() {
	ErrorPages		errPageMap;
	int i = 0;
	//главный цикл жизни сервера (Желательно потом разбить на еще доп методы, это я сделаю сам)
	int select_res;
	for (;;){
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
		if (this->clientSessionHandler(errPageMap))
			return (1);
		++i;
	}
}
