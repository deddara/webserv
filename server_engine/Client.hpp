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
	std::string	read_buff;

	int			_fd;
	int 		state;
	int 		serv_port;
	std::string serv_host;

	Request		*reqst;
	Response	*resp;

public:

	Client(int fd);
	~Client();

	void setFd(int fd);
	int getFd();
	int & getStatus();
	void setStatus(int status);
	std::string const &getBuff();
	Request * getRequest();
	Response * getResponse();

	std::string const & getServHost();
	int const & getServPort();

	void buffAppend(std::string const & buff);
	void clearBuff();

};

#endif