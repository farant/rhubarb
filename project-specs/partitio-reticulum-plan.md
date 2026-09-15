# T35 — the partition lattice — Implementation Plan

> **For agentic workers:** execute INLINE (superpowers:executing-plans),
> one tranche per turn with Fran's approval between tranches (standing
> preference, memory `plan-execution-inline-checkpoints`). No subagents.
> Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** an instrument that says, before any decoder weight exists,
which census columns earn a bucket — judged held out, leave one corpus
out, with a veto when any corpus falls.

**Architecture:** everything is a partition of the same rows (candidate
column, gold answer, corpus fold). `lib/partitio` is the exact algebra
ported from knotapel's `scrutinium.h`; `lib/partitio_aestimatio` is the
graded judgment (training counts = totals minus the held-out fold) and
the greedy chain. The oracle prints self-describing `COLUMNAE` header
rows for its LIS contest rows plus eight appended columns; a thin oratio
instrument (`oratio_reticulum` library + `reticulum.c` main) reads them
by title and calls the library.

**Tech stack:** C89 in Latin (`latina.h`), piscina, xar,
tabula_dispersa, chorda / chorda_aedificator, filum, credo; one Python
recount script in `oratio/census/`.

**Spec:** `project-specs/oratio-spec.md` — §2 "The partition lattice"
(decisions 49–53) and §7 "Design — T35, the partition lattice" (commit
`d6c50c65`). Read both before T35 a. Ledger: decretum 01M2H2Q8JA,
desideratum 01M24V1387.

## Status

| tranche | state |
|---|---|
| T35 a the algebra | done — gate `probatio_partitio` (10,623 random pairs, Dilworth = brute force), plant red at the first hand case; `duplex` (a latina.h macro) renamed `geminus` |
| T35 b the judgment | done — gate `probatio_partitio_aestimatio` (three grids, every hand number on the first run); the leak plant went red FIRST in grid I's `limen III` case (not grid III as predicted), grid II immune; numerals `DCLIII`/`DCCCLXXXIV` absent from latina.h |
| T35 c the oracle columns | pending |
| T35 d the instrument | pending |
| T35 e first run, acceptance, records | pending |

---

## Global Constraints

- C89 only, the house flags from `tools/vexilla.sh` (`VEXILLA_C89`):
  `-pedantic -Wall -Wextra -Werror -Wconversion -Wsign-conversion
  -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -Wwrite-strings`.
  Examen at commit refuses `//`, declaration after statement, `_Bool`,
  variadic macros, flexible arrays.
- Latin identifiers, comments, messages. Every word `#define`d in
  `latina.h` is a forbidden identifier (`nomen`, `magnitudo`, `casus`,
  `registrum`, `per`, …); single capitals I V X L C D M are Roman
  numerals; numbers in code are numerals (`ZEPHYRUM`, `I`, `XX`, `M`).
- `i8 i32 i64` are UNSIGNED, `s32 s64` signed, `b32` boolean. No −1 in
  an i32: use an explicit sentinel (a count that cannot be an index).
  A difference that can be negative is `s32` (`lucrum`).
- chorda is NOT NUL-terminated (`mensura`, `datum`); print with
  `%.*s`; never pass `datum` to a C string function.
- Memory from piscina only; no malloc. A result that must survive is
  allocated before a `piscina_notare` scratch region.
- Integers only in the judgment (rows and permille); never a float.
- Decisions 49–53 verbatim in intent: held-out gain (leave one corpus
  out, training-only majorities, `limen` before a vote); no fold falls
  (chain veto, pooled chain printed beside); machine rows describe
  themselves (`via TAB COLUMNAE TAB KIND TAB titles`, columns appended,
  never reordered, reader refuses by name); gold is named (`aurum-`
  prefix, never a feature); unjudgeable is said aloud (INAESTIMABILIS).
- New identifier WORDS need a glossary entry or a rename: run
  `./oratio/vocabula.sh -nova` before every commit (the pre-commit hook
  blocks on it anyway). `recensio` is unknown to WORDS — it names only
  the Python script, which the lint does not read.
- House format before commit: `./silva/formator.sh <viae> -scribere`
  then `-vitia` (the pre-commit formats whole files).
- Commit only through `silva.commissio(msg, viae, portae)` with an
  explicit path list; `FAQ.md`, `gesta/annales/tabula.md`,
  `gesta/annales/tabularium.jsonl` are Fran's and never in `viae`.
  Pattern: `scratchpad/commissio_spec_t35.py` (git status must equal
  `viae` apart from those three, else ABORT).
- Every gate is red at birth by a planted fault
  (`silva.planta(via, vetus, novus, porta_nomen, filtrum)`), reverted.
- Worklog entries (dated, English) for tricky finds; write docs FIRST,
  run gates LAST, then commit (a green receipt on an unchanged tree is
  reused by commissio).

## File structure

| file | responsibility | tranche |
|---|---|---|
| `include/partitio.h`, `lib/partitio.c` | `Partitio`, canonical numbering, refines / equal / meet / join, `PartitioReticulum` | a |
| `probationes/probatio_partitio.c` | hand cases, random laws, Dilworth = brute force | a |
| `include/partitio_aestimatio.h`, `lib/partitio_aestimatio.c` | held-out judgment, greedy chain | b |
| `probationes/probatio_partitio_aestimatio.c` | three hand-computed grids | b |
| `lib/partitio.worklog.md` | worklog for both library files | a, b |
| `oratio/fontes/oratio_oraculum.h/.c` | judgment fields on LIS, column table, row + header formatter, `thesaurus` | c |
| `oratio/instrumenta/oraculum.c` | prints header + rows through the library formatter | c |
| `oratio/probationes/probatio_oratio_oraculum.c` | header, append, cross-count laws | c |
| `oratio/fontes/oratio_oraculum.worklog.md` | new worklog | c |
| `oratio/fontes/oratio_reticulum.h/.c` | TSV with headers → partitions → report text + machine rows | d |
| `oratio/instrumenta/reticulum.c`, `oratio/reticulum.sh` | main + wrapper (pattern `sententiae.c`, `verba.sh`) | d |
| `oratio/probationes/probatio_oratio_reticulum.c` | fixture in, machine rows compared, refusals | d |
| `oratio/probationes/fixa/reticulum/proba.tsv`, `exspectata.tsv` | fixture and expected machine rows | d |
| `oratio/compile_probationes.sh` | `partitio`, `partitio_aestimatio` in `RADIX_FONTES` | d |
| `oratio/fontes/oratio_reticulum.worklog.md` | new worklog | d |
| `oratio/census/reticulum_recensio.py` | independent recount | e |
| spec §7 as-built, `oratio/census/README.md`, `oratio/CLAUDE.md` | records | e |

