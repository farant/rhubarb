---
id: CON0039
title: "Tensor logic temperature parameter maps to DKC sector count k"
connects:
  - "Domingos tensor logic: sigmoid temperature T controls deductive vs analogical reasoning"
  - "DKC k-sector MVN activation with non-monotonic parity solutions (D50, D91)"
type: structural_parallel
strength: moderate
source: "extractions/group-d-knotapel-relevance.md, Connection 8"
---

Paper 1's temperature T controls reasoning mode: T->0 deductive (sharp boundaries), T->infinity analogical (soft boundaries). DKC's sector count k controls computational selectivity: k=2 (coarsest, 5 NPN classes), k=6 (all 13 including parity, 906 solutions), k=8 (parity drops to 96).

Low T (deductive) = high k (many sectors, fine discrimination). High T (analogical) = low k (few sectors, coarse discrimination).

The non-monotonic behavior (906@k=6 > 756@k=7 > 96@k=8) is Paper 1's no-free-lunch for temperature: too sharp loses capacity through over-discrimination; too soft through under-discrimination. The optimal "temperature" is k=6 because gcd(6,8)=2 creates the right incommensurability with the Z[zeta_8] lattice.

The gcd(k,8) anti-correlation is an incommensurability result that tensor logic's temperature framework cannot yet predict. This is genuinely new -- no framework in Paper 1 accounts for interaction between temperature and weight algebra lattice structure.

D91 confirms: varying k from 2 to 48 does not shift depth gaps between XOR levels. Resolution controls visibility, not capability; the parity constraint itself is the wall.
