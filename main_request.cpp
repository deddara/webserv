#include <iostream>

#include "Request.hpp"

int main() {
	std::string raw = "POST / HTTP/1.1\r\n"
	                  "User-Agent: curl/7.16.3 libcurl/7.16.3 OpenSSL/0.9.7l zlib/1.2.3\r\n"
	                  "Host: /\r\n"
	                  "Accept-language:en, mi\r\n"
	                  "accept-charset: utf-8\r\n"
	                  "Allow: GET, HEAD, POST\r\n"
	                  "Authorization:credentials\r\n"
	                  "Referer:/\r\n"
	                  "Content-Language: en\r\n"
	                  "Content-Length: 727\r\n"
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


//	std::cout << "----------------Check headers-----------------" << std::endl;
//	for (Request::map_type::const_iterator it = req.begin(); it != req.end(); ++it) {
//		if (req.is_valid_value(it->first)) {
//			if (it->first == "head") {
//				// проверка существования url = it->first - status = TODO
//				std::cout << it->first << ": " << it->second[0] << " " << it->second[1] << std::endl;
//			}
//			else if (it->first == "host") {
//				// проверка host = it->second[0] и порта = it->second[1], если порт указан
//				// status = TODO
//				std::cout << it->first << ": " << it->second[0];
//				if (it->second.size() == 2)
//					std::cout << ":" << it->second[1] << std::endl;
//				else
//					std::cout << std::endl;
//			}
//			else if (it->first == "accept-charset" ||
//					it->first == "accept-language" ||
//					it->first == "allow" ||
//					it->first == "authorization" ||
//					it->first == "referer" ||
//					it->first == "user-agent") {
//				// проверка ?  - нужна? statuses = TODO
//				std::cout << it->first << ": " << it->second[0] << std::endl;
//			}
//			else if (it->first == "content-length") {
//				// сравнение значения хедера и длины поля = TODO
//				std::cout << it->first << ": " << std::stod(it->second[0]) << std::endl;
//				std::cout << "  body length = " << req.get_body().size() << std::endl;
//			}
//		}
//	}
	return 0;
}
