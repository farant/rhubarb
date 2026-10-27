# contractio, interpolatio and the vertex-model bracket: plan (phase 1)

Status: REVISED after design review (2026-10-27), awaiting Fran's
approval. The review (verdict "approve after fixes": H1–H4, M1–M10,
L1–L7) is at the end, with what each finding changed.

Agreed with Fran (2026-10-27), before this plan was written:
- two new house libraries, `contractio` (tensor networks and their
  contraction) and `interpolatio` (evaluate, interpolate, reconstruct),
  and the vertex-model bracket as a consumer inside `laqueus`, not in a
  knotapel demo;
- `contractio` v1 orders greedily; min-fill is a later task;
- the bracket is interpolated in A, at points where −i·A is a square
  mod p, not in the auxiliary variable ω.

## Purpose

Phase 1 of `project-specs/crossings-contraction-counting-brainstorm.md`
(§6 C1–C4 and C9, §8 phase 1): an exact contraction engine, validated on
knot invariants. On its own it removes `laqueus`'s 24-crossing ceiling
(`LAQUEUS_TRANSITUS_MAXIMI`, a 2^c state sum) for knotapel; later phases
use the same engine for #SAT, colorings and the crossing decompositions
of the October 2026 ETH papers.

## Governing rules

1. **Exact only.** Every result is exact or refused. No floating point
   anywhere. Reconstruction across primes is driven by a proven
   coefficient bound supplied by the caller, never by "the answer
   stopped changing". The extra prime and the extra points per prime
   are consistency checks: they catch arithmetic, bound and window
   faults, not a model that is wrong but self-consistent, which only an
   independent oracle catches (L3).
2. **Two tiers, one answer.** `contractio` executes the same network on
   a hot tier (`i32` residues modulo n) and a reference tier (any
   `anulus`). Tests accept a hot result only where it equals the
   reference result reduced mod n. The vertex-model bracket runs on the
   hot tier only: it needs i and ω, which `ANULUS_POLYNOMIORUM` cannot
   express (L4); its oracle is the state sum and the knot table.
3. **Structure separate from values.** A network and its contraction
   order are built once; values are supplied per execution. One
   network, one order and one workspace serve every point and prime.
4. **Bounded memory, stated in bytes before allocation.** The order
   lays out every intermediate in one slab (fixed offsets, by lifetime)
   and reports the slab's size in bytes. A workspace is allocated once
   from that size with `piscina_conari_allocare` and refused, not
   aborted, above the caller's ceiling. Execution then allocates
   nothing on the hot tier (H2). (Context: two fabrica processes at
   16 + 14 GB caused a kernel panic on 10-23.)
5. **Deterministic.** The greedy key is stated exactly (T2); the same
   network built the same way gives the same order, slab and output on
   every machine and optimization level.
6. **Nothing knot-specific in `contractio` or `interpolatio`.** The
   vertex model, turning numbers, PD conventions and braid-to-PD live
   in `laqueus`.
7. **Forward-compatible structs.** Every options or query struct comes
   with an initializer function returning defaults, and callers must
   start from it, so fields added later are never garbage in old
   callers (M1).

## API v1

Sketch for approval; the headers land in T1/T2 and become the record.
Identifiers to pass the Latin lint before landing (L7).

### interpolatio.h

