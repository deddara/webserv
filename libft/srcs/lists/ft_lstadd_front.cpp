/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_front.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awerebea <awerebea@student.21-school.ru>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/06 18:01:41 by awerebea          #+#    #+#             */
/*   Updated: 2020/12/15 16:32:46 by awerebea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** Adds the element ’new_elem’ at the beginning of the list.
*/

#include "libft.h"

void	ft_lstadd_front(t_list **lst, t_list *new_elem)
{
	t_list	*element;

	if (lst && new_elem)
	{
		element = new_elem;
		element->next = *lst;
		*lst = element;
	}
}
