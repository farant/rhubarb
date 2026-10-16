# Demo 128: Braids as Exact ReLU Networks; a Growth Census; an Audit of D106 — Findings

Three questions (plan: `plan.md`, approved 2026-10-15; its "Decisions
made during" sections record each task's choices):

1. **N1:** is a braid word an exact, untrained ReLU network?
2. What does exact loop growth say about every braid up to a given
   length: periodic, positive entropy, or neither?
3. Does D106's classification of 4-strand braids into
   "pseudo-Anosov" and "periodic" survive?

Everything is exact (Magnus integers, Z[zeta_8] via `cyclotomia`,
exact real roots via `radices`). The braid machinery is the house
library `plexus` (Artin action, Dynnikov loop action, reduced Burau,
the Burau certificate at roots of unity), built for this demo. The
frozen copy is `demo-snapshot.c` (one file, compiles alone; banner
gives the clang line and commit).

## Result

1. **Yes, a braid word is an exact ReLU network.** Each generator's
   Dynnikov update (Thiffeault 2010, eq. 8-15) is a straight-line
   program of affine forms and ReLUs with integer weights in
   {-1, 0, 1} and no biases. Each letter has two ReLU stages. On
   4,000 random (word, vector) pairs, n = 3..8, the program equals
   `ansa_agere` exactly, including 1,000 with coordinates near
   1000 * 2^200.
2. **Census.** 2,589 distinct braids on B_3 up to length 8 and 5,121
   on B_4 up to length 6, each classified exactly: P (periodic), E
   (positive entropy, every one CERTIFIED), Z (zero entropy, not
   periodic). The smallest rates are the known minimal dilatations,
   phi^2 = 2.61803 on B_3 and 2.29663 on B_4.
3. **D106's rule decides exactly "Burau(-1) spectral radius > 1".**
   That is a sufficient condition for positive entropy, not a
   classification. On B_4 up to length 6 it calls 1,008 of the 4,014
   positive-entropy braids "periodic"; of the 2,115 braids it calls
   periodic, 171 are (both counts include the identity, which D106
   lists apart). In D106's own first 128 catalog entries (its
   contingency table), its 88 "periodic" entries are 8 periodic, 64
   zero-entropy non-periodic and 16 of positive entropy.
4. **D106's catalog is explained exactly.** Its table is the exact
   shortest-length enumeration through depth 5. At depth 6 it has
   3,444 braids where there are 3,542: it deduplicated by its TL
   matrix alone, which merges 98 pairs of distinct braids. A replica
   of its search reproduces its whole table, depths 0-6.
5. **Part D:** iterating each braid on each loop, the active-ReLU
   pattern settles within 5 passes into a periodic sequence on every
   orbit of both censuses (observed over 60 passes). On the settled
   tail the network is ONE integer linear map per period, checked
   exactly. On B_4, period 2 appears mostly among braids that Burau
   certifies only at a cube root of unity.

## The layer programs (Part A)

sigma_2 on B_4 (registers a1 a2 b1 b2), as the demo prints it:

```text
t0  = relu(b2)              t6  = a1 - t1 - t3
t1  = relu(b1)              t7  = b2 + t2 - t4
t2  = a1 - a2 - t0 + b1 - t1
t3  = relu(t0 + t2)         t8  = a2 - b2 + t0 - b1 + t1
t4  = relu(t2)              t9  = t8 + t2 + t5
t5  = relu(b1 - t1 - t2)    t10 = b1 - t2 + t4
a1' = t6   b1' = t7   a2' = t9   b2' = t10
```

| generator | ReLU | affine | depth | ReLU depth | coordinates touched |
|---|---|---|---|---|---|
| sigma_1, sigma_{n-1} and inverses | 2 | 2 | 4 | 2 | 2 |
| middle sigma_i and inverse | 5 | 6 | 4 | 2 | 4 |

