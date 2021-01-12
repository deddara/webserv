#include <iostream>

#include "Request.hpp"


//
size_t pos_of_char(unsigned char c) {
	if (c >= 'A' && c <= 'Z')
		return c - 'A';
	else if (c >= 'a' && c <= 'z')
		return c - 'a' + ('Z' - 'A') + 1;
	else if (c >= '0' && c <= '9')
		return c - '0' + ('Z' - 'A') + ('z' - 'a') + 2;
	else if (c == '+' || c == '-')
		return 62;
	else if (c == '/' || c == '_')
		return 63;
	else
		throw std::runtime_error("Auth credentials are not valid");
}

std::string decode(std::string const &str) {
	if (str.empty())
		return "";
	size_t pos = 0;
	std::string ret;
	while (pos < str.size()) {
		size_t pos1 = pos_of_char(str[pos + 1]);
		ret.push_back(((pos_of_char(str[pos+0])) << 2) + ((pos1 & 0x30 ) >> 4));
		if (str[pos + 2] != '=' && str[pos + 2] != '.') {
			size_t pos2 = pos_of_char(str[pos + 2] );
			ret.push_back(((pos1 & 0x0f) << 4) + ((pos2 & 0x3c) >> 2));
			if (str[pos + 3] != '=' && str[pos + 3] != '.') {
				ret.push_back(((pos2 & 0x03) << 6) + pos_of_char(str[pos + 3]));
			}
		}
		pos += 4;
	}
	return ret;
}

int main() {
	std::string raw = "POST /hello.txt HTTP/1.1\r\n"
	                  "User-Agent: curl/7.16.3 libcurl/7.16.3 OpenSSL/0.9.7l zlib/1.2.3\r\n"
	                  "Host: www.example.com\r\n"
	                  "Accept-language:en, mi\r\n"
	                  "accept-charset: utf-8\r\n"
	                  "Allow:\r\n"
	                  "Authorization: Basic dXNlcjpwYXNzd29yZA==\r\n"
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
	                  "  <p>Разум — это Будда, а прекращение умозрительного мышления — это путь. \n"
	                  "  Перестав мыслить понятиями и размышлять о путях существования и небытия, \n"
	                  "  о душе и плоти, о пассивном и активном и о других подобных вещах, \n"
	                  "  начинаешь осознавать, что разум — это Будда, \n"
	                  "  что Будда — это сущность разума, \n"
	                  "  и что разум подобен бесконечности.</p>\n"
	                  " </body> \n"
	                  "</html>";

	Request req(raw);

//	std::cout << "-------------Map elements-------------" << std::endl;
//	for (Request::map_type::const_iterator it = req.begin(); it != req.end(); ++it) {
//		if (req.is_valid_value(it->first))
//			std::cout << it->first << ":" << it->second.front() << std::endl;
//	}
//
//	std::cout << "-----------------Body-----------------" << std::endl;
//	std::cout << req.get_body_pos() << std::endl;
//
//	std::cout << "----------------Error-----------------" << std::endl;
//	std::cout << "Request is bad: " << req.error() << std::endl;


	// Type + Credentials
	std::string type_creds = req.getData().find("authorization")->second[0];
	// Credentials
	std::string auth = type_creds.substr(type_creds.find(' ') + 1);

	std::string decoded_creds;

	// Check for validity of credentials
	try {
		// decode - function for decode from base64_string to std::string
		decoded_creds = decode(auth);
	} catch(std::exception &e) {
		std::cerr << e.what() << std::endl;
		// some actions: response(401)?
	}

	std::cout << "Credentials: " << decoded_creds;
	return 0;
}
