# T36 — refinements of the partition lattice — Implementation Plan

> **For agentic workers:** execute INLINE (superpowers:executing-plans),
> one tranche per turn with Fran's approval between tranches (standing
> preference, memory `plan-execution-inline-checkpoints`). No subagents.
> Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** make the T35 instrument say which BUCKETS are real, not only
which columns: captive groups named, a profile per group, evidence rows
per group and corpus, a k-meet table to depth three, a seeded chain, and
the refused-but-pooled steps named as conditioned candidates.

**Architecture:** every change is an appended field, an appended machine
column or a new row kind; nothing is renamed except the two `cadentes`
titles, which split by meaning. `lib/partitio` (the algebra) is
untouched. `lib/partitio_aestimatio` gains captive detection, per-group
per-fold counts, the profile and an options struct for the chain. The
instrument library `oratio_reticulum` gains the k-meet table, the seed,
the conditioned rows, the group rows and interleaved folds. The oracle
appends one LIS column (`lemma-capitis`). The Python recount grows to
every row kind the C tool computes from the judgment.

**Tech stack:** C89 in Latin (`latina.h`), piscina, xar,
tabula_dispersa, chorda / chorda_aedificator, credo; Python 3 standard
library for the recount.

**Spec:** `project-specs/oratio-spec.md` — §2 decisions 54–56 and §7
"Design — T36, refinements of the partition lattice" (commit
`8e746456`). Read both before T36 a. The T35 plan
(`partitio-reticulum-plan.md`) records the as-built definitions the
new fields extend. Ledger: quaestiones 01M2HA2RAF (decided: universal
buckets, no tolerance), 01M2HB154D (decided: captive rule).

## Status

| tranche | state |
|---|---|
| T36 a the judgment | done — grid IV (captive flags at limen II, not IV), profiles on grids I–III and the seeded chain all green on the first run; the captive plant went red FIRST in grid I's one-group case (an inverted test makes every spread-out group captive), green after the revert; a training tie is APERTUS (spec text refined) |
| T36 b the search | done — k-meet table (`RETICULUM-INFIMUM` for `-PAR`), seed, CONDICIO, split titles, constants out of the lattice; parity fixture and regenerated grid II rows green on the first run; the sub-meet hash table and the tuples must be allocated BEFORE the scratch mark (worklog); plant = the subtraction zeroed |
| T36 c the evidence | done — GREX / GREX-SORS rows, `-sortes-alternae`, `lemma-capitis` (present on 100 % of contest rows), recount grown to every judged kind and equal to both fixtures; the plan's interleaved-fold count was off by one row (22 → 23) and the C tool and the recount agreed against it; plant = margin as the majority |
| T36 d second run, acceptance, records | pending |

---

## Global Constraints

- C89 only, the house flags from `tools/vexilla.sh` (`VEXILLA_C89`):
  `-pedantic -Wall -Wextra -Werror -Wconversion -Wsign-conversion
  -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -Wwrite-strings`.
  Examen at commit refuses `//`, declaration after statement, `_Bool`,
  variadic macros, flexible arrays.
- Latin identifiers, comments, messages. Every word `#define`d in
  `latina.h` is a forbidden identifier (`nomen`, `magnitudo`, `casus`,
  `registrum`, `duplex`, `per`, …); single capitals I V X L C D M are
  Roman numerals; numbers in code are numerals (`ZEPHYRUM`, `I`, `XX`,
  `M`); `latina.h` defines only SOME numerals (`DCLIII` absent) — a
  missing one is a decimal literal.
