/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosuman <timo42@proton.me>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 09:32:52 by tosuman           #+#    #+#             */
/*   Updated: 2024/02/01 09:42:03 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/memory.h"
#include "../../libft/libft.h"
#include <stdlib.h>

t_bool	dont_free(void *data)
{
	(void)data;
	return (1);
}

t_bool	ft_free(void *ptr)
{
	free(ptr);
	return (TRUE);
}

t_bool	free_all_ptrs(void)
{
	t_ddeque	*ptrs;

	ft_printf("FREEING PTRS\n");
	ptrs = manage_ptrs(NULL);
	ddeque_free(ptrs, ft_free);
	ft_printf("DONE\n");
	return (TRUE);
}
