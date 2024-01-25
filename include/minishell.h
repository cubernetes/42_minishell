/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosuman <timo42@proton.me>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 17:35:26 by tosuman           #+#    #+#             */
/*   Updated: 2024/01/24 19:52:42 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H 1

# define PS0 "> "
# define PS1 "\\u@\\h:\\w\\$ "

typedef struct s_state
{
	char	*ps0;
	char	*ps1;
}			t_state;

#endif /* minishell.h. */
