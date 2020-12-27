/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VirtServer.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awerebea <awerebea@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/17 10:31:24 by awerebea          #+#    #+#             */
/*   Updated: 2020/12/27 18:59:19 by awerebea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VIRTSERVER_HPP
# define VIRTSERVER_HPP

# include <iostream>
# include <vector>
# include <map>
# include <set>
# include "Location.hpp"

struct									s_errExitData {
	int									code;
	std::string							word;
};

class									VirtServer
{
	std::multimap<std::string, std::vector<std::string> >
										pr_data;
	std::string							pr_host;
	int									pr_port;
	std::vector<std::string>			pr_serverName;
	std::map<int, std::string>			pr_errorPage;
	int									pr_limitClientBody;
	std::vector<Location *>				pr_location;
	int									fd;

	// std::vector<std::string>			pr_serverFields;
	std::set<std::string>				pr_serverFields;
	struct s_errExitData				errStruct;
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
	std::multimap<std::string, std::vector<std::string> > const &
										getData() const;

	// **** FOR PARSE ****
	// std::vector<std::string> const &	getServerFields() const;
	std::set<std::string> const &		getServerFields() const;

	struct s_errExitData const &		setPairInData(std::string const &,
											std::vector<std::string> const &);

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
