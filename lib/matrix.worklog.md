# matrix.worklog.md

## 2026-10-06 — natus (v1: determinant, rank, kernel)

`matrix`: exact matrices over any `anulus` — #5 in Fran's math map. API
approved by Fran: generic over a ring descriptor; build-then-freeze
(`matrix_pone` only while building, every operation returns a new
matrix); v1 = construction/text/arithmetic + determinant, rank, kernel;
v2 = Smith and Hermite normal forms over Z with certificates. Text
`[a, b; c, d]` (comma between entries, semicolon between rows — entries
may contain spaces). A matrix with a zero dimension other than 0×0 prints
as "[]" (dimensions lost) — found when the oracle tried to write r×0.

Design:

- **One fraction-free echelon (`_scala`, Bareiss)** serves all three: after
  the pivot ("cardo") at (r, c), each E[i][j] (i > r, j > c) is an (r+2)
  minor, so the division by the previous pivot is exact (Sylvester's
  identity) — also with columns that have no pivot. Determinant = last
  pivot with the row-swap sign; rank = number of pivots.
- **Kernel without division**: for a free column f, z_f = 1; for pivot row
  i backwards (pivot p), z_p = −Σ_{j>p} E[i][j] z_j and every entry set
  so far is multiplied by E[i][p] — earlier equations are homogeneous, so
  they stay satisfied. Entries lie in the ring; over Z they span the
  rational kernel (not a lattice basis — Hermite form, v2), not reduced.
- **Officinae** (four internal scratch piscinae): 0/1 alternate for the
  submatrix still being eliminated, II per-entry temporaries (rolled back
  after every entry), III stable (work table, finished pivot rows, the
  previous pivot). EVERY value that crosses into another piscina goes
  through `anulus->transcribe` — no value can share storage with a
  piscina about to be rolled back (the hazard polynomium's review found).
  Multiplication: each entry summed in the temporary piscina, copied out.
  Fallback if scratch creation fails: all = caller piscina, no rollback.
- `matrix_apex_officinarum()` cost counter, like magnus/polynomium.

Verification:

- **Oracle with different algorithms** (scratchpad matrix/oraculum.py):
  determinant by Leibniz (permutation sum), also over dict-polynomials;
  rank by Fraction elimination (Z, Q; cross-checked against minors up to
  4×4) and by the largest non-zero minor (Z[t]); products. 390 vectors:
  150 determinants (67 zero), 150 ranks (0–4, deficient by construction
  B·C), 90 products; over Z (entries to 10^21), Q, Z[t,t^-1].
- **Knots**: Alexander polynomials from Seifert matrices, det(V − t·Vᵀ)
  built with matrix ops (transpose, multiply by t·I, subtract) then
  `polynomium_normale`: trefoil t² − t + 1, figure-eight t² − 3t + 1,
  cinquefoil T(2,5) t⁴ − t³ + t² − t + 1 — the first real consumer of
  polynomium_divide_exacte.
- **Laws over 300 random cases** (Z, Q, Z[t] in turn; deficient matrices
  by construction): det(AB) = det A·det B, det Aᵀ = det A, row swap via a
  permutation matrix negates det (and P·A ≠ A unless the rows are
  equal — falsifiability), A·I = A, (A+B)−B = A, rank A = rank Aᵀ,
  kernel has columns − rank columns, rank K = that, A·K = 0.
- Hand cases: pivot swaps, a pivot-less middle column, Q 1/60, singular
  Z[t]; errors (dimension, ring mismatch, non-square, exponent bound
  inside elimination) with exitus untouched; 13 malformed texts.
- Memory: det of a 20×20 matrix of ~30-digit integers leaves 228 B in the
  caller's piscina; scratch peak 27 KB (93 KB without the alternating
  rollback — the bound 48 KB tells them apart; at 12×12 the gap was too
  thin: 7.8 vs 13.6 KB).
- **Planted faults, all red**: Bareiss division skipped; swap sign not
  flipped; pivot-less column stops elimination; kernel without rescaling;
  finished row not copied to the stable piscina; alternating scratch not
  rolled back (first SURVIVED — bound too loose; fixed with 20×20);
  aequalis ignoring elements; middle-row comma count unchecked (first
  SURVIVED — only the last row was tested; "[1, 2; 3; 4, 5]" added);
  determinant without sign; product not copied out of the temporary
  piscina (crashed).
- Gate `venenum` now also runs probatio_matrix (poisoned + ASan/UBSan).
- 108 checks; formator CONFORMIS; Latin lint NOVA 0 ("pivot" → *cardo*,
  test locals renamed).

Tooling lesson: I passed `tools/venenum_probare.sh` to `formator.sh
-scribere` with the C files — it rewrapped `[ "$1" -eq 0 ]` across a line
and the C inside a heredoc; `bash -n` still passed. Restored from git.
Never give a .sh to the C formatter (the commit hook does not format .sh).

Deferred: v2 Smith/Hermite over Z with certificates (needs gcd with
Bézout), kernel reduction (content over Z), a threshold to skip scratch
for tiny matrices (benchmarks decide, agenda A3), Q[t] when it exists.

## 2026-10-06 — review I: Gauss–Jordan kernel, small-matrix path, Z[t] note

