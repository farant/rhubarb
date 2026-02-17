# Knotapel Demo 07: Braid Closure Bridge

## What This Tests

The bridge between two worlds that Demos 01-06 built separately:
- Demo 01: PD notation -> state-sum bracket (the O(2^n) oracle)
- Demos 02-06: braid word -> matrix pipeline bracket (O(n))

Can we convert braid words to PD notation via closure and prove both
representations compute the same invariant?

## Hypothesis

1. Braid closure produces valid PD notation with correct arc counts
2. The state-sum bracket from generated PD matches the braid bracket exactly
3. Seifert circles from PD equal the braid strand count (a theorem)
4. Link component count from PD matches braid permutation cycle structure

## Results

All 25 tests pass:

- **Part A: PD Construction** (5/5) -- braid_to_pd() generates correct
  arc counts for all test braids (trefoil, figure-eight, Hopf, 3-strand
  braid, unlink)

- **Part B: Bracket Match** (5/5) -- PD bracket == braid bracket for ALL
  braids, including chiral trefoil (the hardest test). The bridge is exact.

- **Part C: Seifert Circles** (7/7) -- Seifert circles = strand count for
  all braid closures. Also verified on Demo 01's hand-coded PD notation
  (trefoil=2, figure-eight=3, Hopf=2).

- **Part D: Link Components** (8/8) -- Correct component counts for all
  braids and Demo 01 PD. Discovered: sigma_1 sigma_2 sigma_1 on 3 strands
  is a 2-component link (not a trefoil), and sigma sigma^{-1} closes to a
  2-component unlink.

## What We Learned

1. **PD chirality is the hard bug.** The "obvious" counter-clockwise labeling
   from the incoming under-strand produces the mirror-image bracket. Fix:
   swap positions 1 and 3, giving [NE, SE, SW, NW] for positive crossings
   instead of [NE, NW, SW, SE]. This aligns A/B smoothings with braid
   cup/cross resolutions.

2. **Seifert circles != link components.** Both trace through PD diagrams,
   but Seifert smooths crossings (sign-dependent: positive -> B-smoothing,
   negative -> A-smoothing), while link components follow strands through
   (always (p+2)%4). Seifert counts geometric circles; link components
   count topological strands.

3. **The Seifert rule is universal.** The sign-dependent pairing (positive
   -> B-smoothing, negative -> A-smoothing) works for BOTH our braid-
   generated PD AND Demo 01's standard hand-coded PD. One algorithm for
   both conventions.

4. **Braid permutations predict link components.** sigma_1 sigma_2 sigma_1
   on 3 strands has permutation (0,2)(1) -> 2 components. The identity
   braid (sigma sigma^{-1}) closes to n separate loops. The closure
   topology follows directly from the braid's permutation cycle structure.

## Building

    gcc -std=c89 -Wall -Wextra -pedantic -o demo07 main.c
    ./demo07

## What's Next

- **Demo 08:** PD -> braid word via Vogel's algorithm (the "compiler"
  direction). Seifert circles become braid strands, crossings become
  generators. This completes the round-trip: any knot diagram can enter
  the matrix pipeline.

## Framework Connection

This demo proves the Compilation Pipeline's first link: braid words and
PD notation are interchangeable representations. The state-sum bracket
(O(2^n) oracle from Demo 01) and the matrix pipeline (O(n) from Demos
02-06) compute identical invariants. The bridge is exact, with no
floating point anywhere.
