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
drops. λ = 6 is the size nobody had computed in any arithmetic before
this run (the design reviewer had confirmed λ ≤ 5 by proxy, reporting
match/mismatch only); all 70 of its ranks agree with the independent
numpy oracle.

So the cost curve of §11 item 6 stands on a checked formula: the pieces,
each in its cheaper direction, cost 2^17.0 against the crossing's 2^20 at
λ = 10 (8×), 2^52.3 against 2^60 at λ = 30 (200×), and the worst piece
approaches 12^λ from below (3.03 bits per bit at λ = 4, 3.43 at 30).
**At the level of ranks, ANT's one-bit decomposition saves at every λ;
whether real tables can realize those ranks is the next experiment.**

## Result

1. **The closed form is right** [checked exactly for λ ≤ 6; derived
   for all λ]. R2 + R2b at λ ≤ 6 are a machine check of the derivation's
   key step (diagonal in the rotated basis, eigenvalues as stated) for
   every piece; the general-λ statement rests on the hand derivation
   (brainstorm §11 items 1-2), which the review re-derived independently.
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
   there]. Ranks drop below Q at every 0 < i < λ for λ ≥ 2. One regularity
   holds at every λ ≤ 6: rank of A_1 (rows) = 2λ + 2 (Q: 3λ + 1). Not
   interpreted; the plan registered nothing here.
5. **For the next experiment** [descriptive]. The factorization is
   explicit and lives in Z[½]: per bit the rows basis has Gram matrix
   diag(2, 2, 1, 1), the columns basis diag(2, 1, 1, 2), so B⁻¹ = Gram⁻¹·Bᵀ.
   Every term of one entry of A_i carries the same sign, (−1)^i·
   (−1)^popcount(x ⊕ u) (checked on every nonzero entry, λ ≤ 6): no
   cancellation happens inside an entry, so a table-level experiment can
   carry unsigned counts plus a sign gauge.

## Registered predictions

| | Prediction | Verdict |
| --- | --- | --- |
| R1 | Σ_i A_i = [x = v][y = u], whole words, λ ≤ 6 | HOLDS (6 of 6 λ; oracle agrees) |
| R2 | R_i·b = μ_i(b)·S^⊗λ·b, C_i·b = μ'_i(b)·b, all b, λ ≤ 6 | HOLDS (12 of 12) |
| R2b | per-bit determinants ±2 (rows −2, columns +2); all 4^λ tuples | HOLDS (14 of 14) |
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
demo's 270 plain-elimination ranks and the 124 `matrix_gradus` ranks it
shares; its own R1 holds at every λ.

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
s CPU (λ = 5 includes `matrix_gradus` at 2^31 − 1; λ = 6 plain
elimination only, fast because the pieces are sparse - the elimination
skips zero rows). Peak memory 225 MB (custos, compressed included; cap
2 GB). Oracle `python3 -I oracle_d135.py 6`: peak 1,030 MB (cap 4 GB).
Frozen copy: `demo-snapshot.c` (`knotapel/archive.sh`).
