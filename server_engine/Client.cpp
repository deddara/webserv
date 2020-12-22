#include "Client.hpp"

Client::Client(int fd, std::string const & host, int const & port) : _fd(fd), state(0), resp(0), serv_host(host), serv_port(port), read_buff(nullptr){
	reqst = new Request();
	resp = new Response();
}

Client::~Client() {
		delete reqst;
		delete resp;
}

void Client::setFd(int fd) { _fd = fd; }
int Client::getFd() { return _fd; }
int & Client::getStatus() {return state; }
void Client::setStatus(int status) { state = status; }
Request * Client::getRequest() { return reqst; }
Response * Client::getResponse() { return resp; }

const std::string & Client::getServHost() { return serv_host; }

const int & Client::getServPort() { return serv_port; }

void Client::buffAppend(char* buff) {
	char *tmp;
	tmp = read_buff;
	read_buff = ft_strjoin(read_buff, buff);
	if (tmp)
		free(tmp);
};

const char * Client::getBuff() { return read_buff; }

void Client::clearBuff()
{
	free(read_buff);
	read_buff = nullptr;
}