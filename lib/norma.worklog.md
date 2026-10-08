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
