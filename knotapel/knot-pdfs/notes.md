# DKC Research Paper Notes

Extracted from 18 academic papers in `knot-pdfs/` and `research-ingress/`. Organized by DKC relevance.

## Cross-Paper Synthesis

Four independent frameworks converge on the same conclusion about DKC's operating point (A = zeta_8, delta = 0):

| Paper | Framework | Statement |
|-------|-----------|-----------|
| Witten 1989 | TQFT | Framing anomaly at level k=0 is discarded but contains structure |
| Rasmussen 2004 | Khovanov categorification | s-invariant lives at the singularity boundary; collapse pattern = data |
| Belletete et al. 2017 | TL representation theory | Non-semisimple indecomposables at beta=0 ARE the computational resource |
| Gainutdinov et al. 2013 | Logarithmic CFT | Dense polymer catastrophe at c=-2 forces logarithmic partners |

**The singularity is generative, not degenerate.**

DKC's delta=0 point has a name in physics: **dense polymers** (p=2, q=i, n=0, c=-2). It is the simplest root-of-unity case and the unique point where the LCFT is a free theory (symplectic fermions), exactly solvable.

### Key Recurring Themes

1. **Writhe carries essential data**: Kirby (1978) proved framing determines 3-manifold topology. Jones (1984) then normalized it away. DKC's use of raw bracket vindicated by the foundational theorem of surgery theory.

2. **Order 24 keeps appearing**: S_4 monodromy in branched coverings (24 elements), binary octahedral group at zeta_8 (24 elements). Both act as hard capacity ceilings. Related via cube/octahedron symmetries.

3. **Stabilization = sector enlargement**: Adding a trivial 5th sheet resolves node singularities (Piergallini). Adding sectors to k=6 unlocks parity (DKC). Same pattern: trivial augmentation enables previously blocked operations.

4. **Non-interleaving = vanishing**: Non-interleaving braids give bracket=0 at delta=0 (DKC). Disjoint-support monodromy creates node singularities (Piergallini). Null states have zero norm at c=0 (LCFT). All reflections of the same phenomenon.

5. **Crossing depth = computational hierarchy**: Confirmed by finite type invariant hierarchy (Bar-Natan et al.), signature growth formulas (Davies et al.), Jordan cell rank growth (LCFT), and indecomposable module depth (Belletete et al.).

### Unexplored Bridges (Potential Next Directions)

- **Logarithmic partners for DKC**: LCFT says null states at delta=0 should have logarithmic partners carrying the real information. DKC hasn't defined these yet.
- **Zigzag algebra connection**: TL at beta=0 has AR quivers matching Huerfano-Khovanov zigzag algebras, which appear in Khovanov homology categorification. Unexplored bridge.
- **Affine TL at roots of unity**: Standard modules W_{j,z} have unknown structure. Two-parameter family potentially much richer.
- **Grid diagram bracket evaluation**: Nobody has evaluated grid-diagram state sums at specific roots of unity.
- **K(B,k) family as DKC test case**: Grid homology can't distinguish these knots. If bracket can, DKC carries complementary information.
- **Natural slope quantum interpretation**: Davies et al.'s cusp geometry quantity may connect to Levine-Tristram signature at roots of unity.

---

# HIGHEST DKC RELEVANCE

---

# Paper: [Belletete, Ridout & Saint-Aubin 2017] "Restriction and Induction of Indecomposable Modules over the Temperley-Lieb Algebras"
**File:** 1605.05159.pdf (research-ingress/)
**ArXiv:** 1605.05159

55-page comprehensive classification of ALL finite-dimensional indecomposable TL_n(beta) modules. **CRITICAL: beta = 0 IS the DKC regime** (q = i, A = zeta_8).

## Paths Not Taken

- **One-boundary and two-boundary TL algebras (p.3):** "The complexity of the modules over the one-boundary Temperley-Lieb algebras and their quotients seems to go well beyond that of the indecomposable modules that will be classified in the present paper." The blob algebra (one-boundary TL) has Kac boundary conditions whose Virasoro-module identifications "remain at the level of conjecture." Explicitly open.