- `i8 i32 i64` are UNSIGNED, `s32 s64` signed, `b32` boolean. No −1 in
  an i32: a sentinel is a count that cannot be an index (`S` for "no
  fold", `A` for "no vote", `k` for "no column"). A difference that can
  be negative is `s32` (`lucrum`, `incrementum`).
- chorda is NOT NUL-terminated (`mensura`, `datum`); print with
  `%.*s`; never pass `datum` to a C string function.
- Memory from piscina only; no malloc. Every result that must survive
  is allocated BEFORE a `piscina_notare` scratch region; a
  `ChordaAedificator` never exists during computation (it would grow
  inside a region and lose its buffer on the refit).
- Integers only in the judgment (rows and permille); never a float.
  Permille = `(i32)((i64)x * (i64)M / (i64)n)`.
- Decisions 49–56 verbatim in intent. New ones: 54 no tolerance, a
  refused-but-pooled step is a CONDITIONED candidate with its falling
  corpora named; 55 a captive group of at least `limen` rows flags the
  column INAESTIMABILIS beside the half rule; 56 the profile COACTUS |
  ORDINATUS | APERTUS with mutability its own column.
- Machine rows: `via TAB KIND TAB fields`; header `via TAB COLUMNAE
  TAB KIND TAB titles` once per kind, printed even without rows;
  columns appended, never reordered. Numeric CLI flags are DECIMAL.
- New identifier WORDS need a glossary entry or a rename: check with
  `./oratio/quaere.sh <word>` BEFORE staging (`vocabula.sh -nova` sees
  only tracked files); the pre-commit lint blocks otherwise. Expected
  new words, all classical: `captivus`, `margo`, `habitus`, `semen`,
  `alternus`, `profunditas`, `condicio`, `coactus`, `ordinatus`,
  `apertus`, `totalis`, `secundus`.
- House format before commit: `./silva/formator.sh <viae> -scribere`
  then `-vitia` (fix only vitia on ADDED lines: intersect with
  `git diff -U0`).
- Commit only through `silva.commissio(msg, viae, portae)` with an
  explicit path list; `FAQ.md`, `gesta/annales/tabula.md`,
  `gesta/annales/tabularium.jsonl` are Fran's and never in `viae`; the
  untracked `project-specs/exempla/kalendarium.html` and
  `salutatio2.html` belong to other sessions. Pattern:
  `scratchpad/commissio_t35d.py` (git status must equal `viae` apart
  from those, else ABORT).
- Every gate is red at birth by a planted fault
  (`silva.planta(via, vetus, novus, porta_nomen, filtrum)`), reverted.
- Worklog entries (dated, English) for tricky finds; write docs FIRST,
  run gates LAST, then commit (a green receipt on an unchanged tree is
  reused by commissio).
- Select machine rows by FIELD, never by grep of the kind name alone
  (`\tLIS\t` matches the header row too): `awk -F'\t' '$2=="KIND"'`.
- zsh: quote `===` separators; `${=F}` splits a flag string.

## File structure

| file | responsibility | tranche |
|---|---|---|
| `include/partitio_aestimatio.h`, `lib/partitio_aestimatio.c` | captive groups, per-group per-fold counts, margin, profile, `PartitioCatenaOptiones`, seed | a |
| `probationes/probatio_partitio_aestimatio.c` | grid IV, profiles on I–III, seeded chain | a |
| `lib/partitio.worklog.md` | append | a |
| `oratio/fontes/oratio_reticulum.h/.c` | options, k-meet table, seed, CONDICIO, split titles, constants, 64-bit permille, GREX rows, interleaved folds | b, c |
| `oratio/instrumenta/reticulum.c`, `oratio/reticulum.sh` | new flags | b, c |
| `oratio/probationes/probatio_oratio_reticulum.c` | regenerated expected rows, parity fixture, seed, refusals, GREX, alternae | b, c |
| `oratio/probationes/fixa/reticulum/exspectata.tsv` | regenerated once (cause named) | b |
| `oratio/probationes/fixa/reticulum/paritas.tsv`, `exspectata_paritas.tsv` | new: pure triple interaction | b |
| `oratio/fontes/oratio_oraculum.h/.c`, `oratio/probationes/probatio_oratio_oraculum.c` | `lemma-capitis` appended to LIS | c |
| `oratio/census/reticulum_recensio.py` | grows to CONDICIO, INFIMUM, GREX, GREX-SORS, seed, alternae | c |
| `oratio/fontes/oratio_reticulum.worklog.md`, `oratio/fontes/oratio_oraculum.worklog.md` | append | b, c |
| spec §7 as-built T36, `oratio/census/README.md`, `oratio/CLAUDE.md` | records | d |

---

### Task T36 a: the judgment (`lib/partitio_aestimatio`)

**Files:**
- Modify: `include/partitio_aestimatio.h`, `lib/partitio_aestimatio.c`,
  `probationes/probatio_partitio_aestimatio.c`, `lib/partitio.worklog.md`
- Callers to update in the same tranche (the signature changes):
  `oratio/fontes/oratio_reticulum.c` (`_computare`, `_finem_computare`,
  `_greges_scribere`) — the instrument gate must stay green.

**Interfaces:**
- Consumes: `partitio.h` unchanged.
- Produces (exact; T36 b and c read these):

```c
nomen enumeratio {
    PARTITIO_HABITUS_COACTUS,     /* sors quaeque unanimis */
    PARTITIO_HABITUS_ORDINATUS,   /* sors quaeque suffragatur margine > 0, idem */
    PARTITIO_HABITUS_APERTUS      /* aliter: sine suffragio, par, discors */
} PartitioHabitus;
#define PARTITIO_HABITUS_NUMERUS III
externus constans character* constans
    PARTITIO_TITULI_HABITUUM[PARTITIO_HABITUS_NUMERUS];   /* coactus ordinatus apertus */

nomen structura {
    /* … campi T35 b immutati … */
    i32* captivus;         /* [g]: sors quae ordines omnes gregis tenet; S = nulla */
    i32  greges_captivi;
    i32  ordines_captivi;
    i32* retenti;          /* [g*S+s]: ordines sortis s in grege g */
    i32* recti_gregum;     /* [g*S+s]: eorum recte responsi */
    i32* margo;            /* [g*S+s]: maximum disciplinae minus secundum */
    i32* habitus;          /* [g]: PartitioHabitus */
    b32* mutabilis;        /* [g]: suffragia inter sortes diversa */
} PartitioAestimatio;

nomen structura {
              i32  limen;           /* XX */
              i32  lucrum_minimum;  /* 0 = limen */
              b32  sortes_vetant;   /* VERUM */
    constans Partitio* semen;       /* gradus 0; NIHIL = una */
} PartitioCatenaOptiones;

vacuum partitio_catena_optiones_initium (PartitioCatenaOptiones* o);

nomen structura {
    constans Partitio*   semen;           /* gradus 0 (una si nullum) */
    PartitioAestimatio*  initium;         /* aestimatio seminis */
    /* … numerus, columnae, partes, aestimationes ut T35 b … */
} PartitioCatena;

PartitioCatena* partitio_catenam_struere (Piscina* piscina,
    constans Partitio* constans* columnae, i32 k,
    constans Partitio* aurum, constans Partitio* sortes,
    constans PartitioCatenaOptiones* optiones);
```

Definitions (G groups, A gold groups, S folds; `totales[g]` = rows of
group g over all folds; `in_sorte(g, s)` = its rows in fold s;
`disciplina(g, s)` = `totales[g] − in_sorte(g, s)`):
- `captivus[g]` = the s with `in_sorte(g, s) == totales[g] > 0`, else
  S. `greges_captivi` counts groups with `captivus < S`,
  `ordines_captivi` their rows. `inaestimabilis` = `2 * inaestimati >
  ordines` OR some captive g with `totales[g] >= limen` (decision 55).
- `retenti[g*S+s]` = `in_sorte(g, s)`; `recti_gregum[g*S+s]` = rows of
  fold s in g answered right (by the vote, or by the base when g has no
  vote there — the same rows that enter `sortes[s].recti`).
- `margo[g*S+s]` = the largest training count of g in fold s minus the
  second largest (0 when they tie, 0 when training is empty).
- `habitus[g]`: COACTUS when in EVERY fold g votes and `margo ==
  disciplina(g, s)` (unanimous); ORDINATUS when in every fold g votes
  with `margo > 0` and every vote is the same gold; APERTUS otherwise
  (a fold without a vote, a tie voted to the base, or votes that
  differ). Refinement of the spec text: a TIE is open, not ordered — the
  tie vote is the base's, not the group's. Record in the as-built.
- `mutabilis[g]` = two voting folds with different votes (the count
  `greges_mutabiles` is its sum, unchanged).
- Chain: as T35 b with `cur = semen != NIHIL ? semen : partitio_una`,
  `catena->semen = cur`; a column already inside the seed meets to the
  same partition, gains 0 and the gain floor refuses it (the instrument
  always passes `lucrum_minimum >= I`).

- [ ] **Step 1: Write the failing assertions** in
  `probationes/probatio_partitio_aestimatio.c` (numbers derived here;
  copy the derivations into the file as comments):

  **Grid IV — the captive group** (11 rows, `limen` II). Column
  `x`/`y`, gold O/S, folds A B C (first appearance: x = 0, y = 1; O = 0,
  S = 1):

  | rows | fold | column | gold |
  |---|---|---|---|
  | 0–2 | A | x x x | O O S |
  | 3–4 | A | y y | S S |
  | 5–7 | B | y y y | S S O |
  | 8–10 | C | y y y | S O S |

  ```c
  constans i32 columnae_notae[XI] = { 0,0,0,1,1, 1,1,1, 1,1,1 };
  constans i32 auri_notae[XI]     = { 0,0,1,1,1, 1,1,0, 1,0,1 };
  constans i32 sortium_notae[XI]  = { 0,0,0,0,0, 1,1,1, 2,2,2 };
  ```
  (write the digits as `ZEPHYRUM`/`I`/`II`.)
  - held out A: training gold S4 O2 → base S; `basis` 3 (x S, y S,
    y S). x has 0 training rows → no vote, `inaestimati` += 3, answers
    S → 1 right; y training S4 O2 (margo 2) → S → 2 right. `recti` 3.
  - held out B: training O3 S5 → base S; `basis` 2. x training O2 S1
    (margo 1) → votes O, no held-out rows; y training S4 O1 (margo 3)
    → S → 2 right of S S O. `recti` 2.
  - held out C: the same shape → `basis` 2, `recti` 2, y margo 3.
  - totals: `ordines` 11, `recti` 7, `basis` 7, `lucrum` 0,
    `inaestimati` 3, `puritas_intra` (2 + 6) · 1000 / 11 = 727,
    `greges_mutabiles` 0; `captivus` {0, III}, `greges_captivi` 1,
    `ordines_captivi` 3; **`inaestimabilis` VERUM** (captive x has 3 ≥
    2 although 2·3 < 11 — the half rule alone would say FALSUM).
  - `habitus` x APERTUS (no vote in A), y ORDINATUS; `mutabilis` 0 0.
  - `retenti` x {3,0,0} y {2,3,3}; `recti_gregum` x {1,0,0} y {2,2,2};
    `margo` x {0,1,1} y {2,3,3}.
  - **`limen` IV**: x never votes; `inaestimati` still 3; captive
    counts unchanged; `inaestimabilis` FALSUM (3 < 4 and 6 < 11) — the
    threshold in the captive rule is real.

  **Profiles on the existing grids** (add to their blocks):
  - Grid I, `limen` II: `numerus` pl COACTUS, sg APERTUS (training tie
    2:2); `forma` versus COACTUS, prosa APERTUS; the meet's four groups
    COACTUS; `una` ORDINATUS (S6 O2, margo 4). `limen` III on the meet:
    all four APERTUS. Fold partition as candidate: every group APERTUS,
    `captivus` {0,1,2}, `greges_captivi` 3, `ordines_captivi` 12,
    `inaestimabilis` VERUM (already).
  - Grid II: `f` u ORDINATUS (margo 2, 2, 8 — unanimous in C only),
    v COACTUS (margo 9, 9, 10 = training); `h` ORDINATUS; `mutabilis`
    0 0; captives 0.
  - Grid III: the one group APERTUS, `mutabilis[0]` VERUM; `margo`
    {2, 2, 0}.

  **Seeded chain on grid I** (`limen` II, `lucrum_minimum` I, veto
  VERUM, `semen` = `columna_numeri`): `catena->semen` equals
  `columna_numeri`, `initium->summa.recti` 9; step 1 accepts `formae`
  (index I; `numeri` meets to itself, gain 0): `numerus` 1,
  `columnae[0]` I, `aestimationes[0]->summa.recti` 12, `partes[0]`
  equals the meet. Without a seed the same call still takes ZERO steps
  (unchanged assertion).

  Every existing call becomes `partitio_catenam_struere(piscina,
  columnae, II, aurum, sortes, &o)` with `o` from
  `partitio_catena_optiones_initium` then `o.limen = II;
  o.lucrum_minimum = I; o.sortes_vetant = …`.

- [ ] **Step 2: Run red.** `./compile_tests.sh partitio_aestimatio` →
  compile failure naming `PartitioCatenaOptiones` (exit 1).

- [ ] **Step 3: Implement.** In `partitio_aestimare`: allocate the new
  arrays with `_allocare_nullum` (sizes `G`, `G*S`); `totales[g]` in
  phase I; in phase II's group loop track `secundum` beside `maxima`
  (`secundum = maxima` before `maxima` moves; a value equal to `maxima`
  raises `paria` and sets `secundum = maxima`), write `retenti`,
  `margo = maxima − secundum`, and `recti_gregum` from the same
  `numeri_sortium[_locus(g, responsum, s)]` that feeds `sors->recti`;
  `captivus[g] = s` when `in_sorte == totales[g] && in_sorte > 0`
  (initialise every `captivus[g] = S`). Phase III becomes per group:
  votes, `mutabilis[g]`, `habitus[g]` (`disciplina = totales[g] −
  retenti[g*S+s]`). Phase IV adds the captive counts and the OR into
  `inaestimabilis`. `PARTITIO_TITULI_HABITUUM` defined in the `.c`.
  The chain: replace the three parameters, `semen`, and refuse
  `optiones == NIHIL` or a `semen` with a different row count. Update
  the instrument's three call sites (`c->una` → `catena->semen`).

- [ ] **Step 4: Run green.** `./compile_tests.sh partitio` (both
  probationes) and `./oratio/compile_probationes.sh reticulum`
  (unchanged fixture still equal — the instrument's numbers do not
  move in T36 a).

- [ ] **Step 5: Planted fault.** `silva.planta('lib/partitio_aestimatio.c',
  '<the captive test: in_sorte == totales[g]>', '<in_sorte != totales[g]>',
  'radix', 'partitio_aestimatio')` → RED in grid IV (`greges_captivi`,
  `inaestimabilis`), then GREEN after the revert. Record in the worklog.

- [ ] **Step 6: Worklog, words, format.** `lib/partitio.worklog.md`
  entry "T36 a": the tie-is-open refinement, the captive rule, the
  options struct; `./oratio/quaere.sh captivus margo habitus semen
  coactus ordinatus apertus totalis secundus`; formator on the four
  files.

- [ ] **Step 7: Commit** `partitio T36 a: GREGES CAPTIVI, HABITUS,
  OPTIONES CATENAE …`; viae the header, library, probatio, worklog and
  `oratio/fontes/oratio_reticulum.c`; `portae=[('radix', 'partitio'),
  ('oratio', 'reticulum')]`.

---

### Task T36 b: the search (instrument)

**Files:**
- Modify: `oratio/fontes/oratio_reticulum.h/.c`,
  `oratio/instrumenta/reticulum.c`, `oratio/reticulum.sh` (usage
  comment), `oratio/probationes/probatio_oratio_reticulum.c`,
  `oratio/probationes/fixa/reticulum/exspectata.tsv` (regenerated),
  `oratio/fontes/oratio_reticulum.worklog.md`
- Create: `oratio/probationes/fixa/reticulum/paritas.tsv`,
  `exspectata_paritas.tsv`

**Interfaces:**
- Consumes: T36 a exactly.
- Produces: options fields `i32 profunditas` (II), `i32 prima` (XX),
  `Xar* initium` (chorda titles), `-greges` accepting `catena-libera`
  and `initium`; row kinds and titles:

```c
TITULI_COLUMNAE:   columna greges ordines recti basis lucrum puritas
                   puritas-intra inaestimati mutabiles cadentes-basi
                   inaestimabilis greges-captivi ordines-captivi
TITULI_CATENAE:    vetans gradus columna greges recti lucrum cadentes-gradu
TITULI_CONDICIONIS: gradus columna sors recti priores        (RETICULUM-CONDICIO)
TITULI_INFIMI:     profunditas columnae greges recti incrementum
                   cadentes-basi inaestimati                 (RETICULUM-INFIMUM)
```
`RETICULUM-PAR` is removed (`TITULI_PARIS` deleted). Machine row order:
COLUMNA, SORS, CATENA, CONDICIO, INFIMUM, TEGIT, DUPLEX.

Definitions:
- CONSTANT column: a feature with one group after `-ubi`. Judged and
  printed as a COLUMNA row; excluded from the lattice (built over the
  non-constant features only, so no TEGIT or DUPLEX row names it), from
  the k-meet table and from the seed; the human report prints one line
  `constantes: a b` after the single columns (nothing when none).
- k-MEET table: every subset of size 2..`profunditas` of the
  non-constant features, enumerated in feature order (lexicographic
  tuples), each meet folded left in a scratch region (`infimum(infimum
  (p[a], p[b]), p[c])`), judged, and recorded BEFORE the refit as
  `{profunditas, tuple, greges, recti, cadentes_basi, inaestimati}` in
  an xar plus a `tabula_dispersa` keyed by the tuple's bytes (i32 ×
  depth) → index. `incrementum` = `recti` − the best `recti` among its
  (k−1)-subsets (depth 1 = the single columns). `-profunditas` above IV
  is refused (`reticulum: profunditas maior quam IV: N`). Printed per
  depth, the first `prima` by `incrementum` descending, ties in
  enumeration order (insertion sort as today).
