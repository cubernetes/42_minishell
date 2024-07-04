/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   productions2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 18:46:59 by tischmid          #+#    #+#             */
/*   Updated: 2024/07/04 18:51:38 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

t_tree	*production_part_to_child(t_tree production)
{
	t_tree	*child;

	child = ft_malloc(sizeof(*child));
	ft_memcpy(child, &production, sizeof(production));
	if (production.type == TOKEN)
		child->token = new_token(child->token->str,
				child->token->type, true);
	return (child);
}

t_list	*production_to_children(t_tree *production)
{
	int		size;
	t_list	*children;

	children = lnew();
	size = 0;
	while (!tree_is_null((t_tree *)production + size))
		lpush(children,
			as_tree(production_part_to_child(production[size++])));
	return (children);
}
