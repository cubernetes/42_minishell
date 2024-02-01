/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putptr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <timo42@proton.me>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 16:51:47 by tischmid          #+#    #+#             */
/*   Updated: 2023/10/05 23:10:27 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

int	ft_putptr(void *ptr)
{
	if (!ptr)
		return (ft_putstr(NIL_PTR_STR));
	return (ft_putchar('0') + \
			ft_putchar('x') + \
			ft_puthex((unsigned long)ptr, 0));
}
