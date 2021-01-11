/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awerebea <awerebea@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/17 10:33:01 by awerebea          #+#    #+#             */
/*   Updated: 2021/01/09 17:38:07 by awerebea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
# define LOCATION_HPP

# include <iostream>
# include <vector>
# include <map>
# include <set>
# include "includes.hpp"

struct									s_errExitData {
	int									code;
	std::string							word;
};

class									Location
{
	std::multimap<std::string, std::vector<std::string> >
										pr_data;
	std::string							pr_prefix;
	std::set<std::string>				pr_locationsFields;
	struct s_errExitData				errStruct;
public:
										Location();
										~Location();

	std::multimap<std::string, std::vector<std::string> > const &
										getData() const;
	std::string const &					getPrefix() const;
	std::set<std::string> const &		getLocationFields() const;

	void								setPrefix(std::string const &);
	struct s_errExitData const &		setErrStruct(int, std::string);
	struct s_errExitData const &		setDataPair(std::string const &,
											std::vector<std::string> &);
	struct s_errExitData const &		checkCgiSettings();
	int									getLimitClientBody() const;

	};

#endif
