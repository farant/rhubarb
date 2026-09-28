# toml_arbor Plan — TOML 1.0 on materia (seventh client)

**STATUS: OPEN 2026-09-28** — written from the approved spec; no task
executed yet.

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:executing-plans
> (inline, per house convention — no subagents) to implement this plan
> task-by-task, ONE TASK PER TURN with Fran's approval. Every task OPENS
> with a measurement that may re-slice it; a re-slice is shown to Fran
> before work continues. Steps use checkbox (`- [ ]`) syntax.

**Goal:** TOML 1.0.0 as a materia client — a total, byte-exact tree for
every input; a cooked view of tables and typed values with every
semantic rule enforced; every error in a file reported with a location;
STML projection, canon, computus twin; proven by toml-test (valid and
invalid), by Python's `tomllib`, and by a wild corpus of real files; the
three house consumers migrated and `lib/toml.c` retired; briar exposes
it to `.thistle` authors.

**Architecture:** spec §§II–VIII. `toml_lector` (two modes, key and
value; every byte a token) → `toml_arbor` (iterative builder, `malum`
for what the grammar cannot place, recovery at the next newline outside
brackets) → `MateriaNodus` against the generated `TOML_REGISTRUM`.
`toml_coctum` walks the tree into `TomlValor` tables/values and emits
semantic diagnostics. Syntax diagnostics come from the declaration
(`diagnosticum=` / `absentia=`) through `materia_diagnostica_derivare`;
rendering is `materia_pictor_scribere`. `toml_oraculum` compares a
cooked view with a tagged-JSON expectation (toml-test's own, and a
`tomllib` golden).

**Tech Stack:** C89 via latina.h, house flags (`tools/vexilla.sh`),
credo, piscina, chorda, Xar, tabula_dispersa, utf8, json (the oracle
comparator), materia (`materia/fontes`), the generator
`./materia/coquere.sh`, canon, pythonica (`silva.commissio`,
`silva.planta`). Python 3.12.6 `tomllib` for the golden generator only.

