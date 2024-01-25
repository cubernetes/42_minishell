/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 15:06:35 by tischmid          #+#    #+#             */
/*   Updated: 2024/01/25 22:20:17 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

char	*ft_strmapi(char const *s, char (f)(unsigned int, char))
{
	char				*str;
	unsigned int		idx;

	str = malloc(sizeof(*str) * (ft_strlen(s) + 1));
	if (!str)
		return (NULL);
	idx = (unsigned int)-1;
	while (s[++idx])
		str[idx] = (f)(idx, s[idx]);
	str[idx] = 0;
	return (str);
}
