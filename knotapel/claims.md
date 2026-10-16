# Knotapel claims

The live status of knotapel's claims: what holds, what fell, what was
never checked. It replaces the archived atlas
(`archived-analysis/atlas/`) as the place to look first. Started
2026-10-13 from the exact audits D114–D125.

## How to read this file

The body is in the sententiae format. Each line is one claim at a
stable decimal address, and a child qualifies or drills into its
parent. Questions are their own nodes, answered at `.1`. Contested
claims carry objections (`N.obj.1`) with replies. Cite a claim as
`KC 21.4`.

**Addresses never change.** A new claim is appended after the last
sibling, even where logical order suffers (a new top-level question
sits in its part with a "logically follows" note). A claim that turns
out wrong is never deleted: change its grade and add a child that says
why. Every future audit updates this file in the same commit as its
correction notes.

**Negative results are claims too.** A "No," answer carries a grade
like any other. `[proved]` or `[exact]` means the thing does not
exist. `[exact-search]` means the search found none, and the bounds are
stated.

A node that only summarizes other nodes carries no grade of its own:
its strength is that of the nodes it cites.

### Grades

| grade | meaning |
|---|---|
| `[definition]` | a definition |
| `[proved]` | a written proof, checked |
| `[exact]` | exact, complete computation in house arithmetic, re-derived by an independent oracle |
| `[exact-search]` | exact cells, but a bounded or greedy search: holds as far as the search reaches |
| `[intervention]` | the demo's own code re-run with one fault removed (not exact arithmetic) |
| `[checked]` | checked by hand algebra or a scratch script in the 2026-10-03 reading (KR), not a committed demo |
| `[falls]` | refuted; a child gives the corrected statement |
| `[not-reproduced]` | the demo's own code does not print the recorded result |
| `[unaudited]` | recorded from float- or `long`-era code, never re-run exactly |
| `[interpretation]` | a reading of results, not a result |
| `[open]` | an open question |

### Sigla

| siglum | source |
|---|---|
| KC | this file, `knotapel/claims.md` |
| Dn | demo n: `demo_n_*/findings.md` (audits) or `main.c` (as built) |
| KR n | `docs/knotapel.html`, "Notes from this reading" (2026-10-03), note n of its numbered list |
| AA | the archived atlas, `archived-analysis/atlas/` |
| XL | `explorers-log.md` (the synthesis of 2026-10-13 is at its end) |

### Proposed audits

Each is a desideratum in the tabularium, region "knotapel research",
ranked by how much it bears on KC 47.

| | ledger | settles | question |
|---|---|---|---|
| P1 | …E72XG3 | 47, 48, 19.7 | group catalogs against knot-free point sets under one exact activation (done: D127, see 47.1.5) |
| P2 | …5BB3SQ | 18.2, 47.obj.1, 49 | is D118's Q8-coset characterization geometric or group-built? |
| P3 | …S0E97V | 47.1.1, 53.4 | are XOR verdicts constant on knot classes? is D66's representation the TL one? |
| P4 | …JYSRSC | 9 | exact D29 forward DKC against determinant-star and random-phase nulls |
| P5 | …DHY1TG | 11, 14 | exact D50/D63 sector search with nulls and both boundary conventions |
| P6 | …9ZQ5E1 | 22.6 | all 1,307,504 15-subsets of 2O, exactly |
| P7 | …6DD3NK | 40, 50 | exact Temperley–Lieb radical audit, n ≤ 10 |
| P8 | …NPCP6E | 51, 26.3.1 | beyond-greedy XOR14 search at ζ₁₂ (low priority) |
| P9 | …6AED71 | 33 | D89–D93 |
| P10 | …G9DPA5 | 41 | gauge-fixed b for D85, n = 4, 6, 8 |
| P11 | …N4FWHD | 19.1 | independent oracle runs for D119 at N = 5–6 |

### Revisions

- **Review I (2026-10-13, before merge into main).** An independent
  reviewer checked every node against its source: 8 high, 16 medium
  and 15 low findings, all verified before acting. The main
  correction: catalog entries are not knot invariants, and the ζ₁₂
  catalogs are not braid images at all (KC 53). Nodes were corrected
  in place and new nodes appended; no address was removed or
  renumbered.
- **D127 (2026-10-15), audit P1.** Appended: 19.1.2, 19.7.1, 19.9.1,
  19.10, 47.1.4.1, 47.1.5 to 47.1.5.3, 48.1.1.1, 48.1.2, 48.2.1 and
  48.4.6.1. D127 was reviewed independently before these nodes were
  written (its findings.md, "Review").
- **D128 (2026-10-15).** Appended: 31.2.1, 31.4.1, 42.1.1 and Part X
  (54, 55): D106's braid classification audited, and the braid word
  as an exact ReLU network. D128 was reviewed independently before
  these nodes were written (its findings.md, "Review"). Revised the
  same day after a fact-check review (1 high, 3 medium, 10 low):
  54.2 no longer says "never wrong"; 54.1, 54.7.1 and 55.1 graded
  [proved]; 54.3.1 separates the measured rate from the exact class;
  54.1.3 appended.
- **D129 (2026-10-16).** Appended: 31.2.2, 55.2.3.1.1 to
  55.2.3.1.4, and 56 (linear regions of braid networks). Its library
  pieces (nervus_regiones_plani, the plexus census helpers) were
  reviewed independently before the demo; the demo's own independent
  review (task T5d) follows these nodes.

## Part I. Definitions

```text
1. What is a catalog?
1.1. The ordered list of distinct values produced by a braid
     enumeration or by closing generators under products: bracket
     values in Z[zeta_n], or unit quaternions taken mod sign.
     [definition]
1.2. Its order matters to any search that keeps only its first
     winners (see 27.2, 45.1).

2. What does it mean for weights to compute a Boolean function?
2.1. Each input bit selects or signs a weight, the neuron combines
     them (see 2.4), an activation maps the result to a cell, and the
     cells' labels give the output (see 2.5). [definition]
2.2. A cell is a sector of the sum's angle (k sectors), crossed in
     most quaternion demos with the nearest axis of an axis table.
     [definition]
2.3. Paired +-q encoding (D66, D79-D87, D92, D118, D124, D125): each
     weight contributes +q, -q or 0 for its two bits, so n weights
     read 2n bits, and XOR2n is the parity of all 2n. [definition]
2.4. Four encodings occur. [definition]
2.4.1. Inclusion, z = sum of x_i w_i (D29, D48, D50, D57, D63-D65).
2.4.2. Multiplicative, z = product of w_i^(x_i) (D55, D56, D61,
       D62).
2.4.3. Signed, one weight per bit, S = sum of +-q_i (D94-D97, D106,
       D119-D123); there N counts both bits and weights.
2.4.4. Paired +-q (see 2.3).
2.5. Two labeling regimes. Up to D64 the labels are fixed (sector
     index mod 2, or a fixed readout such as the split sigmoid). From
     D65 on they are free: the weights compute f if no cell receives
     both values of f, so the activation is a lookup table. [definition]
     (KR 35)
2.6. D66 and D118 use Voronoi cells: the nearest catalog element (the
     24-cell Voronoi), plus a cell for the zero sum. [definition]

3. Which cell does a sum on a boundary land in?
3.1. Exact rule: the demo's own formula evaluated exactly (sector by
     floor, axis = first maximal in table order). [definition]
3.2. Robust: the set separates under every resolution of every tie.
     [definition]
3.3. Possible: the set separates under some resolution, chosen per
     exact point. [definition]
3.3.1. Possible is near-vacuous where every sum is a two-way tie (all
       half-angles 90 degrees): each sum can then take its own
       parity's side. (see 21.5, 22.2.1)
3.4. Float: the demo's double-precision code. At an exact tie its
     answer is a rounding accident. (see 43.1)

4. What is capacity?
4.1. The largest XOR level reached, and the counts of winning sets,
     for a given catalog, activation, tie rule and search. [definition]
4.1.1. So capacity is never a property of the catalog alone.
4.2. A search is exhaustive if it tests every weight set of a size,
     and greedy if it extends a bounded list of winners: D81, D82 and
     D87 take triples from the first 100 entries, keep 32 winners per
     level and stop at 6 weights; D79 keeps 1,024 winners and its
     XOR12 search stops at 50; D84 keeps its first 32. [definition]
```

## Part II. The bracket and its catalogs

