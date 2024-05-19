/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ht.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosuman <timo42@proton.me>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/31 12:49:11 by tosuman           #+#    #+#             */
/*   Updated: 2024/05/19 06:41:35 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include <stdlib.h>

static uint64_t	fnv_1a_64(char *input_str)
{
	unsigned char	*str;
	uint64_t		hash;

	str = (unsigned char *)input_str;
	hash = OFFSET_BASIS_64;
	while (*str)
	{
		hash ^= (unsigned long)*str;
		hash *= FNV_PRIME_64;
		str++;
	}
	return (hash);
}

static uint64_t	hash(char *key)
{
	return (fnv_1a_64(key));
}

void	ht_set(t_kv *ht[MAX_HT_SIZE], char key[static 1], t_type value)
{
	int		idx;
	t_kv	*new_kv;
	t_kv	**kv;

	idx = (int)(hash(key) % MAX_HT_SIZE);
	kv = &ht[idx];
	while (*kv && (*kv)->k && ft_strcmp((*kv)->k, key))
		(*kv) = (*kv)->n;

	if (*kv && (*kv)->k)
	{
		free(((t_var *)(*kv)->v.as_ptr)->value);
		free((*kv)->v.as_ptr);
		/* free(key); */ /* TODO: watch out! */
		(*kv)->v = value;
		return ;
	}
	new_kv = ft_malloc(sizeof(*new_kv));
	new_kv->k = key;
	new_kv->v = value;
	new_kv->n = ht[idx];
	ht[idx] = new_kv;
}

t_type	ht_get(t_kv *ht[MAX_HT_SIZE], char key[static 1])
{
	t_kv	*kv;

	kv = ht[hash(key) % MAX_HT_SIZE];
	while (kv && kv->k && ft_strcmp(kv->k, key))
		kv = kv->n;
	if (kv && kv->k)
		return (kv->v);
	return ((t_type){0});
}

/* print() must not print any newlines */
void	ht_print(t_kv ht[MAX_HT_SIZE], void (print)(char *k, void *v))
{
	int		i;
	t_kv	*kv;
	int		j;

	i = -1;
	while (++i < MAX_HT_SIZE)
	{
		ft_printf("%d: ", i);
		kv = &ht[i];
		j = 0;
		while (1)
		{
			if (!kv || !kv->k)
				break ;
			if (j++ > 0)
				ft_printf(" -> ");
			print(kv->k, kv->v.as_ptr);
			kv = kv->n;
		}
		ft_printf("\n");
	}
}

/* TODO: ONLY add malloc'd strings to ht */
void	ht_destroy(t_kv *ht[MAX_HT_SIZE])
{
	int		i;
	t_kv	*prev;
	t_kv	*kv;

	i = -1;
	while (++i < MAX_HT_SIZE)
	{
		kv = ht[i];
		while (kv)
		{
			/* free(kv->k); */ /* TODO: watch out! */
			free(((t_var *)kv->v.as_ptr)->value);
			free(kv->v.as_ptr);
			prev = kv;
			kv = kv->n;
			free(prev);
		}
	}
}
