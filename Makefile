# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pgrussin <pgrussin@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/22 15:02:16 by tischmid          #+#    #+#              #
#    Updated: 2024/07/10 21:44:33 by tischmid         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# TODO: adapt for MacOS
# Makefile for [ "$(uname -s)" = "Linux" ]

# TODO: Check gcc and clang
CC             ?= cc
RM             := /bin/rm -f
MKDIR          := /bin/mkdir -p

NAME            := minishell
BONUS           := $(NAME)
LIBFT           := libft.a
LIBFT_          := $(patsubst lib%,%,$(patsubst %.a,%,$(LIBFT)))

unexport _SRC
_SRC += signals1.c
_SRC += signals2.c
_SRC += get_cwd_for_prompt.c
_SRC += ft_gethostname.c
_SRC += ft_getuid.c
_SRC += ft_getusername.c
_SRC += get_username_from_krb.c
_SRC += ft_getpid.c
_SRC += ft_getumask.c
_SRC += expansion1.c
_SRC += expansion2.c
_SRC += expansion3.c
_SRC += expansion4.c
_SRC += expansion5.c
_SRC += joining.c
_SRC += execve.c
_SRC += execve2.c
_SRC += execve3.c
_SRC += execve4.c
_SRC += execve5.c
_SRC += execve6.c
_SRC += tokenize1.c
_SRC += tokenize2.c
_SRC += tokenize3.c
_SRC += tokenize4.c
_SRC += tokenize5.c
_SRC += tokenize6.c
_SRC += tokenize7.c
_SRC += printing.c
_SRC += new_tree_tokens.c
_SRC += helper.c
_SRC += get_production_idx.c
_SRC += hashtable.c
_SRC += productions1.c
_SRC += productions2.c
_SRC += build_ast1.c
_SRC += build_ast2.c
_SRC += build_ast3.c
_SRC += executor.c
_SRC += executor2.c
_SRC += executor3.c
_SRC += executor4.c
_SRC += executor5.c
_SRC += executor6.c
_SRC += shell_vars.c
_SRC += glob.c
_SRC += minishell.c
_SRC += minishell2.c
_SRC += minishell3.c
_SRC += minishell4.c
_SRC += minishell5.c
_SRC += minishell6.c
_SRC += minishell7.c
_SRC += echo.c
_SRC += unset.c
_SRC += pwd.c
_SRC += env.c
_SRC += exit.c
_SRC += cd.c
_SRC += cd2.c
_SRC += shift.c
_SRC += declare1.c
_SRC += declare2.c
_SRC += declare3.c
_SRC += declare4.c
_SRC += declare5.c
_SRC += readonly.c
_SRC += source.c
_SRC += colon.c
_SRC += export.c
_SRC += environment.c
_SRC += ft_mktemp.c
_SRC += heredoc.c
_SRC += heredoc_helper.c
_SRC += shell_opt_init.c
_SRC += minishell_error.c

vpath %.c                \
	src                  \
	src/parsing/         \
	src/tokenization/    \
	src/expansion/       \
	src/exec/            \
	src/builtins/        \
	src/builtins/declare \
	src/unistd/

_OBJ             := $(_SRC:.c=.o)
_HEADERS         := minishell.h

OBJDIR           := ./obj
SRCDIR           := ./src
LIBFT_DIR        := ./libft
SRC              := $(addprefix $(SRCDIR)/,$(_SRC))
OBJ              := $(addprefix $(OBJDIR)/,$(_OBJ))
MINISHELL_HEADER := $(addprefix $(SRCDIR)/,$(_HEADERS))

# TODO: improve makefile
# TODO: change -O0 to -O3 and remove -g3
# TOOD: add back -Werror
# TODO: add back std=c99 if possible
CFLAGS           := -O0 -g3 -fPIE -Wall -Wextra \
                    -pedantic -Wconversion \
                    -Wunreachable-code -Wshadow \
                    -fdiagnostics-color=always
CPPFLAGS         := -I$(LIBFT_DIR) -I$(SRCDIR)

LDFLAGS          := -L$(LIBFT_DIR)
# TODO: Remove -rdynamic # for backtrace
LDLIBS           := -l$(LIBFT_) -lreadline -rdynamic

all: libft $(NAME)

$(NAME): $(LIBFT_DIR)/$(LIBFT) $(OBJ)
	$(CC) -o $@ $(LDFLAGS) $(OBJ) $(LDLIBS)

libft:
	$(MAKE) -C $(LIBFT_DIR)

$(OBJDIR)/%.o: %.c $(MINISHELL_HEADER) | $(OBJDIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

$(OBJDIR):
	$(MKDIR) $@

bonus: $(BONUS)

clean:
	$(MAKE) -C $(LIBFT_DIR) $@
	$(RM) $(OBJ)
	$(RM) -r $(OBJDIR)

fclean: clean
	$(MAKE) -C $(LIBFT_DIR) $@
	$(RM) $(NAME)
	$(RM) $(BONUS)

re: fclean all

.PHONY: re fclean clean all bonus libft
