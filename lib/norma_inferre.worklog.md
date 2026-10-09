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

## 2026-10-09 - discriminator detection (norma-spec-3 B1.2)

- Object positions keep CANDIDATES born from the first instance's string
  keys; a candidate dies when its key is absent, not a string, or exceeds
  discrimen_maximum distinct values. Each live candidate keeps one
  sub-shape per tag value (a PARTITION). A partition shape must not keep
  candidates of its own object level - it would re-partition itself
  forever (`partitio` flag); its nested positions do keep theirs, so
  discriminators inside variants are recovered.
- Merge: a candidate survives iff alive on both sides (empty dest copies
  the source's); value sets unioned under the cap. Checked by a merge-law
  case where the tag key dies only in B, both merge orders.
- Selection (§IV): gain = sum of required keys per partition - partitions
  x required keys overall; needs gain > 0 and differing key sets; max
  gain wins; ties prefer `type`, then bytewise order.
- **Evidence threshold (spec amended):** only values seen >= II times
  count toward gain and key-set difference, and >= II such values are
  needed. Found by B1.1's own test: two samples with an id-like string
  field (a new value each) made one-sample partitions where every key
  looks "required" -> a false discriminator. A variant seen once still
  appears in the draft (as its own variatio); it just cannot be the
  evidence for the split.
- Variants are drafted from their partition WITHOUT the tag field (the
  core declares it implicitly); values sorted bytewise.
- Law III (recovery): CCC values (TYPICA/FINES alternating) from a
  response schema (discriminator inside content[]) and a top-level union
  (nullable field, optional field, array) -> identical skeleton (genus,
  fields, required-ness, discriminator key + variants, nullable). The
  generator omits optional fields and emits nulls often enough (AUDIENDA
  answered). Whole suite < 0.4 s.
- Plants (all red): discriminator never chosen; evidence threshold off;
  merge keeping a candidate dead on one side; tag field kept in variants.

## 2026-10-09 - map-like flag, opt-in enums/bounds, bin/norma inferre (B1.3)

- Map-like position: > XXXII distinct keys, none present in more than
  half the object instances -> `apertum` objectum with no fields and the
  descriptio "inferentia: positio tabulae similis (N claves distinctae)";
  checked before discriminator selection. NOTANDUM soundness holds (an
  open object raises no notes).
- Opt-in only (InferentiaOptiones.electio / .fines), both needing >=
  exempla_minima samples: enums from the distinct set when not over the
  cap and <= electio_maxima, sorted bytewise; bounds from observed
  min/max (numerus takes the min/max over integers AND floats). Plant (b)
  - enums without opt-in - was caught by the "values never leak" check
  (SECRETUM appeared), which is exactly that check's purpose.
- `bin/norma inferre [-titulus T] [-status N] [-electio] [-fines]
  [-sine_formis] [-clausum] <via>...`: a via is a herbarium pile when
  `herbarium_enumerare` finds specimens there (filtered by status,
  default 200), else read as one .json file; non-JSON skipped and
  counted; "N exempla, M omissa" on stderr; exit 2 when nothing usable.
- Smoke on the committed vates specimens: 4 samples, a sound draft that
  `bin/norma c` and canon accept. It also shows the evidence rule on real
  data: one `server_tool_use` block among several `text` blocks gives no
  second repeated value, so `content[]` stays one object with optional
  fields - the reviewer sees it; B2's evidence report will say why.
