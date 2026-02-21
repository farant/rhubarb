# Paper: [Barbensi, Celoria, Harrington, Stasiak, Buck 2020] "Grid diagrams as tools to investigate knot spaces and topoisomerase-mediated simplification of DNA topology"
**File:** sciadv.aay1458.pdf + aay1458_sm.pdf (supplementary)

## Paths Not Taken

- **Twisted hooked juxtapositions unexplored.** The Discussion (p.6) mentions that "the effects of twisted hooked juxtapositions on simplification of knots [(28); see also (29)] can be addressed by observing the consequences of passages within a tight 'diagonal row' of juxtapositions resembling a plectoneme in supercoiled DNA." They flag this as possible but don't pursue it. A diagonal-row geometry would be a fundamentally different local operation from their rectangular hooked juxtapositions — more physically realistic for supercoiled DNA but combinatorially much harder to enumerate.

- **Juxtaposition angle effects deferred.** Also in Discussion (p.6): "To study the effects of the juxtaposition angle considered earlier [(30, 31); see also (32)], one may observe the consequences of passages in most tight juxtapositions, which are not immediately followed by next tight juxtapositions, as such juxtapositions in a 3D situation naturally predispose the contacting segments to be perpendicular to each other." They acknowledge angular dependence matters but stay with right-angle grid geometry throughout.

- **Temperature/physical parameters excluded by design.** (p.7): "We remark that our model, being purely topological, does not consider such physical quantities such as temperature." They explicitly note the model is temperature-independent and equal-weight — this is a deliberate choice to isolate topology from thermodynamics, but it means their transition probabilities are a "topological prior" divorced from any energy landscape.

- **Composite knots partially explored.** They include connected sum 3_1 # 3_1 in their analysis (visible in Fig. 5, p.5) but don't systematically analyze how composite knots behave in the configuration network. The composite knot appears to follow similar decay curves but this is noted almost in passing.

## Technical Machinery

- **Grid diagrams as pairs of permutations.** (SM p.3) A grid diagram is entirely specified by two permutations σ_X and σ_O on n elements. The number of disjoint cycles in σ_X · σ_O^{-1} gives the number of link components (1 cycle = knot). This is a very clean combinatorial encoding — every grid diagram is a point in S_n × S_n, making the configuration space finite and exactly enumerable for small n. **DKC note:** This permutation-pair encoding is structurally parallel to how we encode braid words, but grid diagrams have a different move set (interleaving commutations vs. braid relations).

- **Interleaving commutation as the atomic move.** (SM p.3, Def. 3 + Theorem 1.4) Two consecutive columns/rows with nontrivially intersecting intervals that neither contains the other can be swapped. This single swap = one crossing change = one topoisomerase action. The constraint "neither is contained in the other" is the interleaving condition — it's what prevents trivial Reidemeister moves that don't change knot type.

- **Exact enumeration via Sage + knot invariant battery.** (SM p.5) They determine knot type using "a combination of knot invariants [Alexander and Jones polynomials, determinant, and signature]." For GN ≤ 7 they enumerate ALL pairs of permutations (complete). For GN 8-19 they random-sample 16,000 configurations. The completeness claim for small GN is important — it means the adjacency tables for GN ≤ 7 are exact, not statistical.

- **Ergodicity argument via Python's random().** (SM p.4) Their sampling model rests on the claim that uniform random sampling of permutation pairs gives ergodic coverage of knot types proportional to their representation in the grid space. This is correct but subtly depends on the space being S_n × S_n with the uniform distribution — different distributions on diagrams would give different "knot priors."

- **Hooked juxtaposition parameterized by rectangle area.** (SM p.5) They create GN-3 adjacency tables per grid number, one for each rectangle area threshold l (1 ≤ l ≤ GN-3). The monotonic increase of knotting reduction factor with decreasing area (Fig. S5) is the core quantitative result. At area = 1 (elementary square = "strongly hooked"), the topological simplification is maximized.

## Speculative Remarks

- **Grid number as proxy for polymer length.** (p.2) They note that "a grid of size n can be seen as corresponding to an equilateral polymer chain with 2n statistical segments," so GN 5 ≈ 10 segments ≈ 3 kb of DNA. But they immediately caution this is approximate. The question of whether grid number complexity faithfully mirrors 3D polymer complexity at larger scales is left open.

- **"It is possible that the effective bending..."** (p.5) They speculate that topoisomerase IV may create even tighter bending than their elementary-square model captures: "It is possible that the effective bending in hooked juxtapositions interacting with the type II topoisomerase, topoisomerase IV, is stronger than the one that we can model using grid diagrams." This implies their 15-fold knotting reduction factor is a LOWER BOUND on what the real enzyme achieves (experimental: 50-90 fold).

