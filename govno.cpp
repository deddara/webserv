#include "./server_engine/CGI.hpp"
#include "stdlib.h"
#include "unistd.h"
//struct cgi_data{
//
//	std::map<std::string, std::vector<std::string> >	const *data;
//	int												body_len;
//	struct sockaddr_in *							addr;
//	std::string										serv_host;
//	int												serv_port;
//};


int main()
{
//	struct cgi_data cgi_;
//	cgi_.body_len = 10000000;
//	cgi_.serv_host = "127.0.0.1";
//	cgi_.serv_port = 80;
//	char *data = (char*)malloc(sizeof(char) * cgi_.body_len);
//	memset(data, 'a', cgi_.body_len);
//	while (1)
//	{
//		std::string filepath = "/home/deddara/webserv/site/test/YoupiBanane/youpi.bla";
//		std::string bin = "/home/deddara/webserv/ubuntu_cgi_tester";
//		Cgi  *cgi = new Cgi(cgi_, filepath, bin, data);
//		int res = cgi->handler();
//		delete cgi;
//		std::cout << res << std::endl;
//	}
int res;
char *tmp = (char*)malloc(res = (2 + 2) + 10);
std::cout << res;
}