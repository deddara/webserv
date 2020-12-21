#include "Client.hpp"

Client::Client(int fd, std::string const & host, int const & port) : _fd(fd), state(0), resp(0), serv_host(host), serv_port(port){
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

void Client::buffAppend(std::string const & buff) {
	read_buff.append(buff);
};

const std::string & Client::getBuff() { return read_buff; }

void Client::clearBuff()
{
	read_buff.clear();
}