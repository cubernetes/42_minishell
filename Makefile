# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/22 15:02:16 by tischmid          #+#    #+#              #
#    Updated: 2024/04/14 17:21:12 by tosuman          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Makefile for [ "$(uname -s)" = "Linux" ]

# TODO: Check gcc and clang
CC             := cc
RM             := /bin/rm -f
MKDIR          := /bin/mkdir -p

NAME            := minishell
BONUS           := $(NAME)
LIBFT           := libft.a
LIBFT_          := $(patsubst lib%,%,$(patsubst %.a,%,$(LIBFT)))

unexport _SRC
_SRC           += minishell.c
_SRC           += signal_handling.c
_SRC           += parsing.c
_SRC           += tokenize.c
_SRC           += expansion.c
_SRC           += globbing.c
_SRC           += executor.c
_SRC           += execve.c
_SRC           += ht.c
_SRC           += env.c

vpath %.c src src/parsing src/signal_handling src/tokenization \
          src/expansion src/execution/ src/utils/

_OBJ           := $(_SRC:.c=.o)
_HEADERS       := minishell.h

OBJDIR         := obj
INCLUDEDIR     := include
LIBFT_DIR      := ./libft
SRC            := $(addprefix $(SRCDIR)/,$(_SRC))
OBJ            := $(addprefix $(OBJDIR)/,$(_OBJ))
INCLUDE        := $(addprefix $(INCLUDEDIR)/,$(_HEADERS))

# TODO: change -O0 to -O3 and remove -g3
# TOOD: add back -Werror
# TODO: add back std=c99 if possible
CFLAGS         := -O0 -g3 -fPIE -Wall -Wextra \
                  -pedantic -Wconversion \
                  -Wunreachable-code -Wshadow \
                  -fdiagnostics-color=always
CPPFLAGS       := -I$(LIBFT_DIR) -I$(INCLUDEDIR)
LDFLAGS        := -L$(LIBFT_DIR) -rdynamic
LDLIBS         := -l$(LIBFT_) -lreadline

all: libft $(NAME)

$(NAME): $(LIBFT_DIR)/$(LIBFT) $(OBJ)
	$(CC) -o $@ $(LDFLAGS) $(OBJ) $(LDLIBS)
	$(MAKE)

libft:
	$(MAKE) -C $(LIBFT_DIR)

$(OBJDIR)/%.o: %.c $(INCLUDE) | $(OBJDIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ -I$(INCLUDEDIR) $<

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
