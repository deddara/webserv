#ifndef CGI_H
# define CGI_H
# include "includes.hpp"
# include <map>
# include <vector>
#include "cgi_data.hpp"
#include "unistd.h"
#include "Bytes.hpp"
#include <sys/types.h>
#include <sys/wait.h>

class Cgi{
private:
	char 				* resp_buff;
	const cgi_data 		& _cgi_data;
	std::string const 	&file_path;
	Bytes				bytes;
	const char			*body;

	int pipes[2];
	int err_pipe[2];
	int status = 0;
	pid_t pid;
	char **_argv;
	char **_env;

public:
	typedef std::map<std::string, std::vector<std::string> >    map_type;
	Cgi(cgi_data const & data, std::string const &path, const char *);
	~Cgi();


	char const 	*	getResponse() const;
	Bytes const &	getBytes() const;

	char			**setEnv();
	int 			execute();
	int				handler();
	int				buffAppend(const char *buff, int len);
	int				read_response();


};

#endif
