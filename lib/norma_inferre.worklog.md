# norma_inferre worklog

## 2026-10-09 - the core: shapes, merge, rules (norma-spec-3 B1.1)

- One SHAPE per position (root, object field, array element): counts
  per JsonGenus, presence per key, string rune lengths + distinct values
  (capped at max(electio_maxima, discrimen_maximum)+1, then a 'superati'
  flag - order-independent: set semantics), format survivors as a bitmask
  checked through the PUBLIC judge (`norma_forma(norma_textus(p), f)`,
  one judge per format built at creation), integer and float min/max kept
  apart, array element shape + length range.
- Draft rules: one genus -> it; INTEGER+FLUITANS -> numerus; +NULLUM ->
  aut_nullum; NULLUM alone -> nullum; other mixes -> liberum; required =
  presence == object instances; objects NOTANDUM by default; arrays never
  seen non-empty -> tabulatum(liberum); a format only when >= exempla_minima
  strings and every one passed.
- Field order in the draft is BYTEWISE SORTED, not order of first
  appearance: otherwise a shuffled corpus (law II) gives a different
  export. Reviewers lose API order; determinism wins.
- `coniungere` builds a fresh inference and merges both sides into it -
  neither input changes (asserted).
- Known looseness inherited from the core: the judge's `uri` check
  accepts any `letters:` prefix ("Error: x" passes); `date-time` is
  `fasti_ex_iso`. With exempla_minima V and every-sample-passes, a false
  format is possible but rare - the draft is reviewed.
- Plants (all red): required as "present in any"; null not folded into
  aut_nullum; merge assigning presence instead of summing; a failed
  format bit never cleared.
