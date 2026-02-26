# Data Tables — DKC Research Atlas

All numerical tables extracted from the demo index and explorer's log. Exact numbers preserved.

*Updated 2026-02-20: Added Demo 38 (dimension reduction), Demo 39 (symmetry decomposition), Demo 60 (ell=7 cubic wall), Demo 63 (angular anatomy), Demo 39 Parts G-J (three gram forms) data.*

*Updated 2026-02-21: Added Demos 64–71 (parity matroid recursion, Clifford staircase, quaternionic DKC, coordinate system zoo, stereographic DKC, multi-root S² comparison, musica nodorum, spectral DKC).*

*Updated 2026-02-21: Added Demos 72–82 (spherical design test, eigenvector automaton, computational invariant, binocular DKC, 8-input scaling, activation zoo, recursive scaling/phase diagram, ζ₁₂ capacity, group finiteness survey, capacity scaling law, crossing depth).*

*Updated 2026-02-21: Added Demos 83–84 (framing as computational content, null states and logarithmic structure).*

*Updated 2026-02-23: Added Demos 85–92 (indecomposability parameter b, direct b on P_{0,0}, null indispensability at zeta_12, anti-correlation mechanism, depth law mechanism, sum-angle structure, activation bottleneck, function scaling/parity-lock theorem).*

*Updated 2026-02-24: Added Demos 93–94 (1wpi function scaling/complement-blindness/pigeonhole, binary icosahedral group/solvability bottleneck).*

*Updated 2026-02-26: Added Demos 102–109 (6-strand radical A/B test, dimension scaling, activation coarseness sweep, 8-strand k-regime, topological entropy, algebraic graph density, parity vocabulary, D55 graph analysis).*

---

## 1. Axiality Hierarchy Table

The master table relating evaluation level, delta value, ring structure, axiality type, catalog size, and DKC results.

### Core Hierarchy

| ℓ | δ | Ring | Rank | Axiality Type | Catalog Size | DKC Re>0+mult | DKC Im>0+mult |
|---|---|------|------|---------------|-------------|---------------|---------------|
| 2 | 0 | Z[ζ₈] | 4 | Z-axial | 100 | 13/13 at k=6 | — |
| 3 | 1 | Z[ζ₆] | 2 | Z-axial | 6 | Dead (binary collapse) | Dead |
| 4 | √2 | Z[ζ₁₆] | 2 (Z[i]-axial) | Z[i]-axial | 56 | 11/13 | 12/13 |
| 5 | φ | Z[ζ₁₀] | 4 (Z[ζ₅]-axial) | Z[ζ₅]-axial | 116 | 11/13 | 12/13 |
| 6 | √3 | Z[ζ₂₄] | 8 (full) | full rank | 29 | 11/13 | 13/13* |

*floating-point boundary artifact; any perturbation drops to 11/13

**Provenance:** Demos 29, 53-60; Explorer's Log axiality hierarchy sections.

### Extended Hierarchy with FK

| ℓ | δ | Bracket Ring | FK Ring | Bracket DKC | FK DKC |
|---|---|-------------|---------|-------------|--------|
| 2 | 0 | Z[ζ₈], Z-axial | — | 13/13 at k=6 | — |
| 3 | 1 | Z[ζ₆], Z-axial | Q=1 trivial | Dead | Dead |
| 4 | √2 | Z[ζ₁₆], Z[i]-axial (2D) | Z[ζ₈] (4D) | 13/13 split-sig | 12/13 at k=2 |

**Provenance:** Demo 56 (Potts-TL Dictionary).

### Entanglement Vanishing

| ℓ | δ | % of non-interleaving n=4 braids with zero bracket |
|---|---|-----------------------------------------------------|
| 2 | 0 | 100% |
| 3 | 1 | 0% |
| 4 | √2 | 43.7% |

**Provenance:** Demo 54, Explorer's Log.

---

## 2. Radical Dimension Tables

### δ=0 (ℓ=2) — Demo 51

| n | dim (C_n) | Gram rank | rad dim | rad² dim | Nilpotency | Semisimple? |
|---|-----------|-----------|---------|----------|------------|-------------|
| 2 | 2 | 1 (=C₁) | 1 | 0 | 2 | NO |
| 3 | 5 | 5 | 0 | — | — | YES |
| 4 | 14 | 5 (=C₃) | 9 | 5 (=C₃) | 3 | NO |
| 5 | 42 | 42 | 0 | — | — | YES |
| 6 | 132 | 42 (=C₅) | 90 | 42 (=C₅) | 3 | NO |
| 7 | 429 | 429 | 0 | — | — | YES |
| 8 | 1430 | 429 (=C₇) | 1001 | 429 (=C₇) | 3 | NO |

**Pattern:** Odd n = semisimple. Even n: Loewy length exactly 3 (sandwich theorem).

**Provenance:** Demo 51; Explorer's Log "Confirmed Data Table."

### δ=1 (ℓ=3) — Demo 52

| n | dim | Gram rank | rad | rad² | nilpotency |
|---|-----|-----------|-----|------|------------|
| 2 | 2 | 2 | 0 | — | semisimple |
| 3 | 5 | 2 | 3 | 1 | 3 |
| 4 | 14 | 11 | 3 | 1 | 3 |
| 5 | 42 | 18 | 24 | 16 | 3 |
| 6 | 132 | 99 | 33 | 17 | 3 |
| 7 | 429 | 207 | 222 | 170 | 3 |
| 8 | 1430 | 1004 | 426 | 218 | 3 |

**Provenance:** Demo 52; Explorer's Log.

### δ=√2 (ℓ=4) — Demo 52

| n | dim | Gram rank | rad | rad² | nilpotency |
|---|-----|-----------|-----|------|------------|
| 2 | 2 | 2 | 0 | — | semisimple |
| 3 | 5 | 5 | 0 | — | semisimple |
| 4 | 14 | 9 | 5 | 1 | 3 |
| 5 | 42 | 33 | 9 | 1 | 3 |
| 6 | 132 | 58 | 74 | 26 | 3 |
| 7 | 429 | 297 | 132 | 36 | 3 |

**Provenance:** Demo 52; Explorer's Log.

### δ=φ (golden ratio, ℓ=5) — Demo 52

| n | dim | Gram rank | rad | rad² | nilpotency |
|---|-----|-----------|-----|------|------------|
| 2 | 2 | 2 | 0 | — | semisimple |
| 3 | 5 | 5 | 0 | — | semisimple |
| 4 | 14 | 14 | 0 | — | semisimple |
| 5 | 42 | 35 | 7 | 1 | 3 |
| 6 | 132 | 115 | 17 | 1 | 3 |
| 7 | 429 | 270 | 159 | 37 | 3 |

**Provenance:** Demo 52; Explorer's Log.

### δ=√3 (ℓ=6) — Explorer's Log

| n | dim | Gram rank | rad | rad² | nilpotency |
|---|-----|-----------|-----|------|------------|
| 2-5 | — | — | 0 | — | semisimple |
| 6 | 132 | 123 | 9 | 1 | 3 |
| 7 | 429 | 402 | 27 | 1 | 3 |

**Provenance:** Explorer's Log ℓ=6 section.

### δ=2cos(π/7) (ℓ=7, cubic field) — Demo 60

First cubic number field case: Q(2cos(π/7)) with minimal polynomial x³-x²-2x+1=0.

| n | dim | Gram rank | rad | rad² | nilpotency |
|---|-----|-----------|-----|------|------------|
| 2 | 2 | 2 | 0 | — | semisimple |
| 3 | 5 | 5 | 0 | — | semisimple |
| 4 | 14 | 14 | 0 | — | semisimple |
| 5 | 42 | 42 | 0 | — | semisimple |
| 6 | 132 | 132 | 0 | — | semisimple |
| 7 | 429 | 418 | 11 | 1 | 3 |
| 8 | 1430 | 1391 | 39 | 1 | 3 |

**Pattern:** Semisimple for n<7. First non-semisimple at n=ℓ=7. rad(TL_7) = 11 = 2·7-3. rad(TL_8) = 39 = 7²-7-3. rad²=1, nilpotency=3 — all universal formulas confirmed at cubic case.

Cross-validated at two independent primes: p₁=1,000,000,007 (δ mod p₁ = 420063799) and p₂=1,000,000,009 (δ mod p₂ = 913264519).

**Provenance:** Demo 60; Explorer's Log.

### Radical Dimension Formulas at First Non-Semisimple n

| ℓ | δ | Field degree | n=ℓ rad dim | Formula | n=ℓ+1 rad dim | Formula |
|---|---|-------------|------------|---------|---------------|---------|
| 2 | 0 | 1 (rational) | 1 | 2(2)-3=1 ✓ | — | — |
| 3 | 1 | 1 (rational) | 3 | 2(3)-3=3 ✓ | 3 | 3²-3-3=3 ✓ |
| 4 | √2 | 2 (quadratic) | 5 | 2(4)-3=5 ✓ | 9 | 4²-4-3=9 ✓ |
| 5 | φ | 2 (quadratic) | 7 | 2(5)-3=7 ✓ | 17 | 5²-5-3=17 ✓ |
| 6 | √3 | 2 (quadratic) | 9 | 2(6)-3=9 ✓ | 27 | 6²-6-3=27 ✓ |
| 7 | 2cos(π/7) | 3 (cubic) | 11 | 2(7)-3=11 ✓ | 39 | 7²-7-3=39 ✓ |

**Formulas:** rad(TL_ℓ) = 2ℓ-3. rad(TL_{ℓ+1}) = ℓ²-ℓ-3 for ℓ≥3. rad²=1 at n=ℓ and n=ℓ+1 for all ℓ≥3. **Universal across number field degree** (confirmed through cubic at ℓ=7).

**Provenance:** Explorer's Log "Discovered Formulas" section; Demo 60.

---

## 3. DKC Landscape Matrices

### DKC at δ=0 (ℓ=2): Forward DKC Zoo — Activation × NPN Class

100M quartets searched. Solution counts from Demo 48.

| NPN Class (canon) | Name | Orbit | Topo Freq | Solutions | % |
|-------------------|------|-------|-----------|-----------|---|
| 0x03 | AND2' | — | — | 25,085,227 | 32.2% |
| 0x0F | BUF | — | — | 16,711,816 | 21.4% |
| 0x07 | ~A~(BC) | — | — | 13,574,170 | 17.4% |
| 0x01 | AND3' | — | — | 10,108,257 | 13.0% |
| 0x19 | 3v-0x19 | — | — | 3,843,271 | 4.9% |
| 0x1E | A^(B\|C) | — | — | 2,263,901 | 2.9% |
| 0x1B | 3v-0x1B | — | — | 1,690,752 | 2.2% |
| 0x3C | XOR2 | — | — | 1,613,217 | 2.1% |
| 0x06 | ~A(B^C) | — | — | 1,402,076 | 1.8% |
| 0x17 | MAJ' | — | — | 1,160,474 | 1.5% |
| 0x18 | ISOLATE | — | — | 371,435 | 0.5% |
| 0x16 | EXACT1 | — | — | 126,666 | 0.2% |
| 0x69 | XNOR3 | — | — | **0** | 0.0% |

Constants: FALSE=17,299,057, TRUE=4,749,681 (22.0% total).

**Provenance:** Demo 48.

### DKC at δ=0: Activation Function Zoo (k-sector) — Demo 50

Parity solution counts by k-sector:

| k | Parity (XNOR3) Solutions | All 13 NPN reachable? |
|---|--------------------------|----------------------|
| 2 | 0 | No (5/13) |
| 3 | 0 | No (6/13) |
| 4 | 0 | No (11/13) |
| 5 | 0 | No |
| 6 | **906** | **Yes (13/13)** |
| 7 | 756 | Yes |
| 8 | 96 | Yes |

**Four-tier hierarchy:** k=2 (5 classes), k=3 (+1), k=4 (+6), k=6 (+1=all 13).

**Non-monotonic:** 906@k=6 > 756@k=7 > 96@k=8.

**Provenance:** Demo 50; Explorer's Log.

### Complete Parity Scaling Law for Z[zeta_8] — Demo 63

| n inputs | min k | solutions at min k | solutions at k=15 | mechanism |
|----------|-------|-------------------|-------------------|-----------|
| 3 | 6 | 906 | 23,004 | triskelion |
| 4 | 8 | 96 | 16,108 | tetraskelion |
| 5 | 15 | 3,020 | 3,020 | pentaskelion |
| 6 | — | 0 | 0 | **impossible** (constraint wall) |
| 7 | — | 0 | 0 | **impossible** (constraint wall) |
| 8+ | — | 0 | 0 | **impossible** (pigeonhole) |

**Parity ceiling for Z[zeta_8] is n=5.**

**Provenance:** Demo 63 Part J.

### Sector-Octant Mapping Table — Demo 63

Class-1 octant sets for each sector count k (mod 2 activation):

| k | class-1 octants | count | notes |
|---|----------------|-------|-------|
| 2 | {4,5,6,7} | 4 | half-plane (Im > 0) |
| 3 | {3,4,5} | 3 | 120° boundaries |
| 4 | {2,3,6,7} | 4 | quadrant structure |
| 5 | {2,3,5,6} | 4 | |
| 6 | {2,4,5,7} | 4 | **parity octants** (unique) |
| 7 | {2,4,6} | 3 | |
| 8 | {1,3,5,7} | 4 | odd octants only |
| 15 | {1,2,3,4,5,6,7} | 7 | all non-zero (universal k) |

k=6 is the UNIQUE k in {2,...,16} whose class-1 set equals {2,4,5,7}.

**Provenance:** Demo 63 Parts B, D.

### Octant Distribution over 100 Catalog Values — Demo 63

| Octant | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 |
|--------|---|---|---|---|---|---|---|---|
| Count | 17 | 8 | 17 | 11 | 11 | 11 | 17 | 8 |
| Ray | a | b | c | d | a | b | c | d |
| Ray count | 28 | 19 | 34 | 19 | — | — | — | — |

Symmetric: octants {0,2,4,6} (real/imaginary axes) have counts {17,17,11,17}; octants {1,3,5,7} (diagonal axes) have counts {8,11,11,8}.

**Provenance:** Demo 63 Part A.

### Parity Triskelion Octant Distribution — Demo 63

At k=6 (3-input parity, 906 solutions):

| Octant triple | Solutions | Percentage |
|---------------|-----------|------------|
| {2,5,7} | 636 | 70.2% |
| {2,4,7} | 270 | 29.8% |

Octants 2 and 7 are mandatory in every parity solution. Only two of the C(4,3)=4 possible triples from {2,4,5,7} actually occur.

**Provenance:** Demo 63 Part E.

### Oriented Matroid Type Distribution — Demo 63

21 distinct OM types from 512 octant triples. Key finding:

| OM type | sign pattern | NPN classes at k=6 | parity? |
|---------|-------------|-------------------|---------|
| Alternating | (-,+,-) or (+,-,+) | 13/13 | YES |
| Uniform | (+,+,+) or (-,-,-) | 12/13 | NO |
| All others | mixed | ≤12/13 | NO |

0 degenerate (any cross-product = 0) OM types achieve parity.

**Provenance:** Demo 63 Part G.

### DKC at δ=√2 (ℓ=4): 3-Input Results — Demo 55

| Activation | Mult Reachable | Parity Solutions | Notes |
|-----------|----------------|------------------|-------|
| Re(z) > 0 | 11/13 | **9,334** | Simplest, most parity |
| Im(z) > 0 | 12/13 | 3,114 | |
| Split-sigmoid | 13/13 | 481 | All classes, least parity |
| Sector k=2 | 13/13 | 5,314 | |
| Sector k=4 | 13/13 | 6,712 | |
| Sector k=6 | 13/13 | 5,314 | |
| Sector k=8 | 13/13 | 8,000 | |
| Magnitude τ=1.0 | 3/13 | 0 | Very weak |

**Provenance:** Demo 55; Explorer's Log.

### Cross-ℓ DKC: Multiplicative × Additive × Activation — Demo 57/58

#### ℓ=4 (δ=√2)

| Activation | Mult | Add | Union | Add-only |
|-----------|------|-----|-------|----------|
| Re>0 | 11/13 | 5/13 | 11/13 | (none) |
| Im>0 | 12/13 | 5/13 | 12/13 | (none) |
| Split-sig | 13/13 | 12/13 | 13/13 | (none) |
| Sector k=2 | 13/13 | 11/13 | 13/13 | (none) |
| Sector k=4 | 13/13 | 12/13 | 13/13 | (none) |
| Sector k=6 | 13/13 | 13/13 | 13/13 | (none) |
| Sector k=8 | 13/13 | 13/13 | 13/13 | (none) |
| Mag τ=1 | 3/13 | 12/13 | 12/13 | 9 classes |

**Provenance:** Demo 57.

#### ℓ=5 (δ=φ)

| Activation | Mult | Add | Union | Add-only |
|-----------|------|-----|-------|----------|
| Re>0 | 11/13 | 5/13 | 11/13 | (none) |
| Im>0 | 12/13 | 5/13 | 12/13 | (none) |
| Split-sig | 13/13 | 12/13 | 13/13 | (none) |
| Sector k=2 | 13/13 | 11/13 | 13/13 | (none) |
| Sector k=4 | 13/13 | 12/13 | 13/13 | (none) |
| Sector k=6 | 12/13 | 13/13 | 13/13 | 0x1B |
| Sector k=8 | 13/13 | 13/13 | 13/13 | (none) |
| Mag τ=1 | 6/13 | 12/13 | 12/13 | 6 classes |

**Provenance:** Demo 58.

#### ℓ=6 (δ=√3)

| Activation | Mult | Add | Union | Add-only |
|-----------|------|-----|-------|----------|
| Re>0 | 11/13 | 5/13 | 11/13 | (none) |
| Im>0 | 13/13* | 5/13 | 13/13 | 0x1B, 0x06 |
| Split-sig | 12/13 | — | — | — |
| Sector k=2 | 13/13 | — | — | — |

*floating-point boundary artifact

**Provenance:** Demo 59.

### FK (Potts) DKC at ℓ=4 — Demo 56

FK catalog: 9 distinct partition function values.

| Activation | NPN Reachable | Parity | Missing |
|-----------|--------------|--------|---------|
| Re(z) > 0 | 11/13 | 113 | ~A(B^C), 3v-0x1B |
| Split-sigmoid | 10/13 | 19 | ~A(B^C), 3v-0x1B, A^(B\|C) |
| Sector k=2 | **12/13** | 32 | **3v-0x1B only** |
| Sector k=4 | 10/13 | 19 | ~A(B^C), 3v-0x1B, A^(B\|C) |
| Sector k=6 | 11/13 | 32 | ~A(B^C), 3v-0x1B |

**Provenance:** Demo 56; Explorer's Log.

### 0x1B Resistance Profile (Cross-ℓ)

| Setting | Re>0 | Im>0 | Split-sig | Sector k=2 | Sector k=8 | Magnitude |
|---------|------|------|-----------|-----------|-----------|-----------|
| ℓ=4 bracket (56 val) | 0 | 0 | **144** | **480** | **2,496** | 0 |
| ℓ=5 bracket (116 val) | 0 | 0 | reachable | reachable | reachable | 0 |
| ℓ=4 FK (9 val) | 0 | 0 | 0 | 0 | 0 | 0 |
| δ=0 (Demo 48) | — | — | 1,690,752 | — | — | — |

**Provenance:** Demo 56 five-lens analysis.

---

## 4. Bracket Value Catalogs

### δ=0 (ℓ=2) — Demo 35

| Braid | n | Bracket (Z[ζ₈]) |
|-------|---|------------------|
| Hopf link (σ₁²) | 2 | 2 |
| Trefoil (σ₁³) | 2 | -3ζ₈ |
| Figure-eight (σ₁σ₂⁻¹σ₁σ₂⁻¹) | 3 | 5 |
| σ₁σ₂σ₃ | 4 | ζ₈ |
| σ₁²σ₃² | 4 | 0 |

n=4 lengths 1-5: 15 distinct nonzero values, ALL Z-axial (pure integer × single ζ₈ power).

**Provenance:** Demo 35; Explorer's Log.

### Demo 01 Bracket Values

| Knot | Bracket |
|------|---------|
| Trefoil (left-handed 3₁) | -A⁻⁵ - A³ + A⁷ |
| Figure-eight (4₁) | A⁸ - A⁴ + 1 - A⁻⁴ + A⁻⁸ |
| Hopf link | -A⁴ - A⁻⁴ |
| Loop value d | -A² - A⁻² |

**Provenance:** Demo 01.

### δ=1 (ℓ=3) — Demo 53

Only 6 distinct values: {±1, ±ζ₆, ±ζ₆²}. Magnitudes always exactly 1. 100% Z[ζ₆]-axial.

**Provenance:** Demo 53; Explorer's Log.

### δ=√2 (ℓ=4) — Demo 54

56 distinct nonzero bracket values. 100% Z[i]-axial. Verified across 89,426 braids.

Gaussian integer catalog structure:
- n=2: 8 distinct GIs, 4 negation pairs (ALL pair up)
- n=3: 12 distinct GIs, 6 negation pairs (ALL pair up)
- n=4: 15 distinct GIs, 7 negation pairs (one unpaired: (2-2i) missing (-2+2i))

Complete negation pairing forced: A⁸ = -1.

**Provenance:** Demo 54; Explorer's Log.

### δ=φ (ℓ=5) — Demo 58

116 distinct values. Mostly 3-coefficient (55 entries) or 4-coefficient (31 entries). Only 32/116 have their negative also in catalog. Max |coefficient| = 6.

**Provenance:** Demo 58; Explorer's Log.

### δ=√3 (ℓ=6) — Demo 59

Only 29 distinct bracket values (despite rank-8 ring Z[ζ₂₄]). All entries use only 1 or 2 nonzero coefficients. Non-monotonic: catalog is SMALLER despite ring being LARGER.

**Provenance:** Demo 59; Explorer's Log.

### Bracket Catalog at δ=0 — Demo 29

100 distinct Z[ζ₈] bracket values from 2-3 strand braids (length 1-8). At A = -ζ₈ = e^{i5π/4}, only single-loop states survive (delta=0 kills all multi-loop states).

A = (0,-1,0,0), A⁻¹ = (0,0,0,1) in Z[ζ₈] basis.

**Provenance:** Demo 29.

---

## 5. Compression Ratio Data

### Demo 35 Compression (distinct TL matrices / total braids)

**n=3:**
| Length | Total braids | Distinct matrices | Ratio |
|--------|-------------|-------------------|-------|
| 1 | 4 | 4 | 1.0x |
| 2 | 16 | 13 | 1.2x |
| 3 | 64 | 34 | 1.9x |
| 4 | 256 | 81 | 3.2x |
| 5 | 1024 | 182 | 5.6x |
| 6 | 4096 | 395 | 10.4x |

