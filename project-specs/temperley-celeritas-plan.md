# temperley celeritas: speed pass for the TL route (D136 T4, step 2)

Status: v2 APPROVED by Fran (2026-10-31); T0 next.
v1 (4dfe7544) was approved by Fran in outline. The review (verdict
"approve after fixes": H1–H3, M1–M6, L1–L5) is at the end, with what
each finding changed.

Agreed with Fran (2026-10-30): T4 goes promote (step 1, done:
project-specs/temperley-lieb-plan.md, 49b08975..79e7fad9), then a speed
pass with registered predictions (this plan), then track A (Jones
statistics on random braid closures, after a literature check). Parallel
points (L3) are a separate plan after this one (Fran, 2026-10-31).

## Purpose

The TL route costs ~1.5 ns × c × Catalan(n) per point (D136 findings,
Result 2), ~13,000 points for a full 12 × 801 bracket: 78 CPU-minutes
at 12 strands, ~12 h at 14 and ~6.5 days at 16. Exactness rules out
stopping when the answer looks stable (brainstorm §11), so speed has to
come from the kernel (this plan) and from parallel points (next plan).

## Governing rules

1. **Exact only**, as in contractio-plan.md rule 1. No floats anywhere.
2. **Registered before measured.** The predictions below are fixed
   before any new code is timed. Gates stated here decide whether an
   optional lever is adopted; nothing moves a threshold. A prediction
   that can only be framed after T1 exists (Montgomery's, if adopted)
   is registered by a dated amendment BEFORE it is measured.
3. **Same answers.** Every change keeps all suites green; every timed
   point in the measurement tool asserts old value = new value; at the
   end all 45 D136 brackets are recomputed through the library and
   compared with the certified `knotapel/demo_136_tl_braid_closures/
   brackets.txt` (sha256 de53b6e5…).
4. **Callers unchanged** (`laqueus_uncinus_ex_plexu`, the TL route's
   functions). One contract change is possible and is gated (M2): if
   the TL route adopts primes below 2^31, `laqueus_modulus_temperley_
   aestimare` returns ERRATUM for p ≥ 2^31, the header says so, and
   step 1's rule 4 ("same interpolation settings as the vertex route")
   gets a dated amendment. The vertex route is untouched either way.

## The levers

### L1. In-place normalized update (T1)

Today each letter costs a full scaling of the d × d matrix plus a row
gather (left multiplication: row img(s) += f × row s, letters last to
first): about 1.9 × Catalan(n) multiply-and-reduce operations per
letter. Writing σ_i^ε = A^ε (1 + c E_i) with c = A^(−2ε) and pulling
A^w (w = the writhe) out of the trace gives ρ(β) = A^w Π(1 + c_k E_k),
same order. In a standard module (checked by the reviewer for every
module n ≤ 16): rows with genus TEMPERLEY_DELTA (the arc at points
i−1, i) map to themselves times δ; rows with genus TEMPERLEY_UNUM are
the sources and map INTO DELTA rows; NIHIL rows map to 0. So only DELTA
rows change, no DELTA row is a source, and

  new M[r] = α M[r] + c S_r,   S_r = Σ_{s → r} M[s],   α = 1 + cδ = −A^(−4ε)

is exact in place, with one buffer. Only four constants per point:
α±, c± (α = −A^(−4ε) because δ = −A² − A⁻²).

**Kernel shape (fixed, M1): gather.** At creare, for each letter i and
module j, a source list per DELTA row (CSR: offsets + source indices),
built from mt's copied tables inside the existing piscina mark. Per
DELTA row r and column: S = Σ M[s][col], **kept reduced as it is summed**
(s += x; if (s ≥ p) s −= p), then (α M[r][col] + c S) reduced (H1: an
unreduced S of up to ⌊n/2⌋ = 8 sources overflows 64 bits). The CSR
table is counted in `laqueus_temperley_sumptus(...).octeti`.

Exact operation counts (reproduced by the reviewer):

| n | current (× Catalan per letter) | DELTA-row elements (× Catalan) | source reads (× Catalan) | ratio of multiply-reduce |
| --- | ---: | ---: | ---: | ---: |
| 8 | 1.855 | 0.300 | 0.555 | 6.2 |
| 12 | 1.879 | 0.283 | 0.596 | 6.7 |
| 14 | 1.886 | 0.278 | 0.608 | 6.8 |
| 16 | 1.892 | 0.274 | 0.618 | 6.9 |

Each DELTA-row element costs two multiplications and one or two
reductions (below); each source read is an add and a compare.

### L2. Reduction (T2, gated)

- **Primes below 2^31 (lazy: one reduction).** With S reduced and
  p < 2^31, α M + c S < 2 (p−1)² < 2^63: one `%` per element. With
  primes near 2^32 it needs two: (α M mod p) + c S < 2^64. Gate G1 (T2):
  adopt primes below 2^31 for the TL route only if the A/B per-point
  ratio (two-reduction T1 kernel against one-reduction) is at least
  **1.10×** at 12 × 801 and 16 × 801. If adopted: rule 4's contract
  change (M2). The prime count does not change at 12 × 801 (M3: 23
  either way).
- **Montgomery** (R = 2^32, p < 2^31, R folded into α±, c± so the matrix
  stays in normal form and the identity stays 1; p' by Newton,
  x ← x (2 − p x) mod 2^32, five steps). The reviewer's microbenchmark
  on this machine has it 0.76–0.87× the speed of `%` for this shape
  (Apple's divider is fast). Gate G2 (T2, only if G1 adopted): time it
  on the T1 kernel; adopt only if at least **1.20×** per point; if
  adopted, register its per-point prediction by dated amendment before
  measuring it at the three sizes.

### L3. Points in parallel (separate plan, next)

Recommended shape: a tool-level driver that runs worker copies of itself
through `processus_incipere`, each evaluating a slice of (prime, point)
pairs, feeding `interpolatio` through a new "precomputed values" entry
point (forking inside the library would collide with examen 96/97).

Future lever, recorded (review L3): only tr M is needed, and left
multiplication acts on columns independently, so column blocks of width
B can run through all letters in a d × B buffer (~1 MB instead of
53 MB at 16 strands for B = 64): the working set stays in cache, the
i32 d² ceiling relaxes, and one point can split across processes. It
would supersede P2's memory figure. Not in this plan.

## Measurement (H3: A/B in one binary)

`tools/uncinus_mensura.c` gains a TL section carrying a **frozen copy of
today's kernel** (79e7fad9) built only on the public `temperley` API
(`temperley_modulus_creare` + `temperley_actio` give identical tables);
nothing old stays in lib/. Per size: at least **9 pairs in ABBA order**
(old, new, new, old, …), same prime (the largest prime ≡ 1 mod 4 below
2^31, 2,147,483,629, accepted by both kernels), same binary and flags
(-O2), CPU time per call, braids from the R0 generator (seed 2028). Each
pair asserts old value = new value. The statistic is the **median of the
per-pair ratios**, with the range reported. Load average is recorded,
not gated (it is the wrong proxy for P-core/E-core placement and cache
contention; the reviewer saw 15–20% drift between reps but ±7% within
A/B pairs). Long runs under `caffeinate -i` and custos.

## Registered predictions

- **P1 (T1).** A/B per-point ratio old/new, median of ≥ 9 ABBA pairs, at
  least **3.0×** at 12 × 801, 14 × 801 and 16 × 801. (Multiply-reduce
  ratio 6.7–6.9; the threshold is under half of it, for memory traffic,
  the source reads and the second reduction.)
- **P2 (T1, check, not prediction).** `laqueus_temperley_sumptus(16, c)
  .octeti` drops by at least 45% (closed form: ~107.0 MB → ~55.5 MB with
  the CSR counted, −48%), and the byte figure still equals the measured
  allocation within alignment bytes for n = 2..16 (T2b's check, rerun).
- **P3 (T1, end to end).** The full 12 × 801 bracket through
  `laqueus_uncinus_ex_plexu` equals brackets.txt's line exactly. Its
  CPU time is DESCRIPTIVE (one run; its expected value is T0's
  bracket time / P1's measured ratio, since the evaluation count is
  unchanged: 23 primes × 563 points = 12,949, a computed check,
  review M3).
- (G1, G2 are gates, not predictions; a Montgomery prediction, if G2
  adopts it, is registered by amendment.)

Not predicted (descriptive): per-point times for n = 8 … 16; the
dispatcher's refitted TL constant (old 150, new value recorded).

## Tests

- All existing suites stay green. The faults this plan can introduce
  are caught by EXISTING oracles (review H2): stabilization at points
  (`temperley motus`), TL = vertex at points (`temperley puncta`), the
  state sum, known knots, the certified brackets.
- `temperley puncta` additionally asserts that its braid has w ≠ 0
  (8 × 101: w = −23), so a dropped A^w cannot hide there.
- If G1 adopts primes below 2^31: the TL = vertex point test moves from
  p = 4,294,967,029 to 2,147,483,629 (still catches a 32-bit multiply
  fault), and a new check asserts ERRATUM from the TL route at
  p ≥ 2^31.
- The measurement tool's A/B asserts old = new at every timed point
  (n up to 16, the largest admitted prime): the strongest check of the
  kernel at sizes the suite cannot reach.
- Plants (literal triples in a script under build/, named reds; a run
  without a summary is inconclusive):
  - S unreduced (no compare-subtract in the source sum) → red in
    `temperley summa statuum`, `temperley vertex`, `temperley uncini
    certi` (interpolation primes near 2^32: c × S with S of two or more
    residues passes 2^64) and `temperley puncta` (its p = 4,294,967,029
    leg); NOT in `temperley motus`, whose p ≈ 2^30 keeps c × S below
    2^64 for up to 5 sources;
  - a DELTA row counted among its own sources → `temperley summa
    statuum`, `temperley vertex`, `temperley puncta`;
  - α = 1 + c instead of 1 + cδ → the same three;
  - c = A^(+2ε) → `temperley motus`, `temperley puncta`, `temperley
    summa statuum`;
  - the A^w factor dropped → `temperley motus`, `temperley puncta`;
  - if Montgomery is adopted: a constant not pre-scaled by R.

## Dispatcher (M5)

`laqueus_temperley_sumptus(...).multiplicationes` stays the nominal
c × Catalan(n) (tests pin 801 × 208012 and 15 × 35357670); only the TL
price constant (CL = 150) is refitted from T1's measured per-point
times (one constant fits within ~10% across n, since the op ratio is
6.2–6.9). T0 prints the flip constant K* = 185 · sv.multiplicationes /
st.multiplicationes for every dispatcher test braid. Known consequence:
the 7 × 32 cap-flip braid (K* = 130.4) flips to TL on price once the
constant drops below 130.4, losing its purpose; T3 replaces it with a
braid whose K* is below the new constant and whose TL bytes are still
below its vertex workspace (recensio T4 M1: common at n = 7..14),
recording old and new braids. T3 also recomputes the margins of the
12 × 101 and 12 × 201 vertex choices and of the 12 × 201 test's
`sv.octeti_officinae < st.octeti` (TL bytes at 12 strands fall from
~757 KB to ~405–485 KB; vertex workspace 279 KB).

## Tasks

- **T0. Baseline and K*.** The TL section of `tools/uncinus_mensura.c`
  (frozen old kernel, A/B harness, K* printout); old-kernel per-point
  times n = 8 … 16 at c = 801 and one full 12 × 801 bracket time
  (current library, 79e7fad9) for P3's expectation; a dated amendment
  with the numbers. No threshold moves.
- **T1. In-place update.** Gather kernel with CSR, one buffer, A^w,
  S reduced, two reductions (primes near 2^32 kept); sumptus and header
  ("duo alvei" → one buffer + CSR); tests and plants; measure P1, P2, P3.
- **T2. Reduction gates.** G1 (primes below 2^31, one reduction), then
  G2 (Montgomery) only if G1 adopted; each adopted change with its tests,
  plants, contract amendment and (for G2) registered prediction.
- **T3. Acceptance and dispatcher.** All 45 D136 brackets recomputed
  through `laqueus_uncinus_ex_plexu` against brackets.txt; refit CL;
  replace the 7 × 32 test braid and recheck the margins (M5); worklog.
  Fran decides here whether to run a full 14 × 801 bracket.
- **T4. Review.** Independent reviewer, own oracle, not building in the
  live tree.

## AUDIENDA

- How much of the 6.7× survives memory traffic is what P1 answers.
- The reviewer's microbenchmark already suggests Montgomery loses on
  this machine; G2 will likely drop it.
- The A/B design measures old and new under the same conditions; it
  does not measure absolute speed under a quiet machine, which nobody
  can promise here.
- Q2 (the 14 × 801 headline) is Fran's call at T3.

## Design review (2026-10-31, consiliarius-celeritas)

Verdict: approve after fixes; L1's mathematics confirmed (convention,
the DELTA/UNUM structure for every module n ≤ 16, in-place = old kernel
360/360 in the reviewer's oracle, α = −A^(−4ε), operation counts).
Findings and where they went:
- **H1** "the source sum and two products fit in 64 bits" is false for
  an unreduced S (up to ⌊n/2⌋ = 8 sources: 9 (p−1)² > 2^64; Montgomery
  likewise) → S kept reduced as summed; one reduction only with
  p < 2^31, else two; plant "S unreduced".
- **H2** the two new tests (mirror, σσ⁻¹ insertion) are blind to the
  faults they targeted (both faults are mirror-equivariant and keep w)
  → dropped; plants' expected reds point at the existing oracles that
  do catch them (stabilization, TL = vertex, state sum); w ≠ 0
  asserted; old = new asserted at every timed A/B point.
- **H3** "load below 2" will not fire on this shared machine, and T0 vs
  T1 across time cannot resolve 3× (D136's own per-point time varied
  2.3×) → A/B in one binary with a frozen old kernel, ≥ 9 ABBA pairs,
  median ratio, load recorded not gated.
- **M1** kernel shape unspecified; the hazard plant did not fit → gather
  with CSR, counted in sumptus; plants matched to the gather form.
- **M2** primes below 2^31 are a contract change (the TL = vertex test at
  4,294,967,029 would go red) → gated, with the amendment, the new test
  prime and the ERRATUM check written down.
- **M3** P4 was deterministic (23 primes either way at 12 × 801) → turned
  into a computed check; P5's arithmetic re-derived (now P3).
- **M4** the Montgomery gate timed the wrong shape and the plan had no
  rule for adopting primes below 2^31 alone → two gates, G1 then G2,
  timed on the T1 kernel; Montgomery's prediction registered by
  amendment if adopted.
- **M5** a faster TL route flips the 7 × 32 cap-flip test on price →
  K* printed in T0, braid replaced in T3, margins recomputed,
  multiplicationes stays nominal.
- **M6** P2 is a formula, the CSR must be counted, the allocation check
  rerun, the header updated → done above.
- **L1** α = −A^(−4ε) noted (the tool asserts it); **L2** DELTA/UNUM
  naming used; **L3** column blocks recorded as a future lever; **L4**
  ≥ 9 pairs, caffeinate; **L5** bracket-from-points step confirmed.
