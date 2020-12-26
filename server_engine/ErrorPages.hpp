/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorPages.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awerebea <awerebea@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/25 17:26:20 by awerebea          #+#    #+#             */
/*   Updated: 2020/12/26 12:32:03 by awerebea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRORPAGES_HPP
# define ERRORPAGES_HPP

# include <iostream>
# include <map>
# include <vector>

class								ErrorPages {
	std::map<int, std::vector<std::string> >
									errorPageTemplates;
public:
									ErrorPages();
									~ErrorPages();
	std::map<int, std::vector<std::string> > const &
									getErrorPageTemplates() const;
	void							valClear(std::vector<std::string> &);
};

#endif