```text
5. Are the early demos' bracket values correct?
5.1. Yes, where checked. All 87,890 braids of D29's enumeration agree
     with a second exact route (symbolic delta in polynomium,
     evaluated in cyclotomia). [exact] (D117 Part A)
5.2. D29's multiplication table agrees with Z[zeta_8] multiplication
     on 10,000 random pairs. [exact] (D117 Part A)
5.2.1. The table is bilinear, so checking its 16 basis products would
       make this complete.
5.3. The other demos' hand-written rings (Cyc16, Cyc5, Cyc24, 3x3
     matrices, quaternion tables) were not compared with the house
     library. [unaudited] (D117)
5.3.1. UBSan was silent on the paths run, which rules out signed
       overflow there, not a wrong multiplication table.

6. Does D29's catalog hold 64 distinct bracket values?
6.1. No. 64 is the first 8,192 of 69,960 nonzero brackets
     (MAX_CATALOG). All 69,960 take 100 distinct values. [falls]
     (D117 Part A)
6.2. Those 100 values lie over 30 determinants (1-45): each is a
     determinant times a phase, an eight-pointed star of integers,
     not a lattice. [exact] (D113)

7. Are the early demos' named knots knots?
7.1. Not all. sigma1 sigma2 sigma1, labelled the trefoil in D2, D3,
     D6 and D10, closes to a Hopf link with one stabilizing crossing.
     [checked] (KR 1)
7.2. D10's six-braid table is mostly links: only the figure-eight is
     a knot. [checked] (KR 3)
7.3. The three-strand "T(2,n)" entries of D11-D17 close to
     two-component links. [checked] (KR 4)
7.3.1. Results that compare entries with each other stand; results
       stated about torus knots are about these links.
7.4. D1's printed f-polynomial is mis-normalized: the writhe sign does
     not match the bracket's chirality. [checked] (KR 2)
7.5. The bracket is multiplicative under connected sum; D14's and
     D16's contrary conclusions come from the Hopf-link label.
     [checked] (KR 5, 8)
7.6. Not every zero of the bracket is a zero of delta, and the
     delta-exponent is not the Arf invariant. [checked] (KR 6, 9)

8. Is the bracket at delta = 0 always axial (n times a power of A)?
8.1. Recorded as verified on about 131,000 braids, with a three-step
     proof sketch (D35, XL). [unaudited; superseded by 8.2] (AA
     theorems.md)
8.2. Yes. Every bracket of 151,559 braids (2 strands to length 12, 3
     to length 8, 4 to length 6) is a monomial n zeta_8^k with |n| =
     det(L), the determinant computed independently by reduced Burau.
     [exact] (D113)
8.2.1. This is classical: <L> = (-A^3)^w V_L(t) with t = A^-4 = -1,
       and |V_L(-1)| = det(L).
8.3. The bracket is 0 exactly when det(L) = 0; "delta = 0 detects
     split links" is "split links have determinant 0". [exact] (D113;
     KR 24)

52. What do the bracket catalogs at the other roots of unity carry?
    (logically follows 8)
52.1. Classical invariants times a phase: at delta = 0 the
      determinant (see 8.2), at delta = 1 (-1)^crossings (V = 1), at
      delta = sqrt2 the Arf invariant and the component count, at
      delta = sqrt3 |V| = sqrt3^d with d the mod-3 rank. [checked]
      (KR 23, 24, 28)
52.2. delta = phi (D58) is the first hard point: exact evaluation is
      #P-hard there (Jaeger, Vertigan and Welsh). [checked] (KR 28)
52.3. So until D58 every bracket catalog in the project was a
      classical invariant times a phase. [checked] (KR 28)

53. Which catalogs are images of the braid group?
    (logically follows 52)
53.1. At zeta_8, D66's generators (1+i)/sqrt2 and (1-k)/sqrt2 satisfy
      aba = bab and generate 2O (see 17.1). Whether this hand-chosen
      representation is projectively the Temperley-Lieb one at
      A = e^(i5pi/4) is not established. [checked] (KR 34)
53.2. The zeta_12 catalogs of D79-D87 and D125 are not braid images.
      Their generators are rotations by one angle about perpendicular
      axes, which satisfy aba = bab only at half-angle pi/4; at pi/6
      they do not. [checked] (KR 41; D82 main.c, init_su2)
53.2.1. So every zeta_12 result (Part V) is about a group generated
        by two rotations, and none can bear on knots.
53.3. D94's 2I generators (orders 6 and 10) are not conjugate, so
      they do not represent the braid group; the braid-correct zeta_10
      representation also has image 2I. [checked] (KR 41, 47)
53.4. Even at zeta_8, an entry is an invariant of a braid word as
      represented, not of a knot: sigma1 and sigma2 are conjugate and
      close to the same link, yet have different axes and so
      different cells. [checked] (KR 40)
53.5. An entry's depth, writhe and framing belong to the first word
      that produced it, not to the element: in a finite group many
      words of different writhe give one element. [checked] (KR 43)
53.6. Proposed audit P3 (ledger ...S0E97V): are verdicts constant on
      knot classes, and is D66's representation the TL one? [open]
```

## Part III. Boolean functions from bracket weights

```text
9. Do bracket values compute Boolean functions without training?
9.1. Yes: D29's catalog contains weights that compute XOR with no
     training (the forward DKC result). [checked] (KR 16)
9.1.1. The bracket values are exact (see 5.1); the search was re-run
       uncapped in floats in the 2026-10-03 reading, not in exact
       arithmetic.
9.2. Uncapped, 28,644 distinct triples work (10.9% of 262,144). D29's
     "65,536 valid triples", "~35%" and "10,357" came from caps, and
     its phase sweep has 72 angles, not 360. [checked] (KR 16)
9.3. 64 points with the same magnitudes and random phases give 35,248
     to 37,889 working triples (13.4-14.5%): that bracket values are
     unusually good at XOR is not supported. [checked] (KR 16)
9.3.1. The neuron is a soft XOR of two half-plane decisions, so it
       computes XOR whenever Re and Im realize two different threshold
       functions. [checked] (KR 16)
9.4. Proposed audit P4 (ledger ...JYSRSC): the same comparison
     exactly, over all 100 values. [open]

10. Why is three-input parity unreachable with the split-sigmoid
    neuron (D48)?
10.1. Because of the neuron, not the bracket. Its output is
      [Re z > 0] XOR [Im z > 0], the XOR of two threshold functions,
      and no pair of the 104 three-input threshold functions XORs to
      parity. [checked] (KR 19)
10.1.1. So parity is unreachable for every weight set, before any
        bracket is computed.

11. At what sector count does three-input parity first appear (D50)?
11.1. At k = 6 for the bracket catalog; re-run, the counts for
      k = 2..8 are 0 0 0 0 906 756 96. [checked] (KR 20)
11.2. Null catalogs (shuffled pairings, random magnitudes on the same
      directions, random Z[zeta_8] elements, random phases) also first
      reach parity at k = 6 and none below. [checked] (KR 20)
11.3. The bracket catalog behaves like a typical eight-direction star
      of integers. [checked] (KR 20)
11.4. k >= 2n is forced for any weights (inclusion encoding, no bias,
      sectors read mod 2): n weights need n different odd sectors. So
      an equal threshold cannot tell the bracket from a null.
      [checked] (KR 31)
11.5. Counts depend on the boundary convention (k = 6: 906 -> 1,002
      when the sectors are rotated by -1e-9); thresholds do not.
      [checked] (KR 32)
11.6. Above k = 6 the counts do differ: the decline after k = 6
      appears for the eight-direction catalogs (bracket, star,
      shuffle), not for generic or random-phase ones. [checked]
      (KR 20)
11.7. Proposed audit P5 (ledger ...DHY1TG): the same search exactly,
      with the nulls and both boundary conventions. [open]

12. How many of the 13 NPN classes of three-input functions do
    half-plane activations reach, on the multiplicative encoding
    z = w1^a w2^b w3^c?
12.1. 11 of 13; 0x06 and 0x1B are unreachable. [checked] (D61, D62;
      KR 30)
12.1.1. KR 30 gives a complete argument (arrangement vertices on
        (pi/12)Z^3, cell barycenters on (pi/48)Z^3), confirmed at
        N = 96, 192, 384 and on 20 million random configurations.
        D62's proof B is right once one missing case is added; its
        proof A covers one case only.
12.2. On the inclusion encoding, Re z > 0 is a perceptron with zero
      threshold and reaches 5 of 13; the inclusion split sigmoid
      reaches 12 of 13. [checked] (KR 27)
12.3. "delta = sqrt2 strictly dominates delta = 0" (13 against 12 of
      13) falls: it compared a multiplicative search with an inclusion
      one, and within one encoding the two roots agree. [falls]
      (KR 25, 27)

13. Does the two-sector activation reach 12 of 13 classes (D56)?
13.1. No. Exactly 11 of 13, including parity. [falls] (D117 Part B)
13.1.1. Class 0x06's 3 solutions came from overflowed long products
        (92 of 729 weight triples overflow); exactly it has 0.
13.2. Under the other four activations D56's class totals survive
      exactly (11 / 10 / 10 / 11), and its parity counts survive under
      all five; per-class solution counts move under every one.
      [exact] (D117 Part B)

14. Are the multi-input parity ceilings sound (D63-D65)?
14.1. The sets survive, but D63 and D64's ordered counts are a float
      zero-test artifact: 3,020 ordered XOR5 solutions in 27 sets,
      where permutation invariance requires 27 x 120 = 3,240. D65,
      summing exactly, gets 3,240. [checked] (KR 33)
14.2. The ceilings themselves: with parity-labelled sectors no k in
      2..16 gives XOR6 (D63-D65); with free labels XOR6 appears at
      k = 24 with zero margin, and XOR7 at k = 91 (D65). [unaudited]
      (AA theorems.md)
14.2.1. The atlas conflicts with itself on XOR5: only at k = 15 (D63)
        against k = 14 (D65).
14.2.2. XOR5 at k = 15 is above the bound of 10 (see 11.4); the
        ceiling of five belongs to the eight-ray, bounded-magnitude
        catalog. [checked] (KR 31)
14.3. Proposed audit P5 settles these too (see 11.7). [open]

15. Does the "Barrington-Radical Principle" (D102) hold?
15.1. No, as stated. Programs over abelian groups compute parity (over
      Z/2), so the implication is reversed. [checked] (KR 53)
15.1.1. What such groups provably cannot do: abelian and nilpotent
        groups cannot compute AND in polynomial length
        (Barrington-Straubing-Therien); for solvable groups this is
        open. (literature, cited by review I; not checked here)
15.2. These neurons add their weights rather than multiply them, so
      Barrington's theorem, which is about products, does not apply
      to them. [checked] (KR 47, 53)

16. Does the parity-lock theorem (D92, +-q encoding) hold?
16.1. Yes: a written proof in proofs/parity-lock-theorem.md, parts
      (a)-(e) checked by review I. [proved]
16.1.1. Part (f) ("standard functions") is right read as symmetric
        functions; "standard" is not defined there.
```

