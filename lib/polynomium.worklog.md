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
