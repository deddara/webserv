/* *************************************************************************************/
/*                                                                                     */
/*                                                             |\---/|                 */
/*  Client.cpp                                                 | o_o |                 */
/*                                                             ‾‾‾‾‾‾‾                 */
/*  By: deddara <deddara@student-21.school.ru>                 ┌┬┐┌─┐┌┬┐┌┬┐┌─┐┬─┐┌─┐   */
/*                                                             _││├┤  ││ ││├─┤├┬┘├─┤   */
/*  created: 12/25/20 22:28:34 by deddara                      ─┴┘└─┘─┴┘─┴┘┴ ┴┴└─┴ ┴   */
/*  updated: 12/25/20 22:29:15 by deddara                      +-++-++-++-++-++-++-+   */
/*                                                             |)[-|)|)/-\|2/-\        */
/*                                                                                     */
/* **********************************************************²**************************/
#include "Client.hpp"
#include "sys/time.h"

Client::Client(int fd, std::string const & host, int const & port) : _fd(fd), state(0), serv_host(host), serv_port(port), read_buff(nullptr), body_buff(nullptr){
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

const char * Client::getBuff() { return read_buff; }

const char * Client::getBody() {
	return body_buff;
}

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