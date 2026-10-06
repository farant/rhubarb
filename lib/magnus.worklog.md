# magnus.worklog.md

## 2026-10-05 — natus

`magnus` (Latin "great"): exact integers of any size. The first of the
house math foundations Fran placed in the HOUSE tier (not knotapel/) after
the knotapel read-through: demos 110–112 built whole chapters of results on
silent 64-bit overflow in a private `Rat` type (docs/knotapel.html notes 58
and 59: with 128-bit integers, 6₃'s construction-word spectrum went from 37
polynomials to 6, the figure-eight's "8 working scales" to 111). Planned
order: magnus → rationals (`fractio`) → polynomials → exact linear algebra;
knotapel's knot library and a rebuilt raqiya consume them.

Design (API reviewed with Fran before code):

- **Immutable values, piscina memory**, like chorda. Every operation returns
  a new value. Limbs are never written after creation, so results may SHARE
  limbs with arguments (`magnus_nega` flips the sign over the same limbs);
  the header says a result lives as long as its arguments' piscina.
- **Canonical form is an invariant**: a value that fits `s64` is ALWAYS
  stored inline (`membra == NIHIL`), anything else has ≥ 2 limbs and a
  non-zero top limb. One representation per number; the tests check it after
  every operation (`_canonicus`).
- **Fast path in `s64`** for add/subtract (bounds checked BEFORE the
  operation — signed overflow is undefined in C), multiply when both factors
  are < 2^31 in magnitude, and division; no allocation. Everything else goes
  through magnitudes.
- **Magnitudes are unsigned 32-bit limbs** (`i32` is unsigned in the house),
  little-endian, with `i64` intermediates. Unsigned wraparound is defined;
  each inner step is exact: (2^32−1)^2 + 2(2^32−1) = 2^64 − 1. Borrow is
  read from the top bit of the wrapped 64-bit difference.
- **Euclidean division**: 0 ≤ r < |b| always. C89 leaves the sign of `%`
  with negative operands to the implementation; the fast path divides the
  unsigned magnitudes and adjusts, so no signed `/` or `%` is ever used.
- **Division**: Knuth's algorithm D (TAOCP 4.3.1) for multi-limb divisors,
  short division for one limb. Normalization shifts are done on a combined
  64-bit value so the shift count never reaches 32 (shifting a 32-bit value
  by 32 is undefined).
- **No floating point anywhere** (Fran, explicitly): decimal output divides
  repeatedly by 10^9 in integers and sizes its buffer from the chunk count
  (9 digits per chunk, no log10); quotient digits are estimated by integer
  division.
- **gcd with Bézout witnesses** (`_testatus`): g = u·a + v·b, a certificate
  checkable without this library.

Verification:

- **Independent oracle**: 21 vectors from Python integers (sum, difference,
  product, Euclidean quotient and remainder, gcd), plus 12^100 and 100!.
  Two vectors are Hacker's Delight's 16-bit Knuth-D edge cases scaled to 32
  bits; an instrumented scratch build confirmed both reach step D6
  (add-back, ~2/2^32 of quotient digits at random — random tests would
  essentially never hit it) and one also the D3 estimate correction.
- **Properties over 1500 random triples** (sors seed 2026; numbers up to 80
  digits plus a list of s64/2^32/2^64 boundary values): string round trip,
  (a+b)−b = a, commutativity, distributivity, a = qb + r with 0 ≤ r < |b|,
  g = ua + vb with g | a and g | b, canonical form of every result.
- **Planted faults** (each red, then restored byte-identical): D6 add-back
  removed (caught by the Python vectors); add fast path without its bounds
  check (caught by properties, case 29); Euclidean adjustment skipped on
  the multi-limb path (caught by properties, case 6).
- 1204 checks green; strict house flags; formator CONFORMIS; Latin lint
  NOVA 0 (checked after refreshing the index — a planted nonsense word WAS
  caught, so the check is live).

Tooling notes from this birth:

- A new lib/*.c is not linked by `./compile_tests.sh` until
  `./tools/compile_tests_fontes_generare.sh` regenerates
  `compile_tests_fontes_generata.sh` (aedilis closures). In this worktree
  bin/aedilis was stale (no `--thesaurus`): `./tools/aedilis_struere.sh`
  first.
- C89 caps string literals at 509 characters (`-pedantic` warns); long
  test vectors are split into adjacent literals, which also keeps the
  72-column rule.

Deferred (each addable without changing the API): shifts and bit
operations, square root, modular exponentiation, Karatsuba, an in-place
API, and a small-value path for parsing (parsing always allocates today).

## 2026-10-05 — first review agent: findings and fixes

A read-only review agent (differential fuzzing against Python under
UBSan/ASan, 80k cases, operands to 45k digits, ~40 mutants) found no wrong
arithmetic. Verified and acted on:

- **Misaligned limbs** (UB; `piscina_allocare` aligned to 1). Fixed house-wide
  by Fran's decision: piscina default alignment 8 (dfa6a05a); magnus also
  allocates limbs explicitly aligned.
- **gcd memory grew with the number of Euclid steps**: every step allocated
  fresh limbs in the CALLER's piscina. Now Euclid runs in two internal
  scratch piscinae used alternately (each step computes into the other,
  copies the survivors there, empties the old one) and only the result is
  copied to the caller. Small inputs keep the old allocation-free loop.
  Measured on F(10000), F(9999) (Euclid's worst case, ~2090 digits): caller
  usage 13.2 MB → 0 B (gcd), 30.8 MB → 1.7 KB (with witnesses); peak RSS
  38 MB → 5.6 MB; time unchanged.
- **Vacuous oracle**: with `magnus_aequalis` replaced by `redde VERUM` the
  whole suite passed — every property used it as the judge. Added
  falsifiability checks (small, big, mixed pairs) and the property
  a ≠ a + 1; that mutant now fails 5 checks and the properties at case 0.
- Tests added: null-data chorda (FALSUM, output untouched); NIHIL
  out-parameters (divide fast and slow path, `ad_s64`); 10^10000 printed
  (10001 chars) and parsed back; Fibonacci gcd identity
  gcd(F_m, F_n) = F_gcd(m,n) and Bézout on F(10000); caller-memory bounds
  (< 496 B, < 16 KB) — the old gcd fails them with 13.2 MB / 30.8 MB.
- Dead store after Knuth D6 removed (final carry ignored, as Knuth says);
  stale digits comment fixed.

Lesson for the process: my planted faults all targeted the arithmetic,
never the oracle; an independent reviewer found the oracle hole at once.
1234 checks.

## 2026-10-05 — second review pass: the gcd fix had cost speed

The reviewer (same agent, second pass) verified every first-pass fix
(random 30k-digit gcd: caller memory 1.1 GB → 0) and found no lifetime bug
in the alternating-arena code — but medium gcds, and so every fractio
operation, had become 2–5× slower. My "time unchanged" came from ONE large
measurement; true there, false at 20–300 digits. Cause: `piscina_vacare`
memsets the whole capacity, and I called it every Euclid step.

Fixes (reviewer's variants A + B, re-measured here):
- Each scratch piscina is marked right after creation and rolled back with
  `piscina_reficere` (offsets only, no clearing; `_membra_nova` zeroes new
  limbs anyway).
- Operands of at most `MAGNUS_LIMES_ALTERNARUM` = 4 limbs (~38 digits) keep
  the old loop in the caller's piscina (bounded waste of a few KB).
- µs/op, old (pre-arena) vs now: gcd 20 digits 0.57 / 0.60; 40: 2.84 /
  3.53; 100: 10.6 / 12.0; 300: 58.7 / 59.4. fractio_adde follows gcd.
  Memory fix intact.
- New diagnostic `magnus_apex_alternarum()`: the largest scratch-piscina
  usage in the most recent large gcd (deterministic — the first of the
  agenda A3 cost counters). F(10000)/F(9999): 3488 B (gcd), 5240 B
  (witnesses); 0 on the small path.

Coverage gaps the reviewer showed (mutants that survived): result left in
the destroyed scratch piscina (A02) — now a test computes a second large gcd
and re-checks the first result (fails without any sanitizer); scratch never
emptied (A10, RSS 1.3 → 168 MB unnoticed) — now bounded through the apex
(< 16 KB; the mutant reads 8.8 MB / 21.9 MB). Not covered by the suite: the
fallback when a scratch piscina cannot be created (reviewer verified it by
fault injection). 1240 checks.

## 2026-10-05 — review III: scratch piscinae only when BOTH operands are big

The reviewer measured a time jump around 40 digits in fractio: gcd(big, 1)
— constant in fractio (every integer has denominator 1) — opened the two
scratch piscinae because ONE operand was over the threshold. Now
`_per_alternas` requires both operands to exceed `MAGNUS_LIMES_ALTERNARUM`.
Why the caller's memory stays bounded when one is small: after the first
Euclid step the remainder is smaller than the small operand, so from the
second step on everything is small (the witness variant's first quotient
can be big, but it is one value, not a growth per step). Test: gcd(big,
1001) and the Bézout gcd(−7, big) give correct results with
`magnus_apex_alternarum() == 0`; with the old `||` both apex checks are
red. 1245 checks.
