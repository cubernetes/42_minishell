/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 15:04:36 by tischmid          #+#    #+#             */
/*   Updated: 2023/11/22 15:04:38 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new_tail)
{
	t_list	*last;

	if (!lst)
		return ;
	if (!*lst)
	{
		*lst = new_tail;
		return ;
	}
	last = ft_lstlast(*lst);
	last->next = new_tail;
}
