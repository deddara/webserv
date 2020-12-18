#include "Server.hpp"
#include "includes.hpp"
#include "ConfParser.hpp"
#include "VirtServer.hpp"

int main(int argc, char **argv)
{
	Server	serv;

	ConfParser		confPars(argc == 2 ? argv[1] : "./webserv.conf");
	std::vector<VirtServer> const &	servers = confPars.getServer();
	size_t			serversNum = confPars.getServer().size();

	//биндим айпишники
	for (size_t i = 0; i < serversNum; ++i){
		if (serv.setup(servers[i].getHost(), servers[i].getPort()))
			return (1);
	}
	//поднимаем сервер
	if (serv.launch())
		return 1;

}
