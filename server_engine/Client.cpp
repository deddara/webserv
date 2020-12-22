#include "Client.hpp"

Client::Client(int fd, std::string const & host, int const & port) : _fd(fd), state(0), serv_host(host), serv_port(port), read_buff(nullptr), body_buff(nullptr), bytes_readed(0){
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

void Client::bodyAppend(char const *buff, int len) {
	int i = 0;
	if (!body_buff)
	{
		body_buff = (char*)malloc(sizeof(char) * len + 1);
		while (i < len) {
			body_buff[i] = buff[i];
			i++;
		}
		bytes_readed += i;
		body_buff[len] = '\0';
	}
	else
	{
		int i = bytes_readed;
		while (i < len)
		{
			body_buff[i] = buff[i];
			i++;
		}
		bytes_readed += i;
	}
}

const char * Client::getBuff() { return read_buff; }

const char * Client::getBody() {
	return body_buff;
}

void Client::clearBuff()
{
	if (read_buff)
		free(read_buff);
	if (body_buff)
		free(body_buff);
	read_buff = nullptr;
	body_buff = nullptr;
}