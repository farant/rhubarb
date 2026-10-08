# norma spec - schemas that judge JSON and dream it up

Born 2026-10-08 (Fran, during vates-plan-2: "a general purpose runtime
json schema validation library ... kind of like zod or ArkType ... a
schema definition that can double as a mock data generator kind of like
what laravel uses"). Ledger desideratum …EN4B. Brainstorm decisions,
all Fran's: C builder API is the source of truth; validate now, struct
binding next; three extra-key modes, closed by default; formats +
per-field callbacks for generation; discriminated union + nullable in v1;
core + generator split, and the fake-value primitives generic beyond JSON.

*The question: how does house C state the shape of a JSON value once,
and from that one statement judge real values, produce realistic and
hostile fake ones, and tell an API what it accepts?*

## 0. Data (2026-10-08)

- Existing house schemas, neither for JSON values: `nuntium_schema`
  (protobuf-like wire messages, numbered tags; `include/nuntium_schema.h`)
  and `canon` (STML dialects: closed vocabulary, child cardinality,
  attribute genera, "no expression language, ever"; `include/canon.h`).
  norma follows canon's stance: closed by default, NAMED issues.
- JSON values: `include/json.h` (`JsonValor`, s64 integers, f64 floats,
  keys in insertion order -> deterministic `json_scribere`).
- Seeded randomness: `include/sors.h` - `Sors {status, incrementum}`,
  `sors_seminare(Sors*, i64 semen, i64 series)`,
  `sors_derivare(const Sors*, i64 clavis)` (a child stream, PURE in the
  parent), `sors_intra`, `sors_inter`, `sors_casu`, `sors_miscere`.
- UTF-8: `utf8_numerare_runas` (`include/utf8.h:66`) counts code points.
- Dates: `fasti_ex_iso` / `fasti_ad_iso` (`include/fasti.h:435-448`).
- First consumers already in quarta: vates' hand-kept key lists
  (`_claves_summae`, `_claves_textus`, `_claves_petiti` in `lib/vates.c`;
  T7b had to extend them after the live API surprised us), the tool
  `input_schema` vates sends (`VatesInstrumentum.schema`, a JsonValor),
  herbarium's specimen file format.
- No Latin Vulgate in the house (checked: no `vulgat` in include/ or lib/;
  `lib/biblia_dr.c` is Douay-Rheims English). Candidate sources:
  `../gutenberg-mirror` (75 Latin texts; memory: never a COMMITTED
  corpus) and `../lapide` (`_lt` = Latin, quotes the Vulgate).

## I. Framing

Three libraries, each independently testable:

| library | owns | depends on |
|---|---|---|
| `fictio` | fake VALUES, no JSON: names, emails, uuids, dates, real Latin text, bounded numbers and strings, a drawer of difficult strings | sors, chorda, piscina |
| `norma` | the schema model + builders, the JUDGE (issues + notes with paths), JSON Schema EXPORT; declares the generator-callback TYPE only | json, xar, chorda, utf8, fasti |
| `norma_gignere` | walks a schema -> JsonValor in TYPICA / FINES / INVALIDA, using fictio and the per-field callbacks | norma, fictio, json, sors |

Who links what: production validators (vates) -> `norma` only; tests and
mock tools -> `norma_gignere` (pulls the other two); anyone wanting a fake
name or date -> `fictio` only.

Principles:
1. **One statement, three faces**: the same `Norma*` judges, generates
   and exports. No second description of a shape anywhere.
2. **Named, complete verdicts**: every issue with its path and cause, not
   the first one; a cap (256) so a hostile value cannot produce a hostile
   report; the cap itself is recorded.
3. **Validator and generator check each other** (§V): independent code,
   one oracle.
4. **Determinism**: same schema + mode + seed -> same bytes; and stable
   under schema edits (per-path seeding, §IV).

## II. `norma` - the model and the builders

`Norma` is opaque (v2 adds struct binding as NEW functions, no breaking
change). Nodes live in the piscina given to the builders; a node may be
shared by several parents (a DAG). Recursion is not in v1.

```c
/* leaves */
Norma* norma_quodvis  (Piscina*);
Norma* norma_nullum   (Piscina*);
Norma* norma_boolean  (Piscina*);
Norma* norma_integer  (Piscina*);
Norma* norma_numerus  (Piscina*);             /* integer or float */
Norma* norma_textus   (Piscina*);
/* containers */
Norma* norma_tabulatum (Piscina*, Norma* elementum);
Norma* norma_objectum  (Piscina*);
Norma* norma_campus    (Norma* objectum, const char* titulus, Norma* valor, b32 requiritur);
Norma* norma_modus     (Norma* objectum_aut_discrimen, NormaModus modus);
Norma* norma_discrimen (Piscina*, const char* clavis);
Norma* norma_variatio  (Norma* discrimen, const char* valor, Norma* objectum);
/* modifiers (return their node, so they nest) */
Norma* norma_nullabile  (Norma*);
Norma* norma_intra      (Norma*, s64 minimum, s64 maximum);
Norma* norma_intra_f    (Norma*, f64 minimum, f64 maximum);
Norma* norma_longitudo  (Norma*, i32 minimum, i32 maximum);
Norma* norma_electio    (Norma*, const char* const* licita);  /* NIHIL-terminated */
Norma* norma_forma      (Norma*, const char* forma);
Norma* norma_descriptio (Norma*, const char* textus);
Norma* norma_gignens    (Norma*, NormaGignens functio, void* datum);

typedef JsonValor* (*NormaGignens)(Sors* sors, Piscina* piscina, void* datum);
```

Rules:
- **Modes** (`NormaModus`, per object or discriminated union): CLAUSUM
  (default) - an undeclared key is an issue; APERTUM - allowed silently;
  NOTANDUM - allowed, reported as a NOTE (the value stays valid). NOTANDUM
  is novelty detection: a declared API response schema reports every new
  field, not just the first.
- **Text length** counts Unicode code points (as JSON Schema's
  `minLength`), via `utf8_numerare_runas`; array length counts elements.
- **Discriminated union**: a variant does not declare its tag field (it is
  implicitly declared and checked); an unknown tag value is an issue under
  CLAUSUM, a note under NOTANDUM (value valid); a missing / non-text tag is
  always an issue.
- **Formats known to the judge**: `date-time` (`fasti_ex_iso`), `uuid`
  (8-4-4-4-12 hex), `email` (loose: exactly one `@`, non-empty sides, a
  dot in the domain), `uri` (scheme + `:`). An unknown format name is a
  generator hint only (judge silent).
- **Builder misuse** (`norma_campus` on a non-object, minimum > maximum,
  `norma_variatio` on a non-discrimen, a NIHIL child ...) marks the node
  with its first error; `norma_iudicare` reports it as `NORMA_SCHEMA_PRAVA`
  at `$` and judges nothing else. No crash, no stderr from a library.
- `nuntius` texts never echo the judged value (it may be huge or secret).

## III. `norma` - the judge and the export

```c
typedef enum { NORMA_GENUS, NORMA_DEEST, NORMA_EXTRA, NORMA_MINIMUM,
               NORMA_MAXIMUM, NORMA_LONGITUDO, NORMA_ELECTIO, NORMA_FORMA,
               NORMA_VARIATIO, NORMA_DISCRIMEN, NORMA_LIMES,
               NORMA_SCHEMA_PRAVA } NormaCausa;
typedef struct { chorda via; NormaCausa causa; chorda nuntius; } NormaVitium;
typedef struct { b32 validum; Xar* vitia; Xar* notae; } NormaIudicium;

NormaIudicium norma_iudicare (const Norma*, JsonValor* valor, Piscina*);
JsonValor*    norma_json_schema (const Norma*, Piscina*);
```

- **Paths**: `$`, `.clavis`, `[i]`; a key that is not `[A-Za-z_][A-Za-z0-9_]*`
  is written `["..."]` with JSON escaping. Notes use the same record
  (`causa` = NORMA_EXTRA or NORMA_VARIATIO).
- **Cap**: at 256 issues judging stops; the 257th record is NORMA_LIMES.
- **Export** (deterministic key order) - CLAUSUM `additionalProperties:
  false`; APERTUM/NOTANDUM omit it; discriminated union -> `oneOf` of
  variants, each with `properties.<tag>.const` and the tag in `required`;
  nullable -> `type: [X, "null"]`; `format`, `description`; integer
  `minimum`/`maximum`; text `minLength`/`maxLength`; array
  `minItems`/`maxItems`; enum -> `enum`. No `$schema` key (the Anthropic
  `input_schema` does not want one).

## IV. `fictio` and `norma_gignere`

```c
/* fictio: every function takes the caller's Sors */
chorda fictio_nomen          (Sors*, Piscina*);   /* praenomen + nomen gentile */
chorda fictio_email          (Sors*, Piscina*);   /* RFC 2606 domains only */
chorda fictio_uuid           (Sors*, Piscina*);   /* v4 shape */
chorda fictio_tempus         (Sors*, s64 ab, s64 ad, Piscina*);   /* ISO 8601 UTC */
chorda fictio_textus_latinus (Sors*, i32 sententiae, Piscina*);
chorda fictio_textus         (Sors*, i32 minimum, i32 maximum, const char* alphabetum, Piscina*);
s64    fictio_integer        (Sors*, s64 minimum, s64 maximum);
f64    fictio_numerus        (Sors*, f64 minimum, f64 maximum);
const char* fictio_eligere   (Sors*, const char* const* optiones);
chorda fictio_textus_difficilis (Sors*, Piscina*);

/* norma_gignere */
typedef enum { NORMA_TYPICA, NORMA_FINES, NORMA_INVALIDA } NormaModusGignendi;
typedef struct { JsonValor* valor; chorda via_fracta; NormaCausa causa_fracta; } NormaGenitum;
NormaGenitum norma_gignere (const Norma*, NormaModusGignendi, i64 semen, Piscina*);
```

- **Corpus** (`fictio_textus_latinus`): ~50 real Latin sentences copied
  verbatim with source citations into a static table in `fictio.c` (a few
  sentences of public-domain text, not a committed corpus). Never lorem
  ipsum.
- **Difficult drawer**: combining marks, a non-BMP letter, right-to-left
  text, quotes + backslashes, an escaped U+0000, whitespace-only, an emoji
  with a skin-tone modifier, a long string - each valid UTF-8.
- **Per-path seeding**: the walker derives each node's stream with
  `sors_derivare(parent, hash(path))`, so adding a field does not change
  its siblings' values (snapshots survive schema edits).
- **TYPICA**: required fields always, optional 1/2, nullable null 1/10,
  enum pick, union variant pick, numbers within bounds (default 0..1000),
  text by format else Latin words cut to the length bounds, arrays 1..3
  within bounds; a per-field callback replaces the default.
- **FINES**: every bounded node at an edge (lower or upper by seed): exact
  min/max, exact length limits, empty arrays where allowed, optional
  fields all present or all absent, nullable null, unformatted text from
  the difficult drawer within its bounds.
- **INVALIDA**: a TYPICA value, then exactly ONE violation chosen by seed
  among the violable constraints present (wrong type, missing required,
  extra key in a closed object, below min, above max, length out of
  range, outside enum, broken format, unknown tag, missing tag); reports
  `via_fracta` + `causa_fracta`. Nothing violable -> `valor` NIHIL with a
  named cause, never a fake violation.

## V. Testing - the mutual oracle

- `probatio_fictio`: same seed -> same bytes; bounds; RFC 2606 domains
  only; uuid shape; every difficult string valid UTF-8.
- `probatio_norma`: one test per `NormaCausa`; paths incl. `["a.b"]`;
  the cap; notes vs issues; unknown tag CLAUSUM vs NOTANDUM; builder misuse
  -> SCHEMA_PRAVA; export goldens per mapping row + determinism.
- `probatio_norma_gignere`: over a set of schemas (one Anthropic-response
  shaped) and hundreds of seeds: TYPICA and FINES -> zero issues;
  INVALIDA -> exactly one issue, at `via_fracta` with `causa_fracta`;
  same seed -> same bytes; a field added -> siblings unchanged. A per-field
  callback that produces invalid data fails the TYPICA round ("your
  factory makes bad data").
- Every behaviour born red by a plant that compiles (house rule).

## VI. Build order (quarta)

- **N0** `fictio.h`, `norma.h`, `norma_gignere.h` for Fran's review; no
  implementation before approval.
- **N1** fictio (+ corpus). **N2** norma builders + judge. **N3** export.
  **N4** norma_gignere + the mutual oracle.
- **N5** first consumer, inside vates, NO header change: a declared
  NOTANDUM schema of the Anthropic message replaces the hand-kept key
  lists; novelty = norma's notes, ALL of them (herbarium's causa lists
  every new field). Regression oracle already exists: T4's parse tests,
  T7b's live-shape test and the specimen sweep must stay green unchanged.

## VII. Not in this slice (pulled by consumers)

v2 struct binding (`norma_campus_ad(..., offsetof(S, f))` +
`norma_legere(n, valor, &s, p)`); JSON Schema IMPORT (when schemas arrive
from outside, e.g. MCP tools; or as a round-trip oracle); recursion
(forward references + `$defs` export); general `anyOf`/`oneOf`; regex
`pattern` (no house regex engine yet); vates validating tool inputs
(needs `Norma*` on `VatesInstrumentum` - a vates header change).

## VIII. Done means

Headers approved; the three suites green with every behaviour born red;
the mutual oracle holding over hundreds of seeds; N5 landed with vates'
existing tests unchanged and green, and herbarium reporting all new
fields in one causa.

## AUDIENDA

- **Corpus source**: Gutenberg mirror Latin texts vs `../lapide` `_lt`
  files - which has clean, citable Vulgate/Cicero sentences; licensing of
  the chosen edition's header to be checked (a few sentences of a PD text,
  cited, are believed fine).
- **`sors_derivare` key from a path**: needs a stable hash of the path
  string to an i64 (sigillum's first 8 bytes, or the house `friatio`) -
  pick at N4, document the choice (it fixes every generated value).
- **f64 bounds in FINES**: an exact float maximum round-trips through
  `json_scribere`'s shortest form? To check at N4 (the judge must accept
  the boundary the generator emits).
- **Email/uri judges are deliberately loose**; a stricter judge would
  reject values real systems accept. Revisit only on a real miss.
