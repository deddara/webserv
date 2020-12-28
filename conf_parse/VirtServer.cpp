/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VirtServer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awerebea <awerebea@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/17 10:36:59 by awerebea          #+#    #+#             */
/*   Updated: 2020/12/28 11:15:07 by awerebea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "VirtServer.hpp"

VirtServer::VirtServer(): fd(0) {
	pr_serverFields.insert("host");
	pr_serverFields.insert("listen");
	pr_serverFields.insert("server_name");
	pr_serverFields.insert("error_page");
	pr_serverFields.insert("limit_client_body");
	pr_serverFields.insert("location");
}

VirtServer::~VirtServer() {}

std::vector<Location *> const &	VirtServer::getLocation() const {
	return pr_location;
}

std::set<std::string> const &	VirtServer::getServerFields() const {
	return pr_serverFields;
}

std::multimap<std::string, std::vector<std::string> > const &
								VirtServer::getData() const {
	return pr_data;
}

void							VirtServer::setLocation(Location * location) {
	pr_location.push_back(location);
}

void			VirtServer::eraseLocation() {
	for (size_t i = 0; i < pr_location.size(); ++i) {
		delete pr_location[i];
	}
}

struct s_errExitData const &	VirtServer::setErrStruct(int code,
														std::string word) {
	errStruct.code = code;
	errStruct.word = word;
	return errStruct;
}

struct s_errExitData const &	VirtServer::setDataPair(std::string const & key,
										std::vector<std::string> & val) {
	std::pair<
		std::multimap<std::string, std::vector<std::string> >::iterator,
		std::multimap<std::string, std::vector<std::string> >::iterator>
								equalRange;
	std::multimap<std::string, std::vector<std::string> >::iterator
								it;

	if (key == "host") {
		if (pr_data.count(key)) {
			pr_data.erase(key);
		}
	}
	if (key == "listen") {
		if (pr_data.count(key)) {
			pr_data.erase(key);
		}
		if (val.size() != 1) {
			return (setErrStruct(3, key));
		}
		if (checkStringInt(val[0])) {
			return (setErrStruct(5, val[0]));
		}
		int num = ft_atoi(val[0].c_str());
		if (num < 0 || num > 65535) {
			return (setErrStruct(6, val[0]));
		}
	}
	if (key == "server_name") {
		if (pr_data.count(key)) {
			pr_data.erase(key);
		}
		for (size_t i = 0; i < val.size(); ++i) {
			if (checkSuspiciousSymbols(val[i])) {
				return (setErrStruct(7, val[i]));
			}
		}
	}
	if (key == "error_page") {
		if (val.size() != 2) {
			return (setErrStruct(3, key));
		}
		if (checkStringInt(val[0])) {
			return (setErrStruct(5, val[0]));
		}
		int num = ft_atoi(val[0].c_str());
		if (num < 100 || num > 599) {
			return (setErrStruct(19, val[0]));
		}

		// erase duplicates of 'error_page' settings for the same error
		equalRange = pr_data.equal_range(key);
		it = equalRange.first;
		for (; it != equalRange.second; ++it) {
			if (ft_atoi(it->second[0].c_str()) == ft_atoi(val[0].c_str())) {
				pr_data.erase(it);
			}
		}
	}
	if (key == "limit_client_body") {
		if (pr_data.count(key)) {
			pr_data.erase(key);
		}
		if (val.size() != 1) {
			return (setErrStruct(3, key));
		}
		if (checkStringInt(val[0])) {
			return (setErrStruct(10, val[0]));
		}
		int num = ft_atoi(val[0].c_str());
		if (num < 0) {
			return (setErrStruct(11, val[0]));
		}
	}
	pr_data.insert(std::make_pair(key, val));
	return setErrStruct(0, "");
}

void							VirtServer::setFd(const int &num) {
	fd = num;
}

const int &						VirtServer::getFd() const {
	return fd;
}

std::string const &				VirtServer::getHost() const {
	return pr_data.find("host")->second[0];
}

int const						VirtServer::getPort() const {
	return ft_atoi(pr_data.find("listen")->second[0].c_str());
}

std::vector<std::string> const &	VirtServer::getServerName() const {
	return pr_data.find("server_name")->second;
}

int const						VirtServer::getLimitClientBody() const {
	return ft_atoi(pr_data.find("limit_client_body")->second[0].c_str());
}

std::map<int, std::string> const &	VirtServer::getErrorPagePath() {
	std::pair<
		std::multimap<std::string, std::vector<std::string> >::iterator,
		std::multimap<std::string, std::vector<std::string> >::iterator>
								equalRange;
	std::multimap<std::string, std::vector<std::string> >::iterator
								it;

	equalRange = pr_data.equal_range("error_page");
	it = equalRange.first;
	for (; it != equalRange.second; ++it) {
		errPgPathMap.insert(std::make_pair(ft_atoi(it->second[0].c_str()),
					it->second[1]));
	}
	return errPgPathMap;
}
