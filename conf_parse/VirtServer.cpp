/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VirtServer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awerebea <awerebea@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/17 10:36:59 by awerebea          #+#    #+#             */
/*   Updated: 2020/12/21 15:49:59 by awerebea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "VirtServer.hpp"


VirtServer::VirtServer(): pr_port(0), pr_limitClientBody(-1) {
	pr_serverFields.push_back("host");
	pr_serverFields.push_back("listen");
	pr_serverFields.push_back("server_name");
	pr_serverFields.push_back("error_page");
	pr_serverFields.push_back("limit_client_body");
	pr_serverFields.push_back("location");
}


VirtServer::~VirtServer() {}

std::string const &					VirtServer::getHost() const {
	return pr_host;
}

int const &							VirtServer::getPort() const {
	return pr_port;
}

std::vector<std::string> const &	VirtServer::getServerName() const {
	return pr_serverName;
}

std::map<int, std::string> const &	VirtServer::getErrorPage() const {
	return pr_errorPage;
}

int const &							VirtServer::getLimitClientBody() const {
	return pr_limitClientBody;
}

std::vector<Location> const &		VirtServer::getLocation() const {
	return pr_location;
}

std::vector<std::string> const &	VirtServer::getServerFields() const {
	return pr_serverFields;
}

void			VirtServer::setHost(std::string const & str) {
	pr_host = str;
}

void			VirtServer::setPort(int const & num) {
	pr_port = num;
}

void			VirtServer::setServerName(std::string const & str) {
	pr_serverName.push_back(str);

}

void			VirtServer::setErrorPage(int num, std::string const & str) {
	pr_errorPage.insert(std::make_pair(num, str));
}

void			VirtServer::setLimitClientBody(int const & num) {
	pr_limitClientBody = num;

}

void			VirtServer::setLocation(Location const & location) {
	pr_location.push_back(location);
}

void			VirtServer::clearServerName() {
	pr_serverName.clear();
}
