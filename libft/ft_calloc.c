/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 15:08:28 by tischmid          #+#    #+#             */
/*   Updated: 2023/11/22 15:08:28 by tischmid         ###   ########.fr       */
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
		empty = malloc(0);
		if (!empty)
			return (0);
		return (empty);
	}
	if (UINT_MAX / nmemb < size)
		return (0);
	ret = malloc(nmemb * size);
	if (!ret)
		return (0);
	ft_bzero(ret, nmemb * size);
	return (ret);
}
