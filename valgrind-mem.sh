#! /bin/sh -

valgrind --suppressions=readline.supp --track-origins=yes --show-leak-kinds=all --leak-check=full ./minishell