**n=4:**
| Length | Total braids | Distinct matrices | Ratio |
|--------|-------------|-------------------|-------|
| 1 | 6 | 6 | 1.0x |
| 2 | 36 | 27 | 1.3x |
| 3 | 216 | 104 | 2.1x |
| 4 | 1296 | 365 | 3.6x |
| 5 | 7776 | 1214 | 6.4x |

**Provenance:** Demo 35; Explorer's Log.

### Quotient Dimension Reduction at δ=0 — Demo 38

TL_n(0)/rad ≅ TL_{n-1}(0) for even n. Asymptotic 4x dimension reduction, ~64x speedup for O(dim³) operations.

| n | C_n | Gram rank | Radical dim | Reduction factor |
|---|------|-----------|-------------|------------------|
| 2 | 2 | 1 | 1 | 2.00x |
| 3 | 5 | 5 | 0 | 1.00x |
| 4 | 14 | 5 | 9 | 2.80x |
| 5 | 42 | 42 | 0 | 1.00x |
| 6 | 132 | 42 | 90 | 3.14x |
| 7 | 429 | 429 | 0 | 1.00x |
| 8 | 1430 | 429 | 1001 | 3.33x |

Quotient nonzero products (verifying isomorphism):
- TL_2/rad: 1 product (matches TL_1)
- TL_4/rad: 17 products (matches TL_3)
- TL_6/rad: 955 products (matches TL_5)

**Provenance:** Demo 38.

### TL Generator Sparsity (nonzero entries in e₁)

| n | dim (C_n) | Nonzero entries | Formula C_n - C_{n-1} |
|---|-----------|----------------|----------------------|
| 2 | 2 | 1 | 2-1=1 ✓ |
| 3 | 5 | 3 | 5-2=3 ✓ |
| 4 | 14 | 9 | 14-5=9 ✓ |
| 5 | 42 | 28 | 42-14=28 ✓ |

**Provenance:** Demo 35.

---

## 6. Catalan Trace Data — Demo 51

All nonzero fixed-point counts (traces of left multiplication by basis elements) are Catalan numbers. Window formula: nonzero values at n = {C_m : ⌊n/2⌋+1 ≤ m ≤ n}.

| n | Nonzero trace values | Window |
|---|---------------------|--------|
| 2 | {C₂} = {2} | [2,2] |
| 3 | {C₂, C₃} = {2, 5} | [2,3] |
| 4 | {C₃, C₄} = {5, 14} | [3,4] |
| 5 | {C₃, C₄, C₅} = {5, 14, 42} | [3,5] |
| 6 | {C₄, C₅, C₆} = {14, 42, 132} | [4,6] |
| 7 | {C₄, C₅, C₆, C₇} = {14, 42, 132, 429} | [4,7] |
| 8 | {C₅, C₆, C₇, C₈} = {42, 132, 429, 1430} | [5,8] |

Window width = ⌈n/2⌉.

**Provenance:** Demo 51; Explorer's Log.

---

## 7. Fibonacci Rank Theorem Data — Demo 52

Total bilinear rank of the rad² generator (sum of ranks across through-strand sectors) = F(ℓ-1).

| ℓ | δ | Field degree | Sectors | Rank per sector | Total rank | F(ℓ-1) |
|---|---|-------------|---------|-----------------|------------|---------|
| 3 | 1 | 1 | {1} | [1] | 1 | F(2)=1 ✓ |
| 4 | √2 | 2 | {0,2} | [1,1] | 2 | F(3)=2 ✓ |
| 5 | φ | 2 | {1,3} | [2,1] | 3 | F(4)=3 ✓ |
| 6 | √3 | 2 | {0,2,4} | [1,3,1] | 5 | F(5)=5 ✓ |
| 7 | 2cos(π/7) | 3 | {1,3,5,7} | [3,4,1,0] | 8 | F(6)=8 ✓ |

**Palindromic conjecture (even ℓ):** [1,1] at ℓ=4, [1,3,1] at ℓ=6.

**ℓ=7 sector detail (Demo 60):**

| Sector j | Elements | Matrix size | Rank |
|----------|----------|-------------|------|
| 1 | 196 | 14×14 | 3 |
| 3 | 196 | 14×14 | 4 |
| 5 | 36 | 6×6 | 1 |
| 7 | 1 | 1×1 | 0 |
| **Total** | | | **8 = F(6)** |

**Provenance:** Explorer's Log "Fibonacci Rank Theorem" section; Demo 60.

### Decoherence Rate Scaling

| ℓ | rad proportion (2ℓ-3)/C_ℓ | Rate |
|---|---------------------------|------|
| 3 | 3/5 | 60% |
| 4 | 5/14 | 36% |
| 5 | 7/42 | 17% |
| 6 | 9/132 | 7% |
| 7 | 11/429 | 2.6% |

Higher ℓ = less decoherence = more stable coherent sector.

**Provenance:** Explorer's Log; Demo 60.

---

## 8. Sandwich Theorem Data — Demo 51

### Loewy Layers for TL_{2k}(0) (k ≥ 2)

| Layer | Dimension | Structure |
|-------|-----------|-----------|
| Top: TL_{2k}/rad | C_{2k-1} | ≅ TL_{2k-1}(0) (semisimple) |
| Middle: rad/rad² | C_{2k} - 2·C_{2k-1} | Bimodule connecting top and bottom |
| Bottom: rad² | C_{2k-1} | ≅ TL_{2k-1}(0) as bimodule |

### Peirce Block Verification (rad² ≅ odd TL)

| Algebra | Peirce blocks (f₀, f₁, f₂) |
|---------|----------------------------|
| TL_3(0) | [[1,1,0],[1,1,0],[0,0,1]] |
| TL_4(0) rad² | [[1,1,0],[1,1,0],[0,0,1]] ← matches TL_3 |
| TL_5(0) | [[5,5,4],[5,5,4],[4,4,6]] |
| TL_6(0) rad² | [[5,5,4],[5,5,4],[4,4,6]] ← matches TL_5 |
| TL_7(0) | [[42,42,48],[42,42,48],[48,48,69]] |
| TL_8(0) rad² | [[42,42,48],[42,42,48],[48,48,69]] ← matches TL_7 |

**Provenance:** Demo 51; Explorer's Log.

### Multiresolution Pyramid

| Level | Approximation space | Detail space | Dimensions |
|-------|-------------------|-------------|------------|
| k=1 | TL_1 ≅ k (dim 1) | TL_2 detail (dim 1) | 1 + 1 = 2 = C₂ |
| k=2 | TL_3 (dim 5) | TL_4 detail (dim 4) | 5 + 4 + 5 = 14 = C₄ |
| k=3 | TL_5 (dim 42) | TL_6 detail (dim 48) | 42 + 48 + 42 = 132 = C₆ |
| k=4 | TL_7 (dim 429) | TL_8 detail (dim 572) | 429 + 572 + 429 = 1430 = C₈ |

**Provenance:** Explorer's Log.

### Semisimple Decompositions (odd n at δ=0)

| n | Decomposition |
|---|---------------|
| 3 | M₂(k) ⊕ k |
| 5 | M₅(k) ⊕ M₄(k) ⊕ k |
| 7 | M₁₄(k) ⊕ M₁₄(k) ⊕ M₆(k) ⊕ k |

Simple module dims follow ballot numbers: dim S_{n,t} = C(n,(n-t)/2) - C(n,(n-t)/2-1).

**Provenance:** Demo 51; Explorer's Log.

---

## 9. Quantum Dimension Data

### Quantum Integers [n]_q — Demo 05

| n | [n]_q |
|---|-------|
| 1 | 1 |
| 2 | A² + A⁻² |
| 3 | A⁴ + 1 + A⁻⁴ |

Recurrence: [n+1]_q = [2]_q · [n]_q - [n-1]_q

**Quantum trace formula (Demo 05):** tr_q = (-1)^n · δ · bracket

- n=2: 4×4 matrices, 1 generator
- n=3: 8×8 matrices, 2 generators
- n=4: 16×16 matrices, 3 generators
- n=5: 32×32 matrices, 4 generators

**Provenance:** Demos 04, 05.

---

## 10. Knot Bracket Zero Structure — Demo 14

### Zero Counts

| Knot | Zero count | Zero positions |
|------|-----------|----------------|
| Unknot | 4 | π/4 multiples (delta zeros) |
| T(2,3)/mirror | 8 | π/8 lattice |
| T(2,5)/mirror | 0 | — |
| T(2,7)/mirror | 0 | — |
| T(2,9)/mirror | 8 | π/8 lattice |
| T(2,11)/mirror | 4 | π/4 multiples |
| T(2,13)/mirror | 4 | π/4 multiples |
| Figure-eight | 0 | — (min amp 0.021) |
| All composites | 0 | — |

Torus knot pattern: {8, 0, 0, 8, 4, 4} for n=3,5,7,9,11,13.

Zero count partition: {0, 4, 8} only.

**Provenance:** Demo 14.

---

## 11. 11/13 Half-Plane Theorem — Demos 61-62

### The 12 Achievable NPN Classes (Demo 61)

| Canon | Name | Weight | Affine | Witness (p₁,p₂,p₃) at N=360 |
|-------|------|--------|--------|-------------------------------|
| 0x00 | FALSE | 0 | Y | (0,0,0) |
| 0x01 | AND3 | 1 | Y | (1,1,1) |
| 0x03 | AND2 | 2 | Y | (0,1,1) |
| 0x07 | OR-NAND | 3 | N | (1,1,2) |
| 0x0F | BUF | 4 | Y | (0,0,1) |
| 0x16 | EXACT1 | 3 | N | (2,179,179) |
| 0x17 | MINORITY | 4 | N | (1,1,1) |
| 0x18 | ISOLATE | 2 | Y | (1,1,179) |
| 0x19 | 3-SELECT | 3 | N | (1,2,179) |
| 0x1E | XOR-OR | 4 | N | (1,1,180) |
| 0x3C | XOR2 | 4 | Y | (0,1,180) |
| 0x69 | PARITY | 4 | Y | (1,180,180) |

### The 2 Unreachable Classes

| Canon | Name | Orbit size | Proof method (Demo 62) |
|-------|------|-----------|------------------------|
| 0x06 | XOR-AND | 24 | Four-semicircle parallelogram |
| 0x1B | CROSS | 24 | Algebraic contradiction (interval squeeze) |

Total unreachable truth tables: 48 of 256.

**Provenance:** Demos 61, 62; Explorer's Log.

### Rotated Half-Plane Sweep (ℓ=6, Demo 59)

ALL 24 angles (every 15°) give EXACTLY 11/13.

**Provenance:** Demo 59; Explorer's Log.

---

## 13. Spearman Correlation Data — Demos 45-47

### Forward-Reverse Duality

| Metric pair | Spearman ρ | Demo |
|------------|-----------|------|
| Forward solutions vs topo_freq | +0.7830 | 48 |
| Forward solutions vs reverse RMS | -0.7912 | 48 |
| Forward solutions vs orbit size | +0.4698 | 48 |
| Gauge RMS vs topo_freq | -0.8167 | 45 |
| Phases vs RMS (Pearson) | +0.87 | 45 |

### Topology-Frequency Correlation Reversal (Demo 47)

| Activation | Spearman ρ(pn-RMS, topo_freq) |
|-----------|-------------------------------|
| Split-sigmoid | -0.783 |
| MVN-continuous | **+0.550** |
| MVN-k8 | -0.100 |

**Provenance:** Demos 45, 47, 48.

---

## 14. Reverse DKC Decomposition Data — Demos 26-28, 45-47

### Activation Comparison (Demo 47) — 13-class average

| Metric | Split-sig | MVN-cont | MVN-k8 |
|--------|-----------|----------|--------|
| Global gauge RMS | 0.716 | 0.546 | 0.557 |
| Per-neuron gauge RMS | 0.572 | 0.457 | 0.497 |
| Amplitude-only RMS | 0.328 | 0.360 | 0.399 |
| Phase error (rad) | 0.189 | 0.197 | 0.207 |
| Random baseline | 0.809 | — | — |

### Parity (XNOR3) Specifically

| Metric | Split-sig | MVN-cont | MVN-k8 |
|--------|-----------|----------|--------|
| pn-RMS | 0.765 | **0.407** | 0.590 |

### Per-neuron Gauge (Demo 46) — 3 Methods

| Method | Avg RMS |
|--------|---------|
| Global gauge (1D) | 0.7164 |
| Per-neuron gauge (3D) | 0.5723 |
| Amplitude-only | **0.3283** |
| Random baseline | 0.8086 |

**Provenance:** Demos 46, 47.

---

## 15. Gate Complexity Data — Demos 18-19, 22

### NAND Expression Cost (Demo 22)

| Function | NAND cost (crossings) |
|----------|-----------------------|
| Identity | 0 |
| NOT | 6 |
| NAND | 5 |
| AND | 11 |
| OR | 17 |
| MUX | 21 |
| XOR | 25 |
| Toffoli C' | 58 |

### Gate Braid Words (Demo 19)

| Gate | Strands | Word | Crossings |
|------|---------|------|-----------|
| NOT | 2 | [-1,-1,-1,-1,-1,-1] | 6 |
| NAND | 3 | [-1,-1,-1,2,2] | 5 |
| AND (composed) | — | NAND + NOT | 11 |
| XOR (3-NAND) | — | — | ~15 |

**Provenance:** Demos 18, 19, 22.

---

## 16. CSS Feasibility Probe (Negative Result)

| Setting | dim | Gram rank | radical dim |
|---------|-----|-----------|-------------|
| TL_4, δ=0 | 14 | 5 | 9 |
| TL_4, δ=√2 | 14 | 9 | 5 |
| TL_6, δ=0 | 132 | 42 | 90 |
| TL_6, δ=√2 | 132 | 58 | 74 |

Result: CSS approach fails. Radical is self-orthogonal under Gram form (tautological) but NOT under standard inner product. No interesting code structure.

**Provenance:** Explorer's Log "CSS Feasibility Probe" section.

---

## 17. Key Demo Landmark Table

| Demo | Headline | Why It Matters |
|------|----------|----------------|
| 01 | State-sum bracket works | The O(2^k) oracle. Foundation. |
| 06 | JW idempotents, TL_3 polynomial matrices | First TL matrices. |
| 14 | Bracket zeros ∈ {0,4,8} for torus knots | 8-fold symmetry. |
| 18 | NOT + NAND gates from braids | Universal gate set. |
| 19 | Convergence at A = e^{i5π/4} | The "magic angle" IS -ζ₈. |
| 22 | TL_3 as 5×5 matrices, compression ratios | Direct predecessor to Demo 35. |
| 24 | Rational tangles = TL_2. \|D(n-twist)\| = n. | TL_2 case of Demo 35. |
| 25 | 4 closures always linearly independent | Maximum information extraction. |
| 29 | Forward DKC confirmed. Cyc8 exact arithmetic. | 65,536 XOR triples. |
| **35** | **General TL_n matrices over Cyc8, n=2-5** | **Keystone. Unlocks 7 follow-ons.** |
| 38 | Dimension reduction at δ=0. TL_n/rad ≅ TL_{n-1}. | ~4x dim reduction, ~64x speedup for DKC. |
| 39 | Symmetry decomposition. Cellular sectors verified. | Cell filtration, cross-sector coupling, block structure. |
| 45 | Spearman ρ = -0.82 (topology↔decomposition) | Strongest realist evidence. |
| 47 | MVN reverses topology correlation | Phase coherence, not alignment. |
| 48 | Parity has ZERO forward DKC solutions (split-sig) | Wall is sharp. |
| 50 | Parity at k=6 (906 solutions). Triskelion. | The breakthrough. All 13 NPN. |
| 51 | Sandwich theorem. Loewy length 3. | Radical anatomy at δ=0. |
| 52 | Universal nilpotency 3 across ℓ=2-5. | Chebyshev generalization. |
| 55 | Parity at Re>0 (9,334 solutions) at δ=√2. | Activation wall dissolved. |
| 56 | Ising partition functions compute 12/13 NPN. | Physics-DKC bridge. |
| 60 | ℓ=7 cubic wall. All radical formulas universal. | First cubic field; 2ℓ-3, ℓ²-ℓ-3, F(ℓ-1) all hold. |
| 61 | 11/13 half-plane theorem (pure integer proof). | Computational proof. |
| 62 | Analytical proof: WHY 0x06 and 0x1B blocked. | Geometric obstruction identified. |
| 64 | Matroid minor chain. Funnel collapses at n=6. Min wall distance d=4. | Parity hierarchy is deletion-contraction closed; XOR6 wall characterized. |
| 65 | Z[ζ₁₆] leaves parity ceiling at n=5. k=24 breaks convention wall. | Gap-of-2 refuted; generalized activations first break XOR6 at k=24. |
| 66 | 24-cell as braid image. Voronoi beats geographic grid (35 vs 34). | 4D polytope is natural computational geometry; antipodal constraint absolute. |
| 67 | S² is the sweet spot: 14 cells, 36 solutions. Hopf phase is inert. | DKC lives on rotation axis S²; Hopf fiber carries zero information. |
| 68 | Stereographic projection: S² metric preserves solutions; Euclidean gives zero. | Computation is intrinsically curved; plane is a window, not a workspace. |
| 69 | Direction Nesting Theorem. ζ_8 is computational sweet spot. Trivialization at ζ_16. | Coarsest root structures S²; finer roots nest it but trivialize XOR6. |
| 70 | 4 knot notes: C, Eb, E, F#. Yang-Baxter = same melody. Icosahedron fails. | Braid words are melodies; music-space orthogonality r=-0.06. |
| 71 | Sharp phase transition at l=6. 13=13 theorem. 3.5× super-Nyquist compression. | Spectral characterization of XOR6 on S²; bandwidth DOF explains trivialization. |

| 82 | Depth law: max_xor ~ depth + 6. Deep beats vocabulary. | Linear capacity growth; algebraic coherence. |
| 83 | Framing = computational content. Bracket capacity = Jones + 2. | Writhe is independent information. |
| 84 | Null indispensability at ζ₈. 6+3+4 direction partition. | Null entries hold manifold open. |
| 85 | b = -5/8 confirmed. Standard modules fully diagonalizable. | GRS/PR controversy resolved; Jordan blocks on P_{0,0} only. |
| 86 | Delta-form universally diverges on single P_{0,0}. | Novel negative result; multiplicity structurally essential. |
| 87 | Null dispensability at ζ₁₂. Regime transition finite↔infinite. | Direction density absorbs null-only directions. |
| 88 | Anti-correlation: worst integration = best computation. | Body diagonals = anchors; edge midpoints = scaffolding. |
| 89 | 8 hypotheses tested, 5 killed. Algebraic coherence remains. | Cayley density, pairwise coherence, sector saturation all dead. |
| 90 | Axis cancellation + cross-depth constraint = depth law mechanism. | Spectral inversion: bandwidth DOWN while computation UP. |
| 91 | Activation NOT the bottleneck. Parity is the wall. | Depth gap invariant across all resolutions k=2 to k=48. |
| 92 | Parity-Lock Theorem. +/-q encoding computes only XOR/XNOR. | AND/OR/MAJ structurally impossible; 3^k equivalence classes. |
| 102 | Barrington-Radical Principle. Radical carries abelian writhe character. | Simple module beats non-simple at every XOR level. Radical provably useless for parity. |
| 103 | BFS growth is braid group invariant, not module dimension. | dim-5 and dim-9 identical BFS trees; higher dim HURTS XOR (curse of dimensionality). |
| 104 | Atkinson sweet spot at 120 comp. Cross-block IS the signal. | k (cell count) is the real lever, not hash architecture. Sign-rank expansion discovered. |
| 105 | 8-strand revives XOR8 at high k. D93 "XOR dies at N>=7" REFUTED. | Macrame principle: branching interaction reverses at n=8 (within-block > cross-block). |
| 106 | Topological entropy orthogonal to DKC. Prediction falsified. | TL quotient erases all dynamical information; computation is collective/algebraic. |
| 107 | Z/4Z Axis-Alignment Theorem proven. Nesting parity novel. | Constructive phase formula, major index bipartite invariant, q-Catalan identity. |
| 108 | Graph structure predicts parity: Dual-Channel Theorem. | Product closure + additive v_2 connectivity = necessary conditions for parity. |
| 109 | Encoding-dependent dual-channel: product closure polarity inverts. | 7/8 edge types identical between parity and non-parity at delta=sqrt(2). |

**Provenance:** Explorer's Log landmark table + Demos 38-39, 55-71, 82-92, 102-109.

---

## 18. Neglecton Data — Demo 49

| Metric | Value |
|--------|-------|
| Total braids enumerated | 87,890 |
| Standard bracket = 0 | 17,930 (20.4%) |
| Nonzero neglecton | 81,006 (92.2%) |
| Both zero | 2,688 (3.1%) |
| Previously invisible (std=0, neg≠0) | 15,242 |
| Standard catalog values | 100 |
| Neglecton catalog values | 148 |
| Combined catalog | 216 (68 std-only, 116 neg-only, 32 both) |
| Genuinely new angles from neglecton | **0 out of 116** |
| Search space (combined) | 216⁴ = 2,176,782,336 |
| Parity (XNOR3) solutions | **0** |

Neglecton magnitudes start at |z|=4 (4× standard minimum of 1), due to d(δ)/d(ε) = 4.

**Provenance:** Demo 49; Explorer's Log.

---

## 19. rad² Generator Structure — Explorer's Log

### Zero Count at n=ℓ+1

| ℓ | Zero coefficients in rad² generator | Formula |
|---|-------------------------------------|---------|
| 3 | 10 | ℓ²+1=10 ✓ |
| 4 | 17 | ℓ²+1=17 ✓ |
| 5 | 26 | ℓ²+1=26 ✓ |
| 6 | 37 | ℓ²+1=37 ✓ |

**Identity-zero property:** At n=ℓ, the rad² generator has zero coefficient on exactly the identity element and nonzero on all other C_ℓ-1 basis elements.

**Provenance:** Explorer's Log "Tensor Product Structure" section.

---

## 20. Difficulty Hierarchy Reshuffling at δ=√2 — Demo 55

| Class | δ=0 split-sig solutions | δ=√2 Re>0 solutions | Change |
|-------|------------------------|---------------------|--------|
| XNOR3 (parity) | 0 (impossible) | 9,334 | **impossible → abundant** |
| ~A(B^C) | 1,402,076 | 0 | **abundant → impossible** |
| 3v-0x1B | 1,690,752 | 0 | **abundant → impossible** |
| EXACT1 | 126,666 | 6,568 | hard → hard |

**Provenance:** Demo 55; Explorer's Log.

---

## 21. Through-Strand Sector Decomposition — Demos 38, 39

### Sector Status at δ=0 (Demo 38)

Per-sector Gram rank decomposition. DEAD = rank 0 (entirely radical). LIVE = full rank. PARTIAL = intermediate.

