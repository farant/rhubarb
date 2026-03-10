---
id: CON0014
title: "Free Transformer latent Z IS DKC encoding choice (parity-lock theorem proves it)"
connects:
  - "Free Transformer conditioning on explicit latent variables Z from uniform prior"
  - "DKC encoding as latent structure: parity-lock theorem (D92)"
type: mathematical_identity
strength: strong
source: "extractions/group-b-knotapel-relevance.md, Connection 4"
---

DKC's encoding scheme is EXACTLY a latent variable that determines the computational mode. The Parity-Lock Theorem (D92, PROVED) shows that the +/-q paired encoding structurally locks computation to XOR/XNOR -- AND/OR/MAJ are provably impossible under ANY activation. The encoding IS the latent variable Z.

Changing the encoding (from +/-q to 1-weight-per-input) changes the accessible Boolean function space from {XOR, XNOR only} to {all 13 NPN classes}. The encoding determines the function; the weights and activation fill in the details. This is the coin-flip example: once you know the encoding (Z), the truth table follows simply.

Free Transformer's kappa parameter (information rate through Z) maps to DKC's sector count k: at k=2 (maximum compression) 5 NPN classes accessible, at k=6 all 13.

D107's constructive Z/4Z phase formula is literally a 3-bit binary mapper: Entry(r,c) = (integer path count) * zeta_8^{(w - 2*(nest(r) + nest(c))) mod 4}. Three binary variables (writhe parity, nesting of source, nesting of target) determine the cyclotomic phase -- a 3-bit latent code controlling the "logical mode" of each matrix entry.

The Free Transformer's core insight -- standard autoregressive models waste capacity re-inferring latent structure -- maps to DKC's training-as-focusing thesis (D29): training wastes capacity re-discovering topological structure that already exists on the lattice.