```c
/* Lagrange mod p: n puncta distincta x (0 <= x < p, p primus), valores
 * y -> coefficientes c[0..n-1] (gradus < n). FALSUM si puncta non
 * distincta aut p non primus. */
b32
interpolatio_lagrange_modulo (
    constans i32* x,
    constans i32* y,
             i32  numerus,
             i32  p,
         Piscina* piscina,
             i32* coefficientes);

/* aestimator: pro indice k = 0, 1, 2, ... ELIGIT punctum x (x != 0) et
 * reddit f(x) mod p. Ita puncta sine radice quadrata enumerantur (H3). */
#define INTERPOLATIO_UTILE    I       /* punctum et valor scripti */
#define INTERPOLATIO_INUTILE  ZEPHYRUM /* hic index nullum punctum dat; proximum */
#define INTERPOLATIO_ERRATUM  (-I)    /* defectus verus: driver statim FALSUM */
nomen s32 (*InterpolatioAestimator) (
    vacuum* contextus,
       i32  index,
       i32  p,
       i32* punctum,
       i32* valor);

/* f(x) = sum_k c_k x^(imus + passus*k), k = 0 .. (summus - imus)/passus;
 * interpolatur g(y) = x^(-imus) f(x) in y = x^passus. */
nomen structura {
    s32 gradus_imus;          /* exponens minimus possibilis */
    s32 gradus_summus;        /* maximus; (summus - imus) % passus == 0 */
    i32 passus;               /* gradus exponentium (1 ordinarie; uncinus 4) */
    i32 limes_bitorum;        /* |c_k| < 2^limes_bitorum, PROBATUM a vocante */
    i32 residuum_primi;       /* primi p = residuum (mod modulus_primi) */
    i32 modulus_primi;        /* < 2 = omnes primi */
    i32 primus_limes;         /* primi < limes descendentes (ordinarie 2^32 - 1) */
    i32 puncta_addita;        /* puncta probationis super necessaria, per primum (1) */
    i32 conatus_maximi;       /* indices maximi per primum ante FALSUM */
} InterpolatioQuaestio;

InterpolatioQuaestio
interpolatio_quaestio_ordinaria (vacuum);   /* VOCANS SEMPER INDE INCIPIT */

/* Per primum: indices 0, 1, ... dum (summus - imus)/passus + 1 +
 * puncta_addita puncta utilia cum y distinctis (y duplex aut x == 0 =
 * inutile); Lagrange in y; puncta addita congruere debent. Primi
 * additi donec prod(p) > 2^(limes + 1), deinde UNUS ADDITUS qui
 * congruere debet; coefficientes per congruentia_restitue
 * (symmetricus). FALSUM si ERRATUM, conatus exhausti, aut probatio
 * discordat. Gradus 0 (imus == summus == 0) = restitutio scalaris
 * (e.g. numerus #SAT trans primos, phase 2). */
b32
interpolatio_polynomium (
         InterpolatioAestimator  aestimare,
                        vacuum*  contextus,
    constans InterpolatioQuaestio* quaestio,
                       Piscina*  piscina,
                    Polynomium*  exitus);
```

### contractio.h

```c
nomen structura ContractioRete    ContractioRete;     /* opacum */
nomen structura ContractioOrdo    ContractioOrdo;     /* opacum */
nomen structura ContractioOfficina ContractioOfficina; /* opacum */

ContractioRete*
contractio_rete_creare (Piscina* piscina);

/* index dimensionis d >= 1. FALSUM si d == 0 aut rete iam ordinatum. */
b32
contractio_index_addere (
    ContractioRete* rete,
               i32  dimensio,
               i32* id);

/* tensor super indices distinctos (ordo valorum: index ULTIMUS
 * celerrimus; numerus 0 = scalaris). FALSUM si index ignotus, index
 * bis in eodem tensore (v1: vocans vestigium ipse plicat, M5), index
 * iam duobus tensoribus usus, aut rete iam ordinatum. */
b32
contractio_tensor_addere (
     ContractioRete* rete,
      constans i32* indices,
               i32  numerus,
               i32* id);

/* VERUM sse quisque index in duobus tensoribus exacte */
b32
contractio_rete_clausum (
    constans ContractioRete* rete);

nomen structura {
    i32 methodus;             /* CONTRACTIO_AVIDUS (v1 solus) */
} ContractioOptiones;

#define CONTRACTIO_AVIDUS I

ContractioOptiones
contractio_optiones_ordinariae (vacuum);

/* sumptus: omnia i64 SATURANTUR ad CONTRACTIO_SATURATUM (0xFFFF...);
 * latitudo ex log2 dimensionum exacte, non ex apice saturato */
#define CONTRACTIO_SATURATUM ((i64)~(i64)ZEPHYRUM)
nomen structura {
    i64 apex;                 /* elementa tensoris intermedii maximi */
    i64 octeti_officinae;     /* tabula (slab) residuorum, octeti */
    i64 multiplicationes;
    i32 latitudo;             /* log2 apicis, sursum */
} ContractioSumptus;

/* ordo binarius deterministicus et dispositio tabulae (offsets per
 * vitam, primum aptum). Rete post hoc immutabile. NIHIL si rete non
 * clausum (v1). Rete vacuum: ordo trivialis, valor 1. */
ContractioOrdo*
contractio_ordinare (
         ContractioRete* rete,
    constans ContractioOptiones* optiones,
               Piscina* piscina);

ContractioSumptus
contractio_sumptus (
    constans ContractioOrdo* ordo);

/* officina tieri calidi: UNA allocatio (piscina_conari_allocare) magnitudinis
 * octeti_officinae; NIHIL si > octeti_maximi aut allocatio deficit.
 * Reutilis per omnia puncta et primos. */
ContractioOfficina*
contractio_officina_creare (
    constans ContractioOrdo* ordo,
                       i64  octeti_maximi,
                   Piscina* piscina);

/* valores[k] = elementa tensoris k, 0 <= v < modulus (praesumptum, non
 * probatum in ansa calida; tests probant), 2 <= modulus < 2^32.
 * exitus = tensor densus super indices APERTOS ordine id (v1: unum
 * elementum). Nulla allocatio. FALSUM si ordo non huius retis aut
 * officina non huius ordinis. */
b32
contractio_exsequi_modulo (
    constans ContractioRete*  rete,
    constans ContractioOrdo*  ordo,
       ContractioOfficina*    officina,
     constans i32* constans*  valores,
                       i32   modulus,
                       i32*  exitus);

/* tier referentiae: piscina temporaria per gradum (notare/reficere),
 * effectus per anulus->transcribe in piscinam exitus. Ad retia parva:
 * nullus limes octetorum (elementa anuli memoriam externam habent). */
b32
contractio_exsequi_anulo (
    constans ContractioRete*   rete,
    constans ContractioOrdo*   ordo,
    constans vacuum* constans* valores,
          constans Anulus*    anulus,
                   Piscina*   piscina,
                    vacuum*   exitus);
```

