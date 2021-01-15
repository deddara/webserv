#include "CGI.hpp"
#include "stdlib.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

// CGI

Cgi::Cgi(const cgi_data &data, const std::string &path, std::string const & binPath, char *bdy) : resp_buff(NULL),_cgi_data(data), file_path(path), bin_path(binPath), body(bdy), _argv(NULL), _env(NULL), status(0) {
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
	env_map["SCRIPT_NAME"] = bin_path;
	env_map["PATH_TRANSLATED"] = file_path;
	env_map["REMOTE_ADDR"] = "127.0.0.1";


	num = ft_itoa(_cgi_data.serv_port);
	env_map["SERVER_PORT"] = num;
	free(num);

	num = ft_itoa(_cgi_data.body_len);
	env_map["CONTENT_LENGTH"] = num;
	free(num);

//	map_it = _cgi_data.data->find("host");
//	env_map["SERVER_NAME"] = map_it->second[0];

	map_it = _cgi_data.data->find("authorization");
	if (map_it != _cgi_data.data->end() && !map_it->second[0].empty()) {
		size_t pos = map_it->second[0].find(' ');
		env_map["AUTH_TYPE"] = map_it->second[0].substr(0, pos);
		env_map["REMOTE_IDENT"] = env_map["REMOTE_USER"] = map_it->second[0].substr(pos + 1);
	}
//	else
//		env_map["REMOTE_IDENT"] = env_map["REMOTE_USER"] = "";

	map_it = _cgi_data.data->find("head");
	if (map_it->second[1].find('?') != std::string::npos)
		env_map["QUERY_STRING"] = map_it->second[1].substr(map_it->second[1].find('?') + 1);
	else
		env_map["QUERY_STRING"] = "";
	env_map["REQUEST_METHOD"] = map_it->second[0];
	env_map["PATH_INFO"] = map_it->second[1];
	env_map["REQUEST_URI"] = map_it->second[1];
	method = map_it->second[0];

	//for PHP
//	env_map["REDIRECT_STATUS"] = "200";

	env_map["HTTP_ACCEPT-ENCODING"] = "gzip";
	env_map["HTTP_CONTENT-TYPE"] = "test/file";
	env_map["HTTP_HOST"] = "localhost:3038";
	env_map["HTTP_TRANSFER-ENCODING"] = "chunked";
	env_map["HTTP_USER-AGENT"] = "Go-http-client/1.1";
	map_it = _cgi_data.data->find("content-type");
//	if (map_it == _cgi_data.data->end() || map_it->second[0].empty())
//		env_map["CONTENT_TYPE"] = "";
//	else
//		env_map["CONTENT_TYPE"] = map_it->second[0];

	map_it = _cgi_data.data->find("accept");
	if (map_it != _cgi_data.data->end() && !map_it->second[0].empty())
		env_map["HTTP_ACCEPT"] = map_it->second[0];

	map_it = _cgi_data.data->find("x-secret-header-for-test");
	if (map_it != _cgi_data.data->end() && !map_it->second[0].empty())
		env_map["HTTP_X_SECRET_HEADER_FOR_TEST"] = map_it->second[0];
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
	if (pipe(fd) < 0)
		return (1);
	if((pid = fork()) < 0)
		return (1);

	fd_tmp = open("./temp_file",  O_CREAT | O_RDWR, S_IRWXU);
	if(fd_tmp < 0)
		return (1);
	if (pid == 0) {
		close(fd[1]);
		dup2(fd[0], 0);
		close(fd[0]);
		dup2(fd_tmp, 1);
		close(fd_tmp);
		if (execve(_argv[0], _argv, _env) < 0) {
			exit(1);
		}
	}
	return (0);
}

int Cgi::read_response(){
	struct stat f_data;
	int n;
	int body_len = _cgi_data.body_len;

	close(fd[0]);
	usleep(20);
	if (body_len) {
		int i = 0;
		while (_env[i])
		{
			std::cout << _env[i] << std::endl;
			++i;
		}
		write(fd[1], body, body_len);
		if (body) {
			free(body);
			body = NULL;
		}
	}
	close(fd[1]);
	waitpid(pid, &status, 0);
	if (WEXITSTATUS(status) != 0) {
		std::cout << "ERRRR" << std::endl;
		return (502);
	}
	if (fstat(fd_tmp, &f_data) < 0)
		return (500);
	char *tmp = (char*)malloc(sizeof(char) * f_data.st_size + 1);
	n = read(fd_tmp, tmp, f_data.st_size);
	tmp[n] = '\0';
	if (buffAppend(tmp, f_data.st_size))
		return (500);
	bytes.bytesCount(n);
	unlink("./temp_file");
	return (0);
}

int Cgi::handler(){
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
//	write(readPipe[1], body, _cgi_data.body_len);

	int execute_result = read_response();

	if (!execute_result)
		execute_result = 200;

	return (execute_result);
}



char const * Cgi::getResponse() const { return resp_buff; }

const Bytes & Cgi::getBytes() const { return bytes; }
