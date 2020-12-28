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

struct									s_response {
	char *								data;
	size_t								length;
};

class Response{
private:
	std::map<std::string, std::vector<std::string> > const *
										_data;
	std::map<int, std::vector<std::string> > const *
										errorPageTempl;
	int									errCode;
	char *								body;
	size_t								bodyLength;
	size_t								currLocationInd;
	std::vector<Location *>				location;
	std::string							responseHeaders;
	std::string							redirectURI;
	std::map<int, std::string> const *	errorPage;
	std::string							filePath;
	std::string							fileModifiedTime;
	std::string							webservVersion;
	struct s_response					response;
	// errHandlersFlags: 0b00000001 - 403 checked, 0b00000010 - 404 checked
	char								errHandlersFlags;

	void								createErrPagesMap();
	void								errorExit(int, std::string const &);
	int									checkUri();
	int									checkLocation();
	int									checkAllowMethods();
	int									checkFile();
	void								errorHandler();
	void								error403Handler();
	void								error404Handler();
	void								generateRedirectURI(int);
	void								generateBody();
	void								generateFilePath();
	void								buildResponse();

public:
	typedef std::map<std::string, std::vector<std::string> > const
										map_type;
										Response();
										~Response();

	void	setLocation(std::vector<Location *> const &);
	void	setErrorPagePath(const std::map<int, std::string> *);
	void	setErrorPageTempl(const std::map<int, std::vector<std::string> > *);
	void	setErrcode(int const &num );
	void	responsePrepare(int &, map_type *);
	void	connectionHandler(int & status);
	void	clearResponseData();
	struct s_response & getResponseStruct();

};

#endif
