#include "libft.h"
#include <stdlib.h>
#include <strings.h>

static bool	ldestroy_ht(t_data data)
{
	return (ldestroy(data.as_list));
}

t_ctx_meta	gc_ctx_manager(t_method method, char *_ctx_name)
{
	static t_ht	*ctxs[MAX_HT_SIZE];
	static char	*ctx_name = "DEFAULT";
	void		*(*prev_allocator)(size_t);

	prev_allocator = get_allocator();
	set_allocator(malloc);
	if (method == GET_CONTEXT)
	{
		if (ht_get(ctxs, ctx_name).as_list == NULL)
			ht_set(ctxs, ctx_name, as_list(lnew()));
		return (set_allocator(prev_allocator), (t_ctx_meta){.ctx_name = ctx_name, .ctx = ht_get(ctxs, ctx_name).as_list, .ctxs = &ctxs});
	}
	else if (method == SET_CONTEXT)
		return (set_allocator(prev_allocator), ctx_name = _ctx_name, gc_ctx_manager(GET_CONTEXT, NULL));
	else if (method == DESTROY_CONTEXT)
	{
		ldestroy(ht_get(ctxs, _ctx_name).as_list);
		ht_unset(ctxs, _ctx_name);
	}
	else if (method == DESTROY_CURRENT_CONTEXT)
	{
		ldestroy(ht_get(ctxs, ctx_name).as_list);
		ht_unset(ctxs, ctx_name);
	}
	else if (method == DESTROY_ALL_CONTEXTS)
	{
		ht_destroy(ctxs, ldestroy_ht);
		bzero(ctxs, sizeof(*ctxs) * MAX_HT_SIZE);
	}
	return (set_allocator(prev_allocator), (t_ctx_meta){0});
}
