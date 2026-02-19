# Parity Investigation

**Track:** D — DKC Extensions
**Status:** COMPLETE (2026-02-18)
**Dependencies:** Demo 45 (complex function zoo)
**Code path:** `knotapel/demo_47_parity_investigation/`

## Motivation

Demo 45 found that XNOR3 (parity) is the only function whose bracket decomposition is worse than random (RMS = 0.9963 vs baseline 0.8086). It also has topology frequency 0 — braids never produce it. The QM lens interpretation is that parity requires exact destructive interference that the bracket's sum-over-states can't produce. But is this a deep algebraic fact about partition functions and parity, or an artifact of the split-sigmoid activation?

## The Core Question

**Is parity's resistance to bracket decomposition architectural or algebraic?**

- If **architectural:** Changing the activation function (e.g., Aizenberg's MVN with kth roots of unity) will improve parity's decomposition. The split-sigmoid just isn't the right activation for parity.
- If **algebraic:** Parity resists decomposition regardless of activation. The bracket algebra fundamentally cannot express parity. This would be a structural theorem about the boundary between topological and non-topological computation.

## Approach

### Part A: MVN Activation
Replace split-sigmoid with Aizenberg's multi-valued neuron (MVN) activation:
- Activation: f(z) = z / |z| (continuous), or discrete kth-root version
- MVN can learn all 256 three-input Boolean functions including parity (Aizenberg 2011)
- Train MVN network on parity, decompose weights, compare RMS to split-sigmoid

### Part B: Increased Hidden Neurons
Test with 4, 5, 6 hidden neurons (split-sigmoid). Parity requires all neurons to cooperate — maybe 3 isn't enough for the weights to find a lattice-friendly solution.

### Part C: Architectural Comparison
For completeness, run MVN activation on all 13 NPN classes (not just parity). Does MVN improve decomposition across the board, or specifically for topology-resistant functions?

### Part D: Weight Structure Analysis
For parity networks that converge (under both activations): visualize the weight structure in the complex plane. What does a parity-computing weight vector look like vs a MAJ-computing one? Where do they sit relative to the Z[zeta_8] lattice?

## Predictions

- P1: MVN activation will improve parity's decomposition modestly (RMS drops from ~1.0 to ~0.7) but NOT to the level of topology-natural functions (RMS ~0.5-0.6). The resistance is partially architectural, partially algebraic.
- P2: Increasing hidden neurons from 3 to 6 with split-sigmoid will NOT significantly improve parity's decomposition (the problem isn't capacity, it's lattice alignment).
- P3: MVN will improve decomposition for topology-resistant functions (ranks 10-13) more than for topology-natural functions (ranks 1-6). The activation matters more when the function is already fighting the lattice.
- P4: Parity weight vectors under both activations will show a characteristic "spread" pattern — weights distributed across many lattice directions with no dominant cluster. This is the geometric signature of a non-topological function.

## Why This Matters

If parity fundamentally resists bracket decomposition regardless of architecture, that establishes a *computability boundary* for the bracket algebra. Some functions are partition-function-computable (threshold-like, topology-natural) and some aren't (parity-like, topology-unnatural). This boundary would be intrinsic to the algebra, not to any particular implementation.

This connects to topological quantum computation: the Fibonacci anyon model (which uses TL algebra representations) is known to be universal, but universality requires *approximation* — you can approximate any unitary to arbitrary precision, but some unitaries require exponentially more braiding than others. Parity might be in the "exponentially hard" category for the bracket.

## References

- Aizenberg, I. (2011). Complex-Valued Neural Networks with Multi-Valued Neurons. Springer.
- Freedman, Kitaev, Wang (2002). Simulation of topological field theories by quantum computers.
- Demo 45 findings.md — full ranking table and parity analysis
