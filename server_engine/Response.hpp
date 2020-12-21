#ifndef RESPONSE_HPP
# define RESPONSE_HPP
# include "iostream"
# include "includes.hpp"
# include "Request.hpp"
# include <fcntl.h>
# include <sys/types.h>
# include <sys/uio.h>
# include <unistd.h>

class Request;

class Response{
private:
	std::string response;
	std::string body;
	std::string date;
	std::map<std::string, std::vector<std::string> > const & _data;
public:
	Response(std::map<std::string, std::vector<std::string> > const & data) : _data(data) {};
	~Response(){};

	int bad_req();
	int ok();
	int server_error();
	int response_prepare(const Request *);

	void clearStr(){
		response.clear();
	}

	const char*  getStr() {return response.c_str(); }

};

#endif
