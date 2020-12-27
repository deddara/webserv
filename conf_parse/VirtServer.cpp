/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VirtServer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awerebea <awerebea@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/17 10:36:59 by awerebea          #+#    #+#             */
/*   Updated: 2020/12/27 20:57:12 by awerebea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "VirtServer.hpp"


VirtServer::VirtServer() {
	pr_serverFields.insert("host");
	pr_serverFields.insert("listen");
	pr_serverFields.insert("server_name");
	pr_serverFields.insert("error_page");
	pr_serverFields.insert("limit_client_body");
	pr_serverFields.insert("location");
}

VirtServer::~VirtServer() {}

std::vector<Location *> const &		VirtServer::getLocation() const {
	return pr_location;
}

std::set<std::string> const &		VirtServer::getServerFields() const {
	return pr_serverFields;
}

std::multimap<std::string, std::vector<std::string> > const &
									VirtServer::getData() const {
	return pr_data;
}

void			VirtServer::setLocation(Location * location) {
	pr_location.push_back(location);
}

void			VirtServer::eraseLocation() {
	for (size_t i = 0; i < pr_location.size(); ++i) {
		delete pr_location[i];
	}
}

struct s_errExitData const &
				VirtServer::setDataPair(std::string const & key,
										std::vector<std::string> const & val) {
	pr_data.insert(std::make_pair(key, val));
	errStruct.code = 0;
	errStruct.word = "";
	return errStruct;
}
