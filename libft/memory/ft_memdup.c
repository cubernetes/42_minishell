/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosuman <timo42@proton.me>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 19:20:25 by tosuman           #+#    #+#             */
/*   Updated: 2024/03/29 19:22:31 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"
#include <stddef.h>

void	*ft_memdup(const void *src, size_t size)
{
	void	*dest;

	dest = ft_malloc(size);
	ft_memcpy(dest, src, size);
	return (dest);
}
