# Demo 136: brackets of large braid closures through Temperley–Lieb standard modules (plan)

Status: v2 (2026-10-28) after a DESIGN REVIEW before any data
(consiliarius-d136; summarised in "Design review" at the end). v1 was
approved by Fran (2026-10-28) with the defaults of its "Open for Fran":
home `knotapel/demo_136_tl_braid_closures/`, a design review before T0,
demo first and library after (T4), registered target 12 × 801 with 14
and 16 strands descriptive only. Context:
project-specs/crossings-contraction-counting-brainstorm.md §11 (phase 1
engine measured; lever 1 = T5b), §12.3.1 (exact invariants of large
knots); `tools/uncinus_mensura.c` section V (single-braid projections,
2026-10-28). Precedents: D07 (braid bracket = PD bracket, small), D35
(TL_n regular representation over Z[ζ₈], C_n × C_n).

## Question

The vertex model + contraction engine computes the Kauffman bracket of
any planar diagram exactly. On long random braid closures its width
settles near 2 × strands: a cut across a closed braid meets the n
strands going down and the n closure strands coming back. Projected
full brackets, CPU on this Mac (2026-10-28, `-limes 0`):

| strands | 201 crossings | 401 | 801 |
| --- | --- | --- | --- |
| 8 | 32 s | 5 min | 64 min |
| 10 | 20 s | 3.3 h | 33 h |
| 12 | 8 s | 18 h (19.8 s per point) | ~32 days (210 s per point, apex 2^26) |

For a braid closure there is a smaller state space. The bracket of the
closure of β ∈ B_n is a weighted sum of traces of β in the
Temperley–Lieb standard modules W_{n,j}, whose dimensions square-sum to
Catalan(n) = 208,012 at n = 12, against 4^n = 16.7 million vertex-model
states (2^26 measured at 12 × 801). Each braid generator acts by a
sparse matrix (at most one image per basis state).

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
  d_{n,j} = C(n, (n−j)/2) − C(n, (n−j)/2 − 1), with C(n, −1) = 0.
- e_i on a link state s (points i, i+1):
  (a) i and i+1 matched to each other → δ·s;
  (b) i matched to a, i+1 matched to b → arcs (i, i+1) and (a, b);
  (c) one of them a defect, the other matched to a → arc (i, i+1), a
      becomes a defect;
  (d) both defects → 0.
  So each column of e_i has at most one nonzero entry, ∈ {1, δ}.
- Generators (convention **conv0**): σ_i^{±1} ↦ A^{±1}·1 + A^{∓1}·e_i;
  δ = −A² − A⁻².
- Closure: ⟨β̂⟩_δ = Σ_j Δ_j(δ)·tr ρ_j(β), Δ₀ = 1, Δ₁ = δ,
  Δ_{j+1} = δ·Δ_j − Δ_{j−1} (loops counted with δ each); the bracket
  with ⟨O⟩ = 1 is ⟨β̂⟩_δ / δ. This /δ is not a free choice: it matches
  the vertex model's division by d (⟨O⟩ = 1). The formula is the
  standard decomposition of the Markov trace over cell modules (Jones
  1985; Goodman–de la Harpe–Jones, *Coxeter graphs and towers of
  algebras*, ch. 2), recalled, not derived here.
- Validity mod p (review): the link-state matrices have entries in
  Z[δ] and each Δ_j ∈ Z[δ]; the identity holds over Q(A), hence in
  Z[A^±1], hence after any substitution A ↦ a in F_p. Non-semisimple
  points (A a root of unity mod p) are fine because nothing is inverted
  except δ. Points with δ = 0 (or A = 0) are skipped, as the vertex
  model skips them.
- Point: the vertex model's own points, A = i·ω² mod p with p ≡ 1
  mod 4, i = g^((p−1)/4), g the least quadratic non-residue
  (`laqueus_modulus_vertex_aestimare`), so both methods can be compared
  value by value at one (A, p).
