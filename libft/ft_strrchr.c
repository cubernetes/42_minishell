/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 15:07:20 by tischmid          #+#    #+#             */
/*   Updated: 2023/11/22 15:07:21 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

char	*ft_strrchr(char const *s, int c)
{
	char	*last;

	last = 0;
	while (*s)
		if (*s++ == (char) c)
			last = (char *)(s - 1);
	if (*s == (char) c)
		return ((char *) s);
	return (last);
}