## Part IV. The zeta_8 catalog: the binary octahedral group 2O

```text
17. What group do D66's braid generators produce?
17.1. The binary octahedral group 2O: exactly 48 elements, element
      orders 1^1 2^1 3^8 4^18 6^8 8^12, 24 classes mod sign. [exact]
      (D118 Part A)
17.2. "The 24-cell is the braid image" falls. The 24-cell is the
      subgroup 2T; 2O is the 24-cell together with its dual. [falls]
      (D118; KR 34)
17.3. The catalog has exactly 13 rotation axes: 3 four-fold, 4
      three-fold, 6 two-fold. [exact] (D118 Part B)
17.4. The 9 null entries (Re q = 0) are the 9 half-turns: 3 about
      face axes, 6 about edge axes. [checked] (KR 44)
17.4.1. So "null" and "non-null" are unions of conjugacy classes, not
        the radical and its quotient.
17.5. A finite image at these roots is the classical picture: Jones's
      representations are finite at the low-order roots and dense from
      l = 5 on (Freedman, Larsen and Wang). [checked] (KR 34)
17.5.1. That finiteness concerns the semisimple quotient; on the
        Temperley-Lieb modules at delta = 0, sigma_i has infinite
        order (D98).
17.5.2. Whether D66's representation is that one is not established
        (see 53.1).

18. Which antipodal triples compute XOR6 with the 24-cell Voronoi
    activation (D66)?
18.1. Under some tie resolution: exactly the 96 mutually orthogonal
      triples. [exact] (D118)
18.2. Under every tie resolution: exactly the 24 triples inside one
      coset of Q8 = {+-1, +-i, +-j, +-k}, which form one orbit under
      the group's symmetries. [exact] (D118)
18.2.1. The catalog has 24 orthonormal frames; only the 6 that are Q8
        cosets contain a robust triple, and the other 18 contain none.
        So being a frame does not decide robustness. [exact] (D118)
18.3. D66's 35 is the lowest-index tie rule, which 92 of the 96
      symmetry maps break, so it is not a property of the weights.
      [falls] (D118)
18.4. "Beats the 6x6 geographic grid (3 winners)": the geographic
      cells were not recounted. [unaudited] (D118)
18.5. The 1,928 non-orthogonal triples fail under every tie rule: a
      pair sum is exactly equidistant from its own two members.
      [exact] (D118)
18.6. D66's "16 symmetry orbits" are classes of matching labelings,
      not orbits: the 24 robust sets form one orbit. [exact] (D118)
18.7. The activation is the catalog's own 24-cell Voronoi, a
      partition centred on the weights' own group and so tuned to
      them. [checked] (KR 35)
18.8. Proposed audit P2 (ledger ...5BB3SQ): the same
      characterization under group-free activations. [open]

19. Does 2I compute better than zeta_8 at the same catalog size (D94)?
19.1. Yes for XOR at N = 3-6, under every tie rule. At N = 6 the
      random-24 mean of 2I against zeta_8 is +82.7% (rule), +94.2%
      (robust), +37.2% (possible). [exact] (D119)
19.1.1. The independent oracles cover N = 3, 4 (and N = 3 for all 60
        of 2I); the N = 6 figures rest on main.c and D120's second
        in-house exact layer, and 2I's is a mean over 10 sampled
        subsets. Proposed audit P11 (ledger ...N4FWHD). [open]
19.1.2. Under a common table the N = 6 advantage is not general: 20
        random 24-subsets of 2I pass robust XOR in 7,840-14,815
        subsets under T13 (2O: 10,070). D119's comparison used each
        set's own table, and random 2I subsets have 16-21 own axes
        against 2O's 13, so more cells. [exact] (D127)
19.2. "2I wins at N = 7 (3.4x)" falls: D94 compared a scaled mean with
      a raw count. [falls] (D119 Part F)
19.2.1. On one scale, N = 7 XOR is about even (0.97-1.12x), and
        zeta_8 is ahead for AND at N = 7 and 8 (float counts on D94's
        samples).
19.3. "Solvability is the bottleneck" was refuted earlier with a Haar
      control (see 19.7). [checked] (KR 47)
19.4. D94's float counts are tie artifacts in both directions: the
      exact rule sometimes exceeds them. [exact] (D119)
19.5. 2I has exactly 120 elements, 60 classes mod sign and 31 axes;
      D94's float dedup was right. [exact] (D118 Part D)
19.6. D94's 2I is not a braid image (see 53.3).
19.7. With D94's activation, Haar-random unit quaternions beat 2I
      subsets, which beat 2O: XOR at N = 6, 38,939 / 23,137 / 12,983;
      at N = 7, 3,449 / 672 / 197. "The order is the order of
      genericity." [checked] (KR 47, floats)
19.7.1. Exactly, under one common table (T13 or R13), generic rational
        points (G) beat every catalog at every N = 3-7. At N = 7,
        robust XOR (T13): G 1,060-1,641, 2I-D94 754, 2O 456. The
        ordering holds; D127 reads it as genericity (height) more
        than group (see 47.1.5.2). [exact] (D127; G is generic but not
        Haar)
19.8. 2I's first 24 at N = 8: D94's single XOR success is 0 under the
      rule and robustly. [exact] (D119)
19.9. Proposed audit P1 (ledger ...E72XG3): group catalogs against
      knot-free point sets under one exact activation. [open]
19.9.1. Done in D127 (see 47.1.5, 48.1.2).
19.10. "2I beats 2O" at N = 7 holds for D94's first 24 of 2I (754
       against 456, T13), not for 2I: 20 random 24-subsets of 2I pass
       51-283, and the braid-correct first 24 pass 257. Which 24
       points are taken matters more than the group. [exact] (D127;
       logically follows 19.2)

20. What do D95-D97 show about the pieces of zeta_8?
20.1. Cell B (the six quarter-turns, half-angle 45 degrees, about
      three orthogonal axes) is 100% at every N under every
      resolution. [exact] (D121)
20.2. COMM survives N = 7 (8 sets) and NON-COMM dies (0), under every
      resolution. [exact] (D121)
20.2.1. COMM against NON-COMM is the 24-cell (2T) against its dual:
        two geometries. [checked] (KR 48)
20.3. B+C wins at every N, robustly: 96.7, 92.8, 71.7, 29.0%. [exact]
      (D121)
20.4. A+B+C(+E) beats the whole catalog at every N under rule, robust
      and possible. [exact] (D121)
20.5. "0 + 0 = 26%" (all-null, N = 4) falls. Every null set is tied;
      robustly 15 of 126 sets (11.9%). [falls] (D121)
20.6. "The optimal catalog is A+B+C" falls, in D96's own floats as
      well (B+C is higher at every N). [falls] (D121)
20.7. Cell C at N = 6 is decided by ties: rule 16 of 28 (57%, as in
      floats), robust 4 (14%), possible 28. [exact] (D121)
20.8. 2I's first 12 beat zeta_8-COMM at N = 3-5, robustly (1.56,
      1.63, 1.98x). [exact] (D121)
20.9. "Removing C hurts most at N = 3-5, B at N = 6" holds robustly.
      [exact] (D121)
20.10. D94 and D95 count the same table differently in floats (their
       floats differ in the last bit at exact ties); exactly, the two
       agree. [exact] (D121)
20.11. zeta_8 N = 7 XOR, exhaustive: rule 606, robust 456, possible
       2,777. [exact] (D121)

21. Why does cell B compute parity?
21.1. In the signed encoding, weights with a common real part c give
      Re S = c(2k - N): the sum's real part records the Hamming
      weight, and parity is a function of it. [checked] (KR 51)
21.1.1. Half-turns have c = 0, so their sums carry no Hamming weight
        in Re S, and cells A and D alone compute nothing under the
        rule or robustly (D121). Yet A+D computes 15 of 126 robustly
        at N = 4 (see 20.5), so a zero real part is not the whole
        story.
21.2. By half-angle theta, cell B is 100% robust at every N on exactly
      two windows, (21.552, 33.355) and (38.794, 79.453) degrees, with
      closed-form ends arctan(c tan(j pi/24)). [exact] (D122)
21.3. On [33.355, 38.794] degrees, N = 5 fails robustly; at integer
      degrees the band is 34-38. [exact] (D122, D123)
21.3.1. D97 read this band as an "isolated resonance at 35 degrees".
        [falls] (D122)
21.4. 45 degrees is not special: it lies on a breakpoint (u = 1), not
      at the plateau's midpoint. [exact] (D122)
21.5. Total collapse at 90 degrees holds under each of the four
      uniform tie rules (sector lower/upper x axis first/last) and
      robustly; "possible" is vacuous there. [exact] (D122)
21.6. D97's float sweep equals the exact rule at all 21 of its angles:
      the error was the reading, not the arithmetic. [exact] (D122,
      D123)
21.7. At integer degrees, robust 100% at every N holds exactly at
      22-33 and 39-79. [exact] (D123)

22. Do the 9 null entries of zeta_8 carry capacity (D84)?
22.1. No. The 15 non-null entries reach XOR8, as all 24 do, under the
      exact rule and robustly. [exact] (D124)
22.2. With their own 9-axis table, the 9 null entries alone reach no
      XOR level under the rule or robustly. [exact] (D124)
22.2.1. Under "possible" they reach XOR14, because every null sum is a
        two-way tie: vacuity, not capability. (see 3.3.1)
22.2.2. With the full 13-axis table, null-only reaches XOR6 with 4
        sets, under the rule and robustly. [exact] (D124)
22.3. Removing the nulls removes winners, not the maximum: XOR8 sets
      402 -> 106 (rule), 218 -> 36 (robust). [exact] (D124 Part D)
22.4. D84 recorded "non-null XOR6, null-only XOR6, worse than a
      random 15". [falls] (D124)
22.4.1. D84's own code prints XOR8 and 0 on every build tried; where
        the recorded figures came from is unknown. [not-reproduced]
        (D124)
22.5. In D84's search, 2,686,124 of 20,965,173 cell decisions (12.8%)
      are exact ties that the floats resolve differently from the
      rule; none is a real float error. [exact] (D124 Part C)
22.5.1. Ties overall are more: 3,977,481 null-sum ties, 1,395,665
        axis ties and 4,285,855 non-null sector ties, so at least a
        fifth of all decisions. (see 43.1)
22.6. All 10,000 sampled random 15-subsets reach exactly XOR8 under
      the rule, and the non-null subset equals that distribution; the
      exhaustive check over all 1,307,504 has not been run. [exact]
      (D124 Part E)
22.6.1. Proposed audit P6 (ledger ...9ZQ5E1): all 1,307,504. [open]
22.7. XOR8 winners under-use nulls: 0.746 per set against 1.500 for
      random sets (ratio 0.50). [exact] (D124 Part F)
22.7.1. D84's "nulls are specifically required" (0.79, from its first
        32 winners) read a ratio below 1 the wrong way. [falls]
22.8. The k = 1 "synergy" (null-only 0 + non-null 0, full 36) is an
      activation-table confound; like for like, the synergy is the 24
      mixed sets. [exact] (D124 Part F)

23. Is there a zeta_8 / zeta_12 "regime transition" in null
    indispensability (D87)?
23.1. No. Its zeta_8 half falls with 22.4; its zeta_12 half holds only
      as far as the search reaches (see 29.1). [falls] (D124, D125)
```