**n=4:**
| Sector j | Elements | Gram rank | Status |
|----------|----------|-----------|--------|
| 0 | 4 | 0 | DEAD |
| 2 | 9 | 4 | PARTIAL |
| 4 | 1 | 1 | LIVE |

**n=6:**
| Sector j | Elements | Gram rank | Status |
|----------|----------|-----------|--------|
| 0 | 25 | 0 | DEAD |
| 2 | 81 | 25 | PARTIAL |
| 4 | 25 | 20 | PARTIAL |
| 6 | 1 | 1 | LIVE |

**n=8:**
| Sector j | Elements | Gram rank | Status |
|----------|----------|-----------|--------|
| 0 | 196 | 0 | DEAD |
| 2 | 784 | 196 | PARTIAL |
| 4 | 400 | 294 | PARTIAL |
| 6 | 49 | 42 | PARTIAL |
| 8 | 1 | 1 | LIVE |

Sector rank sum exceeds full Gram rank at n>=6, reflecting cross-sector dependencies.

**Odd n (n=3,5,7):** All sectors fully LIVE at δ=0.

**Provenance:** Demo 38; Demo 39.

### Cross-Sector Coupling (Demo 39)

Coupling = (sum of per-sector ranks) - (full Gram rank). Measures cross-sector dependencies in radical structure.

| n | Sector rank sum | Full Gram rank | Coupling |
|---|-----------------|----------------|----------|
| 4 | 5 | 5 | 0 |
| 6 | 46 | 42 | 4 |
| 8 | 533 | 429 | 104 |

**Provenance:** Demo 39.

### Cell Module Dimensions d(n,j) (Demo 39)

d(n,j) = C(n,(n-j)/2) - C(n,(n-j)/2-1). Sector size |V_j| = d(n,j)². Verified exhaustively for n=2..8.

| n | d(n,j) by sector (j=0,2,...,n or j=1,3,...,n) |
|---|-----------------------------------------------|
| 4 | d(4,0)=2, d(4,2)=3, d(4,4)=1 |
| 6 | d(6,0)=5, d(6,2)=9, d(6,4)=5, d(6,6)=1 |
| 8 | d(8,0)=14, d(8,2)=28, d(8,4)=20, d(8,6)=7, d(8,8)=1 |

Check: |V_j| = d²: e.g. n=6 j=2: d=9, sector size=81=9². n=8 j=4: d=20, sector size=400=20².

**Provenance:** Demo 39.

---

## 22. Multi-Delta Gram Rank Comparison — Demo 39

### Full Gram ranks at fixed n, varying δ

**n=4 (dim=14):**
| δ | ℓ | Gram rank | Semisimple? |
|---|---|-----------|-------------|
| 0 | 2 | 5 | No |
| 1 | 3 | 11 | No |
| √2 | 4 | 9 | No |
| φ | 5 | 14 | **Yes** |

**n=6 (dim=132):**
| δ | ℓ | Gram rank | Semisimple? |
|---|---|-----------|-------------|
| 0 | 2 | 42 | No |
| 1 | 3 | 99 | No |
| √2 | 4 | 58 | No |
| φ | 5 | 132 | **Yes** |

**Provenance:** Demo 39.

### Radical Dimensions at n=ℓ Boundary (Demo 39)

At the critical boundary where first non-semisimplicity appears (n=ℓ, δ=2cos(π/ℓ)):

| ℓ | δ | Radical dim | Cross-sector coupling | All sectors LIVE? |
|---|---|-----------|----------------------|-------------------|
| 2 | 0 | 1 | — | No (j=0 DEAD) |
| 3 | 1 | 3 | — | No |
| 4 | √2 | 5 | 3 | No |
| 5 | φ | 0 (semisimple) | 0 | Yes |
| 6 | √3 | 9 | 9 | Yes |
| 7 | 2cos(π/7) | 11 | 11 | Yes |

Pattern: At ℓ≥5, all sectors are LIVE even when radical is nonzero — the radical "hides" in cross-sector coupling.

**Provenance:** Demo 39; Demo 60.

---

## 23. Generator Block Structure — Demo 39

### Block-Triangular but NOT Block-Diagonal

In sector-ordered basis (decreasing j), all generators are block-lower-triangular at δ=0. But even semisimple algebras (odd n) are NOT block-diagonal — a change-of-basis is needed.

**Off-diagonal block entry counts per generator:**

| n | Generators | Off-diagonal counts |
|---|------------|---------------------|
| 3 | e₁, e₂ | [1, 1] |
| 5 | e₁..e₄ | [9, 5, 5, 9] |

**Provenance:** Demo 39.

---

## 24. Graham-Lehrer Semisimplicity Verification — Demo 39

TL_n(δ) at δ=2cos(π/ℓ) is semisimple iff n < ℓ.

| ℓ | n=ℓ result | Confirmed? |
|---|-----------|------------|
| 2 | Non-semisimple (rad=1) | ✓ |
| 3 | Non-semisimple (rad=3) | ✓ |
| 4 | Non-semisimple (rad=5) | ✓ |
| 5 | **Semisimple** (rad=0) | ✓ |
| 6 | Non-semisimple (rad=9) | ✓ |
| 7 | Non-semisimple (rad=11) | ✓ |

Note: ℓ=5 is the unique case where n=ℓ is still semisimple (Graham-Lehrer criterion: n < ℓ is sufficient but the boundary n=ℓ depends on ℓ).

**Provenance:** Demo 39; Demo 60.

---

## 25. Three Gram Forms at n=ℓ Boundary — Demo 39 Parts G-J

### Fixpt vs Markov Radical at n=ℓ

| ℓ | dim(TL_ℓ) | fixpt rank | Markov rank | fixpt rad | Markov rad | excess |
|---|-----------|------------|-------------|-----------|------------|--------|
| 2 | 2 | 1 | 0 | 1 | 2 | 1 |
| 3 | 5 | 2 | 1 | 3 | 4 | 1 |
| 4 | 14 | 9 | 8 | 5 | 6 | 1 |
| 5 | 42 | 35 | 34 | 7 | 8 | 1 |
| 6 | 132 | 123 | 122 | 9 | 10 | 1 |
| 7 | 429 | 418 | 417 | 11 | 12 | 1 |

Pattern: Markov_rad = fixpt_rad + 1 = 2ℓ-2 at n=ℓ. Excess always 1 (single killed block L_ℓ, dim 1).

**Provenance:** Demo 39 Part G.

### Markov Excess at Successive Levels

| Level | killed blocks | ℓ=3 excess | ℓ=4 excess | ℓ=5 excess | ℓ=6 excess | ℓ=7 excess |
|-------|--------------|------------|------------|------------|------------|------------|
| n=ℓ | L_ℓ (dim 1) | 1 | 1 | 1 | 1 | 1 |
| n=ℓ+1 | L_{ℓ+1}(dim 1), L_{ℓ-1}(dim ℓ) | 10 | 17 | 26 | 37 | 50 |
| n=ℓ+2 | L_{ℓ+2}(dim 1), L_ℓ(dim ℓ+1) | 17 | 26 | 37 | 50 | — |
| n=ℓ+3 | 3 blocks | 98 | 233 | 450 | — | — |

Formula: excess = Σ (dim L_j)² for j ≥ ℓ-1 (same parity as n).

**Provenance:** Demo 39 Parts I-J.

### Next-Level Radical (n=ℓ+1)

| ℓ | rad(TL_{ℓ+1}) | = ℓ²-ℓ-3 | unique degenerate | corank |
|---|----------------|-----------|-------------------|--------|
| 3 | 3 | 9-3-3=3 ✓ | V_0 | 1 |
| 4 | 9 | 16-4-3=9 ✓ | V_1 | 1 |
| 5 | 17 | 25-5-3=17 ✓ | V_2 | 1 |
| 6 | 27 | 36-6-3=27 ✓ | V_3 | 1 |
| 7 | 39 | 49-7-3=39 ✓ | V_4 | 1 |

**Provenance:** Demo 39 Part H; formal proof in `proofs/next-level-radical-formula.md`.

### Second-Degeneracy Corank (V_{ℓ-2} at n=ℓ+2)

| ℓ | corank | = ℓ+1 |
|---|--------|-------|
| 3 | 4 | ✓ |
| 4 | 5 | ✓ |
| 5 | 6 | ✓ |
| 6 | 7 | ✓ |

**Provenance:** Demo 39 Part H.

---

---

## 26. Parity Matroid Funnel — Demo 64

### Upward Coverage (Funnel)

| Transition | Extensible sets | Total sets | Coverage |
|-----------|----------------|------------|----------|
| XOR3 → XOR2 | 1459 | 1469 | 99.3% |
| XOR4 → XOR3 | 1503 | 3834 | 39.2% |
| XOR5 → XOR4 | 129 | 673 | 19.2% |
| XOR6 → XOR5 | 0 | 27 | 0% |

The n=6 wall casts a shadow: 80% of XOR4 weight sets are already dead ends (cannot extend to XOR5).

**Provenance:** Demo 64.

### Distinct Unordered Weight Sets

| Level | Distinct sets | Ordered solutions | Notes |
|-------|--------------|------------------|-------|
| XOR2 | 1,469 | — | — |
| XOR3 | 3,834 | — | — |
| XOR4 | 673 | — | — |
| XOR5 | 27 | 3,020 | ~112 per set (≈5!=120, near-complete permutation symmetry) |

**Provenance:** Demo 64.

### Parent-Child Multiplicity

| Transition | Max parents | Mean parents | Notes |
|-----------|-------------|--------------|-------|
| XOR5 → XOR4 | 2 | 1.05 | Near-tree at wall |
| XOR4 → XOR3 | 8 | 1.79 | — |
| XOR3 → XOR2 | 29 | 7.88 | Dense mesh away from wall |

6 collisions in XOR5→XOR4 deletion map: 27×5=135 instances → 129 distinct children + 6 collisions.

**Provenance:** Demo 64.

### Vocabulary Stratification

| Category | Count | Octants |
|----------|-------|---------|
| Good values (extensible) | 44 | — |
| Poison values (orphan-only) | 31 | {1,2,3,5,6,7} — octant 4 absent |
| Total class-1 at k=15 | 83 | — |
| Values appearing in any XOR4 | 75 | — |

Strictly nested: the same 44 good values appear in XOR5 solutions and extensible XOR4 solutions.

**Provenance:** Demo 64.

### Wall Hamming Distance Distribution

| Distance d | Cases | Notes |
|-----------|-------|-------|
| 0 | 0 | — |
| 1 | 0 | — |
| 2 | 0 | — |
| 3 | 0 | — |
| 4 | 38 | Minimum (closest failures); 34/38 = 89.5% are self-doubling |
| ≤8 | 195 | Near-misses total |
| ~15–16 | peak | Distribution peak |

2,241 total extension attempts (27 XOR5 sets × 83 values).

**Provenance:** Demo 64.

### Error Weight Distribution (Near-misses)

| Input weight | Cases |
|-------------|-------|
| 0 | 0 |
| 1 | 0 |
| 2 | 237 |
| 3 | 370 |
| 4 | 446 (peak) |
| 5 | 172 |
| 6 | 23 |

Zero errors at weight 0 or 1 — wall manifests only at pairwise interaction level and above.
Second wall layer: d=7 (2 cases, 0 broken pairs), d=8 (16 cases, 0 broken pairs) — GHZ-type higher-order obstruction.

**Provenance:** Demo 64.

---

## 27. Clifford Hierarchy Staircase — Demo 65

### Z[ζ₈] vs Z[ζ₁₆] Tier Threshold Comparison

| Ring | XOR3 min k | XOR4 min k | XOR5 min k | XOR6 min k (standard) | Parity ceiling |
|------|-----------|-----------|-----------|----------------------|---------------|
| Z[ζ₈] | 6 | 8 | 15 | impossible | n=5 |
| Z[ζ₁₆] | 6 | 8 | 14 | impossible | n=5 |

Tier thresholds are identical. Parity ceiling is algebra-independent.

**Provenance:** Demo 65.

### Z[ζ₁₆] Catalog Structure

| Property | Value |
|---------|-------|
| Base catalog size | 36 distinct values |
| Extended catalog size | 56 values (+20 from 4-strand braids) |
| Angular directions occupied | 14/16 (missing dirs 2 and 11, at 45.0° and 247.5°) |
| Axial / two-component split | 24/36 axial, 12/36 two-component |
| Overlap with Z[ζ₈] | 8/100 Z[ζ₈] values appear in Z[ζ₁₆]; 20/36 Z[ζ₁₆] values lie in Z[ζ₈] subring |
| Pigeonhole bound | 14 (vs 7 for Z[ζ₈]) |
| Gap from pigeonhole to ceiling | 9 (14-5) — widened from 2 (7-5) |

**Provenance:** Demo 65.

### XOR5 Solution Landscape at Z[ζ₁₆]

| k | XOR5 solutions | Notes |
|---|---------------|-------|
| 14 | 120 | Min k for XOR5 |
| 15 | 1,080 (extended) | Incompleteness artifact corrected by catalog extension |
| 17 | 240 | — |
| 21 | 240 | — |
| 31 | 960 | k=31 zero was incompleteness artifact; recovered to 960 with extended catalog |

**Provenance:** Demo 65.

### Generalized XOR6 Results by k (Even k Landscape)

| k | Generalized XOR6 pass count | Notes |
|---|----------------------------|-------|
| ≤23 | 0 | Absolute wall regardless of labeling |
| 24 | 3 | First break; 3/2,700 pass |
| 26 | 4 | — |
| 28 | 4 | — |
| 30 | 10 | — |
| 32 | 6 | — |
| 38 | **0** | Only even k ≥ 24 with zero solutions (38 = 2×19) |
| 40 | 2 | — |
| 62 | 34 | — |

All odd k ≥ 25 tested: 0 solutions. Generalized XOR6 exists only at even k near threshold.
All 3 passing tuples at k=24 consist of exactly 3 antipodal pairs (w, -w). Min margin to sector boundary: 0.000000 rad — zero margin universally.

**Provenance:** Demo 65.

### XOR7 Scaling Data

| k | XOR7 solutions (narrow search) | XOR7 solutions (broader C(100,2) search) |
|---|--------------------------------|------------------------------------------|
| 91 | 1 | — |
| 120 | — | 5 |
| 127 | 1 (narrow) | 4 (broader) |

XOR7 minimum k: 91. Winner anatomy: 3 antipodal pairs + 1 lone weight at 90° (purely imaginary).
Scaling ratio: XOR7/XOR6 = 91/24 ≈ 3.8.

**Provenance:** Demo 65.

---

## 28. 24-Cell Geometry — Demo 66

### 24-Cell Vertex Decomposition

| Type | Count | Coordinates | Notes |
|------|-------|------------|-------|
| Axis-aligned | 4 | ±1, ±i, ±j, ±k | One nonzero component |
| Edge-midpoint | 12 | two components at ±1/√2 | — |
| Body-diagonal | 8 | all components at ±½ | — |
| **Total** | **24** | 4+12+8=24 | Vertices of the 24-cell |

With negatives: 48 quaternions = binary octahedral group.

**Provenance:** Demo 66.

### Geographic Grid vs Voronoi Solution Counts (XOR6)

| Method | Cells | Solutions | Notes |
|--------|-------|-----------|-------|
| 3×3 geographic | 9 | 0 | — |
| 4×4 geographic | 16 | 0 | — |
| 5×5 geographic | 25 | 0 | — |
| 6×6 geographic | 36 | 3 | Antipodal-pairs only |
| 8×8 geographic | 64 | 34 | Antipodal-pairs only |
| 24-cell Voronoi | 25 cells | **35** | Beats 64-cell geographic grid |

Full C(24,6)=475,020 unrestricted search: 0 solutions at any resolution. Antipodal constraint is absolute.

**Provenance:** Demo 66.

### F4 Orbit Decomposition (35 Voronoi XOR6 Solutions)

| Orbit | Size | Type | Notes |
|-------|------|------|-------|
| 1 | 12 | Edge-dominant | Edge cells → label 1 |
| 2 | 7 | Edge-dominant | — |
| 3 | 5 | Body-dominant | Axis/body cells → label 1 |
| 4 | 5 | Body-dominant | — |
| 5 | 4 | Edge-dominant | — |
| 6 | 2 | Body-dominant | — |
| **Total** | **35** | — | Edge-dominant: 22; body-dominant: 13 |

F4 symmetry group order: 576 (two-sided symmetry of 24-cell).

**Provenance:** Demo 66.

### Complementary Hash Collision Table (Bracket vs Quaternion)

| Category | Count | Percentage |
|---------|-------|------------|
| Same bracket, same quaternion (agree both) | 16,047 | 12.86% |
| Same bracket, differ in quaternion | 450 | 0.36% |
| Same quaternion, differ in bracket | 8,424 | 6.75% |
| Differ on both | 99,829 | 80.03% |

Total: 124,750 braid pairs. Quaternion resolves 450 bracket collisions; bracket resolves 8,424 quaternion collisions.

**Provenance:** Demo 66.

### Universal 7/7/11 Cell Structure

All 35 Voronoi XOR6 solutions: exactly 7 cells labeled 0, 7 cells labeled 1, 11 cells unused.
Zero cell always labeled 0. 3 antipodal pairs define 3 great circles on S³; 64 subset sums touch only 14/25 Voronoi cells.

**Provenance:** Demo 66.

---

## 29. Coordinate System Zoo — Demo 67

### Activation Comparison (XOR6 Solutions per Cell Count)

| Coordinate system | Cells | Solutions | Sol/cell | Notes |
|------------------|-------|-----------|---------|-------|
| Custom 13-dir S² Voronoi | 14 | 36 | 2.57 | Data-derived directions |
| 24-cell S³ Voronoi (Demo 66) | 25 | 35 | 1.40 | — |
| Hopf base (8×8) | 64 | 292 | 4.56 | Dominator at high resolution |
| Eigenvector lat/lon (8×8) | 64 | 204 | 3.19 | — |
| Cayley-Klein (8×8) | 64 | 106 | 1.66 | Worst at all resolutions |
| Octahedral Voronoi | 6 | 0 | — | Platonic solid: fails |
| Icosahedral Voronoi | 12 | 0 | — | Platonic solid: fails |
| Cuboctahedral Voronoi | 12 | 0 | — | Platonic solid: fails |
| Hopf phase-only | all k | 0 | — | Completely inert |
| Complex S¹ k=24 (Demo 65) | 25 | 3 | 0.12 | — |

**Provenance:** Demo 67.

### Eigenvalue Decomposition

| Property | Value |
|---------|-------|
| Distinct eigenvalue angles | 4 (0°, 45°, 60°, 90°) |
| Distinct eigenvector directions (mod ±) | 13 (3 axis + 6 edge-midpoint + 4 body-diagonal) |
| (angle, direction) pairs | 17 |
| Quaternions (mod ±) | 24 |
| Bracket values | 100 |

Axis-aligned directions shared across two eigenvalue angles; body-diagonals appear only at 60°; edge-midpoints only at 90°.

**Provenance:** Demo 67.

### Hopf Phase Zero Results (all k tested)

| k | Hopf phase-only solutions |
|---|--------------------------|
| 4 | 0 |
| 8 | 0 |
| 12 | 0 |
| 16 | 0 |
| 24 | 0 |
| 32 | 0 |

Hopf fiber carries zero DKC computational information at all resolutions. All information lives in the S² Hopf base.

**Provenance:** Demo 67.

### Eigenvalue k-Sector Solutions

| k | Solutions |
|---|-----------|
| 4 | 0 |
| 6 | 0 |
| 8 | 4 (first solutions) |
| 12 | 64 |
| 16 | 76 |
| 24 | 72 |
| 32 | 80 (peak) |

**Provenance:** Demo 67.

### Cross-Demo Cell Count Progression

| Demo | Space | Cells | XOR6 Solutions |
|------|-------|-------|----------------|
| D65 | Complex S¹ | 25 sectors | 3 |
| D66 | Quaternionic S³ | 25 Voronoi | 35 |
| D67 | Eigenvector S² | 14 Voronoi | 36 |
| D68 | R² (visualization) | — | — |

The 2D S² projection is the most efficient; full S³ is redundant.

**Provenance:** Demos 65–68.

---

## 30. Stereographic DKC — Demo 68

### S² Metric vs Euclidean Metric Comparison

| Metric for nearest-neighbor | Solutions | Cells | Notes |
|---------------------------|-----------|-------|-------|
| S² great-circle (dot product) | 36 | 14 | Exact match to Demo 67 |
| R² Euclidean distance | 0 | 14 | Broken by area distortion |

7/7/11 cell label structure preserved in R² with S² metric. Computation is intrinsically curved.

**Provenance:** Demo 68.

### Planar Grid Results

| Grid | Cells | Solutions |
|------|-------|-----------|
| 4×4 | 16 | 0 |
| 4×8 | 32 | 2 |
| 6×6 | 36 | 4 |
| 8×4 | 32 | 7 |
| 8×8 | 64 | 33 |

vs 14 cells / 36 solutions with S² Voronoi. Grids cover empty bounding-box regions.

**Provenance:** Demo 68.

### Conformality Distortion (S² → R²)

| Property | Value |
|---------|-------|
| Largest cell (cell 8) area fraction | 17% (4.33 normalized) |
| Smallest cell (cell 1) area fraction | 1% (0.26 normalized) |
| Area ratio largest/smallest | **16.5×** |
| Solution count change | 0 (36 → 36) |
| Metric used | S² dot product (not Euclidean) |

Stereographic projection is angle-preserving but not area-preserving. 16.5× distortion does not affect solutions when S² metric is used.
Bounding box: x∈[−2.41, 1.37], y∈[−1.00, 2.41]; round-trip precision ~1e-15.

**Provenance:** Demo 68.

---

## 31. Multi-Root Catalog Explosion and Nesting — Demo 69

### Catalog Explosion by Root

| Root | Quaternions enumerated | Eigenvector directions | Dir/quat ratio |
|------|----------------------|----------------------|---------------|
| ζ_8 | 24 | 13 | 0.54 |
| ζ_16 | 7,952 | 3,457 | 0.43 |
| ζ_32 | 32,768+ (cap limited) | 4,096+ (cap limited) | 0.13 |

**Provenance:** Demo 69.

### Direction Nesting Results (ζ_8 → ζ_{2N})

| Source → Target | Matched | Avg drift | Max drift | Classification |
|----------------|---------|-----------|-----------|----------------|
| ζ_8 → ζ_16 | 13/13 | 0.0000° | 0.0000° | RIGIDITY |
| ζ_8 → ζ_32 (large cap) | 8/13 | — | 2.07° | DRIFT (cap artifact) |
| ζ_8 → ζ_32 (small cap) | 6/13 | — | 10.9° | CAP ARTIFACT |

Algebraic proof: ζ_8 = (ζ_16)² and ζ_8 = (ζ_32)⁴; squaring a quaternion preserves rotation axis.

**Provenance:** Demo 69.

