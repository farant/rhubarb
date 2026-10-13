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
sibling. A claim that turns out wrong is never deleted: change its
grade and add a child that says why. Every future audit updates this
file in the same commit as its correction notes.

**Negative results are claims too.** A "No," answer carries a grade
like any other. `[proved]` or `[exact]` means the thing does not
exist. `[exact-search]` means the search found none, and the bounds are
stated.

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
| KR | `docs/knotapel.html`, "Notes from this reading" (2026-10-03), cited by note title |
| AA | the archived atlas, `archived-analysis/atlas/` |
| XL | `explorers-log.md` (the synthesis of 2026-10-13 is at its end) |

## Part I. Definitions

```text
1. What is a catalog?
1.1. The ordered list of distinct values produced by a braid
     enumeration or by closing generators under products: bracket
     values in Z[zeta_n], or unit quaternions taken mod sign.
     [definition]
1.2. Its order matters to any search that keeps only its first
     winners (see 26.2).

2. What does it mean for weights to compute a Boolean function?
2.1. Each input bit selects a signed weight; the neuron adds them, and
     an activation maps the sum to a cell. The weights compute f if no
     cell receives sums with both values of f. [definition]
2.2. A cell is a sector of the sum's angle (k sectors), crossed in the
     quaternion demos with the nearest axis of an axis table (Voronoi).
     [definition]
2.3. XOR2n with n weights: in the +-q encoding each weight contributes
     +q, -q or 0 for its two bits, and the target is the parity of
     all 2n bits. [definition]

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
     answer is a rounding accident. (see 40.1)

4. What is capacity?
4.1. The largest XOR level reached, and the counts of winning sets,
     for a given catalog, activation, tie rule and search. [definition]
4.1.1. So capacity is never a property of the catalog alone.
4.2. A search is exhaustive if it tests every weight set of a size,
     and greedy if it extends a bounded list of winners (D81, D82,
     D87: triples from the first 100 entries, 32 winners per level,
     at most 6 weights). [definition]
```

## Part II. The bracket and its catalogs

```text
5. Are the early demos' bracket values correct?
5.1. Yes, where checked. All 87,890 braids of D29's enumeration agree
     with a second exact route (symbolic delta in polynomium,
     evaluated in cyclotomia). [exact] (D117 Part A)
5.2. D29's product table equals Z[zeta_8] multiplication. [exact]
     (D117 Part A)
5.3. The other demos' hand-written rings (Cyc16, Cyc5, Cyc24, 3x3
     matrices, quaternion tables) were not compared with the house
     library. [unaudited] (D117)
5.3.1. UBSan was silent on the paths run, which rules out signed
       overflow there, not a wrong multiplication table.

6. Does D29's catalog hold 64 distinct bracket values?
6.1. No. 64 is the first 8,192 of 69,960 nonzero brackets
     (MAX_CATALOG). All 69,960 take 100 distinct values. [falls]
     (D117 Part A)

7. Are the early demos' named knots knots?
7.1. Not all. sigma1 sigma2 sigma1, labelled the trefoil in D2, D3,
     D6 and D10, closes to a Hopf link with one stabilizing crossing.
     [checked] (KR "Demo 10's knot table is mostly links")
7.2. D10's six-braid table is mostly links: only the figure-eight is
     a knot. [checked] (KR, same note)
7.3. The three-strand "T(2,n)" entries of D11-D17 close to
     two-component links. [checked] (KR, same notes)
7.3.1. Results that compare entries with each other stand; results
       stated about torus knots are about these links.
7.4. D1's printed f-polynomial is mis-normalized: the writhe sign does
     not match the bracket's chirality. [checked] (KR "Demo 1's
     printed f-polynomial is mis-normalized")

8. Is the bracket at delta = 0 always axial (n times a power of A)?
8.1. Recorded as verified on about 131,000 braids, with a three-step
     proof sketch (D35, XL). [unaudited] (AA theorems.md)
```

## Part III. Boolean functions from bracket weights

