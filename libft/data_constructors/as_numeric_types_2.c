/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   as_numeric_types_2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 01:03:43 by tischmid          #+#    #+#             */
/*   Updated: 2024/07/05 01:03:44 by tischmid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

#include <sys/types.h>

t_data	as_pid_t(pid_t as_pid_t)
{
	return ((t_data){.as_pid_t = as_pid_t, .type = TYPE_PID_T});
}

t_data	as_getopt_arg(int as_getopt_arg)
{
	return ((t_data){.as_getopt_arg = as_getopt_arg, .type = TYPE_GETOPT_ARG});
}