### XOR6 Trivialization by Root (Natural Voronoi)

| Root | Voronoi cells | Solutions | Sol/cell | % of all triples |
|------|--------------|-----------|---------|-----------------|
| ζ_8 | 14 | 36 | 2.57 | 0.007% |
| ζ_16 | 3,458 | 499,367 | 144.4 | 90.6% |
| ζ_32 | 4,097 | 493,654 | 120.5 | 89.6% |

At 3,457+ cells the pigeonhole effect dominates — topology is irrelevant.

**Provenance:** Demo 69.

### Fixed-Resolution Comparison (16 Cells)

| Root | Solutions at 16 cells |
|------|----------------------|
| ζ_16 | 450 |
| ζ_32 | 393 |

Finer roots not monotonically better at fixed resolution. Which directions land in which cells matters more than direction count.

**Provenance:** Demo 69.

---

## 32. Knot Notes — Demo 70

### 4 Knot Notes Table

| Eigenvalue angle | Cents | Note | Trace | Interval from C |
|-----------------|-------|------|-------|----------------|
| 0° (identity) | 0 | C | 2.000 | Unison |
| 45° | 300 | Eb | √2 ≈ 1.414 | Minor 3rd |
| 60° | 400 | E | 1.000 | Major 3rd |
| 90° | 600 | F# | 0.000 | Tritone |

