/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   as_pointer_types.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 01:03:40 by tischmid          #+#    #+#             */
/*   Updated: 2024/07/05 01:03:41 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/** Return a data union initialized with the char* argument.
 *
 *  @param as_str: the char* member of the union
 *  @returns: the data union containing `as_str'
 */
t_data	as_str(char *as_str)
{
	return ((t_data){.as_str = as_str, .type = TYPE_STR});
}

/** Return a data union initialized with the void* argument.
 *
 *  @param as_ptr: the void* member of the union
 *  @returns: the data union containing `as_ptr'
 */
t_data	as_ptr(void *as_ptr)
{
	return ((t_data){.as_ptr = as_ptr, .type = TYPE_PTR});
}

/** Return a data union initialized with the t_literator* argument.
 *
 *  @param as_literator: the t_literator* member of the union
 *  @returns: the data union containing `as_literator'
 */
t_data	as_literator(t_literator *as_literator)
{
	return ((t_data){.as_literator = as_literator, .type = TYPE_LITERATOR});
}

/** Return a data union initialized with the t_token* argument.
 *
 *  @param as_token: the t_token* member of the union
 *  @returns: the data union containing `as_token'
 */
t_data	as_token(t_token *as_token)
{
	return ((t_data){.as_token = as_token, .type = TYPE_TOKEN});
}

/** Return a data union initialized with the t_tree* argument.
 *
 *  @param as_tree: the t_tree* member of the union
 *  @returns: the data union containing `as_tree'
 */
t_data	as_tree(t_tree *as_tree)
{
	return ((t_data){.as_tree = as_tree, .type = TYPE_TREE});
}
