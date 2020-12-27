/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awerebea <awerebea@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/17 10:33:01 by awerebea          #+#    #+#             */
/*   Updated: 2020/12/27 21:35:18 by awerebea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
# define LOCATION_HPP

# include <iostream>
# include <vector>
# include <map>
# include <set>

struct									s_errExitData {
	int									code;
	std::string							word;
};

class									Location
{
	std::multimap<std::string, std::vector<std::string> >
										pr_data;
	std::string							pr_prefix;
	// std::vector<std::string>			pr_index;
	// std::vector<std::string>			pr_allowMethods;
	// std::string							pr_root;
	// std::string							pr_autoindex;
	std::set<std::string>				pr_locationsFields;
	struct s_errExitData				errStruct;
public:
										Location();
										~Location();

	std::multimap<std::string, std::vector<std::string> > const &
										getData() const;
	std::string const &					getPrefix() const;
	// std::vector<std::string> const &	getIndex() const;
	// std::vector<std::string> const &	getAllowMethods() const;
	// std::string const &					getRoot() const;
	// std::string const &					getAutoindex() const;

	std::set<std::string> const &		getLocationFields() const;

	void								setPrefix(std::string const &);
	struct s_errExitData const &		setDataPair(std::string const &,
											std::vector<std::string> const &);
	// void								setIndex(std::string const &);
	// void								setAllowMethods(std::string const &);
	// void								setRoot(std::string const &);
	// void								setAutoindex(std::string const &);

	// void								clearIndex();
	// void								clearAllowMethods();
};

#endif
