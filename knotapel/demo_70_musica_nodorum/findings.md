# Demo 70: Musica Nodorum — Knots as Sound

## Summary

Mapped knot invariants to musical structures. The ζ_8 algebra naturally
produces 4 notes — C, Eb, E, F# — spanning from consonance to the tritone.
Braid words become playable melodies. Yang-Baxter equivalent braids produce
**identical** melodies (not just same final note) because trace is
conjugation-invariant. Eigenvalue (pitch) and eigenvector (spatial position)
are orthogonal (r = -0.06), confirming the Hopf decomposition from Demo 67.
Platonic solids (icosahedron, dodecahedron) fail for XOR6 — only the data's
own 13-direction Voronoi computes.

**Test count: 8/8 pass (Parts A–F)**

---

## 1. Four Knot Notes (Part A)

The 4 eigenvalue angles of the binary octahedral group map to 4 musical
notes spanning one octave:

| Angle | Cents | Note | Trace | Interval | Musical character |
|-------|-------|------|-------|----------|-------------------|
| 0°    | 0     | C    | 2.000 | Unison   | Perfect consonance (identity) |
| 45°   | 300   | Eb   | √2    | Minor 3rd | Dark consonance |
| 60°   | 400   | E    | 1.000 | Major 3rd | Bright consonance |
| 90°   | 600   | F#   | 0.000 | Tritone  | Maximum dissonance |

The trace goes from 2 (maximum) to 0 (zero) — a clean descent from
consonance to dissonance. The two intermediate notes are the minor and
major thirds, the foundational intervals of Western harmony.

The 6 pairwise intervals: m2 (45→60°), M2 (60→90°), m3 (0→45°, 45→90°),
M3 (0→60°), tritone (0→90°).

## 2. Braid Melodies (Part B)

Each braid word becomes a melody: at each crossing, extract the eigenvalue
angle of the running quaternion product, map to a note. σ crossings play
in octave 4, σ⁻¹ crossings in octave 3.

**Notable melodies:**

| Braid | Melody | Character |
|-------|--------|-----------|
| Garside (s1 s2)³ | Eb E F# E Eb C | Complete ascending/descending arpeggio |
| (s1 s2⁻¹)⁵ | Eb E₃ F# E₃ Eb C₃ Eb E₃ F# E₃ | Octave-leaping oscillation |
| Trefoil (s1³) | Eb F# Eb | Dissonant bounce |
| Figure-8 | Eb E₃ F# E₃ | Alternating octaves |

The Garside element is the most "musical" — it visits all 4 knot notes
in a rising-falling arch, like a classical motif.

## 3. Yang-Baxter = Identical Melodies (Part B)

**The Yang-Baxter relation σ₁σ₂σ₁ = σ₂σ₁σ₂ produces not just the same
final note, but the same melody at every intermediate step.**

| Step | σ₁σ₂σ₁ melody | σ₂σ₁σ₂ melody |
|------|---------------|---------------|
| 1    | Eb4           | Eb4           |
| 2    | E4            | E4            |
| 3    | F#4           | F#4           |

This is because σ₁ and σ₂ have the same eigenvalue angle (both are
rotations by π/2, just around different axes). The trace tr(q) = 2cos θ
is conjugation-invariant, so products of the same length have the same
trace regardless of which generators are used. The melody depends only
on the LENGTH of each product, not the specific generators.

Deeper implication: the musical structure (eigenvalue sequence) is a
topological invariant — Yang-Baxter equivalent braids are literally
the same song.

## 4. Spatial Sound: Music ⊥ Space (Parts C, D)

Each of the 13 eigenvector directions maps to a spatial sound:
- Latitude → MIDI pitch (C3 at south pole to C5 at north pole)
- Longitude → stereo pan (left/right)

The correlation between eigenvalue distance (musical interval) and
eigenvector distance (spatial separation) is:

| Metric | Value |
|--------|-------|
| Pearson r | -0.0624 |
| Pairs analyzed | 253 |
| Same note, different direction | 72 pairs |

**Music and space are orthogonal dimensions of a quaternion.**

This confirms the Hopf decomposition from Demo 67: a unit quaternion
decomposes into an eigenvalue (angle θ, one real degree of freedom)
and an eigenvector (axis on S², two real degrees of freedom). These
are independent — knowing the pitch tells you nothing about the
position, and vice versa.

## 5. Dodecahedral Connection (Part E)

Fran's observation: 12 keys in the circle of fifths ↔ 12 pentagonal faces
of the dodecahedron. We tested this.

**Key mapping**: The 13 eigenvector directions were assigned to dodecahedral
faces via nearest icosahedron vertex. Keys used: Eb (3 dirs), A (3),
Bb (2), E (2), Db (1), C (1). Not uniformly distributed — the knot
algebra's geometry doesn't align with dodecahedral symmetry.

**XOR6 results:**

| Polyhedron | Vertices | Effective cells | XOR6 solutions |
|------------|----------|-----------------|----------------|
| Eigenvector | 13 | 13 | **36** |
| Icosahedron | 12 | 6 | **0** |
| Dodecahedron | 20 | 10 | **0** |

**The Platonic solids fail.** Only the eigenvector Voronoi — the geometry
intrinsic to the data — computes XOR6. The dodecahedron's symmetry group
(icosahedral, order 120) is unrelated to the binary octahedral group
(order 48) that generates the knot algebra. Wrong symmetry, zero solutions.

Note: undirected eigenvector axes cause antipodal collapse. The 12
icosahedron vertices form 6 antipodal pairs → 6 effective cells. The 20
dodecahedron vertices → 10 effective cells. Even at nominal vertex count,
neither matches the 13-direction sweet spot.

## 6. The Tritone-Antipodal Correspondence

Each icosahedron vertex has an antipodal vertex. With the circle-of-fifths
assignment, antipodal faces carry tritone pairs (C/F#, G/Db, D/Ab, A/Eb,
E/Bb, B/F). The tritone — the most dissonant interval — IS the antipodal
map on the dodecahedron. This is a genuine geometric-musical correspondence,
even though the dodecahedron doesn't compute XOR6.

## 7. ABC Notation for Best Melodies

**Garside Element (s1 s2)³:**
```abc
X:1
T:Garside Element
M:3/4
L:1/4
Q:1/4=80
K:C
_E E ^F | E _E C |
```

**Longest Braid (s1 s2⁻¹)⁵:**
```abc
X:1
T:Longest Braid
M:5/4
L:1/4
Q:1/4=100
K:C
_E E, ^F E, _E | C, _E E, ^F E, |
```

## Cross-Demo Connection

| Demo | What | Key Result |
|------|------|------------|
| 67 | Eigenvector S² | 13 directions, 36 XOR6 solutions |
| 68 | Stereographic R² | Visualization; intrinsically curved |
| 69 | Clifford Staircase | ζ_8 nests in ζ_16; finer roots trivialize |
| **70** | **Musica Nodorum** | **4 notes C-Eb-E-F#; YB = identical melodies; music ⊥ space; Platonic solids fail** |

## Compile & Run

```
cc -std=c89 -pedantic -Wall -Wextra -Werror -Wconversion -Wsign-conversion \
   -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -Wwrite-strings \
   main.c -lm -o demo70
./demo70
```
