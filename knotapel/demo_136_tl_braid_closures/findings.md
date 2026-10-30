# Demo 136: Brackets of Large Braid Closures through Temperley–Lieb Modules — Findings

Plan: `plan.md` (v1 approved by Fran 2026-10-28; v2 after a DESIGN
REVIEW before any data, consiliarius-d136; T0 and T1 amendments).
Context: project-specs/crossings-contraction-counting-brainstorm.md §12.3
(invariants of large knots), §11 lever 2 (fewer primes or points);
tools/uncinus_mensura.c section V (the vertex model's projections:
12 × 801 ≈ 32 days).

The Kauffman bracket of a braid closure is a weighted sum of the braid's
traces in the Temperley–Lieb standard modules W_{n,j}, whose dimensions
square-sum to Catalan(n) (208,012 at 12 strands) instead of the vertex
model's ~2^(2n) states. The demo evaluates that sum at points mod primes
near 2^32 and rebuilds the exact bracket with the same interpolation the
vertex model uses.

**Every registered prediction holds: 692 checks, 0 failures.** The TL
bracket equals the state sum (c ≤ 24) and the vertex model in full
wherever the vertex model can run (every braid up to 12 × 201, and
8 × 401, 8 × 801), and passes the off-interpolation, Jones-identity and
Markov-move checks at the large sizes where it runs alone.

**The headline is 12 × 801: the exact Jones polynomial in 78 minutes**
(4,700 s, 12,949 evaluations, 0.36 s per point), where the vertex model
was projected at about a month. Per point the TL route is 634× faster
than the vertex model there (registered: ≥ 100×), and 86× at 12 × 401
(registered: ≥ 25×).

## Result

1. **The TL closure formula is right in this code** [checked]. R1 (60
   comparisons with the state sum and the vertex model, every one
   exact), R1p (TL value = vertex value = the reduced exact polynomial
   at points of p* that the interpolation never sees), R4 (Jones
   identities including |V(−1)| = det by two independent determinant
   routes), R5 (stabilization front and back, index flip, conjugation),
   R2 and R3 (link-state counts and the TL and braid relations on every
   basis state, n ≤ 16), conv0 (σ ↦ A·1 + A⁻¹·e, the standard bracket).
2. **Cost per point is 1.4–1.6 ns × c × Catalan(n)** [measured, n = 8 …
   16 at c = 801; descriptive]. 14 strands: 3.3 s per point (T0
   projected 4.6 s); 16 strands: 43 s per point (T0 projected 63 s).
   At ~13,000 points a full 14 × 801 bracket is ~12 h and 16 × 801 ~6.5
   days; that is T4 territory, and lever 2 (fewer points or primes) is
   what would bring 16 strands in.
3. **The vertex model is faster on short braids** [observed]. At
   12 × 201 the vertex model took 5.65 s and the TL bracket 51.7 s: the
   vertex model's cost follows the width of its contraction order (14 at
   12 × 201, 26 at 12 × 801), the TL route's follows Catalan(n) whatever
   c is. The routes cross between c = 201 and c = 401 at 12 strands.
4. **The coefficient bound is ~3.7× loose** [measured]. Largest
   coefficient against the Tait-type bound the interpolation uses:
   175 / 654 bits at 12 × 801, 193 / 642 at 8 × 801, 91 / 333 at
   12 × 401, 42 / 165 at 12 × 201. Every prime past the true size is
   paid for; a tighter a-priori bound would cut the primes (not the
   points) by about that factor.
5. **Exact arithmetic without primes stays small** [measured;
   descriptive]. Carrying the traces over Z[A²] (each letter scaled by
   A³ so entries stay integer polynomials), the largest intermediate
   coefficient is within a few bits of the final bracket's: 39 vs 39
   bits at 8 × 201, 27 vs 19 at 12 × 101, 44 vs 42 at 12 × 201, 192 vs
   193 at 8 × 801 (the largest gap, 8 bits, at 12 × 101). Through
   12 × 201 every coefficient fit in 64 bits (no big-number allocation
   at all); 8 × 401 needed 68 bits. At the single integer point A = 2 the entries grow ~3.7
   bits per letter (up to 3 of them the 8^c scaling): 2,933 bits at
   12 × 801, 105 s. So a prime-free exact route is plausible (lever 2):
   word-sized polynomial arithmetic, no CRT, no bound. As written here
   (one `magnus` per coefficient, dense) it is 2–6× slower than the
   modular route at 12 strands (12 × 201: 194.5 s against 51.7 s), even
   at 8 (8 × 801: 25.2 s against 23.4 s); dense buffers cost memory
   (12 × 201 peaked at 4,966 MB).
