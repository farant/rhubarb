# sententiae — worklog

## 2026-07-25 — S0/S1: the parser and the seal

**INTENTIO.** Build the pure half of sententiae: parse an
explicitly-addressed outline document into a rebuildable projection, and
seal each point so anchors can detect drift. No store, no UI, no I/O —
gated offline in root `probationes/` against the real corpus
(`de-imagine.md`, 273 points).

**RELATIO.** Shipped. 81 asserts, suite 106 → 107. Compiled clean under
the full flag set on the first attempt; censor PURUS, examen ACCIPE.

Design follows `villa_lectores` closely and deliberately: arena last and
never stored, internal helpers return views, public functions transcribe
at the build boundary, refusal is a struct field whose `causa` always
names the reason, derived judgments (`retractum`, `inresolutum`) computed
once in the library so consumers cannot disagree.

---

### Two things the design got wrong, both caught before they cost anything

**1. `PARENS_ABEST` as a structural error would have blocked the real
document on the first run.**

The header originally listed "parent does not exist" among the errors
that refuse a parse. Checking against the corpus before writing the
implementation: **nine loci have no parent**, and all nine are the same
shape —

```
4.obj.1   7.obj.1   10.obj.1   14.obj.1   15.obj.1
15.obj.2  18.obj.1  24.obj.1   24.obj.2
```

A *named* segment introduces a level that has no owning point. `4.obj`
never exists; `4.obj.1` is the first objection. So the rule was demoted
to an anomaly, and narrowed: it fires only when every segment of the
missing parent is numeric. That still catches a real typo (`1.1.1` with
no `1.1`) while accepting named branches. Fires zero times on the corpus,
which is the correct answer.

Worth naming the near-miss: this was found by *querying the corpus*
before implementing, not by running the parser. Had it been found by
running, it would have looked like a parser bug for a while.

**2. The retraction count was 8; it is 7.**

Stated as "8 retractions" in the spec, the interview record, the header
trap, and repeatedly in conversation. The parser found 7 and was right.

Eight *lines* contain `retracted`. Line 17 is the **vocabulary
declaration** in the prooemium — `[retracted: ...]` with a literal
ellipsis, declaring that the grade exists and takes a payload. The other
seven are applications (14.1, 29.1–29.6).

So the trap is sharper than first written: of the seven retracted points,
**six span lines**, and a line-based extractor finds exactly **one**.

The error is instructive rather than embarrassing: it was a *category*
confusion between declaring a grade and applying one — which is the exact
prooemium/body distinction this parser is built around. Measuring with
`grep -c` counted both because grep has no notion of the split. The fix
is in the test, the header, and all three spec files.

---

### The pipeline order is not a preference

```
join continuations → extract tags → extract references
```

A tag extractor that runs before joining sees six of the seven retracted
points as unterminated brackets and silently drops them. This is why
`_pendentem_condere` finishes the joined text *first* and only then calls
`_tags_extrahere`. Any future optimisation that fuses those passes
reintroduces the bug.

### Column-0 discipline, with a number

42 indented lines in the corpus match the address grammar if leading
whitespace is stripped before testing. The invariant is not stylistic —
it is the only thing distinguishing an address from a sentence that
happens to begin with a word and a period.

### The grammar needed tightening

`segmentum := [0-9]+ | [a-z]+` in *initial* position admits any English
word ending in a period — 275 matches instead of 273, both extras being
prose in the prooemium. The first segment must be numeric. Independently,
the prooemium ends at the first fence rather than the first
address-shaped line; the naive rule truncates front matter mid-sentence
at line 15, destroying the grade vocabulary and the siglum table *while
reporting success*.

---

### Three copies of subtle code, extracted before compiling

Finalising a pending sententia happens at three points — fence close,
blank line, next address. The first draft inlined it three times.

Villa's worklog already records where that leads ("two copies of subtle
code drift"), and the consequence here is worse than usual: a divergence
between the three would produce a point with a wrong seal, i.e. a lying
anchor. Extracted to `_pendentem_condere`, which also clears `pendens`
and `aed` itself so no caller has to remember to.

The refactor was done *before* the first compile, on the grounds that
"it works, clean it later" is how three copies become permanent.

---

### `duplex` is a latina.h macro — third firing of the class

`Liber duplex;` expands to `Liber double;`. Renamed to `duplicatus`.

Third time in two days after `ordinarius` and `nomen`. What is worth
recording is the tool behaviour, not the mistake:

