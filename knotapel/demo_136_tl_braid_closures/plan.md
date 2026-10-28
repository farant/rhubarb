# Demo 136: brackets of large braid closures through Temperley–Lieb standard modules (plan)

Status: v1 draft (2026-10-28), for Fran. Not yet design-reviewed; per
the knotapel rule a design review comes before T0. Context:
project-specs/crossings-contraction-counting-brainstorm.md §11 (phase 1
engine measured; lever 1 = T5b), §12.3.1 (exact invariants of large
knots); `tools/uncinus_mensura.c` section V (single-braid
projections, 2026-10-28). Precedents: D07 (braid bracket = PD bracket,
small), D35 (TL_n regular representation over Z[ζ₈], C_n × C_n).

## Question

The vertex model + contraction engine computes the Kauffman bracket of
any planar diagram exactly. On long random braid closures its width
settles near 2 × strands: a cut across a closed braid meets the n
strands going down and the n closure strands coming back. Projected
full brackets, CPU on this Mac (2026-10-28, seed 2028, `-limes 0`):

| strands | 201 crossings | 401 | 801 |
| --- | --- | --- | --- |
| 8 | 32 s | 5 min | 64 min |
| 10 | 20 s | 3.3 h | 33 h |
| 12 | 8 s | 18 h | ~32 days (210 s per point, apex 2^26) |

For a braid closure there is a smaller state space. The bracket of the
closure of β ∈ B_n is a weighted sum of traces of β in the
Temperley–Lieb standard modules W_{n,j}, whose dimensions square-sum to
Catalan(n) = 208,012 at n = 12, against 4^n = 16.7 million vertex-model
states (2^26 measured at 12 × 801). Each braid generator acts by a
sparse matrix (one image per basis state).

Can this route compute the bracket exactly, agree with the vertex
model everywhere both run, and reach 12 strands × 801 crossings in
hours instead of a month?

## Definitions

- Braid word: letters ±i, 1 ≤ i < n, as `laqueus_pd_ex_plexu` reads
  them (σ_i^{+1} = positive crossing; closure of σ₁³ = right trefoil
  3_1).
- Link state of W_{n,j} (j ≡ n mod 2, 0 ≤ j ≤ n): a non-crossing
  partial matching of points 1 … n with (n − j)/2 arcs and j unmatched
  points (defects), no defect inside an arc. Dimension
  d_{n,j} = C(n, (n−j)/2) − C(n, (n−j)/2 − 1).
- e_i on a link state s (points i, i+1):
  (a) i and i+1 matched to each other → δ·s;
  (b) i matched to a, i+1 matched to b → arcs (i, i+1) and (a, b);
  (c) one of them a defect, the other matched to a → arc (i, i+1), a
      becomes a defect;
  (d) both defects → 0.
  So each column of e_i has at most one nonzero entry, ∈ {1, δ}.
- Generators: σ_i^{±1} ↦ A^{±1}·1 + A^{∓1}·e_i; δ = −A² − A⁻².
- Closure: ⟨β̂⟩_δ = Σ_j Δ_j(δ)·tr ρ_j(β), Δ₀ = 1, Δ₁ = δ,
  Δ_{j+1} = δ·Δ_j − Δ_{j−1} (loops counted with δ each); the bracket
  with ⟨O⟩ = 1 is ⟨β̂⟩_δ / δ. This is the standard decomposition of the
  Markov trace over cell modules (Jones 1985; Goodman–de la Harpe–Jones,
  *Coxeter graphs and towers of algebras*, ch. 2), recalled, not derived
  here; R1 tests it.
- Point: the vertex model's own points, A = i·ω² mod p with p ≡ 1
  mod 4 (`laqueus_modulus_vertex_aestimare`), so both methods can be
  compared value by value at one (A, p). Full polynomial:
  `interpolatio_polynomium` with the same window (Kauffman class
  3w mod 4, step 4), the same coefficient bound (Tait graphs,
  `laqueus_modulus_vertex_limes`) and its extra-point / extra-prime
  checks.

## What was seen before registration (disclosure)

