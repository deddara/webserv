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

Bytes & Client::getBytes() {
	return bytes;
}

const std::string & Client::getServHost() { return serv_host; }

const int & Client::getServPort() { return serv_port; }

int Client::buffAppend(char* buff, const int &len) {
	if (!read_buff)
	{
		if (!(read_buff = bytes.bytesDup(read_buff, buff, len)))
			return 1;
	}
	else
	{
		char *tmp = read_buff;
		if (!(read_buff = bytes.bytesJoin(read_buff, buff, len)))
			return 1;
		free(tmp);
	}
	return (0);
};

int Client::buffCut(const unsigned long &len) {
	char *tmp = read_buff;
	if (!(read_buff = bytes.bytesCut(read_buff, len)))
		return 1;
	free(tmp);
	return (0);
}

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
		i = bytes_readed;
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
	bytes.setBytes(0);
	read_buff = nullptr;
	body_buff = nullptr;
}