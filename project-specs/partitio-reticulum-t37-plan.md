# T37 — the instrument's second pass — Implementation Plan

> **For agentic workers:** execute INLINE (superpowers:executing-plans),
> one tranche per turn with Fran's approval between tranches (standing
> preference, memory `plan-execution-inline-checkpoints`). No subagents.
> Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** make the lattice's reports readable and honest at a glance —
a noise floor by fold agreement, the veto's reasons, examples behind a
group, a self-describing machine file — without touching the libraries.

**Architecture:** every change is in `oratio/fontes/oratio_reticulum.c`
(rendering and the end-of-chain probe), `oratio/instrumenta/reticulum.c`
(two flags), `oratio/census/reticulum_recensio.py` (the same rows), and
one appended LIS column in `oratio/fontes/oratio_oraculum.c`. Nothing
is reordered; columns and row kinds are appended.

**Spec:** `project-specs/oratio-spec.md` §7 "Design — T37, the
instrument's second pass". Order of work after T37: decretum
01M2HGPT52 (agreement constraint 01M2HGQ2HM, then the decoder).

## Status

| tranche | state |
|---|---|
| T37 a instrument + recount | pending |
| T37 b sentence id, examples, third run, records | pending |

---

## Global Constraints

As `partitio-reticulum-t36-plan.md` §Global Constraints, verbatim
(C89 house flags, Latin identifiers, latina.h macros forbidden, i32
unsigned, chorda not NUL-terminated, piscina only, no floats, machine
rows self-describing and append-only, decimal CLI flags, new words
checked with `./oratio/quaere.sh` BEFORE staging, formator on added
lines, `silva.commissio` with explicit `viae`, planted fault per gate,
docs first / gates last, select machine rows by FIELD, zsh `${=F}` and
never a command in a variable). Additional to T37:
- No table is reordered by the new counts (Fran 2026-09-14): the
  increment and the gain stay the rankings.
- A human-report law asserts a SUBSTRING; the machine rows are the
  byte-exact contract.

## File structure

| file | responsibility | tranche |
|---|---|---|
| `oratio/fontes/oratio_reticulum.c` | `sortes-lucrantes`, `lucrantes-gradu`, FINIS rows and `recusatae:`, condensed report, OPTIONES row, `-exempla` | a, b |
| `oratio/fontes/oratio_reticulum.h` | options `exempla` | b |
| `oratio/instrumenta/reticulum.c`, `oratio/reticulum.sh` | `-exempla N` | b |
| `oratio/probationes/probatio_oratio_reticulum.c`, `fixa/reticulum/exspectata*.tsv` | regenerated rows, new human laws | a, b |
| `oratio/census/reticulum_recensio.py` | FINIS, OPTIONES, the appended counts | a |
| `oratio/fontes/oratio_oraculum.h/.c`, `probatio_oratio_oraculum.c` | LIS column 33 `sententia` | b |
| worklogs, spec as-built, README, `oratio/CLAUDE.md`, this table | records | b |

---

### Task T37 a: instrument + recount

**Files:** `oratio_reticulum.c`, `probatio_oratio_reticulum.c`, both
expected files, `reticulum_recensio.py`, `oratio_reticulum.worklog.md`.

**Produces (exact):**

```c
TITULI_COLUMNAE:  … inaestimabilis greges-captivi ordines-captivi sortes-lucrantes
TITULI_INFIMI:    profunditas columnae greges recti incrementum cadentes-basi inaestimati sortes-lucrantes
TITULI_CATENAE:   vetans gradus columna greges recti lucrum cadentes-gradu lucrantes-gradu
TITULI_FINIS:     vetans columna greges recti lucrum cadentes causa      (RETICULUM-FINIS)
TITULI_OPTIONUM:  genus aurum sortes ubi columnae praeter gradus limen lucrum
                  profunditas prima initium greges alternae             (RETICULUM-OPTIONES)
```

Definitions:
- `sortes-lucrantes` (COLUMNA, INFIMUM) = folds with `recti > basis`;
  `lucrantes-gradu` (CATENA, from step 1) = folds with `recti >` the
  previous step's; step 0 prints 0.