A word of length m on n strands is a network of depth 4m (2m ReLU
stages). Its state is 2n - 4 coordinates; the hidden width is at most
2n + 1 (the state plus a middle letter's 5 ReLUs). Letters on far-apart
strands commute and could share a layer; the demo does not merge them.

Observed, not asserted: (s1 s2^-1)^20 on E_1 gives (63,245,986;
331,160,282) = (F_39; 2 F_41), Fibonacci numbers, as the dilatation
phi^2 suggests.

> **Correction (2026-10-16, nervus T3).** The coordinates are
> (63,245,986; -39,088,169) = (F_39; -F_38); 331,160,282 = 2 F_41 is
> L, the loop's crossing count. The printed line was wrong, not the
> check: `main.c:654` calls `mstr()` twice in one `printf`, and `mstr`
> returns one static buffer, so the second coordinate printed as L.
> The demo's exact check (network = plexus iteration) compares the
> right values and passed. Now asserted in `probatio_plexus.c`
> (section "nervus"); the frozen demo code is unchanged.

## Growth census (Part B)

Classes: **P** periodic, exactly: beta^n or beta^(n-1) is a power of
Delta^2 (Gonzalez-Meneses and Wiest 2004), decided with
`plexus_aequales_ansis`. **E** positive entropy, certified: Burau's
spectral radius is > 1 at t = -1 or at a primitive cube root of unity
(Kolev's lower bound, decided exactly by `plexus_burau_radius_maior`).
**Z** everything else. Each class is checked against its loop growth
over 60 periods (P repeats with its period; E grows beyond 10^6 per 20
periods and not quasi-affinely; Z grows quasi-affinely), and the
per-level counts equal an independent oracle's (the T6d review).

B_3 up to length 8:

| len | braids | P | E | Z | E share |
|---|---|---|---|---|---|
| 0 | 1 | 1 | 0 | 0 | 0.0% |
| 1 | 4 | 0 | 0 | 4 | 0.0% |
| 2 | 12 | 4 | 4 | 4 | 33.3% |
| 3 | 30 | 10 | 12 | 8 | 40.0% |
| 4 | 68 | 20 | 32 | 16 | 47.1% |
| 5 | 148 | 16 | 92 | 40 | 62.2% |
| 6 | 314 | 34 | 248 | 32 | 79.0% |
| 7 | 656 | 32 | 544 | 80 | 82.9% |
| 8 | 1356 | 92 | 1176 | 88 | 86.7% |

Every E on B_3 is certified at t = -1, consistent with Burau(-1) giving
the exact dilatation on B_3 (Band and Boyland 2007).

B_4 up to length 6 (E split by certificate):

| len | braids | P | E at t = -1 | E only at a cube root | Z | E share |
|---|---|---|---|---|---|---|
| 0 | 1 | 1 | 0 | 0 | 0 | 0.0% |
| 1 | 6 | 0 | 0 | 0 | 6 | 0.0% |
| 2 | 26 | 0 | 8 | 0 | 18 | 30.8% |
| 3 | 98 | 8 | 32 | 16 | 42 | 49.0% |
| 4 | 338 | 24 | 146 | 52 | 116 | 58.6% |
| 5 | 1110 | 36 | 562 | 240 | 272 | 72.3% |
| 6 | 3542 | 102 | 2258 | 700 | 482 | 83.5% |

The E share rises with length on both groups (P3 holds). A generator
is a half twist, class Z, not periodic.

## D106's rule (Part C)

D106's rule on p(x) = det(xI - Burau(-1)): p(1) != 0 means "pA";
otherwise |1 - tr| > 2 means "pA"; else "periodic".

- **p(1) = 0 for every braid in B_4.** Unreduced Burau fixes
  (1, 1, 1, 1), which lies in the kernel of (1, t, t^2, t^3) when
  t^4 = 1 and t != 1, so the reduced matrix has eigenvalue 1. The
  rule's first branch never fires (checked on all 5,121).
- With det = 1, p = (x - 1)(x^2 - (tr - 1)x + 1). So "pA" iff
  |1 - tr| > 2 iff the other two eigenvalues are real with one of
  modulus > 1 iff Burau(-1) has spectral radius > 1 (checked: equal
  sets on all 5,121).
- D106's matrices are not the standard reduced Burau(-1): its
  generators are conjugate, by diag(1, -1, 1), to the
  inverse-transposes of the standard ones (fact-check review). Those
  have the same characteristic polynomial (det 1 and p(1) = 0 give
  c1 = tr), so all of this holds for D106's matrices on every braid,
  not only the 5,121 checked.
- Kolev's bound makes radius > 1 imply positive entropy, so the
  rule's "pA" never marks a zero-entropy braid. It does mark reducible
  braids "pA" (the depth-2 ones below), so as a pseudo-Anosov label it
  can still be wrong. Its "periodic" is wrong whenever entropy is
  positive but invisible at t = -1, and also for the 936 zero-entropy
  braids that are not periodic.

| D106 says | P | Z | E |
|---|---|---|---|
| pseudo-Anosov | 0 | 0 | 3006 |
| periodic | 171 | 936 | 1008 |

This equals the T6d review's independent table. The first
disagreement in breadth-first order (one of 16 at length 3) is
s1 s3^-1 s2, conjugate to s1 s2 s3^-1: Burau(-1) characteristic
polynomial (x - 1)^3, measured rate 2.29663. That matches the minimal
dilatation of B_4, the largest root of x^4 - 2x^3 - 2x + 1 (Song, Ko
and Los 2002, proof completed by Ham and Song, short proof by Lanneau
and Thiffeault; cited, not checked here). Burau at a cube root of
unity certifies its class but bounds the rate only by 1.9577. Every
one of the 1,008 is certified at a cube root of unity.

**P2 (registered), both directions.** Zero-entropy braids called "pA":
0. That direction could not have failed: the plan mis-registered it.
Positive-entropy braids called "periodic": 1,008.

**D106's catalog.** D106 searched B_4 breadth-first from the identity,
generators in the order s1, s1^-1, s2, s2^-1, s3, s3^-1, by right
multiplication, keeping a word iff its TL matrix (W_{4,2} over
Z[zeta_8], A = -zeta_8) was new. The demo replicates that search:

| depth | 0 | 1 | 2 | 3 | 4 | 5 | 6 |
|---|---|---|---|---|---|---|---|
| D106 braids | 1 | 6 | 26 | 98 | 338 | 1110 | 3444 |
| replica | 1 | 6 | 26 | 98 | 338 | 1110 | 3444 |
| exact classes | 1 | 6 | 26 | 98 | 338 | 1110 | 3542 |
| D106 "pA" = replica | 0 | 0 | 8 | 32 | 146 | 562 | 2234 |
| rule on exact classes | 0 | 0 | 8 | 32 | 146 | 562 | 2258 |

Exactly 98 pairs of distinct braids share a TL matrix (no larger
groups), all at length 6, each pair with equal Burau(-1)
characteristic polynomials (the fact-check review found they share
D106's reduced Burau(-1) matrices too) (first: -3 -3 1 1 2 -1 and
-2 1 -3 2 2 -3). Hence 3,542 - 98 = 3,444, and the rule's count drops
by 24. D106's depths 7 and 8 (10,390 and 17,355, capped at 32,768)
are not checked here.

**D106's contingency table** used its first 128 catalog entries
(depths 0-2 and all but 3 of depth 3). They split as D106 printed:
identity, 88 "periodic", 39 "pA". Exactly: the 39 "pA" are E; the 88
"periodic" are 8 P, 64 Z and 16 E. So D106's "periodic vs
pseudo-Anosov" comparison was, in exact classes, mostly
"zero-entropy-not-periodic plus some E" against E.

**D106's other claims.**
- Depth 1 is all "periodic": the six generators are half twists,
  class Z (zero entropy, not periodic).
- The 8 depth-2 "pA" braids have characteristic polynomial
  x^3 - 4x^2 + 4x - 1 and Burau radius (3 + sqrt 5)/2 = phi^2 exactly
  (P4 holds). But each is a word in two adjacent generators: a 3-strand
  braid inside B_4, so reducible, not pseudo-Anosov (the round curve
  around its three punctures is invariant and essential). phi^2 is
  exact on the 3-strand piece.
- The "pA" share rises with depth (P3's direction holds) but
  understates positive entropy by up to 22 points (21.7 at length 5): D106's rule on the
  exact classes gives 30.8 / 32.7 / 43.2 / 50.6 / 63.7% at lengths 2-6
  against E 30.8 / 49.0 / 58.6 / 72.3 / 83.5%.
- D106 says Burau is faithful on B_4 (citing Bigelow 1999 and Krammer
  2000). That is open: Bigelow 1999 shows Burau unfaithful for n >= 5,
  and Bigelow and Krammer proved the Lawrence-Krammer representation
  faithful. Reduced Burau at t = -1 is not faithful on B_4: Delta^2
  maps to t^4 I = I.

**The TL key at delta = 0.** At A = -zeta_8 the loop value is
delta = -A^2 - A^-2 = 0. The TL form G = [[0,1,0],[1,0,1],[0,1,0]] is
singular; its radical is spanned by v = (1, 0, -1), which every e_i
kills, so rho(w) v = A^{e(w)} v. Two mutants of the replica stay green
for this reason or near it: keying on two columns of the matrix (either
outer column is the other plus a multiple of v), and transposing the
product (G intertwines the two keys; since G is singular this is
observed, not proved). Details in `plan.md`, T6d.

## The active-ReLU pattern (Part D)

Each braid is iterated 60 times on every loop E_k of D_n through the
layer programs. Each pass records which ReLUs fire (argument > 0). A
pattern is "settled" if its last >= 20 passes repeat with some period
q <= 12. Per braid: start is the largest over its loops, q their lcm.

| | class | braids | settled | start (median / max) | q = 1 | q = 2 | q > 2 | regions per loop (mean) |
|---|---|---|---|---|---|---|---|---|
| B_3 | P | 209 | 209 | 0 / 0 | 3 | 58 | 148 | 2.7 |
| | E | 2108 | 2108 | 1 / 2 | 2108 | 0 | 0 | 1.8 |
| | Z | 272 | 272 | 1 / 4 | 272 | 0 | 0 | 1.9 |
| B_4 | P | 171 | 171 | 0 / 0 | 1 | 14 | 156 | 3.0 |
| | E | 4014 | 4014 | 2 / 5 | 3630 | 384 | 0 | 2.7 |
| | Z | 936 | 936 | 2 / 5 | 488 | 200 | 248 | 2.9 |

All 5,178 (B_3) and 15,363 (B_4) orbits settled within the run.

Two exact checks:
- Periodic braids repeat their pattern from pass 0, with a period
  dividing the braid's (their loop coordinates repeat, since Delta^2
  fixes every loop of D_n).
- On every settled tail, with the pattern fixed, the network is linear
  (no biases). The demo builds M, the integer matrix of the forced
  q-pass map, and checks v_{s+jq} = M^j v_s exactly on the whole tail.

E braids on B_4 by certificate and period:

| | q = 1 | q = 2 |
|---|---|---|
| Burau(-1) radius > 1 (3,006) | 2,950 | 56 |
| only at a cube root of unity (1,008) | 680 | 328 |

Before the run the plan expected E to settle mostly with q = 1. It
does on B_3 (all) and mostly on B_4; period 2 is concentrated among
the braids that t = -1 cannot see. In an ad hoc debug run (not printed
by the demo), every q = 2 E braid up to length 4 had rate phi^2. The
cause is not established.

## Interpretation [interpretation]

- A braid word is a deep, narrow, sparse ReLU network with weights in
  {-1, 0, 1} and no biases, and it is exact by construction: no
  training, no rounding. Iterating it is a dynamical system on
  piecewise-linear cones. The orbits here leave their first linear
  regions within a few passes and then stay in a fixed cycle of
  regions, where the network IS a single integer matrix M. From then
  on the orbit grows like M^j applied to one vector. Relating that to
  the dilatation exactly (on the right invariant cone) is Hall and
  Yurttas's route and the next plexus piece (ledger …VFF141H), not
  done here.
- D106's entropy null (KC 31.2: pseudo-Anosov braids do not compute
  better) compared its rule's classes. In exact classes its
  "periodic" side was mostly zero-entropy non-periodic braids with 16
  of positive entropy, and only 8 periodic ones. Whether the null
  holds over exact classes is open; re-running D106's Boolean
  comparison is outside this demo.
- D106's TL dedup merged braids because W_{4,2} at delta = 0 does not
  separate them (the key is Burau(-1)-like: KC 31.4). The radical
  vector v that every e_i kills is the structure KC 42.1 describes.

## Checks

Full mode 30 (quick 24), all exact:
- Part A: program shape (n = 3..8); sigma sigma^-1 = identity through
  the networks; the anchor against `ansa_agere` (4,000 pairs);
  (s1 s2^-1)^20 through the network.
- Part B, per group: class counts = the T5 review's Artin counts and
  the T6d review's per-level P/E/Z counts; class consistency with
  growth; every E certified; identity periodic; 300 random pairs of
  distinct classes unequal under Artin; on B_4 delta, epsilon periodic
  and the minimal-dilatation braid at 2.2966.
- Part C: the p(1) = 0 lemma; the cross table; rule = Burau(-1)
  radius > 1; D106's table by its replica; length-6 counts 3,542 /
  2,258; the 98 pairs; the first-128 split and its exact classes; the
  depth-2 braids (polynomial, phi^2, reducible).
- Part D, per group: periodic patterns from pass 0; the linear tail.

## Plants

All through `silva.planta`, all red unless listed:
- T6a (8): signs in c and w, the inverse's b_p, sigma_1's a_1,
  sigma_{n-1}'s b, an interpreter ignoring the ReLU, swapped outputs,
  a miscounted ReLU depth.
- T6b (9): periodicity tested with k = n only, the growth threshold,
  an ignored certificate, D106's |1 - tr| boundary, level dedup off,
  the class key on D_{n+1} without the translation, the class key on
  D_n, an off-by-one periodicity check, the quasi-affine window cut to
  p = 1.
- T6d (4): the replica keeping duplicates at depth 6, the p(1) lemma
  inverted, reducibility on non-adjacent generators, the second
  certificate moved to t = -1.
- T6c (5): the forced map leaking the sign, M transposed, the tail one
  pass late, the P period test off by one, the catalog head shifted by
  one.
- Green, kept as findings: transposed TL product; TL key on two
  columns (see "The TL key at delta = 0"); three other generator
  orders for D106's search (the first-128 split does not depend on
  them).