**Hot-tier kernel (M3).** Each output entry sums products a·b with
a, b < n < 2^32. One product can reach 2^64 − 2^34 + 4, so two products
already overflow `i64`. The kernel accumulates into a two-word
accumulator (low word plus carry count: one add and one compare per
term, exact for any length) and reduces once per output entry:
(carry·(2^64 mod n) + low mod n) mod n. Any modulus 2 ≤ n < 2^32 is
then safe, so the prime limit stays 2^32 − 1.

### laqueus.h (additions)

```c
/* uncinus Kauffman ex codice PD per modulum vertex et contractionem:
 * nullus limes transituum praeter octeti_maximi (officina). Validatio
 * eadem ac laqueus_uncinus_ex_pd (c == 0 -> 1; c == 1 refutatur, L5).
 * sumptus NIHIL licet. Tier calidus solus. */
b32
laqueus_uncinus_ex_pd_rete (
         constans i32* pd,
                  i32  transitus,
                  i64  octeti_maximi,
              Piscina* piscina,
          Polynomium* exitus,
    ContractioSumptus* sumptus);

b32
laqueus_jones_ex_pd_rete (... idem ...);

/* codex PD clausurae tricae (plexus): verbum in generatoribus sigma_i^+-1,
 * n fila; FALSUM si clausura non nodus unus (M9) */
b32
laqueus_pd_ex_plexu (... T5 ...);
```

## The vertex model (T4)

Prototype: `scratchpad/vertex/vm.py` (2026-10-27), equal to the state
sum on all 35 knots up to 8 crossings (70/70); the reviewer's port with
greedy contraction matched all 12,965 non-trivial table knots at two
points each (25,930/25,930) and 135 kinked codes against the state sum.

- **Index per PD edge, dimension 2.** Spin ±1 = the direction a loop
  runs through the edge, relative to dart a (its first occurrence in
  the PD code) toward dart b (its second).
