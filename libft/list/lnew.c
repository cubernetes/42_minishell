/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lnew.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 01:00:06 by tischmid          #+#    #+#             */
/*   Updated: 2024/07/05 01:00:11 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "libft.h"

/** Return newly allocated list.
 *
 *  @returns: the allocated list 
 */
t_list	*lnew(void)
{
	return (ft_memdup(&(t_list){
			._iterator_stack = ft_memdup(&(t_list){0}, sizeof(t_list)), \
		}, sizeof(t_list))
	);
}
