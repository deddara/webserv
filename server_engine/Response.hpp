#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "ErrorPages.hpp"
# include "Location.hpp"
# include "includes.hpp"
# include <fcntl.h>
# include <map>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/uio.h>
# include <unistd.h>

class Response{
private:
	std::map<std::string, std::vector<std::string> > const *
										_data;
	int									errCode;
	std::string							response;
	char *								body;
	size_t								bodyLength;
	std::string							redirectURI;
	std::vector<Location*>				location;
	size_t								currLocationInd;
	std::map<int, std::string> const *	errorPage;
	std::string							filePath;

	void								createErrPagesMap();
	void								errorExit(int, std::string const &);
	int									checkUri();
	int									checkLocation();
	int									checkFile();
	void								errorHandler();
	void								error304Handler();
	void								error403Handler();
	void								error404Handler();
	void								generateRedirectURI(int);
	void								generateBody();
	void								generateFilePath();

public:
	typedef std::map<std::string, std::vector<std::string> > const
										map_type;
										Response();
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
