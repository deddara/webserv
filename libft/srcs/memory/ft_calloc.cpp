/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awerebea <awerebea@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/02 13:02:37 by awerebea          #+#    #+#             */
/*   Updated: 2020/12/15 16:29:10 by awerebea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	unsigned char	*a;
	size_t			n;

	n = nmemb * size;
	if (!(a = (unsigned char*)malloc(n)))
		return (NULL);
	ft_bzero(a, n);
	return ((void *)a);
}
