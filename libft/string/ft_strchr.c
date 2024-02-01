/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 15:06:01 by tischmid          #+#    #+#             */
/*   Updated: 2024/01/25 20:31:07 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

char	*ft_strchr(char const *s, int c)
{
	while (*s)
		if (*s++ == (unsigned char) c)
			return ((char *)--s);
	if (*s == (unsigned char) c)
		return ((char *) s);
	else
		return (NULL);
}
