/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ltop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 00:54:29 by tischmid          #+#    #+#             */
/*   Updated: 2024/07/05 00:54:29 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/** Return the last element of a list without deleting.
 *
 *  @param list: the list to get the last element of
 *  @returns: the last element
 */
t_list_node	*ltop(t_list *list)
{
	return (list->last);
}
