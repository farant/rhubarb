## Demo 11: Waveform Signatures

- **Status**: COMPLETE
- **File**: `demo_11_waveform/main.c` (~649 lines)
- **Tests**: ~16 pass (symmetry checks, greedy angle separation, cross-validation)
- **Depends on**: Demo 10 (unit-circle bracket evaluation, complex arithmetic, state-sum oracle)
- **Feeds into**: Demo 12 (selected optimal angles for classification)

### Headline
Sweeps the Kauffman bracket around the full unit circle (256 samples, 10 knots) to produce waveform signatures, proving chiral pairs share amplitude and greedy angle selection separates all knots.

### Key Results
- Chiral pairs (trefoil/mirror trefoil, torus(2,5)/cinquefoil mirror) have **identical amplitude waveforms** — only phase distinguishes them
- Amphichiral knot (figure-eight) has amplitude symmetric about theta=0: |f(theta)| = |f(-theta)|
- Mirror bracket relation verified: bracket_mirror(e^{i*theta}) = bracket(e^{i*(2pi-theta)})
- Greedy angle selection finds a small set of angles (<=8) that separates all 10 knots (threshold 0.001)
- Best single angle identified that maximizes minimum pairwise separation
- Hardest-to-distinguish pair identified across all angles
- Full pairwise separation matrix computed at best angle

### Theorems/Conjectures
- PROVEN (numerically): |bracket(A)| = |bracket(A^{-1})| on the unit circle (chiral amplitude invariance)
- PROVEN (numerically): Amphichiral knots have theta-reflection-symmetric amplitude
- PROVEN (numerically): bracket_mirror(e^{i*theta}) = bracket(e^{-i*theta}) — the mirror bracket is the time-reversed waveform

### Data
- 10 knots: unknot, trefoil, mirror trefoil, figure-eight, torus(2,5), torus(2,7), granny, square, cinquefoil mirror, single crossing
- 256 samples per knot over theta in [0, 2*pi)
- Waveform statistics table: min/max amplitude, ratio, zero count per knot
- Pairwise separation matrix at optimal angle
- Greedy-selected angle set for Demo 12

### Code Assets
- `waveforms[NUM_KNOTS][NSAMP]`: full complex waveform table (Cx values)
- `amplitudes[NUM_KNOTS][NSAMP]`: precomputed amplitude table
- `compute_waveforms()`: sweeps bracket evaluation around unit circle for all knots
- `test_symmetries()`: tests chiral amplitude identity, amphichiral reflection symmetry, mirror bracket relation
- `waveform_stats()`: min/max amplitude, dynamic range ratio, near-zero count per knot
- `pairwise_separation()`: finds best single angle (max-min separation), hardest pair, prints separation matrix
- `greedy_angles()`: iterative greedy selection of optimal angles maximizing worst-case pairwise distance; terminates when all pairs separated above threshold
- `ascii_waveforms()`: ASCII art visualization of amplitude waveforms (60-column, 5 intensity bands)
- `cross_validate()`: confirms consistency with Demo 10's direct evaluation
- Complex arithmetic library: `cx_make`, `cx_add`, `cx_sub`, `cx_mul`, `cx_div`, `cx_abs`, `cx_exp_i`, `cx_pow_int`, `cx_near`

### Literature Touched
- Kauffman bracket on unit circle as signal processing / waveform analysis
- Chiral knot detection via phase vs amplitude separation
- Greedy sensor placement / optimal angle selection (information-theoretic flavor)

### Open Questions
- How many angles are needed to separate ALL prime knots up to N crossings? (scaling question)
- Can the greedy-selected angles be interpreted geometrically or algebraically?
- Trefoil period test: is the waveform period pi or 2pi? (logged as INFO, not asserted)
- Connection between waveform zeros and knot invariants unexplored
