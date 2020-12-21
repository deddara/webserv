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
	std::string	write_buff;

	int			_fd;
	int 		state;

	Request		*reqst;
	Response	*resp;

public:

	void responseInit(std::map<std::string, std::vector<std::string> > const & data);
	Client(int fd);
	~Client();

	void setFd(int fd);
	int getFd();
	int getStatus();
	void setStatus(int status);
	std::string const &getBuff();
	Request * getRequest();

	Response * getResponse();

	void buffAppend(std::string const & buff);
	void clearBuff();

};

#endif