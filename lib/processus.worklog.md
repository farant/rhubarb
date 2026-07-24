# processus — worklog

## 2026-07-24 — birth (villa V0)

Built because exploration found the house had **zero** process-spawn
capability: no `exec*` anywhere, no `popen`, no `posix_spawn`; every
shell-out was `system()`, and the only two `fork()` sites never exec.

A note on the premise that sent us looking: `systema_posix.h` is
silva's **parser lexicon** — a hand-written miniature header so
`examen` can judge fork/pipe code without false positives. It
declares `fork`/`pipe`/`waitpid` with real Darwin signatures and
implements nothing. "Covers process" meant the judge won't complain,
not that the house can spawn. Easy misread; worth the correction.

### Three design calls

**1. argv vector, never `/bin/sh`.** The entire shell-quoting class
is deleted at the root rather than defended against. This matters
most where a user types free text that becomes a command (villa's
arbitrary-command box). Section IV of the gate pins it: an argument
containing `spatium 'virgula' "duplex" ; rm -rf /` plus a newline
arrives at the child byte-identical.

**2. stdout and stderr captured SEPARATELY.** `system()` cannot do
this at all, and it's not cosmetic: ssh's diagnostics (BatchMode auth
refusal, ConnectTimeout) land on stderr while the remote command's
output lands on stdout. Merging them would corrupt every parse.

**3. A third pipe, FD_CLOEXEC, carrying exec's errno.** If exec
succeeds the pipe closes empty; if it fails the child writes errno.
This distinguishes "binary not found" from "the process ran and
exited 127" — which is load-bearing for villa specifically, because
**ssh itself exits 127 when the REMOTE command is missing.** Without
the third pipe, "ssh isn't installed" and "systemctl isn't on the
droplet" are the same result. Gate section VI pins both sides.

### Semantics worth remembering

`successus` means *spawned and reaped*, NOT *the command succeeded*.
A non-zero exit code is a successful result — the process ran and
answered. Callers check `codex_exitus` separately. Gate section II
pins this so nobody "fixes" it later.

### C89 / strict-flags find

`mora.tv_usec = (longus)(...)` fails under `-Wshorten-64-to-32`:
`tv_usec` is `__darwin_suseconds_t` = **int** on Darwin, while it's
`long` on other POSIX. The fix is to cast to the field's own type
(`integer`), not to a type guessed from the struct's general shape.
Same trap will bite anyone filling a `timeval` in this tree.

### Streaming is deliberately absent, not designed out

`processus_exsequi` collects-with-deadline over `select()` on two
pipes. That loop *is* the streaming loop with the callback elided, so
a later `processus_incipere`/`_legere`/`_finire` triple (for
`journalctl -f`) is a new entry point over the same internals rather
than a second implementation. Do not rewrite; extend.

Gate: `probationes/probatio_processus.c`, 33 asserts, fully offline
(/bin/echo, /bin/sh only — no network, no fixtures, no generated
binaries). Suite 103/103 after landing.
