/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_replace.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 01:25:01 by tischmid          #+#    #+#             */
/*   Updated: 2024/07/05 01:25:29 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_REPLACE_H
# define FT_REPLACE_H	1

# include "libft.h"

typedef struct s_args
{
	const char	**str;
	size_t		*str_idx;
	size_t		*replacement_len;
	const char	**replacement;
	size_t		*pattern_len;
	size_t		*new_str_idx;
	const char	**pattern;
	char		**new_str;
}				t_args;

#endif /* ft_replace.h */