Trace descends from maximum consonance (C, 2.000) to maximum dissonance (F#, 0.000).
6 pairwise intervals: m2 (45→60°), M2 (60→90°), m3 (0→45°, 45→90°), M3 (0→60°), tritone (0→90°).
Pearson r (pitch vs spatial distance): -0.0624 over 253 pairs; 72 same-note/different-direction pairs.
Music and space are orthogonal dimensions of a unit quaternion.
Garside element (s₁s₂)³ melody: Eb4 E4 F#4 E4 Eb4 C4.

**Provenance:** Demo 70.

### XOR6 by Polyhedron / Voronoi Type

| Voronoi type | Cells | XOR6 solutions |
|-------------|-------|----------------|
| Eigenvector 13-dir | 14 | 36 |
| Icosahedron | 6 (after antipodal collapse) | 0 |
| Dodecahedron | 10 (after antipodal collapse) | 0 |

Wrong symmetry group → zero solutions. Only the data-intrinsic binary octahedral structure works.
Tritone-antipodal correspondence: antipodal dodecahedron faces carry tritone pairs (C/F#, G/Db, D/Ab, A/Eb, E/Bb, B/F) — dissonance IS the antipodal map.

**Provenance:** Demo 70.

---

## 33. Spectral DKC — Demo 71

### Spectral Power by Mode (36 XOR6 Winners)

| Mode l | Avg power fraction | Range | Notes |
|--------|-------------------|-------|-------|
| Odd modes (all) | 0% | — | Antipodal symmetry forces zero |
| l=2 | 4.1% | 0.02%–17.6% | Highest inter-winner variance |
| l=4 | ~6.0% | 5.6%–6.3% | Low spread, secondary peak |
| l=6 | 80.7% | 70.0%–83.9% | **Dominant mode; structural invariant** |
| l=8 | — | — | — |
| l=12 | ~6.4%–7.3% | — | Tertiary peak |

Best single winner: 83.9% in l=6. All 36 winners share same spectral shape — spectrum is a structural invariant of XOR6 solutions.

**Provenance:** Demo 71.

### Bandwidth Phase Transition

| Bandwidth l_cut | Winner recovery |
|----------------|----------------|
| ≤5 | 0% |
| 6 | **100%** |

Phase transition is exact and sharp — no partial recovery. Proof: mode l has (2l+1) spherical harmonic components; 2×6+1 = 13 = number of eigenvector directions. At l=5: 11 components trying to address 13 regions (underdetermined). At l=6: exactly 13 (determined).

**Provenance:** Demo 71.

### Nyquist vs Actual Compression

| Property | Value |
|---------|-------|
| Nyquist prediction for l=6 | (6+1)² = 49 cells |
| Actual cells needed | 14 |
| Super-Nyquist compression | 3.5× |
| Explanation | Compressed sensing: binary function at 13 known support positions |

**Provenance:** Demo 71.

### Trivialization Prediction by Root

| Root | Eigenvector directions N | Predicted min bandwidth l | Nyquist cell count |
|------|------------------------|--------------------------|-------------------|
| ζ_8 | 13 | 6 | 49 (actual: 14) |
| ζ_16 | 3,457 | 1,728 | ~2,989,441 (1,729²) |

ζ_16 prediction l≥1728 explains the ~90% XOR6 solution rate in Demo 69 (pigeonhole saturation at bandwidth this high).

Direction density spectrum (von Mises-Fisher, κ=50): l=0 (51.6%), l=8 (35.8%) — spectrum of WHERE directions are, not what computation does.
Musical mapping: l=6 → 200 cents (D, whole tone); computation lives ABOVE the eigenvalue-resonant modes l=2,3,4 (F#, E, Eb).
Quadrature grid: 64θ × 128φ = 8,192 points. Orthonormality max error: 6.82e-14.

**Provenance:** Demo 71.

---

---

## 34. Spherical Design Data — Demo 72

### T-Design Residuals per Harmonic Level (13 Eigenvector Directions)

| Mode l | Residual max|S_{l,m}|| | Status for DKC |
|--------|------------------------|----------------|
| l=1 (odd) | 2.435 | First failure; not DKC-relevant |
| l=2 (even) | 5.11e-16 (machine precision) | PASS — single exact pass |
| l=3 (odd) | large | Fail; odd mode, irrelevant |
| l=4 (even) | 0.576 | First DKC-relevant failure |
| l=5+ | large | Fail |
| l=6 (DKC bandwidth) | large | Fail; compressed sensing explains why this works anyway |

T-design parameter achieved: t=0 (directed). t_even=2; t_odd=0; undirected (26 points, both ±dir): t=3.
S³ full 48-element binary octahedral group: t=1. 24-cell vertices (binary tetrahedral subgroup): 5-design; additional 24 elements break at degree 2.
Delsarte maximum possible for N=13 points: t=7. Achieved: t=0 (worst possible).

**Provenance:** Demo 72 Parts A–E.

### Delsarte Bounds

| t | Min points for t-design on S² | Achieved by 13 dirs? |
|---|-------------------------------|----------------------|
| 1 | 3 | No (t=0) |
| 2 | 6 | No |
| 3 | 10 | No (undirected version yes) |
| 7 | 10 | No (max feasible with 13 points, not achieved) |

**Provenance:** Demo 72 Part E; Delsarte, Goethals, Seidel (1977).

### Perturbation Sensitivity (eps° perturbation → mean XOR6 count)

| Perturbation (mean°) | Mean XOR6 count | Notes |
|---------------------|-----------------|-------|
| 0° (unperturbed) | 36 | Baseline |
| 1° | 38.0 | Exceeds unperturbed — local not global optimum |
| 5° | 35.8 | Robust; approximate Voronoi cell boundary |
| 10° | 11.5 | Collapse begins |
| 20° | 2.1 | Near-total collapse |

**Provenance:** Demo 72 Part H.

### Optimized Design vs Eigenvector Comparison

| Configuration | t=6 design residual | XOR6 solutions |
|--------------|---------------------|----------------|
| Eigenvectors (binary octahedral) | 2.43 | **36** |
| Stochastic-optimized 13 points | 0.93 (2.6× better geometry) | **4** |

Better geometric distribution gives fewer XOR6 solutions. Sampling quality and computational quality are anti-correlated.

**Provenance:** Demo 72 Part G.

---

## 35. Eigenvector Automaton Determinism — Demo 73

### Determinism Table (Multiplicative vs Additive vs Signed)

| Formulation | Transitions | Deterministic | Fraction | XOR6? |
|------------|-------------|---------------|----------|-------|
| Multiplicative braid product | 32 | 26 | 81.2% | NO (balanced in every cell) |
| Additive weight sum (unsigned 13 cells) | 58 | 48 | **82.8%** | YES |
| Signed partition (27 cells) | — | — | **87.1%** | YES |

All 36 winning triples have exactly 82.8% additive determinism — zero variation. Universal structural constant.

**Provenance:** Demo 73 Parts B, D, E, F, G.

### Distinct Partial Sums by Step (Additive Formulation)

| Step | Distinct partial sums | Notes |
|------|----------------------|-------|
| 0 | 2 | Initial |
| 1 | 3 | After first weight |
| 2 | 6 | — |
| 3 | 9 | — |
| 4 | 18 | — |
| 5 | 27 | Final (minimum state count for 100% determinism) |

27 signed cells needed for direction-level 87.1% determinism. 17 of 27 cells occupied at step 5.
Cell collisions (same cell, different partial sum history): 1,296 instances across all steps.

**Provenance:** Demo 73 Parts E, F.

---

## 36. Invariant Hierarchy — Demo 74

### Equivalence Class Counts (Incomparability Table)

| Invariant | Equivalence classes | Notes |
|-----------|---------------------|-------|
| Cell (Voronoi) alone | **14** | Very coarse |
| Quaternion catalog | **24** | Natural mid-level |
| Kauffman bracket alone | **100** | Standard invariant |
| (Bracket, Cell) combined | **119** | 19% improvement over bracket alone |

**Provenance:** Demo 74 Part C.

### Collision Separation at Quaternion Catalog Level

| Pair type | Total pairs | Separated by other invariant | Fraction |
|-----------|-------------|------------------------------|----------|
| Same bracket, different quaternion | 30 | 24 separated by cell | 80% |
| Same cell, different quaternion | 13 | 7 separated by bracket | 54% |
| Same cell AND same bracket | 6 | Unseparated by either | — |

At braid level: 58,078 same-bracket pairs → 56,825 (97.8%) in different cells; 1,253 in same cell (2.2% unseparated).

**Provenance:** Demo 74 Parts A, B, F.

### 6 Computational Type Vectors

| Type | Cells | Quaternion count | Description |
|------|-------|-----------------|-------------|
| 0 | Cell 0 | 3 | Axis σ₁ |
| 1 | Cell 1 | 3 | Axis σ₂ |
| 2 | Cells 2,3,4,5 | 8 | Body 60° cells |
| 3 | Cells 6,7,8,9,11,12 | 6 | Edge cells |
| 4 | Cell 10 | 3 | Third axis |
| 5 | Cell 13 | 1 | Identity |

XOR6-separating winners: 33/36 (92%). Total XOR6-separated bracket collisions across 36 winners: 186.
Refinement chain: 43 bracket groups → 61 type subgroups → 168 quaternion subgroups.

**Provenance:** Demo 74 Parts D, G, I.

---

## 37. Binocular DKC Channel Inventory — Demo 75

### Five-Channel Data Table

| Channel | Entropy (bits) | XOR6 perfect? | Notes |
|---------|---------------|---------------|-------|
| Cell (Voronoi S²) | 1.000 | YES (all 36) | Coarsest channel |
| Eigenvalue angle | 2.004 | YES (all 36) | Subsumes cell |
| (Angle, Cell) joint | 2.282 | YES | 2.3× capacity vs cell |
| Magnitude |S|| | 1.839 | 44% (16/36 triples) | Not reliable predictor |
| Multiplicative product | 4.241 | NO (0/36) | Preserves 24-cell group structure |

**Provenance:** Demo 75 Parts B–H.

### Eigenvalue Angles and Parity (Sum Quaternions, 36 Winning Triples)

| Eigenvalue angle | Origin | XOR parity role |
|-----------------|--------|-----------------|
| 0° | Canonical octahedral | — |
| 30° | Non-canonical (addition-only) | — |
| 35.264° (tetrahedral) | Non-canonical (addition-only) | — |
| 45° | Canonical octahedral | — |
| 60° | Canonical octahedral | — |
| 65.905° | Non-canonical (addition-only) | — |
| 90° | Canonical octahedral | — |

Total distinct: 7 (3 non-canonical, arising only through vector addition breaking 24-cell group closure).
Multiplicative channel: only 4 canonical angles; 0 XOR6 solutions.

**Provenance:** Demo 75 Parts C, H.

### |S| Magnitude Quantization (√n Pattern)

| |S| value | Present? | Notes |
|-----------|---------|-------|
| √0 = 0 | YES | — |
| √1 = 1 | YES | — |
| √2 | YES | — |
| √3 | YES | — |
| √4 = 2 | YES | — |
| √5 | **NO** | Unreachable from 24-cell inner products {-1,-½,0,½,1} |
| √6 | YES | — |

H(|S|) = 1.839 bits. 8 distinct triple families based on angle function; group 0 is largest (8 triples).

**Provenance:** Demo 75 Parts G, E.

---

## 38. 6-vs-8 Input Comparison — Demo 76

### XOR6 vs XOR8 Under S² Voronoi Activation

| Property | XOR6 (3 weights) | XOR8 (4 weights) |
|----------|-----------------|-----------------|
| Candidates | C(24,3) = 2,024 | C(24,4) = 10,626 |
| Winners | 36 (1.78%) | **0 (0%)** |
| Masks per cell | 64/14 ≈ 4.6 | 256/14 ≈ 18.3 |
| Min conflict cells (best case) | — | 2 (11 quadruples, 0.1%) |
| Best accuracy | 100% | 50.0% (random chance) |

**Provenance:** Demo 76 Parts A–C.

### Algebraic Structure Breakdown (3-term vs 4-term Sums)

| Property | 3-term sums (XOR6) | 4-term sums (XOR8) |
|----------|-------------------|-------------------|
| Distinct eigenvalue angles | **7** (algebraic, clean) | **86** (near-continuous) |
| Distinct |S| values | **6** (√{0,1,2,3,4,6}) | **28** (including non-algebraic like 0.4142) |
| Algebraic structure | CLEAN | BROKEN |
| ζ_16 equivalent masks/cell (XOR8) | — | 256/3,458 ≈ 0.07 (trivial) |

**Provenance:** Demo 76 Parts D–E.

---

## 39. 14-Activation Comparison Table — Demo 77

### XOR8 Accuracy and Winners by Activation (10,626 quadruples)

| Activation | Cells | Best accuracy | Winners | Notes |
|-----------|-------|--------------|---------|-------|
| S² Voronoi | 14 | 50.0% | 0 | Demo 76 result |
| Sec(4) | 4 | 77.7% | 0 | — |
| Sec(8) | 8 | 85.2% | 0 | — |
| S³ Voronoi (24-cell) | 24 | 90.2% | 0 | Worse than Sec(16) despite more cells |
| Sec(10) | 10 | 92.6% | 0 | — |
| Sec(12) | 12 | 90.6% | 0 | Non-monotone |
| Polar 14×3 (dir×mag) | 42 | 93.0% | 0 | — |
| Sec(4)×Vor | 56 | 93.8% | 0 | — |
| Polar 14×2 | 28 | 93.8% | 0 | — |
| Sec(16) | 16 | 96.9% | 0 | — |
| Sec(6)×Vor | 84 | 96.9% | 0 | — |
| Polar 14×6 | 84 | **99.2%** | 0 | One misclassified pair |
| **Sec(8)×Vor** | **112** | **100.0%** | **6** | **Phase transition winner** |

Phase transition: 0 winners at 84 cells (Sec6×Vor), 6 winners at 112 cells (Sec8×Vor). Critical threshold ~100 cells.

**Provenance:** Demo 77 Part D, accuracy table.

### XOR8 Winning Quadruples (6 total)

| Quadruple | XOR6-winning triple subsets | Eigenvalue angles used |
|-----------|----------------------------|----------------------|
| [0,1,9,23] | 2 | 45° and 90° only |
| [0,1,18,22] | 2 | 45° and 90° only |
| [2,4,5,23] | 2 | 45° and 90° only |
| [2,9,19,21] | 2 | 45° and 90° only |
| [4,5,15,16] | 2 | 45° and 90° only |
| [17,19,20,21] | 2 | 45° and 90° only |

Every winner: exactly 2 XOR6-winning triple subsets. 12 of 24 embedded triples (50%) are XOR6 winners.
Dominant Voronoi cells: 0, 1, 10 — each appearing 6 times. 15 of 24 catalog quaternions participate.

**Provenance:** Demo 77 Parts D, F.

---

## 40. Phase Diagram Table — Demo 78

### XOR Hierarchy at ζ₈ (Sec(k)×Vor Activation)

| XOR level | Min cells (activation) | Winners | Masks/cell | Hit rate | Status |
|-----------|----------------------|---------|------------|---------|--------|
| XOR6 | 14 (Voronoi only) | 36 | 4.6 | 1.779% | SOLVED |
| XOR8 | 112 (Sec8×Vor) | 6 | 2.3 | 0.057% | SOLVED |
| XOR10 | — | **0** | — | 0% | WALL |
| XOR12+ | — | 0 | — | 0% | IMPOSSIBLE at ζ₈ |

Best XOR10 accuracy at ζ₈: 98.4% = 1008/1024 masks at k=10 and k=16.
Non-monotonicity: k=12 accuracy (96.9%) worse than k=10 (98.4%), mirroring Demo 50's pi/4 incommensurability.

**Provenance:** Demo 78 Parts A–E.

### Recursive Shadow Structure (XOR6 → XOR8)

| Property | Value |
|----------|-------|
| XOR8 winners containing exactly 2 XOR6 sub-triples | 6/6 (100%) |
| Shadows sharing Voronoi cell with parent member | 12/12 (100%) |
| All shadows half-angle | exactly 45° |
| Shadow pairs (share direction+half-angle, differ in orientation) | {0,1}, {4,5}, {19,21} |

**Provenance:** Demo 78 Part E.

---

## 41. ζ₈ vs ζ₁₂ Comparison — Demo 79

### Root of Unity Capacity Comparison (256-entry cap)

| Property | ζ₈ | ζ₁₂ (256 entries) |
|---------|-----|------------------|
| Quaternions | 24 (finite, closes at round 4) | 256 (capped; group continues) |
| Eigenvector directions | 13 | 64 (4.9×) |
| Distinct half-angles | 4 | 12 (3×) |
| Voronoi cells | 14 | 65 (4.6×) |
| XOR6 winners | 36 | 1024+ (capped) |
| XOR8 winners | 6 | 1024+ (capped) |
| XOR10 winners | 0 | **124** |
| XOR12 winners | 0 | 0 (truncation artifact) |

**Provenance:** Demo 79 Parts A–C.

### ζ₁₂ Expanded Catalog (4096 entries)

| Property | Value |
|----------|-------|
| Quaternions | 4096+ |
| Directions | 512 |
| Half-angles | 43 |
| XOR12 winners | 50+ (first 11 of 124 XOR10 parents searched) |
| Every XOR12 winner | requires at least 1 entry with index > 256 |

ζ₁₂ group closure rounds: 5 → 17 → 51 → 127 → 275 → 564 → 1140 → 2292 → 4096+ (roughly doubling each round).
Non-nesting: 3/24 ζ₈ quaternions in ζ₁₂ catalog (12%); 2/13 ζ₈ directions in ζ₁₂ (15%).

**Provenance:** Demo 79 Parts C, D, E.

---

## 42. Group Finiteness Survey — Demo 80

### Seven Roots Tested

| Root | θ | Elements | Closure rounds | Directions | Half-angles | ADE type | Finite? |
|------|---|---------|---------------|------------|-------------|----------|---------|
| ζ₄ | π/2 | 4 | 2 | 3 | 2 | Binary Dihedral 2D₂ | **YES** |
| ζ₆ | π/3 | >4096 | 10 (cap) | 512 | 37 | — | NO |
| ζ₈ | π/4 | 24 | 4 | 13 | 4 | Binary Octahedral (E₇) | **YES** |
| ζ₁₀ | π/5 | >4096 | 7 (cap) | 512 | 71 | — | NO |
| ζ₁₂ | π/6 | >4096 | 8 (cap) | 512 | 43 | — | NO |
| ζ₁₆ | π/8 | >4096 | 8 (cap) | 512 | 51 | — | NO |
| ζ₂₀ | π/10 | >4096 | 7 (cap) | 512 | 67 | — | NO |

Finite: 2. Infinite: 5. Non-monotone: ζ₆ is infinite despite sitting between the two finite cases.
Quantum dimension [2]_q at ζ₈: exactly 0 (q=i, [2]_q = i + 1/i = 0).
ζ₁₀ (Fibonacci anyons territory): INFINITE — not the binary icosahedral group (E₈, order 120).
Angle algebraic degrees: cos(π/2)=0 (rational, finite), cos(π/4)=√2/2 (degree 2, finite), cos(π/3)=1/2 (rational, INFINITE), cos(π/5)=φ/2 (degree 2, INFINITE) — degree alone does not predict finiteness.

**Provenance:** Demo 80.

---

## 43. Capacity Scaling Law — Demo 81

### ζ₁₂ Snapshot Table (9 rounds: catalog, dirs, angles, max XOR)

| Round | Catalog size | Directions | Angles | Max XOR level |
|-------|-------------|------------|--------|--------------|
| 0 | 5 | 2 | 2 | 6 |
| 1 | 17 | 6 | 4 | 6 |
| 2 | 51 | 22 | 6 | **8** |
| 3 | 127 | 46 | 10 | 8 |
| 4 | 275 | 114 | 12 | **10** |
| 5 | 564 | 239 | 16 | 10 |
| 6 | 1140 | 507 | 21 | **12** |
| 7 | 2292 | 512 (saturated) | 35 | 12 |
| 8 | 4096 | 512 (saturated) | 43 | 12 |

Logarithmic scaling law: max_xor ≈ 0.62 × log₂(catalog_size) + 4.6
Each doubling of catalog size adds ~0.62 to max XOR. Each +2 XOR level costs ~3.2 doublings (~10× more entries).

**Provenance:** Demo 81.

### XOR Transition Costs

| Transition | Catalog sizes | Doublings required |
|-----------|--------------|-------------------|
| XOR6 → XOR8 | ~5 → ~51 | 3.35 |
| XOR8 → XOR10 | ~51 → ~275 | 2.43 |
| XOR10 → XOR12 | ~275 → ~1140 | 2.05 |

Decreasing transition gap — possibly sub-logarithmic O(log N / log log N). Requires XOR14 data to confirm.
Direction saturation: MAX_DIR=512 reached at round 7 (catalog=2292).

Predictions: XOR14 at ~38,000 entries (~11 rounds); XOR16 at ~615,000 entries; XOR20 at ~160M; XOR32 at ~10¹⁴ (infeasible).

**Provenance:** Demo 81.

---

## 44. Depth Census and Shallow/Strided/Deep Comparison — Demo 82

### Depth Census (ζ₁₂ closure, per round)

| Depth (round) | New entries | Cumulative entries |
|---------------|------------|-------------------|
| 0 | 5 | 5 |
| 1 | 12 | 17 |
| 2 | 34 | 51 |
| 3 | 76 | 127 |
| 4 | 148 | 275 |
| 5 | 289 | 564 |
| 6 | 576 | 1140 |
| 7 | 1152 | 2292 |
| 8 | 1804 | 4096 |

Round 8 alone is 44% of all entries. Growth: roughly 2× per round (exponential).
Linear depth law: max_xor ≈ depth + 6 (each unit of crossing depth adds one XOR level).

**Provenance:** Demo 82.

### Shallow vs Strided vs Deep Comparison at 127 and 564 Entries

| Subset | Size | Directions | Angles | Max XOR |
|--------|------|-----------|--------|---------|
| Shallow-127 (first N entries) | 127 | — | — | 8 |
| Strided-127 (every K-th) | 127 | — | — | 10 |
| Deep-127 (last N entries) | 127 | — | — | 10 |
| Shallow-564 | 564 | — | — | 10 |
| Strided-564 | 564 | **512** (full) | **43** (full) | 10 |
| Deep-564 | 564 | **476** (less) | **19** (less!) | **12** |

Algebraic coherence beats vocabulary: Strided-564 has maximum possible vocabulary (512 dirs, 43 angles matching the full 4096-entry catalog) yet cannot reach XOR12. Deep-564 with strictly less vocabulary does reach XOR12. Deep entries are 2× more efficient.

**Provenance:** Demo 82 Parts B, C.

### Winner Mean Depth (Monotone Increase)

| XOR level | Mean entry depth in winners |
|-----------|---------------------------|
| XOR6 | 0.52 |
| XOR8 | 0.63 |
| XOR10 | 1.00 |
| XOR12 | 1.98 |

Sample XOR12 winner: indices [0,1,3,6,52,388], depths [0,0,0,1,3,5] — shallow core + progressively deeper harmonics.
ζ₈ control: 24 entries, 4 rounds, max depth 3, max XOR 8; same depth→XOR pattern confirmed.

**Provenance:** Demo 82 Part D.

---

## 45. Framing as Computational Content — Demo 83

### Bracket vs Jones Capacity by Root

| Root | Catalog size | Bracket max XOR | Jones max XOR | Loss |
|------|-------------|-----------------|---------------|------|
| ζ₁₂ | 4096 | XOR12 | XOR10 | -2 |
| ζ₈ | 24 | XOR8 | XOR6 | -2 |

The loss is exact and constant: bracket capacity = N, Jones capacity = N-2 at every root tested. The "N-2" pattern from Demo 79 was Jones capacity, not a bracket truncation artifact.

**Provenance:** Demo 83 Parts A–C.

### Information Collapse Under Jones Normalization (ζ₁₂)

| Quantity | Bracket | Jones | Change |
|---------|---------|-------|--------|
| Distinct quaternions | 4096 | 2710 | -33.8% |
| Directions (S²) | 512 | 512 | unchanged |
| Distinct angles | 43 | 31 | -28% |

Framing lives in angles, not directions. The writhe-dependent phase rotates quaternions in the angular coordinate only; the directional vocabulary is entirely preserved.

**Provenance:** Demo 83 Part B.

### Writhe Census (ζ₁₂, 4096 entries)

| Statistic | Value |
|-----------|-------|
| Writhe range | [-9, 9] |
| Nonzero writhe fraction | 91.4% |
| Mean writhe | 1.697 (positive bias from generator structure) |
| Depth-writhe correlation r | 0.139 |

**Provenance:** Demo 83 Data section.

### Mean |Writhe| by Crossing Depth

| Depth | Mean |writhe| |
|-------|--------------|
| 0 | 0.80 |
| 1 | 1.33 |
| 2 | 1.82 |
| 4 | 2.14 |
| 6 | 2.61 |
| 8 | 3.19 |

Mean |writhe| grows linearly with depth. Low correlation (r=0.139) means these are genuinely independent resources: a deep entry can have low writhe (crossings cancel) or high writhe (crossings accumulate).

**Provenance:** Demo 83 Data section.

### Mixed-Writhe Winner Fractions by XOR Level

| XOR level | Mixed-writhe fraction | Count |
|-----------|----------------------|-------|
| XOR6 | 41% | 13/32 |
| XOR8 | 25% | 8/32 |
| XOR10 | 9% | 3/32 |
| XOR12 | 12% | 4/32 |

Higher-capacity XOR levels use both positive and negative writhe entries. Computation exploits chirality variation, not just magnitude. Writhe-only XOR6 winners: 32 (writhe as standalone unit cannot reach XOR8).

**Provenance:** Demo 83 Part D.

---

## 46. Null States and Logarithmic Structure — Demo 84

### Null Count by Crossing Depth (ζ₈, 24 entries)

| Depth | Total entries | Null entries | Null fraction |
|-------|--------------|--------------|--------------|
| 0 | 5 | 0 | 0% |
| 1 | 10 | 2 | 20% |
| 2 | 8 | 6 | 75% |
| 3 | 1 | 1 | 100% |

Every maximally-deep entry in the ζ₈ catalog is bracket-null (Re(q)=0). Null fraction increases monotonically with depth.

**Provenance:** Demo 84 Part A.

### Gap Theorem: Discrete |Re(q)| Spectrum

| |Re(q)| value | Present in catalog | Notes |
|-------------|-------------------|-------|
| 0 | YES (null) | 9 entries |
| 0.5 | YES | nearest non-null; hard gap of 0.5 from null |
| 0.707 (1/√2) | YES | — |
| 1.0 | YES | — |

Exactly 4 discrete values. No continuum — reflects algebraic structure of the binary octahedral group.

**Provenance:** Demo 84 Part B.

### Direction Geometry (ζ₈ catalog, 13 total S² directions)

| Direction type | Count | Components | Notes |
|---------------|-------|-----------|-------|
| Null-only | 6 | ±1/√2 (cube edge midpoints) | 1.00 dir/entry — maximally efficient |
| Non-null-only | 4 | ±1/√3 (tetrahedral axes) | 0.47 dir/entry |
| Shared | 3 | coordinate axes | appear in both null and non-null entries |

Null entries are disproportionately important: each null entry uniquely covers a distinct direction unavailable to non-null entries.

**Provenance:** Demo 84 Part E.

### Capacity Comparison: Full vs Null vs Non-Null Subsets

| Subset | Size | Directions | Max XOR | vs random-15 baseline |
|--------|------|-----------|---------|----------------------|
| Full catalog | 24 | 13 | XOR8 | — |
| Non-null-only | 15 | 7 | XOR6 | **below baseline** |
| Null-only | 9 | 9 | XOR6 | — |
| Random-15 mean | 15 | — | 7.8 | baseline |

Removing nulls destroys one full XOR level. The non-null-only subset performs strictly worse than random subsets of the same size — nulls are disproportionately important, not incidental.

**Provenance:** Demo 84 Parts C, C.3.

### k_sec Sweep (Null-Only XOR6 Count)

| k_sec | Null-only XOR6 count | Notes |
|-------|---------------------|-------|
| 1 | flat | null-only beats non-null-only on direction alone |
| 2 | flat | — |
| 4 | flat | — |
| 6 | flat | — |
| 8 | flat | — |

Count is flat across all tested k_sec values, confirming all null-only entries share half-angle exactly 90° (single point on S¹). Their entire computational contribution comes from S² directional diversity.

**Provenance:** Demo 84 Part C.4.

### Cross-Root Null Fraction

| Root | Total entries | Null entries | Null fraction |
|------|--------------|--------------|--------------|
| ζ₄ | 4 | 3 | 75% |
| ζ₈ | 24 | 9 | 37.5% |
| ζ₁₂ (truncated at 4096) | 4096 | 121 | 3.0% |

As group order grows, null fraction decreases sharply. ζ₁₂ direction breakdown: 29 null-only, 453 non-null-only, 30 shared. The ζ₈ regime is where nulls represent a structurally significant fraction.

**Provenance:** Demo 84 Part F.

---

## 47. Indecomposability Parameter b — Demo 85

### TL Algebra and Standard Module Dimensions

| n | dim(TL_n) = C_n | dim(W_{n,0}) = C_{n/2} |
|---|-----------------|----------------------|
| 2 | 2 | — |
| 3 | 5 | — |
| 4 | 14 | 2 (= C_2) |
| 5 | 42 | — |
| 6 | 132 | 5 (= C_3) |
| 8 | — | 14 (= C_4) |
| 10 | — | 42 (= C_5) |
| 12 | — | 132 (= C_6) |

**Provenance:** Demo 85.

### b Value Extraction at TL_4

| Quantity | Value |
|----------|-------|
| b_delta (regular rep) | -5/2 |
| 0-TL sector dim | 4 (diagrams: id, e₁, e₂, e₁·e₂) |
| b_PR = b_delta / dim(0-TL) | -5/8 |
| Accuracy | \|b_PR - (-5/8)\| < 10⁻¹² |

**Provenance:** Demo 85.

### Valuation Condition (Leading Coefficients)

| Size | p_Tt | p_tt | 2·p_Tt | Condition p_tt = 2·p_Tt | Result |
|------|------|------|--------|--------------------------|--------|
| TL_4 | 1 | 2 | 2 | 2 = 2 | SATISFIED — b finite |
| TL_6 | 3 | 4 | 6 | 4 ≠ 6 | FAILS — diverges as δ⁻² |

TL_6: 0/14 safe kernel directions; 25-fold multiplicity (dim(0-TL)² = 25) does not rescue valuation.

**Provenance:** Demo 85.

### Standard Module Jordan Block Analysis (H on W_{n,0})

| n | dim(W_{n,0}) | Jordan blocks? | Min poly square-free? |
|---|-------------|----------------|----------------------|
| 4 | 2 | NO | YES |
| 6 | 5 | NO | YES |
| 8 | 14 | NO | YES |
| 10 | 42 | NO | YES |
| 12 | 132 | NO | YES |

Standard modules are fully diagonalizable at every tested size. Jordan blocks exist only on projective covers (P_{0,0}).

**Provenance:** Demo 85.

### Projective Cover P_{0,0} Structure

| Size | dim(P_{0,0}) | Catalan pattern | Composition | Char poly | Jordan blocks at |
|------|-------------|----------------|-------------|-----------|-----------------|
| TL_4 | 5 | C_3 | 0-TL:2 + 2-TL:3 | x(x²-2)² | λ = ±√2 |
| TL_6 | 14 | C_4 | — | x²(u-1)(u-3)(u²-8u+4)² where u=x² | λ = 0, ±(√3+1), ±(√3-1) |
| TL_8 | 42 | C_5 | — | — | **NONE** (completely semisimple) |

P_{0,0} dimension follows Catalan: dim(P_{n,0,0}) = C_{n/2+1}.

**Provenance:** Demos 85, 86.

### Spin Chain Sz=0 Dimensions

| n | dim(Sz=0 sector) |
|---|-----------------|
| 4 | 6 |
| 6 | 20 |
| 8 | 70 |

**Provenance:** Demo 85.

### Fixed-Point Form Gauge Analysis (TL_4)

| Matrix | Value | Meaning |
|--------|-------|---------|
| M (quadratic coefficient) | 0 | b_fixpt is purely affine (flat manifold) |
| E (linear denominator) | 0 | No unique critical point |
| Gauge freedom | 3-dimensional | Both -2 and -5/8 reachable on the manifold |

The delta-parameterized Markov trace breaks the degeneracy and selects b = -5/8.

**Provenance:** Demo 85.

---

## 48. P_{0,0} Divergence Table — Demo 86

### Delta-Form Divergence on Single P_{0,0}

| Size | λ | Form | p_Tt | p_tt | 2·p_Tt | Gap | Result |
|------|---|------|------|------|--------|-----|--------|
| TL_4 | ±√2 | loop | 2 | 2 | 4 | -2 | DIVERGE |
| TL_4 | ±√2 | trace | 1 | 1 | 2 | -1 | DIVERGE |
| TL_6 | 0 | loop | 3 | 4 | 6 | -2 | DIVERGE |
| TL_6 | 0 | trace | 3 | 2 | 6 | -4 | DIVERGE |
| TL_6 | ±(√3+1) | loop | 3 | 3 | 6 | -3 | DIVERGE |
| TL_6 | ±(√3+1) | trace | 1 | 1 | 2 | -1 | DIVERGE |
| TL_6 | ±(√3-1) | loop | 3 | 3 | 6 | -3 | DIVERGE |
| TL_6 | ±(√3-1) | trace | 1 | 1 | 2 | -1 | DIVERGE |

Universal negative result: valuation condition p_tt = 2·p_Tt never holds on a single P_{0,0}. All eigenvalues, all sizes, both forms diverge.

Formal coefficient ratios (divergent): TL_4 λ=-√2 loop gives 1/2; TL_6 λ=0 loop gives -5/18. Neither matches -5/8 or -2.

**Provenance:** Demo 86.

---

## 49. Null Dispensability at ζ₁₂ — Demo 87

### Cross-Root Null Statistics

| Root | Total entries | Null entries | Null fraction | Directions | Null-only dirs | Shared dirs | Non-null dirs |
|------|-------------|-------------|--------------|-----------|---------------|------------|--------------|
| ζ₈ | 24 | 9 | 37.5% | 13 | 6 | 3 | 4 |
| ζ₁₂ | 4096 | 121 | 3.0% | 2043 | 67 | 54 | 1922 |

**Provenance:** Demos 84, 87.

### Capacity Comparison: Full vs Non-Null (ζ₁₂)

| Subset | Size | Directions | XOR6 | XOR8 | XOR10 | XOR12 |
|--------|------|-----------|------|------|-------|-------|
| Full catalog | 4096 | 2043 | 32 | 32 | 32 | 32 |
| Non-null only | 3975 | 1976 | 32 | 32 | 32 | 32 |

Removing 121 null entries has **zero** effect on computational capacity at ζ₁₂.

**Provenance:** Demo 87.

### Capacity Comparison: Full vs Non-Null (ζ₈, control)

| Subset | Size | Directions | XOR6 | XOR8 |
|--------|------|-----------|------|------|
| Full catalog | 24 | 13 | 32 | 32 |
| Non-null only | 15 | 7 | 32 | 4 |

Removing nulls at ζ₈ **destroys** XOR8 capacity (32 → 4 winners). Confirms Demo 84.

**Provenance:** Demo 87.

### Null Fraction Dilution Curve

| Root | Null fraction |
|------|--------------|
| ζ₄ | 75% |
| ζ₈ | 37.5% |
| ζ₁₂ | 3.0% |

**Provenance:** Demos 84, 87.

---

## 50. Anti-Correlation and Cuboctahedral Geometry — Demo 88

### Cuboctahedral Orbit Decomposition (13 ζ₈ Directions)

| Orbit | Count | Type | Mutual angle | DKC role |
|-------|-------|------|-------------|----------|
| Coordinate axes | 3 | Shared | 90° | — |
| Body diagonals | 4 | Non-null-only | 70.5° (arccos(1/3)) | Rigid computational anchors |
| Edge midpoints | 6 | Null-only | 60° or 90° | Flexible topological scaffolding |
| **Total** | **13** | — | — | — |

Pairwise angle statistics (78 pairs): min=35.3°, max=90.0°, mean=63.1°.

**Provenance:** Demo 88.

### Perturbation Sensitivity (ε=10°)

| Perturbation target | XOR6 change | Notes |
|--------------------|------------|-------|
| Null-only dirs | -2.4% (GAIN) | Perturbing nulls slightly improves computation |
| Non-null-only dirs | +8.0% loss | Non-nulls are fragile load-bearing anchors |
| All random dirs | +3.7% loss | Intermediate |

Prediction that null directions would be more sensitive was INVERTED.

**Provenance:** Demo 88.

### Design Optimization Results

| Method | Residual change | XOR6 change | Notes |
|--------|----------------|------------|-------|
| D72 Voronoi-only optimization | — | 36 → 4 (89% loss) | Catastrophic |
| D88 unconstrained gradient (5000 steps) | 2.23 → 1.06 (52% improvement) | 1239 → 1160 (6.4% loss) | k-ladder provides 14× better resilience |
| D88 constrained (nulls free, non-nulls clamped 2°) | 2.23 → 1.16 (48% improvement) | 1239 → 1204 (2.8% loss) | Best tradeoff |

**Provenance:** Demos 72, 88.

### Cross-Root Per-Direction Residual

| Root | Directions | Res(t=6) | Res/N (per-direction) |
|------|-----------|---------|----------------------|
| ζ₈ | 13 | 2.225 | 0.171 |
| ζ₁₂ | 2043 | 42.400 | 0.021 |

Per-direction residual at ζ₁₂ is 8× lower — explains null dispensability (Demo 87).

**Provenance:** Demo 88.

### Gram Eigenspectrum (13×13 |dot| Gram Matrix)

| Eigenvalue | Multiplicity | Notes |
|-----------|-------------|-------|
| 6.110 | 1 | Leading |
| 2.000 | 3 | — |
| φ = 1.618 | 2 | Golden ratio pair |
| 0.520 | 1 | — |
| -1/3 | 3 | — |
| -φ+1 = -0.618 | 2 | Golden ratio pair |
| -0.630 | 1 | — |

Golden ratio eigenvalue pairs arise from the specific angular ratios of ζ₈'s three Oₕ orbits.

**Provenance:** Demo 88.

---

## 51. Depth Law Mechanism Hypothesis Tests — Demo 89

### 8 Hypotheses Tested

| # | Hypothesis | Phase | Result | Status |
|---|-----------|-------|--------|--------|
| 1 | Pairwise coherence | 3 | Mean \|qdot\| flat at ~0.42 across all depths, = random | **KILLED** |
| 2 | Direction coverage | 4 | All 13 ζ₈ dirs covered by depth 2 | **KILLED** (as intercept explanation) |
| 3 | Intra > inter-depth coherence | 5 | Intra (0.4214) < inter (0.4254) — opposite direction | **KILLED** |
| 4 | Sector diversity | 7 | Saturates at 4/6 (k=6), 7/12 (k=12), 13/24 (k=24) | **KILLED** |
| 5 | Cayley graph density | 8 | Same-depth entries have 0 edges; deep has fewer yet higher XOR | **KILLED + INVERTED** |
| 6 | Angle coherence (deep) | 10 | Deep shells (d≥3) have random-like angle variance | **KILLED** for deep |
| 7 | Cell diversity | 6 | log₂(distinct_cells) ~ 1.17d + 2, ~66% fill — tracks max_xor | Correlates (causal?) |
| 8 | Paired extension | 11 | Real (100% nesting) but minority at ζ₁₂ (6-20%) | Confirmed as one pathway |

5 cleanly killed, 1 inverted, 2 remain as partial/correlative explanations.

**Provenance:** Demo 89.

### Structural Constants

| Constant | Value | Notes |
|----------|-------|-------|
| Cell fill ratio (k=6) | ~66% | Regardless of depth |
| Voronoi coverage | 100% | Always full at all depths |
| Cayley edges/entry | 4/3 | Convergent |
| Sectors per additional weight | +2 | Matches +2 XOR input step |
| Deep-564 Cayley edges | 0 | vs strided-564: 58 edges |
| Intra-depth coherence | 0.4214 | < inter-depth (0.4254) |

**Provenance:** Demo 89.

### Direction Count by Cumulative Depth (ζ₁₂)

| Depth | Directions |
|-------|-----------|
| 0 | 2 |
| 1 | 6 |
| 2 | 22 |
| 3 | 46 |
| 4 | 114 |
| 5 | 239 |
| 6 | 507 |
| 7 | 975 |
| 8 | 2043 |

**Provenance:** Demo 89.

### Paired Extension at ζ₁₂ (Depth 4, 275 entries, 114 dirs)

| XOR level | Total winners | Shadow winners | Shadow fraction | Nesting rate |
|-----------|--------------|---------------|----------------|-------------|
| XOR6 | 3736 | 234 | 6% | — |
| XOR8 | 2200 | 430 | 20% | 100% |

At ζ₈ (24 entries, 13 dirs): shadow fraction was 100%. Direction scarcity forces shadow pairing; direction abundance enables diversity-dominant pathway.

**Provenance:** Demo 89.

### Distinct Rotation Angles by Cumulative Depth (ζ₁₂)

| Cumulative depth | Distinct angles |
|-----------------|----------------|
| 0 | 2 |
| 1 | 4 |
| 2 | 6 |
| 3 | 10 |
| 4 | 12 |
| 5 | 16 |
| 6 | 21 |
| 7 | 35 |
| 8 | 44 |

Growth rate: ~5 new angles per depth level.

**Provenance:** Demo 89.

---

## 52. Sum-Angle Structure and Spectral Inversion — Demo 90

### Axis Cancellation Data (XOR8 Winners vs Non-Winners)

| Metric | XOR8 winners | Random non-winners |
|--------|-------------|-------------------|
| Mean min_dot (axis alignment) | -0.75 | -0.65 |
| Sum angle range | 30–66° | — |

Winners select anti-aligned axis pairs: partial cancellation of opposite rotation axes produces low sum angles needed for sector separation.

**Provenance:** Demo 90.

### Cross-Depth Sum-Angle Vocabulary

| Pair type | Distinct angles (from 50K pairs) |
|-----------|--------------------------------|
| Shallow × shallow (d=0-1) | 15 |
| Shallow × deep (cross-depth) | 73 |
| Deep × deep (d=7-8) | 1313 |

BFS ancestry constrains pairwise sum-angle vocabulary between related entries.

**Provenance:** Demo 90.

### New Cross-Depth Angle Vocabulary by Depth Level

| Depth | New distinct angles | Useful (<70°) fraction |
|-------|--------------------|-----------------------|
| 1 | 3 | 80% |
| 2 | 6 | — |
| 3 | 10 | — |
| 4 | 22 | — |
| 5 | 33 | — |
| 6 | 72 | — |
| 7 | 120 | — |
| 8 | 258 | 32% |
| cumul. | 381 | — |

Growth ~2× per depth round. Useful (<70°) fraction drops from 80% to 32%.

**Provenance:** Demo 90.

### Spectral Inversion: S² Point Cloud Bandwidth by Depth

| Depth range | BW_90% | BW_99% |
|------------|--------|--------|
| Depth 0 only | l=4 | l=11 |
| Cumulative all | l=2 | l=6 |

Bandwidth DECREASES with depth. Computation improves while positional information content shrinks. Rules out all positional/bandwidth explanations.

**Provenance:** Demo 90.

### XOR Winner Sum-Angle Diversity

| XOR level | Mean distinct pairwise sum angles per winner |
|-----------|---------------------------------------------|
| XOR6 | 2.80 |
| XOR8 | 5.18 |

Higher-level winners use more angular diversity within each tuple.

**Provenance:** Demo 90.

---

## 53. Activation Bottleneck Test — Demo 91

### k_sec Sweep at Fixed Depth 0–4 (275 entries, 114 directions)

| k_sec | Max XOR | XOR8 winners | Notes |
|-------|---------|-------------|-------|
| 2 | 8 | 586 | Minimum resolution (4 total cells) |
| 4 | 8 | 1166 | — |
| 6 | 8 | 2034 | — |
| 8 | 8 | 3280 | — |
| 12 | 8 | 4322 | — |
| 16 | 8 | 5210 | — |
| 24 | 8 | 5800 | — |
| 32 | 8 | 6894 | — |
| 48 | 8 | 7652 | Maximum resolution |

max_xor = 8 at ALL k_sec values. Resolution affects winner count (13× growth), not existence.

**Provenance:** Demo 91.

### Depth Gap Invariance

| k_sec | Depth of first XOR6 | Depth of first XOR8 | Depth gap |
|-------|--------------------|--------------------|-----------|
| 2 | 1 | 2 | 1 |
| 4 | 1 | 2 | 1 |
| 6 | 0 | 1 | 1 |
| 8 | 0 | 1 | 1 |
| 12 | 0 | 1 | 1 |
| 16 | 0 | 1 | 1 |
| 24 | 0 | 1 | 1 |
| 32 | 0 | 1 | 1 |
| 48 | 0 | 1 | 1 |

The depth gap between XOR6 and XOR8 is invariant across all angular resolutions.

**Provenance:** Demo 91.

### Voronoi Direction Count Sweep (k_sec=24, full catalog)

| Direction source | Dirs | XOR8 winners | Max XOR |
|-----------------|------|-------------|---------|
| Depth 0 only | 2 | 2 | 8 |
| Depth 0-1 | 6 | 35 | 8 |
| All (depth 0-4) | 114 | 5800 | 8 |

More directions produce more solutions, not higher arity.

**Provenance:** Demo 91.

---

## 54. Parity-Lock Theorem Data — Demo 92

### Function Comparison Across Depths (ζ₁₂, 4096 entries)

**6-input:**

| Depth | XOR winners | AND winners | OR winners | MAJ winners | THR2 winners |
|-------|-----------|-----------|----------|-----------|-------------|
| 0 | 8 | 0 | 0 | 0 | 0 |
| 4 | ~1500 | 0 | 0 | 0 | 0 |
| 8 | 3866 | 0 | 0 | 0 | 0 |

**8-input:**

| Depth | XOR winners | AND winners | OR winners | MAJ winners |
|-------|-----------|-----------|----------|-----------|
| 0 | 0 | 0 | 0 | 0 |
| 8 | 3080 | 0 | 0 | 0 |

AND/OR/MAJ/THRESHOLD = 0 winners at ALL depths and both arities.

**Provenance:** Demo 92.

### Truth Table Census (10,000 Random 3-Weight Triples)

| Function | Separable | Fraction |
|----------|-----------|----------|
| XOR6 | 8370 | 83.7% |
| AND | 0 | 0.0% |
| OR | 0 | 0.0% |
| MAJ | 0 | 0.0% |

**Provenance:** Demo 92.

### Mask Collision Analysis

| Category | Count | Of total 2016 pairs |
|----------|-------|-------------------|
| Exact quaternion-sum collisions | 76 | 3.8% |
| Same-parity collisions | 76 | 100% of collisions |
| Cross-parity collisions | 0 | 0% |

All collisions are same-parity — preserves XOR but breaks non-XOR functions.

**Provenance:** Demo 92.

### Equivalence Class Analysis (k=3 Weights, 27 = 3³ Classes)

| Function | Class conflicts | Conflicting fraction |
|----------|----------------|---------------------|
| XOR | 0 | 0/27 |
| AND | 1 | 1/27 |
| OR | 1 | 1/27 |
| MAJ | 19 | 19/27 |

Critical class: (0,0,0) contains both mask 000000 and 111111 (both produce sum=0), but AND(000000)=0 and AND(111111)=1.

Distinct cells for 27 classes at k_sec=12: 11.

**Provenance:** Demo 92.

### Sign-Flip Symmetry Verification

| Property | Result |
|----------|--------|
| Sign-flip pairs tested | 13/13 |
| Same sector | 13/13 (100%) |
| Same Voronoi cell | 13/13 (100%) |
| Same combined cell | 13/13 (100%) |
| Same parity | 13/13 (100%) |

combined_cell(S) = combined_cell(-S) for all sums S. Provably, 3^k classes collapse to at most (3^k + 1)/2 distinguishable groups. For k=3: 27 → ≤14 groups; observed: 11 distinct cells.

**Provenance:** Demo 92.

### XOR Winner Cell Counts (Multi-Triple Analysis)

| Category | Distinct cells (of 27 classes) |
|----------|-------------------------------|
| XOR winners | 11–13 |
| Non-winners | 6–11 |

**Provenance:** Demo 92.

---

## 55. Function Scaling — 1-Weight-Per-Input Encoding — Demo 93

### Phase 8 Headline Table (depth 1, 17 entries, 84 cells, exhaustive under phase_cell)

| N | masks | XOR% | AND% | MAJ% | AND/XOR ratio |
|---|-------|------|------|------|---------------|
| 3 | 8 | 96.8% | 97.7% | 99.7% | 1.01 |
| 4 | 16 | 85.6% | 92.3% | 91.1% | 1.08 |
| 5 | 32 | 58.3% | 84.1% | 94.3% | 1.44 |
| 6 | 64 | 9.9% | 71.7% | 49.6% | 7.27 |
| 7 | 128 | 0.02% | 56.8% | 68.7% | 2762 |
| 8 | 256 | 0.00% | 41.6% | 2.4% | infinity |

AND/XOR ratio explodes from 1.01 to infinity as N grows from 3 to 8. XOR is parity (not in AC^0); AND is AC^0; MAJ is TC^0. The circuit complexity hierarchy becomes a hit-rate divergence.

**Provenance:** Demo 93, Phase 8.

### N=6 AND/XOR Ratio by Depth (Convergence)

| Depth | AND/XOR ratio |
|-------|---------------|
| 1 | 7.27 |
| 2 | 2.77 |
| 3 | 1.75 |
| 4 | 1.27 |
| 5 | 1.13 |
| 6 | 1.08 |

Depth disproportionately helps parity. XOR growth factors d=1 to d=6: 3.4x, 1.6x, 1.4x, 1.1x, 1.1x (logarithmic saturation, not exponential).

**Provenance:** Demo 93, Phase 7.

### MAJ/XOR Crossover Data (N=6)

| Depth | MAJ% | XOR% | Easier |
|-------|------|------|--------|
| 1 | 49.6% | 9.9% | MAJ |
| 2 | — | — | — |
| 3 | — | — | crossover region |
| 4 | 73.6% | 76.9% | XOR |
| 6 | 89.0% | 92.2% | XOR |

MAJ easier at low depth (d=1), XOR overtakes at d~3-4. Depth disproportionately benefits parity.

**Provenance:** Demo 93, Phase 7.

### Pigeonhole Numbers

| Parameter | Value |
|-----------|-------|
| Cells at depth 1 (k_sec=12) | 84 |
| N=6 masks | 64 (< 84, XOR survives at 9.9%) |
| N=7 masks | 128 (> 84, XOR dies at 0.02%) |
| N=8 masks | 256 (> 84, XOR dead at 0.00%) |

XOR requires balanced bisection of 2^N masks into cells. When 2^N > cells, impossible. AND requires only 1 unique cell for mask 1...1, so survives well past the pigeonhole wall. Threshold at N=7.

**Provenance:** Demo 93, Phase 8.

---

## 56. Binary Icosahedral Group (2I, E8) — Demo 94

### 2I Group Structure

| Property | Value |
|----------|-------|
| Order | 120 (60 mod ±identity) |
| Generators | s = (1+i+j+k)/2 (order 6), t = (phi + phi_inv·i + j)/2 (order 10) |
| BFS rounds | 7 |
| Conjugacy classes | 9 (half-angles: 0, 36, 60, 72, 90, 108, 120, 144, 180 degrees) |
| Distinct S2 directions | ~31 (vs 13 for z8) |
| Solvable | NO (unique non-solvable finite SU(2) subgroup) |
| ADE type | E8 |

BFS depth profile (symmetric diamond): d0=5, d1=8, d2=11, d3=12, d4=11, d5=8, d6=4, d7=1.

**Provenance:** Demo 94, Phase 1.

### Size-Controlled Comparison — 24 vs 24

z8 (24 entries, solvable) vs 2I random 24-subsets (10-trial mean, non-solvable):

| N | z8 XOR | 2I XOR | z8 AND | 2I AND | Winner |
|---|--------|--------|--------|--------|--------|
| 3 | 1480 | 1514 | 1907 | 1888 | tie |
| 4 | 8010 | 9475 | 9723 | 10025 | 2I |
| 5 | 17201 | 21990 | 37835 | 38862 | 2I |
| 6 | 12983 | 23137 | 111290 | 112408 | 2I |
| 7 | 197 | 672 | 72003 | 241854 | 2I |
| 8 | 1 | 1 | 57449 | 375756 | z8 |

N=6 XOR: 2I wins by 78%. N=7 XOR: 2I wins ~3.4x. N=8: both XOR near zero (pigeonhole), but 2I AND = 375K vs z8 AND = 57K (6.5x).

**Provenance:** Demo 94, Phase 2.

### Depth-Matched Comparison (First 24 from 2I by BFS, 12 directions, 156 cells)

| N | z8 XOR% | 2I-24 XOR% | ratio |
|---|---------|------------|-------|
| 3 | 73.12% | 78.06% | 1.07x |
| 4 | 75.38% | 85.77% | 1.14x |
| 5 | 40.47% | 48.95% | 1.21x |
| 6 | 9.65% | 16.11% | 1.67x |
| 7 | 0.20% | 0.17% | 0.84x |
| 8 | 0.001% | 0.001% | ~1x |

2I advantage ACCELERATES with computational difficulty: 1.07x → 1.14x → 1.21x → 1.67x (N=3 to N=6). At N=7 both near-dead (pigeonhole: only 12 directions → 156 cells for 2I-24 vs 168 for z8).

**Provenance:** Demo 94, Phase 2b.

### Full 2I Capacity (60 entries, 31 directions, 384 cells)

| N | masks | XOR% | AND% | MAJ% | AND/XOR |
|---|-------|------|------|------|---------|
| 3 | 8 | 74.80% | 93.42% | 74.78% | 1.25 |
| 4 | 16 | 91.33% | 94.73% | 74.50% | 1.04 |
| 5 | 32 | 59.98% | 93.74% | 61.34% | 1.56 |
| 6 | 64 | 37.12% | 89.72% | 26.20% | 2.42 |
| 7 | 128 | 2.38% | 81.62% | 5.06% | 34.29 |
| 8 | 256 | 0.00% | 68.83% | 0.01% | 68827 |

Circuit complexity hierarchy universal across group structures. Compared to D93 zeta_12 (17 entries, depth 1): N=6 XOR 37.12% vs 9.85% (3.8x), N=7 XOR 2.38% vs 0.02% (119x), N=8 XOR 0.001% vs 0.00%.

**Provenance:** Demo 94, Phase 3.

### Three-Way XOR Comparison

| N | z8 (24, solvable) | 2I (60, non-solv finite) | z12 (51, non-solv trunc) |
|---|-------------------|--------------------------|--------------------------|
| 3 | 73.1% | 74.8% | 90.5% |
| 4 | 75.4% | 91.3% | 93.2% |
| 5 | 40.5% | 60.0% | 68.1% |
| 6 | 9.6% | 37.1% | 33.8% |
| 7 | 0.20% | 2.38% | 1.64% |
| 8 | 0.001% | 0.001% | 0.00% |

z12 truncated: depth<=2, 51 entries, 22 directions, 276 cells. Crossover at N=6: z12 wins small N (angular variety), 2I wins N=6-7 (non-solvability at computational boundary).

**Provenance:** Demo 94, Phase 4.

### Quantum Dimension Table

| Root | [2]_q | Finite? | Group | Solvable? | Capacity | Kuperberg |
|------|-------|---------|-------|-----------|----------|-----------|
| z4 | -2 | YES (4) | Q8 | Yes | Very limited | Lattice (P) |
| z8 | 0 | YES (24) | 2O (E7) | Yes | XOR8 ceiling | Lattice (P) |
| z12 | 1 | NO (inf) | Dense | N/A (infinite) | XOR12+ | Lattice (P) |
| z10 | phi^-1 ~ 0.618 | NO (inf) | Dense | N/A | Unknown | #P-hard |
| 2I | phi ~ 1.618 | YES (120) | 2I (E8) | NO | Higher than z8 | #P-hard |

2I is the unique non-solvable finite SU(2) subgroup — the bridge between finite/ceiling (solvable groups) and dense/infinite (unbounded). Barrington's theorem predicts the solvable/non-solvable boundary is the computational capacity boundary; D94 confirms this.

**Provenance:** Demo 94, Phase 4 + D80 data.

---

## 57. Commutator Depth and XOR Capacity — Demo 95

### z8 Derived Series Level Membership

| Level | Size | Algebraic identity |
|-------|------|--------------------|
| D0\D1 | 12 | Outermost, non-commutator |
| D1\D2 | 8 | First commutator layer (2T\Q8) |
| D2\D3 | 3 | Deep commutator (Q8\center) |
| D3 | 1 | Identity |

Derived series: 24 > 12 > 4 > 1 (2O > 2T > Q8 > {±1}).

**Provenance:** Demo 95, Phase 2.

### COMM vs NON-COMM vs ALL XOR Hit Rates

| N | COMM(12) | NON-COMM(12) | ALL(24) | COMM/NONC |
|---|----------|--------------|---------|-----------|
| 3 | 48.64% | 42.73% | 71.94% | 1.14x |
| 4 | 44.04% | 51.52% | 75.04% | 0.85x |
| 5 | 15.15% | 13.64% | 40.76% | 1.11x |
| 6 | 6.28% | 5.63% | 9.64% | 1.12x |
| 7 | 1.01% | 0.00% | 0.18% | inf |

ALL(24) dramatically outperforms either 12-element subset — 2.7x at N=5. Cross-layer interaction is the active mechanism.

**Provenance:** Demo 95, Phase 3.

### 90-Degree Split (Re=0, Matched Half-Angle)

| Subset | Size | N=3 XOR | N=4 XOR | N=5 XOR | N=6 XOR |
|--------|------|---------|---------|---------|---------|
| Q8-null (D2) | 3 | 0 | 0 | 0 | 0 |
| Outermost-null (D0) | 6 | 0 | 0 | 0 | 0 |
| All-null (mixed) | 9 | 0 | 33/126 = 26.19% | 0 | 0 |

Pure 0+0=26% synergy at matched geometry. Algebra wins over geometry.

**Provenance:** Demo 95, Phase 3b.

### Matched Comparison — z8-COMM vs 2I-first-12

| N | z8-COMM (12) | 2I-first-12 | ratio (2I/z8) |
|---|-------------|-------------|---------------|
| 3 | 48.64% | 75.91% | 1.56x |
| 4 | 44.04% | 72.12% | 1.64x |
| 5 | 15.15% | 32.58% | 2.15x |
| 6 | 6.28% | 5.95% | 0.95x |
| 7 | 1.01% | 0.00% | 0.00x |

2I wins at N=3-5 (no stratification bottleneck); crossover at N=6 mirrors D94 Phase 4 pattern.

**Provenance:** Demo 95, Phase 4.

---

## 58. TL-Group 2D Synergy Landscape — Demo 96

### Cross-Tabulation (5 Cells)

| Cell | Description | Count | Half-angle | Derived | Null? |
|------|-------------|-------|------------|---------|-------|
| A | D0-null (outermost, bracket-null) | 6 | 90 deg | D0 | Yes |
| B | D0-nonnull (outermost, non-null) | 6 | 45 deg | D0 | No |
| C | D1-nonnull (2T commutators) | 8 | 60 deg | D1 | No |
| D | D2-null (Q8 deep) | 3 | 90 deg | D2 | Yes |
| E | D3-identity | 1 | 0 deg | D3 | No |

Cross-checks: Null (A+D) = 9, COMM (C+D+E) = 12, NONCOMM (A+B) = 12, Grand = 24.

**Provenance:** Demo 96, Phase 1.

### Individual Cell XOR Capacity

| Cell | Size | Dirs | N=3 | N=4 | N=5 | N=6 |
|------|------|------|-----|-----|-----|-----|
| A (D0-null) | 6 | 6 | 0.00% | 0.00% | 0.00% | 0.00% |
| B (D0-nonnull) | 6 | 3 | 100.00% | 100.00% | 100.00% | 100.00% |
| C (D1-nonnull) | 8 | 4 | 100.00% | 100.00% | 42.86% | 57.14% |
| D (D2-null) | 3 | 3 | 0.00% | n/a | n/a | n/a |
| E (D3-identity) | 1 | 0 | n/a | n/a | n/a | n/a |

Cell B = 100% at EVERY N with only 6 entries and 3 directions. Null cells A, D completely dead.

**Provenance:** Demo 96, Phase 2.

### Pairwise Synergy Matrix

| Pair | Size | Dirs | N=3 | N=4 | N=5 | N=6 |
|------|------|------|-----|-----|-----|-----|
| A+B (outermost shell) | 12 | 9 | 42.73% | 51.52% | 13.64% | 5.63% |
| A+C (cross both dims) | 14 | 10 | 46.15% | 74.93% | 14.99% | 1.33% |
| A+D (both null, 90-deg) | 9 | 9 | 0.00% | 26.19% | 0.00% | 0.00% |
| B+C (both nonnull, cross derived) | 14 | 7 | 96.70% | 94.01% | 73.18% | 31.00% |
| B+D (cross both dims) | 9 | 3 | 23.81% | 11.90% | 4.76% | 1.19% |
| C+D (comm subgroup) | 11 | 7 | 48.48% | 49.09% | 20.78% | 7.36% |

Best pair at EVERY N: B+C (both nonnull, cross derived).

**Provenance:** Demo 96, Phase 3.

### Triple Combinations vs ALL

| Subset | Size | Dirs | N=3 | N=4 | N=5 | N=6 |
|--------|------|------|-----|-----|-----|-----|
| ALL (A+B+C+D+E) | 24 | 13 | 71.94% | 75.04% | 40.76% | 9.64% |
| A+B+C (skip Q8) | 21 | 13 | 74.06% | 77.51% | 45.10% | 12.98% |
| A+B+D (skip 2T) | 16 | 9 | 51.43% | 50.55% | 22.34% | 3.97% |
| A+C+D (skip D0nn) | 18 | 13 | 55.39% | 65.59% | 23.07% | 3.31% |
| B+C+D (skip D0null) | 18 | 7 | 75.12% | 71.24% | 32.03% | 7.37% |

A+B+C (skip Q8/D) BEATS full ALL(24) at EVERY N. The 3 Q8-deep-null entries are noise.

Missing cell impact (triple rate minus ALL rate):

| Missing | N=3 | N=4 | N=5 | N=6 |
|---------|-----|-----|-----|-----|
| D (Q8) | +2.12% | +2.47% | +4.34% | +3.34% |
| C (2T) | -20.51% | -24.49% | -18.42% | -5.67% |
| B (D0nn) | -16.54% | -9.45% | -17.69% | -6.33% |
| A (D0null) | +3.19% | -3.80% | -8.73% | -2.27% |

**Provenance:** Demo 96, Phase 4.

---

## 59. Cell B Geometric Perfection — Demo 97

### Angle Sensitivity Sweep (6 Quaternions, 3 Orthogonal Directions)

| Half-angle | N=3 XOR% | N=4 XOR% | N=5 XOR% | N=6 XOR% |
|------------|----------|----------|----------|----------|
| 10 | 100% | 20% | 0% | 100% |
| 15 | 100% | 100% | 0% | 100% |
| 20 | 100% | 100% | 0% | 100% |
| 25 | 100% | 100% | 100% | 100% |
| 30 | 100% | 100% | 100% | 100% |
| 35 | 100% | 100% | 0% | 100% |
| 40 | 100% | 100% | 100% | 100% |
| 42 | 100% | 100% | 100% | 100% |
| 44 | 100% | 100% | 100% | 100% |
| 45 | 100% | 100% | 100% | 100% |
| 46 | 100% | 100% | 100% | 100% |
| 48 | 100% | 100% | 100% | 100% |
| 50 | 100% | 100% | 100% | 100% |
| 55 | 100% | 100% | 100% | 100% |
| 60 | 100% | 100% | 100% | 100% |
| 65 | 100% | 100% | 100% | 100% |
| 70 | 100% | 100% | 100% | 100% |
| 75 | 100% | 100% | 100% | 100% |
| 80 | 100% | 100% | 100% | 0% |
| 85 | 100% | 0% | 0% | 0% |
| 90 | 0% | 0% | 0% | 0% |

Full perfection plateau: 25-75 degrees (~50 degree range). 45 degrees is midpoint, not a critical value. 90 degrees = total collapse. Isolated anomaly at 35 degrees (N=5 drops to 0%).

**Provenance:** Demo 97, Phase 3.

### Large N Exhaustive Test (Cell B, 6 Entries, 3 Directions)

| N | C(6,N) | XOR count | XOR% |
|---|--------|-----------|------|
| 3 | 20 | 20 | 100% |
| 4 | 15 | 15 | 100% |
| 5 | 6 | 6 | 100% |
| 6 | 1 | 1 | 100% |

100% at ALL N through N=6 (exhaustive). Beyond N=6, C(6,N) = 0.

**Provenance:** Demo 97, Phase 4.

### Cell B Separation Quality (N=3, k=12)

| Metric | Value |
|--------|-------|
| XOR pass | 20/20 (100%) |
| Cell-index margin (min) | 4 |
| Cell-index margin (max) | 8 |
| Cell-index margin (mean) | 5.6 |

**Provenance:** Demo 97, Phase 2.

---

## 60. 3-Strand DKC Readout Bottleneck — Demo 98

### 3-Strand BFS Catalog Growth

| Round | Entries | New |
|-------|---------|-----|
| 0 | 1 | 1 |
| 1 | 5 | 4 |
| 2 | 17 | 12 |
| 3 | 47 | 30 |
| 4 | 115 | 68 |
| 5 | 263 | 148 |
| 6 | 577 | 314 |
| 7 | 1233 | 656 |
| 8 | 2048 | 815 (hit cap) |

Group is INFINITE at zeta_8. 2017/2048 (98.5%) interleaving. 113/2048 (5.5%) zero bracket.

**Provenance:** Demo 98, Phase 1.

### Readout Diversity Comparison

| Readout | Distinct | Retention | DOFs |
|---------|----------|-----------|------|
| Trace | 105 | 5.1% | 4 integers |
| Column 0 | 1166 | 56.9% | 20 integers |
| Column 1 | 1166 | 56.9% | 20 integers |
| Column 2 | 1167 | 57.0% | 20 integers |
| Column 3 | 1167 | 57.0% | 20 integers |
| Column 4 | 2048 | 100.0% | 20 integers |

Column 4 is LOSSLESS (100% retention). Trace collapses 25 Cyc8 entries to 1 = 96% information loss.

**Provenance:** Demo 98, Phase 2/2b.

### 2-Strand vs 3-Strand Structural Comparison

| Property | 2-strand | 3-strand |
|----------|----------|----------|
| TL dimension | 2x2 (C_2=2) | 5x5 (C_3=5) |
| Group order | 24 (finite) | Infinite (at zeta_8) |
| Generators | 2 | 4 |
| Interleaving | N/A | 98.5% |
| Readout DOFs (trace) | 4 integers (sufficient) | 4 integers (catastrophically lossy) |
| Readout DOFs (column) | N/A | 20 integers (lossless at col 4) |
| XOR6 capacity | 1109 (at k=20) | 0 (trace readout) |

The readout, not the algebra, is the bottleneck. 3-strand has MORE structure but trace destroys it.

**Provenance:** Demo 98, Phases 1-5.

---

## 61. 3-Strand Delta_1 Block Decomposition — Demo 99

### Activation Hierarchy (XOR Capacity Scan)

| Activation | N=3(XOR6) | N=4(XOR8) | N=5(XOR10) | N=6(XOR12) |
|------------|-----------|-----------|------------|------------|
| 4ent(16) | 227624 | 5990 | 126 | 0 |
| 4ent(32) | 258764 | 8098 | 206 | 8 |
| 4ent(64) | 230568 | 7792 | 210 | 8 |
| quat(16) | 28748 | 284 | 0 | 0 |
| quat(32) | 29832 | 298 | 0 | 0 |
| quat(64) | 29124 | 294 | 0 | 0 |
| 2x2(32) | 373194 | 11974 | 325 | 16 |
| 2x2(64) | 373194 | 16404 | 485 | 16 |
| 2x2(128) | 373194 | 16404 | 485 | 16 |
| tr_oct(8) | 0 | 0 | 0 | 0 |

Hierarchy: 2x2_hash >> 4ent_hash >> quat_hash >> trace_oct. Best per level: XOR6=500,261 at 2x2(64), XOR8=48,143 at 2x2(128), XOR10=485 at 2x2(128), XOR12=16 at 2x2(32+).

**Provenance:** Demo 99, Phase 3.

### XOR Capacity at Depth (Ceiling Breaking)

| Cells | XOR10 | XOR12 | XOR14 |
|-------|-------|-------|-------|
| 64 | 2641 | 675 | 60 |
| 128 | 2966 | 675 | 60 |
| 256 | 4335 | 675 | 60 |
| 512 | 6883 | 759 | 60 |
| 1024 | 9418 | 1452 | 60 |

XOR14=60 from deep entries (d>=6), XOR14=0 from full catalog. Depth is a genuine computational resource. XOR16 is ZERO (genuine ceiling for Delta_1 at ell=2).

**Provenance:** Demo 99, Phases 6 and 8.

### XOR14 Super-Hub Structure

| Hub | Entry | Depth | Writhe | a (Cyc8) | d (Cyc8) | b (Cyc8) | c (Cyc8) | Cartan L1 |
|-----|-------|-------|--------|----------|----------|----------|----------|-----------|
| 7 | idx=7 | d=6 | w=2 | (0,0,5,0) | (0,0,5,0) | (12,0,0,0) | (-2,0,0,0) | 0 |
| 9 | idx=9 | d=6 | w=-6 | (0,0,-8,0) | (0,0,1,0) | (-3,0,0,0) | (-3,0,0,0) | 9 |

Hub commutator: traceless, Cartan L1=84, e-root L1=108, f-root L1=18, e/f ratio=6.0. 100% of 60 winners contain both hubs. 100% mixed-depth (d=6+d=7). Star graph topology. 0/1 survive hub7 swap, 0/7 survive hub9 swap.

**Provenance:** Demo 99, Phases 7/7b/7d.

### Casimir Discriminant (Winner vs Control)

| Metric | Winners | Control |
|--------|---------|---------|
| Mean |C|_L1 | 1301 | 297 |
| Jordan blocks | 384/16384 | 816/16384 |

Winners are 4.4x MORE semisimple than control. Computation happens in the semisimple layer.

**Provenance:** Demo 99, Phase 7c.

### Casimir Gap Scaling (Two-Regime Discovery)

| XOR level | Winner |C| | Control |C| | Ratio |
|-----------|-----------|-------------|-------|
| XOR6 | 54 | 75 | 0.7x (INVERTED) |
| XOR8 | 73 | 104 | 0.7x (INVERTED) |
| XOR10 | 513 | 154 | 3.3x |
| XOR12 | 892 | 219 | 4.1x |
| XOR14 | 1301 | 245 | 5.3x |

Phase transition at XOR8->XOR10 coincides with shallow->deep entry transition. Two regimes: Combinatorial (XOR6-8, nilpotent-proximate, shallow) and Algebraic (XOR10-14, Ext^1 catalytic, deep).

**Provenance:** Demo 99, Phase 10.

### BFS Growth (Fibonacci max_abs)

| Round | Entries | max_abs |
|-------|---------|---------|
| 0 | 1 | - |
| 1 | 5 | 1 |
| 2 | 17 | 2 |
| 3 | 47 | 3 |
| 4 | 115 | 5 |
| 5 | 263 | 8 |
| 6 | 577 | 13 |
| 7 | 1233 | 21 |
| 8 | 2581 | 34 |
| 9 | 5377 | 55 |
| 10 | 8192 | 89 (hit cap) |

max_abs follows exact Fibonacci: 1, 2, 3, 5, 8, 13, 21, 34, 55, 89.

**Provenance:** Demo 99, Phase 1.

### 2-Strand vs 3-Strand Comparison

| Property | 2-strand (TL_2) | 3-strand (Delta_1) |
|----------|-----------------|---------------------|
| Group | Finite (cyclic, order 8) | Infinite (Jordan block) |
| Module | Simple L(1) | Indecomposable Delta_1 |
| Extension | None (semisimple) | Ext^1(L(0),L(1)) != 0 |
| XOR topology | Mutual orthogonality graph | Star graph (2 hubs + sats) |
| XOR ceiling | XOR12 (Cell B) | XOR14 (deep entries) |
| Matched (N=24) | 1109 XOR6 | 527 XOR6 |

2-strand wins at matched small size. 3-strand wins at scale (infinite catalog). Crossover ~100 entries.

**Provenance:** Demo 99, Phase 4.

---

## 62. 4-Strand DKC on W_{4,2} — Demo 100

### BFS Catalog Growth

| Depth | New entries | Cumulative | Growth ratio | Max magnitude |
|-------|------------|------------|--------------|---------------|
| 0 | 1 | 1 | -- | 1 |
| 1 | 6 | 7 | 6.00 | 1 |
| 2 | 26 | 33 | 4.33 | 2 |
| 3 | 98 | 131 | 3.77 | 3 |
| 4 | 338 | 469 | 3.45 | 5 |
| 5 | 1110 | 1579 | 3.28 | 8 |
| 6 | 3444 | 5023 | 3.10 | 13 |
| 7 | 10390 | 15413 | 3.02 | 21 |
| 8 | 17355 | 32768 | 1.67* | 34 |

*Truncated by 32768 cap. Growth stabilizes near 3.1x. sigma_1 max_abs grows linearly (n=k), confirming infinite order.

**Provenance:** Demo 100, Phases 1-2.

### XOR Capacity Scan (bf=30, First 30 Entries)

| k_cells | XOR6 | XOR8 | XOR10 | XOR12 |
|---------|------|------|-------|-------|
| 64 | 1158 | 393 | 176 | 48 |
| 128 | 1937 | 822 | 208 | 48 |
| 256 | 2510 | 2376 | 251 | 48 |
| 512 | 2961 | 6187 | 541 | 48 |

Deep sub-catalog (d>=4, bf=30, k=128):

| XOR level | Winners |
|-----------|---------|
| XOR6 | 2017 |
| XOR8 | 1169 |
| XOR10 | 654 |
| XOR12 | 305 |
| XOR14 | 70 |

Deep entries SUSTAIN XOR capacity through XOR14=70.

**Provenance:** Demo 100, Phase 3.

### Radical/Casimir by Depth

| Depth | mean_rad | mean_rMr | mean_C3 | Count |
|-------|----------|----------|---------|-------|
| 0 | 2 | 2 | 0 | 1 |
| 1 | 2 | 2 | 0 | 6 |
| 2 | 2 | 2 | 3 | 26 |
| 3 | 2 | 2 | 7 | 98 |
| 4 | 2 | 2 | 16 | 338 |
| 5 | 2 | 2 | 36 | 1110 |
| 6 | 2 | 2 | 80 | 3444 |
| 7 | 2 | 2 | 176 | 10390 |
| 8 | 2 | 2 | 308 | 17355 |

Radical content CONSTANT (=2) at all depths. Casimir ~2x per round (exponential growth).

**Provenance:** Demo 100, Phase 4.

### Casimir-by-XOR-Level (Deep bf=30, k=128)

| XOR level | Winners | Win C3 | Ctrl C3 | Ratio |
|-----------|---------|--------|---------|-------|
| XOR6 | 2017 | 64 | 47 | 1.36x |
| XOR8 | 1169 | 93 | 50 | 1.86x |
| XOR10 | 654 | 123 | 76 | 1.62x |
| XOR12 | 305 | 148 | 103 | 1.44x |

Winners consistently have HIGHER Casimir at all XOR levels. Radical content does NOT distinguish (14 vs 14).

**Provenance:** Demo 100, Phase 4.

### Super-Hub Anatomy (6 Hubs, >50% of 70 XOR14 Winners)

| Hub | Entry | Freq% | Depth | Writhe | Trace (Cyc8) | |C3| | Frob L1 |
|-----|-------|-------|-------|--------|--------------|------|---------|
| 0 | 4 | 91.4% | 4 | 2 | (0,0,3,0) | 0 | 15 |
| 1 | 5 | 91.4% | 4 | 2 | (0,0,6,0) | 36 | 19 |
| 2 | 10 | 54.3% | 5 | 3 | (0,0,0,-7) | 56 | 17 |
| 3 | 21 | 65.7% | 5 | 1 | (0,-5,0,0) | 20 | 19 |
| 4 | 24 | 65.7% | 5 | -3 | (0,3,0,0) | 0 | 21 |
| 5 | 25 | 54.3% | 5 | -1 | (0,0,0,5) | 20 | 11 |

Hub pairing by Cyc8 component: {h0,h1} zeta_8^2, {h2,h5} zeta_8^3, {h3,h4} zeta_8.

### Hub Commutators (All Traceless, All Radical_content = 0)

| Pair | Trace | |C3| | Rad | Frob L1 |
|------|-------|------|-----|---------|
| [h0,h1] zeta_8^2 pair | (0,0,0,0) | 24 | 0 | 44 |
| [h2,h5] zeta_8^3 pair | (0,0,0,0) | 426 | 0 | 55 |
| [h3,h4] zeta_8 pair | (0,0,0,0) | 216 | 0 | 86 |
| [h0,h3] top x mid | (0,0,0,0) | 96 | 0 | 84 |
| [h0,h2] top x low | (0,0,0,0) | 384 | 0 | 84 |
| [h3,h2] mid x low | (0,0,0,0) | 552 | 0 | 94 |

Commutators annihilate the radical direction. Co-occurrence: 121 edges, 19/30 entries participate.

**Provenance:** Demo 100, Phases 4-5.

---

## 63. 5-Strand DKC Scaling on W_{5,3} — Demo 101

### Growth Rate Comparison (sl_d Thesis)

| n (strands) | Demo | Module | dim | Generators | Growth rate | Predicted (sl_{n-1}) |
|-------------|------|--------|-----|------------|-------------|---------------------|
| 3 | D98 | TL_3 | 5x5 | 4 | ~2.2x | 2 (sl_2) |
| 4 | D100 | W_{4,2} | 3x3 | 6 | ~3.1x | 3 (sl_3) |
| 5 | D101 | W_{5,3} | 4x4 | 8 | ~4.0x | 4 (sl_4) |
| 6 | D102 | W_{6,0} | 5x5 | 10 | ~5.0x | 5 (sl_5) |
| 8 | D105 | W_{8,0} | 14x14 | 14 | ~7.0x | 7 (sl_7) |

Growth rate CONFIRMED across 5 data points (n=3,4,5,6,8): tracks n-1 cleanly. BFS growth is a braid group invariant (D102: W_{6,0}=W_{6,4}=W_{6,2} identical; D103 confirms).

**Provenance:** Demo 101, Phase 2 + D98/D100 comparison.

### BFS Catalog Growth

| Depth | New entries | Cumulative | Growth ratio | Max magnitude |
|-------|------------|------------|--------------|---------------|
| 0 | 1 | 1 | -- | 1 |
| 1 | 8 | 9 | 8.00 | 1 |
| 2 | 44 | 53 | 5.50 | 2 |
| 3 | 206 | 259 | 4.68 | 3 |
| 4 | 884 | 1143 | 4.29 | 5 |
| 5 | 3600 | 4743 | 4.07 | 8 |
| 6 | 14198 | 18941 | 3.94 | 13 |
| 7 | 13827 | 32768 | 0.97* | 21 |

*Truncated by 32768 cap. Converging toward ~4x per round.

**Provenance:** Demo 101, Phase 1.

### XOR Comparison — D100 (3x3, Non-Semisimple) vs D101 (4x4, Simple)

Deep sub-catalog (d>=4, bf=30, k=128):

| XOR level | D100 (W_{4,2}) | D101 (W_{5,3}) |
|-----------|---------------|---------------|
| XOR6 | 2017 | 2579 |
| XOR8 | 1169 | 2228 |
| XOR10 | 654 | 203 |
| XOR12 | 305 | 27 |
| XOR14 | 70 | 0 |

D101 wins at XOR6-XOR8, D100 wins at XOR10-XOR14. Simple module = broader but shallower capacity. Non-semisimple extension is computationally load-bearing at depth.

**Provenance:** Demo 101, Phase 3.

### XOR Capacity K-Sweep (bf=30, First 30 Entries)

| k_cells | XOR6 | XOR8 | XOR10 | XOR12 |
|---------|------|------|-------|-------|
| 64 | 1916 | 1330 | 732 | 441 |
| 128 | 2600 | 3316 | 844 | 441 |
| 256 | 3080 | 7918 | 1876 | 465 |
| 512 | 3297 | 12229 | 5729 | 512 |

XOR14 at all k values (256-4096): ZERO.

**Provenance:** Demo 101, Phase 3.

### Casimir Inversion (Simple Module vs Non-Semisimple)

Casimir-by-XOR-level (deep bf=30, k=128):

| XOR level | Winners | Win C4 | Ctrl C4 | Ratio |
|-----------|---------|--------|---------|-------|
| XOR6 | 2579 | 55 | 47 | 1.17x |
| XOR8 | 2228 | 71 | 47 | 1.51x |
| XOR10 | 203 | 57 | 64 | 0.89x |
| XOR12 | 27 | 43 | 79 | 0.54x |

INVERSION at XOR10-12: winners have LOWER Casimir. In D100 (non-semisimple) all ratios > 1 (1.36x-1.86x). Simple modules select for near-scalar entries at hard XOR levels.

Casimir by depth:

| Depth | mean_C4 | Count |
|-------|---------|-------|
| 0 | 0 | 1 |
| 1 | 0 | 8 |
| 2 | 4 | 44 |
| 3 | 9 | 206 |
| 4 | 20 | 884 |
| 5 | 41 | 3600 |
| 6 | 83 | 14198 |
| 7 | 118 | 13827 |

~2x per round, matching D100 pattern.

**Provenance:** Demo 101, Phase 4.

### Super-Hub Anatomy (3 Hubs, >1/3 of 203 XOR10 Winners)

| Hub | Entry | Freq% | Depth | Writhe | Trace (Cyc8) | |C4| | Frob L1 |
|-----|-------|-------|-------|--------|--------------|------|---------|
| 0 | 0 | 42.9% | 4 | 4 | (-4,0,0,0) | 0 | 8 |
| 1 | 26 | 67.5% | 5 | 5 | (0,4,0,0) | 0 | 9 |
| 2 | 27 | 70.0% | 5 | 3 | (0,0,0,-5) | 11 | 12 |

Only 3 hubs (sl_4 prediction of 12 FALSIFIED). Hubs 0,1 have |C4|=0 (nearly scalar). Hub commutators: ALL are the zero matrix (all 3 hubs MUTUALLY COMMUTE). Much stronger than D100's tracelessness.

XOR12 hub dominance (27 winners): entries 0, 26, 27 each at 96.3% frequency.

Co-occurrence: 247 edges, 30/30 entries participate. NOT a star topology.

**Provenance:** Demo 101, Phase 5.

---

## 12. Demo Test Counts (Extended)

| Demo | Tests | Status |
|------|-------|--------|
| 01 | 9/9 | COMPLETE |
| 02 | 9/9 | COMPLETE (6 unused warnings) |
| 03 | 8/8 | COMPLETE |
| 04 | 12/12 | COMPLETE |
| 05 | ~30 | COMPLETE |
| 06 | 19/19 | COMPLETE |
| 07 | 25/25 | COMPLETE |
| 08 | 47/47 | COMPLETE |
| 09 | 30/30 | COMPLETE |
| 10 | ~50+ | COMPLETE |
| 11 | ~16 | COMPLETE |
| 12 | 6 parts | COMPLETE |
| 13 | ~8+ | COMPLETE |
| 14 | 7/7 | COMPLETE |
| 15 | ~8 | COMPLETE |
| 16 | 8 | COMPLETE |
| 17 | ~6 | COMPLETE |
| 18 | ~8 | COMPLETE |
| 19 | ~15 | COMPLETE |
| 20 | ~7 | COMPLETE |
| 21 | 6/6 | COMPLETE |
| 22 | ~6 | COMPLETE |
| 23 | 4 | COMPLETE |
| 24 | ~7 | COMPLETE |
| 25 | 8/8 | COMPLETE |
| 26 | 8/8 | COMPLETE |
| 27 | ~7 | COMPLETE |
| 28 | ~12-15 | COMPLETE |
| 29 | ~20 | COMPLETE |
| 35 | ~40 (52/52) | COMPLETE |
| 38 | 13/13 | COMPLETE |
| 39 | 18/18 | COMPLETE |
| 45 | ~11 | COMPLETE |
| 46 | 6/10 | COMPLETE |
| 47 | ~12 | COMPLETE |
| 48 | 14/14 | COMPLETE |
| 49 | ~10 | COMPLETE |
| 50 | ~6 | COMPLETE |
| 51 | 57/57 | COMPLETE |
| 52 | 75/75 | COMPLETE |
| 53 | 59/59 | COMPLETE |
| 54 | 53/53 | COMPLETE |
| 55 | 7/7 | COMPLETE |
| 56 | 17/17 | COMPLETE |
| 57 | ~8 | COMPLETE |
| 58 | 16/16 | COMPLETE |
| 59 | ~15+ | COMPLETE |
| 60 | 15/15 | COMPLETE |
| 61 | ~14 (12/12) | COMPLETE |
| 62 | 7/7 | COMPLETE |
| 64 | 22/22 | COMPLETE |
| 65 | 38/38 | COMPLETE |
| 66 | 30/30 | COMPLETE |
| 67 | 18/18 | COMPLETE |
| 68 | 9/9 | COMPLETE |
| 69 | 9/9 | COMPLETE |
| 70 | 8/8 | COMPLETE |
| 71 | 10/10 | COMPLETE |
| 72 | 8/8 | COMPLETE |
| 73 | 7/7 | COMPLETE |
| 74 | 20/20 | COMPLETE |
| 75 | 23/23 | COMPLETE |
| 76 | 10/10 | COMPLETE |
| 77 | 14/14 | COMPLETE |
| 78 | 9/9 | COMPLETE |
| 79 | 14/14 | COMPLETE |
| 80 | 11/11 | COMPLETE |
| 81 | 14/14 | COMPLETE |
| 82 | 17/17 | COMPLETE |
| 83 | 12/12 | COMPLETE |
| 84 | 17/17 | COMPLETE |
| 85 | 58/58 | COMPLETE |
| 86 | 15/15 | COMPLETE (NEGATIVE RESULT) |
| 87 | 14/14 | COMPLETE |
| 88 | 8/8 | COMPLETE |
| 89 | 14/14 | PRELIMINARY (2 expected fails) |
| 90 | 10/12 | COMPLETE (2 informative fails) |
| 91 | 6/7 | COMPLETE (1 informative fail) |
| 92 | 13/16 | COMPLETE (3 informative fails) |
| 93 | 37/37 | COMPLETE (LANDMARK) |
| 94 | 20/20 | COMPLETE (RESULT) |
| 95 | 19/19 | COMPLETE (RESULT) |
| 96 | 14/14 | COMPLETE (LANDMARK) |
| 97 | 8/8 | COMPLETE (RESULT) |
| 98 | 10/10 | COMPLETE (RESULT) |
| 99 | 24+/24+ | COMPLETE (LANDMARK) |
| 100 | 36/36 | COMPLETE (RESULT) |
| 101 | 44/44 | COMPLETE (RESULT) |
| 102 | 84/84 (2 structural FAILs) | COMPLETE (RESULT) |
| 103 | 76/76 (2 structural FAILs) | COMPLETE |
| 104 | 35/35 | COMPLETE (RESULT) |
| 105 | 96/96 (46 Phase A + 50 Phase B) | COMPLETE |
| 106 | 20/20 | COMPLETE (RESULT) |
| 107 | 42/42 | COMPLETE (RESULT) |
| 108 | 3/3 | COMPLETE (RESULT) |
| 109 | 5/5 | COMPLETE (RESULT) |

**Provenance:** Demo index, all entries.

---

## 64. 6-Strand Radical A/B Test — Demo 102

### Module Structures (TL_6 at delta=0, both dim=5)

| Module | dim | Through-lines | Radical dim | Radical vector | Generator nonzero |
|--------|-----|---------------|-------------|----------------|-------------------|
| W_{6,0} | 5 | 0 (all arcs) | 0 (simple) | N/A | 15/125 |
| W_{6,4} | 5 | 4 | 1 | r=(1,0,-1,0,1) | 8/125 |

### BFS Catalog (both modules, cap 32768)

| Depth | New entries | Cumulative | Growth ratio |
|-------|------------|------------|--------------|
| 0 | 1 | 1 | -- |
| 1 | 10 | 11 | 10.00 |
| 2 | 66 | 77 | 6.60 |
| 3 | 362 | 439 | 5.48 |
| 4 | 1794 | 2233 | 4.96 |
| 5 | 8370 | 10603 | 4.67 |
| 6 | 22165 | 32768 | 2.65* |

*Truncated by cap.

### Max Coefficient Magnitude by Depth

| Depth | W_{6,0} | W_{6,4} |
|-------|---------|---------|
| 0 | 1 | 1 |
| 1 | 1 | 1 |
| 2 | 2 | 2 |
| 3 | 3 | 3 |
| 4 | 5 | 5 |
| 5 | 8 | 8 |
| 6 | 16 | 13 |

Both Fibonacci through d=5. B stays Fibonacci (13) at d=6, A breaks (16).

### XOR Capacity Comparison (deep bf=30, k=128)

| XOR level | A (W_{6,0}) | B (W_{6,4}) full | B quotient-only |
|-----------|-------------|------------------|-----------------|
| XOR6 | 2449 | 2370 | 2195 |
| XOR8 | 850 | 793 | -- |
| XOR10 | 1 | 0 | 0 |
| XOR12 | 0 | 0 | -- |

### Casimir and Radical Statistics

- Mean |C5| (bf=30): A=48, B=23
- XOR6 Casimir-by-level: winners=68, control=59, ratio=1.15x
- Mixing row L1 (W_{6,4}): min=0, max=26, mean=2, zero=7628/32768 (23.3%)
- By depth: d0-d3 mean=0, d4-d5 mean=1, d6 mean=3
- Radical content: constant = 3 for all 32,768 W_{6,4} entries
- Writhe character verification: 32,768/32,768 entries match M*r = A^{writhe}*r (100%)

### 13 Pre-Registered Predictions

| Prediction | Result |
|------------|--------|
| P1 (W_{6,4} rad dim=1) | CONFIRMED |
| P2 (writhe character on generators) | CONFIRMED |
| P3 (W_{6,0} sparser) | KNOWN FALSE (A=15 > B=8) |
| P4 (W_{6,0} rad dim=0) | CONFIRMED |
| P5 (growth ~5x both) | CONFIRMED |
| P6 (frozen radical all entries) | CONFIRMED (LANDMARK) |
| P7 (radical_content constant) | CONFIRMED |
| P8 (W_{6,4} outperforms >=2 XOR) | REFUTED (simple wins everywhere) |
| P9-P13 | 5 untestable (B has 0 XOR10 winners) |

Score: 7/13 confirmed, 1 major refutation (P8), 5 untestable.

**Provenance:** Demo 102.

---

## 65. Pure Dimension Scaling Test — Demo 103

### BFS Depth Profile (identical for W_{6,0} dim=5 and W_{6,2} dim=9)

| Depth | New entries | Cumulative | Growth ratio | Max magnitude |
|-------|------------|------------|--------------|---------------|
| 0 | 1 | 1 | -- | 1 |
| 1 | 10 | 11 | 10.00 | 1 |
| 2 | 66 | 77 | 6.60 | 2 |
| 3 | 362 | 439 | 5.48 | 3 |
| 4 | 1794 | 2233 | 4.96 | 5 |
| 5 | 8370 | 10603 | 4.67 | 8 |
| 6 | 22165 | 32768* | 2.65* | 16 |

*Truncated by 32768 cap.

### XOR Capacity Comparison (k=128, deep bf=30)

| XOR level | A (W_{6,0}, dim=5) | B (W_{6,2}, dim=9) |
|-----------|-------------------|-------------------|
| XOR6 | 2449 | 2238 |
| XOR8 | 850 | 557 |
| XOR10 | 1 | 0 |
| XOR12 | 0 | 0 |

### Subset-Hash Activation Comparison (k=128, deep bf=30)

| Activation | Components | XOR6 | XOR8 | XOR10 | XOR12 |
|------------|-----------|------|------|-------|-------|
| A full | 100 | 2449 | 850 | 1 | 0 |
| B full | 324 | 2238 | 557 | 0 | 0 |
| B sub5 | 100 | 2238 | 701 | 1 | 0 |
| B stride3 | ~108 | 2087 | 767 | 3 | 0 |
| B sub7 | 196 | 2538 | 998 | 1 | 0 |

### Hub Frequency Distributions (XOR6 winners)

| Module | Total winners | Freq values observed | Super-hubs (>33%) |
|--------|--------------|---------------------|-------------------|
| A (W_{6,0}) | 2449 | 181, 191, 198 | 0 |
| B (W_{6,2}) | 2238 | 173, 186 | 0 |

**Provenance:** Demo 103.

---

## 66. Activation Coarseness Sweep — Demo 104

### Rank Comparison

| Module | Raw Rank | Sign Rank | Total Cols | Raw % | Sign % |
|--------|----------|-----------|------------|-------|--------|
| W_{6,0} (dim=5) | 100 | 100 | 100 | 100.0% | 100.0% |
| W_{6,2} (dim=9) | 244 | 292 | 324 | 75.3% | 90.1% |

### Component Sweep (k=128)

W_{6,0} (dim=5, max=100 comp):

| comp | XOR6 |
|------|------|
| 10 | 234 |
| 20 | 1166 |
| 25 | 1457 |
| 40 | 1678 |
| 50 | 2372 |
| 60 | 2353 |
| 75 | 2318 |
| 80 | 2370 |
| 100 | 2449 |

W_{6,2} (dim=9, max=324 comp):

| comp | XOR6 |
|------|------|
| 10 | 254 |
| 20 | 817 |
| 25 | 1186 |
| 40 | 1703 |
| 50 | 1704 |
| 60 | 1885 |
| 75 | 2164 |
| 80 | 2101 |
| 100 | 2360 |
| **120** | **2486** |
| 140 | 2375 |
| 150 | 2300 |
| 175 | 2195 |
| 200 | 2265 |
| 250 | 2190 |
| 300 | 2412 |
| 324 | 2238 |

Atkinson peak at 120 components for W_{6,2}: beats W_{6,0}'s maximum 2449.

### Random-100 Variants (W_{6,2})

| Trial | XOR6 |
|-------|------|
| Sequential first-100 | 2360 |
| Random trial 1 | 2318 |
| Random trial 2 | 2195 |
| Random trial 3 | 2347 |
| Random trial 4 | 2367 |
| Random trial 5 | 2105 |
| Mean | 2266 |
| Spread (max-min) | 262 |

### Branching Decomposition

| Activation | comp | XOR6 | XOR8 |
|------------|------|------|------|
| A full | 100 | 2449 | (ref) |
| B full | 324 | 2238 | 557 |
| B sub6 | 144 | 2415 | 720 |
| B cross-block | 160 | 2412 | 952 |
| B within-block | 164 | 2235 | - |

### Multi-Hash Summary (k=64x64=4096)

| Test | comp | XOR6 | XOR8 |
|------|------|------|------|
| Ref: cross 1-hash | 160 | 3635 | 19994 |
| T1: row split | 180+144 | 3852 | 22005 |
| T2: cross-dir split | 80+80 | 3534 | 17939 |
| T3: cross+diag | 160+64 | 3806 | 21525 |

### k-Sweep

W_{6,2} cross-block (160 comp):

| k | XOR6 | XOR8 |
|------|------|------|
| 128 | 2412 | 952 |
| 256 | 2978 | 3985 |
| 512 | 3369 | 9510 |
| 1024 | 3511 | 14503 |
| 2048 | 3566 | 17549 |
| 4096 | 3635 | 19994 |

W_{6,0} full (100 comp):

| k | XOR6 | XOR8 |
|------|------|------|
| 128 | 2449 | 850 |
| 256 | 3128 | 3894 |
| 512 | 3508 | 9414 |
| 1024 | 3730 | 15173 |
| 2048 | 3807 | 19059 |
| 4096 | 3866 | 21699 |

W_{6,0} wins at every k value in the sweep.

**Provenance:** Demo 104.

---

## 67. 8-Strand k-Regime Test — Demo 105

### W_{8,0} BFS Catalog (cap 32768, 14 generators)

| Round | Cumulative | New | max_abs |
|-------|-----------|-----|---------|
| 0 | 1 | 1 | 1 |
| 1 | 15 | 14 | 1 |
| 2 | 137 | 122 | 2 |
| 3 | 987 | 850 | 3 |
| 4 | 6189 | 5202 | 5 |
| 5 | 32768 | 26579 | 8 |

### W_{8,0} k-Sweep (bf=30)

| k | XOR6 | XOR8 | k/2^8 |
|------|------|-------|-------|
| 128 | 2898 | 5824 | 0.5 |
| 256 | 3345 | 10413 | 1.0 |
| 512 | 3529 | 15982 | 2.0 |
| 1024 | 3663 | 19403 | 4.0 |
| 2048 | 3705 | 21332 | 8.0 |
| 4096 | 3721 | 22266 | 16.0 |

### W_{8,0} Rank Saturation (784 cols, 32768 catalog entries)

| Entries | Raw rank | Raw% | Sign rank | Sign% |
|---------|----------|------|-----------|-------|
| 128 | 81 | 10.3% | 91 | 11.6% |
| 256 | 139 | 17.7% | 186 | 23.7% |
| 512 | 211 | 26.9% | 310 | 39.5% |
| 1024 | 274 | 34.9% | 425 | 54.2% |
| 2048 | 416 | 53.1% | 651 | 83.0% |
| 4096 | 498 | 63.5% | 686 | 87.5% |
| 8192 | 614 | 78.3% | 729 | 93.0% |
| 16384 | 684 | 87.2% | 757 | 96.6% |
| 32768 | 710 | 90.6% | 767 | 97.8% |

### W_{8,2} Rank Saturation (3136 cols, 16384 catalog entries)

| Entries | Raw rank | Raw% | Sign rank | Sign% |
|---------|----------|------|-----------|-------|
| 128 | 90 | 2.9% | 128 | 4.1% |
| 256 | 180 | 5.7% | 256 | 8.2% |
| 512 | 310 | 9.9% | 443 | 14.1% |
| 1024 | 499 | 15.9% | 783 | 25.0% |
| 2048 | 902 | 28.8% | — | — |
| 4096 | 1178 | 37.6% | — | — |
| 8192 | 1702 | 54.3% | — | — |
| 16384 | 2003 | 63.9% | — | — |

Final sign-rank at 16384 = 2003 (63.9%), expansion 1.83x.

### W_{8,2} Branching Decomposition (k=128, bf=30)

| Activation | Components | XOR6 | XOR8 |
|------------|-----------|------|------|
| Full | 3136 | 2898 | 5664 |
| Cross-block | 1568 | 532 | 733 |
| Within-block | 1568 | 2940 | 4743 |

Within-block > cross-block: OPPOSITE of D104's W_{6,2}.

### Corrected Retroactive Rank Table (All Modules, at Catalog Saturation)

| Module | dim | cols | catalog | raw | raw% | sign | sign% | exp |
|--------|-----|------|---------|-----|------|------|-------|-----|
| Delta_1 | 2 | 16 | 32768 | 16 | 100% | 16 | 100% | 1.00x |
| W_{4,2} | 3 | 36 | 32768 | 28 | 77.8% | 32 | 88.9% | 1.14x |
| W_{5,3} | 4 | 64 | 32768 | 62 | 96.9% | 64 | 100% | 1.03x |
| W_{6,0} | 5 | 100 | 32768 | 100 | 100% | 100 | 100% | 1.00x |
| W_{6,2} | 9 | 324 | — | 244 | 75.3% | 292 | 90.1% | 1.20x |
| W_{8,0} | 14 | 784 | 32768 | 710 | 90.6% | 767 | 97.8% | 1.08x |
| W_{8,2} | 28 | 3136 | 16384 | 1096 | 34.9% | 2003 | 63.9% | 1.83x |

**Provenance:** Demo 105.

---

## 68. Topological Entropy — Demo 106

### Contingency Table (k=128, first 128 entries of W_{4,2} catalog)

| Class         | Total | XOR>0 | XOR=0 | Rate  | Mean Score |
|---------------|-------|-------|-------|-------|------------|
| Identity      | 1     | 1     | 0     | 100%  | 4574       |
| Periodic      | 88    | 88    | 0     | 100%  | 3969       |
| Pseudo-Anosov | 39    | 39    | 0     | 100%  | 3857       |

### Boolean Function Sweep (periodic rate vs pA rate, k=128)

| Function | Periodic | pA    | Gap    |
|----------|----------|-------|--------|
| AND3     | 92.0%    | 92.9% | +0.9pp |
| OR3      | 95.1%    | 93.4% | -1.7pp |
| XOR3     | 86.9%    | 86.0% | -0.9pp |
| MAJ3     | 86.3%    | 87.5% | +1.2pp |
| NAND3    | 92.0%    | 92.9% | +0.9pp |
| NOR3     | 95.1%    | 93.4% | -1.7pp |
| AND2     | 88.4%    | 87.9% | -0.5pp |
| XOR2     | 90.8%    | 87.1% | -3.7pp |

Maximum gap: 3.7pp (XOR2), well within noise. Total Boolean function null.

### Pseudo-Anosov Fraction by Depth

| Depth | Total  | Periodic | pA     | pA%   |
|-------|--------|----------|--------|-------|
| 0     | 1      | 0        | 0      | --    |
| 1     | 6      | 6        | 0      | 0.0%  |
| 2     | 26     | 18       | 8      | 30.8% |
| 3     | 98     | 66       | 32     | 32.7% |
| 4     | 338    | 192      | 146    | 43.2% |
| 5     | 1110   | 548      | 562    | 50.6% |
| 6     | 3444   | 1210     | 2234   | 64.9% |
| 7     | 10390  | 2974     | 7416   | 71.4% |
| 8     | 17355  | 3952     | 13403  | 77.2% |

Crossover at depth 5 (50.6%). Fibonacci pA characteristic polynomial: x^3 - 4x^2 + 4x - 1, spectral radius phi^2 = 2.618..., entropy/crossing = log(phi) = 0.481212... (theoretical optimum per Band & Boyland 2007).

**Provenance:** Demo 106.

---

## 69. Algebraic Graph Density — Demo 107

### Distinct Value Growth by Depth (W_{4,2} catalog, 32768 entries)

| Depth | Entries | Cumul Distinct | New |
|-------|---------|----------------|-----|
| 0     | 1       | 2              | +2  |
| 1     | 6       | 4              | +2  |
| 2     | 26      | 7              | +3  |
| 3     | 98      | 13             | +6  |
| 4     | 338     | 23             | +10 |
| 5     | 1110    | 41             | +18 |
| 6     | 3444    | 68             | +27 |
| 7     | 10390   | 114            | +46 |
| 8     | 17355   | 186            | +72 |

186 values decomposed by axis: a-axis (zeta^0) range [-19,+34] (53 values), b-axis (zeta^1) range [-21,+15] (36 values), c-axis (zeta^2) range [-30,+30] (60 values), d-axis (zeta^3) range [-15,+21] (36 values), plus 1 zero.

### Bipartiteness Verification

| Module | Dim | Bipartite | Connected | Partition |
|--------|-----|-----------|-----------|-----------|
| TL_2   | 2   | YES       | YES       | 1+1       |
| TL_3   | 5   | YES       | YES       | 3+2       |
| TL_4   | 14  | YES       | YES       | 7+7       |
| TL_5   | 42  | YES       | YES       | 21+21     |
| TL_6   | 132 | YES       | YES       | 66+66     |
| W_{3,1}| 2   | YES       | --        | 1+1       |
| W_{4,2}| 3   | YES       | --        | 2+1       |
| W_{5,3}| 4   | YES       | --        | 2+2       |
| W_{6,0}| 5   | YES       | --        | 3+2       |
| W_{6,2}| 9   | YES       | --        | 5+4       |
| W_{6,4}| 5   | YES       | --        | 3+2       |

### W_{6,2} Bipartite Invariant Candidates

| Invariant | Match |
|-----------|-------|
| maj mod 2 | 9/9 MATCH |
| nest mod 2 | 7/9 FAIL |
| tsl mod 2 | 6/9 FAIL |
| lae mod 2 | 5/9 FAIL |

Major index is the ONLY working invariant.

### 2-Adic Valuation Distribution (186 values)

| v_2 level | Cumulative count |
|-----------|-----------------|
| v_2=0 | 95 |
| v_2=1 | 48 |
| v_2=2 | 24 |
| v_2=3 | 11 |
| v_2>=4 | 7 |

Geometric decay ~0.5x per level.

### Axis Distribution (complete depths 0-7)

Even (a+c) = 41, odd (b+d) = 72. Odd dominates at unbiased depths; even dominance in full catalog is truncation artifact.

### Orbit Structure (full 186 values)

35 root-of-unity orbits = 35 norm classes (identical for axis-aligned values), 105 Galois orbits (finer partition). Divisibility density: 14.2%.

### Galois Asymmetry

sigma_5 fixes ALL 126 even-axis values; sigma_3 fixes 66; sigma_7 fixes 54. All Galois edge sets are 1-regular (perfect matchings on non-fixed points). b-axis isomorphic to d-axis across all 13 edge types.

**Provenance:** Demo 107.

---

## 70. Parity Vocabulary — Demo 108

### 3-Way Classification (100 standard Z[zeta_8] bracket catalog)

41 parity / 17 poison-only / 42 neutral. 14-value parity/poison overlap.

### Axis Distribution

| Group | Axis 0 | Axis 1 | Axis 2 | Axis 3 |
|-------|--------|--------|--------|--------|
| Parity (41) | 9 | 10 | 14 | 8 |
| Poison (17) | 0 | 2 | 9 | 6 |
| Neutral (42) | 19 | 7 | 11 | 5 |

### Octant Distribution

| Group | oct0 | oct1 | oct2 | oct3 | oct4 | oct5 | oct6 | oct7 |
|-------|------|------|------|------|------|------|------|------|
| Parity | 0 | 0 | 14 | 0 | 9 | 10 | 0 | 8 |
| Poison | 0 | 2 | 2 | 6 | 0 | 0 | 7 | 0 |
| Neutral | 17 | 6 | 1 | 5 | 2 | 1 | 10 | 0 |

Parity octants = {2,4,5,7}, poison octants = {1,2,3,6} — nearly complementary tiling.

### Galois Stabilizer Gradient

| Stabilizer | Catalog | Parity | Parity % | Poison | Poison % |
|------------|---------|--------|----------|--------|----------|
| stab=4 (axis 0) | 28 | 9 | 32.1% | 0 | 0.0% |
| stab=2 (axis 2) | 34 | 14 | 41.2% | 9 | 26.5% |
| stab=1 (axes 1,3) | 38 | 18 | 47.4% | 8 | 21.1% |

Lower Galois symmetry correlates with higher parity capability.

### Key Edge Comparisons (Parity vs Poison)

| Edge type | Parity edges | Parity density | Poison edges | Poison density |
|-----------|-------------|---------------|-------------|---------------|
| product_closure | 17 | 21% | 0 | -- |
| additive_closure | 49 | 10% | 3 | 30% |
| add. closure / v_2 | 5/6 (83%) | -- | 1 (K_2) | -- |
| difference_closure | 110 | 13% | 6 | 21% |
| negation | 0 | -- | 2 | 33% |
| galois_s7 | 0 | -- | 4 | 33% |

### Partition Hierarchy

| Set | Root groups | Galois groups | Norm groups | v_2 groups | Coincidence |
|-----|-----------|-------------|------------|-----------|-------------|
| Parity (41) | 21 | 37 | 21 | 5 | Three distinct levels |
| Poison (17) | 15 | 15 | 15 | -- | Triple collapse (all same) |

**Provenance:** Demo 108.

---

## 71. D55 Value Set Graph Analysis — Demo 109

### Catalog and Vocabulary Comparison (D108 vs D109)

| Metric | D108 (delta=0) | D109 (delta=sqrt(2)) |
|---|---|---|
| Catalog size | 100 | 56 |
| Parity vocabulary | 41 (41%) | 28 (50%) |
| Non-parity | 59 (59%) | 28 (50%) |
| Parity solutions | 906 | 9,334 |
| Simplest activation | k=6 sectors | Re>0 |
| Algebra | Z[zeta_8] | Z[zeta_16] |
| Encoding | additive | multiplicative |

### Edge Analysis — Parity (28 values) vs Non-Parity (28 values)

| Edge type | Parity | Non-parity |
|---|---|---|
| product_closure | 56e, 14.8%, 6 comp (+4 iso) | 176e, 46.6%, 1 comp (+0 iso) |
| same_axis | 86e, 22.8% | 86e, 22.8% |
| additive_closure | 20e, 23.3%, 8 comp (+4 iso) | 20e, 23.3%, 8 comp (+4 iso) |
| difference_closure | 52e, 13.8% | 52e, 13.8% |
| same_norm | 90e, 23.8%, 4 classes | 90e, 23.8%, 4 classes |
| same_v2 | 90e, 23.8%, 4 classes | 90e, 23.8%, 4 classes |
| negation | 0 pairs | 0 pairs |
| divisibility | 50e, 13.2% | 50e, 13.2% |

7 of 8 edge types identical. Product closure is the sole discriminator.

### Quotient Structure (identical for both sets)

| Metric | Parity | Non-parity |
|---|---|---|
| Axis distribution | (8, 6, 8, 6) | (8, 6, 8, 6) |
| Norm classes | 4 | 4 |
| v2 classes | 4 | 4 |
| Galois orbits | 15 | 15 |
| Product axis quotient | K_4 (6/6) | K_4 (6/6) |
| Additive v2 quotient | 3/6 (50%) | 3/6 (50%) |

### Cross-Demo Product Closure Comparison

| | D108-parity | D109-parity | D108-poison | D109-non-parity |
|---|---|---|---|---|
| Product closure | 17e (21%) | 56e (14.8%) | 0e | 176e (46.6%) |
| Product axis quotient | P_3 (3/6) | K_4 (6/6) | empty (0/3) | K_4 (6/6) |
| Additive v2 quotient | 83% (5/6) | 50% (3/6) | K_2 (1 edge) | 50% (3/6) |

Product closure inversion: at delta=0 (additive encoding), parity=HIGH; at delta=sqrt(2) (multiplicative encoding), parity=LOW.

### Prediction Scorecard (5 predictions)

| # | Prediction | Result | Verdict |
|---|---|---|---|
| 1 | Product density > 50% | 14.8% | FALSIFIED (revealed encoding-dependence) |
| 2 | Product axis quotient near K_4 | K_4 (6/6, 100%) | CONFIRMED |
| 3 | Same-axis additive density > D108's 9% | 23.3% | CONFIRMED |
| 4 | More norm classes than D108's 21 | 4 (fewer) | FALSIFIED |
| 5 | Counter: if product sparse, dual-channel falsified | 56 edges, counter triggered | COUNTER TRIGGERED |

**Provenance:** Demo 109.

---

*Generated 2026-02-20 from demo-index.md (2845 lines) and explorers-log.md (1273 lines). Updated with Demos 38, 39, 60. Updated 2026-02-21 with Demos 64–71. Updated 2026-02-21 with Demos 72–82. Updated 2026-02-21 with Demos 83–84. Updated 2026-02-23 with Demos 85–92. Updated 2026-02-24 with Demos 93–94. Updated 2026-02-24 with Demos 95–101. Updated 2026-02-26 with Demos 102–109.*
