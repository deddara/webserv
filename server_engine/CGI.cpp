#include "CGI.hpp"
#include "stdlib.h"
// CGI

Cgi::Cgi(const cgi_data &data, const std::string &path, std::string const & binPath, const char *bdy) : resp_buff(NULL),_cgi_data(data), file_path(path), bin_path(binPath), body(bdy), _argv(NULL), _env(NULL), status(0) {
	return;
}

Cgi::~Cgi() {
	if (resp_buff)
		free (resp_buff);
	if (_env)
	{
		int i = 0;
		while (_env[i])
		{
			free(_env[i]);
			i++;
		}
		free(_env);
	}
	if (_argv)
	{
		int i = 0;
		while (_argv[i])
		{
			free(_argv[i]);
			i++;
		}
		free(_argv);
	}
}

int Cgi::buffAppend(const char *buff, int len){
	if (!resp_buff)
	{
		if (!(resp_buff = bytes.bytesDup(resp_buff, buff, len)))
			return 1;
	}
	else
	{
		char *tmp = resp_buff;
		if (!(resp_buff = bytes.bytesJoin(resp_buff, buff, len,bytes.getBytes())))
			return 1;
		free(tmp);
	}
	return (0);
}

char **Cgi::setEnv() {
	std::map<std::string, std::string>  env_map;
	map_type::const_iterator map_it;
	char *num;

	env_map["SERVER_SOFTWARE"] = "webserv/1.0";
	env_map["SERVER_PROTOCOL"] = "HTTP/1.1";
	env_map["GATEWAY_INTERFACE"] = "CGI/1.1";
	env_map["SCRIPT_NAME"] = file_path;
	env_map["PATH_TRANSLATED"] = file_path;
	env_map["REMOTE_ADDR"] = inet_ntoa(_cgi_data.addr->sin_addr);


	num = ft_itoa(_cgi_data.serv_port);
	env_map["SERVER_PORT"] = num;
	free(num);

	num = ft_itoa(_cgi_data.body_len);
	env_map["CONTENT_LENGTH"] = num;
	free(num);

	map_it = _cgi_data.data->find("host");
	env_map["SERVER_NAME"] = map_it->second[0];


	map_it = _cgi_data.data->find("authorization");
	if (map_it != _cgi_data.data->end() && !map_it->second[0].empty()) {
		env_map["AUTH_TYPE"] = map_it->second[0];
		env_map["REMOTE_IDENT"] = env_map["REMOTE_USER"] = map_it->second[1];
	}
	else
		env_map["REMOTE_IDENT"] = env_map["REMOTE_USER"] = "";

	map_it = _cgi_data.data->find("head");
	if (map_it->second[1].find('?') != std::string::npos)
		env_map["QUERY_STRING"] = map_it->second[1].substr(map_it->second[1].find('?') + 1);
	else
		env_map["QUERY_STRING"] = "";
	env_map["REQUEST_METHOD"] = map_it->second[0];
	env_map["PATH_INFO"] = map_it->second[1];
	env_map["REQUEST_URI"] = "http://" + _cgi_data.serv_host + ":" + std::to_string(_cgi_data.serv_port) + map_it->second[1];
	method = map_it->second[0];

	//for PHP
	env_map["REDIRECT_STATUS"] = "200";

	map_it = _cgi_data.data->find("content-type");
	if (map_it == _cgi_data.data->end() || map_it->second[0].empty())
		env_map["CONTENT_TYPE"] = "";
	else
		env_map["CONTENT_TYPE"] = map_it->second[0];

	map_it = _cgi_data.data->find("accept");
	if (map_it != _cgi_data.data->end() && !map_it->second[0].empty())
		env_map["HTTP_ACCEPT"] = map_it->second[0];


	char **env;
	if (!(env = (char **)malloc(sizeof(char *) * (env_map.size() + 1))))
		return (NULL);
	std::map<std::string, std::string>::iterator it = env_map.begin();
	for (int i = 0; it != env_map.end(); ++it, ++i)
		env[i] = ft_strdup((it->first + "=" + it->second).c_str());
	env[env_map.size()] = NULL;
	return env;
}

int Cgi::execute() {
	readPipe[0] = readPipe[1] = writePipe[0] = writePipe[1] = err_pipe[0] = err_pipe[1] = -1;
	if(pipe(readPipe) < 0 || pipe(writePipe) || pipe(err_pipe) < 0)
		return (1);
	if (dup2(writePipe[1], 1) < 0 || dup2(readPipe[0], 0) < 0)
		return (1);
	close(readPipe[0]);
	close(writePipe[1]);

	if((pid = fork()) < 0)
		return (1);
	if (pid == 0) {
		if (execve(_argv[0], _argv, _env) < 0) {
			dup2(err_pipe[0], 0);
			dup2(err_pipe[1], 1);
			write(1, "1", 1);
			exit(1);
		}
	}
	return (0);
}

int Cgi::read_response(){
	int n = 0;
	int max_fd = 0;
	int body_len = _cgi_data.body_len;
	while (1) {
		char line[1024];
		bzero(line, 1024);
		fd_set readset, writeset;

		waitpid(pid, &status, WNOHANG);
		if (WEXITSTATUS(status) != 0) {
			return (502);
		}

		FD_ZERO(&readset); FD_ZERO(&writeset);
		FD_SET(err_pipe[0], &readset);
		FD_SET(writePipe[0], &readset);
		if (body_len > 0)
			FD_SET(readPipe[1], &writeset);

		if (writePipe[0] > err_pipe[0] && writePipe[0] > readPipe[1])
			max_fd = writePipe[0];
		else if (err_pipe[0] > writePipe[0] && err_pipe[0] > readPipe[1])
			max_fd = err_pipe[0];
		else
			max_fd = readPipe[1];

		if (select(max_fd + 1, &readset, &writeset, 0, 0) < 0) {
			return (500); //handle error
		}

		if (FD_ISSET(readPipe[1], &writeset)){
			write (1, "2", 1);
			int n = write(readPipe[1], body, _cgi_data.body_len);
			body_len -= n;
			continue;
		}

		if (FD_ISSET(err_pipe[0], &readset)){
			return (502);
		}
		n = read(writePipe[0], line, 1024);
		if((buffAppend(line, n)))
			return (500); //handle error
		bytes.bytesCount(n);
		if (n <= 0)
			continue;
		if (n < 1024) {
			return (0); //file readed
		}
	}
}

int Cgi::handler(){
	int oldWrite, oldRead;
	oldWrite = dup(1);
	oldRead = dup(0);

	if(!(_argv = (char **) malloc(sizeof(char *) * 3)))
		return (500);
	if(!(_argv[0] = ft_strdup(bin_path.c_str())))
		return (500);
	if(!(_argv[1] = ft_strdup(file_path.c_str())))
		return (500);
	_argv[2] = 0;

	if (!(_env = setEnv()))
		return (500);

	if (execute())
		return (500);

	dup2(oldRead, 0);
	dup2(oldWrite, 1);
	close(oldWrite);
	close(oldRead);
	oldWrite = -1;
	oldRead = -1;
//	write(readPipe[1], body, _cgi_data.body_len);

	int execute_result = read_response();

	close(err_pipe[0]);
	close(err_pipe[1]);
	close(writePipe[0]);
	close(writePipe[1]);
	close(readPipe[1]);
	close(readPipe[0]);

	return (execute_result);
}



char const * Cgi::getResponse() const { return resp_buff; }

const Bytes & Cgi::getBytes() const { return bytes; }
