# Demo 135: Exact Ranks of the Singlet Pieces — Findings

Plan: `plan.md` (v1 approved by Fran 2026-10-27; v2 after a DESIGN
REVIEW before any data, consiliarius-d135; T0, T1 and pre-T2
amendments). Context: project-specs/crossings-contraction-counting-
brainstorm.md §2.1 item 9 (ANT's one-bit identity), §11 "Singlet pieces
in closed form" (the derivation checked here), §12.1.1 (finite-size
cost curves).

ANT (Appendix H) writes a one-bit crossing as [x = v][y = u] =
[x = u][y = v] − [x ≠ y](−1)^x [u ≠ v](−1)^u. For λ bits the product
expands into 2^λ terms; grouping them by the number i of bits taking the
second ("singlet") term gives λ+1 pieces A_0 … A_λ, each cheap in one
merge direction. Brainstorm §11 derived, by hand after fitting four
measured numbers, that each piece is diagonal in a per-bit rotated basis,
so that **rows-cut rank A_i = F(λ, i), columns-cut rank = F(λ, λ − i),
F(λ, k) = Σ_{d≤k} C(λ, d)·3^d**, with eigenvalues C(λ − d, i − d)·2^(λ−i)
(rows, after factoring out S^⊗λ) and C(λ − d, i)·2^i (columns).

**Everything registered holds, exactly, at every λ ≤ 6, both cuts,
every piece.** The pieces sum to the crossing (R1); every piece is
diagonal with exactly the predicted eigenvalues on all 4^λ basis tensors
(R2), and the basis is a basis (R2b), which together prove the rank
formula over Q and over every odd prime for each λ checked; elimination
gives the same ranks over Z and mod 2^31 − 1 (R3); and the ranks mod 3,
5 and 7 equal the registered literal table (R4) - including every
predicted DROP, which floating point could never have shown, e.g. at
λ = 6 mod 3 the rows ranks are 1, 18, 153, 694, 1233, 2826, 4096
against 1, 19, 154, 694, 1909, 3367, 4096 over Q, and mod 7 nothing
drops. λ = 6 had not been computed in this project, in any arithmetic,
before this run (the design reviewer had confirmed λ ≤ 5 by proxy,
reporting match/mismatch only); all 70 of its ranks agree with the
independent numpy oracle and with the T3 reviewer's own C oracle.

So the cost curve of §11 item 6 stands on a checked formula. Two
measures, not to be confused:
- **one direction** (what a contraction pays): the pieces, each in its
  cheaper direction, cost Σ_i min(F(λ, i), F(λ, λ − i)) against the
  crossing's 4^λ: 2^17.0 against 2^20 at λ = 10 (8×), 2^52.3 against 2^60
  at λ = 30 (200×; formula only above λ = 6);
