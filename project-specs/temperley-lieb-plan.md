# temperley and the TL bracket in laqueus: plan (D136 T4, step 1)

Status: v2 APPROVED by Fran (2026-10-30); T1-T4 done (review folded in 2026-10-31).
v1 (ca3b3fee) was approved by Fran on the API's shape, the library
split, the name and the strand cap. The review (verdict "approve after
fixes": H1–H2, X1, M1–M2, Q1–Q6) is at the end, with what each finding
changed.

Agreed with Fran (2026-10-30), before this plan was written: T4 goes
**promote first, then a speed pass, then track A** (Jones statistics on
random braid closures). This plan is the first step only. The speed pass
gets its own plan with registered predictions; track A gets its own
plan, after a literature check.

## Purpose

D136 showed that the Kauffman bracket of a braid closure through
Temperley–Lieb standard modules is exact, fast where the vertex model is
slow (12 × 801 in 78 CPU-minutes against a projected month), and slower
where the vertex model is cheap (short braids with many strands). The
demo is frozen; this plan moves the method into house libraries with
tests, so that later work (the speed pass, track A, and §12.3.2–3 of
the brainstorm: colored Jones through Jones–Wenzl projectors, TL at
roots of unity) builds on a tested function instead of a demo.

## Governing rules

1. **Exact only**, as in contractio-plan.md rule 1: reconstruction is
   driven by the proven Tait-graph bound, never by "the answer stopped
   changing". Cost comparisons are integer, saturating; no floats.
2. **Same algorithm as the demo first.** The library reproduces D136's
   evaluator (two buffers, row gather, letters last to first, Δ_j
   weights, /δ). Speed changes come later, each against this baseline.
3. **No globals, no qsort.** Everything lives in the structure or in a
   local context; memory from the caller's piscina, under a byte cap
   like the vertex model's `octeti_maximi`.
4. **Same contract as the vertex model** wherever both apply: the same
   points (A = i·ω², one shared interior point-choice function), return
   codes, interpolation settings, window and bound, so the two routes
   are interchangeable and comparable at every point.
   **[Amended 2026-10-31, temperley-celeritas-plan T2 gate G1:** the TL
   route accepts only primes below 2^31 (ERRATUM otherwise) and its
   interpolation uses primes below 2^31, so its kernel needs one
   reduction per element (1.71-1.80× faster per point). Points,
   return codes otherwise, window and bound stay shared; the two routes
   remain comparable at every prime below 2^31.]
5. **Pricing allocates nothing.** Choosing a route builds no workspace
   for the route not taken (H2).

## API

### New library `temperley` (TL_n and its standard modules)

Pure algebra; no knots, no primes. Separate from `laqueus` because the
modules serve more than the bracket (Jones–Wenzl projectors, roots of
unity) and because their tests stand alone.

```c
/* temperley.h - algebra Temperley-Lieb TL_n: moduli standard W_{n,j}
 * Status nexus: n puncta, paria non transeuntia, j defectus (puncta
 * sine pari), nullus defectus intra par inclusus. Dimensio
 * d = C(n, (n-j)/2) - C(n, (n-j)/2 - 1); summa d^2 super j = Catalan(n).
 * ORDO statuum: lexicographicus formae textus, ( < ) < |, puncto 0
 * primo - ordo quo enumeratio eos gignit (nullus qsort).
 * TEMPERLEY_FILA_MAXIMA XVI tenet: clavem 2n bitorum in i32, et
 * indices d^2 (maximum 13.2e6) in i32. */

#define TEMPERLEY_FILA_MAXIMA XVI
#define TEMPERLEY_DEFECTUS (-I)          /* par puncti defectus */
#define TEMPERLEY_ERRATUM  (-I)          /* actio, quaerere: argumenta mala */
#define TEMPERLEY_NIHIL    ZEPHYRUM      /* e_i s = 0 */
#define TEMPERLEY_UNUM     I             /* e_i s = imago */
#define TEMPERLEY_DELTA    II            /* e_i s = delta * imago */

nomen structura TemperleyModulus TemperleyModulus;   /* opacum */

/* W_{fila, defectus}; NIHIL si fila 0 aut > XVI, paritas impar,
 * defectus > fila */
TemperleyModulus* temperley_modulus_creare (i32 fila, i32 defectus,
                                            Piscina* piscina);
i32 temperley_modulus_dimensio (constans TemperleyModulus* m);
i32 temperley_modulus_fila     (constans TemperleyModulus* m);
i32 temperley_modulus_defectus (constans TemperleyModulus* m);

/* e_i (1 <= i < fila, unum-basatum) in statum: genus (NIHIL, UNUM,
 * DELTA) et *imago (non scriptum pro NIHIL); TEMPERLEY_ERRATUM si i aut
 * status extra ambitum (*imago non tactum) */
s32 temperley_actio (constans TemperleyModulus* m, i32 i, i32 status,
                     i32* imago);

/* paria[x] = par puncti x, TEMPERLEY_DEFECTUS si defectus; FALSUM si
 * status extra ambitum (paria non tacta) */
b32 temperley_status_paria (constans TemperleyModulus* m, i32 status,
                            s32* paria);

/* index status cuius paria data sunt; TEMPERLEY_ERRATUM si non status
 * moduli (ad mappas inter modulos, formam Gram, Jones-Wenzl) */
s32 temperley_status_quaerere (constans TemperleyModulus* m,
                               constans s32* paria);

/* "(())|" - ad probationes et ad errores legendos */
chorda temperley_status_ad_chordam (constans TemperleyModulus* m,
                                    i32 status, Piscina* piscina);
```

Implementation notes (binding):
- The search state (current pairing, open stack, count) lives in a
  local context struct passed down the recursion (depth ≤ 16).
- **The key** (X1): base-4 digits, **point 0 most significant**, codes
  `(` = 0, `)` = 1, `|` = 2. This is strictly increasing in enumeration
  order in every module for n ≤ 16 (checked by the reviewer; maximum
  2,863,311,530 < 2³²), so binary search works without sorting. The
  demo's key (point 0 least significant, defect = 0) is not monotone in
  49 modules and only worked because of its qsort.
- A C89 compile-time check (negative-size array typedef) that
  `2 * TEMPERLEY_FILA_MAXIMA <= 32`.
- An image not found while building the action table cannot happen; the
  branch returns NIHIL and is recorded as unreachable, not tested.

### In `laqueus`: the TL route and the shared window and bound

```c
/* fenestra uncinus (classis 3w mod 4) et limes Tait ex codice PD nodi,
 * c >= 2; FALSUM si codex invalidus (exitus non tacti); limes_exactus
 * NIHIL licet. Publica: duae copiae interiores iam exstant
 * (tools/uncinus_mensura.c, D136). */
b32 laqueus_pd_fenestra (constans i32* pd, i32 transitus, Piscina* piscina,
                         s32* imus, s32* summus, i32* limes,
                         Magnus* limes_exactus);

nomen structura LaqueusModulusTemperley LaqueusModulusTemperley;  /* opacum */

nomen structura {
    i64 multiplicationes;     /* per punctum: longitudo x summa d_j^2 */
    i64 octeti;               /* alvei + tabulae + structura */
} LaqueusTemperleySumptus;

/* sumptus SINE aedificatione (formae clausae ex fila et longitudine) */
LaqueusTemperleySumptus laqueus_temperley_sumptus (i32 fila, i32 longitudo);

/* structura semel: tabulae actionis (copiae ex temperley, ordine
 * proprio), duo alvei, verbum copiatum, fenestra et limes. Verbum ut
 * laqueus_pd_ex_plexu. NIHIL si verbum refutatur, catena (plures
 * componentes: fenestra et limes nodorum tantum), longitudo < 2, fila >
 * XVI, aut octeti > octeti_maximi. Alvei per piscina_conari_allocare
 * (piscina certae magnitudinis non abortat). */
LaqueusModulusTemperley* laqueus_modulus_temperley_creare (
    constans s32* verbum, i32 longitudo, i32 fila, i64 octeti_maximi,
    Piscina* piscina);

/* contractus idem ac laqueus_modulus_vertex_aestimare: puncta eadem
 * (functio interior communis), INUTILE ubi delta = 0 (idem ac d vertex),
 * ERRATUM eadem; *punctum et *valor non tacti nisi UTILE. Nihil allocat. */
s32 laqueus_modulus_temperley_aestimare (LaqueusModulusTemperley* mt,
                                         i32 omega, i32 p,
                                         i32* punctum, i32* valor);

b32 laqueus_modulus_temperley_uncinus (LaqueusModulusTemperley* mt,
                                       Piscina* piscina, Polynomium* exitus);
i32 laqueus_modulus_temperley_limes (constans LaqueusModulusTemperley* mt);
```

The byte cap counts what depends on fila alone and is known before any
allocation: the two buffers (2 × max d² × 4 bytes; 106 MB at n = 16),
the action tables (1.2 MB at n = 16) and the structure. It does not
count the O(c) PD, window and bound memory or the interpolation's
memory; the vertex cap leaves its network out the same way, and both
headers will say so. Creation allocates the laqueus tables first, then
builds the temperley modules inside `piscina_notare` / `_reficere`, so
the tables never exist twice.

### The dispatcher

```c
nomen enumeratio {
    LAQUEUS_VIA_NULLA = ZEPHYRUM,     /* c <= 1: forma clausa */
    LAQUEUS_VIA_VERTEX = I,
    LAQUEUS_VIA_TEMPERLEY = II
} LaqueusVia;

/* via vilior SINE aestimatione et sine officina allocata; sumptus
 * utriusque NIHIL licet. FALSUM si verbum refutatur aut neutra via
 * intra octeti_maximi. */
b32 laqueus_via_eligere (constans s32* verbum, i32 longitudo, i32 fila,
                         i64 octeti_maximi, Piscina* piscina,
                         LaqueusVia* via, ContractioSumptus* sumptus_vertex,
                         LaqueusTemperleySumptus* sumptus_temperley);

/* uncinus (et Jones) clausurae tricae per viam electam; via NIHIL licet */
b32 laqueus_uncinus_ex_plexu (constans s32* verbum, i32 longitudo, i32 fila,
                              i64 octeti_maximi, Piscina* piscina,
                              Polynomium* exitus, LaqueusVia* via);
b32 laqueus_jones_ex_plexu ( ...same arguments... );
```

Rules (H2, Q2):
- **Refusals first**, before any pricing or allocation: invalid word,
  and links (multi-component closures; the window and bound are
  knot-only).
- **c ≤ 1 in closed form**, via NULLA: c = 0 gives 1; c = 1 (σ₁^±1 on 2
  strands) gives −A^(±3), Jones 1. Both modulus creares refuse c < 2,
  because the window and bound need a PD the code can read.
- **Eligibility**: vertex needs its workspace ≤ cap; TL needs fila ≤ XVI
  and its bytes ≤ cap.
- **Choice**: the cheaper eligible route by integer cost: vertex
  `multiplicationes` × 185 against TL `multiplicationes` × 150 (D136's
  ~1.85 and ~1.5 ns), saturating (`CONTRACTIO_SATURATUM` must not
  overflow). Tie goes to TL.
- **Pricing the vertex route** needs its contraction order but not its
  workspace: vertex creation is split internally into a planning step
  (PD, network, ordering, giving `ContractioSumptus`) and a workspace
  step, and the dispatcher allocates only for the route it picks. T3
  measures the cost of planning at 12 × 801 (ordering 801 tensors)
  before the crossover test enters the suite.
- **No retry** on the other route if the chosen one fails: both are
  exact, so a failure is a bug and a retry would hide it.

The choices it must reproduce (checked by the reviewer against
`coniectura.out`, margins 8× or more): vertex at 12 × 101 and 12 × 201;
TL at 8 × 201, 10 × 201, 10 × 401, 12 × 401, 8 × 801, 12 × 801.

## Tests

The suites run under `CREDO_NON_RUIT` / `CREDO_NON_PENDET` around the
heavy sections (relations at n ≥ 12, long brackets), so a crash becomes
a named failure and the rest still runs. A run without a credo summary
is inconclusive and is rerun, never counted.

`probatio_temperley.c`:
- dimensions equal the formula and Σ d² = Catalan(n), n ≤ 16;
- **structure from `paria`**, every state, n ≤ 16: an involution
  without fixed points on the paired points, exactly j defects, no two
  arcs cross, no defect inside an arc, string forms strictly increasing;
- **independent enumeration**, n ≤ 10: the test lists all 3ⁿ strings
  over `( ) |`, keeps the balanced ones with `|` only at depth 0, sorts
  them, and requires exactly the module's `ad_chordam` sequence;
- **direct action oracle**, n ≤ 10: the test applies rules (a)–(d) to
  `paria` itself, looks the result up in its own list, and compares
  index and code with `temperley_actio`. (The TL relations alone are
  satisfied by e_i ≡ 0, so they cannot catch an all-NIHIL fault.)
- TL and braid relations on every basis state, n ≤ 12 (16 if the suite
  stays within a few seconds), tracked exactly as (index, power of δ,
  zero flag), so the library's tests need no primes;
- `quaerere` inverts `paria` on every state; refuses a crossing pairing;
- refusals with outputs untouched: fila 0 and > XVI, wrong parity,
  defectus > fila, i = 0, i = fila, status out of range.

`probatio_laqueus_rete.c` (where the vertex tests live), additions:
- **window and bound golden values** (T2a, captured before the
  refactor): (imus, summus, limes, limes_exactus, writhe) for ~20
  explicit knots plus one checksum over all 12,966 table knots, and the
  recorded limes at 12 × 801, 8 × 801, 12 × 401, 12 × 201 (654, 642,
  333, 165 bits); the same values after the refactor, plus the existing
  vertex brackets unchanged;
- TL bracket = state sum (c ≤ 24) on random braids, n ≤ 8; = vertex
  bracket on random braids up to c ≈ 100;
- TL value = vertex value at points of two primes, **punctum and
  valor** both, including the suite's prime 4,294,967,029 (catches a
  32-bit multiply fault);
