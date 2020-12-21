#include "Client.hpp"

Client::Client(int fd) : _fd(fd), state(0), resp(0){
	reqst = new Request();
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

void Client::responseInit(const std::map<std::string, std::vector<std::string> > &data)  {
	resp = new Response(data);
}

void Client::buffAppend(std::string const & buff) {
	read_buff.append(buff);
};

const std::string & Client::getBuff() { return read_buff; }

void Client::clearBuff()
{
	read_buff.clear();
}