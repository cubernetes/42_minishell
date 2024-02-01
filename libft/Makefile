# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/22 15:09:11 by tischmid          #+#    #+#              #
#    Updated: 2024/02/01 10:42:55 by tosuman          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc
unexport CFLAGS
# TODO: change -O0 to -03 and remove -g3
# TODO: change -std=c99 to -std=c89 (used for variadic macro)
# TODO: add back -pedantic flag
CFLAGS += -O0 -g3 -Wall -Wextra -Werror \
          -std=c99 -Wconversion \
          -Wunused -Wunreachable-code -Wshadow
unexport LDFLAGS
AR = ar rcs
RM = /bin/rm -f
MKDIR = /bin/mkdir -p
LN = /bin/ln -f

NAME = libft.a
SRCDIR = ./src
OBJDIR = ./obj
INCDIR = .

unexport SUBDIRS
SUBDIRS += string
SUBDIRS += memory
SUBDIRS += print
SUBDIRS += list
SUBDIRS += deque
SUBDIRS += io

vpath %.c $(SUBDIRS)

unexport SRC
SRC += string/ft_atoi.c
SRC += string/ft_atoi_status.c
SRC += string/ft_atof.c
SRC += string/ft_isalnum.c
SRC += string/ft_isalpha.c
SRC += string/ft_isascii.c
SRC += string/ft_isdigit.c
SRC += string/ft_isprint.c
SRC += string/ft_strchr.c
SRC += string/ft_strdup.c
SRC += string/ft_strndup.c
SRC += string/ft_strlcat.c
SRC += string/ft_strlcpy.c
SRC += string/ft_strlen.c
SRC += string/ft_strncmp.c
SRC += string/ft_strcmp.c
SRC += string/ft_streq.c
SRC += string/ft_strnstr.c
SRC += string/ft_strrchr.c
SRC += string/ft_tolower.c
SRC += string/ft_toupper.c
SRC += string/ft_itoa.c
SRC += string/ft_strtrim.c
SRC += string/ft_substr.c
SRC += string/ft_strjoin.c
SRC += string/ft_split.c
SRC += string/ft_striteri.c
SRC += string/ft_strmapi.c
SRC += string/ft_char_in_charset.c
SRC += string/ft_isspace.c

SRC += memory/ft_bzero.c
SRC += memory/ft_calloc.c
SRC += memory/ft_memchr.c
SRC += memory/ft_memcmp.c
SRC += memory/ft_memcpy.c
SRC += memory/ft_memmove.c
SRC += memory/ft_memset.c

SRC += print/ft_putchar_fd.c
SRC += print/ft_putendl_fd.c
SRC += print/ft_putnbr_fd.c
SRC += print/ft_putstr_fd.c
SRC += print/ft_putendl.c
SRC += print/ft_putstr.c
SRC += print/ft_putnbr.c
SRC += print/ft_putchar.c
SRC += print/ft_putfmt.c
SRC += print/ft_putfmt_fd.c
SRC += print/ft_puthex_fd.c
SRC += print/ft_putptr_fd.c
SRC += print/ft_puthex.c
SRC += print/ft_putptr.c
SRC += print/ft_printf.c
SRC += print/ft_dprintf.c

SRC += list/ft_lstnew.c
SRC += list/ft_lstadd_front.c
SRC += list/ft_lstsize.c
SRC += list/ft_lstlast.c
SRC += list/ft_lstadd_back.c
SRC += list/ft_lstclear.c
SRC += list/ft_lstdelone.c
SRC += list/ft_lstiter.c
SRC += list/ft_lstmap.c

SRC += math/ft_abs.c
SRC += math/ft_max.c
SRC += math/ft_min.c
SRC += math/cmp_int_desc.c
SRC += math/cmp_int_asc.c

SRC += deque/deque_init.c
SRC += deque/deque_free.c
SRC += deque/deque_print.c
SRC += deque/array_list_to_deque.c
SRC += deque/deque_index.c
SRC += deque/deque_slice.c
SRC += deque/deque_sort.c
SRC += deque/deque_copy.c
SRC += deque/deque_size.c
SRC += deque/deque_extend_free.c
SRC += deque/deque_swap.c
SRC += deque/deque_rotate.c
SRC += deque/deque_pop_top.c
SRC += deque/deque_pop_bottom.c
SRC += deque/deque_push_node_bottom.c
SRC += deque/deque_push_value_bottom.c
SRC += deque/deque_push_node_top.c
SRC += deque/deque_push_value_top.c
SRC += deque/deque_equal.c
SRC += deque/deque_argmax.c

SRC += deque/ddeque_init.c
SRC += deque/ddeque_free.c
SRC += deque/ddeque_print.c
SRC += deque/ddeque_index.c
SRC += deque/ddeque_slice.c
SRC += deque/ddeque_sort.c
SRC += deque/ddeque_shallow_copy.c
SRC += deque/ddeque_size.c
SRC += deque/ddeque_extend_free.c
SRC += deque/ddeque_swap.c
SRC += deque/ddeque_rotate.c
SRC += deque/ddeque_pop_top.c
SRC += deque/ddeque_pop_bottom.c
SRC += deque/ddeque_push_node_bottom.c
SRC += deque/ddeque_push_value_bottom.c
SRC += deque/ddeque_push_node_top.c
SRC += deque/ddeque_push_value_top.c
SRC += deque/ddeque_equal.c

SRC += io/get_next_line.c

SSRC = $(wilcard $(SUBDIRS)/*)

unexport _DEPS
_DEPS += libft.h

OBJ = $(SRC:.c=.o)

INCLUDE = $(addprefix $(INCDIR)/,$(_DEPS))

all: $(NAME)

$(NAME): $(OBJ) | $(SUBDIRS)
	$(AR) $@ $(OBJ)

$(OBJDIR)/%.o: %.c $(INCLUDE) | $(OBJDIR)
	$(CC) $(CFLAGS) -c -o $@ -I$(INCDIR) $<

$(SUBDIRS): | $(SRCDIR)
	$(MKDIR) $(subdir)
	$(LN) -s $(subdir)/*.c $(SRCDIR)/

$(SRCDIR):
	$(MKDIR) $@

$(OBJDIR):
	$(MKDIR) $@

bonus: $(NAME)

clean:
	$(RM) $(OBJ)
	$(RM) -r $(OBJDIR)
	$(RM) -r $(SRCDIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

var-%:
	@echo $($*)

.PHONY: re fclean clean all bonus links