- SEED: `-initium t1,t2` → titles judged like `-columnae` (unknown,
  `aurum-`, constant, not among the features → refused by name);
  `semen` = their meet, kept on the context with its column indices;
  CATENA step 0 prints `columna` = the titles joined by `+` (or `-`
  without a seed) and `greges` = the seed's groups.
- CONDICIO: for each step i of the POOLED chain and each fold s with
  `recti < prior recti`: `gradus` i+1, `columna`, `sors`, `recti` (this
  step's fold recti), `priores` (the previous step's). Human block
  `CANDIDATI CONDICIONATI` after the chains: `  numerus-capitis (gradus
  1): la_llct 76 -> 64`, or `  nulli`.
- `_puritas` in i64. `cadentes-basi` (COLUMNA, INFIMUM) = folds with
  `recti < basis`; `cadentes-gradu` (CATENA) = folds below the previous
  step. `-greges catena-libera` reads the pooled chain, `-greges
  initium` the seed (refused without `-initium`: `reticulum: greges
  initium sine -initium`). Usage line adds: `-ubi valores crudos
  confert (ante -gradus)`.

- [ ] **Step 1: The parity fixture** `fixa/reticulum/paritas.tsv` —
  gold = parity of three binary columns, so no single and no pair
  carries information and the triple is exact:

  ```
  # fixtura T36 b: paritas trium columnarum (interactio pura gradu III)
  fixa	COLUMNAE	PROBA	f	g	h	thesaurus	aurum-x
  fixa	PROBA	0	0	0	A	S
  fixa	PROBA	0	0	1	A	O
  fixa	PROBA	0	1	0	A	O
  fixa	PROBA	0	1	1	A	S
  fixa	PROBA	1	0	0	A	O
  fixa	PROBA	1	0	1	A	S
  fixa	PROBA	1	1	0	A	S
  fixa	PROBA	1	1	1	A	O
  ```
  then the same eight rows with `B`, then with `C` (24 rows). Derivation
  (`limen` II, `lucrum` I): gold first appearance S = 0, O = 1; every
  fold has S4 O4, training S8 O8 → base S (lowest index), `basis` 4 per
  fold, 12 total. Singles: each group S4 O4 in training → tie → base →
  `recti` 12, `lucrum` 0, `puritas` 500, `puritas-intra` 500. Pairs:
  each of the four groups has two patterns, one S one O → tie → 12,
  `incrementum` 0. Triple: eight groups, each one row per fold, training
  2 unanimous → 24 right, `incrementum` 24 − 12 = 12. Chains without a
  seed: no candidate gains → step 0 only, `catenae congruunt`. Seeded
  `-initium f,g`: step 0 `f+g` 4 groups 12; step 1 accepts `h` (+12, no
  fold falls); step 2 nothing gains. Lattice: three incomparable
  columns, altitudo 0, latitudo 3, no TEGIT, no DUPLEX, no constants.

  `exspectata_paritas.tsv` (run WITHOUT `-initium`; `-profunditas 3`):
  ```
  fixa	COLUMNAE	RETICULUM-COLUMNA	columna	greges	ordines	recti	basis	lucrum	puritas	puritas-intra	inaestimati	mutabiles	cadentes-basi	inaestimabilis	greges-captivi	ordines-captivi
  fixa	RETICULUM-COLUMNA	f	2	24	12	12	0	500	500	0	0	0	0	0	0
  fixa	RETICULUM-COLUMNA	g	2	24	12	12	0	500	500	0	0	0	0	0	0
  fixa	RETICULUM-COLUMNA	h	2	24	12	12	0	500	500	0	0	0	0	0	0
  fixa	COLUMNAE	RETICULUM-SORS	columna	sors	ordines	recti	basis	inaestimati
  fixa	RETICULUM-SORS	f	A	8	4	4	0
  (f B, f C, g A, g B, g C, h A, h B, h C: all 8 4 4 0)
  fixa	COLUMNAE	RETICULUM-CATENA	vetans	gradus	columna	greges	recti	lucrum	cadentes-gradu
  fixa	RETICULUM-CATENA	1	0	-	1	12	0	0
  fixa	RETICULUM-CATENA	0	0	-	1	12	0	0
  fixa	COLUMNAE	RETICULUM-CONDICIO	gradus	columna	sors	recti	priores
  fixa	COLUMNAE	RETICULUM-INFIMUM	profunditas	columnae	greges	recti	incrementum	cadentes-basi	inaestimati
  fixa	RETICULUM-INFIMUM	2	f+g	4	12	0	0	0
  fixa	RETICULUM-INFIMUM	2	f+h	4	12	0	0	0
  fixa	RETICULUM-INFIMUM	2	g+h	4	12	0	0	0
  fixa	RETICULUM-INFIMUM	3	f+g+h	8	24	12	0	0
  fixa	COLUMNAE	RETICULUM-TEGIT	subtilior	crassior
  fixa	COLUMNAE	RETICULUM-DUPLEX	columna	aequalis
  ```

