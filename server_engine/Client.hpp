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
	void setFd(int fd) { _fd = fd; }
	int getFd() { return _fd; }
	int getStatus() {return state; }
	void setStatus(int status) { state = status; }
	Request * getRequest() { return reqst; }
	Response * getResponse() { return resp; }
	Client(int fd) : _fd(fd), state(0){
		reqst = new Request();
		resp = new Response();
	}
	~Client();
	void buffAppend(std::string const & buff) {
		read_buff.append(buff);
	};

	std::string const &getBuff(){ return read_buff; }
	void clearBuff()
	{
		read_buff.clear();
	}

};

#endif