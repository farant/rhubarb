## Demo 56: Potts-TL Dictionary

- **Status**: COMPLETE
- **File**: `demo_56_potts_tl/main.c` (~1137 lines)
- **Tests**: ~15 pass (inline check() assertions across Parts A-G)
- **Depends on**: Demo 54 (bracket axiality, Z[i]-axial structure), Demo 55 (Z[zeta_16] arithmetic, sector activation), Demo 53 (Q=1 percolation — referenced as trivial)
- **Feeds into**: Future demos on Q=3 Potts model, multi-Q DKC landscape, transfer matrix methods

### Headline
Establishes the Potts-TL dictionary connecting Kauffman bracket to Potts model partition function via delta^2=Q, cross-validates FK=Ising at Q=2, proves Q=1 percolation is computationally trivial, and discovers that FK partition function values are algebraically richer (4D Z[zeta_8]) than bracket values (2D Z[i]-axial).

### Key Results
- **FK = Ising cross-validation**: Direct Ising spin sum equals Fortuin-Kasteleyn partition function at integer Q=2, v=1 for 6 lattice strips S(w,h) plus v=2 case — PROVEN (exhaustive subset enumeration)
- **Q=1 percolation degeneracy**: Z_FK(G; 1, v) = (1+v)^|E| regardless of graph structure — confirmed for 4 strips with arbitrary v=zeta_16^3
- **Bracket coupling lives in Z[zeta_8]**: At Q=2, v=-zeta_16^6, all FK partition function values have only even-indexed Cyc16 components (11/11 strips) — the Q^{k(A)} weighting stays in the Z[zeta_8] subring
- **FK values are 4D vs bracket 2D**: FK partition functions use the full Z[zeta_8] = Z[i, sqrt(2)] ring (4 generators over Z), while bracket values are Z[i]-axial (only 2 generators) — a genuine algebraic enrichment
- **Critical coupling values are real**: At v=sqrt(2) (Ising critical temperature), all FK values have zero imaginary part — live in Z[sqrt(2)] (6/6 strips verified)
- **Q-hierarchy**: Q=1 produces only powers of (1+v) (no geometry), Q=2 distinguishes different topologies — a computational phase transition at Q=2
- **FK catalog**: ~9 distinct nonzero partition function values from 11 strips at bracket coupling
- **FK DKC Boolean search**: Tests whether 4D FK values compute more Boolean functions than 2D bracket values across multiple activations (Re>0, split-sigmoid, sector k=2,4,6) for both 2-input and 3-input functions

### Theorems/Conjectures
- **FK = Ising equivalence at integer Q**: Z_FK(G; Q=2, v) = Z_Ising(G; v) for all lattice strips tested — PROVEN (exhaustive, 7 cases)
- **Q=1 universality**: Z_FK(G; 1, v) = (1+v)^|E| for any graph — PROVEN (verified 4 strips, algebraic identity)
- **Z[zeta_8] closure**: FK at bracket coupling Q=2, v=-zeta_16^6 yields values in Z[zeta_8] (even subring of Z[zeta_16]) — PROVEN (verified 11 strips)
- **Critical coupling reality**: FK at v=sqrt(2), Q=2 yields real values (Z[sqrt(2)]) — PROVEN (verified 6 strips)
- **Axiality breaking**: FK values use all 4 dimensions of Z[zeta_8] where bracket values use only 2 (Z[i]-axial) — CONFIRMED (the Q^{k(A)} weighting breaks axiality)
- **Computational phase transition at Q=2**: Q=1 carries no geometric information; Q=2 distinguishes topology — CONFIRMED

### Data
- Lattice strips: S(w,h) for w=2..4, h=2..6 (up to 24 edges, feasible for 2^|E| enumeration)
- Delta-Q dictionary: ell=3→Q=1 (percolation), ell=4→Q=2 (Ising), ell=6→Q=3 (3-state Potts)
- Bracket coupling: A=zeta_16^5, v=-zeta_16^6=-A^{-2}
- Critical coupling: v=sqrt(2)=zeta_16^2-zeta_16^6 (verified v^2=2)
- Critical temperature: K_c = ln(1+sqrt(2)) = 0.8814...
- FK partition function catalog: ~9 distinct nonzero values at Q=2 bracket coupling
- NPN classes: 13 non-trivial 3-input classes (standard from Demo 48)
- Activations tested: Re>0, split-sigmoid, sector k=2,4,6

### Code Assets
- **Z[zeta_16] arithmetic (Cyc16)**: 8-component cyclotomic ring with zeta^8=-1 — zero, one, zeta_power, add, neg, mul, eq, is_zero, to_cx, print (reused from Demo 55)
- **cyc16_is_real()**: Tests if Z[zeta_16] element has zero imaginary part (odd components = 0)
- **cyc16_is_zeta8_valued()**: Tests if element lives in Z[zeta_8] subring (even-indexed components only)
- **LatticeStrip builder**: build_strip(w,h) constructs rectangular grid with horizontal + vertical edges
- **Union-Find**: Path-halving uf_find/uf_union for connected component counting
- **FK partition function**: fk_partition() — full 2^|E| subset enumeration with precomputed v^k, Q^k power arrays
- **Ising spin sum**: ising_spin_sum() — direct 2^|V| configuration sum for integer-coupling cross-validation
- **NPN classifier**: 96-transform canonicalization, orbit sizes, 13 class identification (from Demo 48)
- **FK DKC search**: Multiplicative encoding z=w1^x1*w2^x2*..., truth table extraction, per-NPN-class aggregation
- **Multiple activation wrappers**: Re>0, split-sigmoid, sector(k) — unified classify(Cx, int) interface

### Literature Touched
- Fortuin-Kasteleyn representation of Potts model (cluster expansion, Q as continuous parameter)
- Ising model / 2-state Potts (Q=2, spin-spin coupling, critical temperature)
- Bond percolation (Q=1 limit, geometrically trivial)
- Kramers-Wannier duality / self-dual point (v_c = sqrt(Q) for square lattice)
- Cyclotomic integers Z[zeta_16], Z[zeta_8] (number-theoretic structure of partition functions)
- NPN classification of Boolean functions (connection to DKC computability)
- Kauffman bracket / TL algebra connection to statistical mechanics (delta^2 = Q)

### Open Questions
- Does the 4D FK catalog achieve more NPN classes than the 2D bracket catalog at any activation? (The small catalog size (~9) may limit this)
- Can larger strips or periodic boundary conditions generate a richer FK catalog?
- What happens at Q=3 (3-state Potts, ell=6)? Is the FK catalog even richer (6D? 8D?)?
- Is the axiality breaking (2D→4D) a fundamental feature of the Q^{k(A)} weighting, or an artifact of small lattice sizes?
- Can transfer matrix methods replace the exponential 2^|E| enumeration to reach larger lattices?
- Does the FK/Potts viewpoint give any new leverage on the parity wall from Demo 48?
