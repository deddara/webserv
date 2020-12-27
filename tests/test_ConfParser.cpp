#include "ConfParser.hpp"
#include "VirtServer.hpp"

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

	for (; itServ != confPars.getServer().end(); ++itServ) {
		std::multimap<std::string, std::vector<std::string> >::const_iterator itData =
			itServ->getData().begin();
		for (; itData != itServ->getData().end(); ++itData) {
			std::cout << itData->first << ": ";
			std::vector<std::string>::const_iterator itVal = itData->second.begin();
			for (; itVal != itData->second.end(); ++itVal) {
				std::cout << *itVal << " ";
			}
			std::cout << std::endl;
		}
	}

	// for (size_t i = 0; i < serversNum; ++i)
	// {
	//     std::cout << std::endl
	//         << "------ Virtual server No. " << i + 1 << " ------" << std::endl;
	//     std::cout << "Host\t\t: " << servers[i].getHost() << std::endl;
	//     std::cout << "Listen port\t: " << servers[i].getPort() << std::endl;
	//     std::cout << "Server name(s)\t: ";
	//     for (size_t j = 0; j < servers[i].getServerName().size(); ++j)
	//         std::cout << servers[i].getServerName()[j] << " ";
	//     std::cout << std::endl;
	//     for (std::map<int, std::string>::const_iterator it =
	//             servers[i].getErrorPagePath().begin();
	//             it != servers[i].getErrorPagePath().end(); it++)
	//         std::cout << "Error page\t: error: " << it->first
	//             << " filepath: " << it->second << std::endl;
	//     std::cout << "Limit body\t: " << servers[i].getLimitClientBody()
	//         << std::endl;
	//     for (size_t j = 0; j < servers[i].getLocation().size(); ++j)
	//     {
	//         std::cout << std::endl
	//             << "-- Location No. " << j + 1 << " --" << std::endl;
	//         std::cout << "Prefix\t\t: "
	//             << servers[i].getLocation()[j]->getPrefix() << std::endl;
	//         std::cout << "Index(es)\t: ";
	//         for (size_t k = 0; k <
	//                 servers[i].getLocation()[j]->getIndex().size(); ++k)
	//         std::cout << servers[i].getLocation()[j]->getIndex()[k] << " ";
	//         std::cout << std::endl;
	//         std::cout << "Allow methods\t: ";
	//         for (size_t k = 0; k <
	//                 servers[i].getLocation()[j]->getAllowMethods().size(); ++k)
	//         std::cout << servers[i].getLocation()[j]->getAllowMethods()[k]
	//             << " ";
	//         std::cout << std::endl;
	//         std::cout << "Root\t\t: "
	//             << servers[i].getLocation()[j]->getRoot() << std::endl;
	//         std::cout << "Autoindex\t: "
	//             << servers[i].getLocation()[j]->getAutoindex() << std::endl;
	//     }
	// }
	return 0;
}
