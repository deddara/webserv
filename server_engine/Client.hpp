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
	char		*body_buff;

	int			_fd;
	int 		bytes_readed;
	int 		state;
	int 		serv_port;
	std::string serv_host;

	Request		*reqst;
	Response	*resp;

public:

	Client(int fd, std::string const &, int const &);
	~Client();

	void setFd(int fd);
	int getFd();
	int & getStatus();
	int const & getBytesReaded() { return  bytes_readed; }
	void setStatus(int status);
	char const *getBuff();
	char const *getBody();

	Request * getRequest();
	Response * getResponse();

	std::string const & getServHost();
	int const & getServPort();

	void buffAppend(char *);
	void bodyAppend(const char *, int len);
	void clearBuff();

};

#endif