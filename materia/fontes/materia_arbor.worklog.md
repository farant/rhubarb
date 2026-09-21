# materia_arbor worklog

## 2026-09-17 — sedes A1: the positioned view (materia-sedes-plan A1)

**What.** `MateriaArborConsilium.sedes_scribere` (last field) makes the
writer stamp `sedes="L:C-L:C"` and `octeti="B-B"` on every node element,
token element and trivium element; locus wrappers, `ante`/`post`, and
the envelope get nothing. The envelope says `visio="sedes"` (or
`"partialis sedes"` with `loci_admissi`), and the reader refuses it with
a cause naming the value (`"visio sedes: non arbor"`; the old
`"visio partialis: non arbor"` is byte-identical for the filter alone,
same attribute order `visio`, `omissi`).

**The range lives in `materia_nodus`, not here.** `materia_tractus_lexematis`
/ `_nodi` / `_conferre` sit beside `materia_sedes_tokeni`, because B's
derivation walker and the writer's refusal ranges need them without the
writer. Start = the effective position (through the origin hook); end =
start advanced over the value by the reader's own cursor rule (`\n` →
next line, column 1; any other byte → column + 1). A token whose hook
says `est_fons` FALSUM (md and oratio derive tokens today) is a
zero-width POINT: its value is not the bytes at that place.

**Node ranges are accumulated bottom-up in the one walk.** The scriptor
carries `tractus` + `tractus_inventus`; `_scribere_nodum_internum` saves
the parent's pair, resets, lets every token use (the transclusion path
too — the node owns the token even when its element is a reference)
merge into it, stamps its own element after the loci, then merges its
range into the saved parent pair and restores. O(n); calling
`materia_tractus_nodi` per node would be O(n·depth). Trivia never merge:
they hang off tokens, not loci, so a node's range never starts at the
comment above it. `materia_tractus_conferre` keeps the first fons it
sees (only C89 includes can mix fontes).

**Measured / noticed while building.**
- Every `MateriaArborConsilium` in the tree (sources, probationes,
  instruments, briar, the C89 shim) is filled by `materia_arbor_consilium_nudum`
  or a helper that calls it; the one struct copy (`cv = c`) copies such
  a consilium. The new field is therefore zero wherever nobody sets it.
- `sedes_colligere` / `paria` are collected but `sedes_valorum` is never
  filled (D2) — left alone; A2's gate pairs elements by document order.
- A test comment holding `/*` broke the build (`-Wcomment`) exactly as the
  memory warns; examen's "domesticum" for `strstr` on a chorda's data does
  not block, but the new gate uses `chorda_ut_cstr` and adds none.
- Gate `probatio_materia_sedes`: 63/63 at first compile; planted
  `finis + I` in `materia_tractus_lexematis` → red at `t.finis == 13`,
  reverted → green.

## 2026-09-17 — sedes A2: the shared verifier, five client gates, instruments

**`materia_sedes_verificare` (new `materia/fontes/materia_sedes.{h,c}`).**
One library call every client STML gate makes per corpus file, like
`materia_registrum_recens`. Its oracle is disjoint from the writer's
arithmetic: it builds a LINE TABLE from the source bytes (binary search
for line/column), computes expected ranges by its own walk (writer
order: node pre-order; a first-use token, then its `ante`, then its
`post` trivia; a repeated use adds no element but still counts toward
the node), pairs them with the STML elements carrying `octeti` in
document pre-order, checks `octeti`, `sedes`, and for source tokens the
source slice against the value. It also checks the view equals the
plain projection minus `sedes`/`octeti`/`visio` (structural walk over
both arbor-tradita trees, two interns so titles compare by content),
and that the reader refuses the view. It refuses by name what it cannot
pair: `loci_admissi`, `templa_activa`, a frons with `liberos_ornare`.

