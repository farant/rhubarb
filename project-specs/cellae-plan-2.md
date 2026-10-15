# cellae II: plan (house tier)

Approved by Fran (2026-10-13): the API sketch and five decisions:
- per-set judgment only (counting and sampling stay in the caller);
- the antipodal encoding stays XOR-only;
- quadratic fields only (`surdus` as a fast path only if T4 measures a
  need);
- D119's frozen exact tables as the regression oracle;
- P1's design questions are left to P1's own plan (T6).

Follows `project-specs/cellae-plan.md` (slice 1, closed with D125).

## Purpose

Audit P1 (ledger …E72XG3, claims.md KC 47–48) asks whether the group
catalogs compute better or worse than knot-free point sets under one
fixed exact activation. P1 as filed uses D94's activation: the signed
encoding with XOR, AND and MAJ. That engine exists only as about 2,000
lines of demo code (D119, D120). `cellae` has a close cousin (D82/D84),
but with a different sector rule, the antipodal encoding, and XOR only.

This slice moves D94's engine into `cellae`, generalized just enough
for P1 and later audits:
- catalogs from **explicit point lists**, not only group closure: null
  catalogs are arbitrary point sets;
- **activation tables from explicit axes**, so one set can be judged
  with another set's table (2O's) or a group-free table. Today the
  table is always built from the catalog judged, the confound of KC
  47.1.4;
- **D94's phase cell** beside D82's combined cell;
- the **signed encoding** with **any truth table** (XOR, AND, MAJ, …),
  judged per weight set under rule, robust and possible.

P1 itself is a demo on top of this slice, with its own plan (task T6).
Fran approved extracting the engine rather than copying it again.

## Semantics

**D94's phase cell** (D119's exact form). For a sum S = (a, v), with
k sectors and nd axes:
- **zero sum:** cell (k − 1)(nd + 1) + nd;
- **sector:** floor(h·k/π) with h = acos(a/|S|) ∈ [0, π], so the
  full rotation angle 2h lies in k sectors.
  - On a boundary, a/|S| = cos(mπ/k) for some 1 ≤ m < k, the rule is
    sector m (the floor of the exact angle) and the tie set is
    {m − 1, m}.
  - Unlike D82's cell there is no fold: S and −S differ.
- **axis:** nd when v = 0. Otherwise the first maximum of
  (v·u)²/|u|² over the table (rule), with all maxima as the tie set.
  This is the same comparison as D82's.
- **cell** = sector · (nd + 1) + axis.

**Signed encoding.** Weights q_1 … q_n. A mask x ∈ {0,1}^n gives
S = Σ ±q_i (+ where the bit is 1). A truth table assigns 0 or 1 to
each of the 2^n masks. A weight set computes the function at k if no
cell receives both values:
- **rule:** rule cells;
- **robust:** under every tie resolution (all tied cells);
- **possible:** under some resolution, chosen per exact point. Two
  masks with the same exact sum and different values fail at once;
  otherwise SAT (DPLL with a decision budget, as D119).
- **"Computes"** (D94): some k of a given ladder passes, per mode
  separately.
- **"Tied"**: some mask has a tie set larger than one cell.

D82's combined cell keeps its antipodal encoding and parity
(`activatio_separat`, unchanged). The new judgment also accepts D82's
cell under the signed encoding, since the cell rule and the encoding
are independent choices.

## API sketch (additions to include/cellae.h)

```c
/* catalog from an explicit point list, in the given order; FALSUM if
 * a point repeats up to sign, the ring is not an ordered quadratic
 * field, or the list is empty. Depth and writhe 0. */
b32
catalogus_ex_punctis (
    constans Quaternio*  puncta,
                   i32   numerus,
               Piscina*  piscina,
             Catalogus** exitus);

#define CELLAE_CELLA_D82  0    /* combined cell (slice 1) */
#define CELLAE_CELLA_D94  1    /* phase cell (this slice) */

/* activation over 'catalogus' (whose sums it judges) with the axis
 * table given explicitly: the vector parts of 'axes' in order,
 * antipodes merged; NIHIL = the catalog's own table (as
 * activatio_ex_catalogo). Axes must lie in the catalog's ring. */
b32
activatio_creare (
    constans Catalogus*  catalogus,
    constans Quaternio*  axes,
                   i32   numerus_axium,
                   i32   cella,             /* CELLAE_CELLA_* */
                   i32   axes_maximae,
               Piscina*  piscina,
             Activatio** exitus);

/* the table's axis i (a quaternion with zero real part), to hand one
 * set's table to another */
Quaternio
activatio_axis (
    constans Activatio* activatio,
                   i32  i);

nomen structura {
    b32 regula;
    b32 robusta;
    b32 possibilis;
    b32 ligata;        /* some mask tied at some k */
    b32 concursus;     /* same exact sum, different values */
    b32 indecisa;      /* SAT budget hit: possibilis unknown, never
                        * guessed */
} Iudicium;

/* signed encoding: one weight set, one truth table (2^n bytes, mask
 * bit i = weight i), a ladder of k; each mode passes if SOME k passes
 * (D94). FALSUM if n is 0 or > VII, an index is out of range, k < 1,
 * or the table is malformed. */
b32
activatio_iudica (
       Activatio* activatio,
    constans i32* pondera,
             i32  n,
    constans i32* scala_k,
             i32  numerus_k,
     constans i8* tabula,
        Iudicium* exitus);

/* truth tables for the three functions of D94 (2^n bytes in piscina) */
constans i8* cellae_paritas (i32 n, Piscina* piscina);   /* XOR */
constans i8* cellae_et      (i32 n, Piscina* piscina);   /* AND */
constans i8* cellae_maior   (i32 n, Piscina* piscina);   /* MAJ */
```

