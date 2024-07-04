/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   as_pointer_types_2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 01:03:41 by tischmid          #+#    #+#             */
/*   Updated: 2024/07/05 01:03:42 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_data	as_var(t_var *as_var)
{
	return ((t_data){.as_var = as_var, .type = TYPE_VAR});
}

t_data	as_gc_ptr(t_var *as_gc_ptr)
{
	return ((t_data){.as_ptr = as_gc_ptr, .type = TYPE_GC_PTR});
}

t_data	as_str_pair(t_str_pair *as_str_pair)
{
	return ((t_data){.as_str_pair = as_str_pair, .type = TYPE_STR_PAIR});
}

t_data	as_kv_pair(t_kv_pair *as_kv_pair)
{
	return ((t_data){.as_kv_pair = as_kv_pair, .type = TYPE_KV_PAIR});
}

t_data	as_list(t_list *as_list)
{
	return ((t_data){.as_list = as_list, .type = TYPE_LIST});
}
