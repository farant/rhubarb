# Demo 135: exact ranks of the singlet pieces (plan)

Status: v2 (2026-10-27) after a DESIGN REVIEW before any data
(consiliarius-d135; summarised in "Design review" at the end). v1 was
approved by Fran (2026-10-27) with the defaults of its "Open for Fran":
home `knotapel/demo_135_singlet_ranks/`, a design review before T0.
v2 fixes Λ = 6 in advance (review M3). Context:
project-specs/crossings-contraction-counting-brainstorm.md §2.1 item 9
(ANT's one-bit form), §11 "Singlet pieces in closed form" (the
derivation this demo checks), §12.1.1 (finite-size cost curves).

## Question

ANT splits a λ-bit crossing into λ+1 signed pieces A_0 … A_λ, each
cheap in one merge direction. Brainstorm §11 derives their ranks in
closed form:

- rows cut: rank A_i = F(λ, i); columns cut: rank A_i = F(λ, λ − i);
  F(λ, k) = Σ_{d≤k} C(λ, d)·3^d;
- reason: after a per-bit change of basis each piece is DIAGONAL, with
  eigenvalue C(λ − d, i − d)·2^(λ−i) (rows) or C(λ − d, i)·2^i
  (columns) on a basis tensor with d positions outside the special
  direction.

If that is right, the whole cost curve (§11 item 6) is known for every
λ without building a matrix, and the next experiment (real tables) can
be designed against exact numbers. This demo checks the derivation
exactly, in independent ways, at a size nobody has computed (λ = 6),
and tests a prediction floating point could never see: ranks mod small
primes.

## Definitions

Bits: x, y, u, v ∈ {0, 1}^λ; bit j of each is x_j, … .

- Per bit: E = [x_j = u_j][y_j = v_j], S = [x_j ≠ y_j](−1)^(x_j) ·
  [u_j ≠ v_j](−1)^(u_j). The crossing per bit is E − S (ANT Appendix H;
  checked by brute force in §11).
- Piece: A_i(x, y, u, v) = (−1)^i Σ_{T ⊆ {1..λ}, |T| = i} Π_{j∈T} S_j
  Π_{j∉T} E_j. Integer valued.
- Rows matrix R_i: row index (x, u), column index (y, v). Columns
  matrix C_i: row index (x, y), column index (u, v). Both 4^λ × 4^λ.
  Index layout: per bit a pair (first, second) → 2·first + second, bit
  pairs concatenated low bit first; the same layout for rows, columns
  and every basis tensor.
- Rows basis per bit: a = |00⟩ + |11⟩ (special); a⊥ = {|00⟩ − |11⟩,
  |01⟩, |10⟩}. Columns basis per bit: σ = |01⟩ − |10⟩ (special);
  σ⊥ = {|00⟩, |11⟩, |01⟩ + |10⟩}. A basis tensor b is a tensor product
  of one vector per bit, one for each of the 4^λ digit tuples; d(b) =
  number of positions not special.
- Predicted eigenvalues: rows μ_i(b) = (−1)^i·C(λ − d, i − d)·2^(λ−i)
  (0 if d > i); columns μ'_i(b) = (−1)^i·C(λ − d, i)·2^i (0 if
  d > λ − i).
- S^⊗λ: the per-bit matrix s ⊗ s, s = [0, 1; −1, 0], tensored λ times
  (rows layout).

## What was seen before registration (disclosure)

- The formula was FOUND by fitting the four worst-piece products
  measured earlier on 10-27 (4, 49, 370, 4,489, numpy floating-point
  ranks, λ ≤ 4), then checked against every piece (both directions, all
  i) at λ ≤ 4, still in floating point (scratch `rangi.py`). So λ ≤ 4
  over Q is NOT a test; it is replication in exact arithmetic.
- The mod-p tables below were computed from the formula alone
  (scratch `praedictio.py`).
- (v2) The DESIGN REVIEWER computed, independently and exactly: R1 and
  R2 at λ ≤ 4; ranks at λ ≤ 5, both cuts, every i, mod 3, 5, 7 and
  2^31 − 1; and compared every entry of the registered table with its
  own formula code. It reported match/mismatch only, never the numbers:
  "MATCH at every case". So λ ≤ 5 is now seen-by-proxy (a confirmation
  leaked, no value did). **λ = 6 is the only size nobody has computed**,
  in any arithmetic.