Counting (exhaustive over C(N, n), or sampled with a caller's RNG)
stays in the caller. D94's sampling streams and scaling are demo
semantics, and a loop over `activatio_iudica` is a few lines. The
existing searches (`activatio_exhaustiva`, `activatio_avida`) stay as
they are, for the antipodal XOR audits.

## Exactness

Slice 1's machinery carries over:
- integer Z[√d] coordinates;
- a cache per exact point;
- the certified float filter, which never decides a tie;
- boundaries placed by position (radices).

For the phase cell, the sector compares a/|S| with cos(mπ/k): the sign
of a, then a²/|S|² ∈ K against cos²(mπ/k), placed by position among
the roots of its minimal polynomial. That is the same technique as
slice 1's tan² comparison.

Fields stay quadratic (v1 scope). P1's catalogs (2O over Q(√2), 2T
over Q, 2I over Q(√5)) and nulls built on suitable conics fit inside.
D120's `surdus` path is a measured option (T4), not a default.

## Tasks

Each task follows the house cycle:
1. implement;
2. tests red, then green;
3. planted faults;
4. worklog;
5. commit behind the gates;
6. stop for Fran's go-ahead.

- **T1. Point-list catalogs and explicit axis tables.**
  `catalogus_ex_punctis`, `activatio_creare` (D82 cell only),
  `activatio_axis`.

  Credo:
  - a catalog from ζ₈'s own points equals its closure, entry for
    entry;
  - an explicit table equal to the own table gives D124's tables
    unchanged;
  - 2O's table carried over to a subset;
  - refusals (repeats, foreign ring, empty).
  *(2026-10-13: landed. Axes are now stored pure, (0, v), with floats
  from the axis itself, replacing `axis_elementum`; the slice-1 suite
  stayed green unchanged. D124's 13-axis numbers reproduced by a second
  route (each subset as its own catalog plus the full table). 10 plants
  red.)*
- **T2. The phase cell.** `CELLAE_CELLA_D94`: sectors by cos² roots,
  the zero sum, a zero vector part, and the tie sets.

  Credo:
  - D120's hand cells (σ1 on the k = 12 boundary m = 3: rule sector
    3, tie {2, 3}; at k = 6 sector 1, no tie; −σ1 below the boundary);
  - every ζ₈ sum of up to 4 weights at k = 6, 12, 24 against an
    independent exact oracle (Python, Q(√2, √3)).
  *(2026-10-14: landed. The sector reduces to slice 1's tan^2 machinery,
  reflected by the sign of a; the point cache stays keyed by {s, -s},
  the per-k cell by (k, sign). Oracle in the test instead of Python:
  D119's own method (signs of a/|s| - cos(m pi/k) by `surdus`) plus the
  existing integer axis oracle. 562,080 cells, 0 disagreements. 8
  plants red.)*
- **T3. The signed encoding and `activatio_iudica`.** Truth tables,
  rule, robust, possible (point clash and SAT), any-k union, tied,
  undecided.

  Regression credo:
  - D119's exact columns (rule, robust, possible, tied) for ζ₈ and
    2I's first 24, every exhaustive row (N = 3–6, XOR / AND / MAJ):
    24 rows × 4 numbers, pinned from D119's frozen output;
  - D119's oracle counts (ζ₈ N = 3, 4, 18 numbers).
  - The slow rows (N = 6) go in a credo section signed `lentus`.
  *(2026-10-14: landed. Every D119 row equal for N = 3-6, both
  catalogs, none undecided. N = 3, 4 run in 6 s; N = 5, 6 (`lentus`) in
  40 s at 463 MB peak. The 18 oracle.py counts are D119_B's N = 3, 4
  entries, so those rows pin them. Each mask carries an orientation
  (s or -s of the point), and the SAT keeps two identities per point
  under D94. `concursus` (same exact sum, different values) is computed
  completely after the k loop, not as a SAT by-product, and is checked
  against a plain quaternion-sum oracle on every N = 3, 4 set. That
  oracle found the bug of the slice: the zero sum must have no
  orientation. The phase cell's float filter (reflected for a < 0)
  moved here from T4, checked set by set against the filter off. A
  reversed ladder {24, 12, 6} was added after a plant survived: on
  {6, 12, 24}, k = 24 refines the others, so "last k only" equals the
  union. 16 plants red.)*
