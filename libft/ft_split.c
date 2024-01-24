/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <timo42@proton.me>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 20:37:07 by tischmid          #+#    #+#             */
/*   Updated: 2023/11/22 04:48:26 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

static size_t	split_arr_len(char const *s, char c)
{
	size_t	len;

	len = 0;
	while (*s)
	{
		while (*s == c)
			++s;
		if (!*s)
			return (len);
		++len;
		while (*s != c && *s)
			++s;
	}
	return (len);
}

static int	save_strlcpy(char **arr, size_t len, char const *src, size_t size)
{
	if (!arr[len - 1])
	{
		while (len--)
			free(arr[len]);
		free(arr);
		return (0);
	}
	ft_strlcpy(arr[len - 1], src, size);
	return (1);
}

char	**ft_split(char const *s, char c)
{
	size_t		arr_len;
	size_t		idx;
	char		**arr;
	char const	*start;

	arr_len = split_arr_len(s, c);
	arr = malloc(sizeof(*arr) * (arr_len + 1));
	if (!arr)
		return (0);
	idx = 0;
	while (*s)
	{
		while (*s == c)
			++s;
		if (!*s)
			break ;
		start = s;
		while (*s != c && *s)
			++s;
		arr[idx] = malloc(sizeof(**arr) * (size_t)(s - start + 1));
		if (!save_strlcpy(arr, ++idx, start, (size_t)(s - start + 1)))
			return (0);
	}
	arr[idx] = (void *) 0;
	return (arr);
}
