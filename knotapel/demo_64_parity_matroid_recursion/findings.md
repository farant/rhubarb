# Demo 64: Parity Matroid Recursion — Findings

## 1. The Matroid Theorem

The DKC parity hierarchy is **fully deletion-contraction closed**:

- **Deletion**: Removing any weight from an XOR_n solution always yields an XOR_{n-1} solution. 100% at every level (n=5→4→3→2).
- **Contraction**: Fixing any input to 1 in an XOR_n solution always yields an XNOR_{n-1} solution. 100% at every level.

This confirms that the parity hierarchy {XOR2, XOR3, XOR4, XOR5} forms a legitimate matroid minor chain in the DKC framework. Every solution at level n contains every solution at level n-1 within it, accessible by single-weight deletion.

**Implementation note**: Contraction requires exact Z[ζ_8] integer arithmetic (Cyc8 representation). Floating-point accumulation produces spurious failures near sector boundaries — 420 failures at the XOR5→XNOR4 level from association-order artifacts, eliminated entirely by exact arithmetic.

## 2. The Funnel Theorem

While deletion downward is 100%, coverage upward collapses dramatically:

| Transition | Coverage | Orphans |
|-----------|----------|---------|
| XOR3 → XOR2 | 99.3% (1459/1469) | 10 |
| XOR4 → XOR3 | 39.2% (1503/3834) | 2331 |
| XOR5 → XOR4 | 19.2% (129/673) | 544 |
| XOR6 → XOR5 | 0% (0/27) | 27 |

The hierarchy is a **funnel, not a cylinder**. Moving toward the ceiling (n=5), the fraction of solutions that can extend upward shrinks rapidly. The n=6 wall doesn't appear suddenly — its shadow is visible all the way down, with 80% of XOR4 weight sets already being "dead ends" that cannot participate in XOR5.

**Distinct unordered weight sets**: XOR5 = 27, XOR4 = 673, XOR3 = 3834, XOR2 = 1469. The hierarchy narrows going up even more dramatically than ordered solution counts suggest. XOR5's 3,020 ordered solutions reduce to just 27 distinct weight compositions — roughly 112 ordered tuples per set (close to 5! = 120, confirming near-complete permutation symmetry).

## 3. Parent-Child Multiplicity

The matroid's branching structure mirrors the funnel:

| Level | Min parents | Max parents | Mean | Character |
|-------|-----------|-----------|------|-----------|
| XOR5→XOR4 | 1 | 2 | 1.05 | Near-tree |
| XOR4→XOR3 | 1 | 8 | 1.79 | Sparse graph |
| XOR3→XOR2 | 1 | 29 | 7.88 | Dense mesh |

Near the wall, the hierarchy is almost a **tree** — each covered XOR4 child has at most 2 XOR5 parents. Away from the wall, it fans into a dense mesh. The 6 XOR4 children with 2 parents correspond to exactly 6 collisions in the deletion map (27 sets × 5 deletions = 135 instances → 129 distinct children + 6 collisions = 135 ✓).

## 4. Vocabulary Stratification

Of the 83 class-1 catalog values at k=15:

- **44 values** appear in XOR5 solutions (the "extensible vocabulary")
- **44 values** appear in extensible XOR4 solutions — exactly the same 44
- **75 values** appear in all XOR4 solutions (extensible + orphan)
- **31 values** are "poison" — they appear ONLY in orphan XOR4 solutions

The vocabulary is strictly nested: {XOR5 values} = {extensible XOR4 values} ⊂ {all XOR4 values}. No values are exclusive to extensible sets that don't also appear in XOR5. The 31 poison values span octants {1,2,3,5,6,7} but notably NOT octant 4.

**Interpretation**: The wall is partly about vocabulary restriction. An XOR4 solution using even one of the 31 poison values can never be extended to XOR5. The 44-value "good" vocabulary is a hard constraint.

## 5. The Wall Under a Microscope

### Hamming Distance from XOR6

2,241 extension attempts (27 XOR5 sets × 83 class-1 values):

- **Minimum Hamming distance: 4** (38 attempts)
- No attempts at d=0, 1, 2, or 3
- Distribution peaks around d=15-16 (near-random territory)
- 195 near-misses with d ≤ 8

The wall is moderately thick: the closest failed extension gets 60/64 = 93.75% of outputs correct but cannot close the gap below 4 errors.

### Error Weight Distribution

Errors by input Hamming weight across all near-misses:
- Weight 0: **0 errors** (all-zero input always correct)
- Weight 1: **0 errors** (single-input cases always correct)
- Weight 2: 237 errors (pair interactions)
- Weight 3: 370 errors
- Weight 4: 446 errors (peak)
- Weight 5: 172 errors
- Weight 6: 23 errors

**Critical observation**: Zero errors at weight 0 and weight 1. Every individual weight is fine. The wall manifests exclusively at the **interaction level** — when two or more weights must cooperate.

### The d=4 Structure

All 38 closest misses: exactly **one weight-2 error** plus 3 higher-weight errors. The single broken pair + its downstream ripple = 4 total errors.

All broken pairs involve the extension weight (position 5). The original 5 weights' C(5,2)=10 pairwise sums are always correct (they're a valid XOR5). Only new pairs with the 6th weight can break.