- TL limes = vertex limes on the same braid;
- stabilization front and back at points (× −A^±3), which reaches sizes
  neither the state sum nor the vertex model can (e.g. 12 × 401 at 3
  points);
- prime p → q → p gives the same value; `aestimare` allocates nothing
  (`piscina_numerus_allocationum` before and after);
- known knots: 3_1, 4_1, T(3,4), T(4,5) against the table;
- a few lines of D136's certified `brackets.txt` (3 × 50, 8 × 101,
  8 × 201) embedded as strings, the braid words with them (or a
  fingerprint of each word, so a change in `sors` cannot silently swap
  the braid);
- the δ = 0 and bad-prime sentinels copied from the vertex suite
  (ω = 5, p = 17 → INUTILE; ω = 0; p = 7, 21, 85, 2³² − 5 → ERRATUM;
  outputs untouched);
- closed forms: σ₁ → −A³, σ₁⁻¹ → −A⁻³ (via NULLA), empty word with
  fila 1 → 1;
- refusals before allocation: Hopf link σ₁², an untouched strand, an
  empty word with fila ≥ 2, a letter out of range;
- the byte cap at its boundary: cap = exact bytes accepted, exact − 1
  refused (`laqueus_temperley_sumptus` gives the exact figure);
- the dispatcher, via `laqueus_via_eligere` only (no evaluation): the
  choices above; 12 × 201 with the cap just below its vertex workspace
  chooses TL; fila XVI with the cap below 106 MB chooses vertex; both
  over the cap → FALSUM.