- **Kinks (M5 choice b).** An edge whose two darts lie on the same
  crossing is not an index: the builder folds it into that crossing's
  tensor, summing its spin there. `contractio` v1 never sees a repeated
  index. Verified on 38 kinked codes (reviewer's `rete.py`).
- **Turning numbers t_e** (quarter turns, direction a → b), from the
  rotation system `_pd_planus` walks: faces are orbits of "other end,
  then next position counterclockwise"; face 0 is the orbit of dart
  (crossing 0, arm 0) and is the outer face (M10.5). Base value
  t_e ≡ (j_b − j_a + 2) mod 4; t_e = base + 4·m_e, with integers m_e
  solving one equation per face: the walk turns +4 quarters for face 0
  and −4 for every other, each corner contributing −1. A BFS spanning
  tree of the dual graph from face 0 solves it, leaves first; non-tree
  edges take m = 0. t_e is signed; |t_e| reaches 31 at c ≤ 13 and grows
  with c (M10.6).
- **Crossing tensor:** A·[smoothing (X0 X1)(X2 X3)] + A⁻¹·[smoothing
  (X0 X3)(X1 X2)] (the conventions of `_uncinus_ex_oris`). Each pair
  contributes 0 unless one end enters and the other leaves; otherwise
  ω^(−1) when turning from arm p to p+1 and ω^(+1) from p to p−1. Edge
  factors ω^(σ·t_e) are folded into the tensor at dart b.
- **Points (H3).** The aestimator receives an index k, sets ω = k + 2,
  A = i·ω² with i = g^((p−1)/4) (`congruentia_potentia`, g a quadratic
  non-residue found once per prime), and returns punctum A with the
  value. These are exactly the points where −i·A is a square: the
  agreed point set, enumerated instead of filtered, with no square
  root needed. ±ω give the same A; the driver drops duplicate y.
- **Window and stride (M1).** All A-exponents of ⟨D⟩ are ≡ 3w (mod 4)
  in the reviewer's check (12,965/12,965); T4 asserts it on the
  oracle side. Kauffman's bound gives the window [−c − 2|s_B| + 2,
  c + 2|s_A| − 2], where |s_A|, |s_B| are the loop counts of the all-A
  and all-B states (two union-find passes). With passus 4 and the
  window rounded to the 3w class, at most c + 1 points per prime are
  needed instead of 6c + 1 (5.9× fewer over the table).
- **d = 0 (M2).** At A⁴ = −1, i.e. y = −1, d vanishes and d·⟨D⟩/d is
  undefined: the aestimator returns INUTILE there. Tested with p = 17,
  A = 2.
- **Bounds.** |coefficients of ⟨D⟩| < 4^c strictly (Σ over states of
  2^(loops − 1) ≤ 2^c · 2^c), so `limes_bitorum` = 2c (L1).
- **Jones:** (−A³)^(−w)·⟨D⟩ with the PD writhe, then the existing
  A = t^(−1/4) step (`_jones_ex_uncino`).

### Why it is correct (M10)

