# Silva → materia (phase 5), plan B — steps 5–6

> Inline on main, one task per turn with Fran's approval, no subagents.
> Re-issued 2026-09-25 at the step-4 boundary (T13 seal) from what
> steps 1–4 found — the sketches in `silva-migratio-plan.md` (T14–T19)
> were written before the migration and are superseded by this file.
> Spec: `project-specs/silva-migratio-spec.md` §4 step 5–6 (D5: the
> diagnostics payoff is INSIDE phase 5, as 5.x). Ledger: park …FE9E
> "phase 5" (decree …XX0BZ); one `opus` per task, closed by
> `silva.commissio(opus=ID)`. Narrative: `materia/phase-log.md`.

> **STATUS.** Nothing started. Plan A closed at T13 (b4abedbd +
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

As T16. Decide with Fran first whether the house accepts the extension
at all (C89 purity vs parsing third-party headers) — a design question,
not a fix.

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
- MAP.txt, `rhubarb.census`, MEMORY updated; lapifex landmine (MG4)
  re-checked.
- Park …FE9E and decree …XX0BZ closed with pointers.

## Ledger items this plan carries (decide per item: in a task, or not)

| item | what | proposal |
|---|---|---|
| 01M3BHHJQA | nodus query families onto `MateriaOrigoUncus` | after T15 (extents consumers see) |
| 01M3D25TYW…GNNR | per-genus comment decorations + css diagnostica | when css wants `tolera` |
| …XHAW3H | half-amalgam guard (silva.h newer than silva.c) | T19, or earlier if silva.h moves |
| …SFJ3T | renominare renames file-local typedefs across files | before the next type rename |
| (none) | S41 sees duplicate static FUNCTIONS only; data/typedef collisions surface as compile errors (loud) | note only — loud is acceptable |
| (none) | `aedilis.stml` covered by no gate | T19 |
