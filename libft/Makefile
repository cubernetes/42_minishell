# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/22 15:09:11 by tischmid          #+#    #+#              #
#    Updated: 2024/03/26 05:10:10 by tosuman          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = clang
AR = ar rcs
RM = /bin/rm -f
MKDIR = /bin/mkdir -p
LN = /bin/ln -f

NAME = libft.a
OBJDIR = ./obj
INCDIR = .

# TODO: change -O0 to -03 and remove -g3
# TODO: add back -Werror
CFLAGS = -O0 -g3 -fPIE -Wall -Wextra \
          -std=c99 -pedantic -Wconversion \
          -Wunreachable-code -Wshadow \
          -fdiagnostics-color=always
CPPFLAGS = -I$(INCDIR)

unexport SUBDIRS
SUBDIRS += string
SUBDIRS += memory
SUBDIRS += list
SUBDIRS += deque
SUBDIRS += math
SUBDIRS += io
SUBDIRS += misc

vpath %.c $(SUBDIRS)

# misc
SRC += error_handling.c

# string
unexport SRC
SRC += ft_atoi.c
SRC += ft_atoi_status.c
SRC += ft_atof.c
SRC += ft_isalnum.c
SRC += ft_isalpha.c
SRC += ft_isascii.c
SRC += ft_isdigit.c
SRC += ft_isprint.c
SRC += ft_strchr.c
SRC += ft_strdup.c
SRC += ft_strndup.c
SRC += ft_strlcat.c
SRC += ft_strlcpy.c
SRC += ft_strlen.c
SRC += ft_strncmp.c
SRC += ft_strcmp.c
SRC += ft_streq.c
SRC += ft_strnstr.c
SRC += ft_strrchr.c
SRC += ft_tolower.c
SRC += ft_toupper.c
SRC += ft_itoa.c
SRC += ft_strtrim.c
SRC += ft_substr.c
SRC += ft_strjoin.c
SRC += ft_split.c
SRC += ft_striteri.c
SRC += ft_strmapi.c
SRC += ft_char_in_charset.c
SRC += ft_isspace.c

# memory
SRC += ft_bzero.c
SRC += ft_calloc.c
SRC += ft_memchr.c
SRC += ft_memcmp.c
SRC += ft_memcpy.c
SRC += ft_memmove.c
SRC += ft_memset.c
SRC += error_handling.c
SRC += ft_malloc.c
SRC += ft_malloc_ddeque_free.c
SRC += ft_malloc_ddeque_init.c
SRC += ft_malloc_ddeque_push_value_bottom.c

# io
SRC += get_next_line.c
SRC += ft_putchar_fd.c
SRC += ft_putendl_fd.c
SRC += ft_putnbr_fd.c
SRC += ft_putstr_fd.c
SRC += ft_putendl.c
SRC += ft_putstr.c
SRC += ft_putnbr.c
SRC += ft_putchar.c
SRC += ft_putfmt.c
SRC += ft_putfmt_fd.c
SRC += ft_puthex_fd.c
SRC += ft_putptr_fd.c
SRC += ft_puthex.c
SRC += ft_putptr.c
SRC += ft_printf.c
SRC += ft_dprintf.c

# list
SRC += ft_lstnew.c
SRC += ft_lstadd_front.c
SRC += ft_lstsize.c
SRC += ft_lstlast.c
SRC += ft_lstadd_back.c
SRC += ft_lstclear.c
SRC += ft_lstdelone.c
SRC += ft_lstiter.c
SRC += ft_lstmap.c

# math
SRC += ft_max.c
SRC += ft_abs.c
SRC += ft_min.c
SRC += cmp_int_desc.c
SRC += cmp_int_asc.c

# deque
SRC += deque_init.c
SRC += deque_print.c
SRC += array_list_to_deque.c
SRC += deque_index.c
SRC += deque_slice.c
SRC += deque_sort.c
SRC += deque_copy.c
SRC += deque_size.c
SRC += deque_extend.c
SRC += deque_swap.c
SRC += deque_rotate.c
SRC += deque_pop_top.c
SRC += deque_pop_bottom.c
SRC += deque_push_node_bottom.c
SRC += deque_push_value_bottom.c
SRC += deque_push_node_top.c
SRC += deque_push_value_top.c
SRC += deque_equal.c
SRC += deque_argmax.c

SRC += ddeque_init.c
SRC += ddeque_print.c
SRC += ddeque_print_debug.c
SRC += array_list_to_ddeque.c
SRC += ddeque_index.c
SRC += ddeque_slice.c
SRC += ddeque_sort.c
SRC += ddeque_shallow_copy.c
SRC += ddeque_size.c
SRC += ddeque_extend.c
SRC += ddeque_swap.c
SRC += ddeque_rotate.c
SRC += ddeque_pop_top.c
SRC += ddeque_pop_bottom.c
SRC += ddeque_push_node_bottom.c
SRC += ddeque_push_value_bottom.c
SRC += ddeque_push_node_top.c
SRC += ddeque_push_value_top.c
SRC += ddeque_equal.c
SRC += ddeque_iter.c
SRC += ddeque_sum.c

_OBJ = $(SRC:.c=.o)
OBJ = $(addprefix $(OBJDIR)/,$(_OBJ))

unexport _DEPS
_DEPS += libft.h
INCLUDE = $(addprefix $(INCDIR)/,$(_DEPS))

all: $(NAME)

$(NAME): $(OBJ)
	$(AR) $@ $(OBJ)

$(OBJDIR)/%.o: %.c $(INCLUDE) | $(OBJDIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

$(OBJDIR):
	$(MKDIR) $@

bonus: $(NAME)

clean:
	$(RM) $(OBJ)
	$(RM) -r $(OBJDIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: re fclean clean all bonus