- [ ] **Step 2: Regenerate `exspectata.tsv` by hand** for `proba.tsv`
  (grid II; `h` is CONSTANT now). Cause, recorded in the worklog: new
  kinds, split titles, constants leave the lattice. Content:
  ```
  fixa	COLUMNAE	RETICULUM-COLUMNA	… (the 14 titles above)
  fixa	RETICULUM-COLUMNA	f	2	26	23	17	6	884	884	0	0	1	0	0	0
  fixa	RETICULUM-COLUMNA	h	1	26	17	17	0	653	653	0	0	0	0	0	0
  fixa	COLUMNAE	RETICULUM-SORS	… (unchanged six rows)
  fixa	COLUMNAE	RETICULUM-CATENA	vetans	gradus	columna	greges	recti	lucrum	cadentes-gradu
  fixa	RETICULUM-CATENA	1	0	-	1	17	0	0
  fixa	RETICULUM-CATENA	0	0	-	1	17	0	0
  fixa	RETICULUM-CATENA	0	1	f	2	23	6	1
  fixa	COLUMNAE	RETICULUM-CONDICIO	gradus	columna	sors	recti	priores
  fixa	RETICULUM-CONDICIO	1	f	C	5	7
  fixa	COLUMNAE	RETICULUM-INFIMUM	profunditas	columnae	greges	recti	incrementum	cadentes-basi	inaestimati
  fixa	COLUMNAE	RETICULUM-TEGIT	subtilior	crassior
  fixa	COLUMNAE	RETICULUM-DUPLEX	columna	aequalis
  ```
  (no INFIMUM row: one non-constant column; no TEGIT row: `h` left the
  lattice.)