- The projections above (`build/nodi_magni/coniectura.out`): sizes,
  widths, per-point times of the vertex model on the seed-2028 braids.
  No TL code has been run.
- Free choices, fixed in T1 on σ₁³ (right trefoil) and the trivial
  braid BEFORE any random braid is compared: which of A·1 + A⁻¹·e_i or
  A⁻¹·1 + A·e_i is σ_i^{+1}, and the /δ normalization. They are the
  only free choices. A wrong one shows as a mirror (A ↔ A⁻¹) or a
  factor δ on every braid, never as a pattern on some braids only.

## Registered predictions

"Holds" means every case matches exactly; one mismatch falls it, and
findings name the case. A case that does not finish (crash, memory
guard, abandoned) is "not tested", never dropped.

- **R1 (agreement, polynomial).** For every braid of the test set
  (Method), the TL bracket equals:
  - the `laqueus` state sum where c ≤ 24;
  - the vertex-model bracket (`laqueus_modulus_vertex_uncinus`) where
    its projected time is ≤ 70 min. That includes 8 × 801 (64 min),
    the largest size where both run in full.
- **R1p (agreement, points).** At 10 × 401, 12 × 401 and 12 × 801 the
  vertex model cannot finish. There the TL value equals the vertex
  value at registered points: the first 8 points of the first prime at
  12 × 401 (~3 min of vertex time), the first 4 at 12 × 801 (~14 min).
- **R2 (dimensions).** The enumerated link states of W_{n,j} number
  d_{n,j} for every n ≤ 16 and j, distinct, valid (non-crossing, no
  enclosed defect); Σ_j d_{n,j}² = Catalan(n).
- **R3 (relations).** On every W_{n,j} with n ≤ 10, mod p at a random δ:
  e_i² = δ·e_i; e_i e_{i±1} e_i = e_i; e_i e_k = e_k e_i (|i − k| ≥ 2);
  and for the generators σ_i σ_{i+1} σ_i = σ_{i+1} σ_i σ_{i+1} and
  σ_i σ_i⁻¹ = 1.
- **R4 (invariants on the exact polynomial).** For every knot in the
  test set, including 12 × 801, with V(t) the Jones polynomial
  ((−A³)^(−w)·bracket, t = A⁻⁴):
  - V(1) = 1;
  - V(e^{2πi/3}) = 1;
  - V(i) = ±1 (the Arf invariant);
  - |V(−1)| = det K, from `laqueus` (determinant, exact magnus) wherever
    T0 finds it affordable.

  These identities hold for every knot, so they check the polynomial
  where no second method finishes.
- **R5 (moves).** Bracket(σ_k β σ_k⁻¹) = bracket(β) (regular isotopy),
  and bracket(β σ_n^{±1} on n+1 strands) = −A^{±3}·bracket(β) (Markov
  stabilization). Checked on the 12 × 801 braid (k = 6; 13 strands for
  the stabilization) and on every smaller test braid.
