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
