# Diakoptics (Parallel Braid Computation)

**Track:** C — TL Matrix Compilation
**Status:** Proposed
**Smaragda ID:** 01KHP7P69XDSMPMN7SAH3QR61T
**Dependencies:** Demo 29 (TL_n matrices), Demo 30 (long braid scaling)
**Code path:** `knotapel/demo_31_diakoptics/`

## Hypothesis

Kron's diakoptics (tearing) applies to braids: cut a long braid at horizontal slices into sections, compute each section's transfer matrix independently, compose. Sections can be computed in parallel. Hierarchical merge gives O(log(k/m)) depth. Matrix multiplication is associative, so tearing is exact — no approximation.

## Predictions

1. Near-linear parallel speedup for long braids.
2. Hierarchical (tree-structured) merge enables O(log k) depth computation.
3. Optimal section size exists (tradeoff: smaller sections = more parallelism but more merges).
4. Compositional verification: computing full adder in one shot vs sections gives identical results.
5. For 1000+ crossing braids, 4-8 parallel workers give measurable speedup.

## Approach

- **Part A:** Section decomposition — partition braid word into sections of m crossings, compute each section's transfer matrix.
- **Part B:** Parallel composition — compute odd/even sections on separate threads, merge. Measure speedup for 1000+ crossings with 2,4,8 workers.
- **Part C:** Hierarchical composition — tree-structured merge (compute all sections, merge pairs, merge pairs of pairs). O(log(k/m)) depth.
- **Part D:** Optimal section size — vary m for fixed total length and strand count, measure total time, find optimum.
- **Part E:** Compositional verification — full adder computed one-shot vs sectioned, verify identical results.
