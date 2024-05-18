# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/22 15:09:11 by tischmid          #+#    #+#              #
#    Updated: 2024/05/18 19:04:29 by tischmid         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# TODO: Check gcc and clang
CC = cc
AR = ar rcs
RM = /bin/rm -f
MKDIR = /bin/mkdir -p
LN = /bin/ln -f

NAME = libft.a
OBJDIR = ./obj
INCDIR = .
LISTDIR = ./list

# TODO: change -O0 to -03 and remove -g3
# TODO: add back -Werror
CFLAGS = -O0 -g3 -fPIE -Wall -Wextra \
          -pedantic -Wconversion \
          -Wunreachable-code -Wshadow \
          -fdiagnostics-color=always
CPPFLAGS = -I$(INCDIR) -I$(LISTDIR)

unexport SUBDIRS
SUBDIRS += string
SUBDIRS += memory
SUBDIRS += list
SUBDIRS += deque
SUBDIRS += math
SUBDIRS += io
SUBDIRS += misc

vpath %.c $(SUBDIRS)
unexport SRC

# misc
SRC += error_handling.c
SRC += cmt.c

# string
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
SRC += ft_memdup.c
SRC += ft_memmove.c
SRC += ft_memset.c
SRC += error_handling.c
SRC += ft_malloc.c
SRC += ft_malloc_deque_free.c
SRC += ft_malloc_deque_init.c
SRC += ft_malloc_deque_push_ptr_right.c

# io
SRC += ft_putchar_fd.c
SRC += ft_putendl_fd.c
SRC += ft_putnbr_fd.c
SRC += ft_putstr_fd.c
SRC += ft_putendl.c
SRC += ft_putstr.c
SRC += ft_putstr_n.c
SRC += ft_putstr_n_clr.c
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
SRC += ft_print_memory.c

# math
SRC += ft_max.c
SRC += ft_abs.c
SRC += ft_min.c
SRC += cmp_int_desc.c
SRC += cmp_int_asc.c

# list
SRC += lswap.c
SRC += lsplit.c
SRC += lsort.c
SRC += lrotate.c
SRC += lpush_left.c
SRC += lpush.c
SRC += lprint.c
SRC += lprint_rev.c
SRC += lpop_left.c
SRC += lpop.c
SRC += lnext.c
SRC += lprev.c
SRC += lnew.c
SRC += llast.c
SRC += liter.c
SRC += liter_rev.c
SRC += lequal.c
SRC += ldestroy.c
SRC += lcopy.c
SRC += deque_extend_right.c
SRC += deque_extend_left.c
SRC += as_pointer_types.c
SRC += as_numeric_types.c
SRC += as_data.c

# deque
SRC += deque_init.c
SRC += deque_print.c
SRC += deque_print_debug.c
SRC += string_list_to_deque.c
SRC += deque_index.c
SRC += deque_slice.c
SRC += deque_sort.c
SRC += deque_shallow_copy.c
SRC += deque_size.c
SRC += deque_extend_right.c
SRC += deque_extend_left.c
SRC += deque_swap.c
SRC += deque_rotate.c
SRC += deque_pop_left.c
SRC += deque_pop_right.c
SRC += deque_push_node_right.c
SRC += deque_push_ptr_right.c
SRC += deque_push_node_left.c
SRC += deque_push_ptr_left.c
SRC += deque_equal.c
SRC += deque_iter.c
SRC += deque_sum.c
SRC += di_begin.c
SRC += di_get.c
SRC += di_next.c

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
