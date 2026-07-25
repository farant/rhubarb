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

## 2026-07-24 (later) — villa V3.5: the incremental path

`processus_incipere` / `pulsare` / `metere` / `abrumpere`.
33 → **72 asserts**, suite 105/105.

The V0 entry above ends with "do not rewrite; extend" — that
instruction turned out to be exactly right and is what this entry
followed.

### Why it was needed

There are **no threads in this house**: zero `pthread` anywhere in
`lib/`. `reactor.h` is a single-threaded `poll()` loop that owns its
own run loop; `actor.h`/`cursus.h` are the entity layer, not
concurrency. So a vitrea app has one thread, and `processus_exsequi`
with `ConnectTimeout=5` freezes the window for five seconds on an
unreachable host.

The villa spec's mitigation — "probe work happens in the per-tick
slot, never inline in an internuntius handler" — does **not** fix
this. Moving a blocking call from the handler into the tick still
blocks the tick. It had to become genuinely non-blocking.

### Two paths, one loop

All loop state moved into `struct Processus`, and
`_ansam_pulsare(p, obstans)` performs **one** step.
`processus_exsequi` drives it with `obstans=VERUM` in a loop;
`processus_pulsare` calls it once with `obstans=FALSUM`.

There is no second copy of the `select()` loop, deliberately — it is
subtle (EINTR, EAGAIN, partial reads, deadline arithmetic, the
CLOEXEC exec-errno pipe), and two copies of subtle code drift. Two
things prove the refactor faithful: the **original 33 asserts pass
unchanged**, and §IX asserts byte-equality of stdout, stderr, and
exit code between the two paths on one command.

### The subtlety that would have broken everything: `select() == 0`

In blocking mode that means *the deadline expired*. In pulse mode it
means *nothing is ready right now* — the most ordinary outcome there
is. Conflating them would time out every process on its first pulse.
So `obstans` is threaded into the *return handling*, not only into
the timeout computation.

### The bug the gate caught: abort waited for the child

`processus_abrumpere` on a `sleep 10` took the **full ten seconds**
(§XII failed on `r.mora_ms < 3000`; the suite ran 11.37s).

`_perficere` drained the pipes to EOF *before* checking `abruptus`
and sending SIGKILL — but EOF only arrives when the child exits, so
abort blocked on precisely the thing it exists to prevent. The
timeout path already carried `!p->tempus_excessum` in that drain
guard for the same reason; abort had simply never been added to it.
One-token fix; suite went 11.37s → 1.22s.

The shape generalises: the guard encodes "terminal condition known ⇒
don't drain," and a **new** terminal state was introduced without
joining the set the old ones belonged to. Nothing in the language
checks that, so when a state machine grows a state, audit every set
membership by hand.

### How the non-blocking claim is actually tested

§X spawns `sleep 0.3` (no output) and **counts pulses**. If a pulse
blocked, the loop runs once. It asserts `pulsus > 1000`, plus
`r.mora_ms > 100` so the child demonstrably ran. No wall-clock
assertion — pulse count measures the property directly and doesn't
go flaky on a loaded machine.

### Details worth keeping

- **`_fistulam_haurire` drains until EAGAIN**, bounded to 64 chunks
  (256 KiB) per pulse. One 4 KiB read per pulse would drag a large
  output across hundreds of ticks; unbounded draining lets a chatty
  child hold the pulse open forever — the very freeze being removed.
  §XV pushes ~93 KB through the pulse path.
- **Failed construction returns a valid handle**, not `NIHIL`, with
  the error latched and `perfectus` pre-set, so callers have one code
  path. `NIHIL` comes back only when there is no arena to allocate
  from.
- **The exec-errno pipe is read only after reaping.** It is not
  `O_NONBLOCK`, and reading it while the child is alive and pre-exec
  would block. Making it non-blocking would be *worse*: `EAGAIN`
  would be indistinguishable from "exec succeeded," silently turning
  a failed exec into a reported success.
- **Pipes close before the reap**, as before — a child blocked
  writing into a full pipe with no reader would hold `waitpid`
  forever.

### Lexicon

`WNOHANG`/`WUNTRACED` added to `silva/fontes/systema_posix.h`,
verified against the real Darwin header (1 and 2). Worth checking: a
wrong `WNOHANG` would make `waitpid` block, and it would present as
"the incremental API doesn't work" rather than "a constant is wrong."

### Tooling gap found on the way

`./silva/examen.sh lib/processus_posix.c -posix` → **ACCIPE, 0
violations**. `mcp__legati__diagnostica` on the same file → **REICE,
21 violations**. The `-posix` routing table lives only in
`.claude/hooks/examen-custos.sh`; the LSP surface never learned it,
so it judges platform files against the ISO lexicon alone and emits
a wall of red on a clean file. Filed `01KYBAG1MJ`; the fix is one
data file both surfaces read.
