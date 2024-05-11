#include "libft.h"

/** Return newly allocated list.
 *
 *  @returns: the allocated list 
 */
t_list	*new_list(void)
{
	return (ft_memdup(&(t_list){
			.current_stack = ft_memdup(&(t_list){0}, sizeof(t_list)),
			.current_idx_stack = ft_memdup(&(t_list){0}, sizeof(t_list))
		}, sizeof(t_list))
	);
}
