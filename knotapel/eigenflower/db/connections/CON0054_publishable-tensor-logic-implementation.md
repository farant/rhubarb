---
id: CON0054
title: "PUBLISHABLE: DKC as tensor logic implementation with exact cyclotomic arithmetic"
connects:
  - "Domingos tensor logic framework (no existing implementation)"
  - "DKC 20,000+ lines of C89 implementing exact cyclotomic tensor arithmetic"
type: cross_pollination
strength: strong
source: "notes/01-analysis.md, Publishable Ideas #3"
---

Domingos's tensor logic framework has no implementation. DKC's Z[zeta_8] arithmetic (16 integer muls per product, zero floats) IS a working exact tensor algebra engine.

The forward DKC computation -- Output[mask] = Activation(Weight[i] * Input[mask, i]) -- is Domingos's einsum over Boolean tensors with a step function, instantiated on an algebraic integer ring rather than R or {0,1}.

What DKC adds beyond the Domingos framework:
1. Exact arithmetic eliminates approximation entirely
2. The parity-lock theorem (D92) is a tensor logic impossibility result Domingos did not anticipate
3. The circuit complexity hierarchy (D93) quantifies the hardness landscape of tensor equations
4. BFS catalog closure IS forward chaining, reaching the deductive closure of the braid group representation
5. Temperature/sector-count mapping reveals non-monotonic behavior the framework cannot yet predict

The code assets in demo_29 (Cyc8 type, ~200 lines) could serve directly as substrate for a tensor logic interpreter over algebraic integers.
