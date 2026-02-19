# Symmetry Decomposition of TL_n

**Track:** C — TL Matrix Compilation
**Status:** Proposed
**Smaragda ID:** 01KHP7P6B4Y1NSGP2DQ457H89J
**Dependencies:** Demo 29 (TL_n matrices), Demo 32 (dimension reduction), Demo 23 (function zoo/NPN classes)
**Code path:** `knotapel/demo_33_symmetry_decomposition/`

## Hypothesis

TL_n decomposes into irreducible blocks (analogous to Kron's symmetrical components). At delta=0, some blocks vanish (killed by loop=0). The Boolean computation is concentrated in a small subset of blocks. The unreachable NPN classes (parity-sensitive functions from Demo 23) correspond to the 'dead' blocks — giving a deeper algebraic explanation for the parity barrier.

## Predictions

1. Boolean computation at delta=0 is concentrated in a small subset of irreducible blocks.
2. The 'dead' blocks (killed by delta=0) correspond exactly to parity-sensitive functions.
3. The 11 reachable NPN classes from Demo 23 are achievable within the 'live' blocks; the 3 unreachable classes require 'dead' blocks.
4. For n=3 (NAND strand count), one small block carries all Boolean computation.
5. The pattern at n=2,3,4 predicts how many NPN classes of n-input Boolean functions are reachable at delta=0 for higher n.

## Approach

- **Part A:** Block diagonalization — for n=2,3,4,5, find basis change that block-diagonalizes TL_n generator matrices.
- **Part B:** Irreducible block catalog at delta=0 — catalog blocks: dimensions, generator action within each, which are zero.
- **Part C:** Per-block Boolean function analysis — for n=3, determine which block(s) carry Boolean computation.
- **Part D:** Connection to NPN classes — verify reachable classes correspond to live blocks, unreachable to dead blocks.
- **Part E:** Prediction for higher strand counts — based on n=2,3,4 pattern, predict reachable NPN classes for higher n.
