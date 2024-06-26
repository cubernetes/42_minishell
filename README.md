# Minishell -- Feature summary (not exhaustive)

## Mandatory Features
- input prompt with history (readline, non-persistent)
- program execution based on `PATH`
- single quote and double quotes
- parameter expansion (`$name` format), also in double quotes
- redirections (`<, >, <<, >>`)
- anonymous pipes (`|`)
- `$?` special parameter for the exit status of the most recent pipeline
- handling of `SIGINT` (`C-c`), `SIGQUIT` (`C-\`), and `EOT` (`C-d`) in all modes
- some builtins (`cho with -n, cd, pwd, export, unset, exit, env`)
- 0 memory/fd leaks and memory corruptions

## Bonus features
- AND (`&&` and OR (`||`) lists
- parenthesis (`(` and `)`) for priorities
- globbing (`*`) for the current working directory

## Extra features (not required by the subject)
- Wordsplitting after parameter expansion using multicharacter IFS (exactly like bash)
    - try one of `unset IFS`, `declare IFS=`, `declare IFS=:`, `declare IFS=": "`
    - then `declare A=" :one     two  three:::::four:   " && /bin/printf '"%s"\n' $A-one-$A-two-$A`
- declare (also with `-p`) and readonly builtin
- declare, readonly, and export without args, with correct(!) quoting
    - try `declare A='cd "$HOME" && echo "Changed dir!" || echo "Sorry $USER, no changing :("' && declare -p A`
- shell (unexported) variables (`declare A=1`)
- readonly variables (`declare -r A=1`, `readonly B=1`)
- unset, yet declared variables (`declare A`, `export X`)
- different prompts (`PS0`, `PS1`, `PS2`, `PS4`)
- prompt expansion (`\u, \w, \W, \h, \H, \$`)
- fallback mechanisms for username resolution in prompt
    1. manual parsing of the binary Kerberos credential cache file (`/tmp/krb5cc_$EUID_...`)
        - `EUID` retrieved via manual parsing of `/proc/self/status`
    2. manual parsing of `/etc/passwd`
    3. `EUID` env var
- fallback mechanisms for hostname resolution in prompt (`/etc/hostname`, then `/proc/sys/kernel/hostname`)
- creation of subshells when using parenthesis
- sequential expansion of lists (try `export A=1 && echo $A`)
- sequential expansion of heredocs from lists (try `export A=1 && cat << EOF\nA: $A\nEOF`)
- correct handling of non-tty stdin, also for heredocs(!) (try `printf '<<EOF cat\nline\nEOF' | ./minishell`)
- correct handling of multiline input
- correct handling of multiline prompt
- bash-identical implementation of the very quirky(!) `exit` builtin
    - try `exit 9223372036854775808 1` vs. `exit 9223372036854775807 1` vs. `exit word 1`
    - try `bash -c 'exit 42'`; `exit 1 1`; `echo $?` vs. `true`; `exit 1 1`; `echo $?`
- handling of ambiguous redirects
- the weird export edge case (`export l='ls -al' e=export && export newls=$l || true && $e newls2=$l || true && echo $newls && echo $newls2`)
- `?` glob character
- additional special parameters:
    1. `$-` expanding to the active shell options
    2. `$$` expanding to the the current PID of the shell
    3. `$0` to `$9` expanding to the positional arguments of minishell (specified after `-c`)
    3. `$#` expanding to the number of positional argument
    4. `$@` expanding to all positional argument, with word splitting when quoted
    5. `$*` expanding to all positional argument, joining with `IFS[0]` when quoted
- default variables
    - PPID, MINISHELL_EXECUTION_STRING
- shift builtin
- source builtin
- `cd` with `-` argument
- correct handling (using `rl_getc_function`) of:
    - `./minishell 2>/dev/null`
    - `./minishell 1>/dev/null`
    - `./minishell | ./minishell | ...`
- the following shell options (exactly like bash) and their negations (using `+`) if applicable:
    - `-c` for an execution string with positional arguments afterwards (`$0`, `$1`, ...)
    - `-s` to specify positional arguments (`$1`, `$2`, ...)
    - `-l` to make minishell act as if it had been invoked as a login shell
    - `-i` to force interactive mode (except when using `-c`)
    - `-a` to autoexport variable assignments
    - `-e` to exit when a pipeline fails with a non-zero exit status
    - `-f` to disable pathname expansion (globbing)
    - `-n` to only read command, not execute them
    - `-t` to exit after executing the first command line
    - `-u` to treat expansion of unset parameters an error
    - `-v` to print every input line after it's read
    - `-v` to print the arguments of a command before it's executed
    - `-C` to disallow overwriting of existing regular files via redirections
- execution of scripts via arguments
- sourcing of $HOME/.mshrc if it exists
- when invoked with `-l` or the first character of argv0 is a hyphen (`-`) (login shell)
    - sourcing of $HOME/.msh_profile if it exists, otherwise $HOME/.profile if it exists
- handling of line continuations in the following contexts:
    - unfinished single quote
    - unfinished double quote
    - escaped newline
    - unfinished subshell
    - unfinished pipeline
    - unfinished control operator (`&&`, `||`)
    - here-documents
- The exlamation point (`!`) reserved word for negation of the exit status of a pipeline
- comments using the hash symbol (`#`)
- the semicolon control operator (`;`)
- handling of backslash-escaping in unquoted and double-quoted contexts
- ANSI C quoting
- Error redirection using `2>` and `2>>`
- inheriting and respecting of umask (tho you can't change it :P)
<!-- TODO: Finish -->

## Notable implementation details
- optional(!) garbage collection with custom(!) contexts
- use of hashtables (`fnv-1a`)
- 0-declaration (reverse-)iterators for the circular deques (besides hashtable the only datastructure being used)
- ft_getopt (including `+` options) (try `declare -prxxx +x +r -xrpxprpxp -- VAR=1 X`)
- LL(1) grammar and parser
- AST with tagged unions
- generic deque implementation using tagged unions
- custom implementation of mktemp (for heredoc tmp files) using `/dev/urandom`
- Not inheriting IFS to prevent exploits

# Generate locale-specific whitespace
The requirements for this project disallow the use of `isspace(3)`, which respects
the current `locale(7)`. However, we need this locale information if we want to
do word splitting correctly, since the `IFS` whitespace defintion is determined
from the `LC_CTYPE` "space" character class, which in turn inherits `" \f\n\r\t\v"` (`locale(5)`).
Other locales, like `am_ET` define further whitespace in this class.

We can obtain all the ASCII characters which are whitespace in the current locale
by defining them in a header. To obtain them, one can leverage C or POSIX sh:
```sh
#! /bin/sh

true Find suitable dump command                                                             &&#
    { command -v xxd                                                                        &&#
    dump='xxd -ps -c1'                                                                      ||#
    { command -v hexdump                                                                    &&#
    dump="hexdump -ve '/1 "'"'"%02x\n"'"'"'"                                                ||#
    { command -v od                                                                         &&#
    dump='od -An -vtx1 -w1 | cut -c2-'                                                      ||#
    { command -v printf                                                                     &&#
    dump='while LC_ALL=POSIX IFS= read -r -d "" -n 1 byte; do printf "%02X\n" "'"'"'$byte"; done' ||#
    dump='printf "\033[31m%s\033[m\n" "No dumper (not event printf?) found, please install either xxd, hexdump, or od. Providing default locale." >/dev/tty | printf "20\n09\n0a\n0b\n0c\n0d"'; }; }; }; } 2>/dev/null 1>&2

2>/dev/null 1>&2 command -v locale && . /dev/stdin << LOCALE
$(locale)
LOCALE
[ -n "${LC_CTYPE}" ] && locale_info=" /* LC_CTYPE="'"'"${LC_CTYPE}"'" */' || locale_info=

printf '\n'
for i in {1..127}; do
    char="$(printf "\\$(printf '%03o' "$i").")"
    case "${char%.}" in [^[:space:]]) false;; esac && printf "%s" "${char%.}" # [:space:] character class respects the locale
done | eval "$dump" | sed 's/^/\\x/' | tr -d '\n' | sed 's/^/# define IFS_WHITESPACE "/; s|$|"'"${locale_info}"'\n|'
```
