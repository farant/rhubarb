---
id: CON0011
title: "H-Net dynamic chunking structurally mirrors DKC BFS depth rounds as chunk boundaries"
connects:
  - "H-Net learned content-aware dynamic chunking via cosine similarity routing"
  - "DKC BFS depth rounds as natural compression boundaries (D82, D90)"
type: structural_parallel
strength: moderate
source: "extractions/group-b-knotapel-relevance.md, Connection 1"
---

H-Net replaces fixed tokenizers with learned boundaries that emerge where adjacent encoder outputs disagree (low cosine similarity). DKC's BFS closure produces natural "boundaries" at each depth round: the value set grows exponentially (~2x per round) but computational power grows linearly.

The depth round IS the natural chunk boundary. Entries born at the same BFS depth share intermediate algebraic products (D90: "algebraic coherence beats raw vocabulary"), exactly as H-Net's chunks share encoder hidden states.

DKC's pipeline is a concrete hierarchical compression instance: Braid word -> TL algebra -> bracket value -> Boolean function, with the numerical pivot at D10 being a literal 50x compression (Poly 808 bytes/entry replaced by Cx 16 bytes/entry).

The ratio loss in H-Net (preventing trivial compression solutions) has a DKC analog: the finite group ceiling (D79-D80). At zeta_8, the binary octahedral group's 24 elements cap the catalog. Moving to zeta_12 (infinite group) removes this ceiling -- analogous to relaxing H-Net's compression ratio constraint.
