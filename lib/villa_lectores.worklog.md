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
