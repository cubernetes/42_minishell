/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_streq.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 15:06:03 by tischmid          #+#    #+#             */
/*   Updated: 2024/01/16 15:50:49 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* negation of ft_strcmp for lack of lamdbas in c... */
int	ft_streq(char const *s1, char const *s2)
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
	return (!(*us1 - *us2));
}