```text
9. Do bracket values compute Boolean functions without training?
9.1. Recorded yes: D29's catalog contains weights that compute XOR
     (the forward DKC result). [unaudited]
9.1.1. The bracket values themselves are exact (see 5.1); the search
       over them ran in floats and was not re-run.

10. Why is three-input parity unreachable with the split-sigmoid
    neuron (D48)?
10.1. Because of the neuron, not the bracket. Its output is
      [Re z > 0] XOR [Im z > 0], the XOR of two threshold functions,
      and no pair of the 104 three-input threshold functions XORs to
      parity. [checked] (KR "Demo 48's parity wall is the neuron's")
10.1.1. So parity is unreachable for every weight set, before any
        bracket is computed.

11. At what sector count does three-input parity first appear (D50)?
11.1. At k = 6 for the bracket catalog. [unaudited] (D50)
11.2. The threshold belongs to the activation. Null catalogs
      (shuffled pairings, random magnitudes on the same directions,
      random Z[zeta_8] elements, random phases) all first reach parity
      at k = 6 and none below. [checked] (KR "Demo 50 against null
      catalogs")
11.3. The bracket catalog behaves like a typical eight-direction star
      of integers. [checked] (KR, same note)

12. How many of the 13 NPN classes of three-input functions do
    half-plane activations reach?
12.1. 11 of 13; 0x06 and 0x1B are unreachable. Recorded as proved,
      computationally at N = 360 and then analytically (D61, D62).
      [unaudited]
12.1.1. The proofs have not been re-checked.

13. Does the two-sector activation reach 12 of 13 classes (D56)?
13.1. No. Exactly 11 of 13, including parity. [falls] (D117 Part B)
13.1.1. Class 0x06's 3 solutions came from overflowed long products
        (92 of 729 weight triples overflow); exactly it has 0.
13.2. D56's class totals and parity counts survive exactly under all
      five activations; its per-class solution counts move under
      every one. [exact] (D117 Part B)

14. Are the multi-input parity ceilings sound (D63-D65)?
14.1. The sets survive, but D63 and D64's ordered counts are a float
      zero-test artifact: 3,020 ordered XOR5 solutions in 27 sets,
      where permutation invariance requires 27 x 120 = 3,240. D65,
      summing exactly, gets 3,240. [checked] (KR "Order of summation
      changes the counts")
14.2. The ceilings themselves (Z[zeta_8] parity ceiling n = 5; XOR4 at
      k = 8; XOR5 at k = 15). [unaudited] (AA theorems.md)

15. Does the "Barrington-Radical Principle" (D102) hold?
15.1. No, as stated. Programs over abelian groups compute parity (over
      Z/2); what solvable groups cannot do in bounded width is AND or
      majority, so the implication is reversed. [checked] (KR "Abelian
      groups compute parity")

16. Does the parity-lock theorem (D92, +-q encoding) hold?
16.1. Recorded as proved, with a written proof in
      proofs/parity-lock-theorem.md. [unaudited]
```

## Part IV. The zeta_8 catalog: the binary octahedral group 2O

