/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awerebea <awerebea@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/02 14:36:08 by awerebea          #+#    #+#             */
/*   Updated: 2020/12/15 16:26:03 by awerebea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

char	*ft_strdup(const char *s)
{
	char	*dest;
	size_t	len;

	len = (size_t)s;
	while (*s)
		s++;
	len = (size_t)s - len;
	s -= len;
	if (!(dest = (char*)malloc(sizeof(char) * (len + 1))))
		return (NULL);
	while (*s)
		*dest++ = (char)*s++;
	*dest = '\0';
	dest -= len;
	return (dest);
}
