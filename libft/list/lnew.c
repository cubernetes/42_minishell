#include "libft.h"
#include "libft.h"

/** Return newly allocated list.
 *
 *  @returns: the allocated list 
 */
t_list	*lnew(void)
{
	return (ft_memdup(&(t_list){
			._iterator_stack = ft_memdup(&(t_list){0}, sizeof(t_list)),
		}, sizeof(t_list))
	);
}
