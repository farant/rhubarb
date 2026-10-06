# congruentia.worklog.md

## 2026-10-06 — natus

`congruentia` (Gauss's term, Disquisitiones art. 1): exact modular
arithmetic, #2 in Fran's math map — the remaining piece of the "exact
core". API approved by Fran: residues are bare `i32` with an explicit
modulus (no struct: hot loops cost nothing); any modulus 2 ≤ n < 2^32
(products fit `i64`, C89 has no 128-bit); inverse refuses when gcd ≠ 1.
Scope now: reduction (s64, magnus), add/subtract/multiply/power,
inverse, symmetric representative, deterministic primality, a descending
prime sequence for CRT, Garner reconstruction (optionally symmetric).
Later: the Z/n ring for `anulus` (after the descriptor refactor — Fran
approved giving every ring function the descriptor as first argument),
NTT with #17, multimodular determinant/Hermite/Smith with agenda A8,
rational reconstruction when a consumer needs it.

Design notes:
- `ex_s64` handles negatives without `%` on negative operands (C89 leaves
  that implementation-defined) and S64_MIN via −(x + 1) + 1.
- `inversa`: extended Euclid in s64 (all quantities ≤ n < 2^32), only
  non-negative divisions.
- Primality: Miller–Rabin with bases 2, 7, 61 — deterministic for n <
  4,759,123,141 (Jaeschke), so for every i32.
- `primus_infra(limes)`: largest prime < limes, so p0 =
  primus_infra(0xFFFFFFFF) = 4294967291, p1 = primus_infra(p0), …
- `restitue` (Garner over magnus): checks residue < modulus and modulus
  ≥ 2 first; non-coprime moduli are found when the running product has
  no inverse mod the next modulus. Symmetric: x − M when 2x > M.
- Needs `magnus_residuum_parvum` (new, in magnus).

Verification:
- **Oracle** (scratchpad congruentia/generare.py): Python integers,
  `pow(a, -1, n)`, primality by trial division, CRT by the direct sum
  Σ r_i M_i (M_i^-1 mod m_i) — not Garner. 449 vectors: reductions of
  s64 extremes and 60-digit integers over ten moduli (2, 10, 65536,
  2^31−1, 2^32−1, 4294967291, …), 200 arithmetic rows (sum, difference,
  product, power with exponents up to 2^64−1, inverse or refusal,
  symmetric), special numbers for primality (Carmichael 561, 41041,
  825265, 321197185; strong pseudoprimes to base 2 — 2047, 3277, 4033,
  4681; 3215031751, which fools bases 2, 3, 5 and 7 but not 61), prime
  predecessors, 60 CRT cases (prime and random moduli, ~30% non-coprime
  → refusal), signed and unsigned.
- **Sieve of Eratosthenes** in the test itself for every n < 2^20 against
  Miller–Rabin (and π(2^20) = 82025).
- Laws over 500 random cases (Fermat a^(p−1) = 1 for the 8 largest primes
  below 2^32, a·a^-1 = 1, a^-1 = a^(p−2), composite moduli) and 200 CRT
  round trips: random signed integers up to 60 digits, reduced mod the
  largest primes until M > 2|x|, reconstructed symmetrically = x.
- Refusals with exitus untouched: inverse of 0, gcd ≠ 1, n < 2; CRT with
  residue = modulus, modulus 1, non-coprime moduli.
- **Plants, all red**: inverse returning the wrong Bézout coefficient;
  Miller–Rabin without base 61 (3215031751 slips through); negative
  reduction without the zero case; multiply in 32 bits (overflow — red
  after 18 minutes: a broken primality test makes `primus_infra` crawl
  down through billions of candidates); Garner without the symmetric
  step; `magnus_residuum_parvum` without the sign; 4 called prime; prime
  predecessor including the limit itself.
- 23 checks (vectors are checked per group); formator CONFORMIS; lint
  NOVA 0 after `reconstrue` → `restitue`, `sinici` → `restitutionis`,
  and a glossary entry for `symmetricus` (neo-Latin). `signatus` is a
  latina macro (`signed`) — examen refused it as a parameter name.