- **T4. Speed, measured first.** *(The filter part landed in T3; T4
  is measurement against D120, plus the combination cache's memory
  (463 MB over about 1M judgments).)*
  *(2026-10-14, checkpoint: measured with a harness reproducing D120
  Parts B-C through `activatio_iudica`. All 36 D119 rows are equal,
  including the sampled N = 7, 8 rows. N = 8 needed its own limit
  (`CELLAE_N_SIGNATA_MAXIMA`). Memory went from 949 MB to 36-69 MB:
  the combination cache was 97% of it and is now per call. Each cell
  is computed once per (mask, k); the exact path takes the vector's
  float hint; keys are binary. About 265 s against D120's 125 s for
  both tables. What's left is exact sums and point keys through general
  algebraic numbers (~46%), which T4b targets. venenum's cellae build
  was broken since T2 and undeclared; both fixed. 8 plants red.)*
- **T4b. Integer image of the catalog** (sketch for Fran first). Exact
  sums and point keys in integers over (p + q√d)/D, the general path
  as the fallback, one key scheme per activation.
  - The certified filter for the phase cell, and the per-point cache
    on signed sums.
  - Compared with D120 on the same rows (D120: Parts A–C in 136 s).
  - `surdus` as a fast path only if the measurement asks for it.

  Credo: filtered == exact on every ζ₈ sum; the statistics separate
  filter, exact and tied decisions.
  *(2026-10-14, done: each part = (p + q·alpha)/D in s64 (|p|, |q| ≤
  2^40), with basis {1, alpha} rather than √d, so no factoring and no
  `surdus` is needed. The canonical sign is taken on the integers; the
  relation to the real representative is stored per point, computed
  once. Representability depends on the value, so `activatio_cella`
  uses the same namespace. New API: `activatio_imago`, plus
  statistics `imagine`/`algebraice`/`puncta`. Both tables N = 3-8 in
  113 s against D120's 125 s; 36/36 rows; zero algebraic sums. 3 credo
  sections, 8 plants red.)*
- **T5. Review** (an independent reviewer with its own oracle), then
  fixes.
  *(2026-10-14, done: an independent Fraction oracle over ~63k weight
  sets and ~46k cells found one serious bug: the float filter took the
  axis doubles as relatively accurate, so tiny axes gave a NaN or the
  wrong direction, wrong verdicts and an out-of-bounds write. Fixed by
  normalising each axis exactly before converting it; when a value
  can't be converted, the vector takes the exact path (the filter
  abstains, it never gives FALSUM). Also fixed: the silent caps of
  64 k (termini, per-point memory, avida scale); external image sums
  bounded at 2^62 (the UB at -2^63); D94 now judged at odd k (k | 24
  oracle and ladders). 4 new credo sections; 10 plants red, 2
  defence-in-depth guards are equivalent mutants. The reviewer's
  witnesses now show 0 disagreements.)*
- **T6. P1** as a demo, with its own plan for Fran's approval first.
  That plan settles, among other things:
  - which group-free table;
  - which nulls (D126: match height, report odd-norm coincidences,
    name the encoding);
  - whether the braid-correct ζ₁₀ generators are included.
  *(2026-10-15, done as D127, knotapel/demo_127_matched_nulls: 108
  point sets, one exact activation, common tables T13 and R13, N = 3–7
  exhaustive, 264 jobs in worker processes. An independent review came
  before the findings and claims (0 H, 6 M, 6 L, 6 N, all acted on):
  it added the entry-order, 20-seed and sign controls and the R13 and
  D119 N = 5–6 anchors. Result:
  - catalogs lose to generic points at every N;
  - against height-matched spheres there is no consistent difference;
  - 2I's standing is entry order;
  - only XOR is a property of a point set.
  The registered rule was underspecified, so no verdict on KC 47.1 is
  drawn. Nodes appended: KC 19.1.2, 19.7.1, 19.9.1, 19.10, 47.1.4.1,
  47.1.5–47.1.5.3, 48.1.1.1, 48.1.2, 48.2.1 and 48.4.6.1. Ledger P1
  fulfilled; follow-up question …A71WQK.)*

## Gates

- `radix` and `silva` (lib/cellae.c changes).
- `generata` (if an amalgam embeds cellae).
- `venenum` (the caches and arenas).
- `oratio` if the glossary is touched.

## Out of scope

- AND/MAJ under the antipodal encoding. There each weight has two
  input bits, and x_i = 0 arises from both bits on or both off, so a
  general function needs bit-level patterns (4^n). XOR is the parity
  of the nonzero coefficients and is supported already.
- Fields beyond quadratic.
- Moving D94's float replica into the library. It reproduces D94's
  printed numbers and stays in the demos.
- Parallel processes.
