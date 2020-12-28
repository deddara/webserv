/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awerebea <awerebea@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/17 11:33:30 by awerebea          #+#    #+#             */
/*   Updated: 2020/12/28 01:11:34 by awerebea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

Location::Location() {
	pr_locationsFields.insert("index");
	pr_locationsFields.insert("allow_methods");
	pr_locationsFields.insert("root");
	pr_locationsFields.insert("autoindex");
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
	pr_data.insert(std::make_pair(key, val));
	return setErrStruct(0, "");
}