## Registered predictions

**Λ = 6, fixed now** (v2). T0 chooses only the METHOD per λ. A case at
λ ≤ 6 that does not finish (crash, memory guard, abandoned) is reported
as "not tested", never dropped. "Holds" means every case matches
exactly; one mismatch means the prediction falls, and the findings name
the case.

- **R1 (pieces sum).** Σ_i A_i(x, y, u, v) = [x = v][y = u] compared on
  WHOLE λ-bit words (not against the product of per-bit E − S), at every
  (x, y, u, v), for λ = 1 … 6.
- **R2 (diagonal).** For every λ ≤ 6, every i, every basis tensor b:
  rows: R_i·b = μ_i(b)·S^⊗λ·b; columns: C_i·b = μ'_i(b)·b. Exact
  integers. R_i and C_i are built from the indicator definitions, never
  from E/S matrix forms; μ is computed by a function separate from the
  matrix builder.
- **R2b (the basis is a basis).** From the SAME vector arrays R2 uses:
  det of the per-bit rows basis matrix and of the columns basis matrix
  are each ±2 (nonzero, odd primes invert it); the basis tensors are
  enumerated over all 4^λ digit tuples (each tuple once). With R2, this
  proves rank R_i = #{b: μ_i(b) ≠ 0} = F(λ, i) over Q and over every
  odd prime with "≠ 0" read mod p. (Without R2b a duplicated basis
  vector would leave R2 green and the argument empty: review H1.)
- **R3 (ranks by elimination).** λ ≤ 4: rank over Z (= Q rank) by
  `matrix_gradus` equals F(λ, i) rows, F(λ, λ − i) columns. 5 ≤ λ ≤ 6:
  rank mod 2^31 − 1 equals F (a LOWER bound on Q rank; the Q equality
  comes from R2 + R2b).
- **R4 (small primes).** rank mod p of R_i and C_i, for p ∈ {3, 5, 7}
  and λ ≤ 6, equals the LITERAL table below (hard-coded in the demo;
  measured ranks are compared with it). Separately the demo asserts its
  formula code (rows Σ_{d≤i, p ∤ C(λ−d, i−d)} C(λ, d)·3^d; columns
  Σ_{d≤λ−i, p ∤ C(λ−d, i)} C(λ, d)·3^d) reproduces the whole table,
  including every no-drop case. Primes above λ predict no drop. The
  cases with drops are:

  | λ | p | rows (i = 0 … λ) | Q value differs at |
  | --- | --- | --- | --- |
  | 3 | 3 | 1, 9, 36, 64 | i = 1, 2 (10, 37) |
  | 4 | 3 | 1, 13, 54, 163, 256 | i = 2, 3 (67, 175) |
  | 5 | 3 | 1, 16, 106, 271, 691, 1024 | i = 3, 4 (376, 781) |
  | 5 | 5 | 1, 15, 105, 375, 780, 1024 | i = 1 … 4 |
  | 6 | 3 | 1, 18, 153, 694, 1233, 2826, 4096 | i = 1, 2, 4, 5 |
  | 6 | 5 | 1, 19, 135, 675, 1890, 3349, 4096 | i = 2 … 5 |

  Every other (λ, p) at λ ≤ 6 with p ∈ {3, 5, 7}: the Q values
  F(λ, i). Columns are the rows reversed (i ↔ λ − i) in every case.

**Known blind spot (review M1):** every term of one entry of A_i has
the same sign, (−1)^i·(−1)^(popcount(x ⊕ u)); for the rows cut it
depends on the row only, for the columns cut it splits into row × column
factors. So |A_i| has the same ranks as A_i: R3 and R4 cannot see signs
(a wrong reduction of −1 mod p, or plant 1, is invisible to them). R1
and R2 are the sign-sensitive checks. The demo also asserts that every
residue lies in [0, p).

**Descriptive (no prediction):** ranks mod 2 (the basis is singular
there); the cost curve of §11 item 6, reprinted from F for λ ≤ 100
beside the verified range; for the next experiment (review item 4):
both cuts' factor matrices printed at λ ≤ 2 for hand reference, the
denominators of the eigenbasis factorization (B⁻¹ = diag(½, ½, 1, 1)·Bᵀ,
so entries lie in Z[½], up to 2^i rows, 2^(λ−i) columns), and the sign
gauge A_i = ±D·N_i with N_i ≥ 0 counting the contributing T, checked
at every λ.