- [ ] **Step 3: Write the failing probatio additions.** Part I compares
  against the regenerated file (unchanged code). New part V — parity:
  `paritas.tsv` with `optiones.profunditas = III`, machine rows equal
  `exspectata_paritas.tsv` line by line; human report contains
  `catenae congruunt`, `altitudo 0, latitudo 3`, and the INFIMUM line
  `f x g x h: greges 8, recti 24, incrementum +12`. Part VI — seed:
  `_titulum_addere(optiones.initium, "f")`, `"g"`, machine rows contain
  `fixa\tRETICULUM-CATENA\t1\t0\tf+g\t4\t12\t0\t0` and
  `fixa\tRETICULUM-CATENA\t1\t1\th\t8\t24\t12\t0`; `-greges initium`
  human report contains `GREGES initium`. Part II adds: the human
  report on `proba.tsv` contains `constantes: h` and `CANDIDATI
  CONDICIONATI` with `f (gradus 1): C 7 -> 5`. Part III adds refusals:
  `profunditas` V → `reticulum: profunditas maior quam IV: 5`;
  `-initium zz` → `reticulum: titulus ignotus: zz`; `-initium aurum-x`
  → `reticulum: columna aurea ut initium: aurum-x`; `-initium h` on
  `proba.tsv` → `reticulum: columna constans ut initium: h`; `-greges
  initium` without `-initium` → `reticulum: greges initium sine
  -initium`.