Reviewer (independent oracle: modular determinants + CRT, Kronecker for
Z[t], rank over three primes; 11k cases, 6k poisoned): no wrong result,
no wrong refusal over Z/Q, no lifetime bug (also with officina creation
failing). Fran approved all fixes.

- **A1, kernel entries grew far beyond need.** My division-free back
  substitution multiplied every entry by each pivot (already a minor):
  25×30 small entries 1207 bits vs 104 primitive. Fix: `_scala(…, plena)`
  — fraction-free GAUSS–JORDAN for the kernel: each step also updates the
  rows ABOVE the pivot (all columns ≠ c; left of c that is a scaling by
  pivot/prior), divisions stay exact, every pivot ends equal to D (the
  last pivot), every entry is a minor. Kernel read off directly: z_f = D,
  z_{p_i} = −E[i][f]. All rows change every step, so in plena mode the
  pivot row goes to the ALTERNATING scratch, not the stable one.
  Measured 20×25 with 30-digit entries: max entry 6326 → 607 digits,
  caller 279 KB → 29 KB, scratch peak 1.4 MB → 42 KB. Tests: ten 6×10
  matrices in −3..3 have every kernel entry ≤ 10^6 (Hadamard (3√6)^6 =
  157464); the 20×25 case bounds digits (< 700), caller (< 64 KB), apex
  (< 128 KB) and checks A·K = 0. The old code fails all four.
  Content reduction (primitive vectors over Z) waits for v2's gcd hook.
- **A2, tiny matrices paid for four piscinae** (2×2 0.5 µs vs 0.007 µs).
  Fix: ≤ 25 elements (multiply: ≤ 125 operations) work in the caller's
  piscina; `_servare` copies the struct there instead of deep-copying.
  Tests: apex 0 for 2×2 det, 5×5 rank, 2×3 kernel, 2×2 multiply; > 0
  for 6×6.
- **A3, Z[t,t^-1] can refuse a well-defined result** (intermediate
  exponent growth: rank [t^(2^29), 1; 1, t^(2^29)] needs t^(2^30); huge
  sparse exponents need polynomium's dense memory). Documented in the
  header; a test pins the refusal (exitus untouched).
- X30 (kernel temporary not rolled back per vector) stays undetected and
  is accepted: its waste is bounded by the kernel's own output size,
  below the elimination's peak.
- My own slips this round: `casus` (latina macro for `case`) as a loop
  variable; a 5×5 test matrix I believed had rank 4 (rows 1 + 5 = 6 × row
  4: rank 3, confirmed by the oracle — the library was right).
- Plants, all red: kernel without Gauss–Jordan; rows above updated only
  right of the pivot; officinae always; `_servare` not copying (crash);
  D = 1. 142 checks; venenum sana.

## 2026-10-06 — review II: results never share the input's memory; size-aware small path

Reviewer: Gauss–Jordan exact and correct (8.5k cases, mostly poisoned;
"all pivots = D" confirmed — the mutant reading D from the FIRST pivot is
equivalent); kernel entries now within ~2 bits of the primitive basis
(25×30: 1207 → 106 vs 104 bits). Two problems with my small-matrix path
(Fran approved both fixes):

- **A1, a regression of mine**: below the threshold `_servare` was a
  struct copy, so a result could point into the INPUT's piscina —
  [7^200] built in P1, det into P2, P1 destroyed → use-after-free (ASan).
  Fix: `_effectus` — determinant, kernel and product entries are ALWAYS
  deep-copied into the caller's piscina, at every size; `_servare` (struct
  copy on the small path) stays for internal values only. Header VITA
  paragraph now states the real contract: elimination and multiplication
  results own their memory; transpose/add/subtract/pone copy element
  structs and may share (as before, now written down).
- **A2, the threshold counted elements only** (5×5 with 1000-digit
  entries: 204 KB in the caller for a 2 KB determinant, 475 KB for an
  EMPTY kernel; polynomium's lesson again). Fix: new ring predicate
  `anulus->parvum` (element stored inline — Z fits s64, Q numerator and
  denominator fit, Z[t] only zero); the small path needs ≤ 25 elements
  (multiply ≤ 125 operations) AND all entries parvum. With the predicate
  the A1 sharing can no longer arise on the small path either (inline
  values own no storage) — `_effectus` keeps the contract explicit.
- Measured whether to raise the 25 limit for inline entries (forced small
  path vs officinae, det): 1-digit 6×6 1.71 vs 2.31 µs, 12×12 15.6 vs
  18.3 µs — but with inline entries near 2^62 the caller waste is 10 KB
  at 6×6, 31 KB at 8×8, 157 KB at 12×12 ("inline" bounds the entries, not
  the minors). Kept 25.
- Tests: the reviewer's sharing case (result limbs differ from the
  input's; value survives destroying the input piscina); 5×5 with
  1000-digit entries — det caller < 16 KB, empty kernel < 4 KB, square
  < 48 KB, apex > 0; parvum per ring incl. a fraction with a small
  numerator and a BIG denominator (the plant dropping the denominator
  check first survived). The previous matrix.c fails the sharing test and
  all three waste bounds.
- Plants, all red: results copied shallow (crash); parvae always true;
  multiply ignoring entry size; Z parvum always true; Q parvum without
  the denominator. 163 checks; venenum sana.