```text
17. What group do D66's braid generators produce?
17.1. The binary octahedral group 2O: exactly 48 elements, element
      orders 1^1 2^1 3^8 4^18 6^8 8^12, 24 classes mod sign. [exact]
      (D118 Part A)
17.2. "The 24-cell is the braid image" falls. The 24-cell is the
      subgroup 2T; 2O is the 24-cell together with its dual. [falls]
      (D118; KR "The group is the binary octahedral group")
17.3. The catalog has exactly 13 rotation axes: 3 four-fold, 4
      three-fold, 6 two-fold. [exact] (D118 Part B)
17.4. The 9 null entries (Re q = 0) are the 9 half-turns: 3 about
      face axes, 6 about edge axes. [checked] (KR "The null entries
      are the half-turns")
17.4.1. So "null" and "non-null" are unions of conjugacy classes, not
        the radical and its quotient.
17.5. The finiteness is classical: Jones's representations have
      finite image at these roots of unity. [checked] (KR)

18. Which antipodal triples compute XOR6 with the 24-cell Voronoi
    activation (D66)?
18.1. Under some tie resolution: exactly the 96 mutually orthogonal
      triples. [exact] (D118)
18.2. Under every tie resolution: exactly the 24 triples inside one
      coset of Q8 = {+-1, +-i, +-j, +-k}, which form one orbit under
      the group's symmetries. [exact] (D118)
18.3. D66's 35 is the lowest-index tie rule, which 92 of the 96
      symmetry maps break, so it is not a property of the weights.
      [falls] (D118)
18.4. "Beats the 6x6 geographic grid (3 winners)": the geographic
      cells were not recounted. [unaudited] (D118)

19. Does 2I compute better than zeta_8 at the same catalog size (D94)?
19.1. Yes for XOR at N = 3-6, under every tie rule. At N = 6 the
      random-24 mean of 2I against zeta_8 is +82.7% (rule), +94.2%
      (robust), +37.2% (possible). [exact] (D119)
19.2. "2I wins at N = 7 (3.4x)" falls: D94 compared a scaled mean with
      a raw count. [falls] (D119 Part F)
19.2.1. On one scale, N = 7 XOR is about even (0.97-1.12x), and
        zeta_8 is ahead for AND at N = 7 and 8.
19.3. "Solvability is the bottleneck" was refuted earlier with a Haar
      control. [checked] (D119, citing KR notes 47-48)
19.4. D94's float counts are tie artifacts in both directions: the
      exact rule sometimes exceeds them. [exact] (D119)

20. What do D95-D97 show about the pieces of zeta_8?
20.1. Cell B (the six +-45 degree rotations about three orthogonal
      axes) is 100% at every N under every resolution. [exact] (D121)
20.2. COMM survives N = 7 (8 sets) and NON-COMM dies (0), under every
      resolution. [exact] (D121)
20.2.1. COMM against NON-COMM is the 24-cell (2T) against its dual:
        two geometries. [checked] (KR "Demo 95's splits are
        geometric")
20.3. B+C wins at every N, robustly: 96.7, 92.8, 71.7, 29.0%. [exact]
      (D121)
20.4. A+B+C(+E) beats the whole catalog at every N under rule, robust
      and possible. [exact] (D121)
20.5. "0 + 0 = 26%" (all-null, N = 4) falls. Every null set is tied;
      robustly 15 of 126 sets (11.9%). [falls] (D121)
20.6. "The optimal catalog is A+B+C" falls, in D96's own floats as
      well (B+C is higher at every N). [falls] (D121)
20.7. Cell C at N = 6 "57%" is decided by ties: robust 4 of 28 (14%),
      possible 28. [falls] (D121)
20.8. 2I's first 12 beat zeta_8-COMM at N = 3-5, robustly (1.56,
      1.63, 1.98x). [exact] (D121)

21. Why does cell B compute parity?
21.1. Weights with a common real part c give Re S = c(2k - N), so the
      sum's real part records the Hamming weight, and parity is a
      function of it. [checked] (KR "Why cell B computes parity")
21.1.1. Half-turns have c = 0, so their sums carry no Hamming weight:
        hence cells A and D compute nothing alone.
21.2. By half-angle theta, cell B is 100% robust at every N on exactly
      two windows, (21.552, 33.355) and (38.794, 79.453) degrees, with
      closed-form ends arctan(c tan(j pi/24)). [exact] (D122)
21.3. On [33.355, 38.794] degrees, N = 5 fails robustly; at integer
      degrees the band is 34-38. [exact] (D122, D123)
21.3.1. D97 read this band as an "isolated resonance at 35 degrees".
        [falls] (D122)
21.4. 45 degrees is not special: it lies on a breakpoint (u = 1), not
      at the plateau's midpoint. [exact] (D122)
21.5. Total collapse at 90 degrees holds under each fixed tie rule
      and robustly; "possible" is vacuous there. [exact] (D122)
21.6. D97's float sweep equals the exact rule at all 21 of its angles:
      the error was the reading, not the arithmetic. [exact] (D122,
      D123)
21.7. At integer degrees, robust 100% at every N holds exactly at
      22-33 and 39-79. [exact] (D123)

22. Do the 9 null entries of zeta_8 carry capacity (D84)?
22.1. No. The 15 non-null entries reach XOR8, as all 24 do, under the
      exact rule and robustly. [exact] (D124)
22.2. The 9 null entries alone reach no XOR level under the rule or
      robustly. [exact] (D124)
22.2.1. Under "possible" they reach XOR14, because every null sum is a
        two-way tie: vacuity, not capability. (see 3.3.1)
22.3. Removing the nulls removes winners, not the maximum: XOR8 sets
      402 -> 106 (rule), 218 -> 36 (robust). [exact] (D124 Part D)
22.4. D84 recorded "non-null XOR6, null-only XOR6, worse than a
      random 15". [falls] (D124)
22.4.1. D84's own code prints XOR8 and 0 on every build tried; where
        the recorded figures came from is unknown. [not-reproduced]
        (D124)
22.5. One cell decision in eight in D84's search is an exact tie
      (2,686,124 of 20,965,173), and none is a real float error.
      [exact] (D124 Part C)

23. Is there a zeta_8 / zeta_12 "regime transition" in null
    indispensability (D87)?
23.1. No. Its zeta_8 half falls with 22.4; its zeta_12 half holds only
      as far as the search reaches (see 29.1). [falls] (D124, D125)
```

