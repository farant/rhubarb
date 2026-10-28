# Demo 135: exact ranks of the singlet pieces (plan)

Status: DRAFT (2026-10-27), for Fran and a design review before any
data. Context: project-specs/crossings-contraction-counting-brainstorm.md
§2.1 item 9 (ANT's one-bit form), §11 "Singlet pieces in closed form"
(the derivation this demo checks), §12.1.1 (finite-size cost curves).

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
exactly, in two independent ways, at sizes nobody has looked at, and
tests a prediction the formula makes that floating point could never
see: ranks mod small primes.

## Definitions

Bits: x, y, u, v ∈ {0, 1}^λ; bit j of each is x_j, … .

- Per bit: E = [x_j = u_j][y_j = v_j], S = [x_j ≠ y_j](−1)^(x_j) ·
  [u_j ≠ v_j](−1)^(u_j). The crossing per bit is E − S (ANT Appendix H;
  checked by brute force in §11).
- Piece: A_i(x, y, u, v) = (−1)^i Σ_{T ⊆ {1..λ}, |T| = i} Π_{j∈T} S_j
  Π_{j∉T} E_j. Integer valued. Σ_i A_i = the λ-bit crossing.
- Rows matrix R_i: row index (x, u), column index (y, v). Columns
  matrix C_i: row index (x, y), column index (u, v). Both 4^λ × 4^λ.
  Index layout: per bit a pair (first, second) → 2·first + second, bit
  pairs concatenated low bit first; the same layout for rows, columns
  and every basis tensor.
- Rows basis per bit: a = |00⟩ + |11⟩ (special); a⊥ = {|00⟩ − |11⟩,
  |01⟩, |10⟩}. Columns basis per bit: σ = |01⟩ − |10⟩ (special);
  σ⊥ = {|00⟩, |11⟩, |01⟩ + |10⟩}. A basis tensor b is a tensor product
  of one vector per bit; d(b) = number of positions not special.
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
  (scratch `praedictio.py`); no matrix was built mod any prime.
- Nothing at λ ≥ 5 has been computed in any arithmetic.

## Registered predictions

Λ = the largest λ the run reaches (T0 sets it; at least 5, 6 if
feasible). "Holds" means every case matches exactly; one mismatch
means the prediction falls, and the findings name the case.

- **R1 (pieces sum).** Σ_i A_i equals the λ-bit crossing at every
  (x, y, u, v), for λ = 1 … Λ.
- **R2 (diagonal).** For every λ ≤ Λ, every i, every basis tensor b:
  rows: R_i·b = μ_i(b)·S^⊗λ·b; columns: C_i·b = μ'_i(b)·b. Exact
  integers. Since the basis matrix has determinant ±2^k ≠ 0, R2 alone
  proves rank R_i = #{b: μ_i(b) ≠ 0} = F(λ, i) over Q, and the same mod
  every odd prime with "≠ 0" read mod p.
- **R3 (rank over Q, second route).** rank R_i = F(λ, i) and rank C_i =
  F(λ, λ − i), by `matrix_gradus` over Z (exact Q rank) for λ ≤ 4 and
  over Z/p for p = 2^31 − 1 (a lower bound on Q rank) for 5 ≤ λ ≤ Λ.
- **R4 (small primes).** rank mod p of R_i and C_i, for p ∈ {3, 5, 7}
  and λ ≤ Λ, equals the count of basis tensors with μ ≢ 0 (mod p):
  rows Σ_{d≤i, p ∤ C(λ−d, i−d)} C(λ, d)·3^d, columns
  Σ_{d≤λ−i, p ∤ C(λ−d, i)} C(λ, d)·3^d. Primes above λ predict no drop.
  The cases with drops (λ ≤ 6) are:

  | λ | p | rows (i = 0 … λ) | Q value differs at |
  | --- | --- | --- | --- |
  | 3 | 3 | 1, 9, 36, 64 | i = 1, 2 (10, 37) |
  | 4 | 3 | 1, 13, 54, 163, 256 | i = 2, 3 (67, 175) |
  | 5 | 3 | 1, 16, 106, 271, 691, 1024 | i = 3, 4 (376, 781) |
  | 5 | 5 | 1, 15, 105, 375, 780, 1024 | i = 1 … 4 |
  | 6 | 3 | 1, 18, 153, 694, 1233, 2826, 4096 | i = 1, 2, 4, 5 |
  | 6 | 5 | 1, 19, 135, 675, 1890, 3349, 4096 | i = 2 … 5 |

  Columns are the rows reversed (i ↔ λ − i) in every case. p = 7 at
  λ ≤ 6: no drop.

**Descriptive (no prediction):** ranks mod 2 (the basis is singular
there, so the derivation says nothing); the cost curve of §11 item 6,
reprinted from F for λ ≤ 100 beside the verified range.

## Method

- One C file (`main.c`, K&R demo style, house headers allowed, frozen
  with `knotapel/archive.sh` when done). Matrices built from the
  indicator DEFINITIONS (loop over T), not from the E/S matrix forms.
- R1, R2: direct integer loops (no library). R2 costs 4^λ basis
  tensors × a 4^λ matrix-vector product per piece and direction:
  about 2·(λ+1)·16^λ multiply-adds (λ = 6: 2^29, seconds).
- R3, R4: `matrix_gradus` over `anulus` Z and Z/p. T0 measures it on
  1024² and 4096²; if it is too slow at 4096², λ = 6 uses a plain mod-p
  elimination in the demo (congruentia operations), cross-checked
  against `matrix_gradus` at λ ≤ 5.
- Oracle: `oracle_d135.py`, written independently: builds the
  matrices from the indicator definitions and computes exact ranks mod
  p by its own elimination (pure Python, λ ≤ 4; λ = 5 if it runs in
  minutes), and Q ranks by fractions at λ ≤ 3. The demo's printed
  ranks must equal the oracle's wherever both run.
- Plants (each must turn a named prediction red):
  1. S with the opposite sign (E + S) → R1;
  2. rows eigenvalue C(λ − d, i) instead of C(λ − d, i − d) → R2, R4;
  3. a⊥'s first vector |00⟩ + |11⟩ (basis singular) → R2;
  4. rows and columns cuts swapped when building matrices → R3;
  5. small-prime rule p ∤ C(λ, i) instead of p ∤ C(λ − d, i − d) → R4
     at λ = 4, p = 3.

## Tasks

- **T0. Feasibility.** Time `matrix_gradus` on random integer 1024²
  and 4096² matrices over Z/p, and over Z at 256²; set Λ and the
  method per size; report. No piece matrix is built.
- **T1. Build.** main.c with R1–R4 and the descriptive rows, the
  oracle, the plants. Checks run only at λ ≤ 2 (hand-checkable: λ = 1
  pieces are 4 × 4) until the plants are red.
- **T2. Registered run.** λ = 1 … Λ, verdicts, findings.md, archive.
- **T3. Review** (a reviewer with its own oracle, as for D132–D134).

## Open for Fran

1. Home: `knotapel/demo_135_singlet_ranks/` (the crossings program has
   no demo folder of its own; D130–D134 already sit in knotapel).
2. Λ: 5 at least, 6 if T0 says it is minutes, not hours.
3. Design review before T0 (proposed: a short one; the risk here is a
   circular check, e.g. R2 and the prediction sharing one formula
   implementation, which the oracle and plants 2 and 5 are meant to
   catch).