---

### Task T35 a: the algebra (`lib/partitio`)

**Files:**
- Create: `include/partitio.h`, `lib/partitio.c`,
  `probationes/probatio_partitio.c`, `lib/partitio.worklog.md`
- Modify (generated): `compile_tests_fontes_generata.sh` via
  `./tools/compile_tests_fontes_generare.sh`

**Interfaces:**
- Consumes: `piscina.h`, `chorda.h`, `tabula_dispersa.h`.
- Produces (exact, used by T35 b and d):

```c
nomen structura {
    i32  numerus;          /* ordines */
    i32  numerus_gregum;
    i32* grex;             /* grex[i]: greges ordine primae apparitionis */
    i32* magnitudines;     /* ordines per gregem */
    i32* primus;           /* primus[g]: ordo primus gregis g */
} Partitio;

Partitio* partitio_ex_notis   (Piscina* piscina, i32 numerus, constans i32* notae);
Partitio* partitio_ex_chordis (Piscina* piscina, i32 numerus, constans chorda* valores);
Partitio* partitio_una        (Piscina* piscina, i32 numerus);
Partitio* partitio_discreta   (Piscina* piscina, i32 numerus);
b32       partitio_subtilior  (constans Partitio* a, constans Partitio* b);
b32       partitio_aequalis   (constans Partitio* a, constans Partitio* b);
Partitio* partitio_infimum    (Piscina* piscina, constans Partitio* a, constans Partitio* b);
Partitio* partitio_supremum   (Piscina* piscina, constans Partitio* a, constans Partitio* b);

nomen structura {
    i32  numerus;          /* k */
    b32* ordo;             /* ordo[i*k+j]: i subtilior j (i != j; diagonalis FALSUM) */
    b32* tegit;            /* tegit[i*k+j]: j tegit i immediate (unicae solae, stricte) */
    i32* geminus;           /* index aequalis primi; sibi ipsi si unica */
    i32  altitudo;         /* catena longissima stricta inter unicas (arcus) */
    i32  latitudo;         /* antichain maxima inter unicas (Dilworth) */
} PartitioReticulum;

PartitioReticulum* partitio_reticulum_struere (Piscina* piscina,
    constans Partitio* constans* partes, i32 k);
```

Refusals: NIHIL input or unequal `numerus` → constructors / meet / join
return NIHIL, `subtilior` / `aequalis` return FALSUM. `numerus` 0 is a
valid empty partition (0 groups; arrays of length 1 allocated).