## Part V. The zeta_12 catalog (not a braid image: see 53.2)

```text
24. Do the zeta_12 catalogs saturate at 512 directions (D79-D84)?
24.1. No. 512 was MAX_DIR. The 4,096-entry catalog has 2,043 axes,
      and D80's groups have 1,708 to 2,431. [falls] (D117 Parts D, F)
24.2. D80's finiteness verdicts are unchanged without the cap.
      [intervention] (D117 Part F)
24.3. "After round 7 directions saturate; XOR10 -> XOR12 needs finer
      angles, not more directions" falls: depth 7 adds 468 new
      directions, and depth 8 at least 1,068. [falls] (D117 Part F)
24.4. These catalogs are not images of the braid group (see 53.2).
      [checked] (KR 41)
24.5. D82's round 8 is truncated by MAX_QCAT (1,804 of 2,304
      elements); the full round adds 1,124 axes and depth 9 adds
      2,156. [intervention] (D117 Part F)
24.6. D84's zeta_12 census is 2,043 / 67 / 1,922 / 54 (total /
      null-only / non-null-only / both), not 512 / 29 / 453 / 30.
      [intervention] (D117 Part F)

25. Does zeta_12 reach XOR12 (D79)?
25.1. Yes. Rounds 6-8 of D82's catalog contain XOR12 sets under the
      rule and robustly, every stored winner re-decided by an
      independent oracle (5,149 winners, 0 wrong). [exact] (D125)
25.1.1. The maxima and counts come from the greedy search and are
        [exact-search].
25.2. D79's XOR10 = 124 at k = 8 is a cap: 214 at k = 6 with all 112
      axes of its 256-entry catalog, both extensions of a capped
      parent set. [falls] (D117 Part D)
25.3. D79's XOR12 with the 4,096 catalog reproduces; "50+" is a stop
      at 50. Uncapped, the smallest ladder k is 6 (12,264 cells),
      against k = 8 (4,104 cells) capped, so the capped result is the
      stronger statement, about a coarser activation. [intervention]
      (D117 Part D)

26. Does capacity grow as "max XOR = depth + 6" (D82)?
26.1. The line fits rounds 0, 2, 4, 6 only. At odd rounds max XOR is
      depth + 5, except round 1 under the rule (depth + 7, reached
      only through exact ties). [exact-search] (D125)
26.2. Above depth 6 the law has no support: at rounds 7 and 8 the
      recorded XOR12 is the search's ceiling of 6 weights.
      [exact-search] (D125)
26.3. Extended one level under the exact rule (from its first 32
      XOR12 winners, with 512 axes and with all axes), the search
      finds no XOR14 at round 7 or 8. [exact-search] (D125)
26.3.1. This bounds the search, not the catalog: an XOR14 set may
        exist outside what it explores. (see 51)
26.4. D81's logarithmic law (0.62 XOR per doubling) is fitted to the
      same nine points and shares the ceiling; its extrapolation
      (XOR14 at about 38,000 entries) rests on it. [exact-search]
      (D125)
26.obj.1. Objection: "depth + 6" fits rounds 0-6 exactly, so it should
          be expected to continue.
26.obj.1.1. Reply: it fits only the even rounds, and its first
            testable prediction (XOR14 at depths 7-8) does not appear
            where the search can see.

27. At matched size, do deep entries beat shallow and strided ones
    (D82 Part D)?
27.1. In D82's own entry order, yes: deep 564 reaches XOR12, shallow
      and strided 564 reach XOR10, under every semantics.
      [exact-search] (D125)
27.2. In other orders, no. Over 10 seeded orders each, strided 564
      reaches XOR12 in 8/10 (rule and robust), deep 564 in 10/10
      (rule) and 7/10 (robust), shallow 564 in 1/10, and 9 of 10
      random 564-entry samples reach it. [exact-search] (D125 Part E)
27.3. "Algebraic coherence beats vocabulary" and "deep entries are
      2x more efficient" fall. [falls] (D125)
27.4. What remains: the first 564 entries (rounds 0-5, 239 axes)
      rarely reach XOR12, and sets drawn from deeper rounds mostly
      do. Depth and vocabulary are not separated. [exact-search] (D125)

28. Do XOR12 winners need depth-8 entries and a shallow core
    (D82 Part B)?
28.1. "XOR12 is the first level to use depth-8 entries" holds only
      with the 512-axis cap; uncapped, no XOR12 winner uses a depth-8
      entry, under the rule or robustly. [falls] (D125 Part D)
28.2. Every winner's depth-0 core comes from the search (triples
      [0, 1, x] from the first 100 entries, extended in order).
      "The generators are irreplaceable" describes the search, not the
      catalog. [falls] (D125 Part D)

29. Are zeta_12's 121 null entries dispensable (D87)?
29.1. Yes, as far as the search reaches: the 3,975 non-null entries
      reach XOR12 under the rule and robustly. [exact-search] (D125
      Part F)
29.1.1. "32 = 32" is the search's stop at 32 winners per level, not a
        count of capacity.

30. Does D90's deep subset have fewer distinct sum angles?
30.1. Yes. Uncapped: deep 6,179, strided 10,160, random 10,184
      (shallow 2,559). [intervention] (D117 Part C)
30.1.1. D90's two "failed predictions" compared 2048 < 2048, its
        MAX_DISTINCT cap.
30.2. D90's "73 distinct angles in 50,000 pairs, against 1,313"
      counted only the first 2,048 pairs: uncapped 2,226 and 7,456.
      The direction holds; the numbers do not. [intervention] (D117
      Part C)

31. Do D100's and D106's sign-hash counts describe the matrices?
31.1. They describe the overflowing hash that ran. A wrapping hash
      reproduces them byte for byte; the intended hash moves most
      counts (D100 deep XOR8 1,169 -> 3,576). [intervention] (D117
      Part E)
31.2. D106's verdict, that pseudo-Anosov braids do not compute better,
      is unchanged under the intended hash. [intervention] (D117
      Part E)
31.2.1. Its classes were D106's rule, not the braids' dynamics: in
        exact classes most of its "periodic" side is not periodic
        (see 54.5). Whether the verdict holds over exact classes is
        open (see 54.9).
31.2.2. On capacity (linear regions, not computation), positive
        entropy buys no exponential growth on B_3: see 56.
31.3. Under the intended hash AND3 = OR3 (320,265 each), as negation
      symmetry requires; D106's "no gap above 3.7 points" is
      hash-specific (6.6 and 7.3 points, favouring periodic braids).
      [intervention] (D117 Part E)
31.4. Burau at t = -1 is the bracket's own W_{4,2} representation, so
      D106's entropy null is a property of the sign readout.
      [checked] (KR 54)
31.4.1. Consistent: at A = -zeta_8, A^-4 = -1, and the 98 pairs of
        distinct braids that share a TL matrix at length 6 also share
        their reduced Burau(-1) matrices (none shares the unreduced
        matrix). TL separates more braids than Burau(-1) does, through
        the phase A^{e(w)} on the radical (see 42.1.1). [exact] (D128
        Part C: characteristic polynomials; matrices: the D128
        fact-check review's script)

32. Does framing change zeta_12 capacity (D83)?
32.1. Its bracket against Jones-normalized comparison (43 against 31
      angles, max XOR 12 against 10) is unchanged without the cap; the
      mixed-writhe XOR12 winners go from 4/32 to 0/32. [intervention]
      (D117 Part F)
32.2. Its exact recount is not done. [unaudited]
32.3. "Framing" has no knot-level meaning here: D83's normalization
      multiplies q by a rotation about one axis, while the Jones factor
      (-A^3)^(-w) is a scalar, and writhe belongs to the first word,
      not the element (see 53.5). [checked] (KR 43)

33. Do D89-D93's results hold?
33.1. D89, D91 and D93 build on D82's depth law and "algebraic
      coherence" (see 26, 27) and have not been audited. D90 is partly
      audited (see 30); D92's parity-lock does not depend on the depth
      law (see 16). [unaudited]
33.2. Proposed audit P9 (ledger ...6AED71). [open]
```

