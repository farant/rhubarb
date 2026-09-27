# Silva → materia (phase 5), plan B — steps 5–6

> Inline on main, one task per turn with Fran's approval, no subagents.
> Re-issued 2026-09-25 at the step-4 boundary (T13 seal) from what
> steps 1–4 found — the sketches in `silva-migratio-plan.md` (T14–T19)
> were written before the migration and are superseded by this file.
> Spec: `project-specs/silva-migratio-spec.md` §4 step 5–6 (D5: the
> diagnostics payoff is INSIDE phase 5, as 5.x). Ledger: park …FE9E
> "phase 5" (decree …XX0BZ); one `opus` per task, closed by
> `silva.commissio(opus=ID)`. Narrative: `materia/phase-log.md`.

> **STATUS.** PHASE 5 COMPLETE 2026-09-26 (T19c-1: park …FE9E closed,
> decree …XX0BZ executed - JS next by its order). Log below. Plan A closed at T13 (b4abedbd +
> T13c): silva consumes materia, M3 6/6, oracle 470 clean, amalgam
> regenerated, freeze lifted.
> T14 RE-SCOPED by its measurement (Fran, 2026-09-25) and DONE: no
> `c89.registrum.stml` (the grammar already derives the registry —
> a declaration would be a second source); instead the declared-
> diagnostics half (`<genus diagnosticum=…>` in c89.stml →
> `SILVA_C89_DIAGNOSTICA`) and a C89 route in `tools/diagnostica`
> (header closure via aedilis, true include order, principal-file
> filter, list root); silva_frons's sedes hook now reports the origin
> ROOT. 430 house C files clean; gate `diagnostica` XXII planted red
> twice. lexicon_c89's generator moves to T19.
> T15 DONE 2026-09-25: the GLR's death point (already returned, then
> dropped by the driver) kept as `SilvaParsura.mortes` (last field) and
> emitted by `silva_mortes_diagnostica`: primary "hic exspectatur" at
> the death lexeme, related "hic coepit" at the unit start; limit and
> interruption become MONITUM. materia: emitted supersedes declared for
> the same node + code; the diagnostic record split into
> `materia_diagnosticum.h` (header without body) so emitters don't drag
> the lint engine into silva's amalgam. aedilis refuses absolute paths
> — the tool relativizes; T14's plant had silently used the fallback.
> T15b DONE 2026-09-25 (Fran asked after T15; T15's own text named
> examen): examen locates syntax errors (one row per GLR death point,
> real line/column — the post-edit hook now tells agents WHERE) and
> prints excerpts in human mode (two-location for syntax, caret for
> semantic findings). examen_vectis.sh was not a registered gate at
> all — now 'examen-vectis' in PORTAE + inventory. legati's own copy of
> the format is desideratum …W87Q (needs a public silva.h surface).
> T16 SPLIT by its measurement (Fran, 2026-09-25): (a) chosen — the
> fix belongs in the system stand-in, not the grammar; but the stand-in
> is loaded only by examen and the officina tools. T16a DONE: va_arg,
> offsetof, va_start, va_end, assert defined in systema_c89.h (the class,
> not the case; lib/silex.c lost 5 false suspects); tools/diagnostica
> adopts silva_lexicon (the common composer examen uses). T16b next:
> briar's pass 2 adopts silva_lexicon (stand-ins riding the embedded
> corpus) — fixes bugs/009 on the reporter's own path.
> T16b DONE 2026-09-25: briar's three silva parse sites share
> `briar_silva_parsare` — context + composed system lexicon (stand-ins
> in the embedded corpus) + closure headers, public API only;
> silva_lexicon compiles into briar (its one silva-internal dependency
> moved to silva/fontes/silva_limes_posix.h). variadica.thistle parses
> and runs; planted red. lapide bugs/009 fixed on every path.
> T17 SPLIT by its measurement (Fran, 2026-09-25): erasing macro, not
> grammar — an empty expansion already leaves `<invocatio-vacua>`
> (queryable) and examen's `tolera` matches by LINE (T15b's "examen does
> not read annotations" was WRONG: silva_c89_semantica.c has read
> `<tolera codex=… (>causa>` since 07-17; it needs a node only for the
> position). Grammar form parked (…WFM6). T17a DONE: `__attribute__` is
> a built-in empty macro of the expander, outside the table (defined()
> false as in clang; user definition wins); the silent `} __attribute__
> ((packed));` misparse is gone too. briar fixture planted red. T17b
> next: the rule + per-line tolera; severity is Fran's.
> T19b ADDED (Fran, 2026-09-25): step 5 promised located errors in
> "examen, legati and briar" — legati (…W87Q) and briar (…SQ9) still
> print only the first line.
> T17b DONE 2026-09-25 (VIOLATIO, Fran): code 91 EXTENSIO_COMPILATORIS
> over three homes (extent laminae, directive lines, untaken branches;
> `#if 0` skipped), per-line tolera; the annotation collector now sees
> comments in those homes too (else `#ifdef __GNUC__` could not be
> exempted). examen_vectis ⑥ planted red twice. Found for T19:
> `examen_vectis.sh -corpus` exclusions stale since ~09-14 (no gate runs
> it). T18 next.
> T18 DONE 2026-09-25: the line cost only the oracle (its pinned binary
> carried the old latina.h compiled in: 810 position-only divergences).
> Fran: code from the pin, inputs from today — the builder transplants
> today's embedded latina into the pristine pinned amalgam (guarded,
> cached); pin unmoved. T19/T19b remain.
> T19 RE-SLICED by its measurement (Fran, 2026-09-25): T19a dead tools
> get teeth · T19b printers · T19c MAP/census/MEMORY + close. Decided:
> silva's duplicate helpers STAY (…PE7W — exporting four utilities would
> widen materia's API; the real duplication is silva's own STML reader);
> the LR toolkit STAYS in silva (…1GHY — no other consumer).
> T19a DONE: `#line` consumed (C89 6.8.4; every briar product failed);
> aedilis builds against the header extracted from silva.c (…XHAW3H);
> gates 'generata' (amalgam pairs, lexicon, 37 snippets, amalgamator
> manifests — aedilis.stml covered at last) and 'examen-corpus'
> (re-pinned; bitsyfont's real C99-ism fixed). T19b next.
> T19b RE-SLICED (Fran, 2026-09-25): T19b-1 one printer · T19b-2
> message quality (found token, latina.h hint, missing-';' heuristic if
> cheap) · lapide's "every error" PARKED (one GLR death per recovery
> unit). T19b-1 DONE: `silva_mortes_scribere` (silva_frons, public in
> silva.h) is the one home of the syntax-death format; examen (bytes
> unchanged), legati (…W87Q) and briar (bugs/001: death line in .thistle
> + excerpts, was the region start) call it. T19b-2 next.
> T19b-2 DONE 2026-09-25: deaths name the found token, add a latina.h
> hint when a name was expected (lapide's `nomen` case), and suggest a
> missing ';' (expected token kinds now kept on the death record). T19c
> (MAP/census/MEMORY, close park and decree) is the last task.
> T19c RE-SLICED: T19c-1 DONE 2026-09-26 - census (3 drifted counters
> refreshed; materia entry + silva as its C89 frontend + oracle
> doctrine), lapifex landmine re-checked (alive: nuntium_schema builds
> on it, test green, silex + schema generator use it), ledger closed
> (4 open items moved to region 'c89 parser'), MEMORY/CLAUDE.md final.
> MAP.txt refresh = SEPARATE task (stale since 08-17; mostly unrelated
> to phase 5: briar, crusta, css, oratio, pythonica absent).

## What steps 1–4 taught (the inputs to this plan)

1. **Every task's measurement resized it** (T6, T7, T10, T13 split;
   T12 closed without code). Keep the rule: each task below OPENS with
   a measurement and is re-sliced with Fran if it is more than ~2× the
   estimate. Bodies here are task-level on purpose.