### Higher-Order Wall (d≥7)

Surprising finding at higher Hamming distances:
- d=7: 2 cases with **0 weight-2 errors**
- d=8: 16 cases with **0 weight-2 errors**

These have ALL pairwise sums correct but fail at weight 3+. The wall has a **second layer**: higher-order interaction constraints beyond the pairwise level. Solving all C(6,2)=15 pair equations is necessary but not sufficient — triple-and-higher interaction sums must also land in correct sectors.

## 6. The Self-Doubling Impossibility Theorem

**Theorem**: Self-extensions (adding a 6th weight equal to one of the existing 5) always fail.

**Proof**: If w[5] = w[i], the pair sum w[i] + w[5] = 2·w[i]. Since multiplication by 2 preserves angle (it scales magnitude but not direction), 2·w[i] has the same angle as w[i]. Since w[i] is a class-1 catalog value (odd sector), 2·w[i] is also in an odd sector = class 1. But the pair output for even-parity input (weight 2) should be class 0. Contradiction. ∎

This mechanism accounts for **34/38 = 89.5%** of all closest misses. The remaining 4 are cross-value extensions where the pair sum happens to land in an odd sector despite the two values being different.

**Geometric interpretation**: Class-1 values point at angles in odd sectors of the k=15 partition. Doubling a vector preserves its direction. Odd + odd parity cannot produce even parity through angle-preserving operations. The wall is fundamentally about the **discreteness of the sector structure** — angles are either in odd sectors or even sectors, and scalar multiplication cannot change which.

## 7. Quantum Mechanical Connections

The DKC framework operates with quantum algebraic resources:

- **ζ_8 = e^{iπ/4}** is the phase of the **T-gate**, the key non-Clifford gate for universal quantum computation
- **Z[ζ_8]** is literally the algebraic ring generated by T-gate operations
- The catalog values are **quantum amplitudes** (Kauffman bracket evaluations)

The n=5 parity ceiling is therefore a statement about the **computational reach of T-gate algebra** for exact Boolean synthesis.

**Entanglement monogamy parallel**: The CKW inequality bounds how entanglement can be shared among multiple qubits. Our finding — 10 pairwise sums can be simultaneously compatible but 15 cannot — is a "monogamy of angular compatibility" in the cyclotomic lattice.

**Contextuality parallel**: The Kochen-Specker theorem says you can't consistently assign values to all quantum observables. Our wall says you can't consistently assign class-0 sectors to all C(6,2) pair sums. The 5 additional constraints at n=6 create an inconsistency — a contextuality-like obstruction.

**Higher-order entanglement**: The d≥7 cases with zero broken pairs but weight-3+ errors are analogous to GHZ-type entanglement — where pairwise correlations are consistent but three-body correlations reveal the true quantum structure.

**Self-doubling as Clifford hierarchy**: Doubling a T-gate phase (π/4) gives an S-gate phase (π/2). The self-doubling impossibility maps to the principle that descending the Clifford hierarchy loses computational power. XOR6 may require resources from a higher level of the hierarchy than Z[ζ_8] provides.

## 8. Open Questions

1. **Clifford hierarchy staircase**: Does XOR6 become achievable at Z[ζ_16] (the next cyclotomic ring, corresponding to fourth-level Clifford hierarchy)? If so, the parity function literally climbs the Clifford hierarchy one rung per additional input.

2. **Higher-order wall characterization**: The d≥7 cases with 0 broken pairs suggest the wall has multiple layers. Can we characterize the triple-interaction constraints explicitly? What is the analog of the self-doubling theorem for weight-3 sums?

3. **Antipodal structure**: The degenerate pairs (w + (-w) = 0) are always "safe." How much of the XOR5 vocabulary's structure depends on antipodal pairing? Are the 27 XOR5 sets built primarily from antipodal pairs?

4. **Funnel rate**: The coverage drops as 99.3% → 39.2% → 19.2% → 0%. Is there a formula (exponential decay? algebraic?) that predicts the coverage fraction as a function of n?

5. **Poison value geometry**: The 31 orphan-only values avoid octant 4 entirely. Is there a geometric explanation for this octant asymmetry?

6. **Connection to Reiner's cyclotomic matroids**: Are the 44 "good" values and 31 "poison" values related to the matroid structure of μ_8 (the cyclotomic matroid at 8th roots)?

## 9. Summary

Demo 64 reveals the DKC parity hierarchy as a **funnel-shaped matroid** with perfect downward closure and collapsing upward extensibility. The wall at n=6 has a precise geometric mechanism: the Z[ζ_8] lattice at k=15 cannot support 6 weights with all 15 pairwise sums in compatible sectors. The dominant obstruction is the self-doubling impossibility (angle-preserving scalar multiplication), with a secondary layer of higher-order interaction constraints.

The findings connect directly to quantum computation through the T-gate correspondence (ζ_8 = T-gate phase), suggesting the parity ceiling reflects the computational depth of a specific level of the Clifford hierarchy. The matroid structure — perfect deletion-contraction closure with a strict vocabulary hierarchy — provides a framework for analyzing DKC capabilities that goes beyond exhaustive search.
