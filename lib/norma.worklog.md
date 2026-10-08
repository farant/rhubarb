# norma worklog

## 2026-10-08 - builders, view, judge (norma-plan-2 N2)

Header approved by Fran (7c635bb5); spec project-specs/norma-spec.md;
rules decided in project-specs/norma-plan-2.md ("Rules decided").

**Judge rules (as built)**
- Paths: `$`, `.clavis` for `[A-Za-z_][A-Za-z0-9_]*`, otherwise
  `["..."]` with JSON escaping (`$["a.b"]`, `$["q\""]`, `$["clāvis"]`);
  `[i]` for array elements. `chorda_aedificator_appendere_evasus_json`
  escapes WITHOUT adding quotes (pinned by the bracket-path tests).
- Where an issue points: DEEST -> the missing field's path; EXTRA -> the
  extra key's path; DISCRIMEN / VARIATIO -> the TAG's path; GENUS, bounds,
  LONGITUDO, ELECTIO, FORMA -> the value's path; LIMES / SCHEMA_PRAVA -> `$`.
- INTEGER accepts only JSON integers (7.5 and 7.0-as-float are GENUS);
  NUMERUS accepts both. Text length = runes (utf8_numerare_runas).
- NOTANDUM: undeclared key / unknown tag -> a NOTE, value stays valid;
  siblings still judged (an unknown first block + an invalid second block
  = one note + one issue).
- Cap: 256 issues, then one NORMA_CAUSA_LIMES at `$` and judging stops
  (a 1000-element wrong-typed array yields 257 records, not 1000).
  Notes stop silently at 256.
- Builder misuse marks the node (first error kept); the judge walks the
  schema first (`_pravitas`, recursing into array elements, fields and
  variants) and reports SCHEMA_PRAVA at `$` without judging anything.
- Known formats judged: date-time (fasti_ex_iso), uuid (8-4-4-4-12 hex),
  email (one '@', a '.' after it, not last), uri (scheme ':' something);
  unknown format names are generator hints only.

**Proof**: probatio_norma 168/168 first run. Plants 6/6 red: NOTANDUM
judged as CLAUSUM, cap removed, every key "simple" (bracket paths), runes
counted as bytes, implicit tag not skipped in variants, pravitas not
recursing into array elements.

## 2026-10-08 - JSON Schema export (norma-plan-2 N3)

`norma_json_schema` -> a JsonValor that drops into `VatesInstrumentum.schema`.
Deterministic key order (pinned by 10 goldens):

| node | keys, in order |
|---|---|
| scalar | type, description, minimum, maximum, minLength, maxLength, enum, format |
| array | type, description, items, minItems, maxItems |
| object | type, description, properties, required, additionalProperties (false only when CLAUSUM) |
| discriminated union | description, oneOf - each variant an object whose properties START with the tag `{"const": value}` and whose required starts with the tag; aut_nullum adds `{"type":"null"}` |
| liberum | `{}` (+ description) |

aut_nullum -> `"type": [X, "null"]`; integer bounds as JSON integers,
float bounds as JSON floats (`0.5`, `2.5` - the writer's shortest form).
No `$schema` key (the Anthropic input_schema does not want one).
Plants 3/3 red: CLAUSUM not exported, tag missing from `required`,
aut_nullum ignored.

## 2026-10-08 - norma_gignere + the mutual oracle (norma-plan-2 N4)

**Generator decisions (as built)**
- Per-path seeding: each node's stream = `sors_derivare(radix, key)`,
  key = the first 8 bytes of SHA-256(path). A field's PRESENCE draws from
  its own stream (`<path>?`), so adding an optional field never changes
  whether later fields appear. Path helpers are a copy of norma.c's (the
  oracle checks they agree byte for byte).
- Defaults: integers/numbers 0..1000, text 3..12 runes, arrays 1..3
  (TYPICA) / 0 or 3 (FINES), dates 1970..2100. A known format wins over
  length bounds; TYPICA text is real Latin cut/cycled to length, FINES
  text comes from fictio's difficult drawer.
- INVALIDA: TYPICA value, then ONE mutation chosen by seed among the
  violable constraints present (wrong type; remove required; extra key in
  CLAUSUM; min-1/max+1; length min-1/max+1 only for unformatted, unlisted
  text and arrays; enum -> "extra_electionem"; format -> "non forma"
  (each only if it stays within the node's length bounds); unknown tag;
  tag removed). Null-allowed, liberum and callback nodes are not mutated.
  Nothing violable -> valor NIHIL.

**The oracle** (probatio_norma_gignere, 727 assertions): 300 seeds x 2
schemas (one shaped like an Anthropic response), every value WRITTEN and
RE-READ before judging; TYPICA and FINES must validate; INVALIDA must
yield exactly one issue at via_fracta with causa_fracta; same seed ->
same bytes; adding a field leaves siblings unchanged; a callback that
returns the wrong type is caught; and COVERAGE is asserted: all ten
violable causes must be exercised (measured first: simplex GENUS 100
DEEST 77 EXTRA 11 MIN 37 MAX 24 LONG 18 ELECTIO 3 FORMA 30; responsum adds
VARIATIO 20 DISCRIMEN 8). A green that could mean "never tried" is not
accepted.

**Plants 4/4 red**, each named by the oracle: sequential (counter)
seeding -> determinism + stability (103); length mutations on a uuid
field -> "$.codex longitudo, vitia 2" (the cascade the rules exclude);
float maximum + 1e-9 in FINES -> "$.ratio supra maximum"; null as the
GENUS mutation of a nullable node -> "vitia 0".

**AUDIENDA resolved**: float boundaries emitted exactly survive
json_scribere -> json_legere and are accepted (300 seeds). The path-hash
choice (SHA-256 prefix) is now fixed: changing it changes every
generated value.