## Review (T6d, 2026-10-15)

An independent reviewer rebuilt the census with its own Python oracle
(free-group growth, periodicity for every k <= 12, the Kolev bound)
and replicated D106's search. H 0, M 4, L 6; every number the demo
printed was right. It drove: the exact TL-only explanation (M1), the
pinned class counts and stricter class checks (M2), certifying the
1,008 (M3, via `plexus_burau_radius_maior`), the depth-2
reducibility (M4), the p(1) = 0 lemma (L1), P2(a) as impossible (L2),
the width wording (L3), stronger plants (L4), D106's faithfulness
claim (L5) and the citations (L6).

## Fact-check review (2026-10-16)

A second reviewer checked the claims.md nodes, this file and the
correction notes against the snapshot, D106's sources and the
literature: H 1, M 3, L 10, N 7, every count right. Fixed: "pA" was
called "never wrong" though it labels reducible braids (H1); "up to 20
points" is 21.7 (M1); the B_4 minimum is a measured rate plus a
literature value, not exact (M2); 55.1 is proved by construction
(M3); the det = 1 step and D106's conjugacy to the inverse-transpose
(L1); reduced Burau for Delta^2 (L2); the identity inside the 2,115
(L3); the notes' wording (L4); the Song-Ko-Los citation (L5); line and
plant counts (L6, L7); the Part D legend (L8, also in main.c, snapshot
refrozen); the revisions order (L9); 54.7.1's argument (L10).

## Not covered

- Exact dilatations from the settled linear maps (plexus, …VFF141H).
- Telling pseudo-Anosov from reducible beyond the depth-2 case.
- D106's depths 7-8 and its Boolean-function comparison (KC 31.2).
- Computing with the braid network (N2, ledger …4291V2).

## Run

```text
./bin/aedilis knotapel/demo_128_braid_relu/main.c &&
  bash build/aedilis/main/struere.sh && ./build/aedilis/main/main
```

Full mode about 4 s (30 checks); `D128_CELER=1` (B_3 up to 5, B_4 up
to 4) about 0.4 s (24 checks). The snapshot gives byte-identical
output (164 lines).
