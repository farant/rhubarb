---
id: LIM0013
title: "DKC operates on 3-5 input Boolean functions — scaling gap to propositional complexity"
type: discovered
source: "notes/01-analysis.md"
impacts: []
related: [LIM0014, LIM0015, LIM0024]
---

DKC's 109 demos prove that algebraic structure encodes computation — but the computation being encoded is Boolean functions with 3-5 inputs. Theological reasoning operates on propositions with complex internal structure. The gap between "compute XOR of 3 bits" and "derive a theological conclusion from premises expressed in Latin prose" is enormous.

From 01-analysis.md: "DKC operates on Boolean functions with 3-5 inputs. Theological reasoning operates on propositions with complex internal structure."

This is the central scaling challenge of eigenflower. The tensor logic framework (Domingos) is the most promising bridge because it treats propositions as Boolean tensors and syllogisms as einsum operations — the same formalism DKC already implements. But the scaling path from 3-input to propositional complexity is nontrivial.

The parity result (Demo 50, k=6 sectors, 906 solutions) shows the DKC mechanism can reach all 13 NPN classes, but this is still within the Boolean function domain. The question is whether the same algebraic machinery generalizes when "inputs" are propositions rather than bits.

Implications:
- The tensor logic interpreter (Phase 1 build priority) needs to handle propositions of arbitrary complexity, not just binary variables.
- May need intermediate representations that decompose complex propositions into Boolean-like structure.
- The syllogism "All men are mortal" has internal structure (quantifier + subject + copula + predicate) that a simple Boolean encoding doesn't capture.