**Plants.** Each is registered as its literal (file, old text, new
text) triple in a script under `build/`, written before the run, with
the test names it must turn red; the report quotes the script. The
harness asserts those names appear among the failures, and a run that
crashes or prints no summary is inconclusive (D136's lessons):
- action case (c) → 0; all codes → NIHIL (must hit the action oracle);
- enumeration choice order swapped (must hit the order test);
- enclosed defects admitted (must hit the enumeration count);
- Δ recurrence with +; /δ dropped; σ⁻¹ given σ's image;
- byte cap off by one (> for ≥);
- dispatcher comparison inverted;
- TL limes − 1 (must hit TL limes = vertex limes);
- g not minimal in the point choice (must hit punctum equality).

Recorded now as untestable, not as survivors: the tie rule (equal costs
do not occur with real braids); the unreachable "image not found" and
`_limes_arborum` failure branches.

## Tasks

- **T0. Approve this plan.** v1 approved by Fran; design review folded
  in as v2; Fran approves v2.
- **T1. `temperley`.** Header, source, probatio, glossary entries
  (`temperley`, `lieb`, and any word the lint flags), plants.
  **T1 note (2026-10-30, done).** 49b08975: `temperley` as specified;
  probatio 9 sections, 1,017 checks, 0.4 s. Plants 8/8 as registered
  (lib/temperley.worklog.md). Deviations: braid relations are left to
  T2b (they need coefficients; the TL relations imply them); glossary
  needed only `temperley` (`lieb` appears in no identifier).
- **T2a. Window and bound refactor.** Capture the golden values at the
  pre-refactor commit (window from `tools/uncinus_mensura.c`'s copy,
  limes from the vertex accessor; record the commit hash), embed them,
  then factor `laqueus_pd_fenestra` out (face tracing comes out of the
  gyration code, so the vertex brackets must stay equal too), and split
  vertex creation into planning and workspace steps.
  **T2a note (2026-10-30, done).** 77c5f8f6: `laqueus_pd_fenestra`
  public; `_modulus_vertex_consilium` (no workspace); golden values
  captured at be6be402 and unchanged after the refactor (21 knots,
  checksum over 12,965 table knots, D136's five braids); plants 4/4,
  the wider window caught only by the golden section (worklog).
- **T2b. `laqueus_modulus_temperley_*`.** The shared point-choice
  function, creare, aestimare, uncinus, limes, `laqueus_temperley_sumptus`;
  probatio additions except the dispatcher; plants.
  **T2b note (2026-10-30, done).** 6c1860f7: the TL route as specified;
  `_punctum_eligere` and `_quaestio_uncini` shared with the vertex
  route (its output unchanged); 7 test sections; plants 7/7
  (worklog). Deviations: the "g not minimal" plant is moot with a
  shared point choice (replaced by "punctum reported as A⁻¹"; g pinned
  by the p = 17 test); stabilization tested on 10 × 401 (11 strands)
  instead of 12 × 401 (13 strands too slow in the test build); the two
  hand-written source lists (knot-table generator, venenum gate) gained
  lib/temperley.c.
- **T3. Dispatcher.** `laqueus_via_eligere`, `laqueus_uncinus_ex_plexu`,
  `laqueus_jones_ex_plexu`; measure planning cost at 12 × 801 first;
  probatio; `lib/laqueus.worklog.md` entry.
  **T3 note (2026-10-30, done).** 6b716f62: dispatcher as specified;
  planning the vertex route measured at <= 0.2 s (12 × 801); every
  registered choice as predicted; 3 test sections; plants 5/5
  (worklog). The vertex cap boundary is tested as a descent, since a
  smaller cap makes the planner choose a smaller order. [Corrected
  after T4 (M1): this note first said the "cap below the vertex
  workspace chooses TL" test could not be built because a cheaper
  vertex route always needs less memory than TL. That was FALSE - the
  reviewer found 1,255 flips in 24,690 random braids, n = 7..14; the
  12 × 201 braid I measured just sits on the other side. The test is
  restored with the reviewer's 7 × 32 braid.]
- **T4. Review.** Independent reviewer with its own oracle
  (recensor-d136's regular-representation oracle can serve as the
  reference), not building in the live tree.
  **T4 note (2026-10-31, done).** See "Review (T4)" at the end.

## Questions for Fran (answered 2026-10-30: all three as recommended)

1. **The library's name** → `temperley` (a proper name, like `jones` and
   `alexander` in laqueus identifiers). A Latin alternative such as
   `copulae` (pairings) would hide the Temperley–Lieb algebra.
2. **Separate library or inside `laqueus`?** → separate (laqueus.c is
   already 3,244 lines).
3. **Fila maximum XVI** → yes. D136 measured up to 16; the cap also
   holds the 32-bit key and the i32 index arithmetic (X1).

## AUDIENDA

- The dispatcher's constants are from one machine and one load; the
  choice can be wrong near the crossover, which costs time, never
  correctness.
- `brackets.txt` as test oracle: computed by the demo, certified by the
  T3 reviewer's independent oracle, so not self-referential, but the
  library shares the demo's algorithm.
- Nothing here has been measured for the library itself; the demo's
  numbers apply only insofar as the code is the same.
- Planning the vertex route at 12 × 801 may itself be slow (ordering
  801 tensors); T3 measures it before relying on it.

## Design review (2026-10-30, consiliarius-temperley and -2)

Two reviewers: the first was cut off by an API error after H1, H2, M1,
M2 and Q1 (partial Q2); a fresh reviewer covered the rest from narrower
questions. Verdict: approve after fixes; the API's shape, the split,
the name and the cap stand. Findings and where they went:

- **H1** the dispatcher's choice was observable only by computing the
  whole bracket (78 min at 12 × 801) → `laqueus_via_eligere`.
- **H2** pricing the vertex route allocated its workspace (557 MB at
  12 × 801) even when TL is chosen → planning and workspace split;
  governing rule 5; eligibility, tie, no-retry and integer saturating
  comparison rules.
- **X1** without the qsort, the demo's state key breaks binary search in
  49 modules → key with point 0 most significant; compile-time check.
- **M1** table ownership → laqueus copies the action into its own layout
  once; `temperley_actio` error code, outputs untouched.
- **M2** state order visible through the API → defined as the string
  order the enumeration already produces; no qsort, no global.
- **Q1** validity untestable through strings (a crossing prints like a
  nesting) → `temperley_status_paria`, `_quaerere`, `_fila`,
  `_defectus`; structure test, independent enumeration, direct action
  oracle (the relations cannot see e_i ≡ 0).
- **Q2** edge cases → c ≤ 1 closed form with `LAQUEUS_VIA_NULLA` (an enum
  value added later would break callers), both creares refuse c < 2,
  links refused before pricing, δ = 0 sentinels copied, one shared
  point-choice function, punctum compared as well as valor.
- **Q3** "vertex probatio byte-identical" cannot see a wider window or a
  looser bound → T2 split into T2a/T2b, golden values captured first,
  `laqueus_pd_fenestra` public, TL limes = vertex limes test.
- **Q4** memory → what the cap counts stated; `LaqueusTemperleySumptus`
  with exact bytes and a boundary test; `piscina_conari_allocare`;
  tables built once; prime 4,294,967,029 in the tests.
- **Q5** tests and plants → added tests and plants above; plants
  registered as literal triples with named reds; inconclusive runs
  rerun; braid words embedded.
- **Q6** house constraints → no collisions; 1-based i tested; s32 for
  partners; i64 casts for products; glossary entries.

## Review (T4, 2026-10-31, recensor-temperley)

Verdict: approve; no H. Built from a scratch copy of 90e0b94d (both
suites pass there). Own oracle (recensor-d136's regular-representation
TL, composing all Catalan(n) diagrams directly; state sum; Burau):
- exact brackets of random knot closures, n = 2..10, c <= 120, through
  `laqueus_uncinus_ex_plexu`, the TL route and `laqueus_jones_ex_plexu`:
  290 of 290 (dispatcher chose vertex 44 times, TL 246);
- TL point values with the library's own punctum: 60 of 60 at 8 × 201
  .. 12 × 801, 13 × 302, 14 × 151 (primes 998244353, 2147483497;
  4294967029 at n <= 8);
- all 80 `temperley` modules, n <= 16, against the reviewer's own model
  (independent enumeration; action by gluing e_i onto half-diagrams, not
  rules (a)-(d)): 420,082 checks, 0 errors;
- T2a old vs new: be6be402's and HEAD's laqueus.c compiled side by side,
  12,965 table knots plus 150 braids, window, bound, vertex bracket and
  (status, punctum, valor) at 8 primes x 12 omegas: byte-identical
  (13,265 lines), independent of the golden capture;
- `laqueus_temperley_sumptus(...).octeti` against measured allocation:
  within 4-80 bytes of alignment at every n = 2..16.

Findings and where they went:
- **M1** the T3 claim "a cap cannot flip the choice to TL" was false
  (1,255 flips in 24,690 braids, n = 7..14) → plan T3 note and worklog
  corrected; the test comment corrected; the flip test restored with
  the reviewer's 7 × 32 braid (cap = TL bytes: TL chosen, bracket equal
  to the uncapped vertex bracket).
- **M2** the "pricing allocates nothing" assertion compared current
  arena usage, which reficere restores unconditionally → replaced by the
  apex of a FRESH piscina around pricing 12 × 801 (< 1% of its 557 MB
  workspace). The reviewer's plant A (pricing builds the workspace)
  now turns that assertion red (build/temperley/plantae_t4.py).
- **M3** no braid above 16 strands went through the dispatcher; plant B
  (TL eligible at zero cost above XVI) survived → σ1…σ16 on 17 strands
  added to 'via electa' (vertex, TL bytes 0) and 'per viam' (bracket
  A^48); plant B now red in both.
- **M4** the plan's CREDO_NON_RUIT / NON_PENDET wrapping of heavy
  sections was not done and not recorded → recorded deviation: credo
  runs the wrapped expression in a forked child whose checks do not
  return, so wrapping would run each heavy section twice; the harness
  already counts a crashed or summary-less run as inconclusive, never
  green.
- **L1** the 16-strand TL cap path → cap = TL bytes − 1 added (vertex).
- **L2** stabilization at 2 points → 3, as planned.
- **L3** writhe absent from the golden values → recorded: the window's
  class encodes it, the vertex Jones in 'tabula tota' covers it, and
  the reviewer's old/new comparison shows it identical on 12,965 knots.
- **L4** orphaned comment above `_quaestio_uncini` → merged.
- **L5** what the TL cap does not count (transient module peak ~1.2 MB
  at n = 16), and only the buffers are non-aborting → header says so;
  creare now rolls the piscina back on every refusal after pricing (it
  used to leave ~53 MB when the second buffer failed).
- **L6** surviving low-value plants (price constants swapped; saturation
  removed; modules not rolled back) and the untestable tie rule →
  accepted as recorded.
- **L7** "p = 17 pins g minimal" overstated (it pins the root i, i.e.
  its sign, not g) → wording corrected; A pinned at a second prime
  (998244353: A = 651909481).
- Header note added: `sumptus_vertex` may describe an order whose
  workspace exceeds the cap (vertex then ineligible).
