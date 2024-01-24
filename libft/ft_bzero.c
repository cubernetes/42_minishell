/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 15:04:16 by tischmid          #+#    #+#             */
/*   Updated: 2023/11/22 15:04:17 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

void	ft_bzero(void *ptr, size_t n)
{
	char	*p;

	p = (char *)ptr;
	while (n--)
		*p++ = (unsigned char) 0;
}