- FINIS: after each chain (vetoed first), one row per column not in the
  chain, in feature order: `greges` and `recti` of the meet of the
  chain's last partition with that column, `lucrum` = its rows right
  minus the last step's (s32), `cadentes` = folds below the last step,
  `causa` = `cadit` when vetoed and a fold fell, `lucrum` when the gain
  is below the floor, `accepta` never (the chain would have taken it) —
  so with the pooled chain every row is `lucrum`. Human, after `finis:`:
  `  recusatae: a cadit X, b cadit X Y` for the rows with `causa cadit`
  and gain ≥ floor, then `  ceterae N lucro deficientes`; nothing when
  no column remains. `_finem_computare` already evaluates every
  candidate in a scratch region: keep per-candidate numbers in arrays
  allocated before the mark (`recti`, `lucrum`, `cadentes`, `cadit[]`
  per fold), the way the chain keeps its `recti[]`/`cadit[]`.
- OPTIONES: printed FIRST in machine mode, header then one row; empty
  lists `-`; `ubi` = `t=v1,v2;t2=v3`; `sortes` = `-` when interleaved;
  `alternae` = 0 when off. The recount prints the same from its parsed
  options (both sides normalise the same way; `-machina` and `-exempla`
  are not options of the run).
- CONDENSED REPORT: in the single-column block, a column with `lucrum ==
  0 && inaestimati == 0 && greges_captivi == 0 && greges_mutabiles == 0`
  and not constant is deferred to one line `  sine lucro: a b c` after
  the listed ones; every listed column with `lucrum != 0` gets `    sortes:
  A +4 B +4 C -2` (per-fold `recti − basis`, descending by value, ties in
  fold order) when `S <= VIII`, else `    sortes: N lucrantes, M
  cadentes, K pares`. Fold-name lists (`cadit …` in column and chain
  lines, the CONDICIONATI lines) print names when the falling folds are
  ≤ VIII, else `cadit N ex S`. Meet lists print rows with `incrementum >
  0` only, `  nulla` when none at that depth.

- [ ] **Step 1: Expected rows regenerated by hand** (cause: appended
  columns, FINIS and OPTIONES kinds). `exspectata.tsv` (proba): OPTIONES
  header + row `PROBA aurum-x thesaurus - - - - 2 1 2 20 - - 0`; COLUMNA
  `f … 0 0 2` (A and B gain, C falls: lucrantes 2), `h … 0 0 0`; CATENA
  vetoed `1 0 - 1 17 0 0 0`, pooled `0 0 - 1 17 0 0 0` and `0 1 f 2 23 6 1
  2`; FINIS after the vetoed chain: `1 f 2 23 6 1 cadit`, `1 h 1 17 0 0
  lucrum`; after the pooled chain: `0 h 2 23 0 0 lucrum` (the meet of
  `f` with constant `h` is `f`, 2 groups, 23 right, gain 0); the rest as
  today. `exspectata_paritas.tsv`: OPTIONES `PROBA aurum-x thesaurus - -
  - - 2 1 3 20 - - 0`; COLUMNA rows end `0 0 0`; INFIMUM rows end `… 0 0
  0` (depth 2) and `… 0 0 3` (the triple: every fold above base); CATENA
  step 0 rows end `0 0`; FINIS after each chain: `f 2 12 0 0 lucrum`, `g
  …`, `h …` for both chains.

- [ ] **Step 2: Failing laws** in the probatio: part I/V byte-exact
  against the regenerated files; part II human: `sortes: A +4 B +4 C -2`
  under `f`, `recusatae: f cadit C` and `ceterae 1 lucro deficientes`
  after the vetoed chain; part V human: `sine lucro: f g h`, `nulla`
  under depth 2, no `sortes:` line; part VI (seeded): `recusatae:` absent
  and `ceterae 2 lucro deficientes`. Run red.

- [ ] **Step 3: Implement**, then `-vitia`, examen, `quaere.sh` on new
  words (`lucrantes`, `pares`, `recusatae`, `ceterae`, `deficientes`,
  `optionum`, `finis` — all classical).

- [ ] **Step 4: Grow the recount** (FINIS from a last-step evaluation of
  every unused column, OPTIONES from its options, the three counts);
  recount == both expected files over `COLUMNA|SORS|CATENA|CONDICIO|
  INFIMUM|FINIS|OPTIONES|GREX|GREX-SORS`.

