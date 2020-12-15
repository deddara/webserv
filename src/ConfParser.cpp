/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfParser.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awerebea <awerebea@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 11:40:21 by awerebea          #+#    #+#             */
/*   Updated: 2020/12/15 21:29:38 by awerebea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfParser.hpp"

ConfParser::ConfParser(std::string const & fpath): _filePath(fpath)
{
	_success = false;
	_servers = nullptr;
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
	while (*pos < len - 1 && !isspace(data[*pos]))
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
		errorExit(1);
	return data;
}

void					ConfParser::errorExit(int code)
{
	std::string const	errors[3] = {
		"Error: config file is unavailable",
		"Error: read fails",
		"Error: incorrect config file structure"
	};
	std::cout << errors[code] << std::endl;
	exit(1);
}

void					ConfParser::parser()
{
	int					fd;
	if ((fd = open(_filePath.c_str(), O_RDONLY)) < 0)
		errorExit(0);

	std::string const	data = readConfFile(fd);

	size_t				pos = 0;
	size_t				len = data.length();
	while (pos < len - 1)
	{
		skipSpaceComm(data, &pos, len);
		if (toLower(pickWord(data, &pos, len)) != "server")
			errorExit(2);
		else
		{
			std::cout << "!" << std::endl; //DEBUG
		}
	}

}