## Part V. The zeta_12 catalog

```text
24. Do the zeta_12 catalogs saturate at 512 directions (D79-D84)?
24.1. No. 512 was MAX_DIR. The 4,096-entry catalog has 2,043 axes,
      and D80's groups have 1,708 to 2,431. [falls] (D117 Parts D, F)
24.2. D80's finiteness verdicts are unchanged without the cap.
      [intervention] (D117 Part F)
24.3. "After round 7 directions saturate; XOR10 -> XOR12 needs finer
      angles, not more directions" falls: depth 7 adds 468 new
      directions, and depth 8 at least 1,068. [falls] (D117 Part F)

25. Does zeta_12 reach XOR12 (D79)?
25.1. Yes. Exactly, rounds 6-8 of D82's catalog reach XOR12 under the
      rule and robustly. [exact-search] (D125)
25.2. D79's XOR10 = 124 at k = 8 is a cap: 214 at k = 6 with all 112
      axes of its 256-entry catalog, both extensions of a capped
      parent set. [falls] (D117 Part D)

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
        exist outside what it explores. (see 48)
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

31. Do D100's and D106's sign-hash counts describe the matrices?
31.1. They describe the overflowing hash that ran. A wrapping hash
      reproduces them byte for byte; the intended hash moves most
      counts (D100 deep XOR8 1,169 -> 3,576). [intervention] (D117
      Part E)
31.2. D106's verdict, that pseudo-Anosov braids do not compute better,
      is unchanged under the intended hash. [intervention] (D117
      Part E)

32. Does framing change zeta_12 capacity (D83)?
32.1. Its bracket against Jones-normalized comparison (43 against 31
      angles, max XOR 12 against 10) is unchanged without the cap; the
      mixed-writhe XOR12 winners go from 4/32 to 0/32. [intervention]
      (D117 Part F)
32.2. Its exact recount is not done. [unaudited]

33. Do D89-D93's results hold?
33.1. They build on D82's depth law and "algebraic coherence" (see 26,
      27) and have not been audited. [unaudited]
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

36. What do honest construction-word spectra contain?
36.1. Small, orderly spectra: the knot, its mirror, simpler knots,
      and mostly unknots. [exact] (D115, D116)
36.2. 7_2's 15-vertex polygon is the exception: 47 distinct Alexander
      polynomials, determinants up to 85, and 50 distinct prime knots
      of the 13-crossing table, many of them non-alternating. [exact]
      (D115, D116 Revisions II)
36.2.1. The spectrum depends on the polygon: over all 15
        fewest-vertex 6_3 polygons the core is {6_3, 3_1, 3_1*,
        unknot}, union 18 classes. [exact] (D115)
36.3. All 9,768 alternatives are classified: 5,084 proven unknots,
      138 unknots by citation (Jones detects the unknot up to 24
      crossings), and 4,546 named by the table. [exact] (D116
      Revisions II)

37. Is D110's "complement chirality theorem" a fact about knots?
37.1. No. It is true by construction: vertices 0, 1, 2 lie on the
      mirror plane, so the complemented word gives the mirror image.
      [proved] (D115)

38. Is D110's majority classifier right?
38.1. Yes. Its 4-check classifier is perfect on the 512 alternatives:
      64 figure-eights, 384 unknots, 32 + 32 trefoils. [exact] (D114)
38.2. Its "left-trefoil if b2 = 1" is right-handed by Jones: the names
      are swapped. [falls] (D114)

39. Do D111's statistics (det spectra, influence, complement closure,
    Hamming profiles) hold?
39.1. No. They were computed on singular polygons with overflowing
      arithmetic; the det spectra recomputed exactly collapse
      (6_3: 22 dets -> 5). [falls] (D114)
39.1.1. The derived statistics were not recomputed.
```

