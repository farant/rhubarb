## Demo 02: Braid Words

- **Status**: COMPLETE
- **File**: `demo_02_braid_words/main.c` (~953 lines)
- **Tests**: 9/9 pass (with warnings — 6 unused PolyMat functions; does not compile clean with -Werror)
- **Depends on**: Demo 01 (reuses Laurent polynomial algebra — Poly type copied, not imported)
- **Feeds into**: Demo 03 (Markov trace / TL module structure deferred there)

### Headline
Computes Kauffman brackets of knots from braid word input via state-sum over resolutions, confirming agreement with Demo 01's PD-code approach.

### Key Results
- Braid closure state-sum reproduces all Demo 01 bracket values (trefoil, figure-eight, Hopf link, unknot)
- Chirality detected: sigma_1^3 and sigma_1^{-3} produce mirror-image brackets (A ↔ A^{-1})
- Figure-eight bracket is palindromic (amphichiral confirmation)
- 2-strand TL representation: rho(sigma_1) = -A^{-3}, rho(sigma_1^{-1}) = -A^3 (scalar in 1D module)
- Markov equivalence across braid representations (2-strand vs 3-strand trefoil) documented as open — writhe normalization alone insufficient, Markov stabilization correction needed

### Theorems/Conjectures
- **Confirmed**: Braid closure state-sum = PD-code state-sum for Kauffman bracket
- **Open**: Markov stabilization correction (adding strands changes bracket; f-polynomial doesn't fully normalize this)
- **Noted**: Full Markov trace via TL module structure deferred to Demo 03

### Code Assets
- `Poly` type: Laurent polynomial ring (copy of Demo 01 — p_zero, p_mono, p_trim, p_add, p_mul, p_eq, p_print)
- `PolyMat` type: matrices over polynomial ring (pm_zero, pm_identity, pm_mul, pm_scale, pm_add, pm_trace, pm_print) — implemented but **unused** in tests, scaffolded for future TL representation work
- `Braid` type: braid word representation (word array of signed generator indices, strand count)
- `braid_count_loops()`: union-find loop counter for resolved braid closures
- `braid_bracket()`: state-sum Kauffman bracket from braid word
- `braid_writhe()` / `braid_jones()`: writhe computation and writhe-normalized f-polynomial
- Union-find (global arrays, path-compressed): reusable pattern for loop counting in any diagram resolution

### Literature Touched
- Temperley-Lieb algebra TL_n(delta) and its path/standard module representation
- Kauffman bracket via braid closure (Kauffman 1987)
- Markov's theorem (braid equivalence under stabilization and conjugation)
- Reduced Burau representation (mentioned, not implemented)
- Spin-chain / tensor product representation of TL generators (explored in comments, abandoned for simpler state-sum)

### Open Questions
- Markov stabilization: how to normalize bracket across different braid strand counts for same knot
- PolyMat infrastructure unused — will Demo 03 use it for explicit TL module matrices?
- Code duplication: Poly type copied verbatim from Demo 01 (no shared library extraction yet)
- Does not compile with `-Werror` due to 6 unused PolyMat functions
