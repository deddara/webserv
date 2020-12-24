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
	std::map<std::string, std::vector<std::string> > const *
										_data;
	int									errCode;
	std::string							response;
	char *								body;
	std::string							redirectURI;
	std::vector<Location*>				location;
	std::map<int, std::string> const *	errorPage;

	std::string			fileName;
	int					checkUri();
	int					checkLocation();
	void				error403Handler();
	void				error404Handler();
	void				generateRedirectURI(int);
	void				generateBody();

public:
	typedef std::map<std::string, std::vector<std::string> > const map_type;
	Response() : _data(nullptr), errCode(0) {};
	~Response(){};

	void setLocation(std::vector<Location*> const & loc) { location = loc; }
	void setErrorPage(const std::map<int, std::string> *);
	void setErrcode(int const &num ) { errCode = num; }

	void response_prepare(int &, map_type *);
	void connectionHandler(int & status);

	void clearStr() {
		response.clear();
	}

	const char*  getStr() {return response.c_str(); }

};

#endif
