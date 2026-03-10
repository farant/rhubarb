---
id: PT0062
title: "MODBP clustering for theological knowledge hierarchy"
status: untested
source: ["Inverse Knowledge Search paper (Li et al., 2026)"]
applicable_to: []
related: [PT0033, PT0034]
---

Use Modularity Belief Propagation (MODBP) to recursively partition the syllogism dependency graph, discovering the natural hierarchical structure of theological knowledge. MODBP partitions until subgraphs are indistinguishable from random, yielding a tree of coarse-to-fine theological areas.

The Inverse Knowledge Search paper produced a 21-level hierarchy from 120K concept nodes. For eigenflower: the Summa's ~3,000 articles would produce a smaller but well-structured graph.

The discovered hierarchy would reveal:
- Which theological treatises are most interconnected
- Which concepts serve as "hubs" in the logical dependency structure
- Natural clusters of related syllogisms
- Bridge concepts that connect different theological domains

This could inform the architecture: assign different model capacity to different clusters, or use the hierarchy to structure the syllogism graph for efficient traversal.

Simpler alternatives to MODBP (Louvain, spectral clustering) could work for the smaller eigenflower graph.
