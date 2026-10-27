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
  *(2026-10-27: landed. API as planned plus three read-only inspection
  calls (`contractio_ordo_gradus_numerus`, `contractio_ordo_gradus`,
  `contractio_ordo_sedes`). Officina = slab only; refused when the slab
  or the multiplication count saturates, which also bounds every step
  to ≤ 63 nontrivial indices, so T3's odometer fits a stack array.
  Outer-product fallback stated in the key (all pairs, same key).
  12 credo sections, 89,235 checks, 0.04 s; 300 random networks
  against an independent count; 19 plants red. Glossary: tensor. See
  lib/contractio.worklog.md.)*
  *(2026-10-27, independent review of c1453413: no H; 19,000-network
  differential fuzz with 0 mismatches. Fixed: officina capped at 2^62
  bytes (piscina wraps near 2^64 — filed separately), O(1) scalar tail
  in the outer-product fallback, new `contractio_ordo_indices` so the
  sorted intermediate index order is tested, header wording. 13
  sections, 106,758 checks; 4 more plants red.)*
- **T3. contractio execution.** Hot tier with the two-word kernel and
  the officina; reference tier with per-step scratch.
  Credo: small networks against brute-force summation over all index
  assignments; random networks over Z/p, Z and Z[t, t⁻¹], hot equal to
  reference reduced mod p; n = 4294967291 and n = 0xFFFFFFFF with every
  value n − 1 and a shared dimension ≥ 2^10 (plant: drop the carry);
  officina refused above `octeti_maximi` and reused across 100
  executions with no growth of the caller's arena; ordo/officina
  mismatch refused; dimension-3 indices.
  From the T2 review (must hold): odometer and strides over dimension
  ≥ 2 indices only — the ≤ 63 bound from unsaturated multiplications
  counts only those, and dimension-1 indices are unbounded in number
  (test: ≥ 64 shared dimension-1 indices in one step); the reference
  tier has no officina, so it refuses saturated multiplications or apex
  itself and sizes per-step scratch with overflow checks and
  `piscina_conari_allocare` (test on T2's saturation networks); n = 1
  → `valores[0][0]`, n = 0 → 1; strides per operand (inputs keep the
  caller's order, intermediates are sorted); check `officina->ordo ==
  ordo` and `ordo->rete == rete`.
  *(2026-10-27: landed. Loop plan per step built at `ordinare`
  (dimension ≥ 2 indices only, strides per operand); hot tier with the
  two-word accumulator and a tight inner loop, no allocation; reference
  tier over any anulus with two private arenas. 21 sections, 118,633
  checks: brute force on 200 networks at five moduli up to 0xFFFFFFFF,
  Z and Z[t, t⁻¹] against the hot tier, every value n − 1, a
  constructed near-2^64 reduction (plant "low word unreduced" survived
  random data), 100 reuses with no arena growth. 15 of 16 plants red;
  the survivor is a deliberately redundant guard. See
  lib/contractio.worklog.md.)*
  *(2026-10-27, independent review of 21c44e3d: no H, no correctness
  M; 62,500-network sanitized fuzz against brute force and an
  evaluator independent of the loop plan, 0 mismatches. Fixed: the
  reference tier caps each intermediate at 2^62 bytes, random tests
  widened to dims 1–5 with all-n−1 networks, n = 1 reduced mod n.
  Kernel speedup (~1.65×, offset tables) deferred to T5.)*
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
  *(2026-10-27: landed. API as planned plus an opaque reusable
  `LaqueusModulusVertex` (`_creare`, `_aestimare(ω, p)`, `_gyrationes`,
  `_sumptus`): the d = 0 check and the loop check need a single point
  and the turning numbers. Crossing tensors stored symbolically
  (A^±1·ω^e terms, kinks summed at build). 9 sections, 55,960 checks,
  2.0 s: all 12,966 table Jones, all 249 knots ≤ 10 against the state
  sum, R1 kinks, 28-crossing knots (state sum refuses), random polygons,
  loops ±4, single points, d = 0. Random R2 insertions not written —
  bigons covered by table and polygon diagrams. 17 plants red.
  Glossary: gyratio. See lib/laqueus.worklog.md.)*
  *(2026-10-27, independent review of 62a02ef5: no H; exhaustive c = 2,
  1.2M random codes, transforms and connected sums to c = 1606 all
  agree. Fixed: composite p now ERRATUM (was a ~p-step search or a
  false accept); connected sums of up to 12 mirrored, kinked, permuted
  knots (c ≥ 100, coefficients > 31 bits) with Jones = product;
  monogon face 0. 10 sections, 56,344 checks.)*
- **T5. Measurement and braids.** First, from the T3 review: the hot kernel
  spends its time in the per-entry odometer (interni loops of length 2
  on braid networks, 1.3–1.8 ns per multiplication); a prototype with
  per-step offset tables over the interni assignments, built at
  `ordinare` and capped with the odometer as fallback, measured ~1.65×
  faster with identical results — measure it in the mensura harness
  and adopt it if it holds. From the T4 review: the number of
  contractions per bracket grows as c² ((window/4 + 3) points × (2c/31
  + 2) primes ≈ c²/16; 8.1 s at c = 803 and 68 s at c = 1606 with apex
  256, hours at braid apex 2^26–2^28) — consider batching several points
  per contraction (a vector of values per tensor entry) and a tighter
  provable coefficient bound (the 2c-bit bound is 2–3× loose); and the
  interpolation scratch grows superquadratically in the caller's arena
  (78 MB at c = 1606) — give it its own arena. `laqueus_pd_ex_plexu` (braid closure
  to PD; even strand counts need odd word length for one component).
  Timings and costs against the state sum at 12, 16, 20, 24 crossings
  (via `mensura`); the greedy-width curve on table knots; braid closures
  above 24 crossings with an exact oracle: a table knot's braid word
  plus random conjugations, stabilizations and R2 insertions must give
  the table's Jones. Results to the brainstorm's §11; a follow-on plan
  for min-fill and slicing if greedy width bites (the reviewer measured
  ~2n for n-strand braids: 12 strands, 801 crossings, 2^26–2^28
  elements, 1–2 GiB live).
  *(2026-10-27, measurement first (Fran): `tools/uncinus_mensura.c` —
  state sum vs network at c = 12–24 (4,600× at 24, all equal), greedy
  width over the whole table (max 8), connected sums to c = 800 (8.6 s,
  40,651 contractions ≈ c²/16, 2.1 ns/mult), random braid closures to
  12 × 201 (10 × 201: apex 2^20, 2.1 s per point, ~77 min projected);
  the offset-table kernel gains 1.1–2.1×, least at large apex. Results
  and the ranked levers (order width, fewer primes via a tighter bound
  or 62-bit primes, several points per sweep, kernel tables) are in
  the brainstorm §11. Braid closure → PD lives in the tool for now
  (checked against the state sum); `laqueus_pd_ex_plexu` still to do.
  Long runs go through the new `tools/custos.sh` after a ~120 GB
  runaway in the tool's first braid generator.)*

## T5a. Fewer primes: spanning-tree coefficient bound (proposed 2026-10-27; revised after design review)

**Why.** Measured (T5): the bracket's coefficients use far fewer bits than the 2c-bit bound pays for — 25–48 bits against 402 on 201-crossing braids, 429 against 1,600 on c = 800 connected sums. Contractions ≈ points × primes, and primes ≈ limes/31 + 2, so the bound is a direct factor of the running time. Stopping early when the answer "looks stable" is excluded by the exactness rule; the bound must be proved.

**Claim.** Let G be either checkerboard (Tait) graph of a connected diagram D: the regions of one colour as vertices, one edge per crossing joining its two corners of that colour (a loop if they are the same region). Then for the bracket in the house normalization (⟨D⟩ = Σ_states A^(#A − #B) d^(loops − 1), d = −A² − A⁻², ⟨O⟩ = 1):

Σₖ |cₖ| ≤ τ(G) ≤ B(G) := Π_{v ≠ r} deg(v),

τ = number of spanning trees, deg = number of non-loop edges at v (multi-edges counted with multiplicity), r any vertex (take one of maximal degree). The bound used is limes = bits(min(B(G), B(G*))), and |cₖ| ≤ min B < 2^limes is exactly interpolatio's contract |cₖ| < 2^limes_bitorum. (If the aestimator ever returned d·⟨D⟩ instead of ⟨D⟩, add one bit.)

**Proof.**
0. *States are spanning subgraphs.* The A-smoothing (X0 X1)(X2 X3) of crossing k always merges the regions at corners (k, 1) and (k, 3); B merges (k, 0) and (k, 2). So for the colour whose corners at k are {1, 3}, A = contracting edge k and B = deleting it; for the other colour the roles swap. Which one is A depends on which corner pair the colour occupies, **not on the crossing's writhe sign**, and the argument below works for any ±1 per edge — crossing signs, mirroring and non-alternation are irrelevant. A state is thus a spanning subgraph H of G (the contracted edges), and its loop count is k(H) + n(H), where k = components and n(H) = |E(H)| − |V| + k(H) (the boundary circles of a neighbourhood of H in the sphere, by Euler per component). So ⟨D⟩ = Σ_H A^(·) d^(k(H) + n(H) − 1) is a function of the graph alone.
1. *Deletion–contraction.* For any edge e: ⟨G⟩ = A^(±1)⟨G − e⟩ + A^(∓1)⟨G / e⟩.
2. *Bridges and loops collapse.* If e is a bridge, every spanning subgraph of G − e has one more component, hence one more state circle: ⟨G − e⟩ = d·⟨G / e⟩, and A^(∓x)·d + A^(±x) = −A^(∓3x), a monomial with coefficient −1. Dually for a loop (A^(±x) + A^(∓x)·d = −A^(±3x)).
3. *Leaves are spanning trees.* Recurse on non-bridge, non-loop edges (deleting a non-bridge keeps G connected). Each leaf is a single vertex, ⟨·⟩ = 1, contributing one ±Aᵏ. The leaf count L satisfies L(G) = L(G − e) + L(G / e) for e neither bridge nor loop, L(G) = L(G / e) for a bridge, L(G) = L(G − e) for a loop, L(one vertex) = 1 — the identities of τ — so L = τ(G) whatever the recursion order. Cut vertices (connected sums) and multi-edges need no special case; a kink gives a loop in one colour and a pendant bridge in the other, both covered.
4. *τ(G) ≤ B(G).* Map each spanning tree to the edge each non-root vertex uses toward r: injective, choices are non-loop edges counted with multiplicity, any root works. deg = 0 only when |V| = 1 (empty product, B = 1).
5. *The min over the two colours needs no duality:* the argument holds for each colour separately. τ(G) = τ(G*) (plane duals) is a cross-check only.
6. *Never worse than 2c:* the degrees of one colour sum to ≤ 2c, and positive integers with sum S have product ≤ 3^(S/3), so log₂ B ≤ 1.057c and bits(B) ≤ 2c for c ≥ 2.

**Checked.** Python (scratch, mine): Σ|cₖ| ≤ τ for all 12,965 nontrivial table knots, equality for exactly 6,729 = the prime alternating knots up to 13 crossings (alternating: Σ|cₖ| of Jones = det = τ); 36 random braid closures (4–8 strands × 51–201): bits(τ) ≈ 0.6c, actual ≈ 0.2c. Design review (independent): a signed deletion–contraction asserting each collapse is ±Aᵏ, matched to an exact state sum and with leaf count = Kirchhoff τ, on every valid c = 2 (64) and c = 3 (2,016) code — including 512 diagrams with a one-vertex colour class — all table knots ≤ 9 and mirrors ≤ 10, kinked and relabelled knots, 400 braid closures, 150 connected sums: 3,411 diagrams, 0 violations; larger kinked braids (c 31–106) and mixed sums (c 91–160): bits(Σ|c|) ≤ bits(τ) ≤ bits(B), max bits(B)/c = 0.84.

| case | 2c | B (min over colours) | τ | actual |
| --- | --- | --- | --- | --- |
| table, c = 13 (mean) | 26 | 10.2 | 8.8 | 5.2 |
| braid 8 × 201 | 402 | 158–168 | 129–134 | 44–45 |
| braid 4 × 201 | 402 | 114–130 | 103–116 | 25–31 |

Expected effect: ~2–2.5× fewer contractions on braid closures, ~2.4× on mixed connected sums, none at table sizes (one prime already suffices). Exact τ (a determinant, O(V³)) would gain about one prime in seven over B; not in v1.

**Design.**
- *Colour classes by union-find, not a parity rule* (review M1). Regions come from the face numbering `_gyrationes` already computes: corner (k, j) = the region between arms j and j + 1 = `faciei[4k + (j + 1) % 4]`. Join corner (k, 0) with (k, 2) and (k, 1) with (k, 3) over all crossings: both Tait graphs are connected, so there are exactly two classes, and they are the colours. Refuse (NIHIL) unless there are exactly two classes and each crossing's adjacent corners lie in different classes. Degrees need no colour: per crossing, if corner (k, 0) ≠ corner (k, 2) add one to each, likewise (k, 1)/(k, 3). Rationale: the first Python version assumed "corners 0, 2 are always one colour" — wrong for non-alternating diagrams — and simulated over the table, that bug turns only 2 tests red with a bits-only check; union-find removes the bug class.
- B(G), B(G*) as `Magnus`, products batched in a u64 accumulator (< 2^32 × degree ≤ 2c < 2^31) and multiplied into the Magnus only on flush (review L1: one Magnus allocation per factor would cost ~c²/16 bytes of arena). limes = bits(min). Stored in the structure.
- `_uncinus_per_rete` passes limes instead of 2c. Nothing else changes.
- Read-only accessors: `laqueus_modulus_vertex_limes(mv)` (bits) and `laqueus_modulus_vertex_limes_exactus(mv, &B)` (the Magnus min product), for tests and `tools/uncinus_mensura`. A counter of aestimare calls and of distinct primes in the structure (`laqueus_modulus_vertex_vocationes`), so a test can see the bound actually wired in (review M4).
- Optional, not v1: prune degree-1 vertices before the product (pendant vertices contribute 1 to τ; kink chains give τ = 1 but B = 2) (L3).

**Tests.**
- All existing `probatio_laqueus_rete` sections green with the smaller bound.
- Σ|cₖ| ≤ B (exact Magnus, not just bits) on every table knot, kinked codes, connected sums, random braid closures; limes ≤ 2c always; limes < 2c for c ≥ 3.
- Exact hand values (review M3), including tight cases where Σ|c| = B = 2^limes − 1, so that "limes − 1" goes red without the whole table:

| diagram | B | limes | Σ\|c\| |
| --- | --- | --- | --- |
| closure of σ₁³¹, T(2, 31) | 31 | 5 | 31 |
| T(2, 7) # T(2, −9) (non-alternating composite, max\|c\| = 7) | 63 | 6 | 63 |
| kinked unknot [[1,1,2,4],[2,4,3,3]] | 1 | 1 | 1 |
| 3_1 / 4_1 / 5_1 / 5_2 / 7_1 | 3 / 6 / 5 / 8 / 7 | 2 / 3 / 3 / 4 / 3 | |
| 3_1 # 3_1 / stabilized trefoil | 9 / 3 | 4 / 2 | |

  Exact limes values also catch "looser" regressions that would quietly lose the speedup.
- Wiring (M4): on a braid closure, the number of primes used matches limes (≈ limes/31 + 2), and is below what 2c would require.
- Plants — only faults that make the bound *too small* should go red (review M2; simulated red counts over the table via the Σ ≤ B test): limes − 1 (441), ⌊log₂ B⌋ instead of bit length (441), multi-edges deduplicated (2,999), each edge counted at one endpoint only (12,811), two vertices excluded per class (5,556), "corners j and j + 1 same colour" (red via the two-class refusal), corners 0/2 forced into one colour (the original Python bug). Not plants (equivalent or looser, stay green): swapping the colours (the min makes it equivalent), loops counted in degrees, root not excluded, one colour only. Note the existing Jones/CRT tests alone barely see a too-small bound at table size (one 31-bit prime gives slack: limes − 2 turns 1 test red) — the Σ ≤ B test is the real detector.
- `tools/uncinus_mensura` re-run: primes and contractions before/after on sections III–IV.

*(2026-10-27: landed. As designed, plus `laqueus_modulus_vertex_uncinus` (the driver on a held network) so the wiring test can count primes. 13 sections, 149,220 checks; reviewer's hand values exact, including the tight T(2,31) and T(2,7) # T(2,−9); 6 plants red. Measured with the same seeds: contractions and primes ~2.1–2.4× fewer — c = 800 sums 8.6 → 3.6 s (like-for-like after the review), braids 8 × 201 43 → 21 s, 10 × 201 projected 77 → 34 min. See lib/laqueus.worklog.md.)*

*(2026-10-27, independent review of 7a2a8deb: no H, no M; exact-integer fuzz of B against Python on ~6,000 diagrams, 0 mismatches. Fixed: dead state-sum branch in the braid section, contraction counter exact, tool measures like-for-like.)*

**Design review (2026-10-27): approve after fixes, no H.** M1 colouring by union-find (above); M2 plant list corrected; M3 exact hand values with tight cases; M4 wiring test; L1 batched Magnus product; L2 reuse `faciei`; L3 optional pruning; L4 connected-sum gain ~2.4×, not 3×; L5 min needs no duality. Proof text completed: states as spanning subgraphs with loop count k + n, exponent from the corner pair rather than the writhe, bridge wording ("one more component"), explicit base identities. Review scripts: scratchpad/recensio_t5a/ (session of 2026-10-27).

## T5b. Better contraction orders: best of several, chosen by exact cost (proposed 2026-10-27; revised after design review)

**Why.** Time per point is set by the *step rank* of the order (largest union of one contraction step; a step costs ~2^step multiplications), and the T2 greedy key minimizes only each step's *result*. The design review converted min-fill elimination orders into pairwise schedules (Python, scratchpad/recensio_t5b/) and measured actual costs:
- step rank of the converted schedule = min-fill width + 1, exactly, on every schedule tried (and an upper bound in general: each live tensor's open indices form a clique in the filled graph);
- over 20 braid closures (8/10/12/16 strands × 201 crossings, 5 seeds each), AVIDUS's total multiplications average **3.35 bits (~10×) above the best order found**, maximum 6.9 bits (~120×); single braids vary widely (one 10 × 201: 25 vs 24 step rank; 16 × 401: 30× gain; 12 × 801: all candidates within 0.7 bits);
- converted min-fill orders usually have *smaller* intermediates too (10 × 201 and 12 × 201: result rank 16 vs 20; slab 2^22.7 → 2^20.0 bytes). The earlier reading "memory ~2n is structural" was an artefact: the experiment printed min-fill's width |N(v)| as its result rank, where the result is |A Δ B| ≤ width − 1. Worst slab increase seen for a winner: +7% (12 × 801).
- Table knots and connected sums: nothing to gain (greedy is already at min-fill's step rank).

Ordering is milliseconds against seconds-to-hours of contraction, and an order's multiplication count is exact before anything runs, so several candidates can be tried and the cheapest kept.

**Decision (Fran, 2026-10-27; tabularium 01M5ZGH8…):** AVIDUS stays the library default (identical answers either way — only time and memory differ; AVIDUS is hand-checkable and renumbering-invariant, and T2's tests depend on it). OPTIMUS is opt-in; laqueus is the first user. Flipping the default is a later, separately reviewed step if measurements show OPTIMUS winning broadly without memory surprises.

**API** (forward-compatible, rule 7):

```c
#define CONTRACTIO_AVIDUS  I    /* clavis T2 (ordinarius) */
#define CONTRACTIO_OPTIMUS II   /* plures ordines, vilissimus per multiplicationes */

/* candidati OPTIMUS, ordine fixo */
#define CONTRACTIO_CANDIDATUS_OMNES     ZEPHYRUM
#define CONTRACTIO_CANDIDATUS_AVIDUS    I    /* clavis T2 */
#define CONTRACTIO_CANDIDATUS_REDUCTIO  II   /* clavis 2^r - 2^a - 2^b */
#define CONTRACTIO_CANDIDATUS_IMPLETIO  III  /* min-fill, nexus per (gradus, id) */
#define CONTRACTIO_CANDIDATUS_FORTUITUS IV   /* + k: min-fill, nexus fortuiti, cursus k */

nomen structura {
    i32 methodus;      /* CONTRACTIO_AVIDUS (ordinarius) | CONTRACTIO_OPTIMUS */
    i32 conatus;       /* OPTIMUS: cursus min-fill fortuiti (ordinarie XVI) */
    i64 semen;         /* OPTIMUS: semen sortis (ordinarie valor fixus in capite nominatus) */
    i64 octeti_maximi; /* OPTIMUS: candidati cum tabula maiore omittuntur
                        * (ordinarie CONTRACTIO_SATURATUM = nullus limes) */
    i32 candidatus;    /* OPTIMUS: CANDIDATUS_OMNES aut unus coactus (probationes) */
} ContractioOptiones;

/* quis candidatus vicit; sumptus simulatus candidati k (FALSUM si k ignotus aut
 * omissus) */
i32 contractio_ordo_candidatus (constans ContractioOrdo* ordo);
b32 contractio_ordo_candidati (constans ContractioOrdo* ordo, i32 k,
                               ContractioSumptus* simulatus);
```

**Candidates** (review M2; excess over best-of-everything, log₂ multiplications, 20 braids: AVIDUS alone 3.35 mean / 6.5 max; this set 0.08 / 0.5):
1. AVIDUS (T2 key) — so OPTIMUS never costs more multiplications than AVIDUS when no memory limit is set;
2. size-reduction key: 2^r − 2^a − 2^b as s64, each 2^x clamped at 2^62 (a heuristic: only determinism matters), ties by ids;
3. min-fill on the line graph (indices adjacent if they share a tensor; dimension-1 indices left out of the fill count), fill evaluated among the 64 lowest-degree vertices per step (a bucket queue, not a sort), ties by (degree, id);
4. `conatus` runs of (3) with ties among equal (fill, degree) broken by `sors_intra` draws from `semen` (integer draws only; on dimension-2 networks all ranks are even, so "near-ties" mean nothing — the T2 id tie-break is where greedy goes wrong).

Dropped after review: union-first key, min-degree, randomized greedy (they add nothing once randomized min-fill is present). The 64 cap costs nothing measurable (8, 16, 64 and uncapped equivalent within seed noise).

**Elimination order → pairwise schedule, online.** Each elimination immediately contracts the two live tensors holding the index (all their shared indices close at once; an index already closed is skipped); leftovers (disconnected pieces, scalars) by the existing fallback. Steps are emitted with sinister < dexter and effectus = n + s, as T2. Because cost accumulates as steps are emitted, a candidate is **abandoned as soon as its partial multiplications reach the incumbent's** (review M6) — this bounds the ordering cost on wide networks, where uncapped fill computation would be O(V·64·d²) ≈ 10¹⁰ at 3-SAT width ~660.

**Choice.** Each candidate produces a step list; one shared simulation (index sets only) gives multiplications, slab bytes and apex. Pick min (multiplications, slab bytes, candidate number), skipping candidates whose slab exceeds `octeti_maximi`; if all exceed, the smallest slab (the officina then refuses as today). Saturated candidates tie and fall to candidate number (the officina refuses them anyway).

**Implementation.**
- `contractio_ordinare` splits into producers → step lists; `_simulare`; `_materializare` (today's `_contrahere`, `_gradum_parare`, `_tabulam_disponere`, `_latitudo`). **One** first-fit slab routine shared by simulation and materialization (the slab depends on step order; two copies would drift).
- Memory: allocate the best and current step buffers (n − 1 pairs each) first; each candidate runs inside `piscina_notare`/`piscina_reficere`, so candidate state never outlives its evaluation; then materialize the winner.
- AVIDUS = producer 1 + materialize, behaviour and tests unchanged. When candidate 1 wins, the OPTIMUS ordo is step-for-step identical to AVIDUS.
- laqueus: `laqueus_modulus_vertex_creare` uses OPTIMUS with `octeti_maximi` from its caller. Test time at table scale (12,966 knots × ~19 orderings) is measured before/after; if it rivals the contractions, a deterministic rule skips extra candidates below an AVIDUS-multiplication threshold (review L8).

**Tests** (`probatio_contractio`, braid-shaped networks built in the test — each crossing a rank-4 tensor on the two strands' current edges, no laqueus dependency):
- every candidate's step list is a valid binary order, and `_independens_probare` passes on OPTIMUS orders of the 300 random networks;
- **forced candidate = simulated cost** (review M3a): for every k, ordinare with candidate k forced gives `sumptus` equal to `contractio_ordo_candidati(k)` field by field, and OPTIMUS returns the argmin;
- **counting anulus** (M3b): a Z/p wrapper whose `multiplica` counts calls; through `contractio_exsequi_anulo` the count equals `sumptus.multiplicationes` exactly, on OPTIMUS orders;
- OPTIMUS multiplications ≤ AVIDUS on every network (no memory limit); when candidate 1 wins, identical orders;
- exact expected winners and costs with `conatus = 0` on in-test braid networks, computed by an independent Python port (as T2); with `conatus > 0`, properties only — valid, cost ≤ deterministic best, same `semen` → same order (no bit-exact port of `sors`);
- `octeti_maximi` respected (a network where the cheaper candidate needs a larger slab picks the other under a tight limit);
- execution under OPTIMUS orders: brute force = hot tier = reference tier;
- options: defaults (AVIDUS, conatus XVI, named semen, octeti_maximi SATURATUM, candidatus OMNES); invalid methodus is now **III** (the existing test at probatio_contractio.c:899 uses II, which becomes OPTIMUS);
- `probatio_laqueus_rete` green under OPTIMUS, plus a **wiring test** (M4): on a fixed braid closure where OPTIMUS strictly wins, `laqueus_modulus_vertex_sumptus` multiplications equal the OPTIMUS value and are below AVIDUS's.

**Plants** (red only): OPTIMUS picks the most expensive candidate; simulated slab or multiplications off by one step (red via forced-candidate equality); conversion contracts the wrong pair; min-fill fill count off (exact winners at conatus = 0); laqueus still passes AVIDUS (wiring test); pruning abandons a candidate before it reaches the incumbent (exact winners).

**Measurement.** `tools/uncinus_mensura` III–IV before/after with the same seeds: winner per network, ns per multiplication per winner (confirming that fewer multiplications means less time in C — the review measured 2^4.5–2^8 multiplications per output entry on the dominant steps, so per-entry overhead is negligible), slab per winner, and laqueus test time.

**Risks.**
- The gain is uneven (0–120× per braid, ~10× mean); at the original 12 × 801 target it may be small. The measurement will say.
- A winner can occasionally have a slightly larger slab (worst seen +7%); `octeti_maximi` bounds it.
- Ordering cost on very wide networks is bounded by incumbent pruning; measured before relying on it for phase 2.

*(2026-10-27: landed. As designed, plus the `limen` option (default 2^20 multiplications: below it only AVIDUS is evaluated — without it the laqueus suite went 2.9 → 10.4 s) and `laqueus_modulus_vertex_ordo` for the wiring test and the tool. 26 sections, 193,060 checks: candidates equal an independent Python port exactly, forced = simulated, counting anulus = multiplications; 7 plants red. Measured, same seeds: braids 8 × 201 20.9 → 6.9 s, 12 × 201 ~99 → 9.2 s, 10 × 201 ~34 min → 12.5 s; sums unchanged. See lib/contractio.worklog.md.)*

**Design review (2026-10-27): approve after fixes, no H.** M1 evidence replaced by multi-seed totals, memory reading corrected (here and brainstorm §11); M2 candidate set (AVIDUS, size key, MF64, 16 randomized MF64); M3 forced-candidate equality, counting anulus, one first-fit routine; M4 laqueus wiring test; M5 exact winners at conatus = 0, properties otherwise, braid networks built in-test; M6 candidates in a reset arena region, online conversion with incumbent pruning, bucket queue; L1 invalid methodus III; L2 octeti_maximi default SATURATUM; L3 `contractio_ordo_candidatus`, named default semen; L4 size key exact definition; L5 cap kept; L6 multiplications are the right objective; L7 refactor traps checked; L8 laqueus test time measured; L9 AVIDUS stays default. Review scripts: scratchpad/recensio_t5b/ (session of 2026-10-27).

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