## Part VI. Construction words

```text
34. Are D112's simplified stick polygons the knots they name?
34.1. No: 10 of 12 are singular (the polygon passes through itself);
      only 4_1 and 6_3 are knots. [exact] (D114)
34.2. Every simplification path breaks the embedding, at a deletion
      the triangle-move check refuses. [exact] (D114)
34.3. D112's raw braid polygons are all genuine knots with the table
      Alexander polynomial. [exact] (D114)

35. Is 6_3 a "knot factory" (37 polynomials, D112)?
35.1. No. Exactly 6 Alexander polynomials: unknot, 3_1, 4_1, 5_1, 5_2,
      6_3. [falls] (D114)
35.1.1. The 37 came from long overflow: the same code with 128-bit
        integers gives the exact 6. [intervention] (D114)
35.2. On honest polygons (legal moves only) every alternative of
      every knot is simple; D112's 50-100% self-intersection rates
      are artifacts. [exact] (D115)
35.3. D112's reachability and stability fall: 6_3's out-degree 10 ->
      5; its "universal core" holds in 11 of 20 trials, not 20; the
      union is 9, not 118. [falls] (D114)
35.4. "100% palindromic after the filter" certified nothing: exact
      Alexander polynomials are palindromic by theorem, and D112's
      wrong ones were palindromic too. [exact] (D114)

36. What do honest construction-word spectra contain?
36.1. Typically small, orderly spectra: the knot, its mirror, simpler
      knots, and mostly unknots. [exact] (D115, D116)
36.1.1. Exceptions: 6_1 reaches 7_4 (det 15 > 9); one 4_1 polygon
        (r17) reaches 5_2; one 6_3 polygon (+23) reaches 8_14; 8_18
        reaches 8_20. (D115, D116)
36.2. 7_2's 15-vertex polygon is the exception: 47 distinct Alexander
      polynomials, determinants up to 85, and 50 distinct prime knots
      of the 13-crossing table (first candidate, up to mirror), many
      of them non-alternating. [exact] (D115, D116 Revisions II)
36.2.1. The spectrum depends on the polygon: over all 15
        fewest-vertex 6_3 polygons the core is {6_3, 3_1, 3_1*,
        unknot}, and the union is 17 named classes. [exact] (D115,
        D116)
36.3. Of the 9,768 alternatives, 5,084 are proven unknots and 4,546
      are named by the table. [exact] (D116 Revisions II)
36.3.1. 138 more are unknots by citation, not proof: Jones detects the
        unknot up to 24 crossings (Tuzun and Sikora). (D116)
36.3.2. A name means Alexander and Jones match a table knot; some are
        ambiguous (8_16|10_156; 12n_468|13n_413|13n_2357), and sums of
        three or more knots were not searched. (D116)
36.4. Five of D112's braid knots (3_1, 5_1, 7_1, 7_3, 7_4) are
      KnotInfo's mirror images: a convention, not an error. [exact]
      (D116 Part A)

37. Is D110's "complement chirality theorem" a fact about knots?
37.1. No. It is true by construction: vertices 0, 1, 2 lie on the
      mirror plane, so the complemented word gives the mirror image.
      [proved] (D115)

38. Is D110's majority classifier right?
38.1. Yes. Its 4-check classifier is perfect on the 512 alternatives:
      64 figure-eights, 384 unknots, 32 + 32 trefoils. [exact] (D114)
38.2. Its "left-trefoil if b2 = 1" is right-handed by Jones: the names
      are swapped. [falls] (D114)
38.3. D110's mirror test fails widely in its 64-bit arithmetic,
      through overflow; with 128-bit integers it holds. [checked]
      (KR 58)

39. Do D111's statistics (det spectra, influence, complement closure,
    Hamming profiles) hold?
39.1. Partly. Its det spectra fall: recomputed exactly they collapse
      (6_3: 22 dets -> 5). [falls] (D114)
39.1.1. The derived statistics (influence, Hamming profiles) were not
        recomputed: unsupported, not refuted. [unaudited] (D114)
39.1.2. Complement closure is true by construction (see 37.1).
```

## Part VII. Temperley–Lieb algebra

