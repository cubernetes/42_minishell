/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_helper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 18:45:30 by tischmid          #+#    #+#             */
/*   Updated: 2024/07/04 18:45:48 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

bool	heredoc_aborted(int op)
{
	static bool	aborted = false;

	if (op == 1)
		aborted = true;
	else if (op == 0)
		aborted = false;
	return (aborted);
}