- [ ] **Step 4: Run red.** `./oratio/compile_probationes.sh reticulum`
  → part I red (titles), parts V/VI red (unknown field / no rows).

- [ ] **Step 5: Implement** per the definitions: options and their
  judgment (order of refusals: after `-greges`, then `-initium` titles,
  then `profunditas`), constants (`b32* constans_columna` over the
  features; `partitio_reticulum_struere` over the compacted list with an
  index map for printing), the k-meet table (`InfimumReticuli {i32
  profunditas; i32* columnae; i32 greges; i32 recti; s32 incrementum;
  i32 cadentes; i32 inaestimati}`, recorded before `piscina_reficere`;
  the lookup table keyed by `chorda {datum = (i8*)tuple, mensura =
  depth * sizeof(i32)}` into an array allocated outside the region), the
  seed (`c->semen`, `c->semen_columnae`, `numerus_seminis`), CONDICIO
  from the pooled chain's per-fold `recti` (already in
  `aestimationes[i]->sortes[s].recti`), the CLI flags (`-profunditas`,
  `-prima` decimal; `-initium` a list). Delete `ParReticuli`/`paria`.

- [ ] **Step 6: Run green**, then by hand:
  `./oratio/reticulum.sh oratio/probationes/fixa/reticulum/paritas.tsv
  -genus PROBA -aurum aurum-x -sortes thesaurus -limen 2 -lucrum 1
  -profunditas 3 -initium f,g -greges initium` → the seeded chain and
  the eight seed groups on screen.

- [ ] **Step 7: Planted fault.** `silva.planta('oratio/fontes/
  oratio_reticulum.c', '<the increment: recti − optimum of the
  sub-meets>', '<recti alone, the subtraction dropped>', 'oratio',
  'reticulum')` → RED on every INFIMUM row of the parity fixture (`2
  f+g 4 12 12 …` where 0 is expected, `3 f+g+h 8 24 24` where 12 is),
  GREEN after the revert. (A plant on the max of the sub-meets would
  stay green: in the parity grid every sub-meet has the same rows
  right — the plant must break what the fixture can see.)

- [ ] **Step 8: Worklog, words, format, commit** `oratio T36 b:
  TABULA INFIMORUM, SEMEN, CONDICIO …`; viae the instrument files, the
  fixtures, the probatio, the wrapper, the worklog; `portae=[('oratio',
  'reticulum')]`.

---

### Task T36 c: the evidence

**Files:**
- Modify: `oratio/fontes/oratio_reticulum.h/.c`,
  `oratio/instrumenta/reticulum.c`, `oratio/reticulum.sh`,
  `oratio/probationes/probatio_oratio_reticulum.c`,
  `oratio/fontes/oratio_oraculum.h/.c`,
  `oratio/probationes/probatio_oratio_oraculum.c`,
  `oratio/census/reticulum_recensio.py`, both worklogs

**Interfaces:**
- Produces:

```c
TITULI_GREGIS:        partitio grex ordines aurum-maximum aurum-maximum-ordines
                      puritas captivus habitus mutabilis sortes-suffragantes
                      retenti recti                              (RETICULUM-GREX)
TITULI_GREGIS_SORTIS: partitio grex sors disciplina suffragium margo
                      retenti recti                              (RETICULUM-GREX-SORS)
```
options `i32 alternae` (0 = off); `-sortes-alternae N`; LIS column 32
`lemma-capitis`.

Definitions:
- GREX rows for the partition `-greges` names: `partitio` = the title,
  `catena`, `catena-libera` or `initium`; `grex` = the values at the
  group's representative row of the partition's columns (a column: its
  value; a chain: seed columns then steps, joined by `+`; the seed: its
  columns); `ordines` = `magnitudines[g]`; `aurum-maximum` = the gold
  title with the most rows in g (tie → lowest gold index),
  `aurum-maximum-ordines` its count, `puritas` = that count · 1000 /
  ordines; `captivus` = the fold title or `-`; `habitus` title;
  `mutabilis` 0|1; `sortes-suffragantes` = folds in which g voted;
  `retenti` = Σ_s `retenti[g*S+s]` (= `ordines`), `recti` = Σ_s
  `recti_gregum`. Groups in first-appearance order. GREX-SORS: one row
  per group and fold in fold order: `disciplina` = `totales[g] −
  retenti`, `suffragium` = the gold title or `-`, `margo`, `retenti`,
  `recti`. Both headers printed always; rows only with `-greges`. The
  human `-greges` view prints `habitus`, `margo` per fold and `captivus`.
- INTERLEAVED folds: `-sortes-alternae N` (N ≥ II, else refused
  `reticulum: sortes alternae pauciores quam II: N`; with `-sortes`
  refused `reticulum: ambae -sortes et -sortes-alternae`); the fold of
  the r-th KEPT row is `r mod N`; fold titles `alterna-0` … built as
  chordas in the piscina; no fold column exists, so no feature is
  excluded on its account; `index_sortium` stays −1 and
  `_sortis_titulus` reads the synthetic titles.
