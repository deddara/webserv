/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorPages.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awerebea <awerebea@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/25 17:26:04 by awerebea          #+#    #+#             */
/*   Updated: 2020/12/26 12:45:45 by awerebea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ErrorPages.hpp"

ErrorPages::ErrorPages() {
	std::vector<std::string>		val(2, "");
	val[1] = "OK";
	errorPageTemplates[200] = val;
	valClear(val);
	val[1] = "Moved Permanently";
	errorPageTemplates[301] = val;
	valClear(val);
	val[1] = "Found";
	errorPageTemplates[302] = val;
	valClear(val);
	val[0] =
		"<html>\n"														\
		"<head><title>400 Bad Request</title></head>\n"					\
		"<body bgcolor=\"white\">\n"									\
		"<center><h1>400 Bad Request</h1></center>\n"					\
		"<hr><center>webserv/0.1a</center>\n"							\
		"</body>\n"														\
		"</html>\n";
	val[1] = "Bad Request";
	errorPageTemplates[400] = val;
	valClear(val);
	val[0] =
		"<html>\n"														\
		"<head><title>403 Forbidden</title></head>\n"					\
		"<body bgcolor=\"white\">\n"									\
		"<center><h1>403 Forbidden</h1></center>\n"						\
		"<hr><center>webserv/0.1a</center>\n"							\
		"</body>\n"														\
		"</html>\n";
	val[1] = "Forbidden";
	errorPageTemplates[403] = val;
	valClear(val);
	val[0] =
		"<html>\n"														\
		"<head><title>404 Not Found</title></head>\n"					\
		"<body bgcolor=\"white\">\n"									\
		"<center><h1>404 Not Found</h1></center>\n"						\
		"<hr><center>webserv/0.1a</center>\n"							\
		"</body>\n"														\
		"</html>\n";
	val[1] = "Not Found";
	errorPageTemplates[404] = val;
	valClear(val);
	val[0] =
		"<html>\n"														\
		"<head><title>500 Internal Server Error</title></head>\n"		\
		"<body bgcolor=\"white\">\n"									\
		"<center><h1>500 Internal Server Error</h1></center>\n"			\
		"<hr><center>webserv/0.1a</center>\n"							\
		"</body>\n"														\
		"</html>\n";
	val[1] = "Internal Server Error";
	errorPageTemplates[500] = val;
	valClear(val);
}

ErrorPages::~ErrorPages() {}

void					ErrorPages::valClear(std::vector<std::string> & val) {
	val.clear();
	if (!val.empty()) {
		std::cout << "Error: system failure" << std::endl;
		exit(1);
	}
}

std::map<int, std::vector<std::string> > const &
						ErrorPages::getErrorPageTemplates() const {
	return errorPageTemplates;
}
