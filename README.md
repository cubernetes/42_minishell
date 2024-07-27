# Warning
Currently a bit broken, due to generous refactors to adhere to the school's standards...
Please submit issues if you find any, as I want to use this as a basis for the 42sh project.
## Known Issues that are Regressions
- Ambiguous redirects are not being handled correctly (`A=one\ two; >$A`)
- Invalid read of size 1 (`export =var`)
- first argument to `readonly` builtin is being skipped
- `./minishell 2>tmp` not behaving quite right

# Disclaimer for 42 Students
If you are a 42 student, do not take the following ("additional") features as a requirement for your own project. This is not what minishell, the project, is about. Evaluators ought NOT to fail you for not implementing some things exactly as they are in bash (notably: `export` without arguments (have you considered ANSI-C-quoting and "unset-but-declared" variables?, does the evaluator even know the purpose/use cases of export without arguments (reproducibility of environment, ssh, ...)?), `exit`'s argument handling (have you considered MAX_LONG overflow or its exit status considering it's a [special builtin](https://www.gnu.org/software/bash/manual/html_node/Special-Builtins.html)?), `pwd` (have you considered symbolically linked directory preservation after `cd` (inode checking)?), have you considered ambiguous redirects (a csh-inspired bash feature) for globbing and parameter expansion?, are you including the CWD when PATH contains the empty string?, are you printing the line on error when run in non-interactive mode?, have you considered CDPATH (frankly, this one is more so aimed at writers of test libraries)?, etc...)

# Minishell – Feature Summary
The [subject](minishell_subject.pdf).

The usual 42 style guidelines (aka "norm") apply (functions <= 25 lines, <= 4 params, <= 5 vars, <= 80 cols, separate declaration, 1 instruction per line, max 5 funcs per file, 4 space-tab indent & align, no globals, constant macros, -Wall -Wextra -Werror, no memory/fd leaks, no memory corruptions, etc.)

Compile with `make`, recompile with `make re`, remove objects with `make clean`.

## 🟩 Mandatory Features (see subject)
- 🟩 input prompt with history (readline, non-persistent)
- 🟩 program execution based on `PATH`
- 🟩 single quotes and double quotes
- 🟩 parameter expansion (`$name` format), also in double quotes
- 🟩 redirections (`<, >, <<, >>`)
- 🟩 anonymous pipes (`|`)
- 🟩 `$?` special parameter for the exit status of the most recent pipeline
- 🟩 handling of `SIGINT` (`intr = ^C`), `SIGQUIT` (`quit = ^\`), and `EOF` (`eof = ^D`) in all contexts
- 🟩 the following regular builtins
    - `echo` with -n option
    - `cd`
    - `pwd`
    - `env` (not a builtin in any shell, but subject requires it...)
- 🟩 the following special builtins
    - `export`
    - `unset`
    - `exit`

## 🟩 Bonus features (see subject)
- 🟩 AND (`&&` and OR (`||`) lists
- 🟩 parenthesis (`(` and `)`) for priorities
- 🟩 globbing (`*`) for the current working directory

## 🟧 Additional features (not required by the subject)
❗ Features decorated as <ins><b>bold and underlined</b></ins> seemed easy to implement, but turned out to be significantly harder

❗ Occasionally, they might also mark features that are almost always implemented "incorrectly" (in other minishell projects), where I see "correct" with reference to it being "useful"/"sensible" to do it this way (i.e. bash does it this way too). This is opinionated, as (e.g.) ambiguous redirects are counterintuitive and raise more eyebrows than not (they are not implemented in these Bourne like shells: ksh, dash, ash, yash, yash -o posix, bash -o posix, mksh, pdksh, posh, pbosh, mrsh, (zsh, fish)).

- 🟩 [Wordsplitting](https://www.gnu.org/software/bash/manual/bash.html#Word-Splitting) after parameter expansion using <ins><b>multicharacter</b></ins> IFS ([<ins><b>exactly</b></ins>](https://www.reddit.com/r/bash/comments/1dnqswy) like bash)
    - 🟩 try one of `unset IFS`, `IFS=`, `IFS=:`, `IFS=": "`
    - 🟩 then `A=" :one     two  three:::::four:   " && /bin/printf '"%s"\n' $A-one-$A-two-$A`
- 🟩 declare (also with `-p`) and readonly (special) builtin
- 🟧 declare, readonly, and export without args (no ANSI-C-quoting)
    - 🟩 try `A='cd "$HOME" && echo "Changed dir!" || echo "Sorry $USER, no changing :("' && declare -p A`
- 🟩 normal shell variables (no flags) (`declare A=1`, `A=1`)
- 🟩 readonly variables (`declare -r A=1`, `readonly B=1`)
- 🟩 unset, yet declared variables (`declare A`, `export X`)
- 🟩 different prompts (`PS0`, `PS1`, `PS2`, `PS4`)
- 🟩 prompt expansion (`\u, \w, \W, \h, \H, \$`)
- 🟩 fallback mechanisms for username resolution in prompt
    1. 🟩 <ins><b>manual</b></ins> parsing of the binary [Kerberos credential cache](https://web.mit.edu/kerberos/www/krb5-latest/doc/formats/ccache_file_format.html) file (`/tmp/krb5cc_$EUID_...`)
        - 🟩 `EUID` retrieved via manual parsing of [`/proc/self/status`](https://man7.org/linux/man-pages/man5/proc_pid_status.5.html)
    2. 🟩 manual parsing of `/etc/passwd`
    3. 🟩 `EUID` env var
- 🟩 fallback mechanisms for hostname resolution in prompt (`/etc/hostname`, then `/proc/sys/kernel/hostname`)
- 🟩 creation of subshells when using parenthesis
- 🟩 <ins><b>sequential</b></ins> expansion of lists (try `A=1 && echo $A`)
- 🟩 <ins><b>sequential</b></ins> expansion of heredocs from lists (try `A=1 && cat << EOF\nA: $A\nEOF`)
- 🟩 correct handling of non-tty stdin, also for <ins><b>heredocs</b></ins> (try `printf '<<EOF cat\nline\nEOF' | ./minishell`)
- 🟩 <ins><b>correct</b></ins> handling of multiline input
- 🟩 <ins><b>correct</b></ins> handling of multiline prompt
- 🟩 bash-identical implementation of the [<ins><b>very quirky</b></ins>](https://www.reddit.com/r/bash/comments/1de0a3n/weird_exit_status_behaviour/) `exit` special builtin
    - 🟩 try `exit 9223372036854775808 1` vs. `exit 9223372036854775807 1` vs. `exit word 1`
    - 🟩 try `bash -c 'exit 42'`; `exit 1 1`; `echo $?` vs. `true`; `exit 1 1`; `echo $?`
- 🟩 handling of csh-like ambiguous <ins><b>redirects</b></ins> (with <ins><b>correct</b></ins> error reporting)
- 🟥 <ins><b>[the weird export/declare/readonly edge case](https://unix.stackexchange.com/a/599170)</b></ins> (`l="ls -al" e=export ; export newls=$l ; $e newls2=$l ; echo "newls:$newls" ; echo "newls2:$newls2"`).
- 🟩 `?` glob character
- 🟩 additional special parameters:
    1. 🟩 `$-` expanding to the active shell options
    2. 🟩 `$$` expanding to the the current PID of the shell
    3. 🟩 `$0` to `$9` expanding to the positional arguments of minishell (specified after `-c`)
    3. 🟩 `$#` expanding to the number of positional argument
    4. 🟥 `$@` expanding to all positional argument, with word splitting when <ins><b>quoted</b></ins>
    5. 🟥 `$*` expanding to all positional argument, joining with `IFS[0]` when quoted
- 🟩 default (and automatic/special) variables
    - 🟩 PPID, _, MINISHELL_EXECUTION_STRING, LINENO/CURRENT_LINE, SHLVL
- 🟩 shift special builtin without arguments
- 🟧 <ins><b>source special builtin</b></ins>
- 🟩 : special builtin (colon, null utility, aka null command)
- 🟩 `cd` with `-` argument
- 🟩 <ins><b>correct</b></ins> handling of:
    - 🟩 `./minishell 2>/dev/null`
    - 🟩 `./minishell 1>/dev/null`
    - 🟩 `./minishell | ./minishell | ...`
    - 🟩 `./minishell -i 2>/dev/null`
- 🟩 the following shell options (exactly like bash) and their negations (using `+`) if applicable:
    - 🟩 `-c` for an execution string with positional arguments afterwards (`$0`, `$1`, ...)
    - 🟩 `-s` to specify positional arguments (`$1`, `$2`, ...)
    - 🟩 `-l` to make minishell act as if it had been invoked as a login shell
    - 🟩 `-i` to force interactive mode (except when using `-c`)
    - 🟩 `-a` to autoexport variable assignments (allexport)
    - 🟩 `-e` to exit when a pipeline fails with a non-zero exit status (errexit, fails categorically, i.e. also in `&&` constructs, unlike bash)
    - 🟩 `-f` to disable pathname expansion (noglob)
    - 🟩 `-n` to only read command, not execute them (noexec)
    - 🟩 `-t` to exit after executing the first command line (onecmd)
    - 🟩 `-u` to treat expansion of unset parameters an error (nounset)
    - 🟩 `-v` to print every input line after it's read (verbose)
    - 🟩 `-x` to print the arguments of a command before it's executed (xtrace)
    - 🟩 `-C` to disallow overwriting of existing regular files via redirections (noclobber)
- 🟥 set special builtin that can set all of the options from above (except `-s`, `-c`, `-l`, and `-i`) and set positional arguments
- 🟥 execution of scripts via arguments
- 🟩 sourcing of $HOME/.mshrc if it exists
- 🟩 when invoked with `-l` or the first character of argv0 is a hyphen (`-`) (login shell)
    - 🟩 sourcing of $HOME/.msh_profile if it exists
- 🟥 handling of line continuations in the following contexts:
    - 🟥 unfinished single quote
    - 🟥 unfinished double quote
    - 🟥 escaped newline
    - 🟥 unfinished subshell
    - 🟥 unfinished pipeline
    - 🟥 unfinished control operator (`&&`, `||`)
    - 🟥 here-documents
- 🟩 The exlamation point (`!`) reserved word for negation of the exit status of a pipeline
    - work categorically, i.e. also in && constructs, unlike bash
    - allows for empty pipelines (i.e. `! || ! ! ; !`)
- 🟩 comments using the hash symbol (`#`)
- 🟩 the semicolon control operator (`;`)
- 🟩 handling of backslash-escaping in unquoted and double-quoted contexts
- 🟩 Error redirection using `2>` and `2>>`
- 🟩 inheriting and respecting of umask (tho you can't change it :P)
- 🟩 assignment words (basic implementation, not allowed with non-assignment-words)
    - try `A=1 B=2 C=3 ; declare -p A B C`
- 🟥 heredocs in the history
- 🟥 <ins><b>[ANSI C quoting](https://www.gnu.org/software/bash/manual/bash.html#ANSI_002dC-Quoting)</b></ins>

## 🟩 Notable implementation details
- 🟩 optional garbage collection with <ins><b>arbitrary</b></ins> contexts
- 🟩 use of <ins><b>hashtables</b></ins> (`fnv-1a` hash algorithm)
- 🟩 <ins><b>0-declaration</b></ins> iterators for the circular deques (the main datastructure)
- 🟩 <ins><b>[ft_getopt](https://www.man7.org/linux/man-pages/man3/getopt.3.html)</b></ins> (including `+` options) (try `declare -prxxx +x +r -xrpxprpxp -- VAR=1 X`)
- 🟩 LL(1) grammar and parser
- 🟩 AST with [tagged unions](https://www.wikiwand.com/en/Tagged_union)
- 🟩 generic deque implementation using tagged unions
- 🟩 functions similar to mktemp, gethostname, getpid, getuid, <ins><b>getusername</b></ins>, getumask
- 🟩 Not inheriting IFS to [prevent exploits](https://unix.stackexchange.com/questions/583596/security-shell-scripts-to-reset-the-ifs-variable-against-misuse)

# 🟩 Generate locale-specific whitespace
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
