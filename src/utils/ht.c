/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ht.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosuman <timo42@proton.me>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/31 12:49:11 by tosuman           #+#    #+#             */
/*   Updated: 2024/04/14 16:11:01 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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

void	ht_set(t_kv ht[TABLE_SIZE], char key[static 1], t_type value)
{
	int		idx;
	t_kv	*new_kv;

	idx = (int)(hash(key) % TABLE_SIZE);
	if (!ht[idx].k)
	{
		ht[idx].k = key;
		ht[idx].v = value;
		ht[idx].n = NULL;
	}
	else
	{
		new_kv = ft_malloc(sizeof(*new_kv));
		new_kv->k = key;
		new_kv->v = value;
		new_kv->n = ht[idx].n;
		ht[idx].n = new_kv;
	}
}

t_type	ht_get(t_kv ht[TABLE_SIZE], char key[static 1])
{
	t_kv	*kv;

	kv = &ht[hash(key) % TABLE_SIZE];
	while (kv && kv->k && ft_strcmp(kv->k, key))
		kv = kv->n;
	if (kv && kv->k)
		return (kv->v);
	return ((t_type){0});
}

/* print must not print any newlines */
void	ht_print(t_kv ht[TABLE_SIZE], void (print)(char *k, void *v))
{
	int		i;
	t_kv	*kv;
	int		j;

	i = -1;
	while (++i < TABLE_SIZE)
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

/* We have garbage collection */
/* void	ht_destroy(t_kv ht[TABLE_SIZE]) */
/* { */
	/* int		i; */
	/* t_kv	*prev; */
	/* t_kv	*kv; */
	/* int		j; */
/*  */
	/* i = -1; */
	/* while (++i < TABLE_SIZE) */
	/* { */
		/* kv = &ht[i]; */
		/* if (kv->k == NULL) */
			/* continue ; */
		/* j = 0; */
		/* while (kv) */
		/* { */
			/* free(kv->k); */
			/* free(kv->v); */
			/* prev = kv; */
			/* kv = kv->n; */
			/* if (j > 0) */
				/* free(prev); */
			/* ++j; */
		/* } */
	/* } */
/* } */
