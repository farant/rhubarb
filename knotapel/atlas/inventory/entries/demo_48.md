## Demo 48: Forward DKC Zoo

- **Status**: COMPLETE
- **File**: `demo_48_forward_dkc_zoo/main.c` (~1078 lines)
- **Tests**: 14/14 pass, 7/7 predictions confirmed
- **Depends on**: Demo 29 (forward DKC for XOR, Cyc8 arithmetic, bracket catalog), Demo 45 (complex function zoo, topology frequency, reverse RMS data), Demo 47 (parity investigation, NPN classification, MVN activation findings)
- **Feeds into**: Demo 39 (symmetry decomposition — algebraic explanation of parity wall), Demo 49 (neglecton weights), Demo 50 (parity reachability at higher k)

### Headline
Exhaustive search of 100 million exact Z[zeta_8] bracket quartets proves XNOR3 (parity) is the ONLY 3-input Boolean function with zero valid forward DKC constructions — the wall is razor-thin (1 Hamming bit from EXACT1) yet categorically impassable.

### Key Results
- **Parity wall CONFIRMED**: XNOR3 has exactly 0 solutions out of 100,000,000 quartets; all other 12 NPN classes have solutions (127K to 25M)
- **Four-tier hierarchy**: AND-type (>10M) > intermediate (1-4M) > boundary (127K-371K) > unreachable (0)
- **Triple correlation**: Forward solution count correlates with Demo 45 topology frequency (rho = +0.78), anti-correlates with reverse RMS (rho = -0.79), weakly correlates with orbit size (rho = +0.47)
- **Wall thickness = 1 Hamming bit**: EXACT1 (tt=0x16) is distance 1 from parity and has 127K solutions — the algebra gets within one bit flip but cannot cross
- **XOR2 vs XNOR3**: 2-input XOR has 1.6M solutions; 3-input parity has 0 — the 2→3 barrier is absolute
- **Constants**: 22% of quartets produce constant functions (FALSE=17.3M, TRUE=4.7M) — large-magnitude bracket values saturate sigmoid
- **Unit-magnitude observation**: All non-trivial computations use low-magnitude catalog values (|z| = 1-2), confirming the computational core lives near the unit circle
- **Three topo_freq=0 classes have solutions**: ISOLATE (371K), XOR2 (1.6M), A^(B|C) (2.3M) — quartet combinatorics is strictly more powerful than single-braid enumeration

### Theorems/Conjectures
- **Parity unreachability (split-sigmoid, Z[zeta_8])**: A single complex neuron with split-sigmoid activation and weights drawn from the Z[zeta_8] bracket catalog CANNOT compute 3-input parity — PROVEN (exhaustive, 100M quartets)
- **Bracket self-consistency**: Forward construction, forward enumeration, and reverse decomposition produce the same preference ordering over Boolean functions — CONFIRMED (rho = +0.78, -0.79)
- **Non-semisimplicity explanation**: The parity wall is the Clifford boundary — bracket evaluation factors through TL/radical (semisimple quotient), killing exactly the degrees of freedom needed for parity — CONJECTURED (algebraic proof deferred to Demo 39)
- **Neglecton prediction**: TL_4 radical elements (neglecton weights) should enable parity computation — CONJECTURED (tested in Demo 49)
- **Distance-solution anti-correlation**: Functions closer to parity in Hamming distance have fewer solutions — CONFIRMED empirically

### Data
- Catalog: 100 distinct Z[zeta_8] bracket values from 2-3 strand braids (length 1-8)
- Search space: 100^4 = 100,000,000 quartets (w1, w2, w3, bias)
- Solution counts by NPN class:
  - AND2' (0x03): 25,085,227 (32.2%)
  - BUF (0x0F): 16,711,816 (21.4%)
  - ~A~(BC) (0x07): 13,574,170 (17.4%)
  - AND3' (0x01): 10,108,257 (13.0%)
  - 3v-0x19 (0x19): 3,843,271 (4.9%)
  - A^(B|C) (0x1E): 2,263,901 (2.9%)
  - 3v-0x1B (0x1B): 1,690,752 (2.2%)
  - XOR2 (0x3C): 1,613,217 (2.1%)
  - ~A(B^C) (0x06): 1,402,076 (1.8%)
  - MAJ' (0x17): 1,160,474 (1.5%)
  - ISOLATE (0x18): 371,435 (0.5%)
  - EXACT1 (0x16): 126,666 (0.2%)
  - XNOR3 (0x69): 0 (0.0%)
- Constants: FALSE=17,299,057, TRUE=4,749,681 (22.0% total)
- Spearman correlations: rho(sol,topo)=+0.7830, rho(sol,RMS)=-0.7912, rho(sol,orbit)=+0.4698
- Parity wall min Hamming distance: 1 (EXACT1 → XNOR3)
- Demo 45 reference data hardcoded for cross-correlation

### Code Assets
- **Cyc8 exact arithmetic**: full Z[zeta_8] ring (add, sub, mul, neg, conj, pow_int, eq, to_cx)
- **Braid bracket oracle**: both exact (Cyc8, delta=0 optimized) and float (Cx, general) implementations
- **Dual-storage catalog builder**: exact + float for each distinct bracket value
- **NPN classifier**: brute-force 96-transform canonicalization, orbit size computation, 13 non-trivial class identification
- **Split-sigmoid activation**: h = sig(Re) + i*sig(Im), p = h_r*(1-h_i) + (1-h_r)*h_i
- **Exhaustive quartet search**: O(n^4) with precomputed partial sums (s12 outside w3 loop), example quartet storage per class
- **Parity distance tracker**: Hamming distance LUT to 0x69/0x96, global minimum tracking
- **Spearman rank correlation**: handles ties, reused from Demo 47
- **Master comparison table**: solution count vs topology frequency vs reverse RMS vs orbit size

### Literature Touched
- Aizenberg (2007, 2011): single MVN computes parity with arbitrary complex weights — proves the wall is in the lattice, not the neuron
- Non-semisimple TQFT / neglectons (2025 Nature Communications): radical of TL at root of unity contains degrees of freedom for universality — the bracket evaluation kills exactly these
- Habiro: quantum topology, non-semisimple invariants
- Nazer-Gastpar: lattice codes (connection via Z[zeta_8] as integer lattice)
- Abramsky: categorical QM + TL non-semisimplicity
- **Novelty claim**: No existing work connects Kauffman bracket evaluation ↔ Boolean function computability boundaries ↔ neural network weight lattices (four-community intersection unoccupied)

### Open Questions
- Can Demo 39's TL_4 block decomposition explicitly identify which blocks are "dead" at delta=0 and show they correspond to parity?
- Do neglecton (radical) elements as weights enable parity? (→ Demo 49)
- Is the four-tier structure (>10M / 1-4M / <500K / 0) invariant under catalog size scaling?
- Why does EXACT1 (distance 1 from parity) survive as reachable — what algebraic property distinguishes it from the one extra bit flip needed for parity?
- Does the unit-magnitude observation mean that a smaller catalog restricted to |z| ≈ 1 would preserve all non-constant computational classes?
