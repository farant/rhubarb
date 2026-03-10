---
id: HYP0008
title: "Ternary weights {-1, 0, +1} map naturally to affirm/deny/ignore, making BitNet b1.58 the ideal quantization for syllogistic reasoning"
status: proposed
source: "notes/01-analysis.md"
evidence_for: []
evidence_against: []
testable_by: []
related: []
---

## Claim

BitNet's ternary weight scheme {-1, 0, +1} has a natural semantic interpretation for syllogistic logic:
- **+1 = affirm:** the connection supports the proposition
- **-1 = deny:** the connection negates the proposition
- **0 = ignore:** the connection is irrelevant to this inference

This is "almost too perfect" for syllogistic logic. A syllogism's premises either affirm or deny a relationship between terms, and irrelevant terms are simply not involved. The three-valued weight scheme captures exactly these three cases.

Combined with TRM's recursive refinement (7M params, beats billion-parameter models on reasoning), this gives a concrete architecture: a tiny ternary model that reasons by iteratively applying the same compiled syllogistic transformation.

## Evidence For

- **BitNet b1.58 paper (Group A extraction):** Demonstrates that ternary weights achieve competitive performance with full-precision weights at dramatically reduced compute and memory cost.
- **DKC axis-alignment (D107):** Axis-aligned entries in Z[zeta_8] decompose as integer magnitude + 2-bit phase, which is structurally isomorphic to ternary on 4 axes. The ternary decomposition is already present in DKC's algebraic structure.
- **Group A crossover Connection 1:** BitNet ternary <-> Z/4Z axis-alignment is identified as "genuinely deep."
- **Logical naturalness:** Aristotelian syllogisms operate on exactly these three values — a premise affirms (A, I figures), denies (E, O figures), or is irrelevant to the conclusion. The mapping is not forced.
- **GPTQ-as-CVP:** Weight quantization to ternary is literally a Closest Vector Problem with known algorithms and provable error bounds. Compilation from exact weights to ternary is mathematically grounded.

## Evidence Against

- The semantic interpretation {affirm, deny, ignore} is suggestive but has not been verified computationally. A ternary weight might HAPPEN to be +1 without MEANING "affirm" in any recoverable sense.
- Ternary weights may be too coarse for the fluency layer, which needs gradient/degree rather than binary affirm/deny. Language is more nuanced than logic.
- The ternary scheme works well for simple syllogisms but it is unclear whether it handles more complex reasoning patterns (analogy, degrees of certainty, argument from authority) that require finer-grained relationships.
- Most BitNet results are on standard NLP tasks, not on syllogistic reasoning specifically.

## What Would Confirm It

- Compiling a set of syllogisms into ternary weight matrices and verifying that the affirm/deny/ignore interpretation is recoverable from the compiled weights.
- Showing that a ternary model outperforms (or matches) a full-precision model of the same architecture on syllogistic reasoning tasks, suggesting the quantization is well-suited rather than lossy.
- Demonstrating that the DKC axis-alignment to ternary decomposition preserves computational capability.

## What Would Refute It

- Finding that ternary quantization of compiled syllogistic weights introduces errors that break logical correctness — that the {-1, 0, +1} grid is too coarse for the exact tensor equations.
- Showing that the affirm/deny/ignore interpretation does not survive training — that trained ternary weights in a reasoning model don't correspond to logical operations.
- Demonstrating that a higher-precision weight scheme (e.g., 4-bit) significantly outperforms ternary on syllogistic tasks.

## Implications If True

- Integer-only inference becomes possible — no floating point needed. This aligns perfectly with the C89/exact-arithmetic philosophy.
- Memory and compute requirements drop dramatically (~10x over fp32), making 50-100M parameter models very practical on M2.
- The ternary structure provides built-in interpretability — every weight has a semantic meaning.
- Combined with TRM recursion, the architecture becomes: "iteratively apply the same ternary syllogistic transformation" — elegant and transparent.
