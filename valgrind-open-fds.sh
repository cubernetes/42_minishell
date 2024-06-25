#! /bin/sh -

valgrind -q --log-file=>(grep -v '^==[[:digit:]]\+==\s*$') --track-fds=all ./minishell