- `lemma-capitis`: `OratioOraculumLis.lemma_capitis` (chorda) = the
  LEMMA token of the head element's FIRST reading (`h->nodus`,
  analyses[0], `loci[ORATIO_ANALYSIS_LEMMA].datum.token->valor` when the
  value is a token, else empty), condensed into the census piscina with
  `_chordam_condere`; `ORATIO_COLUMNAE_LIS_NUMERUS` XXXII, title
  `"lemma-capitis"` appended, `_lis_campus_chordae(a, lis->lemma_capitis)`
  appended in `oratio_oraculum_lis_linea`. The first 31 fields stay
  byte-identical.

- [ ] **Step 1: Write the failing assertions.**
  `probatio_oratio_reticulum.c` part VII — GREX on `proba.tsv` with
  `-greges f`: rows
  `fixa\tRETICULUM-GREX\tf\tu\t12\tO\t9\t750\t-\tordinatus\t0\t3\t12\t9`,
  `fixa\tRETICULUM-GREX\tf\tv\t14\tS\t14\t1000\t-\tcoactus\t0\t3\t14\t14`,
  and GREX-SORS `f u A 8 O 2 4 4`, `f u B 8 O 2 4 4`, `f u C 8 O 8 4 1`,
  `f v A 9 S 9 5 5`, `f v B 9 S 9 5 5`, `f v C 10 S 10 4 4` (derivation:
  held out A, u's training = B's four O + C's S S S O → O5 S3, margo
  2; held out C, u's training = eight O, margo 8, held-out u S S S O
  answered O → 1 right). On `paritas.tsv` with `-initium f,g -greges
  catena`: eight GREX rows `catena\t0+0+0\t3\tS\t3\t1000\t-\tcoactus\t0
  \t3\t3\t3` … (gold S O O S O S S O in order) and 24 GREX-SORS rows
  `disciplina 2, suffragium S|O, margo 2, retenti 1, recti 1`. Part
  VIII — interleaved: `proba.tsv` with `optiones.alternae = II`
  (`sortes = NIHIL`): COLUMNA `f\t2\t26\t23\t17\t6\t884\t884\t0\t0\t0
  \t0\t0\t0`, `h\t1\t26\t17\t17\t0\t653\t653\t…`; SORS `f alterna-0 13
  11 9 0`, `f alterna-1 13 12 8 0`, `h alterna-0 13 9 9 0`, `h alterna-1
  13 8 8 0`; CATENA vetans `1 1 f 2 23 6 0` (the veto that fell on C by
  corpus does NOT fall when the folds interleave — decision 54's logic
  in miniature; derivation: even rows fold 0 = u O4 S2, v S7; odd rows
  fold 1 = u O5 S1, v S7 (rows 5 7 13 15 17 23 25); held out 0: base S
  basis 9, u votes O (O5 S1) → 4 right, v S → 7: 11; held out 1: basis
  8, u O (O4 S2) → 5, v → 7: 12. CORRECTED at execution: the first
  draft counted v's odd rows as six; the C tool and the recount agreed
  on 23 against the draft's 22, and the recount of the rows settled it.
  Note `thesaurus` becomes a FEATURE here (no fold column excludes it)
  and the chain takes it at step 2 — pass `-praeter thesaurus` on real
  data unless that is the question). Refusals: `alternae` I → `reticulum: sortes alternae pauciores
  quam II: 1`; both → `reticulum: ambae -sortes et -sortes-alternae`.
  `probatio_oratio_oraculum.c`: title XXXI equals `lemma-capitis`; on
  CIRCSE the LIS rows with a non-empty field 34 are more than half of
  the rows (the head is a resolved verb almost always).

- [ ] **Step 2: Run red** (`reticulum` and `oraculum` suites).

- [ ] **Step 3: Implement** the four pieces (GREX kinds, `-sortes-
  alternae`, `lemma-capitis`, `-greges` human additions). The oracle
  helper `_lemma_lectionis(vocabulum, ordinalis)` mirrors
  `_accidens_lectionis` but returns the LEMMA token's chorda (empty when
  absent or not a token).

- [ ] **Step 4: Grow the recount** `oratio/census/reticulum_recensio.py`:
  flags `-profunditas`, `-prima`, `-initium`, `-sortes-alternae`,
  `-greges`; constants excluded from INFIMUM; `catena()` takes a seed;
  emit CONDICIO, INFIMUM (subsets by `itertools.combinations`, increment
  against the best sub-tuple), GREX, GREX-SORS with the exact titles;
  the COLUMNA header gains the captive columns, CATENA's last title is
  `cadentes-gradu`. Check it against BOTH fixtures first: the recount's
  rows for `proba.tsv` and `paritas.tsv` equal the expected files (a
  probatio assertion is not needed — the T36 d acceptance is the gate;
  record the fixture check in the worklog).

- [ ] **Step 5: Run green**, by hand `./oratio/reticulum.sh …
  paritas.tsv … -initium f,g -greges catena -machina | awk -F'\t'
  '$2=="RETICULUM-GREX"'` → eight rows.

- [ ] **Step 6: Planted fault.** `silva.planta('lib/partitio_aestimatio.c',
  '<margo = maxima − secundum>', '<margo = maxima>', 'oratio',
  'reticulum')` → RED on the GREX-SORS rows (`margo` 5 for u in A) and
  on the profiles (u becomes COACTUS), GREEN after the revert.

- [ ] **Step 7: Worklogs, words, format, commit** `oratio T36 c: ORDINES
  TESTIMONII GREGUM, SORTES ALTERNAE, LEMMA CAPITIS …`; `portae=
  [('oratio', 'reticulum'), ('oratio', 'oraculum')]`.

---

### Task T36 d: second run, acceptance, records