1. **Every face equation has an integral right-hand side.** A face walk
   steps from arm j_i along an edge to arm j'_i, then turns to arm
   j_(i+1) = j'_i + 1 (corner −1). The base values plus corners sum to
   Σ (j'_i − j_i + 2 − 1) = Σ (j_(i+1) − j_i), which telescopes to
   0 (mod 4) around the closed walk. The target ±4 is also ≡ 0, so each
   face's equation in the m_e has a right-hand side divisible by 4, and
   the m_e are integers.
2. **The sign is forced.** Summing all face equations, each edge is
   walked once in each direction and cancels, leaving minus the corner
   count: Σ targets = −4c. That holds for +4 outer / −4 inner
   (4 − 4(c + 1) = −4c) and fails for the opposite (reviewer: 799/799).
   The prototype's sign retry is dead code and is not ported.
3. **A solution exists.** The dual graph is connected, with ±1
   incidences, and its supplies sum to zero; a spanning tree solves it.
   There are no dual self-loops: a 4-regular plane graph is Eulerian,
   hence bridgeless, so no edge has the same face on both sides.
4. **The model computes the bracket.** Integer turnings satisfying the
   face sums form an orthogonal representation of the plane graph,
   realizable as an actual drawing (Tamassia 1987). In every state each
   loop is then a simple closed curve whose turning is ±4 quarters
   (Hopf's Umlaufsatz), so summing its spin gives ω⁴ + ω⁻⁴ = −A² −
   A⁻² = d per loop, and the full contraction is d·⟨D⟩. Checked
   independently of the solver: 6,344 random states, every loop ±4.

## Tasks

Each task follows the house cycle: implement; tests red, then green;
planted faults (`silva.planta`); worklog; commit via `silva.commissio`
(owed gates: at least `radix`, `generata`, and `silva` for each new
`lib/*.c`); independent review where the task is load-bearing; stop for
Fran's go-ahead.

- **T0. This plan.** Design review done (below); Fran's approval.
- **T1. interpolatio.** `interpolatio_lagrange_modulo`,
  `interpolatio_quaestio_ordinaria`, `interpolatio_polynomium`.
  Credo: random polynomials mod p (degree 0–200) recovered exactly;
  Laurent shifts (imus < 0) and passus 4; a fixed integer polynomial
  with coefficients near 2^200 recovered with the bound exactly at the
  edge; an aestimator lying at one prime → FALSUM via the extra prime,
  lying at one point → FALSUM via `puncta_addita`; ERRATUM on the third
  call → FALSUM after exactly three calls; always INUTILE → FALSUM at
  `conatus_maximi`; duplicate y dropped; prime filter honored
  (p ≡ 1 mod 4), including a small `primus_limes` (p = 17); degree 0 as
  scalar reconstruction.
  *(2026-10-27: landed. API as approved; 11 credo sections, 15,918
  checks in 0.1 s against known random Laurent polynomials evaluated in
  the test. The point-lie test was first one-prime-only, which the extra
  prime also catches; it is now constant across primes, so the
  extra-point check is tested on its own. 9 plants red. Glossary:
  interpolatio, lagrange. See lib/interpolatio.worklog.md.)*
  *(2026-10-27, independent review of e8a13213: no H; M1-M5 fixed —
  puncta_addita wrap, x = 0 restored to INUTILE as planned, a test at
  the margin of the prime bound (slack-0 and ceil(log2) plants used to
  survive), residue filter stepping by the modulus with inadmissible
  filters refused, windows validated up front and output sized from
  its nonzero span — plus L1, L2, L4, N2. 17 sections, 15,997 checks;
  15 plants red.)*
- **T2. contractio structure, greedy order and slab.** Header with the
  whole v1 API (execution refuses until T3). Greedy key, exactly: among
  pairs of live tensors sharing at least one index, minimize (result
  entries, saturating; then smaller tensor id; then larger tensor id);
  the result takes the next id. Slab: first-fit offsets by lifetime,
  deterministic.
  Credo: hand-built networks (chain, ring, 2×2 grid, star) with
  hand-computed orders, costs and slab sizes; disconnected components;
  empty network and zero-index tensors; renumbering index ids leaves
  order and cost unchanged; the same insertion order gives an identical
  order and slab (insertion order may change cost: not claimed
  invariant, M7); apex and multiplications against an independent count
  in the test; saturation at a network whose apex exceeds 2^64;
  refusals (repeated index, third use of an index, mutation after
  ordinare, open network).
- **T3. contractio execution.** Hot tier with the two-word kernel and
  the officina; reference tier with per-step scratch.
  Credo: small networks against brute-force summation over all index
  assignments; random networks over Z/p, Z and Z[t, t⁻¹], hot equal to
  reference reduced mod p; n = 4294967291 and n = 0xFFFFFFFF with every
  value n − 1 and a shared dimension ≥ 2^10 (plant: drop the carry);
  officina refused above `octeti_maximi` and reused across 100
  executions with no growth of the caller's arena; ordo/officina
  mismatch refused; dimension-3 indices.
- **T4. Vertex-model bracket in laqueus.** Turning numbers, network
  builder (kink folding), `laqueus_uncinus_ex_pd_rete`,
  `laqueus_jones_ex_pd_rete`.
  Credo:
  - equal to `laqueus_uncinus_ex_pd` (state sum) on every table knot up
    to 10 crossings;
  - Jones equal to `tabula_nodorum`'s stored value on all 12,966 knots
    (independent oracle: state sum matched to KnotInfo; only
    `_pd_legere`/`_pd_planus` shared), one time-bounded section;
  - kinks and traces (H4): the 2-crossing unknots
    `[[1,1,2,4],[2,4,3,3]]` and `[[4,1,1,2],[2,4,3,3]]`; trefoil plus a
    stabilization `[[1,7,2,6],[7,3,8,2],[3,1,4,8],[4,6,5,5]]` and its
    negative twin; table knots with random R1/R2 insertions, Jones equal
    to the table (and `laqueus_pd_simplificare` returning them to the
    original crossing count where it can); `diagramma_pd` of random
    polygonal knots;
  - every loop of random states turns ±4 (independent of the solver,
    M8); exponents ≡ 3w (mod 4) on the state-sum side;
  - d = 0 point (p = 17, A = 2) returned INUTILE;
  - invalid and non-planar codes refused as before; c = 0 → 1.
- **T5. Measurement and braids.** `laqueus_pd_ex_plexu` (braid closure
  to PD; even strand counts need odd word length for one component).
  Timings and costs against the state sum at 12, 16, 20, 24 crossings
  (via `mensura`); the greedy-width curve on table knots; braid closures
  above 24 crossings with an exact oracle: a table knot's braid word
  plus random conjugations, stabilizations and R2 insertions must give
  the table's Jones. Results to the brainstorm's §11; a follow-on plan
  for min-fill and slicing if greedy width bites (the reviewer measured
  ~2n for n-strand braids: 12 strands, 801 crossings, 2^26–2^28
  elements, 1–2 GiB live).

## Not in v1

Min-fill ordering, slicing, open (output) indices (the `exitus` layout
is fixed now so the signature never changes), repeated indices inside a
tensor, hyperindices without copy tensors, block-sparse sectors (phase
3), the eight-square and singlet decompositions (phase 3), batched
points, floating point.

## Risks

| Risk | Mitigation |
| --- | --- |
| Greedy order poor on braid closures | T5 measures it; min-fill and slicing as a follow-on; state sum stays as oracle up to 24 |
| Interpolation bound or window wrong | Bound proved above; window asserted on the oracle side in T4; extra prime and extra points catch a bad bound as a mismatch |
| Turning numbers wrong on some PD shape | Correctness argument above; loops-turn-±4 check on random states; all table knots; kinked codes by hand and by random R1/R2 |
| Hot-tier overflow | Two-word accumulator specified; T3 tests at n near 2^32 with every value n − 1; plant drops the carry |
| Memory blow-up | One slab in bytes, refused before allocation; no allocation during execution; officina reused |

## Design review (2026-10-27): findings and changes

Report: `scratchpad/recensio_contractio/report.md` (session of
2026-10-27), with the reviewer's scripts. Verdict: approve after fixes.

| Finding | Change in this plan |
| --- | --- |
| H1 aestimator FALSUM ambiguous; driver could scan ~2^32 points | Three-state return (UTILE / INUTILE / ERRATUM) and `conatus_maximi`; T1 tests both |
| H2 memory rule unachievable (arena sums all intermediates; allocation aborts) | Slab layout at `ordinare`, size in bytes, `ContractioOfficina` allocated once with `piscina_conari_allocare`; reference tier on per-step scratch |
| H3 no modular square root in the house | Aestimator chooses its point: ω = k + 2, A = i·ω²; same point set, no root |
| H4 kink/trace path untested | T4 credo: hand kink codes, stabilized trefoil, random R1/R2 insertions, polygonal knots |
| M1 6× fewer points via stride and Kauffman window | `passus`, window from |s_A|, |s_B|; struct initializer so new fields are safe |
| M2 d = 0 at A⁴ = −1 | INUTILE there; test p = 17, A = 2 |
| M3 i64 overflow after two products | Two-word accumulator kernel; tests at n near 2^32 |
| M4 contracts (ids, empty/open networks, mutation, saturation) | `b32` + id out-params; frozen after `ordinare`; NIHIL on open; empty = 1; saturating counters; `latitudo` from exact logs |
| M5 trace semantics contradictory | v1 refuses repeated indices; `laqueus` folds kink edges |
| M6 forward compatibility | `exitus` = dense tensor over open indices (one element in v1); `ContractioOptiones`; degree-0 interpolation as scalar CRT |
| M7 T2 invariance claim wrong for insertion order | Greedy key stated exactly; invariance claimed for index renumbering only |
| M8 face-sum check circular | Loops-turn-±4 on random states instead |
| M9 braid to PD missing | `laqueus_pd_ex_plexu` in T5 |
| M10 proofs unwritten | "Why it is correct" above |
| L1–L7 | Bound 2c bits; `modulus_primi < 2`; extra checks' scope stated; hot tier only for the bracket; c = 0 / c = 1; inputs < n presumed, tested; Latin lint before headers |
