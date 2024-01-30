# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tischmid <tischmid@student.42berlin.de>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/22 15:09:11 by tischmid          #+#    #+#              #
#    Updated: 2024/01/30 18:24:20 by tosuman          ###   ########.fr        #
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

NAME = libft.a
OBJDIR = obj
SRCDIR = .
INCDIR = .

unexport _SRC
_SRC += ft_atoi.c
_SRC += ft_atoi_status.c
_SRC += ft_atof.c
_SRC += ft_bzero.c
_SRC += ft_calloc.c
_SRC += ft_isalnum.c
_SRC += ft_isalpha.c
_SRC += ft_isascii.c
_SRC += ft_isdigit.c
_SRC += ft_isprint.c
_SRC += ft_memchr.c
_SRC += ft_memcmp.c
_SRC += ft_memcpy.c
_SRC += ft_memmove.c
_SRC += ft_memset.c
_SRC += ft_strchr.c
_SRC += ft_strdup.c
_SRC += ft_strndup.c
_SRC += ft_strlcat.c
_SRC += ft_strlcpy.c
_SRC += ft_strlen.c
_SRC += ft_strncmp.c
_SRC += ft_strcmp.c
_SRC += ft_streq.c
_SRC += ft_strnstr.c
_SRC += ft_strrchr.c
_SRC += ft_tolower.c
_SRC += ft_toupper.c
_SRC += ft_putchar_fd.c
_SRC += ft_putendl_fd.c
_SRC += ft_putnbr_fd.c
_SRC += ft_putstr_fd.c
_SRC += ft_putendl.c
_SRC += ft_putstr.c
_SRC += ft_putnbr.c
_SRC += ft_putchar.c

_SRC += ft_itoa.c
_SRC += ft_strtrim.c
_SRC += ft_substr.c
_SRC += ft_strjoin.c
_SRC += ft_split.c
_SRC += ft_striteri.c
_SRC += ft_strmapi.c

_SRC += ft_putfmt.c
_SRC += ft_putfmt_fd.c
_SRC += ft_puthex_fd.c
_SRC += ft_putptr_fd.c
_SRC += ft_puthex.c
_SRC += ft_putptr.c
_SRC += ft_printf.c
_SRC += ft_dprintf.c

_SRC_BONUS += ft_lstnew.c
_SRC_BONUS += ft_lstadd_front.c
_SRC_BONUS += ft_lstsize.c
_SRC_BONUS += ft_lstlast.c
_SRC_BONUS += ft_lstadd_back.c
_SRC_BONUS += ft_lstclear.c
_SRC_BONUS += ft_lstdelone.c
_SRC_BONUS += ft_lstiter.c
_SRC_BONUS += ft_lstmap.c

_SRC += ft_char_in_charset.c
_SRC += ft_isspace.c
_SRC += ft_abs.c
_SRC += ft_max.c
_SRC += ft_min.c

_SRC += deque_init.c
_SRC += deque_free.c
_SRC += deque_print.c
_SRC += array_list_to_deque.c
_SRC += deque_index.c
_SRC += deque_slice.c
_SRC += deque_sort.c
_SRC += deque_copy.c
_SRC += deque_size.c
_SRC += deque_extend_free.c
_SRC += deque_swap.c
_SRC += deque_rotate.c
_SRC += deque_pop_top.c
_SRC += deque_pop_bottom.c
_SRC += deque_push_node_bottom.c
_SRC += deque_push_value_bottom.c
_SRC += deque_push_node_top.c
_SRC += deque_push_value_top.c
_SRC += deque_equal.c
_SRC += deque_argmax.c

_SRC += ddeque_init.c
_SRC += ddeque_free.c
_SRC += ddeque_print.c
_SRC += ddeque_index.c
_SRC += ddeque_slice.c
_SRC += ddeque_sort.c
_SRC += ddeque_shallow_copy.c
_SRC += ddeque_size.c
_SRC += ddeque_extend_free.c
_SRC += ddeque_swap.c
_SRC += ddeque_rotate.c
_SRC += ddeque_pop_top.c
_SRC += ddeque_pop_bottom.c
_SRC += ddeque_push_node_bottom.c
_SRC += ddeque_push_value_bottom.c
_SRC += ddeque_push_node_top.c
_SRC += ddeque_push_value_top.c
_SRC += ddeque_equal.c

_SRC += cmp_int_desc.c
_SRC += cmp_int_asc.c

_SRC += get_next_line.c

unexport _DEPS
_DEPS += libft.h

_OBJ = $(_SRC:.c=.o)
_OBJ_BONUS = $(_SRC_BONUS:.c=.o)

OBJ = $(addprefix $(OBJDIR)/,$(_OBJ))
OBJ_BONUS = $(addprefix $(OBJDIR)/,$(_OBJ_BONUS))
SRC = $(addprefix $(SRCDIR)/,$(_SRC))
SRC_BONUS = $(addprefix $(SRCDIR)/,$(_SRC_BONUS))
DEPS = $(addprefix $(INCDIR)/,$(_DEPS))

all: $(NAME)

$(NAME): $(OBJ)
	$(AR) $@ $^

$(OBJDIR)/%.o: %c $(DEPS) | $(OBJDIR)
	$(CC) $(CFLAGS) -c -o $@ -I$(INCDIR) $<

$(OBJDIR):
	$(MKDIR) $@

bonus: $(OBJ) $(OBJ_BONUS)
	$(AR) $(NAME) $^

clean:
	$(RM) $(OBJ) $(OBJ_BONUS)
	$(RM) -r $(OBJDIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: re fclean clean all bonus