- **both directions** (ANT's accounting): the worst piece's row rank ×
  column rank stays below 12^λ, 3.03 bits per bit at λ = 4 and 3.43 at
  λ = 30 (formula), against 4 for the crossing.

**At the level of ranks, ANT's one-bit decomposition saves at every λ**
(computed for λ ≤ 60; for λ ≥ 22, Σ min ≤ (λ + 1)·F(λ, ⌊λ/2⌋) ≤
(λ + 1)·2^λ·3^(λ/2) < 4^λ). The saving is not monotone at small λ (2,
1.78, 2.91, 2.69, 4.16, 3.93 for λ = 1 … 6). **Whether real tables can
realize those ranks is the next experiment.**

## Result

1. **The closed form is right** [checked exactly for λ ≤ 6; derived
   for all λ]. R2 + R2b at λ ≤ 6 are a machine check of the derivation's
   key step (diagonal in the rotated basis, eigenvalues as stated) for
   every piece; the general-λ statement rests on the hand derivation
   (brainstorm §11 items 1-2), confirmed by the design reviewer and
   re-derived independently by the T3 reviewer (S·a = a, (s⊗s)² = I,
   a·aᵀ = 2 on a and 0 on a⊥, hence eigenvalue 2^(λ−i)·#{T ⊇ D}).
2. **The small-prime drops are exactly the predicted ones** [checked].
   A rank falls mod p precisely where p divides the binomial in the
   eigenvalue (Lucas), and nowhere else: 36 registered (λ, cut, p) rows,
   0 mismatches. Primes above λ - which includes every prime the
   contraction engine uses (~2^31) - see the full rank.
3. **λ = 6 tables** (rows; columns are these reversed):

   | | i = 0 | 1 | 2 | 3 | 4 | 5 | 6 |
   | --- | --- | --- | --- | --- | --- | --- | --- |
   | Q (= mod 2^31 − 1, mod 7) | 1 | 19 | 154 | 694 | 1909 | 3367 | 4096 |
   | mod 3 | 1 | 18 | 153 | 694 | 1233 | 2826 | 4096 |
   | mod 5 | 1 | 19 | 135 | 675 | 1890 | 3349 | 4096 |
   | mod 2 (descriptive) | 1 | 14 | 92 | 364 | 1092 | 2016 | 4096 |

4. **Mod 2** [descriptive, no prediction; the rotated basis is singular
   there, so neither the derivation nor R2 + R2b says anything]. For
   2 ≤ λ ≤ 6, ranks drop below Q at every 0 < i < λ. Two regularities
   hold at every 2 ≤ λ ≤ 6 (rows): rank of A_1 = 2λ + 2 (Q: 3λ + 1), and
   rank of A_{λ−1} = 2^(λ−1)·(2^λ − 1) (6, 28, 120, 496, 2016; the second
   found by the T3 reviewer); and the columns ranks are the rows ranks
   reversed. All observed only, λ ≤ 6; not interpreted.
5. **For the next experiment** [descriptive]. The factorization is
   explicit and lives in Z[½]: per bit the rows basis has Gram matrix
   diag(2, 2, 1, 1), the columns basis diag(2, 1, 1, 2), so B⁻¹ = Gram⁻¹·Bᵀ
   (the plan's "B⁻¹ = diag(½, ½, 1, 1)·Bᵀ" was the rows case only; for
   columns it is diag(½, 1, 1, ½)·Bᵀ). Denominators of the coefficients
   μ/‖b‖² are at most 2^i (rows) and 2^(λ−i) (columns), by the Gram
   matrices (stated, not printed by the demo).
   **No cancellation inside an entry** [proved; sign checked]: a nonzero
   term needs x_j = u_j at every bit outside T, so popcount((x ⊕ u) ∧ T)
   = popcount(x ⊕ u) and every nonzero term of an entry has the same sign
   (−1)^i·(−1)^popcount(x ⊕ u). The demo checks the weaker statement that
   every nonzero ENTRY has that sign (λ ≤ 6), not the plan's stronger
   "A_i = ±D·N_i with N_i counting T" (not implemented; the one-line proof
   above gives it). A table-level experiment can therefore carry unsigned
   counts plus a sign gauge.

## Registered predictions

| | Prediction | Verdict |
| --- | --- | --- |
| R1 | Σ_i A_i = [x = v][y = u], whole words, λ ≤ 6 | HOLDS (6 of 6 λ; oracle agrees) |
| R2 | R_i·b = μ_i(b)·S^⊗λ·b, C_i·b = μ'_i(b)·b, all b, λ ≤ 6 | HOLDS (12 of 12) |
| R2b | per-bit determinants ±2 (rows −2, columns +2); all 4^λ tuples | HOLDS (2 determinants + 12 enumeration counts, which cannot fail by construction) |
| R3 | rank over Z (λ ≤ 4) and mod 2^31 − 1 (λ = 5, 6) = F | HOLDS (12 of 12) |
| R4 | ranks mod 3, 5, 7 = literal table, λ ≤ 6 | HOLDS (36 of 36) |
| - | formula code = table (all λ ≤ 6, p ∈ {Q, 3, 5, 7}, both cuts) | HOLDS |

R3 at λ ≥ 5 is a lower bound on the Q rank; the Q equality there comes
from R2 + R2b, as registered. Ranks cannot see signs (plan, "Known blind
spot"): R1 and R2 are the sign-sensitive checks, and both hold.

## What was seen before the run

- λ ≤ 4 over Q: the formula was fitted to the four worst-piece products
  and checked against every piece in floating point (scratch), so λ ≤ 4
  over Q is replication in exact arithmetic, not a test.
- λ ≤ 5, all four primes: the design reviewer computed them
  independently and reported only "MATCH at every case".
- Memory measurements (pre-T2 amendment) ran demo and oracle to λ ≤ 5
  with all output discarded.
- λ = 6: nothing, in any arithmetic, before this run.

## Checks (133, all pass)

R1 6, R2 12, R2b 14, R3 12, R4 36, formula = table 1, cross-check 34
(the demo's own mod-p elimination against `matrix_gradus` on every
matrix where both ran: λ ≤ 4 at 3, 5, 7, 2^31 − 1; λ = 5 at 2^31 − 1),
residues in [0, p) 12, sign gauge 6 (descriptive). Oracle
(`oracle_d135.py`, numpy, per-bit products on a 4-D array, plain index
order): its 270 ranks (λ ≤ 6, five primes, both cuts, every i) equal the
demo's 270 plain-elimination ranks - recorded by a rerun after the T3
review, `oracle_d135.py 6 --compare build/d135_t2/demo.out`: "COMPARE:
270 demo ranks checked against the oracle, 0 mismatches" (the first
comparison, of the 270 plain and the 124 shared `matrix_gradus` ranks,
was a scratch script with no recorded output) - and its own R1 holds at
every λ. The T3 reviewer's oracle (own C, per-bit Kronecker recursion,
own elimination; R2 as a full matrix identity in numpy) agrees
everywhere: see "Review (T3)".

## Plants (silva.planta, T1, all seven as the plan stated)

1 S with the opposite sign → R1, R2 red, R3/R4 green (blind spot);
2 wrong rows eigenvalue → R2 red; 3 a⊥ vector replaced by a → R2, R2b
red; 4 cuts swapped → R3 red (also R2, R4); 5 wrong small-prime rule →
formula = table red, R4 green (it compares the literal table); 6
duplicated basis vector → R2b red, R2 GREEN (the review's H1); 7 |A_i| →
R2 red (also R1), R3/R4 green (blind spot).

## Not covered

- General λ is the hand derivation, not this demo; λ ≤ 6 is what the
  machine checked.
- Ranks are not table sizes. Nothing here says a contraction can store
  piece A_i at size F(λ, i); the surviving states are a sector (at most i
  positions outside the special direction, after a per-bit rotation), not
  a product of indices. That is the next experiment (brainstorm §12.1.1,
  §4.7: sector-labelled tensors, a λ-bit gadget with real tables).
- Only the singlet pieces. The eight-square (Lemma 2) and Appendix B
  pieces are not touched.
- One gadget: one cut crossed by λ crossing bits. Composition in a
  network (12.1.3, one tree against many) is untouched.

## Run

`D135_TEMPORA=1 tools/custos.sh build/d135_t2/main` (binary built from
commit 2f35af12), detached; λ = 1 … 6 in 0.0, 0.0, 0.0, 2.1, 51.8, 18.5
s CPU (λ = 5 includes `matrix_gradus` at 2^31 − 1, which treats the
matrix as dense; λ = 6 runs the demo's own elimination only). The λ = 6
eliminations took seconds where T0's dense random matrices predicted up
to ~40 min: the pieces are sparse - an entry is nonzero only if every
bit fits the E or the S pattern, 6 of 16 settings, so at most (6/16)^λ of
the entries (0.28% at λ = 6; the densest piece, i = 3, measured by the T3
reviewer at 37,312 of 16.8M, 0.22%) - and their structure keeps fill-in
low; the elimination skips rows whose pivot-column entry is zero.
Peak memory 225 MB (custos, compressed included; cap 2 GB). Oracle
`python3 -I oracle_d135.py 6`: peak 1,030 MB (cap 4 GB). Frozen copy:
`demo-snapshot.c` (`knotapel/archive.sh`).

## Review (T3, 2026-10-27, recensor-d135)

No H: every registered verdict stands and every number recomputed
matches. Own oracle (C, shares no code with main.c or the oracle):
R1 whole-word λ ≤ 6, 0 wrong; 270 ranks (λ ≤ 6, five primes, both cuts,
every i) = demo plain 270/270, demo `matrix_gradus` mod p 124/124,
oracle_d135 270/270; demo Z ranks (λ ≤ 4) = F 28/28; the λ = 6 table
exact in every cell; R2 as a full matrix identity ((S^⊗λ B^⊗λ)ᵀ R_i B^⊗λ
= Gram·diag(μ), (B^⊗λ)ᵀ C_i B^⊗λ = Gram·diag(μ')) on all 84 pieces;
S^⊗λ = (−1)^λ A_λ (rows). Registration: no moved goalposts; R2 judged
airtight as implemented (every row r, every b; S^⊗λ from its
definition; B^⊗λ invertible over Q and mod odd p). Changes made:

- M1 the cost-curve sentence mixed two measures (one-direction Σ min
  against 4^λ; both-direction worst row × column rank against 12^λ) →
  split, each named.
- M2 the sign-gauge check tests entry signs, not "no cancellation" →
  one-line proof added, check described as what it is, the plan's
  N_i form noted as not implemented.
- M3 the oracle comparison had no recorded output → rerun with
  `--compare`, line quoted.
- L1 R2b's 12 enumeration counts cannot fail → said so. L2 the
  re-derivation attributed. L3 "saves at every λ" argued (λ ≤ 60
  computed, bound for λ ≥ 22); non-monotone at small λ. L4 mod-2 claims
  scoped to 2 ≤ λ ≤ 6, second regularity and reversal added. L5
  "nobody" → "not in this project". L6 the speed explanation corrected
  (pivot-column skip, structure) with the (6/16)^λ bound and the
  reviewer's measured density. L7 the plan's B⁻¹ line was rows-only;
  denominator bounds stated.
