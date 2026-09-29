# toml_arbor — TOML as a materia client, the substrate's seventh

*2026-09-28. Designed with Fran in one brainstorming session, after
briar v3. Seventh materia client after the C89 shim, css, md, oratio,
html and crusta; this spec copies crusta's as-built shape
(`project-specs/crusta-arbor-spec.md` §11) wherever the language
allows. Born of desideratum 01M3G4JVTSKCB2BRD4874EJTXF (Fran,
2026-09-26: "migrate the toml parser to materia ... it's not really
feature complete") and the lapide tester's feature-requests/013
(`toml` flattens `[tables]`).*

**What it is.** A parser for TOML 1.0.0 as a materia tree — total,
byte-exact through the emitter for every input, valid or not —
plus a **cooked view** (real tables, typed values) that consumers
query, located diagnostics for every error in a file, an STML
projection, a canon and a benchmark twin, proven against three
oracles: the public toml-test suite, Python's `tomllib`, and a wild
corpus of real files. It replaces `lib/toml.c` and `include/toml.h`;
the three tools that read TOML migrate to the new API.

**Why "toml".** Like css, md and html, the client keeps the format's
own name; the format is a proper noun, not a word to translate.

---

## 0. Decisions

| # | decision | rationale |
|---|---|---|
| T1 | **Full TOML 1.0.0**, judged by toml-test's 1.0.0 list — valid AND invalid cases | Fran, 2026-09-28 (option "full"). Rejecting bad input is half the spec; a library finished once never surprises a consumer later. |
| T2 | **New API over the cooked view; consumers migrate; `lib/toml.c` and `include/toml.h` retire** | Fran (option "new API"). No shim carries the old `s32` numbers or the flattening. Breaking for `.thistle` authors — briar changelog FRANGIT line. |
| T3 | **Hand-written lexer with two modes (key / value) and an iterative builder** — the crusta shape, not an LR table | TOML's lexing depends on position: `1979-05-27` is a date as a value and a bare key before `=`; `true = true` is legal. A mode decides the token; nothing is relabelled afterwards (crusta C6). |
| T4 | **Totality**: every byte lands in the tree; what the grammar cannot place is a `malum` node; the builder recovers at the next newline outside brackets | TOML is line-oriented, so recovery is cheap and exact. Consequence: **every** error in a file is reported, not the first only (the C89 lesson of lapide bugs/017, here for free). |
| T5 | **Semantic rules live in the cooking step, not the grammar** | Duplicate keys, table redefinition, extending a closed inline table, `[[x]]` over a static array, integer overflow: each a named diagnostic with a location, and the first definition as a related location. |
| T6 | **Lookup paths use TOML's own dotted-key syntax** (`a.b`, `site."google.com"`) | No second path language; a key containing a dot stays reachable. The path is parsed by the same lexer in key mode. |
| T7 | **Typed getters return VERUM only if the key is present AND of that type** | Today's getters return `0` / `""` for absent keys, which hid missing configuration. |
| T8 | **Tables keep insertion order; every cooked value points at its tree node** | Deterministic iteration; errors and future format-preserving edits reach the source bytes. |
| T9 | **Three oracles**: toml-test pinned (one fetch, asked first), `tomllib` differential (local), wild corpus byte law (local) | Like the shim's three oracles. Two need no network at all. |
| T10 | **Out of scope: writing TOML; TOML 1.1** | Named futures (§9). The byte-faithful tree is what makes format-preserving edits possible later. |
| T11 | **Wiring is part of the deliverable**: aedilis learns `toml/fontes`; briar's corpus exposes the new library | aedilis once lost materia's sources silently (T13a, silva migration); the tester reaches `toml` only through briar. |

---

## I. The census (2026-09-28)

**The library being replaced.** `include/toml.h` declares a subset:
basic and multiline basic strings, `s32` integers, arrays of strings,
booleans. `[section]` headers are *ignored* ("claves fiunt
top-level"), so two sections' keys collide silently. No floats,
datetimes, inline tables, dotted keys, arrays of tables, literal
strings, hex/octal/binary integers; no way to enumerate keys
(`toml_numerus_introituum` only).

**Consumers** (all read-only; `./silva/nexus.sh toml_legere`):

| file | use |
|---|---|
| `tools/capsula_generare.c:693` | `toml_legere` + `toml_capere_boolean` on keys built as `<prefix>_compress` |
| `tools/capsula_caudae_adiungere.c:578` | `toml_legere` + `toml_capere_tabulatum` / `_boolean` on `<prefix>_files`, `<prefix>_compress` |
| `tools/librarium_merge.c:974` | parses **LLM-written** TOML blocks extracted from fenced replies; case-variant key fallbacks (`Tags`/`tags`, `Catholic`/`catholic`) |
| `probationes/probatio_toml.c` | 16 calls, the library's own test |

**Files.** 12 tracked `.toml` files, the longest 17 lines, all flat
`key = value` (prefix convention `libri_files`, `libri_compress`); no
headers, dates, floats or inline tables anywhere. The tester's one
file (`../lapide-v2/instrumenta/externa/llama-server.toml`) is flat
too; their request is for configs with several sections.

**Oracle material on this machine.** Python 3.12.6 with `tomllib`
(standard library, TOML 1.0, strict). Real-world `.toml` files:
`~/.cargo` 1,587 · `/opt/homebrew` 219 · `~/Documents/projects` 403
(`find -name '*.toml' | wc -l`). toml-test is **not** on disk.

---

## II. Architecture and files

A client directory `toml/`, mirroring `crusta/`:

| module | job |
|---|---|
| `toml_lector` | lexer, two modes; every byte is a token (comments, whitespace, newlines included) |
| `toml_registrum` | the node vocabulary as a table (the definition is data, never a program) |
| `toml_arbor` | builder: tokens → materia tree; `emit(parse(x)) == x` for every x |
| `toml_coctum` | cooked view: tree → tables and typed values; the semantic rules (T5) |
| `toml_diagnostica` | syntax + semantic errors as `MateriaDiagnosticum`, rendered by `materia_pictor_scribere` |
| `toml_oraculum` | cooked view → toml-test tagged JSON |
| `toml_computus` | benchmark twin, arena columns pinned |
| `toml.h` | public API (§V) |
| STML projection, `toml.canon`, exempla, facies | as in css, md, crusta |

Tests in `toml/probationes/`, runner `toml/compile_probationes.sh`,
gate `toml` registered in `pythonica/silva.py` PORTAE (a runner
outside the gate table is dead — materia memory, B10).

---

## III. The lexer (`toml_lector`)

**Key mode** — at line start, inside `[ ]` / `[[ ]]` headers, and in
inline tables before `=`: bare keys (`A-Za-z0-9_-`), basic and
literal quoted keys, `.`, `=`, `]`, `]]`.

**Value mode** — after `=` and inside arrays:
- the four string forms: `"…"`, `"""…"""`, `'…'`, `'''…'''`
  (escapes decoded in cooking, not lexing; the token keeps the raw
  bytes);
- `true` / `false`;
- `[` (array), `{` (inline table), `,`;
- **one greedy number-like token, classified afterwards**: decimal,
  hex (`0x`), octal (`0o`), binary (`0b`) integers with `_`; floats
  with fraction/exponent, `inf`, `nan`, signs; the four datetime
  kinds. Trap named up front: `1979-05-27 07:32:00` — a single space
  joins date and time into one token; `1979-05-27 # comment` does
  not.

**Everywhere**: comment `#` to end of line; whitespace (space, tab);
newline (LF, CRLF — the CR travels with the materia `cr` attribute,
B6); the input is UTF-8-validated with the house `utf8` library, and
control characters are refused where the spec refuses them (toml-test
has many such invalid cases).

Newline is a token that **ends a key/value pair** outside brackets
and is trivia inside arrays (inline tables stay single-line in 1.0).
The mode, not a later pass, decides.

---

## IV. The tree (`toml_registrum`, `toml_arbor`)

Node vocabulary (sketch — final Latin names in the plan, checked
against the glossary and latina.h):

- `documentum`; `caput_tabulae` (`[a.b]`); `caput_seriei_tabularum`
  (`[[a.b]]`); `par` (key `=` value);
- `clavis` (dotted; children are segments: bare / basic-quoted /
  literal-quoted);
- values: the four string genera, `integer`, `fluitans`, `boolean`,
  `tempus`, `series` (array: elements, commas, newlines, comments
  kept), `tabula_inlinea`;
- trivia: whitespace, newline, comment;
- `malum`: a span the grammar cannot place (T4).

Laws, gated: **byte law** (`emit(parse(x)) == x` for every file,
valid or invalid); **totality** (no byte outside a token, no token
outside a node); **STML round trip** (write, read, write equal — two
cycles, per the stml text-semantics lesson); **totalitas** (every
genus appears in the fixtures).

---

## V. The cooked view and the API (`toml_coctum`, `toml.h`)

`TomlValor` has a kind: table, array, string (decoded), **`s64`**
integer, `f64` float (inf/nan included; parsed with `strtod` on an
underscore-free copy), boolean, or datetime `TomlTempus` — one of
four kinds (offset date-time, local date-time, local date, local
time) with year..second, fractional seconds, and offset minutes.
Tables keep insertion order (lookup through `tabula_dispersa`, order
through `xar`). Every value keeps its tree node (T8).

API sketch (names polished in the plan):

```c
TomlDocumentum*  toml_legere (chorda textus, constans character* via,
                              Piscina* piscina);          /* numquam NIHIL */
b32              toml_successus (constans TomlDocumentum* doc);
Xar*             toml_diagnostica (constans TomlDocumentum* doc);
constans TomlValor* toml_radix (constans TomlDocumentum* doc);
constans TomlValor* toml_quaerere (constans TomlDocumentum* doc,
                                   constans character* via_clavium);
b32 toml_chorda   (doc, via_clavium, chorda* exitus);   /* T7 */
b32 toml_integer  (doc, via_clavium, s64* exitus);
b32 toml_fluitans (doc, via_clavium, f64* exitus);
b32 toml_boolean  (doc, via_clavium, b32* exitus);
b32 toml_tempus   (doc, via_clavium, TomlTempus* exitus);
i32                 toml_tabulae_numerus (constans TomlValor* tabula);
chorda              toml_tabulae_clavis  (constans TomlValor* tabula, i32 i);
constans TomlValor* toml_tabulae_valor   (constans TomlValor* tabula, i32 i);
i32                 toml_seriei_numerus  (constans TomlValor* series);
constans TomlValor* toml_seriei_elementum (constans TomlValor* series, i32 i);
```

A document with errors still cooks what it can; `toml_successus`
reports whether it is valid. This satisfies the tester's
feature-requests/013 minimum: dotted lookup, enumeration of tables and
keys, duplicate keys refused with a named error.

---

## VI. Diagnostics

Syntax errors come from the builder ("expected …", the rejected text
in the author's spelling); semantic errors from cooking, each with the
first definition as a related location ("`versio` iam definita" —
second caret at the original). All are `MateriaDiagnosticum`, derived
through `materia_diagnostica` and rendered by `materia_pictor_scribere`
(`file:line:column`, excerpt, caret) — the renderer crusta uses. Every
error in the file is reported (T4).

---

## VII. Oracles and gates

1. **toml-test, pinned.** Fetched once at a tagged release (asked
   before fetching; MIT-licensed data), stored frozen under
   `toml/probationes/fixa/toml-test/` with its licence and a
   provenance note (tag, commit, date). The gate runs the **1.0.0
   file list**: each valid case's cooked view, serialized to tagged
   JSON, equals the expected JSON (floats by value, datetimes
   normalized); each invalid case is rejected. Pass counts pinned,
   only rising; target 100%; any exclusion named with its cause.
2. **`tomllib` differential.** A small Python harness renders
   `tomllib`'s result as the same tagged JSON. Over toml-test and the
   wild corpus, both parsers must agree on accept/reject and on the
   cooked values; each disagreement is named (our bug, or a
   documented divergence).
3. **Wild corpus.** A snapshot of the local real-world files (frozen
   input, not the live directories); byte law 100% (hard gate),
   accept/reject agreement with `tomllib`.
4. **Per-client suites**: registry, lexer, tree, cooked, STML,
   canon, totalitas, computus — gate `toml`, owed automatically by
   paths under `toml/` (portae debitae).

Gates are born red by a plant that compiles.

---

## VIII. Migration and wiring

- The three tools and `probatio_toml` move to the new API. Their
  flat prefixed keys (`libri_files`) keep working as flat keys; moving
  them to `[libri]` tables is optional and not in this plan.
- `lib/toml.c` and `include/toml.h` are deleted once nothing includes
  them (`./silva/nexus.sh` shows zero uses).
- aedilis learns the `toml/fontes` root; the aedilis gate proves the
  tools' closures now contain it (T11).
- briar's corpus exposes the new library (`-bibliothecae` lists it,
  `-bibliotheca toml` shows the new header); changelog line marked
  FRANGIT with the replacement calls, and the tester's
  feature-requests/013 cited.
- **Risk to measure, not assume:** `librarium_merge` reads
  LLM-written TOML and the old parser was lax. The migration step
  checks the strict parser against whatever librarium data exists;
  the tool already skips a block that fails to parse.

---

## IX. Named futures (nothing built)

- **Writing TOML**: serializing a cooked view, and format-preserving
  edits through the tree (change one value, keep every comment).
- **TOML 1.1**: newlines and trailing commas in inline tables, `\e`
  and `\xHH` escapes, optional seconds in times. toml-test ships a
  separate 1.1.0 list; a later dialect flag, not a rewrite.
- **Consumers**: sectioned configs for capsula tools; briar project
  settings; lapide's pins in one file.

---

## X. As built (2026-09-28, plan closed — governs where the body differs)

Built in one day, Q1–Q14 (commits 8a94f763 … 77382abf and the closure).
Orientation: `toml/CLAUDE.md`. Deltas from the body above:

- **Diagnostic owners are three, not one step.** Scalars
  (`toml_scalaria_iudicare`, every token incl. trivia, at the bad byte),
  structure (`toml_coctum`, 8 codes, first definition related), syntax
  (derived from the declaration — and SKIPPED on a clean parse, since
  materia's derivation is quadratic in depth; equivalence asserted per
  fuzz case). §VI named one derivation pass.
- **Table semantics are tomllib's**, carried on the objects: `modus`
  IMPLICITA / EXPLICITA / PUNCTATA / INLINEA plus `sectio` for dotted
  tables. Inline newlines and trailing commas are named as TOML 1.1.
- **Two verdicts beyond the letter of 1.0**, pinned: second 60 allowed
  (RFC 3339; tomllib refuses — no real file has one); fractional
  seconds truncated at 9 digits (tomllib: 6) — the one wild divergence,
  "microsecunda", named by cause.
- **The empty key `""`** lives outside the hash index
  (`tabula_dispersa` refuses length 0).
- **`librarium_merge` stays lenient through a named tool-local helper**
  (`_capere_planum`, the old flattening order); the library is strict.
  Proven on 3,311 real LLM replies: equal per element except 19 bugs of
  the old library fixed.
- **The corpus widening came in Q12, not Q13**: retiring `lib/toml.c`
  would have broken every silex-generated project (they vendor
  `capsula_generare`). Client roots are one list in two homes
  (`SILEX_RADICES_CLIENTIUM`, `RADICES_CLIENTIUM`) with a drift guard.
- **API additions**: `toml_chordae` (array of strings, Q12);
  `toml_tabulae_filius` shared by cooking, oracle and API.
- **Twin golden** is three frozen toml-test files, not the largest wild
  file (the wild corpus is manifest-only).

Numbers: toml-test 1.0.0 205/205 valid equal, 474/474 invalid rejected,
no exclusions; tomllib 679/679 and wild 1,794 with zero disagreements;
byte law on toml-test 679, house 12, wild 1,794; totality 6,136 cases,
linear at depth 100 000; 13 suites, 14,267 assertions.

**AUDIENDA answered:**
- toml-test tag v2.2.0 (commit ce08da1d); 1.0.0 list 205 valid / 474
  invalid (a web summary had said 256/512).
- Wild corpus: 1,795 unique files, 3.27 MB — kept as a sha256 manifest
  only (Fran: no third-party files in the repo); its golden is
  regenerated, never committed.
- `strtod`: exact on every toml-test float (Q8 compares by value;
  205/205) and on every wild file (Q9).
- `librarium_merge`: 1 of 3,311 real replies rejected by the strict
  parser (`[Summary]` + a bare string); 21 use headers — hence the named
  lenient helper.
- Substrate changes: **none** (no commit in the arc touches
  `materia/fontes` or `materia/grammatica`). materia's own depth limits
  (emission and STML at 100 000, quadratic derivation) are pinned
  `RUIT_CUM` on park …FAD8, not changed.

---

## AUDIENDA (as written before the build — answered in §X)

- toml-test's current tag and the exact size of its 1.0.0 list —
  read at fetch time and written into the provenance note.
- The wild corpus's size on disk, and whether a snapshot belongs in
  git or in a pinned-manifest store outside it (decided in the plan
  after measuring).
- Whether `strtod` on this libc rounds every toml-test float exactly
  (expected: yes; the oracle will say).
- How much of `librarium_merge`'s real input the strict parser
  rejects (§VIII risk).
- Whether any materia substrate change is needed (crusta needed one;
  html none). Recorded as found, per the client-two lesson.
