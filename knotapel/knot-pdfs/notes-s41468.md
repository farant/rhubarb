# Paper: [Bar-Natan, Bar-Natan, Halacheva, Scherich 2024] "Yarn ball knots and faster computations"
**File:** s41468-023-00144-7.pdf
**Journal:** Journal of Applied and Computational Topology 8:175–192 (2024)

## Paths Not Taken

- **Hyperbolic volume conjecture (p.180):** The authors conjecture (attributed to D. Bar-Natan, van der Veen) that hyperbolic volume has "S3D" — savings in 3D for small knots. They note this is open and leave it completely unexplored. The argument is suggestive: more complicated knot → more complicated complement in S³ → harder to put hyperbolic structure on it → stingy quantity → should have 3D savings. This is a clean path-not-taken.

- **Genus as S3D quantity (p.180):** They note the genus of a knot "should" be best computed in 3D (via Seifert surface) but they don't know if it actually has S3D. Left completely open.

- **Are Alexander, Jones, HOMFLY-PT polynomials C3D? (p.180):** Explicitly flagged as unknown at time of writing. "Are the Reshetikhin-Turaev invariants C3D? Are knot homologies C3D?" These are major open questions about whether the most important knot invariants benefit from 3D computation.

- **Improving the upper bounds (p.179):** They suspect the bounds in Theorems 3.2 and 3.3 can be improved "by closer consideration of the counting arguments used in the proofs" — they explicitly encourage others to do this work. The current ~V^d bound for type-d finite type invariants in 3D may not be tight.

- **SO(3) vs SO(2) footnote (p.176, footnote 1):** Fascinating aside — Jones's original description via braids is "1D" because the full 3D rotation group SO(3) acts on 3D space but only SO(2) acts on planar diagrams. This means the braid approach already throws away rotational symmetry. The paper's 3D approach recovers some of this.

## Technical Machinery

