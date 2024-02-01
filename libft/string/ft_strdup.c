/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 15:06:11 by tischmid          #+#    #+#             */
/*   Updated: 2024/01/25 22:21:06 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"
#include <stdlib.h>

char	*ft_strdup(char const *s)
{
	char	*s2;
	size_t	len;

	len = ft_strlen(s);
	s2 = malloc(sizeof(*s2) * (len + 1));
	if (!s2)
		return (NULL);
	ft_strlcpy(s2, (char *) s, len + 1);
	return (s2);
}
