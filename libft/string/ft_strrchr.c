/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 15:07:20 by tischmid          #+#    #+#             */
/*   Updated: 2024/01/25 22:23:35 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

char	*ft_strrchr(char const *s, int c)
{
	char	*last;

	last = NULL;
	while (*s)
		if (*s++ == (char) c)
			last = (char *)(s - 1);
	if (*s == (char) c)
		return ((char *) s);
	return (last);
}
