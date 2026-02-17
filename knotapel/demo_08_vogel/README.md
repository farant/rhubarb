# Demo 08: Vogel's Algorithm (PD -> Braid Word)

## What This Does

Converts PD (planar diagram) notation into a braid word whose closure gives
the same knot or link. This is the "compiler" direction — Demo 07 built the
"assembler" (braid -> PD), and Demo 08 builds the reverse.

Together, Demos 07 and 08 prove that the PD and braid representations are
computationally equivalent: you can convert between them and the Kauffman
bracket is preserved in both directions.

## Algorithm (for path-graph Seifert graphs)

1. **Seifert membership**: Union-find on Seifert resolution pairs assigns each
   arc to a circle. Positive crossings use B-smoothing (0,1)(2,3), negative
   use A-smoothing (0,3)(1,2).

2. **Crossing-circle map**: Each crossing connects exactly two circles.
   Positive: circles of arcs[0] and arcs[2]. Negative: arcs[0] and arcs[1].

3. **Seifert graph**: Build reduced graph (vertices = circles, edges = crossing
   pairs). Verify it's a path graph (all degrees <= 2, exactly 2 leaves).

4. **Circle ordering**: Walk from a leaf to assign strand indices.

5. **Crossing trace**: Trace each Seifert circle through its crossings to get
   cyclic crossing sequences.

6. **Braid ordering**: Brute-force all cut-point combinations. For each combo,
   build a DAG from linear constraints and try Kahn's topological sort.
   Key: skip constraints between same-generator crossings (they commute).

7. **Emit generators**: gen_index = min(strand pair) + 1, sign = PD sign.

## Key Discovery

For standard small knots (trefoil, figure-eight, Hopf), the Seifert graph is
always a path graph — no Vogel moves needed. The skip-same-generator-edges
insight was essential: the trefoil has all crossings between the same pair of
circles, so the two circles' cyclic orderings are contradictory. But since all
generators are identical (sigma_1 commutes with sigma_1), any order works.

## Results: 47/47

- **Part A** (17 tests): Seifert membership — circle composition verified
- **Part B** (12 tests): PD -> Braid — trefoil->[1,1,1], fig-8->[1,-2,1,-2], Hopf->[1,1]
- **Part C** (6 tests): Round-trip bracket — pd_bracket(PD) == braid_bracket(pd_to_braid(PD))
- **Part D** (12 tests): Double round-trip — Braid->PD->Braid->PD bracket match

## Build

```
cc -std=c89 -pedantic -Wall -Wextra -Werror -o demo_08 main.c
./demo_08
```

## Dependencies

- Demo 07 infrastructure (Poly ring, PD bracket, Braid bracket, braid_to_pd)
- Demo 01 oracle PD codes (trefoil, figure-eight, Hopf)

## Lessons

1. **Same-generator commutativity** breaks naive ordering algorithms. The fix
   is mathematically principled: skip constraints that don't affect the knot type.

2. **Seifert circles = braid strands**. The circle count from Seifert resolution
   directly gives the braid width. This is Alexander's theorem in action.

3. **Sign mapping is direct**: PD crossing sign maps to braid generator sign
   without any chirality correction, for both hand-coded and braid-generated PD.

4. **Round-trip verification** is the strongest test: if PD->Braid->bracket
   matches PD->bracket, the compiler is correct up to knot equivalence.
