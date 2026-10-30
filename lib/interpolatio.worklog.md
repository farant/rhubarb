# interpolatio worklog

## 2026-10-27 — T1 of project-specs/contractio-plan.md

New house library: Lagrange interpolation mod p and the evaluate →
interpolate → reconstruct driver that the vertex-model bracket (T4) and
later #SAT counts (phase 2, degree 0) use.

**Design points that came from the plan's design review:**
- The aestimator returns three states (UTILE / INUTILE / ERRATUM) and
  the driver has a per-prime attempt budget (`conatus_maximi`). With a
  plain b32, "this point is unusable" and "real failure" were the same
  value, and an always-unusable evaluator would have scanned ~2^32
  points per prime.
- The aestimator CHOOSES its point (index → punctum, valor). The
  bracket needs points where −i·A is a square mod p, and the house has
  no modular square root; enumerating ω and returning A = i·ω² walks
  exactly that set without one.
- `passus`: f(x) = Σ c_k x^(imus + passus·k) is interpolated as
  g(y) = x^(−imus)·f(x) in y = x^passus. The bracket's exponents all lie
  in one class mod 4, so passus 4 cuts the points needed about 4×.
- `interpolatio_quaestio_ordinaria()` is the only way callers should
  start a query: C89 callers set fields one by one, so a field added
  later would otherwise be garbage in old callers.

**Exactness.** Primes are enumerated FIRST (descending below
`primus_limes`, filtered by residue) until Σ floor(log2 p) ≥ limes + 2,
which guarantees product > 2^(limes+1) > 2·max|c|. Then one extra prime.
Each coefficient is reconstructed symmetric (Garner via
`congruentia_restitue`) and must (a) reduce to the extra prime's residue
and (b) have at most `limes_bitorum` bits. Per prime, `puncta_addita`
extra points must satisfy the interpolant. None of these checks catch a
model that is wrong but self-consistent; that is the oracle's job.

**Duplicate points.** y = x^passus can repeat (±x for passus 2, the
eight roots for passus 8, ±ω in the bracket). Repeats are skipped as if
INUTILE; Lagrange itself refuses duplicates (product of differences has
no inverse), so a skipped check shows up as FALSUM, not a wrong answer.

**Tests** (probationes/probatio_interpolatio.c, credo v2, 11 sections,
15,918 checks, 0.1 s): the oracle is in the test — a known random
Laurent polynomial (coefficients up to 200 bits, imus in [−20, 5],
passus 1/2/4) evaluated by direct summation, recovered exactly; Lagrange
on random polynomials mod 17, 998244353, 4294967291, 4294967161 (degree
≤ 200); coefficients ±(2^200 − 1) exact at limes 200 and refused at 199;
every prime lying in turn refused; a consistent lie at any of the first
five points refused (index K only by the extra-point check — the
reconstruction would otherwise be right); ERRATUM on call 3 stops after
exactly 3 calls; always-INUTILE stops after exactly `conatus_maximi`;
repeated points; prime filter (17 then 13 below 18; all ≡ 1 mod 4 for
large primes); too few primes; degree 0 (300-bit scalar); invalid
queries leave `exitus` untouched and never call the evaluator.

**Test gap found while planning plants:** the first version lied at one
point in the FIRST prime only; the extra prime catches that too, so the
extra-point check was never tested on its own. The lie is now constant
across all primes.

**Plants (silva.planta, gate radix filtered to interpolatio), 9/9 red:**
P1 synthetic division sign; P2 extra-point check skipped; P3 extra-prime
check skipped (first form `FALSUM && ...` inside `||` did not compile
under -Wlogical-op-parentheses — replanted as a self-comparison); P4 bit
bound skipped; P5 duplicate-y skip removed; P6 sign of imus; P7 INUTILE
and ERRATUM swapped; P8 attempt budget off by one; P9 prime filter
inverted. Each went red at the assertion meant for it.

## 2026-10-27 — T1 review (independent, after e8a13213) and fixes

Reviewer ported lib/interpolatio.c to Python line by line and fuzzed it
(Lagrange 3,000 cases, driver 400 random Laurent polynomials, passus up
to 8, imus −40..40): 0 mismatches. No H. Five M, fixed here:

- **M1** `necessaria = K + puncta_addita` wrapped in u32 (0xFFFFFFFF + 4
  = 3): Lagrange then read past the points array and the extra-point
  loop ran zero times — a possible wrong VERUM. Now refused up front
  when K + puncta_addita > 2^31 − 1.
- **M2** x = 0 was ERRATUM in code but INUTILE in the approved plan,
  and the header had been rewritten to match the code without a note.
  T4's aestimator (ω = k + 2) does hit ω ≡ 0 at small primes. Now
  INUTILE, as planned.
- **M3** the line that guarantees exactness (Σ floor(log2 p) vs the
  bound) had no test at its margin: primes near 2^32 carry a hidden
  spare bit, so "slack 0" or "ceil(log2)" survived the whole suite. New
  section "margo primorum" with primes 263, 257, 251, 241 (limes 16):
  both plants now go red. The slack itself is now +1, not +2 (Σ floor ≥
  L + 1 gives product ≥ 2^(L+1) > 2·max|c| since |c| ≤ 2^L − 1).
- **M4** the residue filter Miller-Rabin-tested every odd candidate and
  filtered afterwards: an inadmissible filter (gcd(r, m) > 1) scanned
  ~2^31 numbers. Now candidates step by the modulus, and inadmissible
  filters are refused up front.
- **M5** windows were not validated up front (the ±(2^30 − 1) exponent
  limit only bit at the very end), allocations scaled with the window
  through the fatal allocator, and at S32 extremes passus 1 wrapped K to
  0 and passus 0xFFFFFFFF looped forever. Now |imus|, |summus| ≤
  POLYNOMIUM_EXPONENS_MAXIMUS up front; large allocations use
  `piscina_conari_allocare` (FALSUM on failure); the dense output spans
  only first to last nonzero coefficient, so memory follows the result,
  not the window.

Also: **L1** `limes_bitorum + 2` wrapped — capped at
INTERPOLATIO_LIMES_MAXIMUS (2^24) and compared in s64; **L2** primes
giving fewer than K + puncta_addita distinct y ((p − 1)/gcd(passus,
p − 1)) are skipped identically in both enumeration passes (p = 17 with
passus 4 used to burn 2^20 calls); **N2** a skipped duplicate y must
carry the same g, else FALSUM (wrong passus or window, caught on the
first ±x pair); **L4** header's refusal list completed.

New tests (17 sections, 15,997 checks): margin of the prime bound;
filter step and 5 ≡ 1 (mod 4); prime skipping (passus 8 below 18:
13, 11, 7); x and p − x (18 calls exactly) and a false passus refused
after 2 calls; punctum 0 skipped, punctum p / valor p / unknown status
refused after exactly 2 calls; zero polynomial at limes 0 and 10; K = 1
at −7 and 9 (2^90); exponents near ±10^6 with passus 4; puncta_addita 0
(K calls per prime); failure only in the extra prime (INUTILE: np·3 +
conatus calls; ERRATUM: np·3 + 1); every new refusal with 0 calls and
exitus untouched; Lagrange with 18 points mod 17.

Not planted: the M1 cap and the M4 admissibility refusal — without them
the prime scan runs minutes (all primes below 2^32 fail the aptness
test), so a plant would hang rather than go red. They are covered by
the refusal tests with "0 aestimator calls".

T4 notes from the review: the aestimator must cache i = g^((p−1)/4) per
prime itself (indices restart at 0 per prime, primes descend, the extra
prime is last); set `conatus_maximi` explicitly (each call is a
contraction); a window that is too narrow is caught only with
probability ~1 − 1/p per prime by one extra point — consider
puncta_addita 2; at p = 17 the bracket's point set gives y = ω^8 = 1
for every usable A, so the planned (p = 17, A = 2) check must be a
direct aestimator unit test, not a driver run.

**Plants after the fixes (silva.planta, radix/interpolatio), 15/15 red:**
the eight earlier ones still applicable (P1-P8) plus P10 slack 0 and
P11 ceil(log2) (both red only through the new "margo primorum" — they
survived the old suite), P12 punctum 0 as ERRATUM, P13 duplicate-y
values not compared, P15 filter stepping by 1 instead of the modulus,
P17 prime aptness ignored (first form `redde VERUM` left the parameters
unused under -Werror; replanted as `... || VERUM`), P18 dense output
exponent without the first-nonzero offset.
