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
    - try one of `unset IFS`, `declare IFS=`, `declare IFS=x`
    - then `declare A=" one     two  threexxxxxfour   " && /bin/printf '"%s"\n' before-$A-after | /bin/cat -A`
- declare (also with `-p`) and readonly builtin
- declare, readonly, and export without args, with correct(!) quoting
    - try `declare A='cd "$HOME" && echo "Changed dir!" || echo "Sorry $USER, no changing :("' && declare -p A`
- shell (unexported) variables (`declare A=1`)
- readonly variables (`declare -r A=1`, `readonly B=1`)
- unset, yet declared variables (`declare A`, `export X`)
- different prompts (`PS0`, `PS1`, `PS2`)
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
- `$-` and `$$` special parameters
- `$0` to `$9` positional parameters
- `cd` with `-` argument
- `-c` option with argv handling
<!-- TODO: Finish -->

## Notable implementation details
- optional(!) garbage collection with custom(!) contexts
- use of hashtables (`fnv-1a`)
- 0-declaration (reverse-)iterators for the circular deques (besides hashtable the only datastructure being used)
- ft_getopt (try `declare -prxxx -x -r -xpxprpxp -- VAR=1 X`)
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
byte defining them in a header. To obtain them, one can leverage C or bash:
```bash
#! /usr/bin/env bash

true Find suitable dump command              &&#
    { command -v xxd                         &&#
    dump='xxd -ps -c1'                       ||#
    { command -v hexdump                     &&#
    dump="hexdump -ve '/1 "'"'"%02x\n"'"'"'" ||#
    { command -v od                          &&#
    dump='od -An -vtx1 -w1 | cut -c2-'       ||#
    dump='printf "\033[31m%s\033[m\n" "No dumper found, please install either xxd, hexdump, or od. Providing default locale." >/dev/tty | printf "20\n09\n0a\n0b\n0c\n0d"'; }; }; } 1>/dev/null 2>&1

for i in {1..127}; do
    char="$(printf "\\$(printf '%03o' "$i").")"
    [[ "${char%.}" == [[:space:]] ]] && printf "%s" "${char%.}" # [:space:] character class respects the locale
done | eval "$dump" | sed 's/^/\\x/' | tr -d '\n' | sed 's/^/"/; s/$/"\n/'
```
