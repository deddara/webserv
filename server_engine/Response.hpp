#ifndef RESPONSE_HPP
# define RESPONSE_HPP
# include "iostream"
# include "includes.hpp"
# include <fcntl.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/uio.h>
# include <unistd.h>
# include "Location.hpp"
#include "map"

class Response{
private:
	std::string							response;
	char *								body;
	int									err_code;

	std::vector<Location*>				location;
	std::map<int, std::string> const *	pr_errorPage;
	std::map<std::string, std::vector<std::string> > const * _data;

	std::string			fileName;
	int					checkUri();
	int					checkLocation();

public:
	typedef std::map<std::string, std::vector<std::string> > const map_type;
	Response() : _data(nullptr), err_code(0) {};
	~Response(){};

	void setLocation(std::vector<Location*> const & loc) { location = loc; }
	void setErrorPage(const std::map<int, std::string> *);
	void setErrcode(int const &num ) { err_code = num; }

	void response_prepare(int &, map_type *);
	void connectionHandler(int & status);

	void clearStr() {
		response.clear();
	}

	const char*  getStr() {return response.c_str(); }

};

#endif