**Measured over the corpora (all green at first run):** crusta 339,503
elements; css 9,679; html 16,439; md 2,491,843; oratio 2,643,746 — zero
divergences. **Zero derived points anywhere:** md's and oratio's derived
tokens carry `byte_offset` −1 (synthetic), so they get no attributes at
all. D1's point rule is exercised only by materia's own gate today.

**Size.** Over the 237 house `.sh` files the view is 57,459,770 bytes
against 23,264,631 for the plain projection (×2.47; the source is
826,532 bytes). Most of the growth is the pretty writer breaking an
element with two long attributes onto aligned lines. Opt-in; not pinned.

**Instruments.** `-sedes` on `crusta/`, `html/`, `md/`, `oratio/arbor.sh`
(`consilium.sedes_scribere = sedes` right before the write).

**Plant.** Writer column `+ I` → `probatio_crusta_stml` red at
`sedes.sana`; reverted → green.

**First commit attempt refused (the gate working).** The oratio
identifier lint in the shadow clone saw the untracked file for the first
time and named `verificatio` (the struct `Verificatio`) as a new unknown
word — `quaere.sh` had been run on `verificare`, not on the noun. Renamed
`Collatio`; the two-letter locals of the structural compare (`ia`/`ib`,
`na`/`nb`, `sn`/`so`) became full words before the retry. Lesson: run
`quaere.sh` on EVERY identifier word of a new file (a regex split of the
file's identifiers), not on the words remembered as new.

**Second refusal: crusta's computus golden.** `crusta/probationes/fixa/computus/basis.tsv`
measures five FIXED house files, one of them `html/compile_probationes.sh`,
which A2 edits (its materia module list gained `materia_sedes`). The
row moved by exactly that edit (+25 bytes, +1 token, +2 nodes, +13
allocations); regenerated with `COMPUTUS_SCRIBERE=1`, the other four
rows unchanged. A runner edit is a corpus edit for crusta's computus
gate — run `crusta ... computus` before committing any change to those
five files.

## 2026-09-17 — A1's struct change left stale objects in five client builds

A1 appended `sedes_scribere` to `MateriaArborConsilium`. The client
runners (css, md, html, oratio, crusta) rebuild an object when its
`.c` is newer or when any header in a HAND-WRITTEN list is newer, and
that list was `include/` plus the client's own `fontes/` and
`probationes/` — never `materia/fontes`, the substrate every one of
them compiles. So nothing that merely included `materia_arbor.h` was
rebuilt. After today's runs `excubitor.sh` still counted oratio 24,
html 20, md 14, css 10, crusta 9 stale objects (23 of oratio's behind
`materia_nodus.h`/`materia_arbor.h`, with its runner freshly run).

It was more than cosmetic. `oratio_resolutio.c:2043` holds a
`MateriaArborConsilium` on its stack; the stale object reserved the
old size, then called down to the fresh `materia_arbor_consilium_nudum`,
which writes the new last field past the end — undefined behaviour
in every local oratio run since A1. The commits were never at risk:
shadow gates build from scratch. And measured afterwards, the oracle's
873 rows on fully rebuilt objects are byte-identical to the morning
baseline taken on the stale ones, so the overwrite happened to land on
something harmless.

How it surfaced: A3's re-measure. `oratio/oraculum.sh` links
`oratio/build/*.o` and relinked only when an `oratio_*.o` was newer,
so a `lib/stml_macros.c` change gave "rows identical" from the
previous day's binary. Sixteen instruments across crusta, html, md
and oratio had the same prefix-only glob; their siblings
(`briar/arbor.sh`, `oratio/reticulum.sh`, every `computus.sh`) already
watched `*.o`.

Fix, three layers:
- the five guards list `materia/fontes` (html and oratio also
  `md/fontes`, which they compile);
- after the object loops each runner calls `./excubitor.sh
  <cliens>/build/ -tacitus`, whose include graph is DERIVED
  (`build/inclusiones.tsv`), so the next forgotten directory is caught
  without anyone remembering to add it: stale → print the report and
  exit 2 before any test compiles; excubitor unable to judge (graph
  absent) → a loud CAUTIO;
- the 16 instruments relink on any newer `*.o`, and both `oraculum.sh`
  refuse stale objects with exit 2, naming the runner to run. The
  per-file `arbor.sh` instruments do not check: `silva.exemplaria`
  calls them hundreds of times and the check costs about half a second.

Born red: crusta's runner with its old guard planted back left 8
objects stale behind materia headers and stopped with exit 2 and
"NIHIL CURSUM"; restored, it rebuilt 15 objects and ran. Both oracles
refused with exit 2 on one backdated object. Touching a non-prefix
object (`stml_macros.o`) relinked the oracle binary. All five runners
then left excubitor PURUS. Crusta's computus golden moved on its
`html/compile_probationes.sh` row only (8233 → 9525 bytes, 1517 → 1620
tokens, 1583 → 1697 nodes, 8456 → 9052 allocations): the runner is one
of its measured house files.

Lesson: a guard built from a hand list of directories drifts the day
the dependency graph grows; a guard that reads the derived graph does
not. Check with the derived one, keep the hand list as the fast path.

## 2026-09-17 — one walker derives the declared diagnostics (B2)

`materia_diagnostica_derivare` (`materia/fontes/materia_diagnostica.c`)
turns B1's baked declarations into located records over ANY tree —
parsed, read back from STML, or a view. One pre-order walk: a node's
rows are recorded BEFORE descending, so "the last source token seen"
is the one before the node, which is what a node with no tokens of its
own points at (`x (` → both of the empty `par`'s rows sit just after
`x`). Tokens are visited in BYTE order (ante trivia, token, post
trivia) — the emitter's order, which is what the byte-order check
needs. A token used twice (transclusion) is counted once, through a
hash of the pointer's bytes.

Three species, and where each points: GENUS at the node's range,
VACUA at the node's range, ABSENTIA at a ZERO-WIDTH point at the
node's end ("`)` expected HERE", after the last token, not on it).
`vacua` is true for an unwritten list as well as one holding only
nodes of `inanis` genera (D5). Every client also gets one check with
no declaration at all: `materia:ordo-octetorum`, a source token that
starts before the greatest end already seen in the same fons — crusta's
`heredoca_transposita`, generalised. `emissa` carries what a parser
must report itself (a class that leaves no trace in the tree, D6);
a record with `initium` −1 and a node gets its range computed here.
Output is sorted by (initium, codex), stable.

The writer's refusals now carry a place: `MateriaArborScriptura` gains
a last field `tractus`, filled from the refused TOKEN when the refusal
knows one (the scriptor remembers it in `lexema_refutatum`) and from
the `sedes` node otherwise; `initium` −1 means no place at all. A NUL
inside a token value now reports `6`, `2:3` instead of just "scriptura
fracta".

Traps met while building:
- **`registrum` is a latina macro** (`register`). The plan's signature
  named a parameter `registrum`; the header parsed into an error node
  and `scribe` refused it. The parameter is `genera`.
- A `(vacuum*)VERUM` sentinel would be an int-to-pointer cast; the
  seen-set stores the address of a file-scope `PRAESENS` instead.
- The NUL fixture cannot use a string literal: `-Wwrite-strings` plus
  `-Wcast-qual` reject `(i8*)"a\0b"`. It builds a local array.

Gate `probatio_materia_diagnostica` 58/58, green at first run and
therefore proven only by its plants: the `inanis` list never matching
(the separator case), `_punctum_prius` always returning the file start
(the no-token case), the order check disabled (the transposed case),
the seen-set disabled (the twice-used token case) — each red at its own
assertion, green on revert. Two more plants did not compile and ran
nothing, which `silva.planta` reported instead of pretending: removing
a call left a helper unused under `-Werror`.

## 2026-09-17 — the derivation walk: iterative AND linear (B3 found it)

B2's walker was recursive, and its header claimed "the walk has the
writer's depth limits, no worse". Wiring crusta's agreement check into
the fuzz gate disproved both halves at once (quaestio 01M2R9MKFQ).

First fault: two mutually recursive functions, two frames per level.
Measured with a crusta nest: 20,000 deep lives, 40,000 SIGSEGV — while
the writer lives at 40,000 (a pinned fuzz case) and dies at 45,000. So
the walk was SHALLOWER than the writer it claimed to match. Fixed with
an explicit stack.

Second fault, hiding behind the first: the fuzz gate still failed, and
not by crashing. `_nodum_notare` asked `materia_tractus_nodi` for each
node's range, and that function walks the node's whole subtree — so a
deep chain re-walked it per level, O(n²). Timings with the check on:
10,000 → 1.6 s, 20,000 → 7.0 s, 40,000 → 31.6 s (0.02 s without).
credo kills a child past 5 s and reports it as a crash, which is what
"CREDO_NON_RUIT failed" actually meant. The lesson is old and worth
re-learning: a timeout and a crash look identical from the outside.

Now the walk accumulates each node's range bottom-up exactly as A1's
writer does (`d->tractus` / `tractus_inventus` saved per node on the
stack, every locus token merged, trivia never), and records a node's
rows when its subtree is finished. Post-order is equivalent for the
"point" rule: a node with no range has no source token anywhere
beneath it, so "the last token seen before it" is the same either way,
and the output is sorted by (initium, codex) regardless. A token used
twice still merges into both parents (the seen-set now guards only the
order check, not the range accumulation) — that matches
`materia_tractus_nodi`, which visits each parent independently.

After: 10,000 → 0.18 s, 40,000 → 0.06 s (500×), and at 100,000 the
walk survives while the writer's recursion dies (01M1FAD8) — the
header's claim is now true. Pinned in materia's own gate: a 50,000-deep
chain yielding 50,001 diagnostics.

## 2026-09-19 — `nul`: the option that was already there

Fran asked whether `&null;` or `<byte value="0000_0000"/>` would be a
reasonable way to carry NUL through STML. Neither can work, and the
reason is the same for both: **the values that need it are RAW.**

- `&null;` — entities are not resolved inside raw content. That is
  what raw means. Making raw resolve exactly one entity is the same
  bargain that was rejected for `<\/crusta-litteralis>`: half-raw is
  not raw.
- `<byte/>` — an element makes the value MIXED, and `elementum mixtum
  crudum esse NON potest`. You would trade raw away to keep NUL, and
  lose the whitespace case and the closing-tag case with it — solving
  one problem by un-solving two neighbours.

The option already in the file, twelve lines above the refusal, was
`_cr_exuere`: **strip the byte, keep its offsets in an attribute,
reinsert on read.** CR needs it because STML normalises CRLF even
inside raw. NUL needs it for the same reason and in the same place.
So the pair became `_octetum_exuere`/`_octetum_induere`, parameterised
by (byte, attribute name), with `cr` and `nul` as the two call sites.

It is worth naming why the old refusal looked principled. Its comment
said NUL is unrepresentable "quia chorda longitudinem fert et textus
terminatore legitur" — the first half true, the conclusion false. The
limit was never in rawness; it was that nobody had extended the
neighbouring mechanism. A guard citing a true fact for a false
conclusion reads exactly like a law.

**Measured:** html `circuitus` 1,672/1,708 → **1,708/1,708**, NUL
refusals 36 → 0. Every client suite green (materia 14, css 10, md 14,
html 14, crusta 16, oratio 19, shim 398). `nul` declared beside `cr`
in all six canons.

### The plant that did not fire, and what it cost to find out

Write strips CR first (offsets in the TRUE value — the old contract,
so existing `cr` documents stay byte-identical) then NUL (offsets in
the CR-stripped value); read restores NUL first, CR second. I wrote a
paragraph of comment about that ordering.

Then I planted it: swapped the read order. **Every suite stayed
green** — materia 14/14, html 14/14, crusta 16/16. The law was
entirely untested, because order only matters when ONE value carries
BOTH bytes, and no corpus case does: html5lib's NUL files have no CR
in the same token, and crusta's three fixtures are NUL-only.

Added a case to `probatio_materia_arbor` carrying `a\r\0b`, asserting
the bytes and not merely the length — the swapped order returns the
right LENGTH with the positions exchanged, so a `mensura` check passes
and a `memcmp` fails. With the case in place the same plant fires, on
the memcmp, exactly as it should.

Three plants total: order swap (fires only after the new case — the
finding), NUL strip disabled (materia, html and crusta each go red),
and the earlier refusal pins, which went red on their own the moment
the mechanism landed and were promoted to round-trip assertions rather
than flipped.

### Gates promoted, not relaxed

Four gates asserted the old refusal. None was merely inverted:

- `probatio_html_circuitus` — its comment had PREDICTED this ("cum
  materia recidat, hae rubent et casus in circuitum promoventur"). Pin
  36 → 0.
- `probatio_html_totalitas`, `probatio_crusta_totalitas` — now assert
  the full round trip and that `nul=` is actually present. Asserting
  success alone would pass if NUL were silently dropped.
- `probatio_materia_diagnostica` — NUL there was only the VEHICLE for
  triggering a writer refusal; the subject is that refusals carry a
  position. Vehicle swapped to a value carrying its own closing
  sequence, which still refuses. Worth recognising the shape: a test
  that uses a limitation as scaffolding breaks when the limitation is
  lifted, and the fix is a new trigger, not a weaker assertion.

## 2026-09-21 — the closing-sequence refusal is gone

`_valorem_crudum_notare` no longer refuses a value carrying its own
closing sequence; it just marks the element raw. `lib/stml.c` carries
these values now (escape ladder, plan steps 1–2), and materia builds
an `StmlNodus` tree that `stml_scribere` emits — so the escape follows
automatically and **no escaping code belongs here at all**. The task
was a deletion.

Fran's original trigger now lints clean:
`# vide </crusta-commentum> hic` → `0 diagnostica`, exit 0.

**The gates named it, in the shape they were built for.** crusta's pin
carried the note `RUBET CUM MATERIA RECIDAT - tunc in CASUS
promovendi`; it went red on the first run after the deletion and was
promoted. Same for html's five. Counters moved with causes: crusta stml
`IDEM` 51 → 52 and `RECUSATA_SEQUENTIA` 1 → 0; crusta canon `iudicata`
51 → 52 (judged is stronger than not-refused — the document also passes
the canon).

**Two call sites, and only one of them is where you'd look.** The raw
mark is set from the lexeme path AND the trivia path. Planting the old
guard back on the lexeme path alone reddened `probatio_crusta_stml` but
left `probatio_crusta_canon` GREEN — because crusta's case is a
*comment*, which travels the trivia path. Planting both reddened both.
A single plant would have left half the change unverified.

**`probatio_materia_diagnostica` lost its refusal vehicle for the
SECOND time**, and that is the finding worth keeping. It asserts the
*tractus* of a refusal, so it needs some refusal to ride on. NUL was
the first vehicle until the `nul` attribute carried NUL (2026-09-19);
the closing sequence was the second until today. Both were **limits the
house intended to remove** — so the test was scheduled to break from
the day it was written.

The third vehicle is an unknown token genus: a genus the lexicon does
not contain cannot be written *by definition*. That is a violated
contract, not a missing capability, so nothing will ever "fix" it.
**Rule: choose a refusal vehicle from the contract, never from a
limit.** The cause is now pinned too (`genus lexematis ignotum`) — the
two previous vehicles each changed silently underneath an assertion
that only checked *that* it refused, never *why*.

`stml_crudi_terminatorem_fert` now has no production caller; only the
gates use it. Left in place — it is a legitimate public predicate — but
worth a look if it is still caller-less in a month.
