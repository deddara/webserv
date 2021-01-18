/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfParser.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awerebea <awerebea@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 11:40:21 by awerebea          #+#    #+#             */
/*   Updated: 2021/01/18 18:02:26 by awerebea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfParser.hpp"

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
	std::string		errors[26] = {
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
		"Error: malloc fails",
		"Error: limit client body \"" + word + "\" overflows",
		"Error: duplicate location \"" + word + "\" found",
		"Error: invalid cgi-file extension \"" + word + "\" found",
		"Error: invalid cgi settings in location \"" + word + "\"",
		"Error: forbidden POST method without cgi settings in location \"" +
			word + "\"",
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
	Location *		location = nullptr;
	std::string		key;
	int				ret = 0;
	int				i = 0;
	std::set<std::string>::const_iterator	it;

	while (checkBrackets(pr_data[pr_pos]) != 2)
	{
		skipSpaceComm();
		if (pr_data[pr_pos] == ';')
			errorExit(16, "");
		ret = 0;
		if (checkBrackets(pr_data[pr_pos]) == 1)
			errorExit(9, "");
		key = toLower(pickWord());
		i = 0;
		if ((it = server.getServerFields().find(key)) ==
											server.getServerFields().end()) {
			errorExit(4, key);
		}
		skipSpaceComm();
		if (pr_data[pr_pos] == ';')
			errorExit(3, key);
		if ((ret = checkBrackets(pr_data[pr_pos])))
			errorExit(ret == 1 ? 9 : 8, key);
		if (*it == "location") {
			location = locationBlockProc(pickWord());
			for (size_t i = 0; i < server.getLocation().size(); ++i) {
				if (location->getPrefix() ==
						server.getLocation()[i]->getPrefix()) {
					errorExit(22, location->getPrefix());
				}
			}
			server.setLocation(location);
		} else {
			std::vector<std::string>			valArray;
			while (pr_data[pr_pos] != ';')
			{
				skipSpaceComm();
				if ((ret = checkBrackets(pr_data[pr_pos])))
					errorExit(ret == 1 ? 9 : 8, key);
				valArray.push_back(pickWord());
				skipSpaceComm();
			}
			pr_errStruct = server.setDataPair(key, valArray);
			if (pr_errStruct.code) {
				errorExit(pr_errStruct.code, pr_errStruct.word);
			}
			pr_pos++;
		}
		skipSpaceComm();
	}
	pr_pos++;
	server.createErrPagePathMap();
	return server;
}

Location *				ConfParser::locationBlockProc(std::string const & str) {
	std::string			key;
	std::string			val;
	int					ret = 0;
	std::set<std::string>::const_iterator	it;

	if ((ret = checkBrackets(pr_data[pr_pos])))
		errorExit(ret == 1 ? 12 : 13, "");
	if (pr_data[pr_pos] == ';')
		errorExit(16, "");
	skipSpaceComm();
	if (checkBrackets(pr_data[pr_pos]) != 1)
		errorExit(2, "");
	pr_pos++;

	Location *			location = new(Location);

	// add '/' in start of prefix if needed
	location->setPrefix(str);
	if (location->getPrefix()[0] != '/') {
		location->setPrefix("/" + location->getPrefix());
	}

	while (checkBrackets(pr_data[pr_pos]) != 2) {
		ret = 0;
		skipSpaceComm();
		if (checkBrackets(pr_data[pr_pos]) == 1)
			errorExit(9, "");
		key = toLower(pickWord());
		if ((it = location->getLocationFields().find(key)) ==
										location->getLocationFields().end()) {
			errorExit(4, key);
		}
		skipSpaceComm();
		if (pr_data[pr_pos] == ';')
			errorExit(3, key);
		if ((ret = checkBrackets(pr_data[pr_pos])))
			errorExit(ret == 1 ? 9 : 8, key);
		std::vector<std::string>			valArray;
		while (pr_data[pr_pos] != ';')
		{
			skipSpaceComm();
			if ((ret = checkBrackets(pr_data[pr_pos])))
				errorExit(ret == 1 ? 9 : 8, key);
			valArray.push_back(pickWord());
			skipSpaceComm();
		}
		pr_errStruct = location->setDataPair(key, valArray);
		if (pr_errStruct.code) {
			errorExit(pr_errStruct.code, pr_errStruct.word);
		}
		pr_pos++;
		skipSpaceComm();
	}
	pr_pos++;
	pr_errStruct = location->checkCgiSettings();
	if (pr_errStruct.code) {
		errorExit(pr_errStruct.code, pr_errStruct.word);
	}
	if (location->getData().count("allow_methods")) {
		std::multimap<std::string, std::vector<std::string> >::const_iterator it
			= location->getData().find("allow_methods");
		size_t i = 0;
		for (; i < it->second.size(); ++i) {
			if (it->second[i] == "POST") {
				break ;
			}
		}
		if (i < it->second.size() && !location->getData().count("cgi_ext")) {
			errorExit(25, str);
		}
	}
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
		if (!pr_server[i].getData().count("host")) {
			errorExit(17, "host");
		}
		if (!pr_server[i].getData().count("listen")) {
			errorExit(17, "listen");
		}
		for (size_t j = 0; j < pr_server[i].getLocation().size(); ++j) {
			// if (!pr_server[i].getLocation()[j]->getData().count("index")) {
			//     errorExit(17, "index");
			// }
			if (!pr_server[i].getLocation()[j]->getData().
													count("allow_methods")) {
				errorExit(17, "allow_methods");
			}
			if (!pr_server[i].getLocation()[j]->getData().count("root")) {
				errorExit(17, "root");
			}
		}
	}
}

void				ConfParser::checkForDuplicates() {
	std::vector<std::string>	host;

	for (size_t i = 0; i < pr_server.size(); ++i) {
		for (size_t j = 0; j < host.size(); j++) {
			if (pr_server[i].getData().find("host")->second[0] == host[j]) {
				if (pr_server[i].getData().find("listen")->second[0] ==
						pr_server[j].getData().find("listen")->second[0]) {
					for (size_t k = 0;
							k < pr_server[i].getData().find("server_name")->
							second.size(); ++k) {
						for (size_t l = 0;
							l < pr_server[j].getData().find("server_name")->
							second.size(); ++l) {
							if (pr_server[i].getData().find("server_name")->
									second[k] ==
									pr_server[j].getData().find("server_name")->
									second[l]) {
								errorExit(18, pr_server[i].getData().
										find("server_name")->second[k]);
							}
						}
					}
				}
			}
		}
		host.push_back(pr_server[i].getData().find("host")->second[0]);
	}
}
