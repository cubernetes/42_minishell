/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ltop_left.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 00:54:23 by tischmid          #+#    #+#             */
/*   Updated: 2024/07/05 00:54:23 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/** Returns the first element of a list.
 *
 *  @param list: the list to get the first element of
 *  @returns: the first element
 */
t_list_node	*ltop_left(t_list *list)
{
	return (list->first);
}