- Full polynomial: `interpolatio_polynomium` with settings copied
  exactly from `_uncinus_per_rete` (lib/laqueus.c, interior):
  - window from the PD code (Kauffman class 3w mod 4), step 4;
  - limes_bitorum = `laqueus_modulus_vertex_limes` (Tait graphs);
  - residuum_primi 1, modulus_primi 4, puncta_addita 2;
  - conatus_maximi = K + 2 + 64, K = (summus − imus)/4 + 1.
  The TL evaluator returns INUTILE at exactly the points `aestimare`
  skips (ω ≡ 0, δ = 0) and chooses i the same way. Otherwise "same
  points, same evaluations" would break silently.

## What was seen before registration (disclosure)

- The projections above (`build/nodi_magni/coniectura.out`): sizes,
  widths, bounds and per-point times of the vertex model on the
  section-V braids. No TL code has been run in this tree.
- The design reviewer computed, in its own scratch Python, on named
  knots (3_1, 4_1, T(3,4), T(4,5)) and on 57 of its OWN random braids
  (n = 2 … 5, not seed 2028), and reported MATCH for:
  - R2 at n ≤ 12;
  - the trace formula against a brute-force loop-counting state sum,
    including at non-semisimple points mod p;
  - conv0 giving the standard right trefoil;
  - R5's stabilization factor under conv0;
  - the R4 identities on the named knots.
  No seed-2028 value was computed by anyone.
- conv0 is therefore PREDICTED to be T1's outcome (assuming `laqueus`
  uses the standard bracket). T1 checks it on σ₁³ directly against the
  state sum (c = 3), not through interpolation (review M4: under the
  swapped convention an odd-writhe knot falls outside the window class
  and interpolation refuses instead of returning a mirror image). The
  trivial braid on one strand (0 crossings, refused by the PD and
  vertex paths) is compared to 1 directly.

## Registered predictions

"Holds" means every case matches exactly; one mismatch falls it, and
findings name the case. A case that does not finish (crash, memory
guard, abandoned) is "not tested", never dropped.

- **R0 (same braids; a gate before any R-check).** Each test braid is
  the first knot from a fresh `sors_seminare(2028, 0)` per (n, L),
  drawn by a verbatim copy of `_plexus_fortuitus` (interior in
  `uncinus_mensura.c`), lengths as below after its parity bump. The
  demo's braids must reproduce coniectura.out's bound and vertex width
  and apex:
  - 8 × 801: limes 642, latitudo 16;
  - 10 × 401: limes 334, latitudo 22;
  - 12 × 401: limes 333, latitudo 24;
  - 12 × 801: limes 654, latitudo 26.
  Otherwise "same braid" in R1p and R6 is unverified, and the demo
  stops.
- **R1 (agreement, polynomial).** For every braid of the test set, the
  TL bracket equals:
  - the `laqueus` state sum where c ≤ 24;
  - the vertex-model bracket (`laqueus_modulus_vertex_uncinus`) where
    its projected time is ≤ 70 min. That covers n ≤ 12 at c ≤ 201 and
    8 × 801 (64 min), the largest size where both run in full.
- **R1p (agreement, off-interpolation points).** A named prime
  p* ≡ 1 mod 4 that the interpolation never uses (chosen in T1 below
  every prime interpolatio draws, and asserted unused), ω = 2, 3, …
  (skipping INUTILE points). At each point:
  - the vertex value;
  - the TL evaluator's value;
  - the exact TL polynomial reduced at (A, p*)
  must all be equal, with both sides' A asserted identical.
  Point counts: 10 × 401: 16 points (~1 min of vertex time);
  12 × 401: 8 (~3 min); 12 × 801: 4 (~14 min). With the reduced
  polynomial in the comparison, a wrong polynomial passes one point
  with probability ≤ degree/p* (≈ 4·10⁻⁷), so these points also check
  the window, the bound, the reconstruction across primes and the
  interpolation.
- **R2 (dimensions).** For every n ≤ 16 and j, the enumerated link
  states of W_{n,j}:
  - number d_{n,j};
  - are distinct;
  - are valid (non-crossing, no enclosed defect).
  Also Σ_j d_{n,j}² = Catalan(n) and Σ_j d_{n,j} = C(n, ⌊n/2⌋).
