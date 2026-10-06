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
