# Path Integral / Ising Model Connection

**Track:** D — DKC Extensions
**Status:** Proposed
**Smaragda ID:** 01KHNAT70PCW55YW0RGSETP49D
**Dependencies:** Demo 20 (delta=0 as Ising critical point), Demo 18 (state-sum bracket)

## Hypothesis

Our bracket evaluation IS a path integral — sum over all 2^n smoothing configurations, weighted by A^(a-b) * delta^(loops-1). Each smoothing is a 'path' in the Feynman sense. The total is a partition function. At delta=0 (our operating point), this is the Ising model partition function at criticality. Can we make this connection explicit and verify it computationally?

## Predictions

1. The bracket for an n-crossing braid, evaluated at the 8th root, equals the Ising partition function on the corresponding Tait graph at the critical temperature.
2. The transfer matrix method (standard in statistical mechanics) will reproduce our state-sum bracket values exactly, providing independent verification.
3. Correlation functions in the Ising model correspond to specific sub-braid bracket evaluations.
4. The critical exponents of the Ising model (nu=1, eta=1/4, etc.) are visible in how bracket values scale with braid length at delta=0.
5. Moving off delta=0 corresponds to moving off the Ising critical temperature, connecting Demo 20's error landscape to the Ising phase diagram.

## Approach

- **Part A:** Tait graph — Build the Tait graph (checkerboard graph) for a braid diagram. Each crossing becomes an edge, each region becomes a vertex.
- **Part B:** Ising partition function — Compute Z = sum exp(-beta * H) for the Ising model on the Tait graph. Find the beta (inverse temperature) that corresponds to our A = e^{i*5*pi/4}.
- **Part C:** Equivalence verification — Compare state-sum bracket with Ising Z for a family of braids. They should match up to a known normalization.
- **Part D:** Transfer matrix — Implement transfer matrix computation for the same braids. Verify it matches both the state sum and the Ising Z.
- **Part E:** Critical scaling — Evaluate bracket for braids of increasing length at delta=0. Look for power-law scaling consistent with Ising critical exponents.
