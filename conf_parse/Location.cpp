/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awerebea <awerebea@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/17 11:33:30 by awerebea          #+#    #+#             */
/*   Updated: 2021/01/09 18:43:51 by awerebea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

Location::Location() {
	pr_locationsFields.insert("index");
	pr_locationsFields.insert("allow_methods");
	pr_locationsFields.insert("root");
	pr_locationsFields.insert("autoindex");
	pr_locationsFields.insert("cgi_ext");
	pr_locationsFields.insert("cgi_bin");
	pr_locationsFields.insert("limit_client_body");
}

Location::~Location() {}

std::string const &				Location::getPrefix() const {
	return pr_prefix;
}

std::set<std::string> const &	Location::getLocationFields() const {
	return pr_locationsFields;
}

void							Location::setPrefix(std::string const & str) {
	pr_prefix = str;
}

std::multimap<std::string, std::vector<std::string> > const &
								Location::getData() const {
	return pr_data;
}

struct s_errExitData const &	Location::setErrStruct(int code,
														std::string word) {
	errStruct.code = code;
	errStruct.word = word;
	return errStruct;
}

struct s_errExitData const &	Location::setDataPair(std::string const & key,
										std::vector<std::string> & val) {
	if (key == "index") {
		if (pr_data.count(key)) {
			pr_data.erase(key);
		}
		for (size_t i = 0; i < val.size(); ++i) {
			if (checkSuspiciousSymbols(val[i])) {
				return (setErrStruct(7, val[i]));
			}
		}
	}
	if (key == "allow_methods") {
		if (pr_data.count(key)) {
			pr_data.erase(key);
		}
		for (size_t i = 0; i < val.size(); ++i) {
			if (checkSuspiciousSymbols(val[i])) {
				return (setErrStruct(7, val[i]));
			}
			val[i] = toUpper(val[i]);
			if (val[i] != "GET" && val[i] != "HEAD" && val[i] != "POST")
				return (setErrStruct(15, val[i]));
		}
	}
	if (key == "root") {
		if (pr_data.count(key)) {
			pr_data.erase(key);
		}
		if (val.size() != 1) {
			return (setErrStruct(3, key));
		}
	}
	if (key == "autoindex") {
		if (pr_data.count(key)) {
			pr_data.erase(key);
		}
		if (val.size() != 1) {
			return (setErrStruct(3, key));
		}
		val[0] = toLower(val[0]);
		if (val[0] != "on" && val[0] != "off") {
			return (setErrStruct(14, val[0]));
		}
	}
	if (key == "cgi_ext") {
		if (pr_data.count(key)) {
			pr_data.erase(key);
		}
		for (size_t i = 0; i < val.size(); ++i) {
			if (checkSuspiciousSymbols(val[i]) || val[i][0] != '.') {
				return (setErrStruct(23, val[i]));
			}
		}
	}
	if (key == "cgi_bin") {
		if (pr_data.count(key)) {
			pr_data.erase(key);
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
			return (setErrStruct(21, val[0]));
		}
	}
	pr_data.insert(std::make_pair(key, val));
	return setErrStruct(0, "");
}

struct s_errExitData const &	Location::checkCgiSettings() {
	if (pr_data.count("cgi_ext") != pr_data.count("cgi_bin")) {
		return (setErrStruct(24, pr_prefix));
	}
	if (pr_data.count("cgi_ext")) {
		std::multimap<std::string, std::vector<std::string> >::const_iterator
								itExt = pr_data.find("cgi_ext");

		std::multimap<std::string, std::vector<std::string> >::const_iterator
								itBin = pr_data.find("cgi_bin");

		if (itExt->second.size() != itBin->second.size()) {
			return (setErrStruct(24, pr_prefix));
		}
	}
	return setErrStruct(0, "");
}
