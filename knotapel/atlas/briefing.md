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

## Current Frontier

Most recent landmarks: Demos 64-71 opened two major arcs. **The wall arc** (D64-65): the
parity hierarchy is a funnel-shaped matroid with self-doubling as the dominant mechanism;
the ceiling n=5 is algebra-independent (identical at Z[zeta_8] and Z[zeta_16], gap-of-2
REFUTED); but generalized activations break the convention layer at k=24 for XOR6. **The
quaternionic-spherical arc** (D66-71): SU(2) braid representations produce the 24-cell
polytope (24 quaternions); the computation lives on S² (eigenvector direction), NOT S¹ or
S³; the Hopf phase carries zero computational information; a custom 13-direction S² Voronoi
achieves 36 XOR6 solutions at only 14 cells; and the 13=13 theorem (D71) explains why:
bandwidth l=6 because 2×6+1 = 13 = number of eigenvector directions. Three formal proofs
filed (P01-P03). 59 demos total.

**Next (Tier 1 priorities):**
1. **Analytical proof of the 13=13 bandwidth bound** -- prove l=⌈(N-1)/2⌉ for N-point
   binary Voronoi on S². Cleanest standalone publication target. Unlocks Paper 5.
2. **XOR7 on S²** -- D65 showed XOR7 needs k=127 sectors on S¹. Does S² reduce this
   dramatically (as it did for XOR6: k=24→14 cells)? Tests spectral framework scaling.
3. **Sandwich theorem formal proof** -- radical dimension formula PROVEN; remaining:
   algebra isomorphism, nilpotency=3, Fibonacci rank. Small-medium effort. Unlocks Paper 3.

## Deeper Reading (all paths relative to `knotapel/`)

- Research narrative: `atlas/synthesis/narrative.md` | Four-lens analysis: `atlas/synthesis/four-lenses.md`
- Cross-demo connections: `atlas/synthesis/connections.md` | Novelty: `atlas/synthesis/novelty.md`
- Demo inventory: `atlas/inventory/demo-index.md` | Theorems: `atlas/inventory/theorems.md` | Proofs: `atlas/inventory/proofs-index.md`
- Literature: `atlas/research/literature-index.md`
- Next priorities: `atlas/planning/next-priorities.md` | Future demos: `atlas/planning/future-demos.md`
- Open questions: `atlas/planning/research-questions.md` | Vision: `atlas/planning/vision.md`
