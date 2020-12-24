#ifndef CLIENT_HPP
# define CLIENT_HPP
# include "iostream"
# include "vector"
# include "Request.hpp"
# include "Response.hpp"

enum states{
	rdy_recv,
	rdy_parse,
	rdy_respone,
	finish
};


class Client{
private:
	char		*read_buff;

	int			_fd;
	int			state;
	std::string serv_host;
	int			serv_port;

	Request		*reqst;
	Response	*resp;

public:

	Client(int fd, std::string const &, int const &);
	~Client();

	void setFd(int fd);
	int getFd();
	int & getStatus();
	void setStatus(int status);
	char const *getBuff();
	Request * getRequest();
	Response * getResponse();

	std::string const & getServHost();
	int const & getServPort();

	void buffAppend(char *);
	void clearBuff();

};

#endif
