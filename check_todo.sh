#! /bin/sh

# TODO: ONLY remove this file when the following command doesn't print anything
# (except this file)
grep --color=always --exclude-dir=.git -inr TODO
