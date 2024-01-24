/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 15:06:18 by tischmid          #+#    #+#             */
/*   Updated: 2023/11/22 15:06:19 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	total_len;
	char	*joined_str;

	total_len = ft_strlen(s1) + ft_strlen(s2);
	joined_str = ft_calloc(total_len + 1, sizeof(*joined_str));
	if (!joined_str)
		return (0);
	ft_strlcat(joined_str, s1, total_len + 1);
	ft_strlcat(joined_str, s2, total_len + 1);
	return (joined_str);
}
