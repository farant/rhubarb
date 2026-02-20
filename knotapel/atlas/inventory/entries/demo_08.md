## Demo 08: Vogel's Algorithm (PD -> Braid Word)

- **Status**: COMPLETE
- **File**: `demo_08_vogel/main.c` (~887 lines)
- **Tests**: 47/47 pass
- **Depends on**: Demo 07 (braid -> PD direction), Demo 01 (PD bracket oracle, knot PD codes)
- **Feeds into**: completes the PD <-> braid round-trip; any later demo needing to convert arbitrary PD notation into braid words

### Headline
Compiler from PD notation to braid word via Seifert circle decomposition — completes the bidirectional PD/braid bridge with full bracket-preserving round-trip verification.

### Key Results
- Seifert circle membership correctly computed via union-find for trefoil (2 circles), figure-eight (3 circles), Hopf link (2 circles)
- Trefoil PD -> braid word [1,1,1] on 2 strands (sigma_1^3)
- Figure-eight PD -> braid word [1,-2,1,-2] on 3 strands
- Hopf link PD -> braid word [1,1] on 2 strands (sigma_1^2)
- Round-trip bracket match: pd_bracket(PD) == braid_bracket(pd_to_braid(PD)) for all three knots
- Double round-trip: Braid_1 -> PD_1 -> Braid_2 -> PD_2, brackets match at every stage
- Also tests trefoil on 3 strands (sigma_1 sigma_2 sigma_1) for double round-trip

### Theorems/Conjectures
- VERIFIED: Alexander's theorem in practice — Seifert circle count = braid width
- VERIFIED: PD <-> Braid conversion preserves Kauffman bracket (round-trip and double round-trip)
- VERIFIED: For standard small knots, Seifert graph is always a path graph (no Vogel moves needed)

### Data
- Trefoil: 6 arcs -> 2 Seifert circles ({0,2,4}, {1,3,5}) -> 2-strand braid
- Figure-eight: 8 arcs -> 3 Seifert circles ({0,4}, {1,3,5,7}, {2,6}) -> 3-strand braid
- Hopf link: 4 arcs -> 2 Seifert circles ({0,3}, {1,2}) -> 2-strand braid

### Code Assets
- `seifert_membership()`: union-find Seifert resolution — positive crossings use B-smoothing (0,1)(2,3), negative use A-smoothing (0,3)(1,2)
- `trace_circles()`: traces each Seifert circle through crossings to get cyclic crossing order
- `find_braid_order()`: brute-force cut-point search + Kahn's topological sort for consistent braid word ordering; skips same-generator constraints (they commute)
- `pd_to_braid()`: full 7-step PD -> braid compiler (Seifert membership -> crossing-circle map -> Seifert graph path check -> circle ordering -> crossing trace -> braid ordering -> emit generators)
- `braid_to_pd()`: carried from Demo 07 for round-trip tests
- Path-graph check: all vertices degree <= 2, exactly 2 leaves
- Returns error codes: -1 (not a path graph), -2 (no valid ordering found)

### Literature Touched
- Vogel's algorithm for PD -> braid conversion
- Alexander's theorem (every link is a closed braid)
- Seifert circles / Seifert surface construction
- Kahn's algorithm for topological sort

### Open Questions
- Only handles path-graph Seifert graphs — full Vogel moves (for non-path Seifert graphs) not yet implemented
- Could the brute-force cut-point search become expensive for knots with many crossings per circle? (currently bounded by MAX_X=20)
- No Markov stabilization — output braid may not be minimal
