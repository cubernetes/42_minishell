/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_char_in_charset.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <timo42@proton.me>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 19:43:41 by tischmid          #+#    #+#             */
/*   Updated: 2023/06/02 20:38:04 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_char_in_charset(char c, char const *charset)
{
	while (*charset)
		if (*charset++ == c)
			return (1);
	return (0);
}