- **R3 (relations).** On every W_{n,j} with n ≤ 16, mod p at a random
  A (δ = −A² − A⁻²), applying both sides to each basis state:
  - e_i² = δ·e_i;
  - e_i e_{i±1} e_i = e_i;
  - e_i e_k = e_k e_i (|i − k| ≥ 2);
  - σ_i σ_{i+1} σ_i = σ_{i+1} σ_i σ_{i+1};
  - σ_i σ_i⁻¹ = 1.
- **R4 (invariants on the exact polynomial).** For every knot in the
  test set, including 12 × 801, with V(t) the Jones polynomial
  ((−A³)^(−w)·bracket, t = A⁻⁴), evaluated EXACTLY in `magnus` from
  coefficient sums by exponent class. With S_r = Σ c_k over k ≡ r
  (mod 4) and T_r = Σ c_k over k ≡ r (mod 3):
  - V(1) = Σ c_k = 1;
  - V(e^{2πi/3}) = 1: T₁ = T₂ and T₀ − T₂ = 1;
  - V(i) = ±1: S₁ = S₃ and S₀ − S₂ = ±1;
  - |V(−1)| = |Σ (−1)^k c_k| = det K, from `laqueus` (exact magnus),
    wherever T0 finds it affordable.
  All four identities are unchanged under t ↔ 1/t, so R4 cannot see a
  mirror image (declared).
- **R5 (moves, at points).** Identities between values at fixed points
  mod p: every point of one prime (~7 min at 12 strands, ~25 min at 13).
  No coefficient bound or vertex structure is needed. Under conv0:
  - stabilization at the back: value(β σ_n^{±1}, n+1 strands) =
    −A^{±3}·value(β);
  - stabilization at the front: every index shifted by +1, σ₁^{±1}
    prepended, n+1 strands → the same factor −A^{±3};
  - flip: σ_i → σ_{n−i} (the same knot and writhe) → the same value;
  - conjugation, σ_k β σ_k⁻¹ → the same value: a smoke test only, since
    trace cyclicity and R3 already imply it (review M1).
  Checked on the 12 × 801 braid (k = 6; 13 strands for the
  stabilizations) and on every smaller test braid.
- **R6 (speed).** Same braid, same point, CPU time per point, TL
  against vertex:
  - ≥ 25× at 12 × 401 (model ~55×: 1.67·10⁸ against 8.93·10⁹
    multiplications);
  - ≥ 100× at 12 × 801 (model ~430× by count: 3.3·10⁸ against
    1.4·10¹¹; ~315× by measured time at ~2 ns).
  The full exact 12 × 801 bracket completes in ≤ 8 h CPU (model
  ~2.4 h, ~13,000 point-and-prime evaluations).

**Known blind spot (rewritten after review M3).** R1 runs n = 12 in
full at c ≤ 201, so it exercises every Δ_j for j ≤ 12 and every e_i
there. What R1 cannot see is faults that depend on LENGTH:
- the window, the bound and the number of primes at large c;
- buffers sized by c;
- any code path taken only for long words.
R1p (off-interpolation points against the exact polynomial) and R4 are
the checks for those; plant 8 shows they fire. R4 is blind to mirror
images; R1 and R5's stabilization factor are not.

**Descriptive (no prediction):**
- largest coefficient (bits) against the Tait bound, per size;
- span of V against c (equality iff the diagram is reduced alternating,
  Kauffman–Murasugi–Thistlethwaite; random braid closures will almost
  never reach it);
- CPU time per point for n = 4 … 16 at c = 801 (16 strands: Catalan =
  35 million, about 2 min per point projected);
- the largest intermediate matrix entry when the trace is carried over
  Z at ONE integer point (A = 2, `magnus`) for c ≤ 801, and as
  polynomials in A only for c ≤ 201, under custos (whether a
  prime-free exact route is plausible; brainstorm §11, lever 2).

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
  E_i M gathers rows (row image(s) += coeff·row s), into a second
  buffer. Then Σ_j Δ_j·tr M, divided by δ.
  - Memory: 2 × (largest module)² words (12 strands: 2 · 297²;
    16 strands: 2 · 3,640², 106 MB).
  - Cost: ≤ 2·c·Catalan(n) multiplications (case (d) gives zeros).
  - Optional in T1 (review L1): σ = A^{±1}(1 + A^{∓2}·e) with A^w
    pulled out of the trace. Only rows with an arc (i, i+1) change, so
    the update runs in place with one buffer and ≤ c·Catalan(n)
    multiply-adds. R6 thresholds stay as registered.
