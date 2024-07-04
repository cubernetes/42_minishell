/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_start_context.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 01:04:34 by tischmid          #+#    #+#             */
/*   Updated: 2024/07/05 01:06:13 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*gc_start_context(char *new_ctx_name)
{
	static t_list	*context_stack;
	char			*old_ctx;

	if (new_ctx_name == NULL)
	{
		if (context_stack == NULL)
			return (NULL);
		new_ctx_name = gc_get_context_name();
		old_ctx = lpop(context_stack)->as_str;
		gc_ctx_manager(SET_CONTEXT, old_ctx);
		return (new_ctx_name);
	}
	old_ctx = gc_get_context_name();
	gc_set_context("CTX_MANAGER");
	if (context_stack == NULL)
		context_stack = lnew();
	(void)lpush(context_stack, as_str(old_ctx))->as_str;
	gc_ctx_manager(SET_CONTEXT, new_ctx_name);
	return (old_ctx);
}
