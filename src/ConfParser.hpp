/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfParser.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awerebea <awerebea@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 11:35:34 by awerebea          #+#    #+#             */
/*   Updated: 2020/12/15 12:12:01 by awerebea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFPARSER_HPP
# define CONFPARSER_HPP

# include <iostream>

class							ConfParser
{
	std::string					_filePath;

								ConfParser(ConfParser const &);
	ConfParser &				operator=(ConfParser const &);
public:
								ConfParser();
								ConfParser(std::string const &);
								~ConfParser();

	std::string const &			getPath() const;

};

#endif
