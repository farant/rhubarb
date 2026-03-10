---
id: REF0009
title: "DKC Demo 92 — Parity-Lock Theorem (P04)"
type: dkc_theorem
source_project: knotapel
demo_number: 92
theorem_id: "P04"
summary: "+/-q encoding is structurally locked to XOR/XNOR; AND/OR/MAJ produce zero winners at every depth. The encoding concentrates ALL computational power on parity -- the single Boolean function outside AC^0. Formally proven."
related: []
---

## Description

The Parity-Lock Theorem (proven formally as P04) establishes that under +/-q paired encoding, the weight sum for input masks 00...0 and 11...1 are both zero, making any function where f(0...0) != f(1...1) structurally impossible. The encoding creates 3^k equivalence classes where parity is constant on every class (0 conflicts) but AND has 1/27 conflicts, OR has 1/27, and MAJ has 19/27.

This is not merely "regularization" -- the constraint structurally selects for parity by making all non-parity functions literally impossible. The encoding does not limit computation; it concentrates it on the hardest possible function (parity, outside AC^0).

## Relevance to Eigenflower

- The encoding IS the latent variable Z in the Free Transformer framework: a single binary choice (encoding) determines the entire accessible function space
- Provides the algebraic formalization of TRM's "less is more" principle: extreme constraint provably eliminates easy functions and retains only the hardest one
- The 3^k equivalence class analysis provides a template for analyzing any input encoding's effect on accessible computations
- Directly relevant to Eigenflower's encoding design: encode inputs so easy solutions are structurally impossible, forcing the system to find hard solutions
- The parity-lock IS a tensor logic impossibility result -- a constraint on the tensor equation that Domingos's framework does not anticipate
