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
