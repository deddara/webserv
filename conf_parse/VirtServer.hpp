/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VirtServer.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awerebea <awerebea@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/17 10:31:24 by awerebea          #+#    #+#             */
/*   Updated: 2020/12/28 00:58:32 by awerebea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VIRTSERVER_HPP
# define VIRTSERVER_HPP

# include <iostream>
# include <vector>
# include <map>
# include <set>
# include "Location.hpp"
# include "includes.hpp"

class									VirtServer
{
	std::multimap<std::string, std::vector<std::string> >
										pr_data;
	std::vector<Location *>				pr_location;

	std::set<std::string>				pr_serverFields;
	struct s_errExitData				errStruct;
public:
										VirtServer();
										~VirtServer();

	std::multimap<std::string, std::vector<std::string> > const &
										getData() const;
	std::vector<Location *> const &		getLocation() const;

	// methods used for parsing
	std::set<std::string> const &		getServerFields() const;
	struct s_errExitData const &		setErrStruct(int, std::string);
	struct s_errExitData const &		setDataPair(std::string const &,
											std::vector<std::string> &);
	void								setLocation(Location *);
	void								eraseLocation();
};

#endif
