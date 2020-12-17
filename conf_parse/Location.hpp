/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awerebea <awerebea@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/17 10:33:01 by awerebea          #+#    #+#             */
/*   Updated: 2020/12/17 14:12:49 by awerebea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
# define LOCATION_HPP

# include <iostream>
# include <vector>

class									Location
{
	std::string							pr_prefix;
	std::vector<std::string>			pr_index;
	std::vector<std::string>			pr_allowMethods;
	std::string							pr_root;
	std::string							pr_autoindex;

	std::vector<std::string>			pr_locationsFields;
public:
										Location();
										~Location();

	std::string const &					getPrefix() const;
	std::vector<std::string> const &	getIndex() const;
	std::vector<std::string> const &	getAllowMethods() const;
	std::string const &					getRoot() const;
	std::string const &					getAutoindex() const;

	std::vector<std::string> const &	getLocationFields() const;

	void								setPrefix(std::string const &);
	void								setIndex(std::string const &);
	void								setAllowMethods(std::string const &);
	void								setRoot(std::string const &);
	void								setAutoindex(std::string const &);

	void								clearIndex();
	void								clearAllowMethods();
};

#endif
