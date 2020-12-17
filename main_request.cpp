#include <iostream>

#include "Request.hpp"

int main() {
	std::string raw = "POST /hello.txt HTTP/1.1\n"
	                  "User-Agent: curl/7.16.3 libcurl/7.16.3 OpenSSL/0.9.7l zlib/1.2.3\n"
	                  "Host: www.example.com\n"
	                  "Accept-language:en, mi\n"
	                  "accept-charset: utf-8\n"
	                  "Allow:\n"
	                  "Authorization:\n"
	                  "Referer:\n"
	                  "Content-Language: en\n"
	                  "Content-Length:\n"
	                  "Content-Location:\n"
	                  "Content-Type:\n"
	                  "\n"
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
	std::cout << req.get_body() << std::endl;

	std::cout << "----------------Error-----------------" << std::endl;
	std::cout << "Request is bad: " << req.error() << std::endl;


	std::cout << "----------------Check headers-----------------" << std::endl;
	for (Request::map_type::const_iterator it = req.begin(); it != req.end(); ++it) {
		if (req.is_valid_value(it->first)) {
			if (it->first == "head") {
				// проверка существования url = it->first
				std::cout << it->first << ": " << it->second[0] << " " << it->second[1] << std::endl;
			}
			else if (it->first == "host") {
				// проверка host = it->second[0] и порта = it->second[1], если порт указан, иначе там мусор
				std::cout << it->first << ": " << it->second[0];
				if ()
				<< ":" << it->second.size() << std::endl;
			}
		}
	}
	return 0;
}