```text
40. Is the structure of TL_n at delta = 0 (radical, sandwich theorem,
    cellular decomposition) sound?
40.1. Recorded as proved or exhaustively verified for n = 2-8 (D35-D39,
      D51-D52), often citing known theory (Graham-Lehrer;
      Goodman-de la Harpe-Jones). [unaudited] (AA theorems.md)
40.2. D51 and D52's tables follow from textbook cellular-algebra
      formulas: a thirty-line rule reproduces 27 of their 28 rows
      (TL2(0): rad^2 is 0, where the rule gives 1). [checked] (KR 21)
40.3. rad^2 = A/rad holds as bimodules, not as algebras (rad^2 times
      rad^2 = 0), so the atlas's first "novel" result falls as stated.
      [falls] (KR 22)
40.4. "Every TL_n(0) admits exactly three orthogonal idempotents"
      describes the search: TL7(0) has 35 primitive orthogonal
      idempotents. [falls] (KR 22)
40.5. Proposed audit P7 (ledger ...6DD3NK). [open]

41. Is the indecomposability parameter b = -5/8 for dense polymers
    (D85)?
41.1. Not established. [checked] (KR 45)
41.1.1. The delta-form leaves b gauge-dependent: b = -2.5 - 2c^2
        along one kernel direction.
41.1.2. The division by 4 is asserted, not derived.
41.1.3. -5/8 is percolation's value; delta = 0 is dense polymers, and
        b is a limit over growing lattices, not one n = 4 value.
41.2. Proposed audit P10 (ledger ...G9DPA5). [open]

42. Do D101, D103 and D105 test simplicity correctly?
42.1. No. They take the radical as the vectors killed by every e_i,
      which detects only copies of the one-dimensional
      representation. [checked] (KR 52)
42.1.1. W_{4,2} at delta = 0 has such a copy: in D106's basis
        v = (1, 0, -1) is killed by every e_i, so each sigma_i acts on
        it by A. The TL form [[0,1,0],[1,0,1],[0,1,0]] is singular,
        with radical spanned by v. [proved] (D128 plan.md, T6d)
42.2. W_{6,2} and W_{8,2} are not simple: they have proper submodules
      of dimension 4 and 14. [checked] (KR 52)
42.3. D100's frozen radical exists for even n only. [checked] (KR 50)
```

## Part VIII. Method findings

```text
43. Are exact ties rare in these catalogs?
43.1. No, they are the norm. Every null sum lies on a sector boundary
      at every even k. In D119, every near-boundary sector decision
      (397,565,109) and every near-tie axis decision (93,638,928) was
      an exact tie. In D84's search ties are at least a fifth of all
      cell decisions. [exact] (D119, D120, D124)
43.2. So float verdicts are arbitrary exactly where the structure
      lives, and every capacity count should report rule, robust and
      possible. [interpretation]
43.3. In D125's zeta_12 rule search, 19.3 M of 105 M cell decisions
      needed the exact path, and 15.5 M of those were ties. [exact]
      (D125)

44. What faults did the old demos have?
44.1. Silent caps (D29, D79-D84, D90), long and int overflow (D56,
      D100, D106, D111-D112), float ties (D66, D84, D94-D96),
      greedy-search artifacts (D82), mislabelled knots (D2-D17),
      figures the code does not print (D84) and mixed scales (D94).
      (see 6.1, 24.1, 30.1.1, 13.1.1, 31.1, 35.1.1, 18.3, 43.1, 27.2,
      7.1, 22.4.1, 19.2)
44.1.1. In the cyclotomic line, silent caps were the dominant fault.
        (D117 Lessons)
44.2. A cap can decide a test: D90's failures compared 2048 < 2048.
      (see 30.1.1)
44.3. Recorded figures that the demo's code does not print are
      possible (D84). (see 22.4.1)

45. Is a greedy search's result a property of the catalog?
45.1. No. D82's results depend on the entry order, the 100-entry
      triple pool and the 32-winner stop (see 27.2, 28.2). A search
      result must state its bounds.

46. Do free symmetry checks catch faults?
46.1. Yes. Permutation invariance (see 14.1), the AND/OR negation
      symmetry (see 31.3) and the group's symmetries (see 18.3) would
      each have flagged a fault at no cost.
46.2. The mirror pairing of construction words is another (see 37.1,
      38.3).
```

## Part IX. Open questions

