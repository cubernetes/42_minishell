/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosuman <timo42@proton.me>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 09:33:32 by tosuman           #+#    #+#             */
/*   Updated: 2024/02/01 09:40:56 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MEMORY_H
# define MEMORY_H 1

# include "../libft/libft.h"
# include <stddef.h> /* size_t */

void		*ft_malloc(size_t size);
t_ddeque	*manage_ptrs(void *ptr);
t_bool		free_all_ptrs(void);

#endif /* memory.h. */
