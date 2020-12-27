/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VirtServer.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awerebea <awerebea@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/17 10:31:24 by awerebea          #+#    #+#             */
/*   Updated: 2020/12/27 11:00:56 by awerebea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VIRTSERVER_HPP
# define VIRTSERVER_HPP

# include <iostream>
# include <vector>
# include <map>
# include "Location.hpp"

class									VirtServer
{
	std::string							pr_host;
	int									pr_port;
	std::vector<std::string>			pr_serverName;
	std::map<int, std::string>			pr_errorPage;
	int									pr_limitClientBody;
	std::vector<Location *>				pr_location;
	int									fd;

	std::vector<std::string>			pr_serverFields;
public:
										VirtServer();
										~VirtServer();

	std::string const &					getHost() const;
	int const &							getPort() const;
	int const &							getFd() const;
	std::vector<std::string> const &	getServerName() const;
	std::map<int, std::string> const &	getErrorPagePath() const;
	int const &							getLimitClientBody() const;
	std::vector<Location *> const &		getLocation() const;

	// **** FOR PARSE ****
	std::vector<std::string> const &	getServerFields() const;

	void								setHost(std::string const &);
	void								setPort(int const &);
	void								setFd(int const &);
	void								setServerName(std::string const &);
	void								setErrorPage(int, std::string const &);
	void								setLimitClientBody(int const &);
	void								setLocation(Location *);

	void								clearServerName();
	void								eraseLocation();
};

#endif
