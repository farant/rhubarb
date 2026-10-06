# polynomium.worklog.md

## 2026-10-05 — natus

`polynomium` (neo-Latin): exact Laurent polynomials Z[t, t⁻¹] over
`magnus` — #4 in Fran's math library map, third house math foundation
after magnus and fractio. Consumer evidence: 14 knotapel demos each define
`typedef struct { int c[MAX_TERMS]; int lo, len; } Poly;` — dense Laurent,
fixed capacity, `int` coefficients (the overflow class that broke demos
110–112). API sketched and approved by Fran first.

Decisions (Fran's):

- **One Laurent type**: ordinary polynomials are the case imus ≥ 0; knot
  invariants (Kauffman bracket in A, Jones and Alexander in t) need
  negative exponents anyway. Dense, one variable, integer coefficients;
  rational and Z/p coefficient types and multivariate later, as their
  own types.
- **Text format** `3t^2 - t + 1 - 2t^-1` with the variable letter passed
  in; the parser accepts any term order, repeated exponents, free spaces;
  the printer writes the canonical form.
- **Zero polynomial**: gradus 0, `est_nullum` tells it apart.
- **Exponent overflow → b32** (option 1 of three): every operation that
  takes or moves an exponent can refuse; exitus untouched.

Found while implementing (told Fran): with exponents anywhere in s32,
`adde(t^(2³¹−1), t^(−2³¹))` has a dense span of 2³² coefficients and the
i32 count WRAPS to 0 — memory corruption, not an OOM crash. So exponents
are bounded |e| ≤ 2³⁰ − 1 (`POLYNOMIUM_EXPONENS_MAXIMUS`, public): every
sum's span fits i32, every exponent × k fits s64, the mirror is always in
range. Cost: `monomium` and `normale` also return b32 (monomium takes an
exponent; normale shifts imus to 0, which can push summus past the bound).

Design:

- Immutable values in the piscina; results may share coefficient VALUES
  (and so magnus limbs) with arguments; `translata` shares the whole array.
- Canonical form: zero = no coefficients, imus 0; otherwise the first and
  last coefficients are non-zero. Equality = equality of parts.
- **Exact division**: shift both to a non-zero constant term (t is a unit
  in Z[t, t⁻¹]), long division from the TOP, each quotient digit must
  divide exactly in Z, final remainder below deg b must be zero.
- `dilata` (t → t^k) and `contrahe` (inverse; refuses non-divisible
  exponents): contrahe by −4 turns the normalized Kauffman bracket into
  Jones (A = t^(−1/4)). C89 `%` and `/` with negative operands are avoided
  (magnitudes, then sign).
- `valor`: Horner over fractio on the coefficient array, then × x^imus
  (fractio_potentia refuses 0^negative).

Verification:

- **Oracle with different algorithms** (scratchpad polynomium/oraculum.py):
  dicts {exponent: coefficient} with Python ints; division from the LOWEST
  coefficient upward, verified by multiplying back. 620 vectors: 150
  arithmetic (inputs half in mixed non-canonical text), 150 divisions (75
  exact by construction, 75 refused), 40 powers, 80 evaluations (incl.
  x = 0 refusals), 80 dilate/contract, 60 content/normal form, 60
  text-normalization. Coefficients −5..5, ±1, and ~10^30.
- **Known knot values**: right trefoil bracket −A⁵ − A⁻³ + A⁻⁷, × (−A³)⁻³,
  contracted by −4 → Jones −t⁴ + t³ + t; its mirror differs (chiral);
  figure-eight Jones is mirror-invariant; Alexander normal forms.
- **Laws over 500 random cases** (sors): text round trip (letter 'x'),
  falsifiability (a ≠ a+1, a ≠ a·t), commutativity, associativity,
  distributivity, a + (−a) = 0, a³ = a·a·a, (a·b)/b = a, a·b + 1 refused
  by a non-unit b, valor a ring homomorphism, contrahe∘dilata = id,
  mirror involution, a(t^k)(x) = a(x^k), translata = × t^k and inverse,
  normale idempotent and invariant under −t³, content divides every
  coefficient and content(−6a) = 6·content(a). Canonical form asserted on
  every result.
- Range: monomium at ±MAX accepted, ±(MAX+1) refused; multiplica,
  potentia, translata, dilata, divide, ex_coefficientibus refusals with
  exitus untouched; boundary products accepted.
- **Planted faults, all red**: final remainder check skipped; truncated
  quotient digit accepted; top zeros not stripped; "1t" printed (first
  plant didn't COMPILE — unused variable — redone); multiply range check
  skipped; contrahe sign wrong (bus error: wrong exponent writes out of
  bounds); valor without × x^imus; aequalis ignoring coefficients;
  repeated exponents overwrite; subtrahe as adde; normale without sign.
- ASan + UBSan clean. 115 checks (vectors are checked per group);
  formator CONFORMIS; Latin lint NOVA 0 after glossary entries
  (polynomium, monomium, coefficiens — neo-Latin) and renames.

Not testable at a sane size: `normale` refusing needs summus − imus >
2³⁰ − 1, i.e. > 2³⁰ coefficients (24 GB). My first hand cases built such
spans by accident (parse of `t^MAX − t^−MAX`): macOS overcommitted and the
suite took 21 s instead of 0.24 s — dense memory is real; never write a
test with a wide sparse polynomial.

Deferred (no API change needed): gcd and pseudo-division, division with
remainder, derivative, Karatsuba/NTT multiplication (benchmarks decide),
scratch-piscina accumulation in multiply and divide (today each partial
sum of big coefficients allocates in the caller's piscina — the same
lesson as magnus gcd; matters once Bareiss calls divide_exacte in a loop).

## 2026-10-06 — review I: findings and fixes (Fran approved all four)

Reviewer: independent oracle by Kronecker substitution (pack into one big
integer), 37k differential cases, 40k parser strings, sanitizers — no wrong
result. Acted on:

- **A1, letter ≥ 0x80 never matched** (i8 data byte vs signed char; print
  then parse refused, platform-dependent). Fix: the letter must be ASCII
  a–z / A–Z (a lone byte ≥ 0x80 is not UTF-8 anyway); the parser refuses
  others; comparison cast to i8.
- **A2, a 30-byte string could ask for 51 GB** (dense span from text).
  Fix: `POLYNOMIUM_AMPLITUDO_LECTIONIS` = 2^20: the parser refuses
  summus − imus + 1 above it; the header says span is the cost. Operations
  themselves keep no such limit (adde etc. stay infallible).
  `potentia((1+t), 2^29)` is in range but infeasible (coefficient growth)
  — same class as magnus_potentia, documented, not guarded.
- **A3, multiply/divide left every partial sum in the caller's piscina**
  (200×200 terms × 1000 digits: 64 MB for a 334 KB result). Fix:
  "officinae", two internal scratch piscinae (operations with
  na·nb ≥ 64 only). Multiply: each output coefficient computed whole in a
  scratch piscina, final value copied with the new `magnus_transcribe`,
  scratch rolled back. Divide: the modified residue window [k, k+nb−1] is
  rewritten every step, so it lives in ALTERNATING scratch piscinae (as in
  magnus gcd); quotient digits copied out. Re-measured with the reviewer's
  probe: caller memory ≈ result size (64 MB → 0.3 MB; 6.6 MB → 0.04 MB),
  time unchanged.
  Hazard found while writing it: an interior ZERO coefficient of b makes
  x − 0 — that IS x, possibly still in the scratch piscina about to be
  rolled back. Guarded (copied into the live scratch). Today's magnus
  always allocates a fresh copy for x − 0, so the guard's plant survives
  (equivalent mutant); kept because magnus's contract allows sharing.
- New diagnostic `polynomium_apex_officinarum()` (like
  magnus_apex_alternarum): without it, a missing rollback was invisible
  to the suite (caller memory looks fine; only the scratch grows).
  50×50 terms × 100 digits: ~7.3 KB.
- Coverage the reviewer showed: every boundary test was a monomial
  (imus = summus), so either of a pair of range checks alone passed —
  two-term boundary tests for translata, multiplica, divide_exacte;
  ex_coefficientibus low check; coefficiens one past the end; tabs.
- Tests added: letters, span boundary (2^20 accepted, 2^20 + 1 refused),
  caller-memory and apex bounds for multiply and divide (divisor with
  interior zeros), 30 large random cases (8–40 terms, up to 60 digits,
  interior zeros) checked against a product built term by term
  (multiplica_scalari + translata + adde — a path without scratch) and
  divided back.
- Plants, all red: copy-out skipped in multiply; quotient digit not copied
  out; no rollback in multiply / divide (red only via the new apex);
  span off-by-one; non-ASCII letter accepted. 155 checks; sanitizers clean.

## 2026-10-06 — review II: officinae only when a sum can leave s64

Reviewer: no lifetime bug — a POISONED piscina (reficere overwrites
released bytes with 0xA5) plus ASan/UBSan, 10.5k fuzz cases, injected
officina-creation failures; magnus_transcribe correct. Acted on:

- **A1, term-count threshold paid time for nothing** with small
  coefficients (8×8: 0.42 → 0.80 µs) — the knot-polynomial regime, where
  the magnus fast path allocates nothing anyway. First fix: officinae
  only if some |c| ≥ 2^31. Its plant ("scan omitted") then showed the
  criterion was WRONG, not just slow: coefficients < 2^31 keep each
  product in s64, but a sum of two products (~2^62) already overflows
  and allocates in the caller. Final criterion (`_officinis_utendum`):
  na·nb ≥ 64 AND NOT (all |c| < 2^31 and max|a|·max|b|·min(na,nb) <
  2^63) — i.e. officinae unless no partial sum can leave s64. Test:
  40 terms of ±(2^31−1) squared — products fast, sums overflow; caller
  bounded and apex > 0 (the |c|-only criterion is red here).
- The coefficient-at-a-time loop itself cost 16–35% on small inputs, so
  the caller path keeps the original (i, j) loop; only the officinae
  path computes coefficient by coefficient. Re-benchmarked (reviewer's
  bench_thr, µs, before officinae → now): multiply 8×8 0.42 → 0.45,
  16×16 2.06 → 2.12, 64×64 41.1 → 44.2; divide 8×8 0.51 → 0.57, 64×64
  26.4 → 30.8 (per-step bookkeeping and the scan; not split further);
  100-digit 32×32 multiply 164 → 162, divide 179 → 168.
- Divide's interior-zero guard now runs only when officinae are in use.
- C2/C3: few big terms (7×9, 100 digits) and many small terms (40×40,
  4 digits) → apex 0 for multiply and divide; a text that CONTAINS the
  byte 0xE9 refused with letter 0xE9.
- "Scan omitted" (officinae whenever na·nb ≥ 64) still survives and must:
  where the criterion says no officinae, nothing allocates, so using
  them is invisible except in time. Equivalent by construction.
- Open, asked Fran: C1, a piscina poisoning mode as a gate — two
  lifetime mutants (rollback of the wrong scratch in divide's
  zero-digit branch; rollback before copy-out in multiply) die only on
  a poisoned build, since reficere does not free memory.
- 170 checks; ASan/UBSan clean; also clean on the reviewer's poisoned
  piscina.

## 2026-10-06 — `polynomium_transcribe`

Deep copy (new coefficient array, every coefficient through
`magnus_transcribe`) for `anulus`'s scratch hook; `translata` and friends
share arrays, so a shallow copy would not survive a rollback. Tested in
probatio_matrix's ring block (array and limbs differ; text survives
destroying the source piscina).
