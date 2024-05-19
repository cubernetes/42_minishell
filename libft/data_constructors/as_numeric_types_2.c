#include "libft.h"

#include <sys/types.h>

t_data	as_pid_t(pid_t as_pid_t)
{
	return ((t_data){.as_pid_t = as_pid_t, .type = TYPE_PID_T});
}
