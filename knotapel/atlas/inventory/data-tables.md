# Data Tables — DKC Research Atlas

All numerical tables extracted from the demo index and explorer's log. Exact numbers preserved.

*Updated 2026-02-20: Added Demo 38 (dimension reduction), Demo 39 (symmetry decomposition), Demo 60 (ell=7 cubic wall), Demo 63 (angular anatomy), Demo 39 Parts G-J (three gram forms) data.*

*Updated 2026-02-21: Added Demos 64–71 (parity matroid recursion, Clifford staircase, quaternionic DKC, coordinate system zoo, stereographic DKC, multi-root S² comparison, musica nodorum, spectral DKC).*

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

## 12. Demo Test Counts

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

**Provenance:** Demo index, all entries.

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

**Provenance:** Explorer's Log landmark table + Demos 38-39, 55-71.

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

*Generated 2026-02-20 from demo-index.md (2845 lines) and explorers-log.md (1273 lines). Updated with Demos 38, 39, 60. Updated 2026-02-21 with Demos 64–71.*