- **Infinite knotting reduction at GN 5.** (p.5) For the smallest nontrivial grid number (GN 5), the knotting reduction factor for trefoils at strongly hooked juxtapositions is INFINITE — every trefoil → unknot passage occurs at hooked sites, and no unknot → trefoil passage occurs at hooked sites. This is a small-n artifact but a striking one.

## DKC Overlaps

- **Crossing depth ↔ "hookedness" as computational resource.** The paper's central finding is that LOCAL GEOMETRY at the crossing site determines the direction of topological simplification. More hooked = more simplifying. This resonates with DKC's finding that crossing depth (more generator multiplications in the braid group) drives computational capacity. Both frameworks identify a "depth" or "tightness" parameter at the crossing level that controls the topology-changing power of the operation. The grid diagram version is geometric (rectangle area); the DKC version is algebraic (depth in the group).

- **Knot space as a directed graph with asymmetric fluxes.** (p.3-4, Fig. 3-4) When passages are restricted to hooked juxtapositions, the interconversion network becomes DIRECTED — simplification flows are stronger than complexification flows. This asymmetry is reminiscent of DKC's finding that certain braid operations preferentially produce simpler bracket values. The "knotting reduction factor" is essentially a ratio of forward vs. backward flux, which parallels how activation functions in the MVN framework gate certain computational directions.

- **Configuration space size grows super-factorially.** (p.7) The number of grid configurations is n/2((n-1)!)^2 which grows super-factorially with grid number. They note that "the number of admissible crossing changes increases more than linearly with the complexity." This combinatorial explosion is analogous to the exponential state-sum growth that makes Kauffman bracket computation #P-hard — both are manifestations of the same underlying combinatorial complexity of knot spaces.

- **Trefoil as universal attractor under hooked passages.** (p.4-5) Under geometric selection (hooked juxtapositions only), the trefoil consistently has the strongest outgoing flux toward the unknot. The trefoil is the simplest nontrivial knot and corresponds to the simplest nontrivial braid (σ₁³). In DKC terms, this is the minimal crossing-depth configuration that produces a nontrivial bracket value. The paper confirms that this minimal configuration is topologically "closest" to triviality — which is consistent with our finding that low crossing-depth braids have limited computational capacity.

- **Grid diagram ↔ Kauffman bracket connection (implicit).** The authors use Jones and Alexander polynomials to IDENTIFY knot types but never evaluate them AT SPECIFIC ROOTS OF UNITY. They're using invariants as classifiers, not as computational objects. The grid diagram formalism could in principle be used to compute the Kauffman bracket directly (grid homology does something related), but this paper doesn't go there. This is a significant unexploited connection — grid diagrams give you a combinatorial handle on the state sum that could potentially be related to the DKC state-sum decomposition.

- **Entanglement vanishing connection (weak).** The paper notes (p.4) that "most (<91.7%) of the strand passages occurring in unknotted diagrams do not change the topology." This means most crossing changes on unknotted configurations produce unknotted configurations — the unknot subspace is highly "sticky." This is tangentially related to DKC's entanglement vanishing (non-interleaving braids have bracket = 0 at delta = 0), in that both phenomena describe conditions under which crossing operations produce trivial results.

## Surprises

- **2D grid diagrams produce MORE knotting than 3D models for same segment count.** (p.4, SM p.6-7) This is counterintuitive — you might expect a 2D model to be less tangled than 3D. But grid diagrams with GN n effectively represent polymers with 2n segments, and the overpassing convention (vertical always over horizontal) creates crossings that would require 3D freedom to avoid. The authors note "in polygonal chains formed by essentially 2D grid diagrams, knotting is stimulated as compared to 3D situation." This means grid diagrams OVER-sample complex knots relative to physical polymers.

- **Detailed balance holds perfectly for unbiased passages.** (p.3) For unbiased (all interleaving commutations allowed), the incoming and outgoing fluxes between any pair of knot types are EXACTLY equal — not approximately, but exactly for the enumerated cases. This is a consequence of the symmetry of interleaving commutations (they're self-inverse: swapping columns i and j, then swapping again, returns to the original). This perfect symmetry breaks when you restrict to hooked juxtapositions, which is what creates the directed simplification flow.

- **Knot fauna becomes dramatically richer at GN 9-16.** (Fig. 4, p.4) The circos plots for GN 6 vs GN 9 vs GN 16 show a dramatic transition from a nearly unknot-dominated space to a richly populated knot zoo. The rate at which new knot types "turn on" as a function of grid number would be interesting to map — it might reveal a phase transition in knot-space density similar to DKC's four-tier structure (k=2, k=3, k=4, k=6 sectors).

- **The "sudoku" constraint of grid diagrams.** (p.2) Each row and column must contain exactly two markings (one X, one O), creating a constraint pattern the authors explicitly compare to sudoku. This means grid diagrams are NOT arbitrary crossing patterns — they have a built-in combinatorial rigidity. For DKC, this suggests that grid-diagram-based state sums would automatically exclude many "pathological" braid configurations and might give cleaner results than working with arbitrary braids.