2. **Dry runs in a scratch snapshot** (`git archive HEAD | tar -x`)
   found T13's three layers without touching the tree. Use them for
   any task that regenerates committed artifacts.
3. **The oracle stays the judge.** Anything that changes silva's
   output on purpose (T15's diagnostics do not; T16/T17 do) is a NAMED
   divergence in the dispares file, never counted away. The pin
   (7a4847b0) moves only by decision, with the old pin recorded.
4. **Tools that nobody runs are dead** (ligare dead since fed3e78,
   aedilis blind to materia since the fork). T19 audits the rest.
5. **Hand-written silva.h is a live wire**: consumers compile it
   against the committed silva.c (desideratum …XHAW3H). Any task
   touching the public surface regenerates in the same commit.

## Global constraints

As plan A (C89 in Latin via latina.h, house flags, `i32` unsigned,
chorda not NUL-terminated, new C files via `scribe`, gates born red by
a plant that COMPILES, worklog first and gates last, formator
`-scribere` then `-vitia`, words via `./oratio/quaere.sh` then
`./oratio/vocabula.sh -nova`, never `silva.Editio` on a `.sh`, every
new gate in pythonica's table AND the 'suitae probationum' inventory).
**Every commit green:** silva, oracle, M3, consumers (briar,
saltuarius, aedilis, officina, amalgamata) and, when materia moves,
every materia client (css, md, oratio, html, crusta).

---

## Step 5 — the payoff: C89 in the diagnostics pipeline

### Task 14: the C89 registry, declared

`silva/grammatica/c89.registrum.stml` like crusta's
(`crusta/grammatica/crusta.registrum.stml`) → `coquere`; declared
diagnostics and seals apply to C89; `lexicon_c89` generated FROM the
declaration (today `silva/fontes/silva_lexicon_c89.c` is generated from
silva sources, a shim-era artefact that entered the amalgam at T13a).
**Measure first:** what crusta's registry declares that C89 would
need, and whether silva's `silva_registrum.h` tables (genera, loci)
can be emitted from the declaration byte-identically (the oracle's STML
path reads them). Gate: oracle byte-identical (registry is not output),
M3, `coquere` seal stable.

*T14 as built (2026-09-25).* See the STATUS line; details in
`tools/diagnostica.worklog.md` and `silva/fontes/silva_frons.worklog.md`.
Also found: `silva/generare.sh` lacked `-I materia/fontes` and
`materia_token.o` — not run since T8; fixed, regenerates byte-identical
(sceletum, imparilis, silva.h splice, hospes splice, c89.canon).
Grammar canon gained `diagnosticum`/`gravitas` on `<genus>` (names as
materia/grammatica/registrum.canon).

### Task 15: the GLR names where it died

The GLR records the token where all heads died (+ expected kinds if
cheap) and emits it through `emissa`: primary sedes "hic exspectatur",
relata "hic coepit" at the unit start — clang's two-location shape.
examen, legati and briar print it via `lib/excerptum`. Closes lapide
bugs/001; briar's clang relay (desideratum …SQ9) becomes the second
opinion. **Measure first:** where `silva_glr.c` loses the death point
today, and what the adversarial corpus (M3 bar, 24) yields — every
adversarial file must get a located error, planted-red. Gate: M3 with
a new assertion on the adversarial bar (location present), oracle
unchanged (diagnostics are not STML output — verify).

### Task 16: `va_arg(va, character*)` (lapide bugs/009)

An ordinary frontend fix, landed as a NAMED oracle divergence (the
pinned silva fails where the live one parses). Measure: the minimal
repro and which layer rejects it (expander vs GLR).

### Task 17: `__attribute__((…))` (lapide bugs/010)

**DECIDED (Fran, 2026-09-25; decree in the ledger, T17 tag):** silva
ACCEPTS the extension — parsing never breaks on vendor or foreign
code — and examen gains a rule that NAMES its use in house C89 code,
UNLESS the author exempts it with a declared comment annotation (the
house's excusatio pattern: STML in a comment, attached structurally —
cf. crusta's excusatio, materia_excusatio, the T11 annotation kernel).
The measurement decides: (1) accept via a stand-in macro vs a grammar
form — the use must stay QUERYABLE ("all `__attribute__`" is a
code-as-database question; an erasing macro keeps it only through the
origin chain, a grammar node moves the registry seal); (2) the C89
exemption annotation's form and where examen reads annotations (it
does not today, T15b); (3) the rule's severity.

*T17 as split (2026-09-25).* **T17a** (done): built-in empty macro in
`silva_expandere.c` (`_def_internum_quaerere`), see
`silva/fontes/silva_expandere.worklog.md`. **T17b**: an examen code for
the extension (reads the invocation's name from the empty extent, so a
layout/flow attribute — packed, aligned, cleanup — can say that silva's
model is blind to it), a per-line `tolera` absorption for node-less
findings (the gate the portability family's comment already names),
`examen_vectis` section planted red (flagged / exempted with cause /
dead exemption → TOLERA_IRRITUM). Severity: VIOLATIO + suppressible
recommended (verify tolera accepts a VIOLATIO code) vs DOMESTICUM.

### Task 18: latina.h description line

silva's embedded copy (`silva_latina_datum.c`, generated by the
amalgamator) and `probatio_silva_contextus`'s byte pin move together.
Small; measure the pin sites first.

## Step 6 — cleanup

### Task 19: cleanup and close

- **True copies deleted** (T13a/T13b found them by collision):
  `_silvae_scriptura_fracta`, `_silvae_spatium_solum`,
  `_silvae_numerus_ad_literas`, `_silvae_elementum_proximum`,
  `HEX_CIFRAE_SILVAE` are materia's helpers under a second name —
  delete silva's by calling materia (export or share) where the call
  is natural; leave homonyms that are different functions. Measure
  with a clone census over the six substrate files first.
- LR toolkit's home decided (glr/generare/coquere).
- `lexicon_c89`'s generator (`silva/instrumenta/lexicon_c89_generare.sh`)
  folded into silva's table generator, or kept — moved here from T14.
- `examen_vectis.sh -corpus`: exclusions stale (stb_image.h, bitsyfont,
  briar fabrica fixtures with `#line`) — re-pin with causes or register
  the mode as a gate; today nothing runs it (found T17b).
- MAP.txt, `rhubarb.census`, MEMORY updated; lapifex landmine (MG4)
  re-checked.
- Park …FE9E and decree …XX0BZ closed with pointers.

### Task 19b: located errors in legati and briar

Step 5 named three printers; examen and tools/diagnostica print the
GLR death point with excerpts, legati and briar do not. legati copies
examen's format today (desideratum …W87Q: needs a public silva.h
surface for `silva_mortes_diagnostica` or its records); briar prints
only the first error line (…SQ9 — clang becomes the second opinion).
Measure first: what surface both need, and whether one printer serves
all four.

## Ledger items this plan carries (decide per item: in a task, or not)

| item | what | proposal |
|---|---|---|
| 01M3BHHJQA | nodus query families onto `MateriaOrigoUncus` | after T15 (extents consumers see) |
| 01M3D25TYW…GNNR | per-genus comment decorations + css diagnostica | when css wants `tolera` |
| …XHAW3H | half-amalgam guard (silva.h newer than silva.c) | T19, or earlier if silva.h moves |
| …SFJ3T | renominare renames file-local typedefs across files | before the next type rename |
| (none) | S41 sees duplicate static FUNCTIONS only; data/typedef collisions surface as compile errors (loud) | note only — loud is acceptable |
| (none) | `aedilis.stml` covered by no gate | T19 |
