/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfParser.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awerebea <awerebea@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 11:35:34 by awerebea          #+#    #+#             */
/*   Updated: 2020/12/16 21:06:26 by awerebea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFPARSER_HPP
# define CONFPARSER_HPP

# include <iostream>
# include <list>
# include <vector>
# include <fcntl.h>
# include <unistd.h>
# include <stdlib.h>
# include "libft.h"

struct Locations
{
	std::string					prefix;
	std::vector<std::string>	index;
	std::vector<std::string>	allowMethods;
	std::string					root;
	std::string					autoindex;
};

struct Servers
{
	std::string					host;
	int							port;
	std::vector<std::string>	serverName;
	std::string					errorPage;
	int							limitClientBody;
	std::vector<Locations>		locations;
};

class							ConfParser
{
	std::string					_filePath;
	bool						_success;
	std::vector<Servers>		_servers;
	std::vector<std::string>	_serverFields;
	std::vector<std::string>	_locationsFields;
	bool						_locationBlockInProgress;

								ConfParser();
								ConfParser(ConfParser const &);
	ConfParser &				operator=(ConfParser const &);

	void						errorExit(int, std::string const &);
	void						parser();
	void						serverBlockProc(std::string const &,
															size_t *, size_t);
	void						locationBlockProc(std::string const &,
										size_t *, size_t, std::string const &);
	std::string const			readConfFile(int);
public:
								ConfParser(std::string const &);
								~ConfParser();

	std::string const &			getPath() const;
	bool						getParsResult() const;
};

#endif
