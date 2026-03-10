---
id: CON0003
title: "Orchid data-dependent convolution parallels DKC eigenvector automaton routing"
connects:
  - "Orchid data-dependent convolution with conditioning network"
  - "DKC eigenvector automaton and Hopf phase suppression (D67, D73)"
type: structural_parallel
strength: moderate
source: "extractions/group-a-knotapel-relevance.md, Connection 3"
---

Orchid's conditioning network generates a data-dependent kernel that determines HOW the convolution processes the input. In DKC, the eigenvector direction of the partial sum determines WHICH Voronoi cell the computation enters, which determines the output. Both are "data-dependent routing" mechanisms.

The 82.8% automaton determinism (D73) quantifies how much information the routing captures versus how much is lost. The 17.2% non-determinism arises from discarded magnitude information ("compass without odometer").

Orchid's phase suppression conditioning (|F(Conv1d(x))|, killing phase) is structurally parallel to DKC's Hopf decomposition (D67), where the S^1 phase fiber is computationally inert and all information lives in the S^2 base. Both systems discover that phase information should be suppressed and directional information preserved.

The 13=13 theorem (D71: minimum bandwidth l=6 because 2*6+1=13 = number of eigenvector directions) provides a principled bandwidth selection for Orchid's conditioning kernel length.
