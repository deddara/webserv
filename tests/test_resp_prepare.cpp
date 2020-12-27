#include "Client.hpp"
#include "ConfParser.hpp"
#include "ErrorPages.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "Server.hpp"
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
	size_t			serversNum = confPars.getServer().size();

	ErrorPages		errPageMap;

	// for (size_t i = 0; i < serversNum; ++i) {
	//     std::cout << std::endl
	//         << "------ Virtual server No. " << i + 1 << " ------" << std::endl;
	//     std::cout << "Host\t\t: " << servers[i].getHost() << std::endl;
	//     std::cout << "Listen port\t: " << servers[i].getPort() << std::endl;
	//     std::cout << "Server name(s)\t: ";
	//     for (size_t j = 0; j < servers[i].getServerName().size(); ++j)
	//         std::cout << servers[i].getServerName()[j] << " ";
	//     std::cout << std::endl;
	//     for (std::map<int, std::string>::const_iterator it =
	//             servers[i].getErrorPage().begin();
	//             it != servers[i].getErrorPage().end(); it++)
	//         std::cout << "Error page\t: error: " << it->first
	//             << " filepath: " << it->second << std::endl;
	//     std::cout << "Limit body\t: " << servers[i].getLimitClientBody()
	//         << std::endl;
	//     for (size_t j = 0; j < servers[i].getLocation().size(); ++j) {
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

	std::string raw = "GET /site/index.html HTTP/1.1\r\n"
	"User-Agent: curl/7.16.3 libcurl/7.16.3 OpenSSL/0.9.7l zlib/1.2.3\r\n"
	"Host: www.example.com\r\n"
	"Accept-language:en, mi\r\n"
	"accept-charset: utf-8\r\n"
	"Allow:\r\n"
	"Authorization:\r\n"
	"Referer:\r\n"
	"Content-Language: en\r\n"
	"Content-Length:\r\n"
	"Content-Location:\r\n"
	"Content-Type:\r\n"
	"\r\n"
	"<!DOCTYPE html>\n"
	"<html>\n"
	" <head>\n"
	"   <title>!DOCTYPE</title>\n"
	"   <meta charset=\"utf-8\">\n"
	" </head>\n"
	" <body>\n"
	"  <p>Разум — это Будда, а прекращение мышления — это путь. \n"
	"  Перестав мыслить понятиями и размышлять о путях и небытия, \n"
	"  о душе и плоти, о пассивном и активном и о других подобных вещах, \n"
	"  начинаешь осознавать, что разум — это Будда, \n"
	"  что Будда — это сущность разума, \n"
	"  и что разум подобен бесконечности.</p>\n"
	" </body> \n"
	"</html>";
	Request			req(raw);
	// std::cout << "-------------Map elements-------------" << std::endl;
	// for (Request::map_type::const_iterator it = req.begin(); it != req.end(); ++it) {
	//     if (req.is_valid_value(it->first))
	//         std::cout << it->first << ":" << it->second.front() << std::endl;
	// }

	// std::cout << "-----------------Body-----------------" << std::endl;
	// std::cout << req.get_body_pos() << std::endl;

	// std::cout << "----------------Error-----------------" << std::endl;
	// std::cout << "Request is bad: " << req.error() << std::endl;

	int				status;
	Response		res;
	res.setErrorPagePath(&confPars.getServer()[0].getErrorPagePath());
	res.setErrorPageTempl(&errPageMap.getErrorPageTemplates());
	res.setLocation(confPars.getServer()[0].getLocation());
	res.responsePrepare(status, &req.getData());
	return 0;
}
