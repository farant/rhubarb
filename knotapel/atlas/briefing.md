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
| 4 | Four-tier NPN hierarchy: k=2(5 classes), k=3(+1), k=4(+6), k=6(+1=parity) | 50 | PROVEN |
| 5 | 11/13 half-plane theorem: 0x06 and 0x1B unreachable; clean analytical proofs | 61-62 | PROVEN |
| 6 | Complex Minsky-Papert: k=2 half-plane cannot compute 3-input parity, any weights | 49 | PROVEN |
| 7 | Sandwich theorem: rad^2(TL_{2k}(0)) isomorphic to TL_{2k-1}(0) | 51 | VERIFIED |
| 8 | Catalan trace theorem: nonzero TL traces are Catalan numbers; window formula | 51 | VERIFIED |
| 9 | Fibonacci rank: bilinear rank of rad^2 generator = F(ell-1) across ell=3..7 | 52,60 | VERIFIED |
| 10 | Axiality: every bracket at delta=0 is axial in Z[zeta_8] (131K braids, 0 exceptions) | 35 | VERIFIED |
| 11 | Triskelion: 3-input parity needs 3 odd sectors at 120-deg, octants {2,4,5,7} only | 50 | CONJECTURED |

## Current Frontier

Most recent landmarks: The radical dimension formula rad(TL_ℓ) = 2ℓ−3 is now **formally
proven** (`proofs/radical-dimension-formula.md`) via Graham-Lehrer cellular theory — V_{ℓ-2}
is the unique degenerate cell module with corank 1. Demos 61-62 proved the 11/13 theorem
analytically. Demo 60 confirmed all radical formulas at ell=7 (first cubic number field).
Demo 50 resolved the parity wall: the lattice always had what parity needed; split-sigmoid
could not.

**Next (Tier 1 priorities):**
1. **Sandwich theorem formal proof** -- radical dimension formula PROVEN; remaining:
   algebra isomorphism, nilpotency=3, Fibonacci rank. Small-medium effort. Unlocks Paper 3.
2. **4-input parity** -- does XOR4 require k=8 sectors? Tests triskelion generalization
   (k=2n). Medium effort. Second data point for a structural law. Unlocks Paper 1.
3. **Catalog completeness bound** -- prove the 100-value catalog contains all bracket
   values up to braid length L. Small effort. Closes the key reviewer objection for Paper 1.

## Deeper Reading (all paths relative to `knotapel/`)

- Research narrative: `atlas/synthesis/narrative.md` | Four-lens analysis: `atlas/synthesis/four-lenses.md`
- Cross-demo connections: `atlas/synthesis/connections.md` | Novelty: `atlas/synthesis/novelty.md`
- Demo inventory: `atlas/inventory/demo-index.md` | Theorems: `atlas/inventory/theorems.md` | Proofs: `atlas/inventory/proofs-index.md`
- Literature: `atlas/research/literature-index.md`
- Next priorities: `atlas/planning/next-priorities.md` | Future demos: `atlas/planning/future-demos.md`
- Open questions: `atlas/planning/research-questions.md` | Vision: `atlas/planning/vision.md`
