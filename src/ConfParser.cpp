/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfParser.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awerebea <awerebea@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 11:40:21 by awerebea          #+#    #+#             */
/*   Updated: 2020/12/15 12:12:58 by awerebea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfParser.hpp"

ConfParser::ConfParser(): _filePath("./webserv.conf") {}

ConfParser::ConfParser(std::string const & fpath): _filePath(fpath) {}

ConfParser::ConfParser(ConfParser const & other) { *this = other; }

ConfParser::~ConfParser() {}

ConfParser &			ConfParser::operator=(ConfParser const & other)
{
	if (this != &other)
		_filePath = other._filePath;
	return *this;
}

std::string const &		ConfParser::getPath() const
{
	return _filePath;
}