- **R6 (speed).** Same braid, same point: TL time per point ≥ 100×
  faster than the vertex model at 12 × 401 and 12 × 801. The cost model
  predicts about 2·c·Catalan(n) multiplications (12 × 801:
  3.3·10⁸ against the vertex model's 1.4·10¹¹, ~430×). The full exact
  12 × 801 bracket completes in ≤ 8 h CPU; the model says ~2.4 h at
  ~2 ns per multiplication and the same ~13,000 point-and-prime
  evaluations.

**Known blind spot.** At 12 × 801 nothing independent computes the
whole polynomial. R1p checks 4 points of a degree-~3,200 window; R4
checks four identities; R5 checks the code's consistency under moves,
not its correctness. A systematic error that keeps all three (e.g. a
wrong Δ_j at j ≥ 12 only, which R1 at n ≤ 8 cannot see) is what R1p
catches. Hence R1p at the largest sizes, and plant 2.

**Descriptive (no prediction):**
- largest coefficient (bits) against the Tait bound, per size;
- span of V against c (span ≤ c, equality iff alternating-adequate);
- per-point time for n = 4 … 16 at c = 801 (16 strands: Catalan =
  35 million, about 2 min per point projected);
- the largest intermediate matrix entry if the trace is carried over Z
  instead of mod p (tells whether a prime-free exact route is
  plausible: brainstorm §11, lever 2).

## Method

- One C file `main.c` (K&R demo style, house headers: `piscina`,
  `sors`, `magnus`, `polynomium`, `interpolatio`, `congruentia`,
  `laqueus`), frozen with `knotapel/archive.sh` when done.
- Link states: an array of partner indices per state (−1 = defect);
  enumeration by recursion; index by a hash table (`tabula_dispersa`)
  from the state's encoding. Per (n, j, i): tables image(s) and
  coefficient tag ∈ {0, 1, δ}, built once.
- Trace per point: per j, carry M (d_j × d_j, residues mod p, u32) from
  the identity through the word: M ← A^{±1}·M + A^{∓1}·(E_i M), where
  E_i M gathers rows (row image(s) += coeff·row s). Then
  Σ_j Δ_j·tr M, divided by δ. Memory: the largest module squared
  (12 strands: 297² words; 16 strands: 3,640², 53 MB).
- Full bracket: `interpolatio_polynomium` with a TL evaluator callback
  choosing the same points as the vertex model.
- Test set (fixed now): seed 2028, the same generator as `uncinus_mensura`
  section V (`_plexus_fortuitus`, knots only).
  - n = 3 … 12, lengths 20, 50, 101, 201;
  - 8 × 401 and 8 × 801;
  - 10 × 401, 12 × 401 and 12 × 801;
  - named braids: σ₁³, σ₁σ₂⁻¹σ₁σ₂⁻¹ (4_1), (σ₁σ₂)^4 on 3 strands
    (T(3,4) = 8_19), (σ₁σ₂σ₃)^5 on 4 strands (T(4,5)).
  - Every run under `tools/custos.sh`; long ones detached (python
    double-fork + setsid), watched with Monitor.
- Plants (each must turn the named check red):
  1. case (c) of e_i returns 0 → R3 red, R1 red;
  2. Δ_{j+1} = δ·Δ_j + Δ_{j−1} → R1 red at n ≥ 2 (j = 2 already);
  3. σ_i^{−1} given σ_i^{+1}'s image → R1 red on chiral braids;
     4_1 alone stays green (the test set has chiral knots);
  4. the /δ normalization dropped → R1 red;
  5. states with an enclosed defect admitted → R2 red;
  6. one coefficient of the final polynomial changed by 1 → R4 red
     (tests the identity checks themselves);
  7. letters of the conjugating pair not inverted (σ_k β σ_k) → R5 red.

## Tasks

- **T0. Feasibility.** No bracket computed.
  - Enumerate link states for n ≤ 16 (counts, time, memory).
  - Time the row-gather kernel on random u32 matrices of size 297 and
    3,640 (ns per multiply-add).
  - Time `laqueus`'s determinant on the 8 × 801 and 12 × 801 knots, to
    decide where R4's det check runs.
  - Write the findings into this plan as a dated amendment.
- **T1. Build.** main.c (link states, e_i tables, trace at a point,
  full bracket); R2 and R3; fix the two free choices on σ₁³ and the
  trivial braid; R1 on n ≤ 6 short braids; plants 1–7.
- **T2. Runs.** The test set; R1, R1p, R4, R5, R6; findings.md;
  archive.
- **T3. Review.** An independent reviewer with its own oracle; it does
  not build in the live tree.
- **T4 (Fran's call).** Promote the evaluator into `laqueus` (e.g.
  `laqueus_uncinus_ex_plexu`) with a probatio; then 14 and 16 strands,
  and track A (Jones statistics on random braid closures).

## Open for Fran

1. Home `knotapel/demo_136_tl_braid_closures/`, design review before
   T0: as D135.
2. Demo first, library after (T4): recommended, so the code meets its
   answer keys before it gets an API. The alternative is to write it as
   `laqueus` code with a probatio from the start.
3. Target fixed at 12 × 801, with 14 and 16 strands descriptive only
   (T4): recommended. Raising the registered target to 14 strands
   would need the R1p vertex points there, and those are out of reach
   (apex ≥ 2^28).
