/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosuman <timo42@proton.me>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 07:46:23 by tosuman           #+#    #+#             */
/*   Updated: 2024/02/01 07:46:49 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