- [ ] **Step 1: Write the failing probatio** `probationes/probatio_partitio.c`
  (credo pattern of `probatio_piscina.c`: `piscina_generare_dynamicum`,
  `credo_aperire`, blocks `imprimere("\n--- Probans … ---\n")`,
  `credo_imprimere_compendium`, exit by `credo_omnia_praeterierunt`).
  Hand cases, every expectation exact:

  1. `partitio_ex_notis` over `{7, 3, 7, 9}` → `numerus_gregum` 3,
     `grex {0,1,0,2}`, `magnitudines {2,1,1}`, `primus {0,1,3}`.
  2. `partitio_ex_chordis` over `"nom" "acc" "nom" "gen"` (chordae via
     `chorda_ex_literis`) → `grex {0,1,0,2}`.
  3. Family over n = 4: `P0 = discreta`, `P1 = {0,0,1,1}`,
     `P2 = {0,1,0,1}`, `P3 = {0,0,0,1}`, `P4 = una`, `P5 = {5,5,9,9}`.
     - `aequalis(P1,P5)` VERUM; `subtilior(P1,P5)` and `(P5,P1)` VERUM;
       `subtilior(P1,P2)`, `(P2,P1)`, `(P1,P3)`, `(P3,P1)` FALSUM;
       `subtilior(P0,Px)` VERUM for all; `subtilior(Px,P4)` VERUM for all.
     - `infimum(P1,P2)` aequalis `P0`; `infimum(P1,P3)` has `grex {0,0,1,2}`;
       `supremum(P1,P2)` and `supremum(P1,P3)` aequalis `P4`.
     - `partitio_reticulum_struere` over `{P0..P5}` (k = 6):
       `geminus {0,1,2,3,4,1}`; `tegit` VERUM exactly at (0,1) (0,2) (0,3)
       (1,4) (2,4) (3,4) — six cells, none on row 5 or column 5, not (0,4);
       `altitudo` 2; `latitudo` 3.
  4. Unequal rows: `subtilior(P1, partitio_una(p, III))` FALSUM;
     `infimum(p, P1, partitio_una(p, III))` NIHIL.
  5. Laws over seeded random partitions (LCG `x = x * 1103515245 + 12345`
     in i32, seed `XLII`; n = VII rows; labels `x % (1 + x % IV)`), CC
     families of k = II..XII:
     - every pair: `infimum(a,b)` subtilior a and b; a and b subtilior
       `supremum(a,b)`; `infimum(a,a)` aequalis a; `subtilior(a,a)`;
       `subtilior(a,b) && subtilior(b,a)` ⇒ `aequalis(a,b)`;
     - every triple: `subtilior(a,b) && subtilior(b,c)` ⇒ `subtilior(a,c)`;
     - `latitudo` equals BRUTE FORCE: over every bitmask of unique
       indices (`geminus[i] == i`), the largest set with no `ordo` in
       either direction between members (scrutinium's method, k ≤ 12).
     - `ordo` agrees with `partitio_subtilior` cell by cell.

- [ ] **Step 2: Register and run red.**
  `./tools/compile_tests_fontes_generare.sh` then
  `./compile_tests.sh partitio`. Expected: link/compile failure naming
  `partitio_ex_notis` (header absent) — exit 1, not 2.

- [ ] **Step 3: Implement `include/partitio.h` and `lib/partitio.c`.**
  The whole library rests on ONE canonicalizer; every constructor and
  the meet go through it:

```c
interior Partitio*
_partitio_creare (
    Piscina* piscina,
         i32 numerus)
{
    Partitio*      p;
    memoriae_index longitudo;

    longitudo = (memoriae_index)(numerus > ZEPHYRUM ? numerus : I)
        * magnitudo(i32);
    p = (Partitio*)piscina_allocare(piscina, magnitudo(Partitio));
    si (p == NIHIL)
    {
        redde NIHIL;
    }
    p->numerus        = numerus;
    p->numerus_gregum = ZEPHYRUM;
    p->grex           = (i32*)piscina_allocare(piscina, longitudo);
    p->magnitudines   = (i32*)piscina_allocare(piscina, longitudo);
    p->primus         = (i32*)piscina_allocare(piscina, longitudo);
    si (p->grex == NIHIL || p->magnitudines == NIHIL || p->primus == NIHIL)
    {
        redde NIHIL;
    }
    memset(p->magnitudines, ZEPHYRUM, longitudo);
    redde p;
}

/* claves[i] = clavis ordinis i (octeti); grex = ordo primae apparitionis */
interior Partitio*
_partitio_ex_clavibus (
            Piscina* piscina,
                 i32 numerus,
    constans chorda* claves)
{
    Partitio*       p;
    TabulaDispersa* tabula;
    i32*            identitates;
    i32             i;

    p = _partitio_creare(piscina, numerus);
    si (p == NIHIL)
    {
        redde NIHIL;
    }
    tabula = tabula_dispersa_creare_chorda(piscina,
        numerus > XVI ? numerus * II : XXXII);
    identitates = (i32*)piscina_allocare(piscina,
        (memoriae_index)(numerus > ZEPHYRUM ? numerus : I) * magnitudo(i32));
    si (tabula == NIHIL || identitates == NIHIL)
    {
        redde NIHIL;
    }
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        vacuum* valor;

        si (tabula_dispersa_invenire(tabula, claves[i], &valor))
        {
            p->grex[i] = *(i32*)valor;
        }
        alioquin
        {
            identitates[p->numerus_gregum] = p->numerus_gregum;
            (vacuum)tabula_dispersa_inserere(tabula, claves[i],
                &identitates[p->numerus_gregum]);
            p->primus[p->numerus_gregum] = i;
            p->grex[i]                   = p->numerus_gregum;
            p->numerus_gregum            = p->numerus_gregum + I;
        }
        p->magnitudines[p->grex[i]] = p->magnitudines[p->grex[i]] + I;
    }
    redde p;
}
```

  - `partitio_ex_notis`: copy `notae` into a piscina i32 array (so the
    chorda `datum` points at mutable memory — no `-Wcast-qual`), build
    `claves[i] = { magnitudo(i32), (i8*)&copia[i] }`, canonicalize.
  - `partitio_ex_chordis`: canonicalize `valores` directly.
  - `partitio_una`: all zeros through `partitio_ex_notis`;
    `partitio_discreta`: labels `i`.
  - `partitio_subtilior` — O(n), no allocation, through the
    representative row:

```c
    per (i = ZEPHYRUM; i < a->numerus; i++)
    {
        si (b->grex[i] != b->grex[a->primus[a->grex[i]]])
        {
            redde FALSUM;
        }
    }
    redde VERUM;
```

  - `partitio_aequalis`: equal `numerus` and `numerus_gregum` and
    `memcmp` of `grex` (canonical numbering makes this exact).
  - `partitio_infimum`: i64 array `k64[i] = (i64)a->grex[i] *
    (i64)b->numerus_gregum + (i64)b->grex[i]`, claves of
    `magnitudo(i64)` bytes over `k64[i]`, canonicalize.
  - `partitio_supremum`: union-find over rows (`parens[i] = i`; path
    halving in `_radix`); for every row `_unire(i, a->primus[a->grex[i]])`
    and `_unire(i, b->primus[b->grex[i]])`; labels `_radix(i)` through
    `partitio_ex_notis` (first appearance renumbers the roots).
  - `partitio_reticulum_struere`:
    1. `ordo[i*k+j] = i != j && partitio_subtilior(partes[i], partes[j])`.
    2. `geminus[i]` = smallest j ≤ i with `aequalis` (j = i when unique).
    3. strict order among uniques: `stricte[i*k+j] = unica(i) && unica(j)
       && ordo[i*k+j] && !ordo[j*k+i]`.
    4. `tegit[i*k+j] = stricte[i*k+j]` and no unique m with
       `stricte[i*k+m] && stricte[m*k+j]`.
    5. `altitudo`: `h[] = 0`; repeat k times: for strict (i,j)
       `h[j] = max(h[j], h[i] + 1)`; `altitudo = max h`.
    6. `latitudo = unicae − maximum matching` over `stricte` (Dilworth:
       refinement is transitive, so a minimum chain cover equals the
       unique count minus a maximum matching). Kuhn's augmenting path:

```c
interior b32
_augere (
    constans b32* stricte,
              i32 k,
             i32* socius_dexter,   /* socius_dexter[j]: i, aut k si liber */
             b32* visus,
              i32 i)
{
    i32 j;

    per (j = ZEPHYRUM; j < k; j++)
    {
        si (!stricte[i * k + j] || visus[j])
        {
            perge;
        }
        visus[j] = VERUM;
        si (   socius_dexter[j] == k
            || _augere(stricte, k, socius_dexter, visus, socius_dexter[j]))
        {
            socius_dexter[j] = i;
            redde VERUM;
        }
    }
    redde FALSUM;
}
```

  For each unique i: clear `visus`, `si (_augere(...)) congruentia++`.

- [ ] **Step 4: Run green.** `./compile_tests.sh partitio` → the new
  probatio passes; exit 0. Then `./tools/compile_tests_fontes_generare.sh
  -probare` → exit 0 (list fresh).

- [ ] **Step 5: Planted fault, red then green.** In a scratch Python
  file: `silva.planta('lib/partitio.c', '<the line "p->grex[i] =
  *(i32*)valor;">', 'p->grex[i] = i;', 'radix', 'partitio')` — the
  canonical numbering broken; expect RED on hand case 1 and on
  `aequalis(P1,P5)`, then green after the automatic revert. Record both
  compendia in the worklog.

- [ ] **Step 6: Worklog + lint + format.** `lib/partitio.worklog.md`
  (2026-09-xx): source (scrutinium, what changed and why — O(n)
  refinement through the representative, Dilworth instead of brute
  force, tegit), the planted fault and its compendium.
  `./oratio/vocabula.sh -nova` (glossary lines for any new word, e.g.
  `augere`/`stricte` if unknown); `./silva/formator.sh include/partitio.h
  lib/partitio.c probationes/probatio_partitio.c -scribere`, then `-vitia`.

- [ ] **Step 7: Commit.** `silva.commissio(msg, ['include/partitio.h',
  'lib/partitio.c', 'probationes/probatio_partitio.c',
  'lib/partitio.worklog.md', 'compile_tests_fontes_generata.sh'
  (+ 'oratio/glossarium.stml' if touched)], portae=[('radix',
  'partitio')])`. Latin message: `partitio T35 a: algebra partitionum
  …` naming the source, the changes from scrutinium, the gate and the
  plant.

---

### Task T35 b: the judgment (`lib/partitio_aestimatio`)

**Files:**
- Create: `include/partitio_aestimatio.h`, `lib/partitio_aestimatio.c`,
  `probationes/probatio_partitio_aestimatio.c`
- Modify: `lib/partitio.worklog.md` (append), generated source list

**Interfaces:**
- Consumes: T35 a exactly as produced.
- Produces:

```c
nomen structura {
    i32 ordines;
    i32 recti;
    i32 basis;         /* recti responso basis solo */
    i32 inaestimati;   /* ordines quorum grex suffragium non tulit */
} AestimatioSortis;

nomen structura {
    i32               numerus_gregum;     /* candidatae */
    i32               numerus_aureorum;   /* greges auri */
    i32               numerus_sortium;
    AestimatioSortis* sortes;             /* numerus_sortium */
    AestimatioSortis  summa;
    s32               lucrum;             /* summa.recti - summa.basis */
    i32               puritas_intra;      /* permille, in specimine */
    i32               greges_mutabiles;
    b32               inaestimabilis;     /* 2 * inaestimati > ordines */
    i32*              suffragia;          /* [g*S+s]: grex auri; numerus_aureorum = nullum */
    i32*              numeri;             /* [g*A+a]: ordines gregis g auri a */
} PartitioAestimatio;

PartitioAestimatio* partitio_aestimare (Piscina* piscina,
    constans Partitio* p, constans Partitio* aurum,
    constans Partitio* sortes, i32 limen);

nomen structura {
    PartitioAestimatio*  initium;         /* partitio una */
    i32                  numerus;         /* gradus accepti */
    i32*                 columnae;        /* index columnae gradus cuiusque */
    Partitio**           partes;          /* infimum cumulatum */
    PartitioAestimatio** aestimationes;
} PartitioCatena;

PartitioCatena* partitio_catenam_struere (Piscina* piscina,
    constans Partitio* constans* columnae, i32 k,
    constans Partitio* aurum, constans Partitio* sortes,
    i32 limen, i32 lucrum_minimum, b32 sortes_vetant);
```

Definitions (G groups, A gold groups, S folds; `C[(g*A+a)*S+s]` rows
of group g, gold a, fold s; `T[g*A+a]` over folds; `TS[a*S+s]` over
groups):
- base `b_s` = argmax over a of `Σ_g T[g*A+a] − TS[a*S+s]`; a TIE goes
  to the lowest gold index.
- a group votes iff its training rows `Σ_a (T − C[..s])` ≥ `limen`, for
  the strict argmax of its training counts; a tie votes `b_s`.
- a row of fold s in group g is answered by the vote, or by `b_s` when
  g has no vote (and counts in `inaestimati`); `recti_s` counts answers
  equal to gold; `basis_s = TS[b_s*S+s]`; `ordines_s = Σ_a TS[a*S+s]`.
- `greges_mutabiles` = groups with two voting folds that voted
  differently. `puritas_intra = Σ_g max_a T[g*A+a] * M / n`.
- chain: `cur = partitio_una`, `initium = aestimare(cur)`. Each step,
  inside `piscina_notare` … `piscina_reficere`, evaluate every unused
  column `c` as `infimum(cur, columnae[c])`, keeping only `recti[c]`
  and `cadit[c]` (VERUM if any fold's `recti` is below the current
  step's) in arrays allocated BEFORE the scratch mark. Then in order
  of `recti` descending, ties by index: `gain = (s32)recti[c] −
  (s32)cur.recti`; if `gain < (s32)lucrum_minimum` stop the chain;
  if `sortes_vetant && cadit[c]` skip to the next; otherwise ACCEPT —
  recompute the meet and its aestimatio outside the scratch region,
  push, mark used, continue.

- [ ] **Step 1: Write the failing probatio**
  `probationes/probatio_partitio_aestimatio.c`. Three grids, every
  number derived in a comment above its assertions (copy these
  derivations into the file):

  **Grid I — interaction** (12 rows; folds A rows 0–3, B 4–7, C 8–11;
  each fold the same four rows):

  | row pattern | numerus | forma | aurum |
  |---|---|---|---|
  | 1st | pl (1) | versus (0) | S (0) |
  | 2nd | sg (0) | versus (0) | S (0) |
  | 3rd | sg (0) | prosa (1) | O (1) |
  | 4th | pl (1) | prosa (1) | S (0) |

  With `limen` II:
  - `una`: recti 9, basis 9, lucrum 0, each fold recti 3 basis 3,
    inaestimati 0, mutabiles 0, puritas_intra 750.
  - `numerus`: recti 9, basis 9, lucrum 0 (sg ties 2:2 in training →
    votes S), puritas_intra 750, mutabiles 0.
  - `forma`: identical numbers.
  - `infimum(numerus, forma)`: 4 groups, recti 12, basis 9, lucrum +3,
    each fold recti 4, puritas_intra 1000, inaestimati 0.
  - chain over `{numerus, forma}` with `lucrum_minimum` I, veto VERUM:
    `numerus` 0 — neither single column gains; the INTERACTION is
    invisible to the greedy chain (why the instrument prints pairs).
  With `limen` III: `infimum(numerus, forma)` → every group has 2
  training rows < 3 → recti 9, inaestimati 12, `inaestimabilis` VERUM;
  `numerus` still recti 9, inaestimati 0.
  The fold partition itself as candidate (`limen` II): recti 9,
  inaestimati 12, `inaestimabilis` VERUM (the held-out group never has
  training rows).

  **Grid II — the veto** (26 rows, in this order; gold numbering by
  first appearance makes O = 0, S = 1; f: u = 0, v = 1; h constant):
  fold A: u O O O O, v S S S S S; fold B: the same nine; fold C:
  u S S S O, v S S S S.
  With `limen` II:
  - f: fold A recti 9 basis 5; B 9 / 5; C 5 / 7 → recti 23, basis 17,
    lucrum +6; mutabiles 0; puritas_intra 884 (u 9 O of 12, v 14 S of
    14 → 23000 / 26); inaestimati 0.
  - h: recti 17, basis 17, lucrum 0; puritas_intra 653.
  - chain `{f, h}`, `lucrum_minimum` I: veto VERUM → `numerus` 0 (f
    gains +6 but fold C falls 7 → 5; h gains 0). Veto FALSUM →
    `numerus` 1, `columnae[0]` 0, `aestimationes[0]->summa.recti` 23
    (step 2 meets h → f again, gain 0, stops). `initium->summa.recti` 17.

  **Grid III — ties and mutability** (6 rows; one column constant;
  folds A rows 0–1, B 2–3, C 4–5; gold S S | O O | S O → S = 0, O = 1):
  - held out A: training O3 S1 → votes O; A recti 0, basis 0.
  - held out B: training S3 O1 → votes S; B recti 0, basis 0.
  - held out C: training S2 O2 → base TIE → S (lowest index); group
    TIE → votes b_C = S; C recti 1, basis 1.
  - totals recti 1, basis 1, lucrum 0, `greges_mutabiles` 1,
    `suffragia` `{1, 0, 0}` for the one group.
  Grid III is the LEAK DETECTOR: if training counts included the
  held-out fold, every fold would see S3 O3 → S, and recti would be 3.

- [ ] **Step 2: Register and run red.** Generator, then
  `./compile_tests.sh partitio_aestimatio` → compile failure naming
  `partitio_aestimare` (exit 1).

- [ ] **Step 3: Implement** `include/partitio_aestimatio.h` and
  `lib/partitio_aestimatio.c` exactly to the definitions above.
  Allocation sizes as `memoriae_index` products; `C`, `T`, `TS` from the
  caller's piscina (the chain is responsible for the scratch mark).
  `puritas_intra` as `(i32)((i64)summa_maximorum * (i64)M / (i64)n)`
  with `n > 0` guarded (0 rows → all zeros, `inaestimabilis` FALSUM).

- [ ] **Step 4: Run green.** `./compile_tests.sh partitio` → both
  partitio probationes pass; generator `-probare` exit 0.

- [ ] **Step 5: Planted fault — the circularity.** `silva.planta(
  'lib/partitio_aestimatio.c', <the training expression T − C>,
  <T alone>, 'radix', 'partitio_aestimatio')` → RED on grid III
  (recti 3), then green after revert. Record in the worklog.

- [ ] **Step 6: Worklog, lint, format** (as T35 a step 6); the worklog
  entry records the interaction grid finding (a greedy chain cannot see
  a pure interaction) and the leak plant.

- [ ] **Step 7: Commit** `partitio T35 b: iudicium partitionum retentum
  …` with viae the three new files, the worklog and the generated list;
  `portae=[('radix', 'partitio')]`.

---

### Task T35 c: the oracle columns

**Files:**
- Modify: `oratio/fontes/oratio_oraculum.h` (OratioOraculumLis at
  L118–142; declarations after `oratio_oraculum_contentiones` L459),
  `oratio/fontes/oratio_oraculum.c` (`_litem_notare` L2541;
  `_ligationes_iudicare` L2727 — `caput_nostrum` allocation L2767,
  initialisation L2781, first claim L2961–2969, LIS call site
  L3183–3198), `oratio/instrumenta/oraculum.c` (LIS loop L894–946),
  `oratio/probationes/probatio_oratio_oraculum.c` (resolved block,
  after the lites law L811–815; `principale` L1008)
- Create: `oratio/fontes/oratio_oraculum.worklog.md`

**Interfaces:**
- Produces (used by T35 d and e through the TSV only — titles are the
  contract):

```c
nomen enumeratio {
    ORATIO_IUDICIUM_VICTOR = ZEPHYRUM,
    ORATIO_IUDICIUM_VICTA,
    ORATIO_IUDICIUM_AMBO,
    ORATIO_IUDICIUM_NEUTRA,
    ORATIO_IUDICIUM_IDEM,
    ORATIO_IUDICIUM_IGNOTUM,
    ORATIO_IUDICIUM_NUMERUS
} OratioOraculumIudicium;

externus constans character* constans ORATIO_TITULI_IUDICIORUM[];
    /* "victor" "victa" "ambo" "neutra" "idem" "ignotum" */

#define ORATIO_COLUMNAE_LIS_NUMERUS XXXI
externus constans character* constans ORATIO_COLUMNAE_LIS[];

/* thesaurus: nomen plagulae ante "-ud-" (sine directorio; sine "-ud-":
 * ante extensionem) - sors census */
chorda oratio_oraculum_thesaurus (Piscina* piscina, constans character* via);
/* "via\tCOLUMNAE\tLIS\t<tituli>" sine linea nova */
chorda oratio_oraculum_lis_columnae (Piscina* piscina, constans character* via);
/* "via\tLIS\t<XXXI campi>" sine linea nova */
chorda oratio_oraculum_lis_linea (Piscina* piscina, constans character* via,
    constans OratioOraculumLis* lis);
```

Appended `OratioOraculumLis` fields (after `vox_capitis`, in this order):
`s32 iudicium_arcus; s32 iudicium_lectionis; s32 relatio_victae;
s32 relatio_victoris; b32 ante_victoris; i32 distantia_victae;
i32 distantia_victoris;`

`ORATIO_COLUMNAE_LIS`, in printed order — the first 23 are today's
fields and are PINNED:
`victor victa dependens caput ante aurum-casus casus-victae
casus-victoris aurum-deprel aurum-caput-idem caput-victoris-idem
clausula primum-clausulae nominativi nominativi-certi
nominativi-concordes accusativi-certi genus-victae numerus-capitis
persona-capitis vox-capitis numerus-victae numerus-victoris` then
`thesaurus aurum-arcus aurum-lectio relatio-victae relatio-victoris
ante-victoris distantia-victae distantia-victoris`.

Judgments (computed in `_litem_notare` from new parameters
`b32 recta, b32 prima_recta, s32 relatio_victae, s32 relatio_victoris`):
- `iudicium_arcus`: `caput_v < 0 || (i32)caput_v == caput_a` → IDEM;
  else `recta && prima_recta` → AMBO, `recta` → VICTA, `prima_recta` →
  VICTOR, else NEUTRA. (Same condition as `_contentionem_notare`:
  `victa_sola = recta && !victor_rectus` with `prima_recta == recta`
  whenever the head is the same or absent.)
- `iudicium_lectionis`: any of `casus_aureus`, `casus_victae`,
  `casus_victoris` < 0 → IGNOTUM; both equal gold → AMBO; loser only →
  VICTA; winner only → VICTOR; else NEUTRA.
- `relatio_victae`: the umbra's `ORATIO_UMBRA_RELATIO` index at the call
  site (−1 if not an INDEX); `relatio_victoris`: new array
  `s32* relatio_nostra` beside `caput_nostrum` (allocated, initialised
  −1, set at the first claim from that claim's umbra).
- `ante_victoris = caput_v >= 0 && dependens_a < (i32)caput_v`;
  `distantia_victae = |dependens_a − caput_a|`;
  `distantia_victoris = caput_v >= 0 ? |dependens_a − caput_v| : 0`.
- Printed: enum titles through a library copy of `_titulus_valoris`
  (`-` when out of range); b32 and distances as `%d`.

- [ ] **Step 1: Baseline before any change.** Build and keep today's LIS
  rows for one file:
  `./oratio/oraculum.sh oratio/probationes/fixa/ud/la_circse-ud-test.conllu
  -machina -lites | grep -P '\tLIS\t' > $S/lis_ante.tsv` (S = scratchpad).

- [ ] **Step 2: Write the failing laws** in `probatio_oratio_oraculum.c`:
  1. once, from `principale`: a new `_columnas_lis_probare()` —
     `ORATIO_COLUMNAE_LIS_NUMERUS` is XXXI; titles pairwise distinct
     (`strcmp`); the first XXIII equal a pinned literal array
     `TITULI_LIS_PINNATI[]` in the probatio (the append law);
     `oratio_oraculum_thesaurus` gives `la_llct` for
     `la_llct-ud-dev.conllu`, `la_ittb` for
     `oratio/build/ud/la_ittb-ud-test.conllu`, `x` for `x.tsv`.
  2. per treebank, in the resolved block right after the lites law
     (L815): with `census.lites` non-empty, count tabs (an interior
     `_tabulae(chorda)`): `_tabulae(oratio_oraculum_lis_columnae(p,
     plagula)) == _tabulae(oratio_oraculum_lis_linea(p, plagula, prima))
     + I` and `_tabulae(linea) == ORATIO_COLUMNAE_LIS_NUMERUS + I`.
  3. CROSS-COUNT laws in the same block: over `census.lites` count
     `iudicium_arcus == VICTA`, `== VICTOR`, `iudicium_lectionis ==
     VICTA`, `== VICTOR`, `!= IGNOTUM`; over `contentiones` sum
     `victa_sola`, `victor_solus`, `victa_casu_sola`,
     `victor_casu_solus`, `casus_iudicati`; five `CREDO_AEQUALIS_I32`.
  Run `./oratio/compile_probationes.sh oraculum` → compile failure
  (fields and functions absent), exit 1.

- [ ] **Step 3: Implement** the enum, title arrays, appended fields,
  `_litem_notare` parameters and judgments, `relatio_nostra`, the three
  library functions (`ChordaAedificator`: `chorda_aedificator_creare`,
  `_appendere_literis`, `_appendere_chorda`, `_appendere_character`,
  `_appendere_s32`, `_appendere_i32`, `chorda_aedificator_finire`),
  and in `instrumenta/oraculum.c` replace the LIS printf loop: print
  `oratio_oraculum_lis_columnae(piscina, titulus)` once when `lites` and
  the list is non-empty, then each `oratio_oraculum_lis_linea(...)`
  with `imprimere("%.*s\n", …)`.

- [ ] **Step 4: Run green.** `./oratio/compile_probationes.sh oraculum`
  → passes. Then prove the old columns untouched:
  `./oratio/oraculum.sh … la_circse-ud-test.conllu -machina -lites |
  grep -P '\tLIS\t' | cut -f1-25 > $S/lis_post.tsv` and
  `cmp $S/lis_ante.tsv $S/lis_post.tsv` → identical; and
  `./oratio/oraculum.sh … -machina -lites | grep -c -P '\tCOLUMNAE\tLIS\t'`
  → 1. `python3 oratio/census/lites.py` on the new rows still runs
  (positions unchanged).

- [ ] **Step 5: Planted fault.** `silva.planta('oratio/fontes/oratio_oraculum.c',
  <the VICTA branch of iudicium_arcus>, <VICTOR>, 'oratio', 'oraculum')`
  → RED on the cross-count law, green after revert.

- [ ] **Step 6: Worklog** `oratio/fontes/oratio_oraculum.worklog.md`
  (new): why the formatter moved into the library (the header law needs
  a row in-process), the judgment definitions and their equivalence with
  `_contentionem_notare`, the byte check of the first 25 fields, the
  plant. Lint, format.

- [ ] **Step 7: Commit** `oratio T35 c: ordines LIS se describunt …`
  (viae: the four modified files + the new worklog); `portae=['oratio']`.

---

### Task T35 d: the instrument

**Files:**
- Create: `oratio/fontes/oratio_reticulum.h`, `oratio/fontes/oratio_reticulum.c`,
  `oratio/instrumenta/reticulum.c`, `oratio/reticulum.sh`,
  `oratio/probationes/probatio_oratio_reticulum.c`,
  `oratio/probationes/fixa/reticulum/proba.tsv`,
  `oratio/probationes/fixa/reticulum/exspectata.tsv`,
  `oratio/fontes/oratio_reticulum.worklog.md`
- Modify: `oratio/compile_probationes.sh` (`RADIX_FONTES` gains
  `"partitio"` and `"partitio_aestimatio"`)

**Interfaces:**
- Consumes: T35 a/b APIs; `filum_legere_totum`; `chorda_fissio`.
- Produces:

```c
nomen structura {
    chorda  titulus;
    chorda* valores;
    i32     numerus;
} OratioReticulumUbi;

nomen structura {
    constans character* genus;       /* -genus */
    constans character* aurum;       /* -aurum */
    constans character* sortes;      /* -sortes */
    Xar*                ubi;         /* OratioReticulumUbi */
    Xar*                columnae;    /* chorda; vacuus = omnes */
    Xar*                praeter;     /* chorda */
    Xar*                gradus;      /* chorda */
    i32                 limen;       /* ordinarius XX */
    i32                 lucrum;      /* ordinarius = limen */
    constans character* greges;      /* NIHIL, titulus columnae, aut "catena" */
    b32                 machina;
} OratioReticulumOptiones;

nomen structura {
    b32    iudicatum;    /* FALSUM = nihil iudicatum (exitus II) */
    chorda causa;        /* si !iudicatum */
    chorda relatio;      /* textus humanus aut ordines machinae */
} OratioReticulumExitus;

OratioReticulumExitus oratio_reticulum_currere (Piscina* piscina,
    chorda tsv, constans character* via, constans OratioReticulumOptiones* optiones);
```

Reading (lines split on `\n`, fields on `\t`, line numbers 1-based):
- `f[1] == "COLUMNAE" && f[2] == genus` → titles `f[3..]`; a second
  header of the kind must be identical, else refuse.
- `f[1] == genus` → a header must precede, else refuse; field count
  must be `2 + titles`, else refuse; other kinds ignored.
- `-ubi t=v1,v2` keeps rows whose column t equals one of the values;
  repeated `-ubi` must all hold.
- `-gradus t`: `"0"` → `0`, `"1"` → `1`, any other all-digit value →
  `plures`; a non-digit value refuses.
- features = `-columnae` if given, else every title; minus `-praeter`;
  minus the `-sortes` column; any `aurum-` title among explicit features
  refuses; `aurum-` titles are dropped from the default set.

Refusal causes (`causa`, printed to stderr by the main, exit 2), exact:
- `reticulum: genus sine ordine COLUMNAE: <K>`
- `reticulum: ordo ante COLUMNAE: linea <n>`
- `reticulum: ordo COLUMNAE discrepans: linea <n>`
- `reticulum: latitudo ordinis falsa: linea <n> (<m> campi, <e> exspectati)`
- `reticulum: titulus ignotus: <t>`
- `reticulum: columna auri praefixum aurum- non fert: <t>`
- `reticulum: columna aurea ut nota: <t>`
- `reticulum: gradus in valore non numerico: <t>, linea <n>`
- `reticulum: sortes pauciores quam II: <s>`
- `reticulum: nulli ordines post -ubi`

Machine rows (`-machina`), each kind preceded by its own header
`<via>\tCOLUMNAE\t<KIND>\t…`:
- `RETICULUM-COLUMNA`: `columna greges ordines recti basis lucrum
  puritas puritas-intra inaestimati mutabiles cadentes inaestimabilis`
  (one per feature, gain descending, ties by title order; `puritas` =
  `recti * M / ordines`; `cadentes` = folds with recti < basis).
- `RETICULUM-SORS`: `columna sors ordines recti basis inaestimati`
  (same column order; folds in the fold partition's group order, named
  by the fold column's value at the representative row).
- `RETICULUM-CATENA`: `vetans gradus columna greges recti lucrum
  cadentes` — first `vetans 1` (row `gradus 0`, `columna -`, the
  `initium`), then `vetans 0`; `lucrum` = recti minus the previous
  step's recti and `cadentes` = folds whose recti fell against the
  previous step (the `gradus 0` row carries 0 and 0).
- `RETICULUM-PAR`: `columna-a columna-b greges recti incrementum
  cadentes` (`incrementum` = pair recti − the better single's; top XX
  by incrementum; `cadentes` against base).
- `RETICULUM-TEGIT`: `subtilior crassior`; `RETICULUM-DUPLEX`:
  `columna aequalis`.

Human output (no `-machina`): header line (rows, gold split, folds,
base permille); single columns table (INAESTIMABILIS listed by name at
the end without numbers); duplicates and covering edges with altitudo /
latitudo; both chains, each ending with the refused best candidate and
its reason (`lucrum <g> < <min>` or `cadit <fold>`), and the first step
where the chains diverge; top XX pairs; `-greges` per group (values,
rows, gold split, vote per fold, `!` when mutable).

- [ ] **Step 1: Fixture.** `fixa/reticulum/proba.tsv` = grid II of
  T35 b as a TSV (kind `PROBA`, via `fixa`), header
  `fixa\tCOLUMNAE\tPROBA\tf\th\tthesaurus\taurum-x`, rows e.g.
  `fixa\tPROBA\tu\tuna\tA\tO` in grid II's order, plus a comment line
  at the top (`# fixtura T35 d: grid II probationis partitio_aestimatio`)
  and one row of another kind (`fixa\tALIUD\t1`) that must be ignored.
  `fixa/reticulum/exspectata.tsv` = the machine rows for
  `-genus PROBA -aurum aurum-x -sortes thesaurus -limen II -lucrum I
  -machina`, from T35 b's derivations:

```
fixa	COLUMNAE	RETICULUM-COLUMNA	columna	greges	ordines	recti	basis	lucrum	puritas	puritas-intra	inaestimati	mutabiles	cadentes	inaestimabilis
fixa	RETICULUM-COLUMNA	f	2	26	23	17	6	884	884	0	0	1	0
fixa	RETICULUM-COLUMNA	h	1	26	17	17	0	653	653	0	0	0	0
fixa	COLUMNAE	RETICULUM-SORS	columna	sors	ordines	recti	basis	inaestimati
fixa	RETICULUM-SORS	f	A	9	9	5	0
fixa	RETICULUM-SORS	f	B	9	9	5	0
fixa	RETICULUM-SORS	f	C	8	5	7	0
fixa	RETICULUM-SORS	h	A	9	5	5	0
fixa	RETICULUM-SORS	h	B	9	5	5	0
fixa	RETICULUM-SORS	h	C	8	7	7	0
fixa	COLUMNAE	RETICULUM-CATENA	vetans	gradus	columna	greges	recti	lucrum	cadentes
fixa	RETICULUM-CATENA	1	0	-	1	17	0	0
fixa	RETICULUM-CATENA	0	0	-	1	17	0	0
fixa	RETICULUM-CATENA	0	1	f	2	23	6	1
fixa	COLUMNAE	RETICULUM-PAR	columna-a	columna-b	greges	recti	incrementum	cadentes
fixa	RETICULUM-PAR	f	h	2	23	0	1
fixa	COLUMNAE	RETICULUM-TEGIT	subtilior	crassior
fixa	RETICULUM-TEGIT	f	h
fixa	COLUMNAE	RETICULUM-DUPLEX	columna	aequalis
```

- [ ] **Step 2: Write the failing probatio** `probatio_oratio_reticulum.c`:
  1. `oratio_reticulum_currere` on `proba.tsv` with the options above →
     `iudicatum` VERUM and `relatio` byte-equal to `exspectata.tsv`
     (compare line by line and print the first differing line).
  2. human run (no `-machina`) → `iudicatum`, `relatio` contains `f`,
     `cadit C` and `INAESTIMABILIS` is absent; with `-sortes thesaurus`
     replaced by a one-fold column (all `A`) → refusal
     `sortes pauciores quam II`.
  3. every refusal from inline TSV strings, asserting `iudicatum` FALSUM
     and the exact `causa` prefix: no header; row before header;
     differing header; short row (line named); unknown `-ubi` title;
     gold without prefix (`-aurum thesaurus`); `aurum-x` in `-columnae`;
     `-gradus f` (non-numeric); `-ubi f=w` (no rows).
  4. `-ubi f=u` keeps 12 rows (`RETICULUM-COLUMNA … h … 12`).
  Add `"partitio"` `"partitio_aestimatio"` to `RADIX_FONTES`, then
  `./oratio/compile_probationes.sh reticulum` → compile failure, exit 1.

- [ ] **Step 3: Implement** `oratio_reticulum.h/.c` (reading, validation,
  partitions per column via `partitio_ex_chordis`, gold and fold
  partitions, `partitio_aestimare` per feature and per pair inside
  `piscina_notare`/`piscina_reficere` keeping only the numbers,
  `partitio_reticulum_struere` over the features, both chains,
  rendering through `ChordaAedificator`), `instrumenta/reticulum.c`
  (argument parsing like `sententiae.c`: `-genus -aurum -sortes -ubi
  -columnae -praeter -gradus -limen -lucrum -greges -machina`,
  comma lists split with `chorda_fissio`; `filum_legere_totum`; exit 0
  judged, 2 refused or usage) and `oratio/reticulum.sh` (copy of
  `verba.sh` with `reticulum` for `verba`).

- [ ] **Step 4: Run green.** `./oratio/compile_probationes.sh reticulum`
  → passes; `./oratio/reticulum.sh
  oratio/probationes/fixa/reticulum/proba.tsv -genus PROBA -aurum aurum-x
  -sortes thesaurus -limen II -lucrum I` prints the human report, exit 0;
  `-aurum thesaurus` → exit 2 with the cause on stderr.

- [ ] **Step 5: Planted fault.** `silva.planta('oratio/fontes/oratio_reticulum.c',
  <the -ubi row test>, <VERUM>, 'oratio', 'reticulum')` → RED on the
  12-row case and the no-rows refusal; green after revert.

- [ ] **Step 6: Worklog** `oratio/fontes/oratio_reticulum.worklog.md`,
  lint, format.

- [ ] **Step 7: Commit** `oratio T35 d: instrumentum reticuli …`;
  `portae=['oratio']`.

---

### Task T35 e: first run, acceptance, records

**Files:**
- Create: `oratio/census/reticulum_recensio.py`
- Modify: `project-specs/oratio-spec.md` (as-built after the T35 design
  block), `oratio/census/README.md` (two rows), `oratio/CLAUDE.md`
  (status paragraph), worklogs; ledger 01M24V1387 → impletum with a
  note; memory `oratio-project.md` bullet.

**Interfaces:**
- Consumes: the LIS TSV with headers; T35 d's machine rows.
- Produces: `reticulum_recensio.py <x.tsv> <same flags>` printing
  `RETICULUM-COLUMNA`, `-SORS`, `-CATENA` rows (with headers) from its
  own implementation of the §7 definitions.

The prediction is written HERE, before the run: on same-verb
subject/object contests with gold `aurum-lectio`, (a) `numerus-capitis`
is among the top single columns; (b) a meet with `numerus-victae` or
`genus-victae` has a mutable group between `la_circse` and the prose
corpora, and the vetoed chain refuses a step the pooled chain takes,
naming `la_circse`.

- [ ] **Step 1: Regenerate the rows.** `S=<scratchpad>`;
  `./oratio/census/metire.sh $S/lites_t35.tsv -lites` → `FINIS 8
  summae`; `grep -c -P '\tCOLUMNAE\tLIS\t' $S/lites_t35.tsv` → 8 (one
  per file with contests; fewer means a file produced no contests —
  name it before going on).

- [ ] **Step 2: Write `reticulum_recensio.py`** (~60 lines, standard
  library only): parse header rows, apply `-ubi`/`-gradus`/`-praeter`,
  build group keys as tuples, and compute per fold with plain
  dictionaries the base (tie → lowest gold index by first appearance),
  votes (≥ limen, strict majority, tie → base), recti, basis,
  inaestimati; the chain by the same ranking and veto. Print the rows in
  T35 d's format. It must NOT import or call the C instrument.

- [ ] **Step 3: The run.**

```
F="-genus LIS -aurum aurum-lectio -sortes thesaurus \
 -ubi caput-victoris-idem=1 -ubi relatio-victoris=subiectum,obiectum-verbi \
 -ubi relatio-victae=subiectum,obiectum-verbi -ubi aurum-lectio=victor,victa \
 -gradus nominativi,nominativi-certi,nominativi-concordes,accusativi-certi,distantia-victae,distantia-victoris \
 -praeter victor,victa,dependens,caput,clausula"
./oratio/reticulum.sh $S/lites_t35.tsv ${=F} > $S/reticulum_t35.txt
./oratio/reticulum.sh $S/lites_t35.tsv ${=F} -machina > $S/reticulum_t35.tsv
python3 oratio/census/reticulum_recensio.py $S/lites_t35.tsv ${=F} > $S/recensio_t35.tsv
```

  (zsh: `${=F}` splits the flags.)

- [ ] **Step 4: Acceptance 1 — the recount.**
  `diff <(grep -P '\tRETICULUM-(COLUMNA|SORS|CATENA)\t' $S/reticulum_t35.tsv)
  <(grep -P '\tRETICULUM-(COLUMNA|SORS|CATENA)\t' $S/recensio_t35.tsv)`
  → empty. Any difference is a defect in one of the two; settle it
  (print the group counts for the first differing column) before going on.

- [ ] **Step 5: Acceptance 2 — the retrodiction.** Read
  `$S/reticulum_t35.txt`: (a) the rank of `numerus-capitis`; (b) the two
  chains and their divergence, then `-greges catena` on the pooled chain
  for the `!` groups. Record the outcome as MEASURED, whichever way it
  falls; if (b) does not hold, stop and discuss with Fran before any
  weight work (the tool or the reading of T32 f is wrong).

- [ ] **Step 6: Records** (docs first, then commit): spec §7 "As built —
  T35" paragraph (numbers from steps 4–5, any definition corrected by
  measurement recorded in place); `oratio/census/README.md` rows for
  `reticulum.sh` and `reticulum_recensio.py`; `oratio/CLAUDE.md` status
  paragraph; worklogs; ledger `gerere 01M24V1387 status impletum` + a
  note with the acceptance result; memory `oratio-project.md` one bullet.

- [ ] **Step 7: Commit** `oratio T35 e: …` (viae: the script and the
  docs; `portae=()` — no code changed).

---

## Self-review

- **Spec coverage.** §7 SOURCE/ALGEBRA → T35 a; JUDGMENT → T35 b
  (additions named here, not in the spec: `numerus_gregum`,
  `numerus_aureorum`, `suffragia`, `numeri` on `PartitioAestimatio`
  and `initium` on `PartitioCatena` — the instrument's per-group table
  and step 0 need them; the BASE tie rule, lowest gold index, which the
  spec left unstated); ORACLE → T35 c (plus the formatter moved into
  the library so the header law is testable in-process); INSTRUMENT →
  T35 d (plus `oratio_reticulum` library for the same reason); GATES →
  the four probationes with their plants; ACCEPTANCE → T35 e; LEFT OUT
  and OPEN → nothing built. Record the additions in the as-built.
- **Placeholders.** None: anchors for `silva.planta` are named by the
  line they replace; the implementer copies the exact text at plant time.
- **Type consistency.** `Partitio*` / `constans Partitio* constans*`
  arrays everywhere; `lucrum` s32; judgments s32 enum values in
  `OratioOraculumLis`; machine-row titles identical in T35 d's fixture,
  the probatio and the Python recount.