```
[suspectum in errore] ... macra latinae intra fracturam parsationis:
  'principale', 'vacuum', 'si', 'imprimere', 'redde', 'I', 'FALSUM',
  'ZEPHYRUM', ...
conditio:     PURUS
```

Censor **saw** the parse fracture and listed eight macros in the region,
none of which was the culprit — then reported `PURUS`. Clang named it
instantly. Recorded as a third `ictus` on `01KXTRM0DB`; a tool that sees
a fault but cannot name it costs a round trip through the compiler, which
is a tool that ends up unused.

---

### Details worth keeping

- **`_album` is a four-character test on `i8`, not `<ctype.h>`.**
  `character` is signed here, so a UTF-8 lead byte would go negative and
  `isspace()` would be undefined. Copied from villa for the same reason.
- **`chorda_sectio` returns empty on an out-of-range slice** rather than
  erroring, so a bounds bug presents as a legitimately-absent field.
  Avoided by constructing views by hand with explicit arithmetic.
- **`_locus_capere` excludes the trailing period** from the returned
  locus: `4.obj.1.1`, not `4.obj.1.1.` — the canonical citation form
  (`DI/4.obj.1.1`).
- **Normalization and sealing are separate public functions.** The
  rewrap test asserts against the normalized *chorda* first and the
  digest second, so a failure says what differed rather than only that
  something did. This immediately paid off while debugging the CRLF
  fixture.
- **`sententiae_per_locum` is a linear scan.** 273 points makes the
  duplicate-locus check O(n²) ≈ 37k chorda compares, microseconds. A
  `tabula_dispersa` was specced and skipped as premature; revisit only if
  a document reaches thousands of points.
- **`ANCORA_MOTA` is detected and never followed.** Following handles
  pure reordering invisibly but must guess when text was both moved and
  edited, and a wrong guess is a lying anchor — worse than a broken one,
  because nothing signals it.

### Instrumenta

- **adhibita:** censor, examen, clang direct, `compile_tests.sh
  sententiae`, `compile_tests_fontes_generare.sh`, ad-hoc corpus queries
- **fructus:** the corpus queries were the highest-yield instrument by a
  distance — the parent-absence finding and the grammar tightening both
  came from querying `de-imagine.md` directly rather than from running
  code. Examen and censor were fast and silent-when-clean on the header
  and the library.
- **asperitates:** censor's `PURUS`-with-`suspectum` on a parse fracture
  (see above). Also: `compile_tests.sh` emits ANSI colour into its
  summary, so reading the result programmatically needs a workaround —
  already filed as `01KYB29BC6` with two ictus.
- **desiderata:** none new. The `Scanner`-struct extraction from villa's
  line helpers now has its second consumer (this library copied
  `_linea_proxima`, `_verbum`-style helpers, `_album`, `_ch`, `_vacua`,
  `_copia`); a third would justify promoting them. Noted in spec-v2 §VIII.9.

### Next

S2 is the store: the `liber` genus, the collision check that must read
the `genera` table rather than trust the `addere` response, and the
paste/mutatio path. The parser is complete and gated; nothing downstream
needs to re-derive its behaviour.

*(S2 followed the same day; see below.)*

---

## 2026-07-25 (later) — S2: the store round trip

**INTENTIO.** Prove that a document surviving the store is *still the same
document* — not merely that inserting it didn't error.

**RELATIO.** `probationes/probatio_sententiae_horreum.c`, 32 asserts,
suite 107 → 108.

### Deviation from the plan, stated

spec-v2 staged S2 as "JS only, headless". That combination does not
exist: the forum's JS lives inside `index.html` and there is no headless
JS harness. So S2 became a **C gate** using `cliens_tabularii` against an
ephemeral daemon (`-portus 0`, scratch db and annals in `build/`), on
villa's fixture pattern. The JS in S3 mirrors the same call sequence.

This is strictly better than the specced version, because root
`probationes/` is swept by a runner and `apps/` is not.

### The assertion that justifies the whole gate

A seal is computed over text. If one byte changes anywhere between the
paste box and the read-back, **every** seal changes and **every** anchor
breaks. The scratch-store experiment during exploration proved insertion
*succeeds*; it did not prove the text *survives*.

So the gate does both halves:

```
CREDO_CHORDA_AEQUALIS (fons_reditus, fons_originalis);   /* octetus pro octeto */
...
CREDO_AEQUALIS_I32 (congruentia, (i32)273);              /* et omnia sigilla congruunt */
```