6. **No random closure is reduced alternating** [descriptive, as the
   plan expected]. span V / c lies between 0.14 and 0.46 over the 45
   test braids (0.39 at 8 × 801, 0.32 at 12 × 801), around a third at
   the long lengths; c − span is never 0. One closure,
   11 × 20, has V = 1 (likely an unknot; V = 1 does not prove it).

## Registered predictions

| | Prediction | Verdict |
| --- | --- | --- |
| R0 | test braids = the projection run's (bound, width; 4 fingerprints) | HOLDS (4 of 4) |
| R1 | TL bracket = state sum (c ≤ 24) and = vertex model (full) | HOLDS (60 of 60) |
| R1p | TL = vertex = reduced exact TL polynomial at points of p* | HOLDS (6 of 6) |
| R2 | link-state counts, validity, Σ d² = Catalan(n), n ≤ 16 | HOLDS (16 of 16) |
| R3 | TL and braid relations on every basis state, n ≤ 16 | HOLDS (15 of 15) |
| R4 | V(1) = 1, V(e^{2πi/3}) = 1, V(i) = ±1, \|V(−1)\| = det | HOLDS (292 of 292) |
| R5 | stabilization (back, front), index flip, conjugation, at points | HOLDS (294 of 294) |
| R6 | TL per point ≥ 25× (12 × 401), ≥ 100× (12 × 801) faster than vertex | HOLDS (86.4×, 634.5×) |
| conv0 | trefoil against the state sum at points; one-strand trivial braid = 1 | HOLDS (3 of 3) |

R4 is blind to mirror images (V(t) and V(1/t) pass alike); R1 and R5's
stabilization factor are not. At c = 801 the determinant comes only from
the colouring-matrix route (67 primes at 12 × 801, 171 bits), which
shares nothing with the TL code or the vertex model; for c ≤ 401 it
agrees with `laqueus_alexander_ex_pd`.

## What was seen before the run

- T0 (no bracket): link-state enumeration, kernel speed (~1 ns per
  multiply-add), determinant timings.
- T1 (`D136_MODUS=t1` and `p8`): the small set (n ≤ 6 at lengths 20
  and 50, the named knots, R1p smoke at 8 × 101) and 10 × 401, whose
  bracket took 27 s with largest coefficient 82 bits. So 10 × 401's
  bracket was seen; its R1p comparison and every 12-strand result were
  not.
- Nothing at 12 × 401 or 12 × 801, in any route, before T2.

## Checks (692, all pass)

R0 4, R1 60, R1p 6, R2 16, R3 15, R4 292, R5 294, R6 2, conv0 3. R5 at
each braid uses (summus − imus)/4 + 3 points of p* (563 at 12 × 801).
Each run asserts the lowest interpolation prime it used is above p*.

## Plants (silva.planta, T1, all eight as the plan stated)

1 case (c) → 0: R1, R3, R4, R5 red; 2 Δ recurrence with +: R1, R4, R5,
conv0; 3 σ⁻¹ given σ⁺'s image: R1, R4, R5 (4_1 red); 3b global swap: R1,
R4, R5, conv0 (4_1 green, as registered); 4 /δ dropped: R1, conv0;
5 enclosed defects admitted: R2; 6 one coefficient + 1: R4; 7
conjugating pair not inverted: R5; 8 value negated when n ≥ 10 and
c > 300 (p8 mode): R1p, R4 red, R1 green. Extra reds beyond the
registered minimum are listed in plan.md's T1 amendment.

## Descriptive (no prediction; `D136_MODUS=desc`, run after T2)

Added after the registered run in a separate mode, so none of it is in
the 692. Times printed only in this mode or with `D136_TEMPORA=1`.

**Per-point time at c = 801** (3 points of p*, alone on the machine):

| n | Catalan | largest module | s per point | ns per c × Catalan |
| --- | ---: | ---: | ---: | ---: |
| 8 | 1,430 | 28 | 0.002 | 1.53 |
| 10 | 16,796 | 90 | 0.019 | 1.41 |
| 12 | 208,012 | 297 | 0.227 | 1.36 |
| 13 | 742,900 | 572 | 0.980 | 1.64 |
| 14 | 2,674,440 | 1,001 | 3.340 | 1.56 |
| 15 | 9,694,845 | 2,002 | 11.891 | 1.53 |
| 16 | 35,357,670 | 3,640 | 43.341 | 1.53 |

