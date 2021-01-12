/* *************************************************************************************/
/*                                                                                     */
/*                                                             |\---/|                 */
/*  Client.cpp                                                 | o_o |                 */
/*                                                             ‾‾‾‾‾‾‾                 */
/*  By: deddara <deddara@student-21.school.ru>                 ┌┬┐┌─┐┌┬┐┌┬┐┌─┐┬─┐┌─┐   */
/*                                                             _││├┤  ││ ││├─┤├┬┘├─┤   */
/*  created: 12/25/20 22:28:34 by deddara                      ─┴┘└─┘─┴┘─┴┘┴ ┴┴└─┴ ┴   */
/*  updated: 12/26/20 19:21:35 by deddara                      +-++-++-++-++-++-++-+   */
/*                                                             |)[-|)|)/-\|2/-\        */
/*                                                                                     */
/* **********************************************************²**************************/
#include "Client.hpp"
#include "sys/time.h"

Client::Client(int fd, std::string const & host, int const & port, struct sockaddr_in & client_addr) :
	read_buff(nullptr), body_buff(nullptr), _fd(fd), state(0), serv_host(host), serv_port(port), addr(client_addr) {
	body_len = 0;
	reqst = new Request();
	resp = new Response();
	gettimeofday(&last_msg, NULL);
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

Chunk & Client::getChunk() {
	return chunk;
}

const std::string & Client::getServHost() { return serv_host; }

const int & Client::getServPort() { return serv_port; }

int Client::bodyAppend(char const *buff, const int &len) {
	if (!body_buff)
	{
		if (!(body_buff = bytes.bytesDup(body_buff, buff, len)))
			return 1;
	}
	else
	{
		char *tmp = body_buff;
		if (!(body_buff = bytes.bytesJoin(body_buff, buff, len, chunk.getBuffSum())))
			return 1;
		free(tmp);
	}
	return (0);
}

int Client::buffAppend(char const * buff, const int &len) {
	if (!read_buff)
	{
		if (!(read_buff = bytes.bytesDup(read_buff, buff, len)))
			return 1;
	}
	else
	{
		char *tmp = read_buff;
		if (!(read_buff = bytes.bytesJoin(read_buff, buff, len,bytes.getBytes())))
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

const char * Client::getBuff() { return read_buff; }

const char * Client::getBody() {
	return body_buff;
}

void Client::setBody(char *str) { body_buff = str; }

struct timeval &Client::getLastMsg(){
	return last_msg;
}

void		Client::setLastMsg(){
	gettimeofday(&last_msg, NULL);
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

void Client::setCgiData() {
	_cgi_data.data = &reqst->getData();
	_cgi_data.addr = &addr;
	_cgi_data.body_len = body_len;
	_cgi_data.serv_host = serv_host;
	_cgi_data.serv_port = serv_port;
}

void Client::setResponse(Response *response) {
	resp = response;
}

void Client::setRequest(Request *req) { reqst = req; }

cgi_data const & Client::getCgiData() const { return _cgi_data; }


void Client::setBodyLen(const int &num) { body_len = num; }
const int & Client::getBodyLen() const { return body_len; }
