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
