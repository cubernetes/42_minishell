/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 15:06:03 by tischmid          #+#    #+#             */
/*   Updated: 2023/11/22 15:06:04 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_strcmp(char const *s1, char const *s2)
{
	unsigned char	*us1;
	unsigned char	*us2;

	us1 = (unsigned char *) s1;
	us2 = (unsigned char *) s2;
	while (*us1 && *us2 && *us1 == *us2)
	{
		++us1;
		++us2;
	}
	return (*us1 - *us2);
}