45,144 bytes through JSON encoding (twice — `datum` is a *string*
containing JSON), the wire, SQLite, and back: **identical**, and all 273
seals match. Anchors survive the store.

The byte comparison says the text is equal. The seal comparison says the
thing that actually matters — that no comment would be orphaned. Both are
kept because they fail differently.

### The gate found the clobber by suffering it

First run: 29/30. The failure was section V's negative control — mutating
an *unknown* field should produce a `violatio camporum` note, and no note
appeared.

Cause: section III had deliberately collided the genus (a second
definitio with `clavis: "liber"` and no `campi`) to demonstrate that the
`addere` response reports a clean success. That collision **clobbered the
registry row**, and `_campos_iudicare` performs no judgment at all on a
genus with no `campi`. So the collision demo silently disabled the
judgment demo two sections later.

This is exactly the blast radius the exploration described — reproduced
accidentally, inside my own test, within minutes of writing it down.

Restructured so the ordering is deliberate and the consequence is
**asserted rather than suffered**:

| §IV | judgment lives — declared field clean, unknown field flagged |
| §V | collision — response is clean **and judgment is now dead** |
| §VI | recovery — re-declare, judgment returns |

§V is now the most valuable section in the file: it pins the real danger.
The documented complaint was "the response doesn't tell you." The actual
cost is "a colliding definition silently switches off field validation
for every entity of that genus, and nothing anywhere says so."

**The general lesson about tests:** these sections had a hidden ordering
dependency through *shared store state*, and the symptom appeared two
sections away from the cause. Braced blocks look independent and are not
when they share a daemon. Where order matters, say so at the top of the
file — which the header now does.

### Details

- **`datum` must be a JSON-encoded *string*, not an object.** Built with
  `json_scribere` on a constructed value, then passed as a chorda. The
  wire escapes it again; the double encoding round-trips cleanly.
- **`status` is not a field.** The genus declares `status_libri`;
  `status` is one of the eight reserved keys and would be both unjudged
  and projected to the lifecycle column.
- **`res {breviter}` is asserted to stay under 4 KB.** Plain `res` prints
  raw datum verbatim and would dump 45 KB into a response — a gate
  against the app (or me, post-compaction) reaching for it.
- **`<stdlib.h>` for `system()`** — the implicit-declaration error is
  loud, but worth noting since the villa test gets it via another header.

### Instrumenta

- **adhibita:** `cliens_tabularii` against an ephemeral daemon, credo,
  censor, `compile_tests.sh`
- **fructus:** the ephemeral-daemon fixture copied from
  `probatio_villa_agens.c` worked unmodified — the second consumer of
  that pattern, and it cost about ten minutes rather than an afternoon.
- **asperitates:** `compile_tests.sh` summary **third firing** of
  `01KYB29BC6` — `| grep 'Tests Passed'` returns nothing, and an empty
  grep *exits 1 silently*, so "no tests ran" and "all tests passed" are
  indistinguishable to a caller. Stripping ANSI in a live pipe still
  failed; the reliable form is redirect-to-file then strip. A
  `-sine-coloribus` flag would delete the whole workaround.
- **desiderata:** none new.

---

## 2026-07-25 (later still) — S3: the reading view

**INTENTIO.** Put the document on screen: a verbatim column with a
gutter, a part sidebar, a jump box, a document header — and only the
reading view. Comments and the side panel wait for S4, which needs
anchor machinery that does not exist.

**RELATIO.** Shipped in two passes. `09de031` plumbing (parse handler +
fumus gate), `172b1a6` the view, `a2a843d` grade highlighting.
Fumus PLENUS; suite 108/108. Fran validated live against a real
document.

### Step zero was a build bug, and it mattered more than it looked

`apps/forum/forum.sh` never ran `capsula_generare`. Villa's launcher
does, and names the bug in a comment. Consequence: every edit to
`index.html` was invisible until the capsula was regenerated by hand.

The part that makes it worse than a papercut: **`fumus.sh` pre-builds
*through* `forum.sh`**, so the app's only automated gate was validating
a UI nobody had written. The bug was *latent* (capsula and source
happened to agree), which is the dangerous state — the first symptom
would have arrived mid-UI-work and looked like a rendering fault.

Fixed and proven, not assumed: touch `index.html`, run the launcher,
watch the capsula regenerate.

### The handler returns everything except the text

`sententias_parsare {fons}` → loci, lines, parts, grades, anomalies,
and **seals**. Deliberately **not** point text.

