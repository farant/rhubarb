## Demo 07: Braid Closure Bridge

- **Status**: COMPLETE
- **File**: `demo_07_braid_closure/main.c` (~791 lines)
- **Tests**: 25/25 pass (compiles clean with all strict flags including -Werror)
- **Depends on**: Demo 01 (PD notation + state-sum bracket), Demo 02 (braid bracket via state-sum), Demos 02-06 (matrix pipeline context)
- **Feeds into**: Demo 08 (reverse direction: PD -> braid word via Vogel's algorithm)

### Headline
Bridges the two separate computation worlds (PD state-sum and braid word) by converting braid words to PD notation via closure, proving both representations compute identical Kauffman brackets.

### Key Results
- `braid_to_pd()` generates valid PD notation from braid words with correct arc counts for all test knots/links
- **Exact bracket match**: PD state-sum bracket from generated PD == braid bracket for all 5 test cases (trefoil 2-strand, figure-eight, Hopf, trefoil 3-strand, unknot)
- Seifert circle count = braid strand count for all braid closures (theorem confirmed computationally)
- Seifert algorithm works universally on both braid-generated PD and Demo 01's hand-coded PD
- Link component count from PD matches braid permutation cycle structure
- Discovery: sigma_1 sigma_2 sigma_1 on 3 strands is a 2-component link (not a trefoil)
- Discovery: sigma sigma^{-1} closes to 2-component unlink

### Theorems/Conjectures
- **Confirmed**: Braid closure PD bracket = braid state-sum bracket (the two worlds agree exactly)
- **Confirmed**: Seifert circles from PD = strand count for braid closures
- **Confirmed**: Link components from PD = permutation cycle count of braid word
- **Noted**: PD chirality bug — "obvious" counter-clockwise labeling gives mirror bracket; fix is [NE, SE, SW, NW] for positive crossings

### Code Assets
- `braid_to_pd()`: core bridge function — braid word to PD notation via closure. Tracks per-position crossing lists, assigns arc labels between consecutive crossings, builds PD with correct over/under conventions
- `seifert_circles()`: Seifert resolution on PD notation. Sign-dependent pairing (positive → B-smoothing, negative → A-smoothing). Universal across PD conventions
- `link_components()`: traces strands through crossings via (p+2)%4 exit rule. Counts topological components
- `pd_bracket()`: Demo 01's state-sum bracket, included for cross-validation
- `braid_bracket()` + `braid_loops()`: Demo 02's braid state-sum (union-find based), included for cross-validation
- Reusable `Poly` Laurent polynomial ring (standard copy from Demo 01)
- PD types (`Xing`, `Knot`) with sign tracking

### Literature Touched
- Braid closure (Alexander's theorem): every link is a closed braid
- Seifert circles and Seifert surfaces
- PD notation conventions (under/over strand labeling, chirality sensitivity)
- Link component counting from diagram data
- Vogel's algorithm (mentioned as Demo 08 direction)

### Open Questions
- Round-trip: can we go PD → braid word (Vogel's algorithm) and back? (Deferred to Demo 08)
- Strands uninvolved in any crossing are silently skipped in `braid_to_pd()` — is this correct for all cases?
