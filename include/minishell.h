/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosuman <timo42@proton.me>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 17:35:26 by tosuman           #+#    #+#             */
/*   Updated: 2024/01/25 22:12:41 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H 1

# define PS0 "> "
# define PS1 "\\u@\\h:\\w\\$ "

# define ERR_NEW_TOKEN "Error while creating new token."

typedef struct s_state
{
	char	*ps0;
	char	*ps1;
}			t_state;

#endif /* minishell.h. */