**Spec:** `project-specs/toml-arbor-spec.md` (decisions T1–T11 in §0).
Precedents: `project-specs/crusta-arbor-plan.md` (task shape, closed),
`crusta/CLAUDE.md` (client shape as built), `crusta/grammatica/
crusta.registrum.stml` (declaration syntax incl. `diagnosticum=`,
`absentia=`), `crusta/fontes/crusta_lexicon.c` (lexicon rows),
`crusta/fontes/crusta_arbor.c` (iterative builder + ligator),
`crusta/probationes/probatio_crusta_*.c` (every gate's shape).

## Global Constraints

- **Latin throughout** — identifiers, comments, messages (docs English).
  `toml`, `json`, `token`, `offset`, `index`, `byte`, `utf8` are house
  words already; any new word is checked with `./oratio/quaere.sh w…`
  BEFORE staging; a missing word is a glossary line.
- **Every latina.h macro is a FORBIDDEN identifier** — the lowercase
  words (`nomen`, `registrum`, `magnitudo`, `casus`, `per`, `duplex`,
  `si`…) AND, since e1e01751, **every Roman numeral 0–3999** (`DI`,
  `MIX`, `CIV`, `LI`…). Beyond 3999 write `IV * M` / `IV * MXXIV`.
- **`chorda` is NOT null-terminated. `i32`/`i64` are UNSIGNED**; counts
  that go down, cursors, `-I` sentinels take `s32`/`s64`. TOML integers
  are `s64`.
- New C files by `./silva/scribe.sh <via> <<'EOF'`; a generated `.c`
  needs its own `.h`; `lib/*.c` or a leading `<tag>` inside a block
  comment breaks it.
- **House format**: `./silva/formator.sh <viae> -scribere` then `-vitia`
  (banners 50 `=`, lines ≤ 72). Scope with `-intra f -intra g` (the flag
  REPEATED) on files that carry old divergences.
- **Generated files are never hand-edited**: `toml_registrum_coctum.{h,c}`
  from `./materia/coquere.sh toml/grammatica/toml.registrum.stml
  -scribere`. Genera and loci APPENDED, never reordered; node sizes from
  `TOML_REGISTRUM.genera[g].loci_numerus`, never hand counts.
- **Exit 2 from any runner = NOTHING RAN.** Never read `$?` after a
  pipe. The runner locks `toml/build/cursor.sera`.
- **Every gate born red by a plant that COMPILES** (`silva.planta`); a
  plant that does not fire is a measurement of the test. After a plant
  that runs a REGENERATING gate, re-run the regenerators too.
- **Docs first, gates last, commit through `silva.commissio(msg, viae,
  portae)`** with explicit paths; never `git add -A`. Commit messages end
  with `Co-Authored-By: <the model that ran> <noreply@anthropic.com>`.
- **Never stage Fran's files**: `FAQ.md`, `gesta/annales/tabula.md`,
  `gesta/annales/tabularium.jsonl`, `gesta/annales/forum.jsonl`,
  `silva/grammatica/c89-formatted.stml`.
- **materia is expected to need ZERO changes.** A substrate defect found
  here gets its own gate in `materia/probationes`, its own commit, and
  `./materia/shim_probare.sh`.
- **Positions are byte offsets** + 1-based linea/columna; tokens point
  into the source, so test sources are allocated from the piscina.
- **Every fetch is an external action — ASK Fran before it** (Q1).
- **Silva is frozen to bug fixes**; nothing here edits `silva/fontes`.

## Review Focus

Five input classes the spec is silent on, most likely to bite first;
each is pinned by a test in the named task.

1. **Numeric-looking dotted keys** — `3.14159 = "pi"` is the table `3`
   with key `14159`, not a float key; `1979-05-27 = 1` is a bare key.
   Expect: keys, never values (Q3 lector cases, Q7 cooked case).
2. **Integer boundaries** — `-9223372036854775808` accepted,
   `9223372036854775808` rejected with a named overflow; `+0`, `-0`
   accepted; `007`, `0x_1`, `1__2`, `_1` rejected (Q7).
3. **CRLF files** — a whole document in CRLF: byte law holds; a
   multiline basic string's decoded value normalizes the newline as
   `tomllib` does (measured in Q1, asserted in Q7); a lone CR is an error.
4. **A UTF-8 BOM at the start** — behaviour measured against `tomllib`
   and toml-test in Q1; whichever is chosen (accept as trivia, or reject
   with a named diagnostic), the byte law holds (Q4) and the verdict is
   pinned (Q7).
5. **Lookup-path edge cases** — empty path, path through an array,
   path into a non-table, quoted segment with escapes (`a."b.c"`),
   malformed path (`a..b`): `toml_quaerere` returns NIHIL, typed
   getters FALSUM, nothing crashes (Q10).

---

## Task Q1: Oracle material — toml-test (ASK), the wild corpus, the `tomllib` golden generator

**Files:**
- Create: `toml/probationes/fixa/toml-test/` (the pinned suite: `tests/`
  subtree only, `LICENSE`, `PROVENIENTIA.md`)
- Create: `toml/probationes/fixa/silvestria/` or
  `toml/probationes/fixa/silvestria.manifestum` (decided in Step 3)
- Create: `toml/instrumenta/tomllib_tagatum.py`, `toml/tomllib_aurum.sh`
- Create: `toml/probationes/fixa/tomllib/aurum.txt` (generated)
- Create: `toml/CLAUDE.md` (stub: what this is, spec, plan)

**Interfaces:**
- Produces: the tagged-JSON contract every later oracle uses — a TOML
  document becomes a JSON object; a table → object; an array → array; a
  scalar → `{"type": T, "value": V}` with `T` ∈ `string integer float
  bool datetime datetime-local date-local time-local` and `V` a string
  (toml-test's format; verified against the fetched files in Step 1).
- Produces: golden block format of `aurum.txt`:
  `#### <via relativa>\n## STATUS: VALIDUM|INVALIDUM\n<json una linea>\n## FINIS\n`
  (the json line is absent for INVALIDUM).

- [x] **Step 1: Measure, then ASK before fetching.** Report to Fran:
  toml-test's current release tag and commit (from the GitHub releases
  page), licence (expected MIT), and the size of `tests/`. On a yes:
  fetch that tag, keep `tests/` + `LICENSE` only, write
  `PROVENIENTIA.md` (repo URL, tag, commit, date, licence, "frozen
  input — never edited; a new tag is a new commit with a cause"). Count
  and record: valid cases, invalid cases, and the entries of the 1.0.0
  file list (expected `tests/files-toml-1.0.0`; confirm the name).
  Verify the tagged-JSON contract above on three valid cases by reading
  them.

- [x] **Step 2: The `tomllib` harness** — `tomllib_tagatum.py <via>`:
  read bytes, `tomllib.loads(bytes.decode('utf-8'))`; on success print
  the tagged JSON (sorted keys, `ensure_ascii=False`, one line): `bool`
  → `bool`; `int` → `integer` with `str(v)`; `float` → `float` with
  `inf`/`-inf`/`nan` spelled so, else `repr(v)`; `str` → `string`;
  `datetime` with tzinfo → `datetime` (`isoformat()`), without →
  `datetime-local`; `date` → `date-local`; `time` → `time-local`; on
  `TOMLDecodeError` or `UnicodeDecodeError` print `INVALIDUM: <msg>` and
  exit 1. `toml/tomllib_aurum.sh <via…>` writes the golden blocks.

- [x] **Step 3: The wild corpus — measure, then decide** — `find ~/.cargo
  /opt/homebrew ~/Documents/projects -name '*.toml'` (excluding
  `node_modules`, `build`, and this repo's own fixtures): count, total
  bytes, largest file, duplicate content (hash). If the deduplicated
  set is ≤ 20 MB: copy it under `fixa/silvestria/` with a manifest line
  per file (`sha256  origo`). If larger: show Fran the numbers and the
  alternative (manifest of paths + sha256 committed; the gate reads the
  live files and SKIPS-WITH-COUNT any whose hash changed). Fran decides.

- [x] **Step 4: Measure the oracles against each other** — run the
  harness over every valid and invalid case of the 1.0.0 list: how many
  valid cases does `tomllib` accept with JSON equal to toml-test's
  expected (after the float/datetime normalizations of Q8)? How many
  invalid cases does it reject? Record both numbers and every
  disagreement in `PROVENIENTIA.md` — this is the credibility of oracle
  2, measured before we depend on it. Also measure Review Focus 3 and 4
  here: `tomllib` on a CRLF multiline string and on a BOM-prefixed file;
  record the verdicts.

- [x] **Step 5: Generate the golden** — `aurum.txt` over the 1.0.0 list
  and the wild corpus; header lines record `python3 --version` and the
  date.

- [x] **Step 6: Commit** (no C yet, so no C gate; `pythonica` if the
  harness is registered there)

```python
silva.commissio("toml: materia oraculi - toml-test <tag> fixum (1.0.0: N valida, M invalida; licentia MIT, provenientia), corpus silvestre (<numeri>), generator aurei tomllib (Python 3.12.6) cum aureo; concordia oraculorum mensurata ante usum\n\nCo-Authored-By: ...",
    ["toml/probationes/fixa/toml-test", "toml/probationes/fixa/silvestria",
     "toml/instrumenta/tomllib_tagatum.py", "toml/tomllib_aurum.sh",
     "toml/probationes/fixa/tomllib/aurum.txt", "toml/CLAUDE.md"], [])
```

**Executed 2026-09-28.** toml-test v2.2.0 (MIT, ce08da1d) fetched after
asking; 1.0.0 list = 205 valid / 474 invalid (counted; the web summary's
256/512 was wrong). `tomllib` agreement TOTAL (205/205, 474/474). Wild
corpus: Fran chose MANIFEST ONLY (1,795 unique files, sha256 + path);
consequently the golden SPLIT — `fixa/tomllib/aurum.txt` (toml-test,
committed, 122 KB) and `toml/build/aurum_silvestre.txt` (regenerated,
never committed: 6.25 MB of third-party content as JSON). Harness gained
a batch mode (`-aurum <radix>`, paths on stdin). Verdicts pinned: CRLF
accepted (multiline CRLF → `\n`), lone CR rejected, BOM REJECTED (as
`tomllib`). Wild bonus: toml_edit's invalid fixtures are in the corpus.

---

## Task Q2: Scaffold — declaration, generated registry, lexicon, runner, registrum gate

**Files:**
- Create: `toml/grammatica/toml.registrum.stml`
- Generate: `toml/fontes/toml_registrum_coctum.{h,c}`
- Create: `toml/fontes/toml_registrum.h`, `toml/fontes/toml_lexicon.{h,c}`
- Create: `toml/compile_probationes.sh` (from `crusta/compile_probationes.sh`)
- Create: `toml/probationes/probatio_toml_registrum.c`
- Modify: `pythonica/silva.py` (`PORTAE['toml']`, `FORMAE['toml']`,
  `SUITAE['toml']`), `toml/CLAUDE.md`

**Interfaces:**
- Consumes: `materia_lexicon.h` (`MateriaLexGenus {titulus,
  orthographia, species, munus}`, `MateriaLexiconCoctum`),
  `materia_coctor.h` (`materia_registrum_recens`), `materia_arbor.h`.
- Produces: `TOML_REGISTRUM`, `TomlGenus` (`TOML_GENUS_DOCUMENTUM` …,
  `TOML_GENUS_NUMERUS_GENERUM`), `TOML_LEXICON`, `TomlLexGenus`, the slot
  enums.

- [x] **Step 1: Measure the vocabulary against spec §IV** — list every
  construct in the toml-test 1.0.0 valid cases' categories and confirm
  each has a home in the genera below; add a genus only with a cause in
  the commit message.

- [x] **Step 2: The declaration** — `toml.registrum.stml`, header comment
  in crusta's shape (spec path; "genera APPENDUNTUR"; T4 totality; the
  newline rule of spec §III). Genera in this order, loci in BYTE order:

```xml
<registrum grammatica="toml" praefixum="TOML" typus="TomlGenus" sedes="toml/fontes">
  <genus titulus="documentum" nota="radix: sententiae ordine octetorum + FINIS">
    <locus titulus="liberi" species="lista-nodus"/>
    <locus titulus="cauda"  species="token"/>
  </genus>
  <genus titulus="caput-tabulae" nota="[clavis]">
    <locus titulus="tok_apertura" species="token"/>
    <locus titulus="clavis"       species="nodus"/>
    <locus titulus="tok_clausura" species="token" absentia="']' deest" gravitas="error"/>
  </genus>
  <genus titulus="caput-seriei" nota="[[clavis]]">
    <locus titulus="tok_apertura" species="token"/>
    <locus titulus="clavis"       species="nodus"/>
    <locus titulus="tok_clausura" species="token" absentia="']]' deest" gravitas="error"/>
  </genus>
  <genus titulus="par" nota="clavis = valor">
    <locus titulus="clavis"     species="nodus"/>
    <locus titulus="tok_signum" species="token" absentia="'=' deest" gravitas="error"/>
    <locus titulus="valor"      species="nodus" absentia="valor deest" gravitas="error"/>
  </genus>
  <genus titulus="clavis" nota="segmenta et puncta ordine octetorum">
    <locus titulus="partes" species="lista-token"/>
  </genus>
  <genus titulus="chorda" nota="quattuor formae: genus lexematis dicit">
    <locus titulus="tok" species="token"/>
  </genus>
  <genus titulus="numerus" nota="integer aut fluitans: coctio classem dicit">
    <locus titulus="tok" species="token"/>
  </genus>
  <genus titulus="boolean"><locus titulus="tok" species="token"/></genus>
  <genus titulus="tempus"  nota="quattuor genera: coctio dicit"><locus titulus="tok" species="token"/></genus>
  <genus titulus="series" nota="[v, v] - commata, lineae, commentaria intus">
    <locus titulus="tok_apertura" species="token"/>
    <locus titulus="liberi"       species="lista-nodus"/>
    <locus titulus="tok_clausura" species="token" absentia="']' deest" gravitas="error"/>
  </genus>
  <genus titulus="tabula-inlinea" nota="{k = v, ...} - una linea (1.0)">
    <locus titulus="tok_apertura" species="token"/>
    <locus titulus="liberi"       species="lista-nodus"/>
    <locus titulus="tok_clausura" species="token" absentia="'}' deest" gravitas="error"/>
  </genus>
  <genus titulus="comma" inanis="verum"><locus titulus="tok" species="token"/></genus>
  <genus titulus="linea" nota="linea nova quae sententiam terminat" inanis="verum">
    <locus titulus="tok" species="token"/>
  </genus>
  <genus titulus="malum" diagnosticum="lexemata quae grammatica ponere non potuit" inanis="verum">
    <locus titulus="tokens" species="lista-token"/>
  </genus>
</registrum>
```

  Judge: `bin/canon_examen toml/grammatica/toml.registrum.stml` — no
  vitia (exit 2 = root not registered: check the path). Attribute names
  and `gravitas` values are checked against crusta's declaration before
  writing; if the registrum canon names them differently, follow it.

- [x] **Step 3: Generate** — `./materia/coquere.sh
  toml/grammatica/toml.registrum.stml -scribere`, then without
  `-scribere` (exit 0 = recens). Read the generated header once.

- [x] **Step 4: Slot enums (`toml_registrum.h`)** — one enum per genus,
  members in locus order, e.g.:

```c
nomen enumeratio { TOML_DOCUMENTUM_LIBERI = 0, TOML_DOCUMENTUM_CAUDA } TomlDocumentumLocus;
nomen enumeratio { TOML_CAPUT_TOK_APERTURA = 0, TOML_CAPUT_CLAVIS,
    TOML_CAPUT_TOK_CLAUSURA } TomlCaputLocus;      /* et caput-seriei */
nomen enumeratio { TOML_PAR_CLAVIS = 0, TOML_PAR_TOK_SIGNUM, TOML_PAR_VALOR } TomlParLocus;
nomen enumeratio { TOML_CLAVIS_PARTES = 0 } TomlClavisLocus;
nomen enumeratio { TOML_SCALARIS_TOK = 0 } TomlScalarisLocus;
    /* chorda, numerus, boolean, tempus, comma, linea */
nomen enumeratio { TOML_INCLUSA_TOK_APERTURA = 0, TOML_INCLUSA_LIBERI,
    TOML_INCLUSA_TOK_CLAUSURA } TomlInclusaLocus;  /* series, tabula-inlinea */
nomen enumeratio { TOML_MALUM_TOKENS = 0 } TomlMalumLocus;
```

- [x] **Step 5: The lexicon (`toml_lexicon.{h,c}`)** — `TomlLexGenus` in
  this order, rows in `GENERA_TOML[]`, `TOML_LEXICON = { GENERA_TOML,
  count, "toml-", (s32)-I }`:

```c
nomen enumeratio {
    TOML_LEX_FINIS = 0,
    /* claves */
    TOML_LEX_CLAVIS_NUDA, TOML_LEX_CLAVIS_BASICA, TOML_LEX_CLAVIS_LITTERALIS,
    TOML_LEX_PUNCTUM, TOML_LEX_SIGNUM,
    /* capita */
    TOML_LEX_TABULA_APERTURA, TOML_LEX_TABULA_CLAUSURA,
    TOML_LEX_SERIES_TABULARUM_APERTURA, TOML_LEX_SERIES_TABULARUM_CLAUSURA,
    /* valores */
    TOML_LEX_CHORDA_BASICA, TOML_LEX_CHORDA_BASICA_MULTA,
    TOML_LEX_CHORDA_LITTERALIS, TOML_LEX_CHORDA_LITTERALIS_MULTA,
    TOML_LEX_NUMERUS, TOML_LEX_TEMPUS, TOML_LEX_VERUM, TOML_LEX_FALSUM,
    TOML_LEX_SERIES_APERTURA, TOML_LEX_SERIES_CLAUSURA,
    TOML_LEX_INLINEA_APERTURA, TOML_LEX_INLINEA_CLAUSURA, TOML_LEX_COMMA,
    /* terminator et trivia */
    TOML_LEX_LINEA_FINIS, TOML_LEX_SPATIUM, TOML_LEX_COMMENTUM, TOML_LEX_LINEA,
    TOML_LEX_IGNOTUM,
    TOML_LEX_NUMERUS_GENERUM
} TomlLexGenus;
```

  FIXUM with orthography for the brackets, `=`, `.`, `,`, `true`,
  `false`, `FINIS ""`; TERMINATOR `"\n"` for `LINEA_FINIS` (SUBSTANTIVUM
  — ends a pair) and `LINEA` (munus LINEA — trivia inside brackets);
  VERBATIM for the rest; `SPATIUM` munus SPATIUM, `COMMENTUM` COMMENTUM.
  `IGNOTUM` = a byte no rule accepts (becomes a malum).

- [x] **Step 6: The runner** — copy `crusta/compile_probationes.sh`,
  rename `crusta`→`toml` (dir, log `build/test_logs/toml.log`, banner
  `TOML PROBATIONES: N/M praeteritae`, env names, mensor prefix
  `toml.`); `RADIX_FONTES` = `piscina chorda chorda_aedificator xar
  friatio tabula_dispersa internamentum stml stml_macros selectio
  similitudo canon credo utf8 json`.

- [x] **Step 7: Write the failing registrum probatio** — transcribe
  `crusta/probationes/probatio_crusta_registrum.c`: lexicon order by
  title, genus order, recens gate over the declaration, `loci_offset`
  contiguity, every slot enum member against the table's titles (count
  = `numerus_locorum`), munus facts (`SPATIUM`/`COMMENTUM`/`LINEA` are
  trivia, `LINEA_FINIS` is NOT), one minimal tree (`documentum` with one
  `par` `a = 1`) through writer → reader → writer byte-equal, with
  `grammatica="toml"` and `<toml-clavis-nuda` present.

- [x] **Step 8: Register in pythonica NOW** — `PORTAE['toml'] =
  (['./toml/compile_probationes.sh'], r'TOML PROBATIONES: \d+/\d+')`,
  `FORMAE['toml'] = 'suita'`, `SUITAE['toml'] = ('toml/probationes',
  'toml/build/%s')`.

- [x] **Step 9: Run: red, then green** — `./toml/compile_probationes.sh registrum`.

- [x] **Step 10: Plant** — swap the `SPATIUM` and `COMMENTUM` rows →
  red on order-by-title; revert. By hand: one byte of the generated `.c`
  → the recens gate names the line; revert.

- [x] **Step 11: Words, format, commit**

```python
silva.commissio("toml: cliens materiae septimus natus - declaratio registri (XIV genera), tabulae coctae, lexicon (XXVIII genera lexematum; linea nova bis: terminator et trivium), cursor, porta registri (rubra nata: ordo lexici)\n\nCo-Authored-By: ...",
    ["toml/grammatica/toml.registrum.stml", "toml/fontes/toml_registrum_coctum.h",
     "toml/fontes/toml_registrum_coctum.c", "toml/fontes/toml_registrum.h",
     "toml/fontes/toml_lexicon.h", "toml/fontes/toml_lexicon.c",
     "toml/compile_probationes.sh", "toml/probationes/probatio_toml_registrum.c",
     "toml/CLAUDE.md", "pythonica/silva.py"], ["toml", "pythonica"])
```

**Executed 2026-09-28.** Vocabulary check: toml-test's ten valid
categories each have a home; no genus added. Deviations from the text:
`gravitas` values are `erratum`/`monitum` (absent = erratum), so the
declaration omits them; three words unknown to the lint were replaced
by house words — basic/literal strings and keys are GEMINA/SIMPLEX
(crusta's names for the two quote styles), the inline table is
`tabula-compacta` (`brevis`/`interior` are latina.h macros), the
single-token slot enum is `TomlLexemaLocus`. The generator also emits
`TOML_DIAGNOSTICA` from `diagnosticum=`/`absentia=`. Runner adds
`filum` and `json` to RADIX_FONTES, drops `processus_posix` and
crusta's corpus/differentia tail. Gate: 228 assertions; plants — two
lexicon rows swapped (red on order-by-title), one generated byte
(`RANCIDUM …coctum.c:79`); both reverted green.

---

## Task Q3: The lector — two modes, the number-like token, UTF-8 and control bytes

**Files:**
- Create: `toml/fontes/toml_lector.{h,c}`, `toml/probationes/probatio_toml_lector.c`

**Interfaces:**
- Consumes: `materia_token_creare(piscina, &forma, genus, valor, offset,
  linea, columna, fons_index)`, `utf8_decodere`, `TomlLexGenus`.
- Produces (`toml_lector.h`):

```c
nomen enumeratio {
    TOML_MODUS_CLAVIS = 0,   /* initium lineae, intra [ ] / [[ ]], ante '=' in {} */
    TOML_MODUS_VALOR,        /* post '=', intra series */
    TOML_MODUS_NUMERUS_MODORUM
} TomlModus;
nomen structura { s32 cursor; i32 linea; s32 linea_initium; } TomlSitus;
nomen structura {
               Piscina* piscina;
    constans character* fons;
                   i32  mensura;
              TomlSitus situs;
                   i32  profunditas;     /* [ { apertae: linea nova trivium */
      MateriaTokenForma forma;
} TomlLector;

b32           toml_lector_incipere (TomlLector*, Piscina*, constans character* fons, i32 mensura);
MateriaToken* toml_lector_proximum (TomlLector*, TomlModus);  /* NIHIL = memoria */
TomlSitus     toml_lector_situs (constans TomlLector*);
vacuum        toml_lector_situm_reponere (TomlLector*, TomlSitus);
vacuum        toml_lector_profunditatem_ponere (TomlLector*, i32 profunditas);
```

- [x] **Step 1: Write the failing lector gate** — cases `{fons, modi[],
  exspectata[]}`; each token's genus and bytes; BYTE COVERAGE (the
  concatenation of every token equals the source, FINIS last). Cases, at
  least:
  - keys: `a = 1`, `"a b" = 1`, `'c:\x' = 1`, `a.b.c = 1`, `a . b = 1`
    (spaces around dots are trivia), `1234 = 1`, `true = true`,
    `3.14159 = "pi"` in CLAVIS → NUDA `3`, PUNCTUM, NUDA `14159`
    (Review Focus 1), `1979-05-27 = 1` → one NUDA.
  - numbers in VALOR (one NUMERUS token each, classification is Q7's):
    `+99`, `-17`, `1_000`, `0xDEAD_beef`, `0o755`, `0b1101`, `3.14`,
    `-0.01`, `5e+22`, `1e06`, `6.626e-34`, `inf`, `+inf`, `-nan`,
    `9_224_617.445_991_228_313`; malformed but TOTAL: `1__2`, `0x`,
    `1.` — still one token, Q7 rejects.
  - datetimes → one TEMPUS: `1979-05-27T07:32:00Z`,
    `1979-05-27T00:32:00.999999-07:00`, `1979-05-27 07:32:00` (the space
    joins), `1979-05-27`, `07:32:00`, `00:32:00.999999`;
    `1979-05-27 # c` → TEMPUS, SPATIUM, COMMENTUM (the space does NOT
    join); `1979-05-27T07:32` (1.0 needs seconds) → one token, Q7 rejects.
  - strings: `"a\"b"` one CHORDA_BASICA; `"""a\n""b"""` one MULTA (inner
    quotes kept); `'''a'''` ; `'a\b'` (no escapes in literal); unclosed
    `"abc` + EOF → one token to the line end (total), Q4 marks it.
  - newlines: `a = 1\n` → … LINEA_FINIS; inside `[1,\n2]` (profunditas
    1) → LINEA (trivia); CRLF both ways (value is two bytes); a lone CR →
    IGNOTUM (Review Focus 3).
  - comments and control bytes: `a = 1 # c` → COMMENTUM without the
    newline; a comment holding a TAB is fine; a comment holding U+0000
    or U+007F or a raw control byte → the COMMENTUM token still covers
    it and `toml_lector` flags nothing (validity is Q7's), BUT invalid
    UTF-8 anywhere (overlong `C0 AF`, surrogate `ED A0 80`, truncated)
    → the offending bytes become one IGNOTUM token. Measure first
    whether `utf8_decodere` rejects overlongs and surrogates; if it does
    not, the lector checks them itself (named in the header comment).
  - BOM: `EF BB BF` at offset 0 → the verdict measured in Q1 (trivia
    SPATIUM or IGNOTUM); pinned here.
  - positions: linea/columna of every token in a three-line source;
    `situs`/`situm_reponere` round trip re-lexes the same tokens.
  Self-measure: cases ≥ 40, bytes covered printed.

- [x] **Step 2: Run and verify it fails** — `./toml/compile_probationes.sh lector`.

- [x] **Step 3: Implement** — one `commutatio` on the mode: `_trivium`
  (blanks, comment, newline as LINEA or LINEA_FINIS by `profunditas`),
  `_clavis` (bare run of `A-Za-z0-9_-`, quoted keys, `.`, `=`, `[`/`[[`,
  `]`/`]]`), `_valor` (strings: basic scans escapes only to find the
  close; multiline closes on the LAST of a run of up to five quotes;
  literal never escapes), `_numerus_similis` (greedy over `0-9 A-Z a-z
  _ + - . :` plus ONE space when the next bytes are two digits and a
  colon and the token so far is a full date `DDDD-DD-DD`), `true`/`false`
  only when the run equals the word exactly, `[ ] { } ,`. Everything
  else → IGNOTUM one UTF-8 sequence long.

- [x] **Step 4: Run and verify** — green, coverage printed.

- [x] **Step 5: Plant** — the datetime space rule without the full-date
  check → `1979-05-27 # c` red; revert.

- [x] **Step 6: Words, format, commit** — `silva.commissio("toml: lector - modi II (clavis/valor), lexema numero simile unum (dies-spatium-hora iunguntur), chordae IV, linea nova per profunditatem, utf8 validata; porta lectoris (tegumentum octetorum; rubra nata: spatium temporis sine die)\n\nCo-Authored-By: ...", ["toml/fontes/toml_lector.h", "toml/fontes/toml_lector.c", "toml/probationes/probatio_toml_lector.c"], ["toml"])`

**Executed 2026-09-28.** Measured first: `utf8_decodere` already rejects
overlongs, surrogates, > U+10FFFF, truncated and lone continuation bytes
(returning the bytes consumed), so the lector relies on it — no check of
its own. Deviation: invalid UTF-8 and control bytes INSIDE comments and
strings stay in the token (cutting a comment at a bad byte would lex its
rest as keys); cooking (Q7) judges them. Outside them a bad sequence, the
BOM, a lone CR, or a bare word in value position (`a = hello`) is
IGNOTUM. `{` in CLAVIS mode is IGNOTUM (never needed there); `}` and `,`
are recognized in both modes. Gate: 59 cases + positions + situs,
240 assertions, 473 bytes covered. First run green; the plan's plant
did not discriminate (`1979-05-27 # c` never joins — `#` is not a
digit), so a case was ADDED that does (`1234567890 12:00:00`: ten
characters, not a date, must not join) and the plant (full-date check
removed) fired red on it; reverted green.

---

## Task Q4: The builder — documents, headers, pairs, values, mala, recovery; byte law over every fixture

**Files:**
- Create: `toml/fontes/toml_arbor.{h,c}`, `toml/probationes/probatio_toml_arbor.c`,
  `toml/probationes/probatio_toml_corpus.c`

**Interfaces:**
- Consumes: Q3's lector; `materia_nodus_creare/ponere/appendere`,
  `materia_valor_token/nodus/lista_nova`, `materia_scribere_nodum`,
  `materia_token_trivia_ante_ponere/_post_ponere`,
  `materia_lexicon_trivium_est`.
- Produces (`toml_arbor.h`):

```c
nomen structura {
    i32 mala;                 /* nodi malum */
    i32 clausurae_absentes;   /* ] ]] } ' " absentes */
    i32 profunditas_maxima;
    b32 sana;                 /* mala == 0 && clausurae_absentes == 0 */
} TomlParsura;

MateriaNodus* toml_arbor_parsare (Piscina*, constans character* fons, i32 mensura,
                                  TomlParsura* relatio);   /* relatio NIHIL licet */
```

- [x] **Step 1: Write the failing arbor gate** — sources from the
  piscina, parsed, emitted, `memcmp` against the source, plus structural
  asserts (`_liberi`, `_genus`, `_tok` helpers as in crusta):
  - `a = 1\n` → documentum.liberi = [par(clavis[a], signum, numerus),
    linea]; empty source → liberi empty, cauda FINIS.
  - headers: `[a.b]\nc = 1\n`, `[[p]]\nx = 1\n[[p]]\nx = 2\n`, `[ a . "b" ]`.
  - values: every scalar kind; `[1, [2, 3], {x = 1}]`; `[\n  1, # c\n  2,\n]`
    (trailing comma legal in arrays); `{ a = 1, b.c = 2 }`.
  - trivia binding (crusta C7 rule, measured on TOML): `# c\na = 1` →
    the comment is ANTE of `a`; `a = 1 # c\n` → POST of the value, the
    newline is the `linea` node; blank lines are ANTE of the next item.
  - mala and recovery (T4): `a = \nb = 2` → par with valor absent (one
    absentia diagnostic), then par b; `a 1` → malum, recovery at the
    newline, the next line parses; `= 1`, `]]`, `}` alone → mala;
    three bad lines in one file → three mala and three following good
    pairs intact; `a = [1, 2` + EOF → series with `tok_clausura` absent,
    `clausurae_absentes == 1`; unclosed `"abc` → one absentia.
  - CRLF: every case above with `\n` → `\r\n` emits byte-exact.
  - BOM case per Q1's verdict: byte-exact.

- [x] **Step 2: Run and verify it fails** — `./toml/compile_probationes.sh arbor`.

- [x] **Step 3: Implement** — crusta's loop (frame stack + ligator): the
  top frame's genus and slot choose the lector MODE (documentum/header/
  inline-table-before-`=` → CLAVIS; after `=` and inside `series` →
  VALOR); `profunditas` = open `[`/`{` count, pushed to the lector;
  `LINEA_FINIS` closes a `par` or header; a token the frame cannot take
  → `_malum` absorbs tokens up to (not including) the next `LINEA_FINIS`
  (T4 recovery), unless inside brackets (then up to the closing bracket
  or the newline, whichever comes first, and the bracket frame closes
  absent). At EOF every open frame closes with absent tokens.

- [x] **Step 4: Run and verify** — green.

- [x] **Step 5: The corpus gate (`probatio_toml_corpus.c`)** — byte law
  and `CREDO_NON_RUIT` over EVERY file: toml-test valid AND invalid (the
  1.0.0 list), the wild corpus, the house `.toml` files (`git ls-files`
  at run time). Counts printed per source; valid-list files with
  `sana == FALSUM` printed by name (expected: 0 — each one is a builder
  bug to fix with an arbor case first).

- [x] **Step 6: Plant** — recovery absorbs THROUGH the newline → the
  three-bad-lines case red (the following pair is lost); revert.

- [x] **Step 7: Commit** — `["toml/fontes/toml_arbor.h", "toml/fontes/toml_arbor.c", "toml/probationes/probatio_toml_arbor.c", "toml/probationes/probatio_toml_corpus.c"]`, `["toml"]`, message naming the corpus counts.

**Executed 2026-09-28.** Deviations: (1) newline by mode needed a second
lector switch — `toml_lector_sententiam_ponere`: a newline terminates
only when a statement is open, so blank and comment-only lines are trivia
(no `linea` node per blank line); (2) the trivia rule is TOML's own, not
crusta's C7 — after a terminating newline or at the document start all
pending trivia go ANTE the next token, otherwise up to the FIRST newline
POST the previous one (worklog `toml/fontes/toml_arbor.worklog.md`);
(3) `TomlParsura` gained `absentiae` (missing key, '=' or value) and `sana`
requires it zero; a bad value becomes the pair's `valor` (a malum) so the
key survives; key segments must alternate with dots. The corpus gate's
first run found one real bug — a 0-byte file reaches the lector as a NULL
pointer (`valid/empty-nothing.toml`), now accepted and pinned. Numbers:
arbor gate 46 cases / 253 assertions; corpus gate — toml-test 679/679
byte-exact (valid insane 0; 208/474 invalid already rejected by syntax,
the rest are Q7's), house 12/12 sane, wild 1,794/1,794 byte-exact
(3.27 MB; 1,733 sane; 1 skipped: changed on disk). Plants: malum through
the newline (three-bad-lines case red), forward-binding rule removed
(ante count red), trailing trivia dropped (corpus gate red on 2,364
files); all reverted green.

---

## Task Q5: STML projection — two cycles, the comparator

**Files:** Create `toml/probationes/probatio_toml_stml.c`.

**Interfaces:** `materia_lexicon_ratum_facere`, `materia_arbor_consilium_nudum(&c, &TOML_REGISTRUM, &ratum, "toml")`, `materia_arbor_scribere_nodum`, `materia_arbor_legere`, `materia_arbor_aequalis` (as crusta P7).

- [x] **Step 1: Write the failing gate** — every Q4 inline case AND the
  whole corpus of Q4 Step 5: `s1 = scribere(radix)`, `r1 = legere(s1)`,
  `s2 = scribere(r1)`, `s1 == s2`, `aequalis(radix, r1, STRUCTURALIS)`,
  emission of `r1` equals the source; FIDELITAS tried and, if accepted,
  asserted everywhere (crusta found it holds). Refusals printed with
  `s.causa`; a refusal for any cause but a named raw-form limit is red.
- [x] **Step 2: Run, fix causes (never assertions), run the suite.**
- [x] **Step 3: Plant** — remove the re-read document's `cauda` before
  the comparator → tree oracle red while `s1`/`s2` stay equal; revert.
- [x] **Step 4: Commit** — `["toml/probationes/probatio_toml_stml.c"]`, `["toml"]`.

**Executed 2026-09-28.** Green on the first run: 41 inline cases (every Q4
source plus five STML stressors — a string and a comment carrying their own
closing sequence, a `\u` escape, a space-joined datetime, a multiline
string) and the whole corpus through both cycles, STRUCTURALIS, FIDELITAS
(measured and ASSERTED on every document, as crusta found) and
`materia_sedes_verificare`: toml-test 679/679, house 12/12, wild
1,794/1,794 (3.27 MB → 51.6 MB STML, ~16×). The corpus walk was factored
into a shared test helper (`probationes/toml_corpus_ambulare.{h,c}`,
compiled as an adiumentum) and the Q4 corpus gate rewritten on it (same
numbers). The oracle-separation mutation is a permanent pin, so the plant
was a different fault: the builder's initium_lineae rule disabled → red,
the comparator naming `lexema/initium-lineae` and the node path. Reverted
green. No substrate change.

---

## Task Q6: `toml.canon` — hand-written, drift-guarded, seal pinned

**Files:** Create `toml/grammatica/toml.canon`, `toml/probationes/probatio_toml_canon.c`.

- [x] **Step 1: Measure the species tables** — a scratch probe over every
  Q5 document prints, per genus and slot, the child genera seen (crusta
  P8's method: measured, not read from the builder).
- [x] **Step 2: Write the failing gate** — `canon_legere`, then for every
  Q5 document `stml_legere` + `canon_iudicare` → zero vitia; the drift
  guard (one rule per `TomlGenus`, count = `TOML_GENUS_NUMERUS_GENERUM`;
  every locus title under its rule; every lexicon title's `toml-*` tag in
  the lexeme fragments; every rule names a live genus); pinned
  `registrum-sigillum` equals `materia_arbor_sigillum(piscina,
  &TOML_REGISTRUM)` live.
- [x] **Step 3: Write the canon** from html's/crusta's header shape, the
  measured species; `<ante>`/`<post>` allow `toml-spatium`,
  `toml-commentum`, `toml-linea` only.
- [x] **Step 4: Plant** — delete the `par` rule → guard AND judgment red;
  a false sigillum → guard red; revert both.
- [x] **Step 5: Commit** — `["toml/grammatica/toml.canon", "toml/probationes/probatio_toml_canon.c"]`, `["toml"]`.

**Executed 2026-09-28.** Species MEASURED by a temporary probe (not
committed) over every Q5 document and ~25 corner cases: per
grandparent/parent the child elements and every attribute. Findings the
canon encodes: a pair's value (and an array element) is one of the seven
value kinds or a `malum`; inline tables hold pairs, commas, mala; a
`malum` holds any lexeme; text-bearing lexemes carry `cr`/`nul`; TOML's
two newline lexemes carry `crlf` (crusta's canon has no such attribute —
its newlines were declared differently); `ante`/`post` hold spatium,
commentum, linea only. The inline cases moved into the shared helper
(`TOML_CASUS`, read by the stml and canon gates). The canon text was
typed through a small script from the measured tables; `toml.canon` is
the source, never regenerated. Gate: 70 rules, drift guard both ways
(14 genera, 25 loci, 28 lexemes, envelope), seal `b7568363` pinned, 41
cases + 2,485 corpus documents judged with zero violations. Plants: the
`par` rule deleted (guard AND judgments red: 620/679 toml-test vitiosa),
a false seal (pin red, every document vitiosum); restored green.

---

## Task Q7: The cooked view — values, tables, the semantic rules, semantic diagnostics

**Re-sliced 2026-09-28 by the opening measurement (Fran approved).**
Of toml-test 1.0.0's 474 invalid cases, 208 are rejected by syntax
(Q4); the 266 that parse clean map onto 19 named rules, none left over:

| directories | cases | rule(s) |
|---|---|---|
| control/ + encoding/ | 25 + 6 | control byte (not TAB) in a string or comment; ill-formed UTF-8 (incl. an encoded surrogate) |
| string/ | 37 | unknown escape (`\x`, `\/`, `\ ` before a non-newline); short `\u`/`\U`; code point a surrogate or > 10FFFF |
| integer/ | 33 | bad integer, cause named (capital prefix, sign before prefix, leading zero, `_` not between digits, digit outside base, prefix without digits, double sign) |
| float/ | 35 | bad float, cause named (dot without digits both sides, double dot, bad exponent, `_`, leading zero, incomplete inf/nan) |
| datetime/ local-datetime/ local-date/ local-time/ | 65 | bad shape (leads, seconds, separator, 4-digit year, trailing bytes); field out of range (named field; Feb 29 in a common year) |
| key/ table/ inline-table/ array/ spec-1.0.0/ | 65 | key already defined (23, related location); table already defined; table already defined by dotted keys; header-defined table not extendable by dotted keys (8); inline table closed (12); key holds another kind (8); empty key segment `[a.]`; newline / trailing comma inside an inline table (5; TOML 1.1 features — the builder accepts them, cooking rejects them naming 1.1) |

Integer overflow has no 1.0 invalid case; the coctum gate pins it.

- **Q7a — scalar cooking** (201 cases): `toml/fontes/toml_valor.h`
  (the value model, types only), `toml/fontes/toml_scalaris.{h,c}`
  (strings incl. quoted keys, integers, floats, booleans, datetimes;
  comments' bytes; one pass over every token incl. trivia), gate
  `probatio_toml_scalaris.c` (inline cases with value AND diagnostic
  code + offset; corpus: every valid file clean, every scalar-category
  invalid file ≥ 1 diagnostic). Step 2's strings/integers/floats/
  datetimes/BOM bullets move here.
- **Q7b — tables and structure** (65 cases): `toml_coctum.{h,c}` as
  below — the walk, table modes, related locations, `TomlCoctum`,
  every-error sorted by offset; Step 2's tables/key-order/every-error
  bullets and Step 6's plant.

**Files:**
- Create: `toml/fontes/toml_coctum.{h,c}`, `toml/probationes/probatio_toml_coctum.c`

**Interfaces:**
- Consumes: Q4's tree; `tabula_dispersa.h`, `xar.h`, `materia_diagnosticum.h`.
- Produces (`toml_coctum.h`):

```c
nomen enumeratio {
    TOML_TABULA = 0, TOML_SERIES, TOML_CHORDA, TOML_INTEGER,
    TOML_FLUITANS, TOML_BOOLEAN, TOML_TEMPUS
} TomlGenusValoris;
nomen enumeratio {
    TOML_TEMPUS_CUM_ZONA = 0, TOML_TEMPUS_LOCALE,
    TOML_DIES_LOCALIS, TOML_HORA_LOCALIS
} TomlGenusTemporis;
nomen structura {
    TomlGenusTemporis genus;
    s32 annus; s32 mensis; s32 dies;
    s32 hora; s32 minutum; s32 secundum;
    s32 nanosecunda;          /* fractio ad IX digitos, reliqui abscisi */
    s32 zona_minuta;          /* CUM_ZONA solum; Z = 0 */
} TomlTempus;
nomen structura TomlValor TomlValor;
structura TomlValor {
    TomlGenusValoris genus;
    constans MateriaNodus* nodus;        /* T8 */
    unio {
        chorda chorda_valor; s64 integer_valor; f64 fluitans_valor;
        b32 boolean_valor; TomlTempus tempus_valor;
        structura { Xar* claves; Xar* valores; TabulaDispersa* index;
                    i32 modus; } tabula;   /* modus: implicita/explicita/punctata/inlinea */
        Xar* series;                       /* TomlValor* */
    } datum;
    b32 series_tabularum;                  /* nata per [[ ]] */
};

nomen structura {
    TomlValor* radix;
    Xar* diagnostica;          /* MateriaDiagnosticum, syntaxis + semantica */
    b32 sanum;
} TomlCoctum;

TomlCoctum toml_coquere (Piscina*, constans MateriaNodus* documentum,
                         constans TomlParsura* parsura);
```

- [ ] **Step 1: Measure** — the toml-test invalid categories (names of
  the directories) → the list of semantic rules they exercise; confirm
  each maps to one named diagnostic below or add one with a cause.

- [ ] **Step 2: Write the failing coctum gate** — every case asserts the
  cooked value AND, for errors, the diagnostic's message and location:
  - strings: escapes `\b \t \n \f \r \" \\ \uXXXX \UXXXXXXXX`; invalid
    `\x`, `\u` surrogate, `\U` above 0x10FFFF → named errors; multiline
    basic: first newline trimmed, line-ending backslash trims whitespace
    and newlines; CRLF in a multiline string → decoded per Q1's measured
    `tomllib` verdict (Review Focus 3); literal strings verbatim; control
    bytes other than TAB in any string or comment → error.
  - integers: bases, underscores only between digits, no leading zeros
    in decimal (`0` alone fine, `007` error), `+0`, `-0`,
    `-9223372036854775808` accepted, `9223372036854775808` → "integer
    extra fines s64" (Review Focus 2); hex digits of either case, prefix
    lower case only.
  - floats: `strtod` on an underscore-free copy; `1.` / `.5` / `1e` /
    `1._5` errors; `inf`/`nan` with signs; `-0.0`.
  - datetimes: every kind; month 1–12, day valid for the month incl.
    leap years (`2024-02-29` fine, `2023-02-29` error), hour ≤ 23, minute
    ≤ 59, second ≤ 60 (leap second allowed by RFC 3339), offset ≤ ±23:59,
    lower-case `t`/`z` accepted; fraction beyond nanoseconds truncated.
  - tables: dotted keys create implicit tables; `[a]` after `a.b = 1` →
    "tabula iam definita per claves punctatas"; `[a]` twice → "tabula
    iam definita"; duplicate key → "clavis iam definita" with the first
    definition as a related location; inline table closed: `a = {x = 1}`
    then `a.y = 2` → error; `[[a]]` after `a = []` → error; `[a.b]` then
    `[[a]]`, `[[a.b]]` nesting per spec examples; `3.14159 = "pi"` →
    table `3` key `14159` (Review Focus 1).
  - key order: insertion order in `claves` equals source order.
  - every-error: a file with a duplicate key AND an overflow AND a
    syntax malum → three diagnostics, sorted by offset.
  - BOM (Review Focus 4): Q1's verdict, pinned.

- [ ] **Step 3: Run and verify it fails.**

- [ ] **Step 4: Implement** — one walk over `documentum.liberi` keeping a
  *current table* pointer (reset by headers); a `par` resolves its
  dotted key through implicit tables; each rule is a function returning
  a diagnostic or NIHIL; syntax diagnostics come first from
  `materia_diagnostica_derivare(piscina, radix, &TOML_REGISTRUM, …)`
  (the declaration's `diagnosticum=`/`absentia=`), semantic ones are
  appended, then all sorted by offset.

- [ ] **Step 5: Run and verify** — green.

- [ ] **Step 6: Plant** — the duplicate-key check skipped for dotted
  keys → the `a.b = 1` / `a.b = 2` case red; revert.

- [ ] **Step 7: Commit** — `["toml/fontes/toml_coctum.h", "toml/fontes/toml_coctum.c", "toml/probationes/probatio_toml_coctum.c"]`, `["toml"]`.

**Q7a executed 2026-09-28.** `toml_valor.h` (the value model, types
only: `TOML_VALOR_*` kinds, prefixed so they don't read like
`TOML_GENUS_*`), `toml_scalaris.{h,c}`, gate `probatio_toml_scalaris.c`.
One owner for scalar diagnostics: `toml_scalaria_iudicare` walks every
token (trivia too, iteratively, skipping `malum` substance) and reports
each error at the offending byte (an escape, a field, a stray `_`), not
the whole lexeme; Q7b cooks values through `toml_scalarem_coquere` with
diagnostica NIHIL, so nothing is reported twice. Location rules: "after
X, missing" sits on X; "missing at end" is a zero-width point at the
end. Strings report EVERY error; a number or datetime reports its first.
Ten codes: `textus/octetus-moderans`, `textus/utf8`, `chorda/effugium`,
`chorda/effugium-mancum`, `chorda/punctum-codicis`, `numerus/integer`,
`numerus/extra-fines`, `numerus/fluitans`, `tempus/forma`,
`tempus/extra-limites`. Green on the first run: 15 integers, 17 floats,
20 strings, 4 quoted keys, 10 datetimes, 88 error cases (code + offset +
count), line/column inside a multiline string, malum skipped with its
trivia judged, BOM verdict pinned; corpus: 205/205 valid clean, 274
scalar-category invalid all rejected (73 syntax + 201 cooking — exactly
the measured count), house 12 clean, wild: 35 flagged, all 35 rejected
by tomllib too; the 1,676 wild files tomllib accepts are all clean.
Second 60 allowed (RFC 3339, the plan) — tomllib refuses it; named for
Q9. Plant: the leading-zero rule skipped → 4 inline + 9 corpus red;
restored green. New words: `nanosecunda` and `scalaris` (glossary
lines; the first commit was refused by the hook for `scalaria` and a
local `basica` — a tree grep had found both, but only in comments and
strings; the local became `gemina`);
`effugium-unicodex` renamed `-mancum` rather than coin a word.

**Q7b executed 2026-09-28.** Measured first: the 65 cases re-read one
by one; correction to the table above — `key/dotted-redefine-table-*`
are KIND conflicts (`a.b = 1` then `a.b.c = 2`), not dotted-key
redefinition. The model is tomllib's (the Q9 oracle, total agreement
with toml-test) moved from its key-path flag tree onto the table
objects: a table's `modus` (IMPLICITA / EXPLICITA / PUNCTATA / INLINEA)
plus `sectio` (header count at birth) for PUNCTATA stands in for
tomllib's EXPLICIT_NEST + pending flags, and FROZEN is "the path runs
through an INLINEA table or a static array" (a fresh `[[x]]` element
is a fresh table, which is tomllib's `unset_all`). Eight codes:
`clavis/iterata`, `clavis/segmentum-vacuum`, `clavis/genus-alienum`,
`tabula/iterata`, `tabula/punctata`, `tabula/extra-sectionem`,
`tabula/clausa`, `tabula-compacta/toml-1.1`; each points at the
offending key SEGMENT, with the first definition as a related location
(`TomlValor.definitio`, new; `tabula.sectio`, new). A refused header
yields an orphan table so its section's pairs are still judged without
cascades. The 1.1 newline check rides the new shared token walker
(`toml_lexemata_ambulare`, now also under `toml_scalaria_iudicare`) with
a stack of CLOSED brackets (an unclosed one is the syntax layer's).
Found during the measurement: **an unterminated string had NO
diagnostic** — the builder counted it, the declaration derives nothing;
now `chorda/aperta` (scalar layer, zero-width at the token's end; Q7a
gate +4 cases). Found in the run: `tabula_dispersa` refuses a
zero-length key (length 0 marks an empty slot), so TOML's `""` key
lives outside the index and is found by scanning `claves`. Gate
`probatio_toml_coctum.c`: 22 valid lookups, table modes and
definitions, key order, 32 structural errors (code + offset + related
offset), every-error (duplicate + overflow + malum → three, sorted),
corpora: toml-test valid 205/205 sanum, invalid **474/474** with a
diagnostic, house 12/12; wild validity equals tomllib on all 1,794
files (1,675 valid, 119 invalid, zero disagreements; probe). Plant:
the duplicate check skipped for dotted keys → inline case + 2 corpus
files red; restored green. Known for Q11: nested values are cooked
recursively (C stack depth = bracket depth).

---

## Task Q8: The toml-test oracle — tagged-JSON comparator, valid AND invalid, pinned rising

**Files:**
- Create: `toml/fontes/toml_oraculum.{h,c}`, `toml/probationes/probatio_toml_oraculum.c`

**Interfaces:**

```c
nomen structura {
    b32 aequalis;
    chorda via_differentiae;    /* "a.b[2]" ubi primum differunt */
    chorda causa;               /* "genus: integer != float" ... */
} TomlComparatio;

TomlComparatio toml_oraculum_comparare (Piscina*, constans TomlValor* coctum,
                                        constans JsonValor* exspectatum);
```

Comparison rules: objects by key set and recursively; arrays by length
and order; scalars by `type` equal AND value: integer as `s64` parsed
from the string; float by value (`nan` equals `nan`; `-0.0` and `0.0`
distinguished by the expected string's sign); datetime by parsing the
expected string with the cooking step's own datetime parser and
comparing fields (`Z` ≡ `+00:00`); string by bytes; bool by value.

- [ ] **Step 1: Write the failing gate** — for every VALID case of the
  1.0.0 list: parse, cook, `json_legere` the `.json`, compare; for every
  INVALID case: parse + cook must produce ≥ 1 diagnostic. Counts: `valida
  paria / dispara`, `invalida reiecta / accepta`. `PINNA_VALIDA` and
  `PINNA_INVALIDA` = the counts at birth (printed first, then pinned),
  rising only; `ORACULUM_OMNIA=1` prints every failure (path + causa),
  `ORACULUM_EXEMPLUM=<via>` one case in full.

- [ ] **Step 2: Run, read the failure classes, fix the cause** — a parser
  fix goes to the arbor gate first, a cooking fix to the coctum gate
  first — until both numbers reach the whole list or the remainder is
  named: each exclusion is a line in `PROVENIENTIA.md` with its cause
  and, if it is a spec reading, the spec sentence.

- [ ] **Step 3: Plant** — pin raised above the count → red; revert.

- [ ] **Step 4: Commit** — `["toml/fontes/toml_oraculum.h", "toml/fontes/toml_oraculum.c", "toml/probationes/probatio_toml_oraculum.c", "toml/probationes/fixa/toml-test/PROVENIENTIA.md"]`, `["toml"]`, message with the final counts.

**Split rule:** if Step 2 has no fixed end after the first pass, split
here (Q8a comparator + gate at birth numbers; Q8b the failure-class
loop) and tell Fran — crusta's P11 precedent.

**Executed 2026-09-28.** Measured first: `json.h` (JsonValor, objects
as ordered pairs with interned keys, "" keys canonical), toml-test's
tagged shape (8 type names; a tagged value is an object with EXACTLY
two string keys `type`/`value` — a TOML table with those keys carries
objects, so no ambiguity; `valid/key/special-word` pins `inf`/`nan`/
`true` as plain keys). `toml_oraculum.{h,c}` as specified, with one
deviation: `JsonValor*` is non-const (the house json accessors take
non-const). Missing expected key → via = the table, causa names the
key; extra cooked keys → via = the table. **At birth: valid 205/205
equal, invalid 474/474 rejected — no exclusions, Step 2's loop had
nothing to do** (Q7 had already driven the invalid half; the valid half
held). Pins 205/474. A perfect birth is a reason to ask what the oracle
forgives: 26 inline comparator cases (each difference kind with its via:
value, type, `-0.0` sign, `inf` sign, array length, element, missing key,
extra key; `Z` ≡ `+00:00`; `type`/`value` keys as a table; `""` key) and
TWO plants — the pin raised to 206 (red), and cooking `\t` as `t`
(inline case AND corpus 202/205, red) — restored green.
PROVENIENTIA.md records the result. Toml suite 9/9.

---

## Task Q9: The `tomllib` differential — the golden, both corpora

**Files:** Create `toml/probationes/probatio_toml_differentia.c`.

*Q1 note (2026-09-28): the golden is split — `fixa/tomllib/aurum.txt`
(toml-test, committed) and `toml/build/aurum_silvestre.txt` (wild corpus,
regenerated by `./toml/tomllib_aurum.sh -silvestre` from the live files,
never committed: it would carry the third-party content). The gate
regenerates the wild golden when it is absent or older than the manifest,
and skips-with-count files whose sha256 no longer matches.*

- [ ] **Step 1: Write the failing gate** — read both goldens (Q1); for
  every block: our verdict (`sanum`) versus `STATUS`; for VALIDUM blocks
  our cooked view versus the block's JSON through `toml_oraculum_comparare`.
  Counts per corpus (toml-test, wild) — `concordes / discordes`; pinned
  rising; the disagreements printed by path with both verdicts. A
  disagreement where toml-test itself sides with us (from Q1 Step 4's
  list) is a named divergence, not a failure.
- [ ] **Step 2: Run; fix causes; pin.** Wild-corpus disagreements are
  where real-world bugs live — each gets an arbor or coctum case first.
- [ ] **Step 3: Plant** — `sanum` inverted for files with zero
  diagnostics → red; revert.
- [ ] **Step 4: Commit** — `["toml/probationes/probatio_toml_differentia.c"]`, `["toml"]`.

**Executed 2026-09-28.** Measured first: the goldens (toml-test 679
blocks: 205 VALIDUM, 474 INVALIDUM; wild 1,795 blocks: 1,676 / 119,
newer than the manifest; JSON lines up to 210 KB, so the gate splits the
whole file itself) and the walker's naming (absolute wild paths; the
golden writes `~` — the gate maps `$HOME` → `~`). Pins: toml-test
agreement exactly **679** (frozen corpus); wild: **zero disagreements**
in verdict and in value (a rising count would go falsely red whenever a
Homebrew/cargo file disappears from Fran's disk). At birth: toml-test
679/679; wild 1,793/1,794 with ONE value disagreement —
`test_tomllib/.../localtime.toml`, `t=00:00:00.99999999999999`: tomllib
keeps microseconds (`.999999`, Python datetime), we keep nanoseconds;
TOML 1.0 requires truncation to the implementation's precision, so both
are right. Named BY CAUSE, not by path (the path carries the Python
version): after a value mismatch the gate truncates our datetimes to
microseconds and compares again; equal → counted as the named divergence
"microsecunda". The comparator itself forgives nothing. Runner: the wild
golden is regenerated when absent or older than the manifest (tested by
moving it aside: regenerated identical in ~1 s); a failed regeneration
is a loud CAUTIO and a red gate. Plant (the plan's): `sanum` inverted →
toml-test 208/679, wild 61/1,794, red; restored green. The second-60
question stays theoretical: no file in either corpus has one. Suite
10/10.

---

## Task Q10: The public API and diagnostics rendering — `toml.h`

**Files:**
- Create: `toml/fontes/toml.h`, `toml/fontes/toml.c`, `toml/probationes/probatio_toml_api.c`

**Interfaces (`toml.h`):**

```c
nomen structura TomlDocumentum TomlDocumentum;

TomlDocumentum*     toml_legere (chorda textus, constans character* via, Piscina*);
                    /* numquam NIHIL (nisi memoria deficit) */
b32                 toml_successus (constans TomlDocumentum*);
Xar*                toml_diagnostica (constans TomlDocumentum*);   /* MateriaDiagnosticum */
chorda              toml_diagnostica_scribere (Piscina*, constans TomlDocumentum*, b32 excerptum);
                    /* omnia per materia_pictor_scribere, ordine octetorum */
constans TomlValor* toml_radix (constans TomlDocumentum*);
constans TomlValor* toml_quaerere (constans TomlDocumentum*, constans character* via_clavium);
b32 toml_chorda   (constans TomlDocumentum*, constans character* via_clavium, chorda* exitus);
b32 toml_integer  (constans TomlDocumentum*, constans character* via_clavium, s64* exitus);
b32 toml_fluitans (constans TomlDocumentum*, constans character* via_clavium, f64* exitus);
b32 toml_boolean  (constans TomlDocumentum*, constans character* via_clavium, b32* exitus);
b32 toml_tempus   (constans TomlDocumentum*, constans character* via_clavium, TomlTempus* exitus);
i32                 toml_tabulae_numerus (constans TomlValor* tabula);
chorda              toml_tabulae_clavis  (constans TomlValor* tabula, i32 index);
constans TomlValor* toml_tabulae_valor   (constans TomlValor* tabula, i32 index);
i32                 toml_seriei_numerus  (constans TomlValor* series);
constans TomlValor* toml_seriei_elementum (constans TomlValor* series, i32 index);
```

`via_clavium` is lexed by `toml_lector` in CLAVIS mode (T6): a
malformed path, an empty path, a path through an array or into a
non-table → NIHIL / FALSUM.

- [ ] **Step 1: Write the failing API gate** — the tester's
  feature-requests/013 verbatim as cases: `[llama-server]\nversio =
  "b11201"\n[poppler]\nversio = "26.09.0"\n` → `toml_chorda(doc,
  "llama-server.versio", &c)` = `b11201`, `"poppler.versio"` =
  `26.09.0`; enumerate root → keys `llama-server`, `poppler` in order;
  the same two sections with `versio` twice in ONE section → FALSUM
  success and the rendered text contains `clavis iam definita` with two
  carets. Typed getters: present+right type VERUM; absent FALSUM;
  present+wrong type FALSUM; `s64` extremes round-trip. Review Focus 5:
  `""`, `"a..b"`, `"arr.x"` where `arr` is an array, `"x.y"` where `x`
  is a string, `'a."b.c"'` reaching a key with a dot. Rendering: a file
  with three errors renders three blocks in offset order, each with
  `via:linea:columna` and a caret.
- [ ] **Step 2: Run, red; implement `toml.c`** (parse Q4 → cook Q7 →
  wrap; path lookup by the lector; rendering by `materia_pictor_scribere`
  per diagnostic).
- [ ] **Step 3: Run, green. Plant** — the path lexer treating a quoted
  segment as bare → `a."b.c"` red; revert.
- [ ] **Step 4: Commit** — `["toml/fontes/toml.h", "toml/fontes/toml.c", "toml/probationes/probatio_toml_api.c"]`, `["toml"]`, citing lapide feature-requests/013.

**Executed 2026-09-28.** Measured first: the tester's request read
verbatim (lapide feature-requests/013: dotted access, list tables,
refuse duplicate keys); `materia_pictor_scribere`'s contract (one
block per diagnostic, related locations in the same block); and a
**name collision**: `include/toml.h` (old `lib/toml.c`) declares
`toml_legere`, `TomlValor`, `TomlDocumentum` and is first on the include
path. Decision (the approved plan's names stand; Q12 deletes the old):
`toml/fontes/toml.h`, the toml runner puts `toml/fontes` BEFORE
`include`, and the two are never linked into one binary. Found while
checking: the derived include graph (`silva/nexus.sh`) resolves
`toml/fontes/toml.c`'s `"toml.h"` to `include/toml.h` — it ignores the
includer's directory, against C's quote-include rule; filed (ledger
…VF3). The runner's own header guard scans `toml/fontes`, so builds are
not stale; the gap closes when Q12 deletes the old header. Built as
specified; `toml_tabulae_filius` (lookup incl. the `""` key outside
the hash index) moved into `toml_coctum` as ONE public function (the
cooker, the oracle and the API each had a copy). Paths are lexed by
`toml_lector` in CLAVIS mode, segments decoded by `toml_chordam_coquere`
— quoted segments with dots and escapes work, anything else is NIHIL.
Gate 86 assertions: 013 verbatim (two sections, dotted lookups, root
enumeration in order; `versio` twice in one section → not successful,
"clavis 'versio' iam definita" at `config.toml:3:1` with two carets —
first definition labelled — and the first value still queryable);
typed getters (right/wrong/absent type, output untouched on FALSUM,
`s64` extremes); 15 bad paths (empty, blanks, `a..b`, through an array,
into a string, trailing/leading dot, junk, unterminated quote, bad
escape, `tab = 1`, NIHIL doc/path) and 4 good ones (`tab."b.c"`,
`tab.'b.c'`, `tab."k\u0041"`, ` tab . kA `); enumeration bounds;
rendering of three errors in offset order with `via:line:col` and
carets (and none without excerpts); the empty file. Plant: quoted
segment taken raw → the four quoted lookups red; restored. Suite 11/11.

---

## Task Q11: Totality and the computus twin

**Files:**
- Create: `toml/probationes/probatio_toml_totalitas.c`,
  `toml/fontes/toml_computus.{h,c}`, `toml/instrumenta/computus.c`,
  `toml/instrumenta/arbor.c`, `toml/computus.sh`, `toml/arbor.sh`,
  `toml/probationes/probatio_toml_computus.c`,
  `toml/probationes/fixa/computus/basis.tsv`
- Modify: `pythonica/silva.py` (mensor `praef` `'toml': 'toml.'`,
  `metiri` `.toml` → `toml/computus.sh`), `toml/CLAUDE.md`

- [ ] **Step 1: Totalitas — failing gate** — transcribe crusta's: random
  bytes, 1/40 mutation, truncation steps over the Q4 corpus, under
  `CREDO_NON_RUIT`, the failing source written to
  `toml/build/totalitas_fractum.toml`. Depth: `[` × 100 000 and `{a=` ×
  100 000, open and closed, PARSE alone → `CREDO_NON_RUIT` (iterative
  builder); parse + emit, STML write, and cooking at the same depth →
  measured and pinned `NON_RUIT` or `RUIT_CUM(SIGSEGV)` with the depth
  named (crusta P9's rule; the cooking walk must be iterative or its
  limit named). NUL bytes in a string and a comment: byte-exact, named
  refusals only.
- [ ] **Step 2: Computus — failing gate** — transcribe crusta's P10:
  `toml_computus_metiri(fons, mensura, &exitus)` columns (nodes, parse
  ms, emit, STML bytes, arena), golden `basis.tsv` over three fixtures
  (a spec example, the largest wild file, one invalid case),
  `COMPUTUS_SCRIBERE=1` regenerates with a cause.
- [ ] **Step 3: Implement what red names; register in pythonica.**
- [ ] **Step 4: Plants** — cooking made recursive for arrays → depth pin
  red; one `nodi` in `basis.tsv` edited → red; revert both.
- [ ] **Step 5: Commit** — the files above, `["toml", "pythonica"]`.

---

## Task Q12: Migration — the three tools, aedilis, retire `lib/toml.c`

**Files:**
- Modify: `tools/capsula_generare.c`, `tools/capsula_caudae_adiungere.c`,
  `tools/librarium_merge.c`, `aedilis.stml` (+ `toml/fontes`), the
  generated `*fontes_generata.sh` snippets that aedilis rewrites
- Delete: `lib/toml.c`, `include/toml.h`, `probationes/probatio_toml.c`
  (its cases re-homed in Q10's gate first)

- [ ] **Step 1: Measure** — `./silva/nexus.sh` every old `toml_*` name:
  the complete call list; `git grep '"toml.h"'` for includes; which
  cases of `probatio_toml.c` Q10's gate does not cover yet (add them
  there first). Then the §VIII risk: find `librarium_merge`'s real
  inputs (its docs/worklog name where replies live), extract every TOML
  block with its own `_extrahere_toml`, parse each with the new
  library, count accepted / rejected, print the first ten rejections
  with diagnostics. Show Fran the numbers before migrating that tool;
  if rejections are real LLM sloppiness, the tool's existing skip path
  stands and the count goes in the commit message.
- [ ] **Step 2: Migrate** — old → new: `toml_legere(t, p)` →
  `toml_legere(t, via, p)`; `toml_successus`/`toml_error` →
  `toml_successus` + `toml_diagnostica_scribere` (the tools now print
  every error with location); `toml_capere_chorda(d, k)` →
  `toml_chorda(d, k, &c)` with the FALSUM branch explicit (T7: a missing
  key is now visible — each call site states its default);
  `toml_capere_tabulatum` → `toml_quaerere` + `toml_seriei_*`;
  `toml_capere_boolean` → `toml_boolean`. `librarium_merge`'s case
  fallbacks (`Tags`/`tags`) stay as two lookups.
- [ ] **Step 3: aedilis** — add `<via (>toml/fontes` to `aedilis.stml`
  inclusa; regenerate the snippets (their `# regeneratio:` commands);
  `./tools/portae_debitae.sh` on the tools must now list `toml` in their
  closure; the aedilis gate proves it.
- [ ] **Step 4: Delete** `lib/toml.c`, `include/toml.h`,
  `probationes/probatio_toml.c`; `./silva/nexus.sh toml_capere_chorda` →
  zero uses.
- [ ] **Step 5: Run** — the tools' own tests (`./compile_tests.sh capsula`,
  the librarium test if any), `radix`, `aedilis`, `generata`.
- [ ] **Step 6: Plant** — remove `toml/fontes` from `aedilis.stml` →
  the aedilis gate red naming the missing source; revert and REGENERATE
  (the plant ran a regenerating gate).
- [ ] **Step 7: Commit** — explicit paths incl. the deletions,
  `["radix", "aedilis", "generata", "toml"]`.

---

## Task Q13: briar exposure — widen the corpus to client roots, the changelog

**Decided 2026-09-28 (Fran, before execution): widen briar's corpus**,
not an amalgam. Fran: briar "at some point will want access to all the
materia clients". The deciding argument: amalgams do not COMPOSE — each
client amalgam carries its own copy of materia, so a `.thistle` using
two amalgamated clients links `materia_*` twice; a shared corpus holds
materia once. The mechanism is general (a list of client roots); THIS
plan registers only `materia/fontes` + `toml/fontes`; every other
client is a one-line addition, after its own ambiguity check, when a
`.thistle` needs it. The amalgam returns only if Step 1 measures a
blocker, shown to Fran.

**Files:** `tools/corpus_infixum.sh` (corpus list + symbol table
sources), the briar derivation/compile path that maps a symbol to its
implementation file (found in Step 1), `briar/MUTATIONES.md`.

- [ ] **Step 1: Measure** — briar's corpus today is `lib/*.c`,
  `include/*.h` and `vendor/*` (`tools/corpus_infixum.sh:80`); header
  derivation maps a used symbol to the `include/*.h` that defines it,
  and the implementation lookup assumes `lib/*.c`. Measure: (1)
  **derivation ambiguity** — every symbol defined in a header under
  `materia/fontes` or `toml/fontes` that is ALSO defined in another
  corpus header (the round-12 `Liber` class) — expected zero, each hit
  named; (2) **binary growth** — briar's size before/after with the two
  roots embedded; (3) **the implementation lookup** — where briar turns
  a symbol into the `.c` it compiles, and what it needs to learn client
  roots (the real code change); (4) the include paths briar passes to
  clang for a client header.
- [ ] **Step 2: Implement** — client roots as ONE list (materia first,
  then toml) read by `corpus_infixum.sh` and the lookup; `briar -bibliothecae`
  lists `toml`; `briar -bibliotheca toml -functiones` prints the Q10
  signatures; a `.thistle` fixture using `toml_chorda(doc,
  "a.b", &c)` builds and runs (briar fixture test).
- [ ] **Step 3: Changelog** — `## inedita` line marked **FRANGIT**:
  `toml` API new (old calls → new, one line each), `[tables]`, dotted
  lookup, enumeration, duplicate keys refused, every error located —
  lapide feature-requests/013.
- [ ] **Step 4: Rebake BEFORE committing** (`briar/compile_probationes.sh`,
  `tools/briar_struere.sh`, `tools/briar_spectator_struere.sh`), gates
  `briar`, `briar-fumus`; commit; rebake again for the stamp.

---

## Task Q14: Closure — records, ledger, memory

**Files:** `toml/CLAUDE.md` (full orientation in crusta's shape),
`project-specs/toml-arbor-spec.md` (§X "As built": deltas, measured
answers to AUDIENDA), this plan (STATUS: CLOSED), `toml/fontes/
toml_arbor.worklog.md` (opened at the first find during Q3–Q13),
`materia/phase-log.md` (INTENTIO written at Q1, RELATIO here);
`./materia/shim_probare.sh` run and counted.

- [ ] **Step 1:** `toml/CLAUDE.md` — status, the gates and what each
  asserts, oracle numbers and pins, laws (T3–T8 one line each), birth
  lessons, Currere.
- [ ] **Step 2:** spec "As built"; phase-log RELATIO (numbers, substrate
  changes — expected none —, shim count).
- [ ] **Step 3:** Ledger — desideratum …TXF → `impletum` with numbers and
  commits; any tail (oracle exclusions, 1.1) as ONE desideratum.
- [ ] **Step 4:** Memory — a `toml-client.md` topic file (reflexes: two
  modes, the number-like token, recovery at the newline, the three
  oracles and where they live), `MEMORY.md` line, `materia-fork.md`
  gains the seventh client.
- [ ] **Step 5: Commit** — the files above, `[]`.

---

## Self-Review (2026-09-28, before execution)

**Spec coverage.** §0 T1 → Q1/Q8 (1.0.0 list); T2 → Q10/Q12; T3 → Q3/Q4;
T4 → Q4 (recovery) and Q7/Q10 (every error reported and rendered); T5 →
Q7; T6/T7 → Q10; T8 → Q7 (`nodus`, insertion order); T9 → Q1/Q8/Q9/Q4
(byte law over the wild corpus); T10 → nothing built (named futures);
T11 → Q12 (aedilis) and Q13 (briar). §I census → Q1/Q12 Step 1. §II
files → Q2–Q11. §III lexer → Q3 (incl. UTF-8, control bytes, CRLF, the
datetime space). §IV tree + laws → Q2/Q4/Q5/Q6/Q11. §V cooked + API →
Q7/Q10. §VI diagnostics → Q7 (derivation + semantic) and Q10
(rendering). §VII oracles → Q1/Q8/Q9, gates born red in every task. §VIII
migration → Q12/Q13 incl. the librarium risk. AUDIENDA → Q1 Steps 1/3/4
(tag, corpus size, oracle agreement), Q7 (strtod via the oracle), Q12
Step 1 (librarium), Q14 (substrate changes).

**Placeholder scan.** No TBD/TODO. One decision is explicitly Fran's at
measurement time (Q1 corpus storage if > 20 MB), with both options and the
numbers to gather; Q13's mechanism was decided before execution (widen the
corpus), with the amalgam kept only as a measured-blocker fallback. Commit messages in Q4–Q13 are
named by content; the exact Latin is written at commit time from what was
measured (counts change the text).

**Type consistency.** `TomlParsura` (Q4) is consumed by `toml_coquere`
(Q7); `TomlValor`/`TomlTempus`/`TomlGenusValoris` (Q7) are the types of
Q8's comparator and Q10's API; `TomlLector`/`TomlModus` (Q3) serve Q4 and
Q10's path lexing; `TomlComparatio` (Q8) is reused by Q9.

**Review Focus.** The five lines above each have a test in their owning
task: 1 → Q3 + Q7; 2 → Q7; 3 → Q3 + Q4 + Q7 (verdict from Q1); 4 → Q3 +
Q4 + Q7 (verdict from Q1); 5 → Q10.
