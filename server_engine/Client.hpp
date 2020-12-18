#ifndef CLIENT_HPP
# define CLIENT_HPP
# include "iostream"
# include "vector"
# include "Request.hpp"

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

public:
	void setFd(int fd) { _fd = fd; }
	int const & getFd() { return _fd; }
	int const & getStatus() {return state; }
	void setStatus(int status) { state = status; }
	Request * getRequest() { return reqst; }
	Client(int fd) : _fd(fd), state(0) {}
	~Client();
	void buffAppend(std::string const & buff) {
		read_buff.append(buff);
	};

	std::string const & getBuff(){ return read_buff; }


};

#endif