- [ ] **Step 5: Run green; planted fault** `>` → `>=` in the
  `sortes-lucrantes` count → RED on the paritas COLUMNA rows (3 for 0),
  GREEN after the revert.

- [ ] **Step 6: Worklog, format, commit** `oratio T37 a: LIMEN STREPITUS
  PER SORTES, FINIS CATENAE, RELATIO CONTRACTA, OPTIONES …`;
  `portae=[('oratio', 'reticulum')]`.

---

### Task T37 b: sentence id, examples, third run, records

**Files:** `oratio_oraculum.h/.c` (+ worklog), `probatio_oratio_oraculum.c`,
`oratio_reticulum.h/.c`, `instrumenta/reticulum.c`, `reticulum.sh`,
`probatio_oratio_reticulum.c`, `fixa/reticulum/proba.tsv`? (NO — the
examples read existing columns; no fixture change), records.

Definitions:
- LIS column 33 `sententia`: `OratioOraculumLis.sententia` (chorda) =
  the sentence's `sent_id` comment when `OratioConlluSententia` carries
  one, else the decimal ordinal of the sentence in the file (1-based),
  condensed into the census piscina. Title appended, formatter appends
  `_lis_campus_chordae`. Law: title XXXII is `sententia`; non-empty on
  every LIS row of every judged treebank.
- `-exempla N` (option `i32 exempla`, default 0): with `-greges`, under
  each group's human line, its first N rows (by row order) as `    linea
  L: t1=v1 t2=v2 t3=v3 <fold>=<v> [sententia=<v>]` where t1..t3 are the
  header's first three titles, the fold title the fold column's (or
  `sors=alterna-k` when interleaved), and `sententia` only when the
  header has that title. Rows come from `c->manentes` (kept rows) whose
  group is g: walk rows, count per group, stop at N. Human only.

- [ ] **Step 1: Failing laws.** Oracle: `ORATIO_COLUMNAE_LIS_NUMERUS`
  XXXIII, title XXXII `sententia`, every LIS row's field 35 non-empty
  (count == rows) on the pinned treebanks. Instrument: proba with
  `-greges f -exempla 2` contains `linea 3: f=u h=una thesaurus=A` and
  `linea 7: f=v h=una thesaurus=A` (rows 3 and 7 of proba.tsv are the
  first `u` and the first `v`); the seeded paritas `-greges catena
  -exempla 1` contains `linea 3: f=0 g=0 h=0 thesaurus=A`. Run red.

- [ ] **Step 2: Implement** (check how `OratioConlluSententia` stores
  `sent_id` — `grep -n sent_id oratio/fontes/oratio_conllu.h` — and fall
  back to the ordinal), green, plants: oracle = sentence id left empty
  → the non-empty law red; instrument = examples printed from the LAST
  rows (walk reversed) → `linea 3` absent → red; both green after revert.

- [ ] **Step 3: Third run** (rows regenerated, same flags as T36 d plus
  `-praeter …,sententia`): the seeded run with `-greges catena -exempla
  3`; read the flipping groups' examples and confirm two sentence ids
  exist in the CoNLL-U files (`grep -c "sent_id = <id>"`). Acceptance:
  recount = C on every kind (FINIS and OPTIONES included) for the main,
  seeded and interleaved runs.

- [ ] **Step 4: Records** — spec §7 "As built — T37", `oratio/census/
  README.md`, `oratio/CLAUDE.md`, both worklogs, this status table,
  memory; commit `portae=[('oratio', 'reticulum'), ('oratio',
  'oraculum')]` for the code, then the records with `portae=()`.

## Self-review

- **Spec coverage.** NOISE FLOOR → a; EXAMPLES + `sententia` → b;
  CONDENSED REPORT → a; TRANSPARENT VETO (FINIS) → a; OPTIONES → a;
  gates and acceptance as the spec names them; LEFT OUT untouched.
- **Placeholders.** None; the proba FINIS numbers are derived above
  (the meet of `f` with a constant is `f`).
- **Type consistency.** `lucrum` in FINIS is s32; counts i32; `causa` a
  literal title; `exempla` i32 on the options struct.
