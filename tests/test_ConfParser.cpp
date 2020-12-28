#include "ConfParser.hpp"

int					main(int argc, char **argv)
{
	if (argc > 2)
	{
		std::cout << "Error: too much arguments" << std::endl;
		return 1;
	}
	ConfParser		confPars(argc == 2 ? argv[1] : "./webserv.conf");
	std::vector<VirtServer> const &	servers = confPars.getServer();
	std::vector<VirtServer>::const_iterator itServ = confPars.getServer().begin();

	for (int i = 1; itServ != confPars.getServer().end(); ++itServ, ++i) {
		std::cout << std::endl
			<< "------ Virtual server No. " << i << " ------" << std::endl;
		std::multimap<std::string, std::vector<std::string> >::const_iterator itServData =
			itServ->getData().begin();
		for (; itServData != itServ->getData().end(); ++itServData) {
			std::cout << itServData->first << ": ";
			std::vector<std::string>::const_iterator itVal = itServData->second.begin();
			for (; itVal != itServData->second.end(); ++itVal) {
				std::cout << *itVal << " ";
			}
			std::cout << std::endl;
		}
		std::vector<Location *> locations = itServ->getLocation();
		for (size_t j = 0; j < locations.size(); ++j) {
			std::cout << std::endl
				<< "-- Location: \"" << locations[j]->getPrefix() << "\" --" << std::endl;
			std::multimap<std::string, std::vector<std::string> >::const_iterator itLocData =
				locations[j]->getData().begin();
			for (; itLocData != locations[j]->getData().end(); ++itLocData) {
				std::cout << itLocData->first << ": ";
				std::vector<std::string>::const_iterator itVal = itLocData->second.begin();
				for (; itVal != itLocData->second.end(); ++itVal) {
					std::cout << *itVal << " ";
				}
			std::cout << std::endl;
			}
		}
	}
	return 0;
}
