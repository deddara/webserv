#ifndef RESPONSE_HPP
# define RESPONSE_HPP
# include "iostream"
# include "includes.hpp"
# include <fcntl.h>
# include <sys/types.h>
# include <sys/uio.h>
# include <unistd.h>
# include "Location.hpp"
#include "map"

class Response{
private:
	std::string response;
	std::string body;
	std::string date;
	std::vector<Location> location;
	std::map<std::string, std::vector<std::string> > const * _data;

public:
	typedef std::map<std::string, std::vector<std::string> > const map_type;
	Response() : _data(nullptr) {};
	~Response(){};

	void setLocation(std::vector<Location> const & loc) { location = loc; }

	int bad_req();
	int ok();
	int response_prepare(int const & errcode, int &, map_type *);

	void clearStr(){
		response.clear();
	}

	const char*  getStr() {return response.c_str(); }

};

#endif
