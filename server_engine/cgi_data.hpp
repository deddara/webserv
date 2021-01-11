#ifndef CGI_DATA_H
# define CGI_DATA_H
#include "map"
#include "iostream"
# include <arpa/inet.h>

struct cgi_data{

	std::map<std::string, std::vector<std::string> >	const *data;
	int												body_len;
	struct sockaddr_in *							addr;
	std::string										serv_host;
	int												serv_port;
};


#endif