# Feature summary
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

## Extra features
- declare (also with `-p`) and readonly builtin
- declare, readonly, and export without args, with correct(!) quoting
    - try `declare A='cd "$HOME" && echo "Changed dir!" || echo "Sorry $USER, no chaning :("' && declare -p A`
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
- `-c` option
- creation of subshells when using parenthesis
- consecutive expansion of lists (try `export A=1 && echo $A`)
- consecutive expansion of heredocs in lists (try `export A=1 && cat << EOF\nA: $A\nEOF`)
- correct handling of non-tty stdin, also for heredocs(!) (try `printf '<<EOF cat\nline\nEOF' | ./minishell`)
- correct handling of multiline input
- correct handling of multiline prompt
- bash-identical implementation of the very quirky(!) `exit` builtin
    - try `exit 9223372036854775808 1` vs. `exit 9223372036854775807 1` vs. `exit word 1`
    - try `bash -c 'exit 42'`; `exit 1 1`; `echo $?` vs. `true`; `exit 1 1`; `echo $?`
- `cd` with `-` argument
- handling of ambiguous redirects
- the weird export edge case (`export l='ls -al' e=export && export newls=$l && $e newls2=$l`)
- `?` glob character

## Notable implementation details
- optional(!) garbage collection with custom(!) contexts
- use of hashtables (`fnv-1a`)
- 0-declaration (reverse-)iterators for the circular deques (besides hashtable the only datastructure being used)
- ft_getopt (try `declare -prxxx -x -r -xpxprpxp -- VAR=1 X`)
- LL(1) grammar and parser
- AST with tagget unions
- generic deque implementation using tagged unions
- custom implementation of mktemp (for heredoc tmp files) using `/dev/urandom`
