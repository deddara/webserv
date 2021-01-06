#ifndef CGI_H
# define CGI_H
# include "includes.hpp"
# include <map>
# include "Client.hpp"

class Cgi{

private:
	char 				* body;
	const cgi_data 		& _cgi_data;
	// CGI
public:
	Cgi(cgi_data const & data) : _cgi_data(data) {};
	~Cgi();
	char **set_env();
	void exec_cgi();
	void get_cgi_response();

};

#endif