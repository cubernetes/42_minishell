/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ljoin.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 00:54:17 by tischmid          #+#    #+#             */
/*   Updated: 2024/07/05 00:54:17 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ljoin(t_list list[static 1], const char delim[static 1])
{
	char	*joined;

	if (list->len == 0)
		return ("");
	joined = list->first->as_str;
	if (list->len == 1)
		return (joined);
	lnext(liter(list));
	while (lnext(list))
	{
		joined = ft_strjoin(joined, delim);
		joined = ft_strjoin(joined, list->current->as_str);
	}
	return (joined);
}