```text
47. Is DKC capacity a property of the knots, or of the point set?
47.1. Not of the knots, as far as the record goes; whether the group
      structure helps or hurts against a fair null is open.
      [interpretation] (XL synthesis 2026-10-13, corrected)
47.1.1. There is no knot -> element map: an entry is an invariant of a
        braid word as represented (see 53.4). The zeta_12 and D94 2I
        catalogs are not braid images at all (see 53.2, 53.3), and
        the bracket catalogs are classical invariants times a phase
        (see 8.2, 52.3).
47.1.2. The neurons add their weights, so a cell depends only on the
        points, not on the group they generate (KR 47). Capacity then
        follows from directions, angles and where exact ties fall
        (see 18.2.1, 21.1, 43.1).
47.1.3. The knot-free controls that exist point the same way: random
        phases do at least as well as D29's brackets (see 9.3); null
        catalogs differ from the bracket only through their directions
        (see 11.6); Haar-random points beat 2I, which beats 2O, under
        D94's activation (see 19.7, floats).
47.1.4. Every audited activation table is built from the point set
        itself (D66, D79-D84, D94; see 18.7, 22.2.2): a third
        ingredient that any test must control.
47.1.4.1. D127 judges every set with two common tables. The comparison
          with generic points holds under both. 2O's lead over the
          height-matched spheres at N = 7 holds under T13 (its own
          table) but not under R13 (265 against up to 272). [exact]
          (D127)
47.1.5. With one exact activation and a common table (D127: D94's
        phase cell, signed encoding, k in {6, 12, 24}; T13 = 2O's 13
        axes or R13 = 13 random directions), the second clause
        resolves only per null and per N:
        - against generic rational points the catalogs lose, at every
          N = 3-7;
        - against height-matched points (spheres of height 5 and 6,
          20 seeds each) there is no consistent difference: inside or
          below the range at N = 3-4, inside at N = 5-6 (2I-D94
          above), above at N = 7 only for 2O under T13 and for
          2I-D94.
        [exact] (D127; independent oracle: every N = 3-4 row and six
        N = 5-7 rows re-derived, 9,900 N = 5-7 subsets agree)
47.1.5.1. P1's registered rule ("supported if no catalog beats its
          matched nulls robustly") cannot be applied. It names
          neither the nulls nor N, and it cannot reach the first
          clause, since no entry is a knot invariant (see 53.4). No
          verdict on 47.1 is drawn from D127. (D127 review M1)
47.1.5.2. The deficit against generic points follows height at least
          as much as group: at N = 5-6 robust XOR roughly orders by
          height (G and AX, then AN, then catalogs and spheres, then
          LIN). [interpretation] (D127)
47.1.5.3. Only XOR is a property of a point set under the signed
          encoding. AND and MAJ change with the sign chosen for each
          weight: 2I-D94's MAJ at N = 7 is 43,620 with canonical signs,
          23,173 with D119's, and 262-3,671 under random signs.
          [exact] (D127)
47.obj.1. Objection: the catalogs are not arbitrary point sets. The
          cleanest results (18.2: Q8 cosets) are group-theoretic, so
          the algebra matters.
47.obj.1.1. Reply, in part: the neurons see points, not products (see
            47.1.2). But being a frame does not decide robustness:
            only the 6 frames that are Q8 cosets contain a robust
            triple (see 18.2.1), and the activation is the group's own
            Voronoi (see 18.7). Whether coset structure matters under
            a group-free activation is untested (see 18.8).
47.obj.2. Objection: no audit has tested a knot-specific prediction,
          so 47.1 rests on absence of evidence.
47.obj.2.1. Reply: granted, and on catalogs whose entries are not knot
            invariants no knot-specific test is possible (see 53.4).
            What can be tested is whether group structure helps or
            hurts against a matched null under a fixed activation
            (see 48.1), and whether verdicts are constant on knot
            classes (see 48.3).

48. What experiment would decide 47?
48.1. A matched null under one fixed activation: group catalogs
      against knot-free point sets (exact rational points on S^3; the
      same axes with random angles; the same angles with random axes),
      each judged with its own table, with a common group-built table
      and with a group-free table, under rule, robust and possible,
      with tie counts reported. Proposed audit P1 (ledger ...E72XG3).
      [open]
48.1.1. Generic random points have no exact ties, so matching a
        catalog's tie structure needs a construction, not a sample.
48.1.1.1. Exactly: generic points (G) have no tied subset at any N, and
          the height-matched spheres are as tie-dense as the catalogs
          or more (H6: 290k-303k of 346,104 tied at N = 7). [exact]
          (D127)
48.1.2. Done in D127, with the nulls of 48.1 plus height-matched
        spheres, entry-order and sign controls (see 47.1.5); the
        registered decision rule was underspecified (see 47.1.5.1).
48.2. Float nulls already exist (see 9.3, 11.6, 19.7) and generic
      points do at least as well; the likely outcome is that group
      structure lowers capacity, through ties and coincident sums.
      [interpretation] (KR 42, 47)
48.2.1. Not confirmed. The generic nulls differ from the catalogs in
        height as much as in group. Among height-matched sets, more
        coincidences did not mean fewer passes: 2O has 4-17x H6's XOR
        concursus at N = 7 and passes more often. [exact] (D127)
48.3. Are XOR verdicts constant on braids with the same closure?
      Proposed audit P3 (ledger ...S0E97V); KR 40 predicts no. [open]
48.4. What do the catalogs' integer relations show (D126)?
48.4.1. A signed coincidence (sum x_i p_i = 0, x_i in {-1, 0, 1})
        joins two input patterns with the same sum, so no activation
        can separate them. It is not a boundary tie, which depends on
        the activation. [definition] (D126)
48.4.1.1. That coefficient set is right for the signed encoding (D94,
          D119-D123). The antipodal 0/1 encoding (D66/D118, D79-D84/
          D124, D125) joins patterns by relations in {-2 .. 2}.
          [checked] (D126 review F3)
48.4.1.2. The antipodal encoding also forces coincidences in every
          point set: x_i = 0 comes from both bits on or both off.
          They join patterns of equal parity, so XOR is unaffected;
          AND and MAJ are not. [checked] (D126 review R7)
48.4.2. Every catalog has relations of norm 3 (p_a +- p_b +- p_c =
        0): 16 in 2T, 32 in 2O, 200 in 2I. The coincidences grow
        fast; in 2I there are 1,800, 16,392 and 169,100 at support 4,
        5 and 6. [exact] (D126, independent oracle)
48.4.3. No knot-free null has any relation within the catalog's radius
        (10, 8, 5). Tested: 30 sets matched in count, either r g over
        the catalog's own field (same relation rank) or rational.
        [exact] (D126)
48.4.3.1. Narrowed: those nulls have coordinate denominators near
          10^30. At small height, random rational unit points have
          many relations: about 4,500 with entries in {-1, 0, 1} at
          height 6 (N = 24), against 2O's 9,472. 48.4.3 measures
          height, not group structure. [exact] (D126 Part F, oracle)
          [last sentence: interpretation]
48.4.4. A null with a catalog's entire relation lattice is a linear
        image of the catalog. A knot-free null can therefore match
        the profile only in part (e.g. its shortest shells), and must
        say which part. [proved] (D126) [second sentence: falls, see
        48.4.4.1]
48.4.4.1. The first sentence stands. The second falls for 2O: a unit
          point set with exactly 2O's relation lattice that is not
          congruent to 2O exists (2T u r sigma_1 2T). [exact] (D126
          Part E, the review's counter-example) [grade corrected in
          round two: the existence is exact; 48.4.4 is what falls]
48.4.4.2. On the unit sphere, a whole-lattice null must be congruent
          to 2T (its points pin every quadratic form, rank 10 of 10).
          [proved] (D126 Part E)
48.4.4.2.1. Precisely: over a real field, every real embedding of
            the null is congruent to 2T. [proved] (D126 review R3)
48.4.4.3. For 2I (quadric rank 35 of 36) this is open. [open]
          [superseded by 48.4.4.4]
48.4.4.4. For 2I it holds too: over a real field, a whole-lattice
          unit null is congruent to 2I as a set (possibly through
          the Galois twist), and no rational unit point set has
          2I's whole lattice. The forms vanishing on 2I are exactly
          the sqrt5 part of the norm; the two embedding norms are
          the only positive semidefinite rank-4 forms in the span.
          [proved] (D126 Part E; sketched by the reviewer, checked by
          the author)
48.4.5. So the catalogs force pairs of input patterns that no
        activation can separate, and generic point sets have none at
        these lengths: the coincident-sum half of 48.2's prediction.
        [interpretation] [falls, see 48.4.5.1]
48.4.5.1. Small height alone produces many coincidences (see
          48.4.3.1). What separates the catalogs from height-matched
          sets is the norm-3 relations (16, 32, 200, against at most
          1) and the odd-norm ones (see 48.4.7): 2O has 2,720 to norm
          8, against 0 at odd height and about 1,000 at height 6.
          [exact] (D126 Parts B, F) ["at most 1" falls, see 48.4.5.2]
48.4.5.2. Against random rational unit points of nearby small height
          (the catalogs' own heights 2 and 4 have only 2T): norm-3
          relations 2O 32 against 1-2 (H = 6, N = 24, 8 seeds); 2I
          200 against 25-39 (H = 6, N = 60). Odd-norm: 2O 2,720
          against 970-1,056; 2I 16,592 against 6,603-7,661. At odd
          height the odd counts are 0 by 48.4.8. [exact] (D126 Part
          F, oracle-pinned; review R1)
48.4.5.3. So the catalogs have more norm-3 and more XOR-relevant
          coincidences than these controls, by 2.2-30x depending on
          the comparison; one sample of controls, no significance
          test. [interpretation]
48.4.6. Whether these coincidences lower capacity, and which matter
        for which function, needs activations: P1. [open]
48.4.6.1. Partly answered: coincidences alone do not decide the counts.
          LIN has 2O's whole relation lattice (identical XOR concursus
          in every row) yet passes 0-5 robustly at N = 7 against 2O's
          456. LIN also changes the weights' norms (max/min |Ap|
          2.5-5.0), so geometry and norms are not separated. [exact]
          (D127)
48.4.7. Only relations of odd norm join input patterns of opposite
        parity, in both encodings, because sum x_i = sum x_i^2 (mod
        2). So only they can matter for XOR. 2I's 1,800 four-term and
        169,100 six-term coincidences cannot. [proved] (D126, review
        F4)
48.4.8. Rational unit points of odd height have no relation of odd
        norm: |v|^2 odd forces an odd coordinate sum, so every
        relation has an even coefficient sum. [proved] (D126 Part F)
48.4.8.1. Stated generally: the points v_i/H share one odd
          denominator H. Mixed odd heights are covered by taking the
          common one. [proved] (D126 review R4)
48.4.9. 2O's profile is 2T's squared (theta_2O = theta_2T^2): 2O = 2T
        u sigma_1 2T in separate coordinate blocks, so 2O adds no
        relation structure beyond two copies of 2T. [proved] (D126
        Part E, review F11)

49. Do robust XOR sets in other groups follow a subgroup structure, as
    Q8 cosets do in 2O (see 18.2)?
49.1. In 2T, Q8 is normal of index 3: does the coset characterization
      recur? [open]
49.2. In 2I it cannot in the same form: 2I/{+-1} = A5 is simple, so 2I
      has no other proper normal subgroup. Which structure, if any,
      decides there? [open]
49.3. Proposed audit P2 (ledger ...5BB3SQ). [open]

50. Do the TL radical results survive an exact audit at a second size
    (see 40)? [open]
50.1. Proposed audit P7 (ledger ...6DD3NK). [open]

51. Is there an XOR14 set at zeta_12 depths 7-8 outside D82's greedy
    search (see 26.3.1)? [open]
51.1. Proposed audit P8 (ledger ...NPCP6E), low priority: this catalog
      is not a braid image (see 53.2). [open]
```

## Part X. Braid dynamics

