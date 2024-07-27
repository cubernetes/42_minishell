# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pgrussin <pgrussin@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/22 15:02:16 by tischmid          #+#    #+#              #
#    Updated: 2024/07/27 16:55:51 by tosuman          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# TODO: improve makefile
#
# TODO: adapt for MacOS
# Makefile for [ "$(uname -s)" = "Linux" ]

# config
NAME := minishell
SRCDIR := src
OBJDIR := obj
LIBFT_DIR := libft

# tools
CC := cc
RM := /bin/rm -f
MKDIR := /bin/mkdir -p

# flags
CFLAGS := -O2 -Wall -Wextra -Werror \
		-pedantic -Wconversion \
		-Wunreachable-code -Wshadow
CPPFLAGS := -MMD -I$(LIBFT_DIR) -I$(SRCDIR)
LDFLAGS := -L$(LIBFT_DIR)
LDLIBS := -lreadline -l$(LIFBT_LIB)
ifeq ($(DEBUG), 1)
	CFLAGS += -ggdb3 -O0
	LDFLAGS += -ggdb3 -O0
endif

ifeq ($(ASAN), 1)
	CFLAGS += -fsanitize=address
	LDFLAGS += -fsanitize=address
endif

ifeq ($(TSAN), 1)
	CFLAGS += -fsanitize=thread
	LDFLAGS += -fsanitize=thread
endif

# sources
SRC :=
vpath %.c src
SRC += main.c
SRC += signals1.c
SRC += signals2.c
SRC += shell_vars.c
SRC += shell_opt_init.c
SRC += joining.c
SRC += glob.c
SRC += environment.c

vpath %.c src/builtins
SRC += echo.c
SRC += unset.c
SRC += pwd.c
SRC += env.c
SRC += exit.c
SRC += cd.c
SRC += cd2.c
SRC += shift.c
SRC += readonly.c
SRC += source.c
SRC += colon.c
SRC += export.c

vpath %.c src/builtins/declare
SRC += declare1.c
SRC += declare2.c
SRC += declare3.c
SRC += declare4.c
SRC += declare5.c
SRC += declare_set.c

vpath %.c src/exec
SRC += execve.c
SRC += execve2.c
SRC += execve3.c
SRC += execve4.c
SRC += execve5.c
SRC += execve6.c
SRC += search_executable.c
SRC += executor.c
SRC += executor2.c
SRC += executor3.c
SRC += executor4.c
SRC += executor5.c
SRC += executor6.c
SRC += executor7.c

vpath %.c src/expansion
SRC += expansion1.c
SRC += expansion2.c
SRC += expansion3.c
SRC += expansion4.c
SRC += expansion5.c

vpath %.c src/parsing
SRC += hashtable.c
SRC += get_production_idx.c
SRC += build_ast1.c
SRC += build_ast2.c
SRC += build_ast3.c
SRC += productions1.c
SRC += productions2.c
SRC += printing.c
SRC += new_tree_tokens.c
SRC += heredoc.c
SRC += heredoc_helper.c
SRC += helper.c

vpath %.c src/shell
SRC += minishell.c
SRC += minishell2.c
SRC += minishell3.c
SRC += minishell4.c
SRC += minishell5.c
SRC += minishell6.c
SRC += minishell7.c
SRC += minishell_error.c

vpath %.c src/tokenization
SRC += tokenize1.c
SRC += tokenize2.c
SRC += tokenize3.c
SRC += tokenize4.c
SRC += tokenize5.c
SRC += tokenize6.c
SRC += tokenize7.c

vpath %.c src/unistd
SRC += get_cwd_for_prompt.c
SRC += ft_gethostname.c
SRC += ft_getuid.c
SRC += ft_getusername.c
SRC += get_username_from_krb.c
SRC += ft_getpid.c
SRC += ft_getumask.c
SRC += ft_mktemp.c

# objects
OBJ := $(SRC:.c=.o)
OBJ := $(addprefix $(OBJDIR)/,$(OBJ))

# dependencies
DEPS := $(OBJ:.o=.d)
-include $(DEPS)

# libs
LIBFT_FILE := libft.a
LIFBT_LIB := $(patsubst lib%,%,$(patsubst %.a,%,$(LIBFT_FILE)))

# rules
.DEFAULT_GOAL := all

all: libft $(NAME)

libft:
	$(MAKE) -C $(LIBFT_DIR)

$(NAME): $(LIBFT_DIR)/$(LIBFT_FILE) $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS) $(LDLIBS)

$(OBJDIR)/%.o: %.c | $(OBJDIR)
	$(CC) $< -c -o $@ $(CPPFLAGS) $(CFLAGS)

$(OBJDIR):
	$(MKDIR) $@

clean:
	$(MAKE) -C $(LIBFT_DIR) $@
	$(RM) $(OBJ)
	$(RM) $(DEPS)
	$(RM) -r $(OBJDIR)

fclean: clean
	$(MAKE) -C $(LIBFT_DIR) $@
	$(RM) $(NAME)

re: fclean all

.PHONY: re fclean clean all libft
