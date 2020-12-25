/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorPages.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awerebea <awerebea@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/25 17:26:04 by awerebea          #+#    #+#             */
/*   Updated: 2020/12/25 17:59:33 by awerebea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ErrorPages.hpp"

ErrorPages::ErrorPages() {}

ErrorPages::ErrorPages(std::map<int, std::string> & errorPageTemplates) {
	errorPageTemplates[400] =
		"<html>"														\
		"<head><title>400 Bad Request</title></head>"					\
		"<body bgcolor=\"white\">"										\
		"<center><h1>400 Bad Request</h1></center>"						\
		"<hr><center>webserv/0.1a</center>"								\
		"</body>"														\
		"</html>";
	errorPageTemplates[403] =
		"<html>"														\
		"<head><title>403 Forbidden</title></head>"						\
		"<body bgcolor=\"white\">"										\
		"<center><h1>403 Forbidden</h1></center>"						\
		"<hr><center>webserv/0.1a</center>"								\
		"</body>"														\
		"</html>";
	errorPageTemplates[404] =
		"<html>"														\
		"<head><title>404 Not Found</title></head>"						\
		"<body bgcolor=\"white\">"										\
		"<center><h1>404 Not Found</h1></center>"						\
		"<hr><center>webserv/0.1a</center>"								\
		"</body>"														\
		"</html>";
	errorPageTemplates[500] =
		"<html>"														\
		"<head><title>500 Internal Server Error</title></head>"			\
		"<body bgcolor=\"white\">"										\
		"<center><h1>500 Internal Server Error</h1></center>"			\
		"<hr><center>webserv/0.1a</center>"								\
		"</body>"														\
		"</html>";
}

ErrorPages::~ErrorPages() {}
