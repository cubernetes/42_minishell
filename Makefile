# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/22 15:02:16 by tischmid          #+#    #+#              #
#    Updated: 2024/01/29 07:15:08 by tosuman          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Makefile for [ "$(uname -s)" = "Linux" ]

NAME            := minishell
BONUS           := $(NAME)
LIBFT           := libft.a
LIBFT_          := $(patsubst lib%,%,$(patsubst %.a,%,$(LIBFT)))

unexport _SRC
_SRC           += minishell.c

_OBJ           := $(_SRC:.c=.o)
_HEADERS       := minishell.h

SRCDIR         := src
OBJDIR         := obj
INCLUDEDIR     := include
LIBFT_DIR      := ./libft
SRC            := $(addprefix $(SRCDIR)/,$(_SRC))
OBJ            := $(addprefix $(OBJDIR)/,$(_OBJ))
INCLUDE        := $(addprefix $(INCLUDEDIR)/,$(_HEADERS))

CC             := cc
RM             := /bin/rm -f
# TODO: change -O0 to -O3 and remove -g3
# TODO: change -std=c99 to -std=c89 (used for variadic macro)
# TOOD: add back -Werror
CFLAGS         := -O0 -g3 -Wall -Wextra \
                  -std=c99 -pedantic -Wconversion \
                  -Wunused -Wunreachable-code -Wshadow
CPPFLAGS       := -I$(LIBFT_DIR) -I$(INCLUDEDIR)
LDFLAGS        := -L$(LIBFT_DIR)
LDLIBS         := -l$(LIBFT_) -lreadline

all: $(NAME)

$(NAME): $(LIBFT_DIR)/$(LIBFT) $(OBJ)
	$(CC) -o $@ $(LDFLAGS) $(OBJ) $(LDLIBS)
	$(MAKE)

$(LIBFT_DIR)/$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

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

.PHONY: re fclean clean all bonus
