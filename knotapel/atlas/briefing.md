# DKC Fast-Onboarding Briefing

## What Is This

Discrete Knotted Computation (DKC) compiles topological invariants -- Kauffman bracket
values evaluated at roots of unity -- into neural network weights that compute Boolean
functions without training. At A = e^{i*5pi/4}, delta = 0, and every bracket value is a
cyclotomic integer in Z[zeta_8] with exact arithmetic (zero floating-point error, ever).
We proved: (1) these exact bracket values compute XOR and all 13 NPN Boolean classes;
(2) the parity function is unreachable under split-sigmoid activation (0/100M quartets)
but reachable at k=6 MVN sectors (906 solutions); (3) two Boolean classes are provably
unreachable by ANY half-plane activation on multiplicative encodings (the 11/13 theorem).
The wall is in activation geometry (convexity), not the weight lattice. Nobody in knot
theory, neural networks, lattice coding, or TQC has connected these four fields before.
The four-pillar assembly: Abramsky (TL = computation) -> Habiro (bracket = cyclotomic
integer) -> Aizenberg (root-of-unity neurons = Boolean logic) -> Nazer-Gastpar (algebraic
lattices = exact computation). Each step is published; the intersection is unoccupied.

## Key Results

| # | Result | Demo | Status |
|---|--------|------|--------|
| 1 | Forward DKC: exact Z[zeta_8] bracket values compute XOR (100+ triples, RMS=0.000) | 29 | PROVEN |
| 2 | Parity wall: XNOR3 has 0/100M quartets under split-sigmoid; 0/2.18B with neglectons | 48-49 | PROVEN |
| 3 | Parity reachable at k=6 MVN sectors (906 solutions); all 13 NPN classes computable | 50 | PROVEN |
| 4 | Parity ceiling n=5 is algebra-independent: identical at Z[zeta_8] and Z[zeta_16] | 63,65 | PROVEN |
| 5 | Two-layer wall: absolute (k≤23, no labeling works) + convention (k≥24, custom works) | 65 | PROVEN |
| 6 | Generalized XOR6 at k=24, XOR7 at k=91 with non-standard sector labelings | 65 | PROVEN |
| 7 | DKC computation lives on S² (eigenvector direction); Hopf phase is inert | 67 | PROVEN |
| 8 | 24-cell polytope emerges from SU(2) braid rep; Voronoi beats geographic grids | 66 | PROVEN |
| 9 | The 13=13 theorem: S² bandwidth l=6 because 2l+1 = 13 = eigenvector directions | 71 | PROVEN |
| 10 | Parity hierarchy is a funnel-shaped matroid; self-doubling impossibility theorem | 64 | PROVEN |
| 11 | 11/13 half-plane theorem: 0x06 and 0x1B unreachable; clean analytical proofs | 61-62 | PROVEN |
| 12 | Radical dimension formula: rad(TL_ℓ) = 2ℓ−3 via Graham-Lehrer cellular theory | 51-52,60 | PROVEN |
| 13 | Next-level radical: rad(TL_{ℓ+1}) = ℓ²−ℓ−3; universal corank 1 at first degeneracy | 39 | PROVEN |
| 14 | Markov RT truncation: dim ker(B_M) = dim J(A) + Σ (dim L_j)² for j≥ℓ−1 | 39 | PROVEN |
| 15 | Direction Nesting: ζ_8 eigenvectors nest exactly in ζ_16 (rigidity, 0° drift) | 69 | PROVEN |
| 16 | Bracket and Voronoi cell are incomparable braid invariants; 119 combined classes | 74 | PROVEN |
| 17 | XOR8 solvable at ζ₈ with S¹×S² activation (6 winners / 10,626); wall was activation | 77 | PROVEN |
| 18 | ζ₈ finite group ceiling: XOR10=0 even with Sec×Vor; 24-element group exhausted | 78 | PROVEN |
| 19 | ζ₁₂ breaks wall: infinite group gives XOR10=124, XOR12=50+; capacity scales with catalog | 79 | PROVEN |
| 20 | Only ζ₄ and ζ₈ finite (ADE: Q₈ and E₇); quantum dimension [2]_q = 0 at ζ₈ | 80 | PROVEN |
| 21 | Linear depth law: max_xor ≈ depth+6; algebraic coherence beats raw vocabulary | 82 | DEMONSTRATED |

## Current Frontier

Most recent landmarks: Demos 72-82 opened two new arcs on top of the D64-71 foundation.
**The characterization arc** (D72-75): bracket and Voronoi cell are incomparable braid
invariants with 119 combined classes (D74); the angle channel subsumes the cell for additive
sums (D75); anti-correlation — worst spherical t-design gives MOST solutions (D72); 82.8%
universal automaton determinism (D73). **The scaling arc** (D76-82): XOR8=0 at ζ₈ with S²
Voronoi alone (D76) but XOR8=6 with S¹×S² combined activation (D77) — the wall was the
activation, not the root, the SAME discovery pattern as D50; ζ₈ finite group ceiling at
XOR10 (D78); ζ₁₂ BREAKS the wall with XOR10=124, XOR12=50+ (D79); only ζ₄ and ζ₈ generate
finite SU(2) subgroups — ADE classification, quantum dim [2]_q=0 at ζ₈ (D80); logarithmic
scaling 0.62 XOR per catalog doubling (D81); and the fundamental reinterpretation: max_xor ≈
depth+6 where depth = crossing number, algebraic coherence beats vocabulary (D82). Three
formal proofs (P01-P03). 70 demos total. Paper 6 proposed: "Crossing Depth, Group
Finiteness, and Capacity Scaling in DKC."

**Next (Tier 1 priorities):**
1. **ζ₁₂ with Sec×Vor activation** -- D79 showed ζ₁₂ breaks the ζ₈ wall; D77 showed
   Sec×Vor is far more powerful than sector alone. Combining them should dramatically
   increase XOR capacity. Immediate Paper 6 impact.
2. **ζ₃₂ finiteness test** -- D80's Power-of-Two Conjecture predicts ζ₃₂ finite. Single
   test_root(32) call. Small effort, high clarity for the ADE/finiteness story.
3. **Analytical proof of the 13=13 bandwidth bound** -- prove l=⌈(N-1)/2⌉ for N-point
   binary Voronoi on S². Cleanest standalone publication target. Unlocks Paper 5.
4. **Sandwich theorem formal proof** -- radical dimension formula PROVEN; remaining:
   algebra isomorphism, nilpotency=3, Fibonacci rank. Small-medium effort. Unlocks Paper 3.

## Deeper Reading (all paths relative to `knotapel/`)

- Research narrative: `atlas/synthesis/narrative.md` | Four-lens analysis: `atlas/synthesis/four-lenses.md`
- Cross-demo connections: `atlas/synthesis/connections.md` | Novelty: `atlas/synthesis/novelty.md`
- Demo inventory: `atlas/inventory/demo-index.md` | Theorems: `atlas/inventory/theorems.md` | Proofs: `atlas/inventory/proofs-index.md`
- Literature: `atlas/research/literature-index.md`
- Next priorities: `atlas/planning/next-priorities.md` | Future demos: `atlas/planning/future-demos.md`
- Open questions: `atlas/planning/research-questions.md` | Vision: `atlas/planning/vision.md`
