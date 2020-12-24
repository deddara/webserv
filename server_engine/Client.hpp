/* *************************************************************************************/
/*                                                                                     */
/*                                                             |\---/|                 */
/*  Client.hpp                                                 | o_o |                 */
/*                                                             ‾‾‾‾‾‾‾                 */
/*  By: deddara <deddara@student-21.school.ru>                 ┌┬┐┌─┐┌┬┐┌┬┐┌─┐┬─┐┌─┐   */
/*                                                             _││├┤  ││ ││├─┤├┬┘├─┤   */
/*  created: 12/24/20 19:19:04 by deddara                      ─┴┘└─┘─┴┘─┴┘┴ ┴┴└─┴ ┴   */
/*  updated: 12/24/20 19:55:25 by deddara                      +-++-++-++-++-++-++-+   */
/*                                                             |)[-|)|)/-\|2/-\        */
/*                                                                                     */
/* **********************************************************²**************************/
#ifndef CLIENT_HPP
# define CLIENT_HPP
# include "iostream"
# include "vector"
# include "Request.hpp"
# include "Response.hpp"
# include "Bytes.hpp"
enum states{
	rdy_recv,
	rdy_parse,
	rdy_respone,
	finish
};


class Client{
private:
	char			*read_buff;
	char			*body_buff;

	int				_fd;
	int 			bytes_readed;
	int 			state;
	int 			serv_port;
	std::string		serv_host;

	Bytes			bytes;
	Request			*reqst;
	Response		*resp;
	struct timeval	last_msg;

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
	Bytes	& getBytes();

	std::string const & getServHost();
	int const & getServPort();

	int buffAppend(char *, const int &);
	int buffCut(unsigned long const &);
	void bodyAppend(const char *, int len);
	void clearBuff();

	struct timeval & getLastMsg();
	void		setLastMsg();

};

#endif