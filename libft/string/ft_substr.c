/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 15:07:24 by tischmid          #+#    #+#             */
/*   Updated: 2024/02/01 11:21:15 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"
#include <stdlib.h>

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	unsigned int	idx;
	size_t			real_len;
	char			*substr;
	char			*empty;

	idx = (unsigned int)-1;
	while (s[++idx] && idx < start)
		;
	if (!s[idx])
	{
		empty = ft_calloc(1, sizeof(*empty));
		if (!empty)
			return (NULL);
		return (empty);
	}
	real_len = 0;
	while (++real_len <= len && s[idx + real_len - 1])
		;
	substr = ft_malloc(sizeof(*substr) * real_len);
	if (!substr)
		return (NULL);
	ft_strlcpy(substr, s + idx, real_len);
	return (substr);
}
