---
id: CON0031
title: "GPTQ weight clipping obstruction parallels DKC parity ceiling"
connects:
  - "GPTQ weight clipping violating Babai's error bound, resolved by SSQR/HPTQ"
  - "DKC parity ceiling at n=5 inputs, resolved by larger groups or different encodings (D63-D65)"
type: structural_parallel
strength: speculative
source: "extractions/group-c-knotapel-relevance.md, Connection 11"
---

GPTQ clips overflowed integers to the representable range, violating Babai's error bound. The paper proposes SSQR (sparse full-precision outliers) and HPTQ (unconstrained integers + Huffman).

DKC's parity ceiling at n=5 is analogous: the finite catalog constrains what Boolean functions are computable. Two impossibility mechanisms: constraint-wall at n=6 (63 constraints unsatisfiable) and pigeonhole at n>=8 (7 class-1 octants insufficient).

The ceiling lifts when:
- Catalog expands to Z[zeta_12] (infinite group, D79)
- Encoding changes (D92)
- Activation generalizes (D65)

GPTQ's SSQR (sparse outliers at full precision) maps to DKC's mixed-catalog (mostly Z[zeta_8] plus a few entries from richer ring). HPTQ (unconstrained integers) maps to DKC's infinite-group regime (Z[zeta_12] has no hard ceiling). In both systems, the ceiling depends on representation policy, not inherent computational limits.
