/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_debug.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tosuman <timo42@proton.me>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 23:34:44 by tosuman           #+#    #+#             */
/*   Updated: 2024/02/07 13:00:13 by tosuman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_DEBUG_H
# define MINISHELL_DEBUG_H	1

# define REP0(X)
# define REP1(X) X
# define REP2(X) REP1(X) X
# define REP3(X) REP2(X) X
# define REP4(X) REP3(X) X
# define REP5(X) REP4(X) X
# define REP6(X) REP5(X) X
# define REP7(X) REP6(X) X
# define REP8(X) REP7(X) X
# define REP9(X) REP8(X) X
# define REP10(X) REP9(X) X

# define REP(HUNDREDS,TENS,ONES,X) \
	REP##HUNDREDS(REP10(REP10(X))) \
	REP##TENS(REP10(X)) \
	REP##ONES(X)

/* # define DEBUG(lvl, fmt, ...) \ */
	/* ft_printf("<>"); ft_printf("\033[35m[DEBUG]\033[m " \ */
	/* REP(0, 0, lvl, "\t") "\033[31m" \ */
	/* fmt "\033[m\n", ##__VA_ARGS__); ft_printf("</>") */

# define DEBUG(lvl, fmt, ...) \
	ft_printf("\033[35m[DEBUG]\033[m " \
	REP(0, 0, lvl, "\t") "\033[31m" \
	fmt "\033[m\n", ##__VA_ARGS__)

#endif /* minishell_debug.h. */