The reader renders from `fons` itself — that is what verbatim means — so
returning parsed text would duplicate 45KB for no consumer. What the
handler returns is precisely what the webview cannot compute: where the
loci are, what they mean, and SHA-256 over normalised text.

Consequence worth noting: the sidebar needs question *text*, and gets it
by slicing `fons` at the line numbers the tree supplies. Verbatim by
construction rather than by discipline.

### The bug that cost the most, and its signature

```c
character hex[SIGILLUM_HEX_MENSURA];          /* stack */
json_chorda_creare(piscina, _ch_forum(hex))   /* dangling */
```

`json_chorda_creare` stores the chorda **without copying the bytes**.
`json_chorda_creare_literis` is the copying twin.

**It presented as a hang, not a crash or a rejection.** The serializer
emitted reclaimed stack, the webview couldn't parse the response, and
the promise never settled — so the `.catch` never fired and the smoke
died on a bare timeout with no cause. Full law in
`debugging-lessons.md` (corrupt-response-hangs).

Irony worth keeping: `sententiae.h` §VII preaches "no view into the
incoming source survives," and I violated exactly that one layer up, in
app code, where lifetimes weren't front of mind.

### A gap in the harness, fixed permanently

`culpam_ostendere` writes to the **webview only**. So a failed assertion
inside `fumus_genera` was indistinguishable from a hang — both produce a
bare timeout. The catch now posts the message via `mittere`, because C
prints sent bodies to stderr.

It paid for itself twice within the hour: once diagnosing the dangling
pointer, once when the highlighting assertion failed and named itself
immediately.

Also reads `.message` before `.nuntius` — `internuntius.vocare` rejects
with an `Error` while the app's display reads `.nuntius`, so real daemon
error text was silently vanishing.

### The column-0 invariant now has three consumers

Dimming for retracted points first reset only on blank lines, so a
retracted point's dimming **bled into the closing fence**. The correct
rule is the parser's own: continuations are always indented, therefore
any column-0 line that isn't an address closes the preceding point.

Then the grade highlighter needed the *same* invariant a third time, to
clear open-bracket state. Three consumers of one law is the point at
which it stops being a parser detail and becomes the format's grammar.

### Highlighting had the parser's trap in it

Multi-line brackets again. Six of the seven retracted points in
`de-imagine.md` open on one line and close on another — so a per-line
highlighter would leave **exactly the ones that matter** uncoloured. The
painter carries bracket state across lines.

Pinned as: 5 spans for the test document, of which **2 are halves of one
grade**, plus an explicit check that the closing half exists.

Also pinned: the painted text is byte-identical to the source line, so
highlighting can never drift from verbatim rendering.

### Details worth keeping

- **The tab is poll-exempt.** Documents don't change unless pasted, and
  polling every 2s would pull 45KB each — five saturate the 256KB client
  buffer and the sixth shows a blank tab. Load on tab entry, parse on
  open. Sidesteps §XIV rather than mitigating it.
- **Reused**: `.linea` flex/gutter shape, `body.latus`, `.sigillum` as
  chips, `#barra-superior`. **Not reused**: the Scholia row-alignment
  strategy, which deliberately opens a gap in the text when a comment
  card is tall — the opposite of §VIII.3.
- **`nav-articuli` is active by exclusion**; forgetting to add the new
  view to that chain lights up Articuli on the new tab.
- **Insertion is new-documents-only.** The document is parsed before it
  is stored, so a structural error never enters the store. Re-paste with
  anchor-damage preview is S4 — with no anchors yet, nothing can break.
- The genus guard reads the `definitio` table, never the `addere`
  response, matching `scholia_genera_curare`.

### Instrumenta

- **adhibita:** fumus (real window, DOM assertions), censor, clang,
  `compile_tests.sh`, git stash for a baseline
- **fructus:** the baseline-by-stash was decisive — I had never run this
  fumus before my own changes, and assuming it was green would have sent
  me hunting in the wrong file. Establish the baseline before diagnosing.
- **asperitates:** `compile_tests.sh | grep` still returns nothing
  (ANSI + pipe); only redirect-to-file works. Third ictus recorded.
- **desiderata:** `01KYCVNJ0H` — a `servat` lifetime contract as the
  second member of the existing `<contractus>` family, which would have
  caught the dangling chorda statically.

### Next

**S4** — the `locus` ancora resolver and the paste damage-preview gate.
It is the only stage whose machinery does not exist: nothing currently
answers *"which anchors point into this document"*. Decide between a
`LIKE` scan over `res.datum` and a materialised anchor index **with a
measurement**, not in advance.