```text
54. Does D106's classification of 4-strand braids into pseudo-Anosov
    and periodic hold? (logically follows 31)
54.1. No. Its rule decides exactly "Burau(-1) has spectral radius
      > 1", a sufficient condition for positive entropy, not a
      classification. [proved] (by 54.1.1 and 54.1.3; D128 Part C
      checks it on 5,121 braids)
54.1.1. On B_4 at t = -1, p(1) = 0 for every braid: unreduced Burau
        fixes (1, 1, 1, 1), which lies in the kernel of
        (1, t, t^2, t^3) when t^4 = 1 and t != 1. So the rule's first
        branch never fires. With det = 1, p = (x - 1)(x^2 - (tr - 1)x
        + 1), so |1 - tr| > 2 exactly when the radius is > 1. [proved]
        (D128 review L1; checked on 5,121 braids)
54.1.2. Its "pA" set equals the set with Burau(-1) radius > 1 on all
        5,121 braids of B_4 up to length 6. [exact] (D128 Part C)
54.1.3. D106's matrices are not the standard reduced Burau(-1): its
        generators are conjugate, by diag(1, -1, 1), to the
        inverse-transposes of the standard ones. Those have the same
        characteristic polynomial (det 1 and p(1) = 0 give c1 = tr),
        so 54.1.1 and 54.1 hold for D106's matrices on every braid.
        [proved] (D128 fact-check review: the six generators, exact)
54.2. Its "pA" never marks a zero-entropy braid: radius > 1 forces
      positive entropy by Kolev's bound (literature, cited by the D128
      review; not checked here). It can still mark a reducible braid
      "pA" (see 54.7.1).
54.3. Its "periodic" is wrong for 1,008 of the 4,014 positive-entropy
      braids of B_4 up to length 6. Of the 2,115 braids it calls
      periodic, 171 are; 936 have zero entropy without being
      periodic. Both counts include the identity, which D106 lists
      apart (2,114 and 170 without it). [exact] (D128 Parts B-C; the
      review's oracle agrees)
54.3.1. The first in breadth-first order (one of 16 at length 3) is
        s1 s3^-1 s2, conjugate to s1 s2 s3^-1. Its class is exact;
        its rate, 2.29663, is MEASURED and matches the minimal
        dilatation of B_4, the largest root of x^4 - 2x^3 - 2x + 1
        (literature: Song, Ko and Los 2002, proof completed by Ham
        and Song, short proof by Lanneau and Thiffeault; cited by the
        D128 review, not checked here). Burau certifies only 1.9577
        at a cube root of unity. [exact] (D128: the class)
54.3.2. Burau at a primitive cube root of unity certifies all 1,008.
        [exact] (D128, plexus_burau_radius_maior)
54.4. D106's catalog is the exact shortest-length enumeration through
      depth 5. At depth 6 it has 3,444 braids where there are 3,542.
      [exact] (D128 Part C)
54.4.1. Why? D106 deduplicated by its TL matrix alone, and at length
        6, 98 pairs of distinct braids share one. A replica of its
        search reproduces its table, braids and "pA" counts, at
        depths 0-6. [exact] (D128 Part C)
54.4.2. Its depths 7-8 (capped at 32,768 entries) are not checked.
        [unaudited]
54.5. D106's contingency table (its first 128 entries) set 88
      "periodic" against 39 "pA" entries. In exact classes the 39
      have positive entropy; the 88 are 8 periodic, 64 zero-entropy
      non-periodic and 16 of positive entropy. [exact] (D128 Part C)
54.6. Does the "pA" fraction rise with depth (D106: 0% to 77.2%)?
54.6.1. Yes in direction: the exact positive-entropy share rises with
        length on B_3 (to 8) and B_4 (to 6). [exact] (D128 Part B)
54.6.2. D106's level understates it by up to 22 points (21.7 at
        length 5): 30.8, 32.7,
        43.2, 50.6, 63.7% (its rule on exact classes) against 30.8,
        49.0, 58.6, 72.3, 83.5% at lengths 2-6. [exact] (D128)
54.7. D106's 8 depth-2 "pA" braids have Burau radius phi^2 exactly,
      as it says. [exact] (D128 Part C)
54.7.1. But each is a word in two adjacent generators, a 3-strand
        braid inside B_4: reducible, not pseudo-Anosov. The round
        curve around its three punctures is invariant and essential
        (the fourth puncture and the boundary lie outside). [proved]
        (D128 Part C checks the words)
54.8. D106 states that Burau is faithful on B_4. [falls]
54.8.1. Faithfulness of Burau on B_4 is a known open problem; Burau
        is unfaithful for n >= 5 (Bigelow 1999 settled n = 5).
        Bigelow and Krammer proved the Lawrence-Krammer
        representation faithful. (literature, cited by the D128
        review; not checked here)
54.8.2. Reduced Burau at t = -1, which D106 used, is not faithful on
        B_4: Delta^2 maps to t^4 I = I. (Unreduced Burau(-1) of
        Delta^2 is not I.) [proved]
54.9. Do pseudo-Anosov braids compute no better than others (see
      31.2) when the classes are exact? [open]

55. Is a braid word an exact ReLU network?
55.1. Yes. Each generator's Dynnikov update (Thiffeault 2010, eq.
      8-15) is a straight-line program of affine forms and ReLUs,
      weights in {-1, 0, 1}, no biases: 2 ReLUs at the end
      generators, 5 in the middle, two ReLU stages per letter.
      [proved] (by construction: eq. 8-15 with f- = f - relu(f),
      D128 main.c build_letter; the transcription is anchored against
      ansa_agere on 4,000 random pairs, n = 3..8, coordinates up to
      1000 * 2^200)
55.2. Iterated on a loop, does the network settle into one linear
      region?
55.2.1. On every orbit of B_3 up to length 8 and B_4 up to length 6
        (every loop E_k), the active-ReLU pattern became periodic
        within 5 passes and stayed so to pass 60. [exact-search]
        (D128 Part D: 60 passes)
55.2.2. On each settled tail the network is one integer linear map
        M per period: v_{s+jq} = M^j v_s exactly. Given 55.2.1 and
        bias-free programs this follows; the check tests the
        forced-map code. [exact] (D128 Part D)
55.2.3. On B_4, period 2 occurs for 384 of the 4,014 positive-entropy
        braids: 328 of the 1,008 that only a cube root of unity
        certifies, 56 of the 3,006 certified at t = -1. [exact]
        (D128 Part D)
55.2.3.1. Why does period 2 concentrate there? [open]
55.2.3.1.1. Partly: period >= 2 forces the limit direction onto a
            wall. A bit that alternates along an orbit converging to
            v has its argument > 0 on one subsequence and <= 0 on the
            other, so 0 at v. [proved] (D129 Part C checks it per
            argument on B_4 up to length 6)
55.2.3.1.2. The converse fails: 568 positive-entropy braids of B_4 up
            to length 6 end on a wall with period 1 (476 certified at
            t = -1, 92 only at a cube root). [exact] (D129 Part C;
            assumes the largest real root of the tail dominates)
55.2.3.1.3. The period belongs to the loop, the tie to the braid:
            loops of one braid can settle with periods 1 and 2 at the
            same limit, and all growing loops agree on the tie.
            [exact] (D129 Part C)
55.2.3.1.4. Why the braids certified only at a cube root end on walls
            more often (420 of 1,008, against 532 of 3,006) is open.
            [open]
55.3. Can the exact dilatation be read from M on the right invariant
      cone (Hall and Yurttas)? [open] (ledger ...VFF141H)
55.4. Can the braid network compute Boolean functions (N2)? [open]
      (ledger ...4291V2)

56. How many linear regions does the network of w^m carve, and does
    the braid's entropy decide it? (logically follows 55)
56.1. On B_3 they grow linearly in m for every braid, never
      exponentially: over every class up to length 8, m = 1..12, the
      registered rule gives H_lin and not H_exp for all 2,108
      positive-entropy classes (and every periodic and zero-entropy
      one). [exact] (D129 Part A: exact sector counts,
      nervus_regiones_plani)
56.1.1. The linear pieces of w^m stay bounded: 8 for every
        positive-entropy class at every m >= 2; for periodic braids
        they cycle, 1 at m = k (w^k is the identity map). [exact]
        (D129 Part A)
56.1.2. Why bounded? A conjugate of the SL(2, Z) matrix by a fixed
        piecewise-linear change of coordinates would explain it.
        [interpretation] (D129 findings)
56.1.3. The regions added per pass do not grow with lambda: they top
        out at 8, the longest word, for lambda up to about 47. Over
        m = 6..12 the most-regions word adds 6 x its cyclically
        reduced length for 1,816 of 2,108 positive-entropy classes.
        [exact] (D129 Part A; the cyclic-length pattern was noticed
        after the run, not registered)
56.2. Regions belong to the word, pieces to the braid: geodesic words
      of Delta sigma_1 carve 12, 12 and 6 regions at m = 4 with the
      same pieces; 408 positive-entropy classes have words with
      different counts at m = 12. [exact] (D129 Part A)
56.3. Is linear region growth special to braids?
56.3.1. No. Networks of the same shape with each instruction's
        coefficients permuted also grow linearly (H_lin 2,089 of
        2,108, H_exp none), but their pieces grow (46 at m = 12
        against 8). [exact] (D129 Part D)
56.3.2. So what braids add is bounded functional complexity, not more
        regions. [interpretation]
56.4. Does the lattice lower bound (interior integer points of the
      shells max |x_i| = R) measure the growth?
56.4.1. No. At m = 12 and R <= 256 it finds 13.3 of the 87.6 mean
        regions of positive-entropy classes on B_3, and no such
        class's full count; on B_4 it stops growing after m = 4.
        [exact] (D129 Parts B and C)
56.4.2. New regions thin by about lambda per pass, toward the
        repelling direction. [interpretation]
56.5. Do the same laws hold on B_4? [open] (needs exact counts in
      d >= 3; ledger ...QK7A9)
```
