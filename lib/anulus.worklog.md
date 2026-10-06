# anulus.worklog.md

## 2026-10-06 — natus

`anulus` (Latin "ring" — literally): a ring descriptor, a table of
function pointers over opaque elements of a given size, so algebra
libraries (matrix first; persistent homology over Z/2, Q[t] later) are
written once over any house ring. Fran chose this over three concrete
matrix types: Bareiss, rank and kernel are one algorithm over any
integral domain with exact division.

- Three rings: `ANULUS_INTEGRORUM` (Z, magnus), `ANULUS_RATIONALIUM` (Q,
  fractio, `corpus` VERUM), `ANULUS_POLYNOMIORUM` (Z[t,t^-1], polynomium,
  text letter 't').
- Every arithmetic entry returns b32 (polynomium's exponent bound,
  inexact exact-division); exitus untouched on refusal. `unum` takes a
  piscina (the polynomial 1 allocates).
- `transcribe` = DEEP copy, the hook for scratch piscinae. It made
  `fractio_transcribe` and `polynomium_transcribe` public (additive).
- Size field is `mensura`: `magnitudo` is the latina.h macro for
  sizeof (examen refused the first draft). Element macros are
  `VALOR_Z/Q/P` — `_Z` was a reserved identifier (underscore + capital).
- Wrappers are thin and pass structs by value, so output aliasing an
  input (`adde(x, y, p, x)`) is safe — matrix relies on it.
- Tested directly in probatio_matrix ("ANULI"): exact division refusals
  (Z remainder, Z by 0, Q by 0, Z[t] non-divisor), corpus flags, unit,
  text; transcribe is deep (limbs/arrays differ from the source, values
  survive destroying the source piscina).

## 2026-10-06 — `parvum` (review matrix-II)

New entry `parvum(a)`: the element lives entirely inside its struct (no
external memory) — Z: fits s64 (`magnus_ad_s64`); Q: numerator AND
denominator fit; Z[t]: only the zero polynomial. matrix uses it to choose
its scratch-free small path (count alone wasted 204 KB on a 5×5 of
1000-digit entries). Placed after `est_nullum`; the three positional
initializers updated. Tested per ring, including 1/10^30 (small
numerator, big denominator).

## 2026-10-06 — Euclidean hooks (matrix v2)

`divisor_communis(a, b, piscina, g, u, v)` (g = u a + v b, normalized; Z:
magnus_divisor_communis_testatus, g ≥ 0) and `divide_cum_residuo(a, b,
piscina, q, r)` (Z: magnus_divide, Euclidean remainder). Appended at the
END of the struct; NIHIL for Q and Z[t,t^-1] — algorithms needing them
return FALSUM. Q[t] (when it exists) is the next ring to fill them.

## 2026-10-06 — `compara_normam` (review matrix-III)

Euclidean norm comparison (Z: |a| vs |b|; NIHIL elsewhere), appended after
`divide_cum_residuo`. matrix's Hermite/Smith now pick the smallest-norm
pivot and reduce the others modulo it instead of Bézout-combining rows —
the Bézout version blew up intermediates (Hermite 36×36: 22.6 s → 11 ms).

## 2026-10-06 — descriptor as first argument; Z/n ring

Fran approved (with congruentia's API): parameterized rings need context,
so EVERY ring function now takes the descriptor first —
`anulus->adde(anulus, a, b, piscina, exitus)` — and `Anulus` has a final
field `contextus` (NIHIL for Z, Q, Z[t]). Done now while matrix is the
only consumer; #3 on the map (Q(√d), Z[ζ_n]) needs the same.

- Struct now tagged (`nomen structura Anulus Anulus;` + `structura Anulus
  { … }`, the house pattern for self-reference).
- Mechanical: wrappers gained `constans Anulus* anulus` and
  `(vacuum)anulus;` (inserted AFTER local declarations — C89); call
  sites rewritten `X->f(` → `X->f(X, ` by one regex (71 in matrix.c, 28
  in the test); suite green before any new code (198).
- `anulus_residuorum(n, piscina)` (n ≥ 2, else NIHIL): copies a template
  descriptor into the piscina, `contextus` → the modulus, `corpus` =
  `congruentia_est_primus(n)`, title "Z/n" with the digits. Elements are
  `i32` residues (always `parvum`). `divide_exacte` multiplies by the
  inverse and REFUSES a non-unit divisor — over composite n, Bareiss can
  therefore return FALSUM (a pivot that is a zero divisor), never a wrong
  value. Euclidean hooks NIHIL (Z/p is a field; Hermite/Smith refuse).
- Tests (probatio_matrix "ANULUS RESIDUORUM"): det over Z/p = det over Z
  reduced mod p for 60 random matrices up to 8×8 (p = 7 and 4294967291,
  small and ~10^21 entries, deficient ones), rank and kernel mod p (A·K =
  0, columns = c − rank), rank drop [1,2;3,4] mod 2 = 1 / mod 3 = 2, the
  Z/6 refusal, text "[8, -1; 14, 3]" → "[1, 6; 0, 3]" over Z/7. Plants
  red: division without the inverse, unit 0, corpus always true, text
  not reduced, modulus fixed at 7.
- tools/venenum_probare.sh: the matrix suite now also links
  lib/congruentia.c (the gate went red on a link error first — its
  source lists are by hand).
