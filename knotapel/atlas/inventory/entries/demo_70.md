## Demo 70: Musica Nodorum

- **Status**: COMPLETE
- **File**: `demo_70_musica_nodorum/main.c` (~812 lines)
- **Tests**: 8/8 pass
- **Depends on**: Demo 66 (24-cell), Demo 67 (13-direction S², Hopf decomposition)
- **Feeds into**: Demo 71

### Headline
The ζ_8 algebra produces exactly 4 musical notes (C, Eb, E, F#) spanning consonance to tritone, and Yang-Baxter equivalent braids are not just topologically equivalent — they are literally the same melody.

### Key Results
- 4 eigenvalue angles of the binary octahedral group map to 4 notes: C (0°, unison), Eb (45°, minor 3rd), E (60°, major 3rd), F# (90°, tritone)
- Trace descends cleanly from 2.000 (C) to √2 (Eb) to 1.000 (E) to 0.000 (F#) — maximum consonance to maximum dissonance
- Braid words become playable melodies: each crossing yields the eigenvalue angle of the running quaternion product, mapped to a note
- Yang-Baxter relation σ₁σ₂σ₁ = σ₂σ₁σ₂ produces identical melodies step-by-step, not just at the final note — melody depends only on product length, not specific generators
- Music (eigenvalue/pitch) and space (eigenvector/position) are orthogonal: Pearson r = -0.0624 over 253 pairs
- Spatial sound mapping: latitude → MIDI pitch (C3 south to C5 north), longitude → stereo pan
- Dodecahedral and icosahedral Voronoi both give ZERO XOR6 solutions; only the data-intrinsic 13-direction eigenvector Voronoi gives 36 solutions
- Tritone-antipodal correspondence on dodecahedron: antipodal faces carry tritone pairs (C/F#, G/Db, D/Ab, A/Eb, E/Bb, B/F) — dissonance IS the antipodal map
- Garside element (s1 s2)³ melody: Eb E F# E Eb C — a complete rising-falling arch visiting all 4 knot notes

### Theorems/Conjectures
- **Conjugation-Invariance of Melody** (confirmed): tr(q) = 2cos(θ) is conjugation-invariant, so the melody of a braid word depends only on the length of each partial product, not which generators are used
- **Hopf Orthogonality** (confirmed): eigenvalue (pitch) and eigenvector (spatial direction) are independent dimensions of a unit quaternion; r = -0.06 empirically
- **Intrinsic Geometry Conjecture** (confirmed): Platonic solids with the wrong symmetry group (icosahedral, order 120) cannot replicate results from the binary octahedral group (order 48); wrong symmetry → zero solutions

### Data
- Pearson r (pitch vs. spatial distance): -0.0624 over 253 pairs, 72 same-note/different-direction pairs
- XOR6 solutions: eigenvector Voronoi = 36, icosahedron (6 effective cells after antipodal collapse) = 0, dodecahedron (10 effective cells) = 0
- 6 pairwise intervals from 4 notes: m2 (45→60°), M2 (60→90°), m3 (0→45°, 45→90°), M3 (0→60°), tritone (0→90°)
- Garside element (s1 s2)³: Eb4 E4 F#4 E4 Eb4 C4
- Longest braid (s1 s2⁻¹)⁵: Eb4 E3 F#4 E3 Eb4 C3 Eb4 E3 F#4 E3

### Code Assets
- Eigenvalue-to-interval mapping (quaternion trace → cents → note name)
- Braid word melody generator (running quaternion product, per-crossing note extraction)
- Spatial sound mapper (eigenvector → latitude/longitude → MIDI pitch + stereo pan)
- XOR6 checker against arbitrary Voronoi cell assignments
- Platonic solid vertex generators (icosahedron, dodecahedron) for comparative Voronoi testing
- ABC notation formatter for top braid melodies

### Literature Touched
- Garside normal form / fundamental braid element (Garside 1969)
- Binary octahedral group and its representation theory (McKay correspondence context)
- Yang-Baxter equation and braid group relations
- Circle of fifths / dodecahedral symmetry correspondence (music theory)
- Hopf fibration S³ → S² (Demo 67 foundational)

### Open Questions
- Do other binary polyhedral groups (tetrahedral order 24, icosahedral order 120) produce analogous note sets, and do they compute XOR for their respective NPN-class counts?
- Is the tritone-antipodal correspondence on the dodecahedron a coincidence of the circle-of-fifths assignment, or does it follow from some deeper symmetry argument?
- Can the Garside melody (Eb E F# E Eb C) be heard as a recognizable motif in any existing musical tradition, or is it genuinely novel?
- What happens to the melody structure under stabilization (adding a strand to a braid without changing the knot type)?
- Does the music-space orthogonality (r ≈ 0) hold for other quaternion algebras, or is it specific to the ζ_8 / binary octahedral case?
