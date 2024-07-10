/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor6.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 21:39:16 by tischmid          #+#    #+#             */
/*   Updated: 2024/07/10 21:53:14 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

t_list	*flatten_children(t_tree *simple_command)
{
	t_list	*new_chldn;

	new_chldn = lnew();
	liter(simple_command->children);
	while (lnext(simple_command->children))
	{
		if (simple_command->children->current->as_tree->type == IO_REDIRECT
			&& liter(simple_command->children->current->as_tree->children))
			while (lnext(simple_command->children->current->as_tree->children))
				lpush(new_chldn, as_token(simple_command->children->current
						->as_tree->children->current->as_tree->token));
		else
			lpush(new_chldn, as_token(simple_command->children->current
					->as_tree->token));
	}
	return (new_chldn);
}

t_list	*expand_join_glob(t_list *new_chldn)
{
	lpush(new_chldn, as_token(new_token("", TOK_EOL, true)));
	new_chldn = expand_tokens(new_chldn);
	if (new_chldn == NULL)
		return (NULL);
	join_tokens(new_chldn);
	if (!option_enabled('f'))
		glob_tokens(new_chldn);
	return (new_chldn);
}

t_list	*clean_chldn(t_list *new_chldn, t_tree *simple_command)
{
	t_list	*cleaned_tokens;

	cleaned_tokens = lnew();
	liter(new_chldn);
	while (lnext(new_chldn))
	{
		if ((new_chldn->current->as_token->type == TOK_INPUT
				|| new_chldn->current->as_token->type == TOK_OVERRIDE
				|| new_chldn->current->as_token->type == TOK_APPEND
				|| new_chldn->current->as_token->type == TOK_HEREDOC)
			&& new_chldn->current->next->as_token->type == TOK_WORD
			&& new_chldn->current->next->as_token->num_tokens_after_split == 0)
			return (close_fds(simple_command), minishell_error(1, 0,
					"%s: ambiguous redirect",
					new_chldn->current->next->as_token->origin), NULL);
		if (new_chldn->current->as_token->num_tokens_after_split != 0
			|| new_chldn->current->as_token->type == TOK_EOL)
			lpush(cleaned_tokens, as_data(new_chldn->current));
	}
	return (cleaned_tokens);
}

/* TODO: get rid of this wrapper mess */
/* TOOD: abstract away into new function */
pid_t	execute_simple_command_wrapper(t_tree *simple_command,
	t_list *commands)
{
	t_list	*new_chldn;

	new_chldn = flatten_children(simple_command);
	new_chldn = expand_join_glob(new_chldn);
	if (new_chldn == NULL)
		return (-256);
	new_chldn = clean_chldn(new_chldn, simple_command);
	if (new_chldn == NULL)
		return (-256);
	if (new_chldn->len <= 1)
		return (close_fds(simple_command), -258);
	new_chldn = build_ast(new_chldn, false)->children->first->as_tree
		->children->first->as_tree->children;
	simple_command->children = new_chldn;
	liter(simple_command->children);
	while (lnext(simple_command->children))
		if (simple_command->children->current->as_tree->type == IO_REDIRECT)
			if (handle_io_redirect(simple_command->children->current->as_tree,
					simple_command))
				return (close_fds(simple_command), simple_command->error - 257);
	return (execute_simple_command(simple_command, commands));
}
