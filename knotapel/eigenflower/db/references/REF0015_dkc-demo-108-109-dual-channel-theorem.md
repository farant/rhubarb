---
id: REF0015
title: "DKC Demos 108-109 — Dual-Channel Theorem"
type: dkc_theorem
source_project: knotapel
demo_number: 108
summary: "Parity requires BOTH multiplicative phase coherence (product closure) AND additive magnitude diversity (v_2 connectivity). Neither channel alone suffices. Encoding-dependent polarity inversion: under multiplicative encoding, the product closure polarity inverts. Maps onto T-gate/Hadamard decomposition in quantum circuit synthesis (Amy Bridge)."
related: []
---

## Description

Demos 108-109 established the Dual-Channel Theorem:
1. Parity-capable vocabulary has 17 product closure edges; poison vocabulary has 0 (maximal structural contrast)
2. Both phase coherence (multiplicative channel) AND magnitude diversity (additive channel) are independently necessary
3. The same Z[zeta_8] value can be parity-capable at k=6 and poison at k=15 -- resolution dependence
4. Under multiplicative encoding (D109), product closure polarity INVERTS relative to additive encoding
5. The Amy Bridge conjecture maps the dual channels onto T-gate (phase rotation) / Hadamard (magnitude mixing) decomposition in quantum circuit synthesis

The 3-way partition (41 parity / 17 poison / 42 neutral out of 100 catalog values) emerged entirely from graph analysis of algebraic relationships -- nobody designed it.

## Relevance to Eigenflower

- The dual-channel requirement constrains any DKC-inspired architecture: it must have both rotational diversity (phase) and scale diversity (magnitude)
- The encoding-dependent polarity inversion shows that the same algebraic structure produces different computational capabilities depending on encoding -- the encoding IS the conditioning variable
- Maps directly to Orchid's cross-attention: conditioning the kernel on a different sequence changes the computational role of the same weights
- The emergent knowledge graph (algebraic relationship graph producing the parity/poison/neutral partition) parallels Paper 1's (SciencePedia) emergent cross-domain structure
- The Amy Bridge conjecture connects DKC to quantum circuit synthesis resource theory
