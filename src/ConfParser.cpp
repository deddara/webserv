/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfParser.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awerebea <awerebea@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 11:40:21 by awerebea          #+#    #+#             */
/*   Updated: 2020/12/16 21:09:10 by awerebea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfParser.hpp"

ConfParser::ConfParser(std::string const & fpath): _filePath(fpath)
{
	_success = false;
	parser();
}

ConfParser::ConfParser(ConfParser const & other) { *this = other; }

ConfParser::~ConfParser() {}

ConfParser &			ConfParser::operator=(ConfParser const & other)
{
	if (this != &other)
		_filePath = other._filePath;
	return *this;
}

void					skipSpaceComm(std::string const &data, size_t *pos,
																	size_t len)
{
	size_t				newPos = 0;
	while (*pos < len - 1)
	{
		while (isspace(data[*pos]))
			(*pos)++;
		if (data[*pos] == '#')
		{
			if ((newPos = data.find("\n", *pos)) != std::string::npos)
				*pos = newPos;
			else
			{
				*pos = len - 1;
				break ;
			}
		}
		else
			break ;
	}
}

std::string				toLower(std::string str)
{
	for (size_t i = 0; i < str.length(); i++)
	{
		if (str[i] >= 'A' && str[i] <= 'Z')
			str[i] += 32;
	}
	return str;
}

std::string				pickWord(std::string const &data, size_t *pos,
																	size_t len)
{
	std::string			word;
	while (*pos < len - 1 && !isspace(data[*pos]) && data[*pos] != ';' \
			&& data[*pos] != '{' && data[*pos] != '}')
		word.push_back(data[(*pos)++]);
	return word;
}

std::string const &		ConfParser::getPath() const { return _filePath; }

bool					ConfParser::getParsResult() const { return _success; }

std::string const		ConfParser::readConfFile(int fd)
{
	std::string			data;
	char				buf[128];
	int					ret = 0;
	while ((ret = read(fd, buf, 128)) > 0)
	{
		buf[ret] = '\0';
		data.append(buf);
	}
	if (ret < 0)
		errorExit(1, "");
	return data;
}

void					ConfParser::errorExit(int code,
													std::string const & word)
{
	std::string			errors[14] = {
		"Error: config file is unavailable",
		"Error: read fails",
		"Error: config file syntax error",
		"Error: invalid number of arguments in \"" + word + \
			"\" directive",
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
	};
	std::cout << errors[code] << std::endl;
	exit(1);
}

void					serverInit(Servers *server)
{
	server->host = "";
	server->port = 0;
	// server->serverName; //INFO
	server->errorPage = "";
	server->limitClientBody = -1;
}

int						checkStringInt(std::string const &word)
{
	size_t				len = word.length();
	for (size_t i = 0; i < len; ++i)
	{
		if (!std::isdigit(word[i]))
			return 1;
	}
	return 0;
}

int						checkSuspiciousSymbols(std::string const &word)
{
	size_t				len = word.length();
	for (size_t i = 0; i < len; ++i)
	{
		if (word[i] == '/')
			return 1;
	}
	return 0;
}

int						checkBrackets(char c)
{
	if (c == '{')
		return 1;
	if (c == '}')
		return 2;
	return 0;
}

void					ConfParser::serverBlockProc(std::string const &data,
													size_t *pos, size_t len)
{
	_serverFields.push_back("host");
	_serverFields.push_back("listen");
	_serverFields.push_back("server_name");
	_serverFields.push_back("error_page");
	_serverFields.push_back("limit_client_body");
	_serverFields.push_back("location");

	skipSpaceComm(data, pos, len);
	if (checkBrackets(data[*pos]) != 1)
		errorExit(2, "");
	(*pos)++;
	Servers				server;
	serverInit(&server);

	std::string			key;
	std::string			val;
	int					ret;

	while (checkBrackets(data[*pos]) != 2)
	{
		ret = 0;
		skipSpaceComm(data, pos, len);
		if (checkBrackets(data[*pos]) == 1)
			errorExit(9, "");
		key = pickWord(data, pos, len);
		int				i = 0;
		for (; i < 6; ++i)
		{
			if (toLower(key) == _serverFields[i])
			{
				skipSpaceComm(data, pos, len);
				if (data[*pos] == ';')
					errorExit(3, key);
				break ;
			}
		}
		if (i == 6)
			errorExit(4, key);
		skipSpaceComm(data, pos, len);
		if (checkBrackets(data[*pos]) == 1)
			errorExit(9, "");
		val = pickWord(data, pos, len);
		if (i == 0)
		{
			server.host = val;
			skipSpaceComm(data, pos, len);
			if ((ret = checkBrackets(data[*pos])))
				errorExit(ret == 1 ? 9 : 8, key);
			if (data[*pos] != ';')
				errorExit(3, key);
			(*pos)++;
		}
		else if (i == 1)
		{
			if (checkStringInt(val))
				errorExit(5, val);
			server.port = static_cast<int>(std::stold(val));
			if (server.port < 0 || server.port > 65535)
				errorExit(6, val);
			skipSpaceComm(data, pos, len);
			if ((ret = checkBrackets(data[*pos])))
				errorExit(ret == 1 ? 9 : 8, key);
			if (data[*pos] != ';')
				errorExit(3, key);
			(*pos)++;
		}
		else if (i == 2)
		{
			if (server.serverName.size())
				server.serverName.clear();
			if (checkBrackets(data[*pos]) == 1)
				errorExit(9, "");
			if (checkSuspiciousSymbols(val))
				errorExit(7, val);
			server.serverName.push_back(val);
			skipSpaceComm(data, pos, len);
			while (data[*pos] != ';')
			{
				if ((ret = checkBrackets(data[*pos])))
					errorExit(ret == 1 ? 9 : 8, key);
				val = pickWord(data, pos, len);
				if (checkSuspiciousSymbols(val))
					errorExit(7, val);
				server.serverName.push_back(val);
				skipSpaceComm(data, pos, len);
			}
			(*pos)++;
		}
		if (i == 3)
		{
			server.errorPage = val;
			skipSpaceComm(data, pos, len);
			if ((ret = checkBrackets(data[*pos])))
				errorExit(ret == 1 ? 9 : 8, key);
			if (data[*pos] != ';')
				errorExit(3, key);
			(*pos)++;
		}
		else if (i == 4)
		{
			if (checkStringInt(val))
				errorExit(10, val);
			server.limitClientBody = static_cast<int>(std::stold(val));
			if (server.limitClientBody < 0)
				errorExit(11, val);
			skipSpaceComm(data, pos, len);
			if ((ret = checkBrackets(data[*pos])))
				errorExit(ret == 1 ? 9 : 8, key);
			if (data[*pos] != ';')
				errorExit(3, key);
			(*pos)++;
		}
		if (i == 5)
		{
			locationBlockProc(data, pos, len, val);
		}
		std::cout << server.host << std::endl; //DEBUG
		std::cout << server.port << std::endl; //DEBUG
		for (unsigned long i = 0; i < server.serverName.size(); ++i)
			std::cout << server.serverName[i] << std::endl; //DEBUG
		std::cout << server.errorPage << std::endl; //DEBUG
		std::cout << server.limitClientBody << std::endl; //DEBUG
	}
}

void					locationInit(Locations *location)
{
	location->prefix = "";
	// location->index; //INFO
	// location->allowMethods; //INFO
	location->root = "";
	location->autoindex = "off";
}

void					ConfParser::locationBlockProc(std::string const &data,
						size_t *pos, size_t len, std::string const &prefix)
{
	_locationsFields.push_back("index");
	_locationsFields.push_back("allow_methods");
	_locationsFields.push_back("root");
	_locationsFields.push_back("autoindex");

	int					ret;

	if ((ret = checkBrackets(data[*pos])))
		errorExit(ret == 1 ? 12 : 13, "");
	skipSpaceComm(data, pos, len);

	Locations			location;
	locationInit(&location);
	location.prefix = prefix;

	if (location.prefix[0] != '/')
		location.prefix.insert(0, "/", 1);

	skipSpaceComm(data, pos, len);
	if (checkBrackets(data[*pos]) != 1)
		errorExit(2, "");
	(*pos)++;

	std::string			key;
	std::string			val;

	while (checkBrackets(data[*pos]) != 2)
	{
		ret = 0;
		skipSpaceComm(data, pos, len);
		if (checkBrackets(data[*pos]) == 1)
			errorExit(9, "");
		key = pickWord(data, pos, len);
		int				i = 0;
		for (; i < 4; ++i)
		{
			if (toLower(key) == _locationsFields[i])
			{
				skipSpaceComm(data, pos, len);
				if (data[*pos] == ';')
					errorExit(3, key);
				break ;
			}
		}
		if (i == 4)
			errorExit(4, key);
		skipSpaceComm(data, pos, len);
		if (checkBrackets(data[*pos]) == 1)
			errorExit(9, "");
		val = pickWord(data, pos, len);
		if (i == 0)
		{
			if (location.index.size())
				location.index.clear();
			if (checkBrackets(data[*pos]) == 1)
				errorExit(9, "");
			if (checkSuspiciousSymbols(val))
				errorExit(7, val);
			location.index.push_back(val);
			skipSpaceComm(data, pos, len);
			while (data[*pos] != ';')
			{
				if ((ret = checkBrackets(data[*pos])))
					errorExit(ret == 1 ? 9 : 8, key);
				val = pickWord(data, pos, len);
				if (checkSuspiciousSymbols(val))
					errorExit(7, val);
				location.index.push_back(val);
				skipSpaceComm(data, pos, len);
			}
			(*pos)++;
		}
		else if (i == 1)
		{
			if (location.allowMethods.size())
				location.allowMethods.clear();
			if (checkBrackets(data[*pos]) == 1)
				errorExit(9, "");
			if (checkSuspiciousSymbols(val))
				errorExit(7, val);
			location.allowMethods.push_back(val);
			skipSpaceComm(data, pos, len);
			while (data[*pos] != ';')
			{
				if ((ret = checkBrackets(data[*pos])))
					errorExit(ret == 1 ? 9 : 8, key);
				val = pickWord(data, pos, len);
				if (checkSuspiciousSymbols(val))
					errorExit(7, val);
				location.allowMethods.push_back(val);
				skipSpaceComm(data, pos, len);
			}
			(*pos)++;
		}
		else if (i == 2)
		{
			location.root = val;
			skipSpaceComm(data, pos, len);
			if ((ret = checkBrackets(data[*pos])))
				errorExit(ret == 1 ? 9 : 8, key);
			if (data[*pos] != ';')
				errorExit(3, key);
			(*pos)++;
		}
		else if (i == 3)
		{
			location.autoindex = val;
			skipSpaceComm(data, pos, len);
			if ((ret = checkBrackets(data[*pos])))
				errorExit(ret == 1 ? 9 : 8, key);
			if (data[*pos] != ';')
				errorExit(3, key);
			(*pos)++;
		}
		std::cout << location.prefix << std::endl; //DEBUG
		for (unsigned long i = 0; i < location.index.size(); ++i)
			std::cout << location.index[i] << std::endl; //DEBUG
		for (unsigned long i = 0; i < location.allowMethods.size(); ++i)
			std::cout << location.allowMethods[i] << std::endl; //DEBUG
		std::cout << location.root << std::endl; //DEBUG
		std::cout << location.autoindex << std::endl; //DEBUG
	}
}

void					ConfParser::parser()
{
	std::vector<std::string>	confFields;
	confFields.push_back("server");
	confFields.push_back("location");
	// bool				serverBlockInProgress = false;
	_locationBlockInProgress = false;


	int					fd;
	if ((fd = open(_filePath.c_str(), O_RDONLY)) < 0)
		errorExit(0, "");

	std::string const	data = readConfFile(fd);

	size_t				pos = 0;
	size_t				len = data.length();
	while (pos < len - 1)
	{
		skipSpaceComm(data, &pos, len);
		if (toLower(pickWord(data, &pos, len)) != "server")
			errorExit(2, "");
		else
		{
			serverBlockProc(data, &pos, len);
		}
	}

}
