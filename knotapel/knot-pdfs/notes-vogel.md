# Paper: [Bartholomew 2011] "An Application of Vogel's Algorithm"
**File:** research-ingress/vogel.pdf
**Author:** Andrew Bartholomew, February 2011

## Paper Summary

Algorithmic paper: describes a computer algorithm that takes a labelled peer code (combinatorial encoding of an oriented link diagram) and produces a braid word. Works for classical, virtual, welded, and flat knots/links. Based on Yamada [3] and Vogel [1] — Vogel's move is a Reidemeister type II move that amalgamates incoherently-oriented Seifert circles, reducing them until all are coherently oriented, at which point the diagram is a closed braid and the braid word can be read off.

This is a purely combinatorial/algorithmic paper with no theoretical depth about invariants, representation theory, or physics. DKC relevance is limited but there are a few algorithmic observations worth recording.

## Paths Not Taken

- **Braid word optimization** (p.6, Section 4): The algorithm produces A braid word but makes no attempt to produce a MINIMAL one. Each Vogel move adds two crossings, so the output braid word is generally longer than necessary. The paper doesn't discuss how many Vogel moves are needed (worst case, average case) or whether the order of moves affects the final braid length. This is computationally relevant — if you're feeding braids into a bracket computation, shorter is better.

- **Choosing WHICH Vogel move** (p.10-11, Section 4.5): When multiple Vogel moves are available (multiple pairs of incoherently-oriented Seifert circles), the algorithm just picks one. There's no analysis of whether different choices lead to different braid lengths or whether an optimal strategy exists. This is a greedy algorithm with no optimality guarantee.

- **Virtual/welded crossing bracket computation** (throughout): The paper handles virtual and welded crossings algorithmically (labels * and # respectively, p.3) but never discusses what happens when you try to compute the Kauffman bracket of the resulting braid. Virtual crossings have their own bracket theory (Kauffman's virtual knot invariants) that differs from the classical case.

## Technical Machinery

- **Peer code representation** (Section 1.1, p.1-3): A compact combinatorial encoding of oriented link diagrams. Each crossing gets a pair of "peer edges" (the two incoming edges). The encoding distinguishes Type I vs Type II crossings based on the relative numbering of peer edges (Definition 1.1, p.3). Labels (+, -, *, #) distinguish classical over/under, virtual, and flat crossings. This gives a complete finite description of any link diagram suitable for computation.

- **Seifert graph as algorithm driver** (Section 2, p.4-5): The Seifert graph Gamma has vertices = Seifert circles, edges = crossings. The REDUCED Seifert graph Gamma' collapses multi-edges. The key insight (Yamada [3], p.4): if all Seifert circles are coherently oriented, the diagram is already a closed braid. So the algorithm reduces to eliminating incoherent orientations.

- **Vogel move mechanics** (Section 3, p.5-6): The move takes two incoherently-oriented Seifert circles s1, s2 that are in the same component of the complement of a third Seifert circle, and performs a Reidemeister II move between them. This AMALGAMATES s1 and s2 into one Seifert circle s_a, and creates a NEW Seifert circle s_n from the two new crossings. Key property: s_a and s_n are COHERENTLY oriented. Net effect: same total number of Seifert circles, but one fewer pair of incoherently-oriented ones.

- **Termination guarantee** (p.6, Lemma 3.1 + Corollary 3.2): Vogel moves reduce the maximum vertex order in Gamma' and eliminate same-type edge pairs at order-2 vertices. The process terminates when Gamma' is a path graph where every vertex has order <= 2 and every order-2 vertex has one Type I and one Type II edge. At this point all Seifert circles are coherently oriented → closed braid.

- **Braid word reading** (Section 4.8, p.13-14): Once in braid form, the Seifert circles become concentric circles. A rotating ray reads off braid crossings in order. The parent-child structure of the reduced Seifert graph (now a path) determines the strand numbering. The "putting s into p" operation (p.14) recursively builds the braid word by interleaving child-strand crossings into parent-strand crossing sequences.

## DKC Overlaps

### Crossing count growth under braiding (LOW-MEDIUM RELEVANCE)

(Section 3, p.5-6): Each Vogel move adds exactly 2 crossings. If a link diagram with n crossings requires m Vogel moves to reach braid form, the resulting braid has n + 2m crossings. The paper doesn't bound m, but for an n-crossing diagram with s Seifert circles, m <= s(s-1)/2 in the worst case (every pair could start incoherently oriented).

**DKC connection**: DKC's crossing depth drives computational power. The braiding algorithm systematically ADDS crossings. This means the braid word for a given knot is generally LONGER than the minimum crossing number of the knot. If DKC is computing brackets of braids produced by an algorithm like this, the crossing depth includes "algorithm overhead" crossings that don't contribute to the knot's topology but do affect the state sum. Understanding which crossings are "essential" vs "overhead" could matter for DKC's capacity analysis.

### Seifert circle structure as computational skeleton (LOW RELEVANCE)

(Section 2, p.4-5): The Seifert graph gives a coarse structural view of a link diagram. The number of Seifert circles equals the braid index (Yamada's theorem). The graph structure (tree? cycle? general?) determines the braiding complexity.

**DKC connection**: The braid index is a lower bound on the number of strands in any braid representation, which constrains the dimension of the representation space in which bracket computation occurs. A link with braid index 2 lives in a 2-strand space; braid index n lives in an n-strand space. DKC's capacity might scale with braid index rather than crossing number per se.

### Virtual crossings as "non-interleaving" markers (LOW RELEVANCE)

(p.3, Section 1.3): Virtual crossings (label *) represent crossings that exist on surfaces but not in 3-space. In the algorithm they're treated differently — they contribute t_i generators instead of sigma_i^{+-1} in the braid word. Flat crossings (label #) are similar.

**DKC connection**: In DKC, non-interleaving braids have bracket = 0 at delta = 0. Virtual crossings are, in a sense, the formalization of "strands that cross on paper but don't really interact." The distinction between classical and virtual crossings in this algorithm might map to the distinction between interleaving and non-interleaving in DKC. A braid with only virtual crossings would be "computationally trivial" in DKC terms.

## Surprises

- **The algorithm works uniformly for classical, virtual, welded, and flat links** (throughout): The same Seifert circle / Vogel move machinery applies regardless of crossing type. The only difference is in the labels assigned. This uniformity is unexpected — you might think virtual crossings would require fundamentally different treatment, but the Seifert circle structure (which ignores crossing type entirely, p.4 "Seifert circles are features of the immersion... and are independent of the nature of the crossings") is the same for all types. The crossing type only matters at the very end when reading the braid word.

- **Seifert circles are topological, crossings are algebraic** (p.4, Figure 4): The Seifert circles depend only on the immersion (shadow) of the link diagram, not on over/under information. This decomposition — topology first (Seifert circles), algebra second (crossing labels) — is a clean separation that might be useful for DKC. The computational content of a braid might decompose similarly: the Seifert structure determines the "skeleton" and the crossing signs determine the "computation."