- **Lattice fusion as categorical operation (p.2, footnote 1):** "The fact that lattice fusion operates between modules over different algebras indicates that it should be thought of as an operation on modules over the Temperley-Lieb categories." The categorical perspective is suggested but completely undeveloped. Fusion stabilizes (doesn't depend on n,m for large n,m) — "This observation is welcome since these integers have no natural interpretation in the continuum limit." Nobody understands WHY.

- **Affine TL at roots of unity (p.52-53):** Standard modules W_{k,z} parameterized by TWO parameters. "The identification of the M_{j,z} is presently unknown in general" at roots of unity. Loewy diagrams show dramatically deeper structure — full diamond lattices instead of zigzags. Explicitly open.

- **Zigzag algebra connection (p.49):** At beta = 0, the TL_n AR quivers "interestingly coincide with those of a zigzag algebra discussed in [Huerfano-Khovanov]." Noted and immediately dropped. Creates unexplored bridge: DKC regime -> TL at beta = 0 -> zigzag algebras -> categorification.

## Technical Machinery

- **Orbit partition of Lambda_n (p.4-5):** Index set partitioned into "critical" integers and non-critical orbits. Central element F_n has eigenvalue delta_k = q^{k+1} + q^{-k-1} on standard module S_{n,k}. Modules with different F_n-eigenvalues have Hom = 0 AND Ext = 0.

- **Ext-group table (p.14, Prop 2.18):** Ext(l_k, l_{k'}) isomorphic to C iff k' = k^+- (immediate neighbors in orbit). Extensions only happen between ADJACENT elements.

- **Recursive B/T construction (pp.19-20):** B^l_k and T^l_k built by iterating extensions. Each step adds two composition factors. These zigzag modules are the complete set of "new" indecomposables.

## DKC Overlaps

- **beta = 0 IS the quantum dimension singularity:** l_0 = 0 VANISHES. S_0 = 0. The entire "vacuum sector" disappears. This is the MODULE-THEORETIC manifestation of [2]_q = 0.

- **Non-semisimplicity = computational richness:** At roots of unity, standard modules are reducible but INDECOMPOSABLE. The "extra structure" is precisely WHY the bracket carries computational information. If semisimple, everything decomposes cleanly and there's nothing to exploit.

- **Orbit confinement = MVN sector structure (Prop 2.2):** All composition factors in a single orbit [k]. Different orbits: Hom = Ext = 0 (complete decoupling). At l = 2 (DKC), orbits maximally small — explains tight capacity ceiling.

- **B^l_k depth = crossing depth:** Each additional crossing depth creates the next module in the recursive family, with one more layer of non-split extension. Deeper = more computational content.

- **Representation-finiteness = capacity ceiling (Theorem 3.10):** TL_n has FINITELY many indecomposable modules. This algebraic finiteness IS the DKC capacity ceiling.

- **Lattice fusion = tangle composition = DKC computation:** Fusion "may result in indecomposables that are neither irreducible, standard nor projective." The computation IS the fusion.

- **Dimer model at beta = 0 is a physical DKC system (pp.51-52):** State space decomposes into B and T modules. Continuum limit is log CFT at c = -2.

## Surprises

- **l_0 = 0 at beta = 0:** Not "becomes small" — it IS zero. The trivial representation doesn't exist. The whole vacuum sector is gone.

- **Induction is not left-exact:** Adding crossings (induction) is fundamentally more complex than removing them (restriction).

- **Weaving looks like braiding (Section 5.2):** The AR quiver construction by interleaving tau-orbits visually resembles a braid diagram. Self-reference.

---

# Paper: [Gainutdinov, Jacobsen, Read, Saleur, Vasseur 2013] "Logarithmic Conformal Field Theory: a Lattice Approach"
**File:** 1303.2082v2.pdf (research-ingress/)
**arXiv:** 1303.2082v2, review article (~40 pages)

## Paths Not Taken

- **Dense polymer fusion not worked out explicitly (Section 8.2, pp.31-32):** The c->0 catastrophe is done for percolation (p=3) but NOT for dense polymers (p=2) — which is DKC's operating point. Working this out would directly describe braid composition at delta=0.

- **Classification of c=0 LCFTs stated as open (p.8):** "There is most likely an infinity of such theories."

- **Blob algebra -> Verma modules (Section 7.3, pp.26-27):** Gives access to FULL Verma modules (not Kac quotients). Dramatically richer. Computational implications not discussed.

## Technical Machinery

- **c=0 catastrophe mechanism (Section 3.2, pp.6-7):** OPE has 1/c divergence. FORCES introduction of logarithmic partner t(z). Result: L_0|t> = 2|t> + |T> (Jordan cell), <T|T> = 0 (null state). The "b" parameter is NOT determined by conformal symmetry — extra data.

- **Lattice fusion = induction (Section 8.1, pp.29-30):** Fusion of TL modules defined as: M_1 x_f M_2 = TL_{N1+N2} tensor product. At roots of unity: produces tilting modules and indecomposable structures. Fusion IS braid composition.

- **Indecomposability parameter from finite lattices (Section 7.2, pp.23-25):** Concrete algorithm for measuring "how logarithmic" a system is. Bulk b=-5 for percolation found numerically BEFORE theoretical prediction.

## DKC Overlaps

- **CRITICAL: Dense polymers IS DKC's operating point.** q = i, n = 0, c = -2. The SIMPLEST root-of-unity case, free fermion point, exactly solvable.

- **Entanglement vanishing = null states.** At c=0, stress-energy tensor T has <T|T> = 0. DKC's non-interleaving braids giving bracket=0 may be null states. Computational content might live in LOGARITHMIC PARTNERS — objects DKC hasn't defined yet.

- **Crossing depth ~ Jordan cell rank.** In bulk (periodic) case, rank is UNBOUNDED. Deeper crossings = higher-rank Jordan cells = more computational power.

- **MVN activation ~ fusion rules at roots of unity.** Composing braids doesn't give clean direct sum — gives tilting modules where outputs are non-separably entangled.

- **Axiality ~ free fermion solvability.** Delta=0 is the free fermion point with enhanced SU(2) symmetry that might explain bracket values constrained to axes.

## Surprises

- **DKC operates at the simplest solvable point.** Dense polymers is the ONE root-of-unity case with a free LCFT. Either deep clue or means interesting computation lives at harder points.

- **The "catastrophe" is a construction principle.** The 1/c divergence FORCES logarithmic partner structure. Degeneracy is not a limitation — it's a feature.

- **Numerical lattice results predicted theory.** Finite-system computations discovered structure before theory. Parallel to DKC demos.

- **Wall Brauer algebra connection at q=i (Section 7.4, p.28).** Additional algebraic structure beyond TL at DKC's operating point, from a different classical group family.

---

# Paper: [Rasmussen 2004] "Khovanov homology and the slice genus"
**File:** 0402131v1.pdf
**arXiv:** math/0402131v1

## Paths Not Taken

- **s = 2tau conjecture (p.2):** Later disproved (Hedden-Ording 2008), but near-agreement for small knots unexplained.
- **Homological width 2 -> s = sigma? (p.14):** Open. All 22 knots where s != sigma in <=11 crossings have width 3.
- **Functoriality of cobordism maps (p.10):** Later proved and structurally important.
- **Integer vs rational coefficients (p.2):** Lee's theory requires Q; fails over Z or F_2. Ground ring fundamentally matters.

## Technical Machinery

- **Lee's TQFT deformation (pp.4-5):** Khovanov: m(v_- tensor v_-) = 0. Lee: m'(v_- tensor v_-) = v_+. Basis change a = v_- + v_+, b = v_- - v_+ diagonalizes everything. Kh'(L) trivially rank 2^n. q-grading broken to filtration. s lives in the spectral sequence from filtration.

- **Canonical generators via orientation (p.5):** If two circles share a crossing, they have DIFFERENT labels. Coherence constraint.

- **Cobordism decomposition (pp.9-12):** Reidemeister = degree 0, 0-handles/2-handles = degree +-1, 1-handles = degree -1. Total: phi_S filtered of degree chi(S).

## DKC Overlaps

- **Non-semisimplicity = computational resource.** Lee's m(v_- tensor v_-) = 0 is a nilpotent/non-semisimple structure. The interesting invariant s measures the "memory" of the singular structure persisting into the generic regime via filtration.

- **Cube of resolutions = state sum.** {0,1}^k cube identical to Kauffman bracket state sum. DKC's computational analysis of states has a direct categorified shadow.

- **Writhe is a first-class citizen.** q-grading includes writhe n_+ - n_-. For positive knots, s(K) = writhe - Seifert circles + 1.

- **Crossing changes as bounded units.** Each crossing change shifts s by at most 1. Crossings are atomic units of complexity.

- **Coherent labels at crossings.** Two circles sharing a crossing always get different labels (a vs b). Binary complementarity echoes axiality theorem.

## Surprises

- **Lee's deformation kills everything but remembers the right thing.** Data is not in the answer but in HOW you get to the answer. Parallel to DKC: bracket values are simple but the PATTERN encodes Boolean computation.

- **The a/b basis de-quantizes the TQFT.** The deformation turns quantum (non-diagonal, interference) into classical (diagonal). All complexity lives in the spectral sequence between them.

---

# Paper: [Witten 1989] "Quantum Field Theory and the Jones Polynomial"
**File:** jones_polynomial_witten.pdf
**Journal:** Comm. Math. Phys. 121, 351-399

## Paths Not Taken

- **Non-compact gauge groups (p.353):** Entire construction depends on compactness. Non-compact G never explored. The finite/infinite DKC dichotomy might formulate as compactness question.
- **Heegaard splittings as alternative to surgery (p.360-361):** Different computational framework (inner product vs sum over reps). Complexity comparison never done.
- **Rational surgery deferred (p.366-367):** Computational cost of reducing rational to integer surgery unanalyzed.

## Technical Machinery

- **Physical Hilbert space one-dimensional on S^2 (Section 2, p.353-358):** THE key technical fact enabling all computations. Connected sums factor, surgery formula works.
- **Surgery formula (Section 4, p.371-380):** Z(M) = sum_i S_0^i Z(S^2 x S^1; R_i, Wilson lines). FINITE sum (i = 0,...,k). Theory exactly solvable.
- **Framing anomaly (p.369-370, eq. 3.17):** Under framing change by n: Z picks up phase exp(2*pi*i*n*h_R). Root-of-unity phase rotation = Z[zeta] structure.

## DKC Overlaps

- **Quantum dimension singularity (p.355-356):** Delta = 0 corresponds to [2]_q = 0. In Witten's framework, this is level k=0 — maximally degenerate (only trivial representation). Standard TQFT considers it "empty." DKC extracts rich computational content. Fundamental tension.

- **Writhe as computational resource (p.357-358, eq. 3.17):** Witten is EXPLICIT that raw CS observable is framing-dependent. Jones normalizes it away. The writhe phase is a root-of-unity rotation — exactly the Z[zeta] structure DKC exploits.

- **Finite vs infinite representation dichotomy (p.355-356):** At roots of unity, representation ring truncates to finitely many integrable reps. At generic q, infinite. Directly parallels binary octahedral (24 elements) vs infinite groups.

## Surprises

- **DKC operates at the "zero" of TQFT.** The computational content may live entirely in what Witten calls the "framing anomaly" — the thing physicists subtract.

- **Verlinde's conjecture falls out in a few lines from 3d surgery.** The 3d viewpoint is more fundamental than 2d.

---

# Paper: [Kirby 1978] "A Calculus for Framed Links in S^3"
**File:** kirbycal.pdf
**Journal:** Inventiones Math. 45, 35-56

## Paths Not Taken

- **Rational surgery not pursued (p.36):** Would introduce denominators — potentially ruinous for discrete computation.
- **Bounded case deferred (p.36-37):** Tangles = "open circuits" in DKC.
- **Computability of equivalence left open:** No bound on stabilizations needed. Undecidability shadow.

## Technical Machinery

- **Cerf theory (Section 2, pp.38-43):** Critical point interactions = births/deaths (stabilization) + crossings (handle slides). 1-parameter family IS the sequence of Kirby moves.
- **Linking matrix (Section 4, pp.48-52):** Captures all algebraic info. Transforms by elementary row/column ops. Classification = symmetric bilinear forms over Z mod stable equivalence.
- **Crossing<->component compiler (Section 5, pp.52-55):** ANY crossing replaced by +-1 framed unknot. Crossing depth trades for component count. Fully reversible "gate decomposition."

## DKC Overlaps

- **Framing = writhe is essential (entire paper).** Kirby says writhe DETERMINES the manifold. Jones normalizes it away. Kirby proved this 8 YEARS before Jones. The field followed Jones. Computational content was hiding in discarded data.

- **Crossing depth <-> component count:** k crossings = unknot + k unit-framed unknots. Computational content decomposes into unit contributions. Startlingly close to gate decomposition.

- **Handle slides are non-commutative.** Generator order matters — exactly like braid groups.

## Surprises

- **Mod-16 periodicity via Rohlin invariant** connects to mod-8 periodicity at zeta_8. Relationship unexplored.
- **The cobordism witness is discarded.** Compilation path may carry computational content of its own.

---

# HIGH DKC RELEVANCE

---

# Paper: [Davies, Juhasz, Lackenby, Tomasev 2022] "The Signature and Cusp Geometry of Hyperbolic Knots"
**File:** 2111.15323v3.pdf (DeepMind + Oxford)

## Paths Not Taken

- **Heegaard Floer -> hyperbolic geometry connection explicitly abandoned (p.24).** Found tau, nu, epsilon correlated with sigma but didn't pursue.
- **Chern-Simons invariant appeared in scatter plots (p.24).** No further discussion. CS = phase of Jones at roots of unity.
- **Natural slope may have quantum interpretation via Levine-Tristram at roots of unity.** Nobody explored.

## DKC Overlaps

- **Writhe/twist parallel:** Natural slope (measuring twist) controls signature. Geometric analog of DKC's writhe finding. Both say twisting data that Jones discards is load-bearing.
- **Crossing depth -> signature growth:** sigma grows linearly with twist; coefficient is SQUARE of linking number. Linking number quadratically amplifies signature.
- **Odd/even parity in correction terms:** Refined theorem uses ONLY geodesics with ODD linking number. Even contributes nothing. Unexplained parity effect echoes DKC parity as hardest Boolean function.

## Surprises

- **ML discovered the theorem.** Feature attribution pointed to Re(mu). Meta-parallel with DKC computational exploration.
- **Signature of random knot ~ sqrt(crossing number).** Diffusion process, not linear growth.

---

# Paper: [Craven, Hughes, Jejjala, Kar 2022] "Learning knot invariants across dimensions"
**File:** 2112.00016v2.pdf

## Paths Not Taken

- **SU(N) instanton Floer with level-rank duality (p.14):** Could recover full Z-grading. Unexplored.
- **All-pairs invariant correlation scan (p.30):** Proposed but not executed. Systematic map of which invariants "know about" others.
- **Knot-quiver correspondence (p.31):** Mentioned only in passing.

## DKC Overlaps

- **Jones at ANY root of unity (k=3-10) predicts s and g with >93-97% accuracy.** Small k works nearly as well as large k. Directly relevant to DKC's interest in specific small roots.
- **Raw Jones polynomial already predicts s at ~95%.** Information is in the polynomial structure itself, not just special evaluations.
- **5_1 and 10_132 have same Jones but different s.** Jones normalization (removing writhe) loses information.
- **Simple 2-layer networks suffice.** Underlying relationship is low-dimensional — consistent with 24-element group capturing essential content.

## Surprises

- **"What is four-dimensional about the Jones polynomial?"** Central mystery. 2D-defined invariant predicts 4D topology at >95%.
- **Kh(q, -q^{-2}) works BETTER than theoretically predicted Kh(q, -q^{-4}).** No known explanation. Novel relationship.
- **Witten's construction is not proven to equal Khovanov homology (p.10, fn.12).**

---

# Paper: [Piergallini & Zuddas 2018] "On branched covering representation of 4-manifolds"
**File:** 1602.07459v5.pdf

## Paths Not Taken

- **Degree 3 question (p.1-2):** Whether degree can be reduced below 4. Suspect NOT possible. Possibly optimal.
- **Lefschetz fibration connection (p.2):** Relating branched coverings to Lefschetz fibrations — monodromy in mapping class groups vs symmetric groups.
- **Covering Kirby calculus (p.4-6):** Completeness result but NOT a practical calculus. Future work.

## DKC Overlaps

- **Order-24 coincidence:** S_4 = 24 elements. Binary octahedral = 24 elements. Both are hard ceilings. Related via cube/octahedron symmetries.
- **Complexity/minimality tradeoff:** Lower degree -> more complex branch surface. Fewer crossings -> less computational power.
- **Two singularity types suffice.** Cones and nodes only. Parallels DKC's two crossing types computing all Boolean functions.
- **Writhe structure:** Covering distinguishes positive/negative crossings. NOT writhe-invariant.
- **"Borrow-and-return" technique:** Temporary self-intersections for relabeling workspace, then removed. Structurally analogous to ancilla qubits.
- **Branch surface complexity as undeveloped invariant.** Minimum complexity for a given 4-manifold never computed.
- **Ribbon/non-ribbon distinction:** Ribbon = no singularities needed. Could connect to trivial/non-trivial braid distinction.

---

# Paper: [Iori & Piergallini 2002] "4-manifolds as covers of the 4-sphere branched over non-singular surfaces"
**File:** 0203087v2.pdf / gt-v6-n1-p13-p.pdf (arXiv + journal versions)
**Journal:** Geometry & Topology, Vol. 6 (2002) 393-401

## DKC Overlaps

- **S_4 monodromy (24 elements) = binary octahedral order.** The S_4 <-> octahedral correspondence is the SAME group-theoretic structure.
- **Disjoint-support node condition = entanglement vanishing.** Singularity exists BECAUSE sheets DON'T entangle. Non-interleaving = node singularity.
- **Stabilization 4->5 parallels k=4->k=6.** Trivial fifth sheet enables previously blocked operations.
- **sigma(M) = -F*F/2 as writhe analog.** Irreducible lower bound that cannot be normalized away.
- **Cobordism as computation.** Create-operate-cancel = quantum circuit pattern.

## Surprises

- **Connected part can always be made unknotted.** ALL topological information in the "trivial" 2-sphere components and their labels. Interesting part is trivial; trivial parts carry information.
- **Non-orientable branching for oriented manifolds.** Counterintuitive but correct.

---

# MEDIUM DKC RELEVANCE

---

# Paper: [Hilden 1974] "Every Closed Orientable 3-Manifold is a 3-Fold Branched Covering Space of S^3"
**File:** 1183536038.pdf
**Journal:** Bulletin of the AMS, Vol. 80, No. 6

2-page announcement. Every closed orientable 3-manifold = irregular 3-fold branched covering of S^3, branch set = single KNOT (not link).

## DKC Overlaps

- **Knots as universal 3-manifold encoders.** Supports DKC thesis that knots are computationally rich.
- **3 is special.** Monodromy to S_3, smallest non-abelian symmetric group.
- **Irregularity essential.** Regular coverings NOT universal. Asymmetric group structure drives computation.
- **Branch set connected.** Single crossing pattern suffices for universality.

## Surprises

- **1974 predates Jones polynomial by a decade.** Encoding existed before the decoder.

---

# Paper: [Ozsvath & Szabo 2004] "Holomorphic disks and genus bounds"
**File:** 0311496v3.pdf
**Journal:** Geometry & Topology 8, 311-334

## Paths Not Taken

- **Tight contact structure generalization (p.314):** Explicitly flagged.
- **Elementary combinatorial proof of Morse-theoretic genus (p.316):** "It would be interesting to find a more elementary proof."

## DKC Overlaps

- **Kauffman states connection (p.316):** Almost hidden in one sentence — simultaneous trajectories on Heegaard diagram "can be identified with the Kauffman states." The genus formula is secretly about Kauffman states. Nobody followed up.
- **L-spaces as "computationally trivial" (Theorem 1.4):** Minimal HF rank, no taut foliations. Parallels finite group capacity ceiling.
- **Zero-surgery captures genus:** ZERO framing (no writhe correction) is the right surgery for genus detection.

## Surprises

- **Kauffman states remark is almost hidden.** One sentence on p.316 connecting HF to bracket world. Not developed.

---

# Paper: [Barbensi, Celoria, Harrington, Stasiak, Buck 2020] "Grid diagrams as tools to investigate knot spaces and topoisomerase-mediated simplification of DNA topology"
**File:** sciadv.aay1458.pdf + aay1458_sm.pdf
**Journal:** Science Advances 2020

## DKC Overlaps

- **Crossing depth <-> "hookedness" as computational resource.** Local geometry at crossing determines simplification direction. More hooked = more simplifying. Parallels crossing depth.
- **Knot space as directed graph.** Under hooked juxtapositions, network becomes DIRECTED. Asymmetric fluxes. Resembles MVN activation gating.
- **Grid diagram <-> Kauffman bracket (unexploited).** They use Jones only as classifier. Never evaluate at specific roots. Significant gap.

## Surprises

- **2D grid diagrams produce MORE knotting than 3D models.** Counterintuitive.
- **Knot fauna dramatically richer at GN 9-16.** Possible phase transition analog to DKC's four-tier structure.

---

# Paper: [Bar-Natan, Bar-Natan, Halacheva, Scherich 2024] "Yarn ball knots and faster computations"
**File:** s41468-023-00144-7.pdf
**Journal:** J. Applied and Computational Topology 8:175-192

## Paths Not Taken

- **Whether Jones, Alexander, HOMFLY-PT are C3D is COMPLETELY OPEN (p.180).**
- **Hyperbolic volume conjectured S3D but unproven (p.180).**

## DKC Overlaps

- **Crossing depth = computational hierarchy.** Type-d invariants: O(V^d) in 3D. Finite type hierarchy = DKC crossing depth hierarchy.
- **V^{4/3} threshold.** Projecting 3D to 2D inflates complexity by 4/3 power.
- **8 crossing types preserve writhe data.** Work with RAW crossing info, not Jones-normalized.

## Surprises

- **3D advantage from ORGANIZATION, not 3D crossings.** Vertical blue lines never participate in crossings. Third dimension = decomposition structure only.

---

# Paper: [Ozsvath, Stipsicz & Szabo 2015] "Grid Homology for Knots and Links"
**File:** GridHomologyBook.pdf (~410 pages, read strategically)

## Paths Not Taken

- **Khovanov-Floer relationship (p.7):** "A precise mathematical relationship... has yet to be discovered." Major open problem.
- **Algorithmic unknot detection (p.339-340):** Grid homology detects unknot but no combinatorial algorithm exploits this.
- **Cobordism maps from grids (p.341-342):** Non-combinatorial part of theory.

## DKC Overlaps

- **State-sum parallel:** Bracket: 2^c states (binary choice per crossing). Grid: n! states (permutations). Both state sums but grid gives chain complex.
- **Exotic R^4 from combinatorics (Section 8.6):** If DKC bracket relates to tau, Boolean computation connects to exotic structures.
- **K(B,k) family blind spots (Section 9.5):** Grid homology can't distinguish these. If bracket can, DKC is complementary.
- **Two skein sequences, not one (Ch 9-10).** Unoriented one has NO decategorified analog. Categorification-only structure.

## Surprises

- **n! generators but polynomial-time Euler characteristic.** Massive cancellation. Same as bracket state sum.
- **Empty rectangles = holomorphic disks.** Analysis = combinatorics in this setting.

---

# LOW DKC RELEVANCE

---

# Paper: [Bartholomew 2011] "An Application of Vogel's Algorithm"
**File:** research-ingress/vogel.pdf

Purely algorithmic paper. Converts link diagrams to braid words using Vogel moves.

## DKC Overlaps

- **Crossing count grows under braiding.** Each Vogel move adds 2 crossings. "Overhead" crossings in braid representation.
- **Virtual crossings as non-interleaving.** Virtual = computationally trivial crossings.
- **Seifert circles topological, crossings algebraic.** Clean separation: skeleton first, computation second.

---

# Paper: [Goldstein-Rose 2017] "Introduction to Knots and Braids Using Seifert Circles"
**File:** research-ingress/GoldsteinRose.pdf

Undergraduate expository paper. No DKC-relevant content. Standard material only.
