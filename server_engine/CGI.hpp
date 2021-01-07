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
	// CGI
public:
	typedef std::map<std::string, std::vector<std::string> >    map_type;
	Cgi(cgi_data const & data, std::string const &path) : resp_buff(NULL),_cgi_data(data), file_path(path) {};
	~Cgi();
	char **set_env();
	void exec_cgi();
	void get_cgi_response();

	int cgiBuffAppend(const char *buff, int len);
	char const *getBody() const;
	Bytes const & getBytes() const { return bytes; }

};

#endif