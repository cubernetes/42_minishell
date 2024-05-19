/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 15:08:28 by tischmid          #+#    #+#             */
/*   Updated: 2024/02/01 11:19:22 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>
#include <limits.h>

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*ret;
	void	*empty;

	if (nmemb * size == 0)
	{
		empty = ft_malloc(0);
		if (!empty)
			return (NULL);
		return (empty);
	}
	if (UINT_MAX / nmemb < size)
		return (NULL);
	ret = ft_malloc(nmemb * size);
	if (!ret)
		return (NULL);
	ft_bzero(ret, nmemb * size);
	return (ret);
}
