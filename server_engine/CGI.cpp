#include "CGI.hpp"
// CGI

Cgi::~Cgi() {}

char **Cgi::set_env() {
	std::map<std::string, std::string>  env_map;

	env_map["AUTH_TYPE"] = "Basic";
	env_map["GATEWAY_INTERFACE"] = "CGI/1.1";
	//
	env_map["SCRIPT_NAME"] = "get/it/from/conf/URI";
	env_map["SERVER_NAME"] = "get/it/from/conf/parse";

	env_map["SERVER_PORT"] = std::to_string(serv_port);
	env_map["SERVER_PROTOCOL"] = _data->find("head")[2];
	env_map["SERVER_SOFTWARE"] = "webserv/1.0";
	env_map["REMOTE_ADDR"] = inet_ntoa(addr.sin_addr);
	Request::value_type tmp = _data->find("authorization");
	if (!tmp.empty() && !tmp[0].empty())
		env_map["REMOTE_IDENT"] = env_map["REMOTE_USER"] = tmp[0];
	else
		env_map["REMOTE_IDENT"] = env_map["REMOTE_USER"] = "";

	//
	env_map["PATH_TRANSLATED"] = "physic/path/to/script";

	tmp = _data->find("head");
	if (tmp[1].find('?') != std::string::npos)
		env_map["QUERY_STRING"] = tmp[1].substr(tmp[1].find('?') + 1);
	else
		env_map["QUERY_STRING"] = "";

	env_map["CONTENT_LENGTH"] = std::to_string(bytes.getBytes() - _data->get_body_pos());
	tmp = _data->find("content_type");
	if (!tmp.empty() && !tmp[0].empty())
		env_map["CONTENT_TYPE"] = tmp[0];
	else
		env_map["CONTENT_TYPE"] = "";

	env_map["PATH_INFO"] = (*_data->find("head")).second[1];
	env_map["METHOD"] = _data->find("head")[0];
	env_map["REQUEST_URI"] = "http://" + serv_host + ":" + std::to_string(serv_port) + _data->find("head")[1];
	char **env = (char **)malloc(sizeof(char *) * (env_map.size() + 1));
	std::map<std::string, std::string>::iterator it = env_map.begin();
	for (int i = 0; it != env_map.end(); ++it, ++i)
		env[i] = ft_strdup((it->first + "=" + it->second).c_str());
	env[env_map.size() + 1] = NULL;
	return env;
}

void Cgi::exec_cgi() {
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

void Cgi::get_cgi_response() {

}
