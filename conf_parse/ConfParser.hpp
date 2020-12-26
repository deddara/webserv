/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfParser.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awerebea <awerebea@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 11:35:34 by awerebea          #+#    #+#             */
/*   Updated: 2020/12/26 14:20:29 by awerebea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFPARSER_HPP
# define CONFPARSER_HPP

# include <iostream>
# include <fcntl.h>
# include <unistd.h>
# include <stdlib.h>
# include <algorithm>

# include "VirtServer.hpp"
# include "Location.hpp"
# include "includes.hpp"

class							ConfParser
{
	std::string					pr_data;
	size_t						pr_pos;
	size_t						pr_len;
	std::vector<VirtServer>		pr_server;

								ConfParser();
	std::string					readConfFile(std::string const &);
	void						errorExit(int, std::string const &);
	void						parser();
	VirtServer					serverBlockProc();
	Location *					locationBlockProc(std::string const &);
	void						skipSpaceComm();
	std::string					pickWord();
	std::string					toLower(std::string);
	std::string					toUpper(std::string);
	void						checkCompleteness();
	void						checkForDuplicates();
public:
								ConfParser(std::string const &);
								~ConfParser();

	std::vector<VirtServer> &	getServer();
};

#endif
