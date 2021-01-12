/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awerebea <awerebea@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/13 12:59:14 by awerebea          #+#    #+#             */
/*   Updated: 2021/01/12 15:19:47 by awerebea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.hpp"

char	*ft_strchr(const char *s, int c)
{
	while (*s != c)
	{
		if (!*s)
			return (NULL);
		s++;
	}
	return ((char *)s);
}

t_glst	*f_gnl_new(int fd)
{
	t_glst		*gnl;

	if (!(gnl = (t_glst*)malloc(sizeof(t_glst))))
		return (NULL);
	gnl->fd = fd;
	gnl->rmndr = NULL;
	gnl->rmndr_start = NULL;
	gnl->next = NULL;
	return (gnl);
}

t_glst	*f_search_gnl(int fd, t_glst **g_head)
{
	t_glst			*g_tmp;

	if (!*g_head)
		return ((*g_head = f_gnl_new(fd)) ? *g_head : NULL);
	g_tmp = *g_head;
	while (g_tmp->fd != fd && g_tmp->next)
		g_tmp = g_tmp->next;
	if (g_tmp->fd == fd)
		return (g_tmp);
	return ((g_tmp->next = f_gnl_new(fd)) ? g_tmp->next : NULL);
}
