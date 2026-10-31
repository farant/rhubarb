# temperley worklog

## 2026-10-30 - T1 (project-specs/temperley-lieb-plan.md v2)

Promoted from knotapel D136's evaluator (enum_rec, build_module,
find_state). Two changes forced by the library rules, both from the
design review:

- **No qsort, no global.** C89 qsort has no context pointer; the demo
  sorted with a global `sort_module`. The enumeration (open, close,
  defect at each point) already emits states in lexicographic order of
  the string form, ( < ) < |, so sorting is unnecessary *provided the
  key follows that order*.
- **Key with point 0 most significant** (review X1). The demo's key put
  point 0 in the low bits with defect = 0; that key is not monotone in
  enumeration order in 49 modules for n <= 16, and only the qsort hid
  it. Base 4, point 0 high, ( = 0, ) = 1, | = 2 is strictly increasing
  in every module up to 16 (max 2,863,311,530 < 2^32). A
  negative-size-array typedef pins 2 * TEMPERLEY_FILA_MAXIMA <= 32.

The key sees only the string form, not the pairing: a crossing pairing
(0-2, 1-3) prints "(())" like the nested one. temperley_status_quaerere
therefore compares the stored pairing row exactly after the key lookup.

Braid relations are not tested here (they need coefficients); they
follow from the TL relations and are covered in laqueus (T2b).

## 2026-10-30 - T1 plants

Harness `build/temperley/plantae_t1.py` (not committed; literal
(old, new) triples registered in the script before the run, each with
the credo sections it must turn red; a run without a credo summary or
verdict file counts as inconclusive, never red). All eight UT DICTUM:

| plant | red sections |
| --- | --- |
| 1 case (c) gives 0 | oraculum actionis, exempla, relationes |
| 2 every code NIHIL | oraculum actionis, exempla (relationes GREEN: e_i = 0 satisfies them) |
| 3 enumeration tries defect first | all but refutationes |
| 4 enclosed defects admitted | dimensiones, enumeratio, structura, oraculum, exempla, quaerere |
| 5 key point 0 least significant (X1) | all eight others |
| 6 quaerere skips exact pairing comparison | quaerere |
| 7 actio writes imago for NIHIL | oraculum actionis, exempla |
| 8 refusal i = fila off by one | refutationes |

Plant 1's first anchor matched twice: `silva.planta` matches
whitespace-tolerantly, so case (b)'s and case (c)'s `genus = UNUM` lines
(different alignment) were the same anchor. Two-line anchor fixed it.