## Method

- One C file (`main.c`, K&R demo style, house headers allowed, frozen
  with `knotapel/archive.sh` when done).
- R1, R2, R2b: direct integer loops (no library). R2 costs about
  2·(λ+1)·16^λ multiply-adds (λ = 6: 2^29, seconds).
- R3, R4: `matrix_gradus` over `anulus` Z and Z/p where T0 shows it is
  fast enough. The review expects it is not at λ = 6: over Z/n it calls
  `congruentia_inversa` on every Bareiss update (lib/anulus.c:656-666),
  about 2·10^10 at 4096². Fallback: a plain mod-p row elimination in the
  demo (one inverse per pivot, cost ~ n²·rank), cross-checked against
  `matrix_gradus` at λ ≤ 5. One piece at a time, each λ = 6 run under
  `tools/custos.sh`.
- Oracle: `oracle_d135.py`, written in T1 independently of main.c:
  numpy, vectorized row elimination mod p, matrices built from the
  indicator definitions (Kronecker products of per-bit 4 × 4 matrices
  allowed for λ ≥ 5, identical to the full indicator loop at λ ≤ 4).
  Covers ranks mod 3, 5, 7, 2^31 − 1 at λ ≤ 6 (λ = 6: tens of minutes,
  per the review). The demo's ranks must equal the oracle's everywhere.
  (The reviewer's own scratch oracle stays a third route for T3.)
- Plants (each must turn the named check red; expected results for the
  others stated where they matter):
  1. S with the opposite sign (E + S) → R1 and R2 red; R3/R4 green
     (blind spot);
  2. rows eigenvalue C(λ − d, i) instead of C(λ − d, i − d) → R2 red;
  3. a⊥'s first vector replaced by |00⟩ + |11⟩ → R2 red, R2b red;
  4. rows and columns cuts swapped when building matrices → R3 red;
  5. small-prime rule p ∤ C(λ, i) instead of p ∤ C(λ − d, i − d) →
     "formula code = table" red (R4 itself compares with the literal
     table and stays green);
  6. a⊥ = {|00⟩ − |11⟩, |01⟩, |01⟩} (duplicate) → R2 GREEN, R2b red;
  7. |A_i| in place of A_i → R2 red; R3/R4 green (blind spot recorded).
  Not planted: swapping the order inside every bit pair everywhere at
  once (a symmetry: a, the set a⊥ and s ⊗ s are unchanged, σ flips
  sign); a swap in one place only is already caught by R2.

## Tasks

- **T0. Feasibility.** Time `matrix_gradus` on random full-rank integer
  1024² and 4096² matrices over Z/p, and over Z at 256²; write the
  method per λ into this plan as a dated amendment before T1. No piece
  matrix is built.
  **T0 amendment (2026-10-27, measured; no piece matrix built).**
  Random full-rank matrices (the worst case), this Mac, -O2, scratch
  program `d135_t0/tempora.c`, each run under `tools/custos.sh`:

  | method | n | time | peak memory |
  | --- | --- | --- | --- |
  | `matrix_gradus` over Z, entries in [−20, 20] | 64 / 128 / 256 | 0.02 / 0.33 / 8.6 s | 10 MB |
  | `matrix_gradus` over Z/(2^31 − 1) | 256 / 512 / 1024 | 0.34 / 3.0 / 22.4 s | 17 MB |
  | plain mod-p elimination (one inverse per pivot) | 512 / 1024 / 4096 | 0.09 / 0.71 / 43.2 s | 65 MB |

  `matrix_gradus` mod p grows ~n³ (about 27 min per 4096² matrix,
  extrapolated; ~25 h for λ = 6's 56 matrices), as the review expected.
  Method per λ, therefore:
  - λ ≤ 4: R3 over Z by `matrix_gradus` (≤ 10 matrices of 256², under
    ~1.5 min); R3/R4 mod p by BOTH `matrix_gradus` and the plain
    elimination, which must agree on every matrix (the cross-check);
  - λ = 5: plain elimination for every prime; `matrix_gradus` also at
    p = 2^31 − 1 for all 12 matrices (cross-check, ≤ ~4.5 min);
  - λ = 6: plain elimination only (56 matrices of 4096², ≤ ~40 min worst
    case, less for low-rank pieces since cost ~ n²·rank), one matrix at
    a time, the run detached and under `tools/custos.sh`.
  The plain elimination is the demo's own code; its agreement with
  `matrix_gradus` at λ ≤ 5 and with the numpy oracle at λ ≤ 6 is what
  makes the λ = 6 ranks trustworthy. Λ stays 6.
- **T1. Build.** main.c with R1–R4, R2b and the descriptive rows, the
  oracle, the plants. Checks run only at λ ≤ 2 (hand-checkable: λ = 1
  pieces are 4 × 4) until the plants behave as stated.
  **T1 amendment (2026-10-27, built; runs at λ ≤ 2 only).**
  - Correction to Method: R2's cost is not 2·(λ+1)·16^λ. Checking all
    4^λ basis tensors with dense matrix-vector products is 64^λ (about
    7·10^10 per piece at λ = 6). main.c exploits that each basis tensor
    has at most 2^λ nonzeros (average about 1.5^λ): about 16^λ·1.5^λ per
    piece, seconds at λ = 6. The check itself is unchanged.
  - main.c: R1, R2, R2b, R3, R4, formula = table, cross-check
    (plain elimination against `matrix_gradus` where both run), residues
    in [0, p), sign gauge (descriptive), ranks mod 2, cost curve, factor
    matrices. `D135_LAMBDA=2`: 51 checks pass; the λ = 1 matrices were
    checked by hand against the definitions (rows piece 0 = ones where
    x = u and y = v; rows piece 1 = the signed anti-diagonal −S).
  - oracle_d135.py (numpy; per-bit products on a 4-D array, plain index
    order): at λ ≤ 2 all 50 demo ranks agree (all five primes), its own
    R1 passes.
  - Plants (silva.planta, gate = rebuild + run at λ ≤ 2; driver and log
    in build/plantae_d135/, not committed), all seven as stated:
    1 → R1, R2 (+ sign gauge) red, R3/R4 green; 2 → R2 red, R3/R4/
    formula green; 3 → R2, R2b red; 4 → R3 red (also R2, R4); 5 →
    formula = table red, R4 green; 6 → R2b red, **R2 green**; 7 → R2 red
    (also R1 and the gauge: |A_i| no longer sums to the crossing; the
    plan was silent on R1), R3/R4 green. main.c restored byte-identical.
  **Before T2 (2026-10-27): memory measured, nothing seen.** At Fran's
  question (keep under 10-15 GB), the demo ran to λ ≤ 5 and the oracle
  to λ ≤ 5 under `tools/custos.sh` with ALL output sent to /dev/null
  (only the custos line read): demo peak 31 MB, oracle 79 MB. λ = 6
  estimate from array sizes (16× per λ): demo ~200 MB (one 4096² piece
  matrix of 8-byte entries + its residues), oracle ~1.3 GB. T2 caps:
  custos 2 GB (demo), 4 GB (oracle).
- **T2. Registered run.** λ = 1 … 6, verdicts, findings.md, archive.
- **T3. Review** (a reviewer with its own oracle, as for D132–D134).

## Design review (2026-10-27, consiliarius-d135, before any data)

Verdict: derivation right, design sound; independent exact check
"MATCH at every case" (λ ≤ 5, four primes; numbers withheld). Findings
and what v2 did:

- H1: R2's rank argument needs the basis to be a basis; a duplicated
  vector passes R2 → R2b and plant 6.
- H2: the reviewer's computation made "nothing at λ ≥ 5 computed"
  false → disclosure updated; λ = 6 is the unseen size.
- M1: signs invisible to rank checks (no cancellation inside an entry,
  sign gauge) → stated blind spot, plant 7, residues asserted in
  [0, p), plant 1's expected results.
- M2: R4's reference ambiguous → literal table hard-coded, separate
  "formula = table" check (plant 5 targets it).
- M3: "Λ = the largest λ reached" was decided after the run → Λ = 6
  fixed now; unfinished cases reported "not tested".
- M4: R3 at λ ≥ 5 is a lower bound → relabelled.
- L1: R1 against whole-word equality; L2: the in-pair swap is a
  symmetry, not a plant; L3: numpy oracle reaches λ = 6.
- Item 4 (next experiment): factorization denominators (Z[½]), sign
  gauge, λ ≤ 2 factor matrices → descriptive outputs.
- Item 5: matrix_gradus over Z/n likely hours at 4096² → fallback
  elimination planned, measured in T0.