(n = 4 … 7 are below the clock's resolution to be meaningful.)

**Exact traces without primes** (run concurrently with the span run):

| braid | Z[A²]: max coefficient bits, intermediate / bracket | Z at A = 2: max entry bits (scaling ≤ 3c) | time Z[A²] / Z / TL bracket |
| --- | --- | --- | --- |
| 8 × 201 | 39 / 39 | 744 (603) | 0.9 s / 0.0 s / 0.41 s |
| 10 × 101 | 26 / 24 | – | 3.0 s / – / 0.67 s |
| 12 × 101 | 27 / 19 | – | 48.7 s / – / 8.3 s |
| 12 × 201 | 44 / 42 | 736 (603) | 194.5 s / 7.2 s / 51.7 s |
| 8 × 401 | 68 / 67 | 1,429 (1,203) | 4.6 s / 0.2 s / 2.7 s |
| 8 × 801 | 192 / 193 | 3,083 (2,403) | 25.2 s / 0.7 s / 23.4 s |
| 12 × 401 | – | 1,483 (1,203) | – / 26.3 s / 578 s |
| 12 × 801 | – | 2,933 (2,403) | – / 105 s / 4,701 s |

Every exact trace, both routes, reproduces the modular TL value at A = 2
mod p*. The polynomial route was planned for c ≤ 201; 8 × 401 and
8 × 801 go past that because they turned out cheap at 8 strands. The A =
2 column is one point: a full bracket that way would need ~(summus −
imus)/4 such points, or the polynomial route.

**Span of V against c** (span of V in t; c − span = 0 iff reduced
alternating; requested lengths, the generator bumps parity):

| n | c ≈ 20 | c ≈ 50 | c ≈ 101 | c ≈ 201 | longer |
| --- | ---: | ---: | ---: | ---: | --- |
| 3 | 6 / 20 | 14 / 50 | 24 / 102 | 50 / 202 | |
| 4 | 7 / 21 | 12 / 51 | 28 / 101 | 62 / 201 | |
| 5 | 5 / 20 | 13 / 50 | 27 / 102 | 66 / 202 | |
| 6 | 3 / 21 | 21 / 51 | 40 / 101 | 81 / 201 | |
| 7 | 6 / 20 | 12 / 50 | 39 / 102 | 63 / 202 | |
| 8 | 3 / 21 | 13 / 51 | 46 / 101 | 67 / 201 | 116 / 401, 315 / 801 |
| 9 | 5 / 20 | 15 / 50 | 31 / 102 | 75 / 202 | |
| 10 | 8 / 21 | 21 / 51 | 37 / 101 | 66 / 201 | 128 / 401 |
| 11 | 0 / 20 (V = 1) | 10 / 50 | 34 / 102 | 72 / 202 | |
| 12 | 7 / 21 | 9 / 51 | 39 / 101 | 67 / 201 | 137 / 401, 259 / 801 |

The largest coefficients repeat the T2 run's exactly (same braids, same
brackets).

## Not covered

- 13 strands and up: points only (descriptive timings), no full
  bracket. T4.
- Mirror images: R4 cannot see them; R1 and R5 can, but R1 runs in full
  only where the vertex model does.
- The exact routes are measured, not optimized: no word-sized
  polynomial route was written.
- Only random braids from one generator (seed 2028 per (n, L)); no
  family where the answer is known in closed form beyond the named
  knots (3_1, 4_1, T(3,4), T(4,5)).

## Run

T2: `D136_MODUS=plene tools/custos.sh build/d136_t2/main` (binary built
from commit 44ac93b6), detached; 692 passed, 0 failed; peak 544 MB
(custos, compressed included; cap 4 GB). Largest single items: 12 × 801
TL bracket 4,700.8 s; 8 × 801 vertex model in full 3,247 s; 12 × 401 TL
bracket 578 s.

Descriptive: `D136_MODUS=desc D136_PARS=times|spans|integers|polynomials
tools/custos.sh build/d136_desc/main` (main.c as committed with this
file); times alone first (peak 246 MB), then the other three in
parallel (integers peak 286 MB; polynomials 4,966 MB, cap 8 GB, the
dense 12 × 201 buffers; spans 1 h 52 min, the 12 × 801 bracket again).

Frozen copy: `demo-snapshot.c` (`knotapel/archive.sh`, `D136_MODUS=t1`).

## Review (T3)

Not yet run.
