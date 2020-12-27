/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awerebea <awerebea@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/17 11:33:30 by awerebea          #+#    #+#             */
/*   Updated: 2020/12/27 22:43:56 by awerebea         ###   ########.fr       */
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

// std::vector<std::string> const &	Location::getIndex() const {
//     return pr_index;
// }

// std::vector<std::string> const &	Location::getAllowMethods() const {
//     return pr_allowMethods;
// }

// std::string const &					Location::getRoot() const {
//     return pr_root;
// }

// std::string const &					Location::getAutoindex() const {
//     return pr_autoindex;
// }

std::set<std::string> const &	Location::getLocationFields() const {
	return pr_locationsFields;
}

void							Location::setPrefix(std::string const & str) {
	pr_prefix = str;
}

struct s_errExitData const &
				Location::setDataPair(std::string const & key,
										std::vector<std::string> const & val) {
	pr_data.insert(std::make_pair(key, val));
	errStruct.code = 0;
	errStruct.word = "";
	return errStruct;
}

std::multimap<std::string, std::vector<std::string> > const &
								Location::getData() const {
	return pr_data;
}

// void						Location::setIndex(std::string const & str) {
//     pr_index.push_back(str);
// }

// void						Location::setAllowMethods(std::string const & str) {
//     pr_allowMethods.push_back(str);
// }

// void						Location::setRoot(std::string const & str) {
//     pr_root = str;
// }

// void						Location::setAutoindex(std::string const & str) {
//     pr_autoindex = str;
// }

// void						Location::clearIndex() {
//     pr_index.clear();
// }

// void						Location::clearAllowMethods() {
//     pr_allowMethods.clear();
// }
