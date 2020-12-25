/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorPages.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awerebea <awerebea@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/25 17:26:20 by awerebea          #+#    #+#             */
/*   Updated: 2020/12/25 17:54:34 by awerebea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRORPAGES_HPP
# define ERRORPAGES_HPP

# include <iostream>
# include <map>

static std::map<int, std::string>				errorPageTemplates;

class									ErrorPages {
										ErrorPages();
public:
										ErrorPages(std::map<int, std::string> &);
										~ErrorPages();
};

#endif