## Part VII. Temperley–Lieb algebra

```text
40. Is the structure of TL_n at delta = 0 (radical, sandwich theorem,
    cellular decomposition) sound?
40.1. Recorded as proved or exhaustively verified for n = 2-8 (D35-D39,
      D51-D52), often citing known theory (Graham-Lehrer;
      Goodman-de la Harpe-Jones). [unaudited] (AA theorems.md)
40.2. D51 and D52's tables follow from textbook cellular-algebra
      formulas. [checked] (KR "Demos 51 and 52's tables follow from
      textbook theory")

41. Is the indecomposability parameter b = -5/8 for dense polymers
    (D85)?
41.1. Not established. [checked] (KR "Demo 85's b depends on the
      gauge")
41.1.1. The delta-form leaves b gauge-dependent: b = -2.5 - 2c^2
        along one kernel direction.
41.1.2. The division by 4 is asserted, not derived.
41.1.3. -5/8 is percolation's value; delta = 0 is dense polymers, and
        b is a limit over growing lattices, not one n = 4 value.

42. Do D101, D103 and D105 test simplicity correctly?
42.1. No. They take the radical as the vectors killed by every e_i,
      which detects only copies of the one-dimensional
      representation. [checked] (KR "'Simple' was tested the wrong
      way")
```

## Part VIII. Method findings

```text
43. Are exact ties rare in these catalogs?
43.1. No, they are the norm. Every null sum lies on a sector boundary
      at every even k. In D119, all 397,565,109 near-boundary sector
      decisions and all 93,638,928 near-tie axis decisions were exact
      ties. In D84's search one decision in eight is a tie. [exact]
      (D119, D120, D124)
43.2. So float verdicts are arbitrary exactly where the structure
      lives, and every capacity count should report rule, robust and
      possible. [interpretation]

44. What was the dominant fault of the old demos?
44.1. Silent caps (D29, D79-D84, D90), then long overflow (D56,
      D111-D112), then float ties (D66, D94-D96, D84). (see 6.1, 24.1,
      30.1.1, 13.1.1, 35.1.1, 18.3, 43.1)
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
      symmetry (D117 Part E) and the group's symmetries (see 18.3)
      would each have flagged a fault at no cost.
```

## Part IX. Open questions

```text
47. Is DKC capacity a property of the knots, or of the point set?
47.1. Of the point set and the activation, as far as audited.
      [interpretation] (XL synthesis 2026-10-13)
47.1.1. Knot -> group element is classical: at zeta_8 the braid image
        is the finite group 2O. (see 17.1, 17.5)
47.1.2. Group element -> Boolean function is geometry: directions,
        angles, and where exact ties fall. (see 18.2, 21.1, 43.1)
47.1.3. Null catalogs reach parity at the same k as the bracket
        catalog (see 11.2), and random 564-entry samples reach XOR12
        as often as deep ones (see 27.2).
47.obj.1. Objection: the catalogs are not arbitrary point sets. The
          cleanest results (18.2: Q8 cosets) are group-theoretic, so
          the algebra matters.
47.obj.1.1. Reply: the algebra matters through the geometry it
            produces. Q8's cosets are orthonormal frames, and frames
            are what the activation reads. Whether the braid origin
            adds anything beyond the group has not been tested
            (see 48).
47.obj.2. Objection: no audit has tested a knot-specific prediction,
          so 47.1 rests on absence of evidence.
47.obj.2.1. Reply: granted. That is why 47.1 is graded an
            interpretation; 48 is the test that would decide it.

48. What experiment would decide 47?
48.1. A matched null model: random point sets matched to a catalog on
      distinct directions, distinct angles and tie structure, run
      through the same activation and search. If capacity matches,
      DKC reduces to geometry; if not, something is left to explain.
      [open]

49. Do robust XOR sets in 2T and 2I correspond to cosets of a normal
    subgroup, as the Q8 cosets do in 2O (see 18.2)? [open]

50. Do the TL radical results survive an exact audit at a second size
    (see 40)? [open]

51. Is there an XOR14 set at zeta_12 depths 7-8 outside D82's greedy
    search (see 26.3.1)? [open]
```
