/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjeremia <jjeremia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/14 10:45:11 by jjeremia          #+#    #+#             */
/*   Updated: 2020/12/14 10:45:12 by jjeremia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

#include <iostream>
#include <string>
#include <map>
#include <vector>

class Request {
public:
	typedef std::map<std::string, std::vector<std::string> >    map_type;
	typedef std::vector<std::string>                            value_type;

private:
	Request();
	void check_common();

	map_type    _data;
	bool        _isbadrequest;

public:
	//Constructors
	explicit Request(std::string const &raw_data);
	Request(Request const &obj);
	Request &operator=(Request const &obj);
	~Request();

	//Check error
	bool error() const;

	//Element access
	const value_type &find(std::string const &key) const;


};


#endif
