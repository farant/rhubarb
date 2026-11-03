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

## 2026-11-02 - verificatio (interpolatio-verificatio-plan T1)

Added the second certificate kind: `interpolatio_polynomium_relatum` (PROBATUM or VERIFICATUM, with a report), `interpolatio_promovere` (VERIFICATUM -> PROBATUM by computing only the remaining primes) and `interpolatio_probationes_necessariae` (the exact error-bound function). `interpolatio_polynomium` keeps the old path byte for byte: it now calls the factored pieces (`_primos_enumerare`, `_restituere_ad_limitem`, `_polynomium_formare`) in the same order, and all 17 old sections pass unchanged.

Design points worth remembering:
- Two tiers per stage: the limes filter (a candidate coefficient >= 2^limes is wrong by the proved bound, no check), then one SELF-check with the main evaluator, and only if that passes, m certifying checks with the verificator. The bound comes from the certifying checks alone.
- m is computed EXACTLY in Magnus: (k_max - 1) a^m 2^s <= b^m with eps = a/b. The review's floor/ceil recipe over-counts by one on 13 of 36 braids (T0); plant J proves the 3x50 vector catches it.
- DISCORDIA: the verificator rejecting the same candidate twice (each time after a passed self-check), or rejecting a candidate equal to the final proved result, is FALSUM. There are NO extra certifying checks at the bound; a VERIFICATUM run that reaches k_max is just PROBATUM (status AD_LIMITEM).
- Promotion needs no stored residues: the verified Q in the symmetric range of M_k IS the CRT residue, so the first k residue rows are Q mod p_j. It regenerates the prime prefix and refuses on any mismatch; a Q corrupted by +M_k passes ADDITUS and limes, so only the final R = Q comparison catches it (plant L).
- N: the default range [2^30, 2^31) uses N_MIN = 2^24 for no filter or modulus 4 (T0 sieve: 25,348,870 / 25,348,667 / 50,697,537); any other filter is refused. Narrowed ranges (tests only, width <= 2^24) count their primes, minus k_max + 1.

The calibration test needed an adversarial polynomial: a random wrong candidate almost never passes a random-point check (a random D has ~1 root mod q, not K - 1), so a naive calibration observes 0 passes and proves nothing. P = small + p1 T(x), T = (x-1)...(x-40) makes the stage-1 error exactly -p1 T(x), with roots x = 1..40 for EVERY q: the worst case the bound allows. Observed 18/800 = 0.0225 stage-1 self-check passes (theory ~0.018), formula eps = 0.089.

The mirror test with laqueus evaluators moves to T2 (probatio_laqueus_rete); here the mirror checker evaluates f(1/x) of a test polynomial, with the precondition f(2) != f(1/2) asserted.

Plants: build/verificatio/plantae_t1.py, 12 registered, 12 red as registered (A-L). Plant L first failed to plant at all: silva.planta refuses a multi-line anchor replaced by fewer lines (format guard); rewritten as `(FALSUM && ...)` with the same line count.

## 2026-11-03 - verificatio acceptance (interpolatio-verificatio-plan T3)

All criteria met; details in the plan's "T3 results". Section XI verified: 45/45 byte-identical, 45/45 RELATIO lines equal to T0's registration (prime count, k_max, evaluations). 14 x 801 verified with the frozen 79e7fad9 kernel as checker: IDEM, 7 of 23 primes, 3262 s CPU. P2t median 3.875x over three ABBA pairs at 12 x 801 (threshold 2.5x).

Measurement lesson: on a shared machine (load 7-21 from other sessions) the absolute 12 x 801 times swung by 40% between pairs while the ABBA ratio moved by 3%. The P3 residual method ((B - evalsV x A/evalsP) / B) came out +0.33%, -1.09%, -3.07%: its noise floor here is about +/-3%, so it cannot certify a 2% threshold on its own. The component estimate (k_true x main point + m x frozen point = 1.46 s, 0.90%) carries P3. For a future sub-percent claim, measure the checks directly (time the m certifying calls inside one run) rather than by subtraction across runs.

## 2026-11-03 - T4 review (recensor-verificatio) folded in

No H; the bound and the draws survived an independent oracle. Two behaviours changed or got documented:

- **DISCORDIA on a returning rejected candidate (M2).** The acceptance branch returned before the discors comparison, so a candidate the certifier had rejected at stage a was ACCEPTED when it came back at stage b and passed. Now any acceptance of a candidate equal to the last rejected one is DISCORDIA (same as the limit rule). Remembering only the LAST rejected candidate is enough: if X is the candidate at stages a and c, it is also the candidate at every stage between (X = P mod M_c implies X = P mod M_b, and |X| < M_a/2 < M_b/2), and a stage whose self-check fails does not overwrite discors.
- **Promotion inherits the prefix residues (M1).** Residues at primes 1..k come from Q itself, so a deliberately forged Q (Q + p_(k+1)...p_(k_max) p_ADDITUS on one coefficient, inside the limes) is promoted. Documented ("AUCTORITAS HEREDITARIA" in the header), not fixed: only recomputing the prefix closes it, and that is the full proved run. The `promotio` test pins the behaviour so a future change has to update the docs.

Non-obvious: **step 1 (reject candidates >= 2^limes) never fires with an honest main evaluator.** For |P| < 2^limes, either 2^limes <= M_k/2 and Q_k = P exactly, or M_k/2 < 2^limes and |Q_k| < M_k/2 < 2^limes. It only matters when the main evaluator is wrong, which is exactly when the bound needs |D| < 2^(limes+1). So the `limes in stadio` test lies at p1, with limes = floor(log2 p1) + floor(log2 p2) (so M_2/2 > 2^limes); a +1 lie did NOT reach the band (p1 - p2 = 12 makes the CRT unit for p1 only ~61 bits), so the test searches the smallest lie size delta whose stage-2 candidate exceeds 2^limes, and asserts that as a precondition.

Plants: build/verificatio/plantae_t4.py, 11/11 red as registered; T1's 12 rerun red. Details in the plan's "Review (T4)".
