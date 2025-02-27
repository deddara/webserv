#include <iostream>

#include "Request.hpp"

int main() {
	std::string raw = "POST /hello.txt HTTP/1.1\r\n"
//	                  "User-Agent: curl/7.16.3 libcurl/7.16.3 OpenSSL/0.9.7l zlib/1.2.3\r\n"
	                  "Host: www.example.com\r\n"
//	                  "Accept-language:en, mi\r\n"
//	                  "accept-charset: utf-8\r\n"
//	                  "Allow:\r\n"
//	                  "Authorization:\r\n"
//	                  "Referer:\r\n"
//	                  "Content-Language: en\r\n"
//	                  "Content-Length:\r\n"
//	                  "Content-Location:\r\n"
//	                  "Content-Type:\r\n"
	                  "\r\n"
	                  "<!DOCTYPE html>\n"
	                  "<html>\n"
	                  " <head>\n"
	                  "   <title>!DOCTYPE</title>\n"
	                  "   <meta charset=\"utf-8\">\n"
	                  " </head>\n"
	                  " <body>\n"
	                  "  <p>Разум — это Будда, а прекращение умозрительного мышления — это путь. \n"
	                  "  Перестав мыслить понятиями и размышлять о путях существования и небытия, \n"
	                  "  о душе и плоти, о пассивном и активном и о других подобных вещах, \n"
	                  "  начинаешь осознавать, что разум — это Будда, \n"
	                  "  что Будда — это сущность разума, \n"
	                  "  и что разум подобен бесконечности.</p>\n"
	                  " </body> \n"
	                  "</html>";

	Request req(raw);

	std::cout << "-------------Map elements-------------" << std::endl;
	for (Request::map_type::const_iterator it = req.begin(); it != req.end(); ++it) {
		if (req.is_valid_value(it->first))
			std::cout << it->first << ":" << it->second.front() << std::endl;
	}

	std::cout << "-----------------Body-----------------" << std::endl;
	std::cout << req.get_body_pos() << std::endl;

	std::cout << "----------------Error-----------------" << std::endl;
	std::cout << "Request is bad: " << req.error() << std::endl;

	return 0;
}
