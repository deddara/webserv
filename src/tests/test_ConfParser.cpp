#include "ConfParser.hpp"

int					main(int argc, char **argv)
{
	if (argc > 2)
	{
		std::cout << "Error: too much arguments" << std::endl;
		return 1;
	}
	ConfParser		confPars(argc == 2 ? argv[1] : "./webserv.conf");
	std::cout << confPars.getPath() << std::endl; //INFO
	return 0;
}