- Full bracket: `interpolatio_polynomium` with the TL evaluator as
  callback, settings as in Definitions.
- Test set (fixed now): every braid per R0 (fresh seed 2028 per (n, L),
  the section-V generator copied verbatim). Requested lengths (the
  generator bumps parity: even n gets 21 and 51, odd n gets 102 and
  202, and so on):
  - n = 3 … 12, lengths 20, 50, 101, 201;
  - 8 × 401 and 8 × 801;
  - 10 × 401, 12 × 401 and 12 × 801;
  - named braids: σ₁³ (3_1), σ₁σ₂⁻¹σ₁σ₂⁻¹ (4_1), (σ₁σ₂)^4 on 3 strands
    (T(3,4) = 8_19), (σ₁σ₂σ₃)^5 on 4 strands (T(4,5)).
  - Every run under `tools/custos.sh`; long ones detached (python
    double-fork + setsid), watched with Monitor.
- Plants (each must turn the named check red; expected results stated):
  1. case (c) of e_i returns 0 → R3 red (the e_i e_{i±1} e_i relation
     only; e_i² and far commutation stay green), R1 red, R5
     stabilization red;
  2. Δ_{j+1} = δ·Δ_j + Δ_{j−1} → R1 red at n ≥ 2, R5 stabilization red
     (conjugation stays green: why it is only a smoke test);
  3. σ_i^{−1} given σ_i^{+1}'s image → R1 red everywhere, 4_1 included
     (its word becomes (σ₁σ₂)², a trefoil);
  3b. global convention swap σ^{±1} ↦ A^{∓1} + A^{±1}e (a mirror
     image) → R1 red on chiral knots, 4_1 green (amphichiral,
     palindromic bracket); R5 stabilization red (factor −A^{∓3}); R4
     blind to it; odd writhe → interpolation refuses (the window
     class);
  4. the /δ normalization dropped → R1 red;
  5. states with an enclosed defect admitted → R2 red;
  6. one coefficient of the final polynomial changed by 1 → R4 red
     (tests the identity checks themselves);
  7. letters of the conjugating pair not inverted (σ_k β σ_k) → R5
     conjugation red;
  8. a fault taken only when c > 401 (one letter skipped in the TL
     evaluator) → R1 green, R1p red, R4 red (shows the large-size
     checks fire).

## Tasks