**Files:**
- Modify: `project-specs/oratio-spec.md` (as-built after the T36
  design block), `oratio/census/README.md` (the two rows),
  `oratio/CLAUDE.md` (status paragraph), worklogs; this plan's status
  table; ledger 01M2HA2RAF and 01M2HB154D → clausum with a note;
  memory `oratio-project.md` bullet.

The predictions are in the spec, written before the run; they are
repeated here so the executor does not reread §7 mid-run:
(1) recount = C on every row kind; (2) at depth III the triple `ante ×
numerus-victae × numerus-capitis` is among the top INFIMUM rows and its
GREX-SORS rows show the flip (before, plural loser, singular verb:
victor from CIRCSE, victa from Perseus; mutable, APERTUS); (3a) with
`-sortes-alternae 6` the vetoed chain ACCEPTS `numerus-capitis` — if
not, decision 54's reading is wrong and stops here; (3b) with `-sortes
lemma-capitis` `numerus-capitis` still gains ≥ +50 pooled; (4) the arc
contests (gold `aurum-arcus`) as a second census, reported.

- [ ] **Step 1: Regenerate the rows** (the lemma column is new):
  `S=<scratchpad>; ./oratio/census/metire.sh $S/lites_t36.tsv -lites`;
  `awk -F'\t' '$2=="COLUMNAE" && $3=="LIS"' $S/lites_t36.tsv | head -1
  | awk -F'\t' '{print NF-3, $NF}'` → `32 lemma-capitis`.

- [ ] **Step 2: The runs** (zsh; `F` = the T35 e flags, verbatim from
  `partitio-reticulum-plan.md` T35 e step 3, plus `-praeter
  lemma-capitis`):
  ```
  ./oratio/reticulum.sh $S/lites_t36.tsv ${=F} -profunditas 3 > $S/r36.txt
  ./oratio/reticulum.sh $S/lites_t36.tsv ${=F} -profunditas 3 -machina > $S/r36.tsv
  python3 oratio/census/reticulum_recensio.py $S/lites_t36.tsv ${=F} -profunditas 3 > $S/rec36.tsv
  ./oratio/reticulum.sh $S/lites_t36.tsv ${=F} -initium ante,numerus-victae -greges catena > $S/r36_semen.txt
  ./oratio/reticulum.sh $S/lites_t36.tsv ${=F} -initium ante,numerus-victae -greges catena -machina > $S/r36_semen.tsv
  python3 oratio/census/reticulum_recensio.py $S/lites_t36.tsv ${=F} -initium ante,numerus-victae -greges catena > $S/rec36_semen.tsv
  A="${F/-sortes thesaurus/}"
  ./oratio/reticulum.sh $S/lites_t36.tsv ${=A} -sortes-alternae 6 -praeter thesaurus > $S/r36_alternae.txt
  L="${F/-sortes thesaurus/-sortes lemma-capitis}"
  ./oratio/reticulum.sh $S/lites_t36.tsv ${=L} -praeter thesaurus > $S/r36_lemma.txt
  ./oratio/reticulum.sh $S/lites_t36.tsv -genus LIS -aurum aurum-arcus -sortes thesaurus -ubi aurum-arcus=victor,victa <the -gradus and -praeter of F> > $S/r36_arcus.txt
  ```
  (`-praeter lemma-capitis` in `F` so thousands of lemma groups do not
  enter the feature set; `-praeter thesaurus` when the corpus is not
  the fold, else it becomes a feature — which is itself interesting,
  run it once both ways and note it.)

- [ ] **Step 3: Acceptance 1.** `diff <(awk -F'\t' '$2 ~ /^RETICULUM-/'
  $S/r36.tsv) <(awk -F'\t' '$2 ~ /^RETICULUM-/' $S/rec36.tsv)` → empty
  (TEGIT/DUPLEX included: the recount does not emit them — so restrict
  both sides to `COLUMNA|SORS|CATENA|CONDICIO|INFIMUM`); the same for
  the seeded pair of files including GREX and GREX-SORS.

- [ ] **Step 4: Acceptance 2–4.** Read the reports; record each
  prediction as MEASURED, whichever way it fell. (3a) failing stops the
  tranche for a conversation with Fran before any records claim
  decision 54 holds.

- [ ] **Step 5: Records** (docs first, then commit): spec §7 "As built —
  T36" (numbers, the tie-is-open refinement, anything measurement
  corrected); census README rows updated (flags, kinds); `oratio/
  CLAUDE.md`; worklogs; this plan's status table; ledger notes and
  closures; memory bullet.

- [ ] **Step 6: Commit** `oratio T36 d: CURSUS SECUNDUS …` (viae the
  docs and the plan; `portae=()`).

---

## Self-review

- **Spec coverage.** §7 JUDGMENT → T36 a (captives, per-group counts,
  margin, habitus, options, seed); SEARCH → T36 b (k-meet, INFIMUM
  supersedes PAR, seed flag, CONDICIO, split titles, constants,
  `-greges catena-libera|initium`, 64-bit permille, usage note);
  EVIDENCE → T36 c (GREX, GREX-SORS, `-sortes-alternae`,
  `lemma-capitis`, the recount); GATES → the plants named per tranche;
  ACCEPTANCE → T36 d; LEFT OUT untouched. One refinement of the spec
  text, stated in T36 a: a training TIE makes a group APERTUS, not
  ORDINATUS (the tie vote is the base's, not the group's) — record in
  the as-built.
- **Placeholders.** None: every expected row is derived in this file;
  plant anchors are named by the expression they replace and copied at
  plant time. T36 b step 7 names the plant actually used.
- **Type consistency.** `captivus`, `habitus` are `i32*` arrays with
  the count sentinel; `margo` i32 (never negative); `incrementum`,
  `lucrum` s32; `PartitioCatenaOptiones` passed as `constans *`; the
  seed is `constans Partitio*` on both options and catena; machine-row
  titles identical in the fixtures, the probatio and the recount.
