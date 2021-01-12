#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "ErrorPages.hpp"
# include "VirtServer.hpp"
# include "includes.hpp"
# include <dirent.h>
# include <fcntl.h>
# include <map>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/uio.h>
# include <unistd.h>
# include "CGI.hpp"
# include "cgi_data.hpp"
# include <stdexcept>

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
	int									reqBodyLen;
	char *								body;
	const char *						reqBody;
	size_t								bodyLength;
	size_t								currLocationInd;
	const VirtServer *					virtServ;
	std::vector<Location *>				location;
	std::string							responseHeaders;
	std::string							redirectURI;
	std::map<int, std::string> const *	errorPage;
	std::string							filePath;
	std::string							fileModifiedTime;
	std::string							webservVersion;
	std::string							dirListing;
	std::string							fileExt;
	struct s_response					response;
	Cgi								*	cgi;

	// errHandlersFlags: 0b00000001 - 403 checked, 0b00000010 - 404 checked
	char								errHandlersFlags;
	int									limitClientBody;

	int                                 checkAuth() const;
	int									checkLocation();
	int									checkAllowMethods();
	int									checkFile();
	void								errorHandler();
	void								error403Handler();
	void								error404Handler();
	void								generateBody();
	void								generateFilePath();
	void								buildResponse();
	void								generateDirListing();
	void								cgi_response_parser(Cgi const &);
	int									checkLimitClientBody(const cgi_data &);
	int									checkExtForCgiHandling();

public:
	typedef std::map<std::string, std::vector<std::string> > const
										map_type;
										Response();
										~Response();

	void	setServerData(VirtServer const &);
	void	setErrorPageTempl(const std::map<int, std::vector<std::string> > *);
	void	setErrcode(int const &num );
	void	setReqBody(const char *);
	void	responsePrepare(int &, std::map<std::string,
						std::vector<std::string> > const *, const cgi_data &);
	void	connectionHandler(int & status);
	void	clearResponseData();
	int		putHandler();

	struct s_response &					getResponseStruct();
	VirtServer const &					getVirtServer() const;
};

#endif
