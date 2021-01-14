/* *************************************************************************************/
/*                                                                                     */
/*                                                             |\---/|                 */
/*  Server.hpp                                                 | o_o |                 */
/*                                                             ‾‾‾‾‾‾‾                 */
/*  By: deddara <deddara@student-21.school.ru>                 ┌┬┐┌─┐┌┬┐┌┬┐┌─┐┬─┐┌─┐   */
/*                                                             _││├┤  ││ ││├─┤├┬┘├─┤   */
/*  created: 12/24/20 21:37:23 by deddara                      ─┴┘└─┘─┴┘─┴┘┴ ┴┴└─┴ ┴   */
/*  updated: 12/25/20 23:24:29 by deddara                      +-++-++-++-++-++-++-+   */
/*                                                             |)[-|)|)/-\|2/-\        */
/*                                                                                     */
/* **********************************************************²**************************/
#ifndef SERVER_H
# define SERVER_H
#include <iostream>
#include "vector"
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/select.h>
#include "zconf.h"
#include "unistd.h"
#include "Client.hpp"
#include "VirtServer.hpp"

class Server{

private:
	int							max_fd;
	int							is_closed;
	std::vector<VirtServer> &	virt_serv;
	std::vector<Client*>		client_session;
	fd_set						readset, writeset;
	size_t						serversNum;
	struct timeval				t;
	char						c;

public:
	typedef std::map<std::string, std::vector<std::string> >    map_type;
	Server(std::vector<VirtServer> & vec) : max_fd(0), is_closed(0), virt_serv(vec) {
		c = '\0';
		serversNum = virt_serv.size();
	}
	~Server(){};

	int launch();
	int setup();
	int set_prepare();
	void recv_msg(std::vector<Client*>::iterator it);
	void closeConnection(std::vector<Client*>::iterator it);
	int newSession();
	int clientSessionHandler(ErrorPages const & errPageMap);
	int createSocket(std::string const & host, int const port, int const & i);
	int postPutHandler(map_type const & data, std::vector<Client*>::iterator it);
	void getLocation(std::vector<Client*>::iterator it, map_type const &);
	int nameCompare(std::string const &, std::vector<VirtServer>::iterator & it);
	int error_headers(Request const &req);
	void chunkHandler(std::vector<Client*>::iterator & it,  char const *buf);
};


#endif
