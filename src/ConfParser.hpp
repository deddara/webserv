/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfParser.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awerebea <awerebea@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 11:35:34 by awerebea          #+#    #+#             */
/*   Updated: 2020/12/15 19:00:47 by awerebea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFPARSER_HPP
# define CONFPARSER_HPP

# include <iostream>
# include <fcntl.h>
# include <unistd.h>
# include <stdlib.h>
# include "libft.h"

class							ConfParser
{
	struct Servers
	{
		std::string				host;
		int						port;
		std::string				server_name;

		struct Servers *		next;
	};

	std::string					_filePath;
	bool						_success;
	Servers *					_servers;

								ConfParser();
								ConfParser(ConfParser const &);
	ConfParser &				operator=(ConfParser const &);

	void						errorExit(int);
	void						parser();
	std::string const			readConfFile(int);
public:
								ConfParser(std::string const &);
								~ConfParser();

	std::string const &			getPath() const;
	bool						getParsResult() const;
};

#endif
