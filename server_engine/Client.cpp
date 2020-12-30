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

Client::Client(int fd, std::string const & host, int const & port, ErrorPages const & errPageMap, struct sockaddr_in & client_addr) :
	read_buff(nullptr), body_buff(nullptr), _fd(fd), state(0), serv_host(host), serv_port(port), addr(client_addr) {
	reqst = new Request();
	resp = new Response();
	resp->setErrorPageTempl(&errPageMap.getErrorPageTemplates());
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

// CGI
char **Client::set_env() {
	std::map<std::string, std::string>  env_map;

	env_map["AUTH_TYPE"] = "Basic";
	env_map["GATEWAY_INTERFACE"] = "CGI/1.1";
	//
	env_map["SCRIPT_NAME"] = "get/it/from/conf/URI";
	env_map["SERVER_NAME"] = "get/it/from/conf/parse";

	env_map["SERVER_PORT"] = std::to_string(serv_port);
	env_map["SERVER_PROTOCOL"] = reqst->find("head")[2];
	env_map["SERVER_SOFTWARE"] = "webserv/1.0";
	env_map["REMOTE_ADDR"] = inet_ntoa(addr.sin_addr);
	Request::value_type tmp = reqst->find("authorization");
	if (!tmp.empty() && !tmp[0].empty())
		env_map["REMOTE_IDENT"] = env_map["REMOTE_USER"] = tmp[0];
	else
		env_map["REMOTE_IDENT"] = env_map["REMOTE_USER"] = "";

	//
	env_map["PATH_TRANSLATED"] = "physic/path/to/script";

	tmp = reqst->find("head");
	if (tmp[1].find('?') != std::string::npos)
		env_map["QUERY_STRING"] = tmp[1].substr(tmp[1].find('?') + 1);
	else
		env_map["QUERY_STRING"] = "";

	env_map["CONTENT_LENGTH"] = std::to_string(bytes.getBytes() - reqst->get_body_pos());
	tmp = reqst->find("content_type");
	if (!tmp.empty() && !tmp[0].empty())
		env_map["CONTENT_TYPE"] = tmp[0];
	else
		env_map["CONTENT_TYPE"] = "";
	env_map["PATH_INFO"] = reqst->find("head")[1];
	env_map["METHOD"] = reqst->find("head")[0];
	env_map["REQUEST_URI"] = "http://" + serv_host + ":" + std::to_string(serv_port) + reqst->find("head")[1];
	char **env = (char **)malloc(sizeof(char *) * (env_map.size() + 1));
	std::map<std::string, std::string>::iterator it = env_map.begin();
	for (int i = 0; it != env_map.end(); ++it, ++i)
		env[i] = ft_strdup((it->first + "=" + it->second).c_str());
	env[env_map.size() + 1] = NULL;
	return env;
}

void Client::exec_cgi() {
	int pipes[2];
	pid_t pid;
	pipe(pipes);
	pid = fork();
	if (pid == 0) {
		dup2(pipes[0], 0);
		dup2(pipes[1], 1);
		write(pipes[1], "first_name=Lebrus&last_name=Shupay", 34); // Body если метод пост

		char **env = set_env();
		int res = execve("cgi_bin/cgi_tester", 0, env);

		//free env if ret = -1
		int i = 0;
		while (env[i])
			free(env[i++]);
		free(env);

		// send response about 'execve = -1';
		std::cout << res << std::endl;
		exit(res);
	} else {
		wait(nullptr);
		char line[1000];
		read(pipes[0], line, 1000);
		close(pipes[0]);
		close(pipes[1]);
		std::cout << line << std::endl;
	}
}

void Client::get_cgi_response() {

}


