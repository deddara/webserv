/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awerebea <awerebea@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/06 18:36:15 by awerebea          #+#    #+#             */
/*   Updated: 2020/12/15 16:32:32 by awerebea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** Adds the element ’new_elem’ at the end of the list.
*/

#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new_elem)
{
	t_list	*element;

	element = *lst;
	if (*lst && lst)
	{
		while (element->next)
			element = element->next;
		element->next = new_elem;
	}
	else
		*lst = new_elem;
}
