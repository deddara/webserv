#ifndef CGI_H
# define CGI_H
# include "includes.hpp"
# include <map>
# include <vector>
#include "cgi_data.hpp"
#include "unistd.h"

class Cgi{
private:
	char 				* body;
	const cgi_data 		& _cgi_data;
	std::string const 	&file_path;
	// CGI
public:
	typedef std::map<std::string, std::vector<std::string> >    map_type;
	Cgi(cgi_data const & data, std::string const &path) : _cgi_data(data), file_path(path) {};
	~Cgi();
	char **set_env();
	void exec_cgi();
	void get_cgi_response();

	char *getBody();

};

#endif