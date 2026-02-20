# Complete Linearity Classification

**Track:** D — DKC Extensions
**Status:** Proposed
**Dependencies:** Demos 55-59 (DKC search results across multiple ℓ values)

## Hypothesis

The coding theory criterion from the five-lens analysis (Demo 56) predicts that non-linear TRUE sets are incompatible with multiplicative encoding + half-plane activations. This demo produces the complete classification and cross-references with empirical DKC data.

## Goals

1. For all 13 NPN classes at 3 inputs: classify canonical TRUE sets as linear, affine (coset of linear), or non-linear over GF(2).
2. For the 222 NPN classes at 4 inputs: same classification (if feasible).
3. Cross-reference with DKC achievability at each (ℓ, encoding, activation) combination from Demos 55-59.
4. Produce the "compilation table": for each Boolean function, the optimal (ℓ, encoding, activation) triple.

## Predictions

1. Affine NPN classes (BUF, XOR2, PARITY): always achievable with multiplicative + Re>0.
2. Non-linear classes (0x06, 0x1B): unreachable with multiplicative + half-plane, but achievable with multiplicative + sector OR additive + Re>0.
3. The linearity criterion perfectly separates Re>0-achievable from Re>0-unreachable under multiplicative encoding.
4. At 4 inputs, the classification predicts which of the 222 classes will be hard.

## Approach

- **Part A:** Enumerate all 13 NPN classes. For each canonical TT, extract TRUE set, test linearity (closure under XOR + contains 000) and affinity (coset of linear code).
- **Part B:** Build achievability matrix from Demos 55-59 data: rows = NPN classes, columns = (ℓ, encoding, activation) triples.
- **Part C:** Test whether linearity perfectly predicts the multiplicative + Re>0 column.
- **Part D:** Extend to 4-input (222 NPN classes, 16-element Boolean cube). Linearity over GF(2)⁴.
- **Part E:** Compilation table: for each class, the simplest (ℓ, encoding, activation) that achieves it.

## Notes

Depends on results from Demos 57-59 to populate the achievability matrix. Can start Part A-C immediately with existing Demo 55-56 data.
