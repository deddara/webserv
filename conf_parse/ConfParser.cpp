/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfParser.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awerebea <awerebea@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 11:40:21 by awerebea          #+#    #+#             */
/*   Updated: 2020/12/22 13:39:51 by awerebea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfParser.hpp"

int					checkStringInt(std::string const &word) {
	size_t			len = word.length();
	for (size_t i = 0; i < len; ++i)
	{
		if (!std::isdigit(word[i]))
			return 1;
	}
	return 0;
}

int					checkSuspiciousSymbols(std::string const &word) {
	size_t			len = word.length();
	for (size_t i = 0; i < len; ++i)
	{
		if (word[i] == '/')
			return 1;
	}
	return 0;
}

int					checkBrackets(char c) {
	if (c == '{')
		return 1;
	if (c == '}')
		return 2;
	return 0;
}

ConfParser::ConfParser(std::string const & fpath): pr_pos(0) {
	pr_data = readConfFile(fpath);
	pr_len = pr_data.length();
	parser();
}

ConfParser::~ConfParser() {}

void				ConfParser::skipSpaceComm() {
	size_t			newPos = 0;
	while (pr_pos < pr_len - 1)
	{
		while (isspace(pr_data[pr_pos]))
			(pr_pos)++;
		if (pr_data[pr_pos] == '#')
		{
			if ((newPos = pr_data.find("\n", pr_pos)) != std::string::npos)
				pr_pos = newPos;
			else
			{
				pr_pos = pr_len - 1;
				break ;
			}
		}
		else
			break ;
	}
}

std::string			ConfParser::toLower(std::string str) {
	for (size_t i = 0; i < str.length(); i++)
	{
		if (str[i] >= 'A' && str[i] <= 'Z')
			str[i] += 32;
	}
	return str;
}

std::string			ConfParser::toUpper(std::string str) {
	for (size_t i = 0; i < str.length(); i++)
	{
		if (str[i] >= 'a' && str[i] <= 'z')
			str[i] -= 32;
	}
	return str;
}

std::string			ConfParser::pickWord() {
	std::string		word;
	while (pr_pos < pr_len - 1 && !isspace(pr_data[pr_pos]) \
			&& pr_data[pr_pos] != ';' \
			&& pr_data[pr_pos] != '{' && pr_data[pr_pos] != '}')
		word.push_back(pr_data[(pr_pos)++]);
	return word;
}

std::string			ConfParser::readConfFile(std::string const & fpath) {
	int				fd = 0;
	int				ret = 0;
	char			buf[128];
	std::string		data;

	if ((fd = open(fpath.c_str(), O_RDONLY)) < 0)
		errorExit(0, "");

	while ((ret = read(fd, buf, 128)) > 0)
	{
		buf[ret] = '\0';
		data.append(buf);
	}
	if (ret < 0)
		errorExit(1, "");
	return data;
}

void				ConfParser::errorExit(int code, std::string const & word) {
	std::string		errors[20] = {
		"Error: config file is unavailable",
		"Error: read fails",
		"Error: config file syntax error",
		"Error: invalid number of arguments in \"" + word + "\" directive",
		"Error: unknown directive \"" + word + "\"",
		"Error: host not found in \"" + word + "\" of the \"listen\" directive",
		"Error: invalid port in \"" + word + "\" of the \"listen\" directive",
		"Error: server name \"" + word + "\" has suspicious symbols",
		"Error: directive \"" + word + "\" is not terminated by \";\"",
		"Error: unexpected \"{\"",
		"Error: number \"" + word + "\" has suspicious symbols",
		"Error: limit client body \"" + word + "\" overflows",
		"Error: location prefix is missing",
		"Error: unexpected \"}\"",
		"Error: invalid value \"" + word + "\" of \"autoindex\" directive",
		"Error: unsupported value \"" + word
			+ "\" of \"allow_methods\" directive",
		"Error: unexpected \";\"",
		"Error: the value of the \"" + word + "\" directive is not set",
		"Error: duplicate server \"" + word + "\" found",
		"Error: invalid error number \"" + word
			+ "\" of \"error page\" directive",
	};
	std::cout << errors[code] << std::endl;
	for (size_t i = 0; i < pr_server.size(); ++i) {
		pr_server[i].eraseLocation();
	}
	exit(1);
}

VirtServer			ConfParser::serverBlockProc() {
	skipSpaceComm();
	if (pr_data[pr_pos] == ';')
		errorExit(16, "");
	if (checkBrackets(pr_data[pr_pos]) != 1)
		errorExit(2, "");
	pr_pos++;

	VirtServer		server;
	std::string		key;
	std::string		val;
	int				ret = 0;
	int				i = 0;

	skipSpaceComm();
	while (checkBrackets(pr_data[pr_pos]) != 2)
	{
		if (pr_data[pr_pos] == ';')
			errorExit(16, "");
		ret = 0;
		if (checkBrackets(pr_data[pr_pos]) == 1)
			errorExit(9, "");
		key = pickWord();
		i = 0;
		for (; i < 6; ++i)
		{
			if (toLower(key) == server.getServerFields()[i])
			{
				skipSpaceComm();
				if (pr_data[pr_pos] == ';')
					errorExit(3, key);
				break ;
			}
		}
		if (pr_data[pr_pos] == ';')
			errorExit(16, "");
		if (i == 6)
			errorExit(4, key);
		skipSpaceComm();
		if (checkBrackets(pr_data[pr_pos]) == 1)
			errorExit(9, "");
		val = pickWord();
		if (i == 0)
		{
			server.setHost(val);
			skipSpaceComm();
			if ((ret = checkBrackets(pr_data[pr_pos])))
				errorExit(ret == 1 ? 9 : 8, key);
			if (pr_data[pr_pos] != ';')
				errorExit(3, key);
			pr_pos++;
		}
		else if (i == 1)
		{
			if (checkStringInt(val))
				errorExit(5, val);
			int num = static_cast<int>(std::stold(val));
			if (num < 0 || num > 65535)
				errorExit(6, val);
			server.setPort(num);
			skipSpaceComm();
			if ((ret = checkBrackets(pr_data[pr_pos])))
				errorExit(ret == 1 ? 9 : 8, key);
			if (pr_data[pr_pos] != ';')
				errorExit(3, key);
			pr_pos++;
		}
		else if (i == 2)
		{
			if (server.getServerName().size())
				server.clearServerName();
			if (checkBrackets(pr_data[pr_pos]) == 1)
				errorExit(9, "");
			if (checkSuspiciousSymbols(val))
				errorExit(7, val);
			server.setServerName(val);
			skipSpaceComm();
			while (pr_data[pr_pos] != ';')
			{
				if ((ret = checkBrackets(pr_data[pr_pos])))
					errorExit(ret == 1 ? 9 : 8, key);
				val = pickWord();
				if (checkSuspiciousSymbols(val))
					errorExit(7, val);
				server.setServerName(val);
				skipSpaceComm();
			}
			pr_pos++;
		}
		if (i == 3)
		{
			if (checkStringInt(val))
				errorExit(5, val);
			int num = static_cast<int>(std::stold(val));
			if (num < 100 || num > 599)
				errorExit(19, val);
			skipSpaceComm();
			if ((ret = checkBrackets(pr_data[pr_pos])))
				errorExit(ret == 1 ? 9 : 8, key);
			if (pr_data[pr_pos] == ';')
				errorExit(16, "");
			val = pickWord();
			server.setErrorPage(num, val);
			skipSpaceComm();
			if (pr_data[pr_pos] != ';')
				errorExit(3, key);
			pr_pos++;
		}
		else if (i == 4)
		{
			if (checkStringInt(val))
				errorExit(10, val);
			int num = static_cast<int>(std::stold(val));
			if (num < 0)
				errorExit(11, val);
			server.setLimitClientBody(num);
			skipSpaceComm();
			if ((ret = checkBrackets(pr_data[pr_pos])))
				errorExit(ret == 1 ? 9 : 8, key);
			if (pr_data[pr_pos] != ';')
				errorExit(3, key);
			pr_pos++;
		}
		if (i == 5)
		{
			server.setLocation(locationBlockProc(val));
		}
		skipSpaceComm();
	}
	pr_pos++;
	return server;
}

Location *				ConfParser::locationBlockProc(std::string const & str) {
	std::string			key;
	std::string			val;
	int					ret = 0;
	int					i = 0;

	if ((ret = checkBrackets(pr_data[pr_pos])))
		errorExit(ret == 1 ? 12 : 13, "");
	skipSpaceComm();

	Location *			location = new(Location);

	location->setPrefix(str);
	if (location->getPrefix()[0] != '/')
		location->setPrefix("/" + location->getPrefix());

	skipSpaceComm();
	if (pr_data[pr_pos] == ';')
		errorExit(16, "");
	if (checkBrackets(pr_data[pr_pos]) != 1)
		errorExit(2, "");
	pr_pos++;

	while (checkBrackets(pr_data[pr_pos]) != 2)
	{
		ret = 0;
		skipSpaceComm();
		if (checkBrackets(pr_data[pr_pos]) == 1)
			errorExit(9, "");
		key = pickWord();
		for (i = 0; i < 4; ++i)
		{
			if (toLower(key) == location->getLocationFields()[i])
			{
				skipSpaceComm();
				if (pr_data[pr_pos] == ';')
					errorExit(3, key);
				break ;
			}
		}
		if (pr_data[pr_pos] == ';')
			errorExit(16, "");
		if (i == 4)
			errorExit(4, key);
		skipSpaceComm();
		if (checkBrackets(pr_data[pr_pos]) == 1)
			errorExit(9, "");
		val = pickWord();
		if (i == 0)
		{
			if (location->getIndex().size())
				location->clearIndex();
			if (checkBrackets(pr_data[pr_pos]) == 1)
				errorExit(9, "");
			if (checkSuspiciousSymbols(val))
				errorExit(7, val);
			location->setIndex(val);
			skipSpaceComm();
			while (pr_data[pr_pos] != ';')
			{
				if ((ret = checkBrackets(pr_data[pr_pos])))
					errorExit(ret == 1 ? 9 : 8, key);
				val = pickWord();
				if (checkSuspiciousSymbols(val))
					errorExit(7, val);
				location->setIndex(val);
				skipSpaceComm();
			}
			pr_pos++;
		}
		else if (i == 1)
		{
			if (location->getAllowMethods().size())
				location->clearAllowMethods();
			if (checkBrackets(pr_data[pr_pos]) == 1)
				errorExit(9, "");
			if (checkSuspiciousSymbols(val))
				errorExit(7, val);
			std::string	upVal = toUpper(val);
			if (upVal != "GET" && upVal != "HEAD" && upVal != "POST")
				errorExit(15, val);
			location->setAllowMethods(upVal);
			skipSpaceComm();
			while (pr_data[pr_pos] != ';')
			{
				if ((ret = checkBrackets(pr_data[pr_pos])))
					errorExit(ret == 1 ? 9 : 8, key);
				val = pickWord();
				if (checkSuspiciousSymbols(val))
					errorExit(7, val);
				location->setAllowMethods(val);
				skipSpaceComm();
			}
			(pr_pos)++;
		}
		else if (i == 2)
		{
			location->setRoot(val);
			skipSpaceComm();
			if ((ret = checkBrackets(pr_data[pr_pos])))
				errorExit(ret == 1 ? 9 : 8, key);
			if (pr_data[pr_pos] != ';')
				errorExit(3, key);
			pr_pos++;
		}
		else if (i == 3)
		{
			std::string	lowVal = toLower(val);
			if (lowVal != "on" && lowVal != "off")
				errorExit(14, lowVal);
			location->setAutoindex(val);
			skipSpaceComm();
			if ((ret = checkBrackets(pr_data[pr_pos])))
				errorExit(ret == 1 ? 9 : 8, key);
			if (pr_data[pr_pos] != ';')
				errorExit(3, key);
			(pr_pos)++;
		}
		skipSpaceComm();
	}
	pr_pos++;
	return location;
}

void				ConfParser::parser() {
	while (pr_pos < pr_len - 1)
	{
		skipSpaceComm();
		if (toLower(pickWord()) != "server")
			errorExit(2, "");
		else
		{
			pr_server.push_back(serverBlockProc());
		}
		skipSpaceComm();
	}
	checkCompleteness();
	checkForDuplicates();
}

std::vector<VirtServer> &	ConfParser::getServer() {
	return pr_server;
}

void				ConfParser::checkCompleteness() {
	for (size_t i = 0; i < pr_server.size(); ++i)
	{
		if (!pr_server[i].getHost().length())
			errorExit(17, "host");
		if (!pr_server[i].getPort())
			errorExit(17, "listen");
		for (size_t j = 0; j < pr_server[i].getLocation().size(); ++j)
		{
			if (!pr_server[i].getLocation()[j]->getIndex().size())
				errorExit(17, "index");
			if (!pr_server[i].getLocation()[j]->getAllowMethods().size())
				errorExit(17, "allow_methods");
			if (!pr_server[i].getLocation()[j]->getRoot().length())
				errorExit(17, "root");
		}
	}
}

void				ConfParser::checkForDuplicates() {
	std::vector<std::string>	host;

	for (size_t i = 0; i < pr_server.size(); ++i) {
		for (size_t j = 0; j < host.size(); j++) {
			if (pr_server[i].getHost() == host[j]) {
				if (pr_server[i].getPort() == pr_server[j].getPort()) {
					for (size_t k = 0;
							k < pr_server[i].getServerName().size(); ++k)
						if (std::find(pr_server[j].getServerName().begin(),
										pr_server[j].getServerName().end(),
										pr_server[i].getServerName()[k]) !=
										pr_server[j].getServerName().end())
							errorExit(18, pr_server[i].getServerName()[k]);
				}
			}
		}
		host.push_back(pr_server[i].getHost());
	}
}
