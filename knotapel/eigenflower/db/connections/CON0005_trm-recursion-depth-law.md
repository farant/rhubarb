---
id: CON0005
title: "TRM recursive refinement and DKC linear depth law are the same mechanism"
connects:
  - "TRM recursive 2-layer network (n=6 times, 42 effective layers)"
  - "DKC linear depth law: max_xor ~ depth + 6 (D82, D89-D91)"
type: mathematical_identity
strength: strong
source: "extractions/group-a-knotapel-relevance.md, Connection 5"
---

TRM's core insight -- recursively applying the same transformation is more powerful than applying a bigger transformation once -- is exactly what the DKC depth law says. In DKC, "depth" is the number of times braid generators are multiplied together. The linear depth law says each additional recursive application adds +1 to computational capacity (max_xor ~ depth + 6).

TRM's "42 effective layers from 2 layers applied 21 times" is the same architecture principle as DKC's "depth-8 entries from generators multiplied 8 times."

The "algebraic coherence beats raw vocabulary" result from D82 is the precise algebraic analog of TRM's "2 layers + deep recursion beats 4 layers + shallow recursion." In both cases, recursive structure creates shared intermediate representations (common BFS ancestors in DKC, latent z state in TRM) that raw width cannot replicate.

TRM's two features (y = current answer, z = reasoning trace) map onto the Dual-Channel Theorem (D108-D109): parity requires BOTH phase coherence (the z-like "how we got here" channel) AND magnitude diversity (the y-like "current state" channel). Both channels are independently necessary.

The balanced exponentials mechanism (D91: supply ~2x/depth, demand 4x/weight, giving slope ~1) could inform TRM's learning rate scheduling.
