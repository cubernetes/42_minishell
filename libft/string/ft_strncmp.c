/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 15:07:15 by tischmid          #+#    #+#             */
/*   Updated: 2024/01/26 01:39:35 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

int	ft_strncmp(char const *s1, char const *s2, size_t len)
{
	unsigned char	*us1;
	unsigned char	*us2;

	if (!len)
		return (0);
	us1 = (unsigned char *) s1;
	us2 = (unsigned char *) s2;
	while (*us1 && *us2 && *us1 == *us2 && --len)
	{
		us1++;
		us2++;
	}
	if (*us1 != *us2)
		return ((*us1 - *us2 != 0)
			* (-2 * (*us1 - *us2 < 0) + 1));
	return (0);
}