- **Grid knots as 3D representation (Section 2, p.181-182):** The key technical construction. A "grid knot" of size L is a knot embedded in an L×L×L grid with arcs along grid segments. Converting yarn ball knot (volume V) to grid knot: resulting grid is bounded in box of size ~10³V. The grid has three colored directions: green ("/"), red ("\"), blue (vertical). ALL crossings occur in triangular "crossing fields" between green and red lines only — blue lines never participate in crossings. There are 2L² triangular crossing fields.

- **Crossing field counting for linking number (p.182-183):** Within each crossing field F_k, they define a height map z: R,G → [0,L] giving vertical strand height, then count inversions (green over red or vice versa) using a sweep-line algorithm with a placeholder ∇. This gives O(L) per field, O(L³)=O(V) total. The key insight: the 3D structure lets you decompose a global counting problem into L² local problems, each solvable in O(L) time.

- **Labeled Gauss diagrams for finite type invariants (Section 3, p.184-187):** The proof of Theorem 3.3 (C_ζ(3D,V) ~ V^d for type-d invariants) uses a sophisticated counting scheme:
  1. Factor through Goussarov-Polyak-Viro: ζ = ω ∘ φ_d where φ_d maps to formal sums of Gauss diagrams
  2. Decorate Gauss diagram arrows with crossing types x₁,...,x₈ (8 types from grid orientation)
  3. For each labeled Gauss diagram D_χ, assign crossing field tuples and pairs of strand sets (B_i, B_{i'}) for heads and tails
  4. Reduce to a combinatorial counting problem: |A| where A counts compatible strand selections satisfying both t-ordering and z-height (Cartesian) conditions

- **Binary sequence decomposition (Section 4, p.189-191):** The proof of Proposition 4.2 is the combinatorial heart. They encode z-values as binary strings of length p = ⌈log₂ L⌉, decompose the Cartesian conditions into subdiagonal constraints on a triangle grid (Fig. 10-11), and use a union-of-sets argument over (p-1)^d choices of diagonal coordinates q̃. The worst case is (2^{p-1})^{d-1} · L ~ L^d, giving the V^d bound.

## Speculative Remarks

- **"Most finite type invariants are C3D" (p.180):** The authors state this as an "opinion" / "naive conclusion" — that in general knot invariants should be C3D (computationally 3D). They acknowledge only one-sided bounds are proven. This is a strong philosophical stance: the 2D diagram approach is fundamentally suboptimal for most invariants.

- **"Conversation Starters" framing (p.179-180):** The paper introduces two deliberate provocations for the community:
  1. C3D: Is FKT_ζ(3D,V) ≪ FKT_ζ(2D, V^{4/3})?
  2. S3D: For "stingy" quantities η, is M_η(3D,V) ≪ M_η(2D, V^{4/3})?
  These are proposed as research programs, not theorems.

- **C3D status is temporal (p.180):** They explicitly note that C3D status depends on current algorithmic knowledge — an invariant might gain or lose C3D status as better algorithms are discovered. This is an unusual epistemic humility for a math paper.

## DKC Overlaps

- **Crossing depth as complexity driver (p.177-178, p.185):** The paper's central thesis resonates with DKC's "crossing depth drives computation." Their complexity measure for finite type invariants is literally the type d — the number of crossings you need to examine simultaneously. Their Theorem 3.3 shows C_ζ(3D,V) ~ V^d, meaning computational cost scales with the depth of crossing interaction. In DKC terms: deeper entries in the group closure (more generator multiplications) = higher type invariants = more computational power. The paper quantifies this for the first time in a 3D setting.

- **The V^{4/3} threshold as 2D→3D boundary (p.177-178):** The key ratio n ~ V^{4/3} (crossings in projection vs volume) is a dimensional analysis result: projecting a 3D object to 2D inflates complexity by exactly the 4/3 power. This is relevant to DKC's concern with how crossing number relates to computational capacity — the 2D crossing count is a dimensionally-distorted view of the true 3D complexity.

- **Writhe implicit in crossing type labeling (p.183, 185):** The 8 crossing types x₁,...,x₈ in the grid formulation encode orientation and over/under information. This is essentially writhe data distributed across the crossing fields. The paper works with RAW crossing information (not Jones-normalized) — they don't normalize away writhe. This aligns with DKC's finding that writhe carries computational information.

- **Finite type invariants as computational hierarchy (p.179):** The type-d hierarchy of finite type invariants maps cleanly to DKC's depth hierarchy. Type 1 = linking number (computable in O(V)). Type d = O(V^d). The Jones polynomial contains ALL finite types (it's a generating function for them), so its complexity is the limit of this hierarchy. This connects to DKC's finding that the full bracket/Jones gives maximum computational power.

## Surprises

- **DNA is a yarn ball, not a pancake (p.176-177):** The paper opens by noting that knotted DNA "is shaped much more like the ball of yarn in Fig. 1B, than the pancake knot in (A)." This is a simple observation but surprisingly rarely stated in the knot theory literature, which overwhelmingly works with 2D diagrams. The entire paper is motivated by this mismatch between mathematical convenience (2D) and physical reality (3D).

- **Only ONE invariant proven C3D (p.180):** Despite the paper's optimism, at time of writing the linking number is the ONLY invariant actually proven to be C3D. All the "important" invariants (Jones, Alexander, HOMFLY-PT, Reshetikhin-Turaev, knot homologies) have unknown C3D status. This is a surprising gap given how natural the 3D approach seems.

- **The reference list is remarkably short (p.192):** Only 11 references for a paper in a major journal. This suggests the 3D computational perspective is genuinely underexplored — there isn't a large body of prior work to cite.

- **Vertical blue lines NEVER participate in crossings (p.182):** In the grid formulation, one entire dimension of the grid (the vertical/blue direction) contributes zero crossings. All crossings happen in the 2D crossing fields between green and red. This means the 3D advantage comes not from 3D crossings but from the 3D ORGANIZATION of 2D crossing information — the third dimension provides a decomposition structure, not additional crossing types. This is a subtle but important distinction.

- **The 8 crossing types are exactly what you need (p.183, Fig. 5):** The classification into x₁,...,x₈ based on strand color, orientation, and over/under is combinatorially clean — exactly 2³ = 8 types. This is the grid analogue of the classical ±1 crossing sign, but carrying more information because orientation within the grid matters. The labeled Gauss diagram machinery (LGD_d) is a natural categorification of classical Gauss diagrams that seems underexploited.