- **T0. Feasibility.** No bracket computed.
  - Enumerate link states for n ≤ 16 (counts, time, memory).
  - Time the row-gather kernel on random u32 matrices of size 297 and
    3,640 (ns per multiply-add).
  - Time `laqueus`'s determinant on the 8 × 801 and 12 × 801 knots, to
    decide where R4's det check runs.
  - Write the findings into this plan as a dated amendment. T0 may
    decide where the det check runs; it may NOT move any R0–R6
    threshold. If T0 shows R6 will fail, that is recorded as a predicted
    failure, not a re-registration.

  **T0 amendment (2026-10-28, measured; no bracket computed).** Scratch
  program `d136_t0/tempora.c` (not committed), this Mac, -O2, every run
  under `tools/custos.sh`. Braids drawn per R0 (fresh seed 2028 per
  (n, L), the generator copied verbatim).

  | measurement | result |
  | --- | --- |
  | link states, n = 1 … 16 | every d_{n,j} = formula; Σ d² = Catalan(n); 12,870 states at n = 16 in 0.1 ms, 206 KB |
  | row-gather kernel, two buffers, ~¼ of rows zero (case d) | d = 297: 0.98 ns · d = 1,001: 1.06 ns · d = 3,640: 1.12 ns per multiply-add |
  | `laqueus_alexander_ex_pd`, 8 strands | c = 51: 0.003 s · 101: 0.06 s · 201: 0.40 s · 401: 13.9 s · 801: > 600 s (stopped by custos) |
  | colouring-matrix determinant mod one prime ((c−1) minor, arcs from the PD code) | ≤ 0.007 s at c = 801, 8 and 12 strands |

  Decisions (within T0's remit; no threshold moves):
  - **R4's det check runs at every test size, including 12 × 801**, by
    two routes:
    - c ≤ 401: |Δ(−1)| from `laqueus_alexander_ex_pd` (≤ 14 s);
    - every c: the demo's own colouring-matrix determinant, the
      (c−1) minor mod primes plus CRT, with the Hadamard bound
      6^((c−1)/2) (~35 primes at c = 801, well under a second).
    The two routes must agree wherever both run. At c = 801 only the
    colouring route runs; it shares nothing with the TL code or the
    vertex model. This route was not in v1/v2 ("from `laqueus`"); it is
    added because `laqueus` does not finish at 801.
  - Cost model, recorded only (the R6 thresholds stand as registered):
    - at ~1 ns per multiply-add a 12 × 801 point is ≤ 3.3·10⁸
      operations, ≤ 0.35 s against the vertex model's 210 s;
    - the full exact 12 × 801 bracket is ~13,000 × 0.35 s ≈ 1.3 h,
      against v2's 2.4 h at an assumed 2 ns.
  - T4 reach, recorded for later: a 14 × 801 point ≈ 4.6 s (full
    bracket ~17 h); a 16 × 801 point ≈ 63 s (full bracket ~9.5 days,
    so points only, unless the number of primes or points falls,
    brainstorm §11 lever 2).
- **T1. Build.** main.c (link states, e_i tables, trace at a point,
  full bracket); R0 gate; R2 and R3; conv0 on σ₁³ against the state sum
  directly and the one-strand trivial braid against 1; R1 on n ≤ 6
  short braids; p* for R1p; plants 1–8.
- **T2. Runs.** The test set; R1, R1p, R4, R5, R6; findings.md;
  archive.
- **T3. Review.** An independent reviewer with its own oracle; it does
  not build in the live tree.
- **T4 (Fran's call).** Promote the evaluator into `laqueus` (e.g.
  `laqueus_uncinus_ex_plexu`) with a probatio; then 14 and 16 strands,
  and track A (Jones statistics on random braid closures).

## Design review (consiliarius-d136, 2026-10-28, before any data)

The maths checked out in the reviewer's own scratch oracle (see
disclosure). Findings and where they went:

- **H1.** R6 ≥ 100× at 12 × 401 contradicted the plan's own model
  (~55×) → per-size thresholds (≥ 25× and ≥ 100×).
- **H2.** R5 did not say polynomial or points; a full 13-strand
  polynomial does not fit the time budget, and the bound needs
  `laqueus_modulus_vertex_creare`, which would likely exceed the
  workspace cap at 13 strands → R5 as identities at fixed points.
- **M1.** Conjugation follows from trace cyclicity plus R3 → front
  stabilization and the index flip added; conjugation demoted to a
  smoke test; plant expectations for R5 registered.
- **M2.** R1p points were interpolation inputs (circular) → a separate
  prime p*, comparison with the reduced exact polynomial, A asserted
  equal, a point count for 10 × 401.
- **M3.** The blind-spot example was wrong (R1 covers n = 12 at
  c ≤ 201) → rewritten around length-dependent faults; plant 8.
- **M4.** Plant 3's expectation was wrong (4_1 goes red) → corrected;
  plant 3b (global swap, the case 4_1 cannot see); conv0 checked
  directly, not through interpolation.
- **M5.** The test set did not pin the generator → R0 fingerprint gate,
  fresh seed per (n, L), parity bump stated.
- **M6.** Interpolation settings and point-skipping must match
  `_uncinus_per_rete` → copied into Definitions (verified against
  lib/laqueus.c).
- **M7.** "Largest entry over Z" was unscoped (many GB as polynomials
  at 12 × 801) → one integer point at full length, polynomials only at
  c ≤ 201.
- **M8.** R4 must be exact → exponent-class sums in `magnus`; mirror
  blindness declared.
- **M9.** T0 may not move thresholds → stated in T0.
- **L1–L6.** Memory line 2 × module², optional in-place update; R3 at
  random A and n ≤ 16 per basis state; C(n, −1) = 0 and the Σ d check;
  span claim corrected (reduced alternating); /δ not a free choice,
  one-strand trivial braid against 1 directly; disclosure of the
  reviewer's computations and the conv0 prediction.
