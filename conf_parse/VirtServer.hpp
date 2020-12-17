/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VirtServer.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awerebea <awerebea@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/17 10:31:24 by awerebea          #+#    #+#             */
/*   Updated: 2020/12/17 14:41:57 by awerebea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VIRTSERVER_HPP
# define VIRTSERVER_HPP

# include <iostream>
# include <vector>
# include "Location.hpp"

class									VirtServer
{
	std::string							pr_host;
	int									pr_port;
	std::vector<std::string>			pr_serverName;
	std::string							pr_errorPage; //map
	int									pr_limitClientBody;
	std::vector<Location>				pr_location;

	std::vector<std::string>			pr_serverFields;
public:
										VirtServer();
										~VirtServer();

	std::string const &					getHost() const;
	int const &							getPort() const;
	std::vector<std::string> const &	getServerName() const;
	std::string const &					getErrorPage() const;
	int const &							getLimitClientBody() const;
	std::vector<Location> const &		getLocation() const;

	// **** FOR PARSE ****
	std::vector<std::string> const &	getServerFields() const;

	void								setHost(std::string const &);
	void								setPort(int const &);
	void								setServerName(std::string const &);
	void								setErrorPage(std::string const &);
	void								setLimitClientBody(int const &);
	void								setLocation(Location const &);

	void								clearServerName();
};

#endif
