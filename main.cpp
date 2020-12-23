#include "Server.hpp"
#include "includes.hpp"
#include "ConfParser.hpp"
#include "VirtServer.hpp"

int main(int argc, char **argv)
{

	ConfParser		confPars(argc == 2 ? argv[1] : "./webserv.conf");
	Server	serv(confPars.getServer());

	serv.setup();

	//биндим айпишники
	//поднимаем сервер
	if (serv.launch())
		return 1;

}
