/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awerebea <awerebea@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/17 11:33:30 by awerebea          #+#    #+#             */
/*   Updated: 2020/12/17 14:35:43 by awerebea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

Location::Location(): pr_autoindex("off") {
	pr_locationsFields.push_back("index");
	pr_locationsFields.push_back("allow_methods");
	pr_locationsFields.push_back("root");
	pr_locationsFields.push_back("autoindex");
}

Location::~Location() {}

std::string const &					Location::getPrefix() const {
	return pr_prefix;
}

std::vector<std::string> const &	Location::getIndex() const {
	return pr_index;
}

std::vector<std::string> const &	Location::getAllowMethods() const {
	return pr_allowMethods;
}

std::string const &					Location::getRoot() const {
	return pr_root;
}

std::string const &					Location::getAutoindex() const {
	return pr_autoindex;
}

std::vector<std::string> const &	Location::getLocationFields() const {
	return pr_locationsFields;
}

void						Location::setPrefix(std::string const & str) {
	pr_prefix = str;
}

void						Location::setIndex(std::string const & str) {
	pr_index.push_back(str);
}

void						Location::setAllowMethods(std::string const & str) {
	pr_allowMethods.push_back(str);
}

void						Location::setRoot(std::string const & str) {
	pr_root = str;
}

void						Location::setAutoindex(std::string const & str) {
	pr_autoindex = str;
}

void						Location::clearIndex() {
	pr_index.clear();
}

void						Location::clearAllowMethods() {
	pr_allowMethods.clear();
}
