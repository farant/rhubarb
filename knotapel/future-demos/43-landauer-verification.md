# Landauer Verification

**Track:** D — DKC Extensions
**Status:** Proposed
**Smaragda ID:** 01KHNASSPZPBN9B1NZF2G3JSHF
**Dependencies:** Demo 21 (reversible gates), Landauer Reversibility Boundary principle

## Hypothesis

Demo 21 builds globally reversible gates (Toffoli, Fredkin) from locally irreversible components (NAND erases 1 bit per evaluation). The Toffoli burns 8 bits of Landauer entropy per input despite being globally reversible. Can we build a Toffoli that is reversible ALL THE WAY DOWN — where every intermediate step is also reversible? This would require replacing NAND/NOT (irreversible primitives) with natively reversible primitives.

## Predictions

1. The Fredkin gate (conditional swap) IS a natively reversible primitive for bracket computation — swapping strand assignments doesn't erase information.
2. A 'reversible expression tree' can be built from Fredkin + CNOT (both reversible) with ancilla bits, achieving zero intermediate Landauer cost.
3. The crossing count for a fully-reversible Toffoli will be HIGHER than the expression-compiled version (58 crossings) because reversibility requires ancilla management overhead.
4. The ratio of reversible-to-irreversible crossing counts measures the 'Landauer tax' of our current approach.
5. At the topological level, fully reversible computation corresponds to braids where every sub-braid is also its own inverse.

## Approach

- **Part A:** Landauer accounting — Count exact bits erased in Demo 21's Toffoli and Fredkin implementations. Map which NAND/NOT evaluations are information-destroying.
- **Part B:** Reversible primitives — Can Fredkin (which IS reversible) serve as the only primitive? Fredkin is universal for reversible computation classically. Implement Fredkin-only Toffoli.
- **Part C:** Ancilla management — Reversible computation from irreversible gates requires ancilla bits (Bennett 1973). Implement the ancilla-based approach: compute forward, copy result, uncompute.
- **Part D:** Crossing count comparison — Compare total crossings: expression-compiled (58), Fredkin-only, and ancilla-based.
- **Part E:** Topological reversibility — Is there a braid-level characterization of 'fully reversible computation'? A braid that is its own inverse under some operation?
