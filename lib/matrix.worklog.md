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
