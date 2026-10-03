# toml — TOML 1.0 as a materia client (seventh)

**Status: PLAN CLOSED 2026-09-28** (plan `project-specs/toml-arbor-plan.md`,
Q1–Q14 in one day; Q7 and Q11 re-sliced by their measurements; every
task's "Executed" note is the build history). Spec:
`project-specs/toml-arbor-spec.md` (decisions T1–T11; **§X = as built,
governs where the body differs**). Phase-log: `materia/phase-log.md`,
2026-09-28 toml entries. Desideratum …TXF fulfilled; the tail is one
desideratum (TOML 1.1, writing). Substrate changes: **none**.

A total, byte-exact TOML tree on materia, a cooked view of typed values,
every error located; proven by three oracles. Replaces `lib/toml.c`
(retired in Q12).

## Oracles (Q1)

- `probationes/fixa/toml-test/` — toml-test v2.2.0 (MIT), frozen;
  `PROVENIENTIA.md` has tag, commit, counts (1.0.0: 205 valid, 474
  invalid) and the measured `tomllib` agreement (total).
- `instrumenta/tomllib_tagatum.py` — Python `tomllib` → toml-test tagged
  JSON; `./toml/tomllib_aurum.sh` writes `probationes/fixa/tomllib/aurum.txt`
  (committed, toml-test) and `build/aurum_silvestre.txt` (wild, NOT
  committed).
- `probationes/fixa/silvestria.manifestum` — the wild corpus as sha256 +
  path only (Fran 2026-09-28: no third-party files in the repo); 1,795
  unique files from `~/.cargo`, `/opt/homebrew`, `~/Documents/projects`.

## The registry is generated (Q2)

`TomlGenus`, `TOML_GENUS_NUMERUS_GENERUM`, `TOML_REGISTRUM`,
`TOML_DIAGNOSTICA` come from `grammatica/toml.registrum.stml` via
`./materia/coquere.sh toml/grammatica/toml.registrum.stml -scribere`
(14 genera, 25 loci). Slot enums by hand in `fontes/toml_registrum.h`;
the lexicon (28 token genera, prefix `toml-`) in `fontes/toml_lexicon.{h,c}`.
To add a genus or locus: append to the declaration, `-scribere`, add
the slot enum, add its row to `LOCI_NOMINATI` in
`probatio_toml_registrum.c`. Node sizes from
`TOML_REGISTRUM.genera[g].loci_numerus`, never a hand count.

Names: `"…"` strings and keys are GEMINA, `'…'` SIMPLEX (crusta's
words); an inline table is `tabula-compacta` (`brevis` and `interior`
are latina.h macros).

## The lector (Q3)

`fontes/toml_lector.{h,c}` — a function of (mode, position): CLAVIS or
VALOR, chosen by the builder; every byte a token; newline is
`LINEA_FINIS` at depth 0 and `LINEA` inside brackets (the builder sets
the depth). One greedy number-like token classified NUMERUS / TEMPUS /
VERUM / FALSUM / IGNOTUM; a single space joins a full date and a time.
Bytes inside comments and strings are never judged here (cooking does);
outside them anything unrecognized is one IGNOTUM token.

## The builder (Q4)

`fontes/toml_arbor.{h,c}` — iterative (frame stack), total: every byte
emits back exactly (corpus gate: toml-test 679/679, house 12/12, wild
1,794/1,794). Laws: newline terminates only an open statement; trivia
after a terminating newline bind FORWARD, otherwise POST through the
first newline; a bad token becomes a `malum` that recovers at the next
newline (outside brackets) or ',' / closer (inside); `TomlParsura` counts
mala, missing closers (incl. unterminated strings) and absentiae. Finds
at find-time: `fontes/toml_arbor.worklog.md`.

## STML (Q5)

Every document round-trips through STML twice with byte-equal text, the
comparator STRUCTURALIS and FIDELITAS, the position map verified, and the
re-read tree emitting the source — 41 inline cases and all 2,485 corpus
files. STML ≈ 16× the source. The corpus walk (toml-test, house, wild
with sha256 check) is shared by the gates: `probationes/toml_corpus_ambulare`.

## Canon (Q6)

`grammatica/toml.canon` — hand-written, species MEASURED over every
document; 70 rules; seal `b7568363` pinned (move it by hand, with a
cause, when the declaration changes — the gate prints the live seal).
Drift guard both ways in `probatio_toml_canon`. Manual judgment:
`bin/canon_examen -canon toml/grammatica/toml.canon <doc.stml>`.

## Scalars (Q7a)

`fontes/toml_valor.h` is the value model (types only). `fontes/
toml_scalaris.{h,c}` decodes strings (and quoted keys), integers
(`s64`, overflow named), floats, booleans, datetimes (`toml_tempus_legere`
is pure — the oracle reuses it), and judges comment bytes.
`toml_scalaria_iudicare(piscina, radix)` is the ONE emitter of scalar
diagnostics (every token incl. trivia, source order, each at the bad
byte); cooking elsewhere passes diagnostica NIHIL. Verdicts: CRLF inside
a multiline string decodes to `\n` (tomllib normalizes CRLF
everywhere); a lone CR is a control byte; second 60 is allowed (tomllib
refuses it). Unterminated strings are reported here (`chorda/aperta`).
`toml_lexemata_ambulare` is the shared in-order token walk.

## Cooked view (Q7b)

`fontes/toml_coctum.{h,c}`: `toml_coquere(piscina, radix, &parsura)` →
`TomlCoctum {radix, diagnostica, sanum}`. Diagnostics = syntax (derived
from the declaration) + scalar + structural, sorted by offset (stable).
Table rules follow tomllib, carried on the table objects: `modus`
IMPLICITA/EXPLICITA/PUNCTATA/INLINEA + `sectio` for dotted tables;
INLINEA tables and static arrays are closed; every structural error
names the key segment and relates the first definition
(`TomlValor.definitio`). The empty key `""` is kept outside the hash
index (`tabula_dispersa` refuses length 0). Wild validity equals
tomllib's on every file.

## The toml-test oracle (Q8)

`fontes/toml_oraculum.{h,c}`: `toml_oraculum_comparare(piscina, coctum,
json)` → `{aequalis, via_differentiae "a.b[2]", causa}` against
toml-test's tagged JSON (the same shape the tomllib golden uses).
`probatio_toml_oraculum`: valid 205/205 equal, invalid 474/474 rejected,
pinned. `ORACULUM_OMNIA=1` prints every failure, `ORACULUM_EXEMPLUM=
valid/…toml` one case with its diagnostics.

## The tomllib differential (Q9)

`probatio_toml_differentia`: our verdict and cooked values against the
tomllib goldens — toml-test 679/679 (pinned exactly), wild corpus zero
disagreements (1,793 agree + 1 named divergence). Divergences are named
by CAUSE: "microsecunda" (tomllib truncates fractions to 6 digits, we
to 9; both follow TOML 1.0). `build/aurum_silvestre.txt` (date-free
header, deterministic) and the house corpus index `build/toml_corpus.lst`
are fabrica actions in `toml/aedificatio.stml` (`toml_aurum_silvestre`,
`toml_corpus`; fabrica spec 3 T4): the runner asks `bin/fabrica sanare`
for them when run by hand (falls back to the scripts without bin/fabrica),
and does nothing under `FABRICA_LECTIONES` (fabrica realized them).

## The public API (Q10)

`fontes/toml.h` / `toml.c` — **use this, not the layers below**:
`toml_legere(textus, via, piscina)` (never NIHIL; the text must outlive
the document), `toml_successus`, `toml_diagnostica`,
`toml_diagnostica_scribere` (materia_pictor blocks: `via:line:col`,
excerpt, carets, first definition labelled), `toml_quaerere(doc,
"a.b")` and typed getters `toml_chorda/integer/fluitans/boolean/tempus`
(FALSUM leaves the output untouched), enumeration `toml_tabulae_*`,
`toml_seriei_*`. Paths are TOML keys lexed by the lector: `a."b.c"`,
escapes and spaces work; empty, malformed, through an array or into a
scalar → NIHIL. The old `lib/toml.c` / `include/toml.h` were retired in Q12.

## Consumers (Q12)

`tools/capsula_generare.c`, `tools/capsula_caudae_adiungere.c` (strict:
explicit defaults, errors printed with location) and
`tools/librarium_merge.c` (LLM output: a named lenient lookup,
`_capere_planum`, reproduces the old flattening — the library itself is
strict). Because `capsula_generare` is part of every silex/briar
project, silex resolves headers in client roots
(`SILEX_RADICES_CLIENTIUM` in `lib/silex.c`: `materia/fontes`,
`toml/fontes`) and briar's corpus embeds them
(`tools/corpus_infixum.sh`). Adding a file to `toml/fontes` changes
what every generated project vendors.

## Totality and the twin (Q11)

`probatio_toml_totalitas` (6,136 generated cases: random bytes,
mutation, truncation, CRLF, nesting, NUL): never crashes, emission ==
source, a non-sana parse carries a diagnostic, cooking with and without
the parsura agrees. Depth 100 000 is linear: the builder keeps a
running bracket count (`unci`, changed only in `_impellere` /
`_depellere`), cooking is a work stack (`Opus`), and a clean parse
skips materia's quadratic derivation. materia's own limits (emission
and STML at 100 000) are pinned `RUIT_CUM`, park …FAD8 — not ours to
fix. `toml_computus` is the benchmark twin (crusta P10 shape), golden
`probationes/fixa/computus/basis.tsv` over three frozen toml-test files.

## briar (Q13)

A `.thistle` uses `toml.h` with no `#include` — derived like any house
header. The client roots are ONE list in two homes:
`SILEX_RADICES_CLIENTIUM` (public, `include/silex.h`) and
`RADICES_CLIENTIUM` (`tools/corpus_infixum.sh`); `probatio_silex`
fails if they drift. `-bibliothecae`, `-bibliotheca toml -functiones`
and `-amalgama` search `include/` then the roots. Fixture
`briar/probationes/fixa/thistle/toml_config.thistle` (fabrica test,
fumus XXI). **A new file-local type in `toml/fontes` must not share a
name with any PUBLIC house name** — the amalgam skips such names when
renaming privates and they collide (`Gradus`, found in Q13).

## Gates

| gate | asserts |
|---|---|
| `registrum` | generated tables == declaration; every locus named |
| `lector` | modes, one token per byte, number-like classification |
| `arbor` | builder laws, recovery, `TomlParsura` counts |
| `corpus` | byte law on toml-test 679, house 12, wild 1,794 (sha256 checked) |
| `stml` | two write-read cycles byte-equal, STRUCTURALIS + FIDELITAS, positions |
| `canon` | 70 rules over every document, seal `b7568363`, drift both ways |
| `scalaris` | decoding and every scalar diagnostic at its byte |
| `coctum` | table modes, 8 structural codes with first definition, order |
| `oraculum` | toml-test: 205/205 valid equal, 474/474 invalid rejected (pinned) |
| `differentia` | tomllib: 679/679; wild zero disagreements, 1 divergence named |
| `api` | `toml.h` incl. the 16 re-homed cases of the old library |
| `totalitas` | never crash, emission == source, depth 100 000 in time |
| `computus` | twin columns against `basis.tsv` |

13 suites, 14,267 assertions (2026-09-28). The root `silex` and briar
gates (`briar`, `briar-fumus`) also cover the client since Q12/Q13;
`portae_debitae` owes `toml` for any `toml/fontes` change.

## Laws (spec §0, as built)

- T3 — the lector is a function of (mode CLAVIS/VALOR, position) plus
  two switches the builder sets (depth; open statement); nothing is
  relabelled afterwards.
- T4 — every byte lands in the tree; `malum` recovers at the newline
  (outside brackets) or `,`/closer (inside); every error is reported.
- T5 — semantic rules live in cooking; one owner per diagnostic class
  (scalar: `toml_scalaria_iudicare`; structure: `toml_coctum`; syntax:
  derived, skipped when sana).
- T6 — lookup paths are TOML keys lexed by the lector.
- T7 — getters return VERUM only if present AND of that type; FALSUM
  leaves the output untouched.
- T8 — tables keep insertion order; every cooked value points at its
  node (`TomlValor.nodus`, `.definitio` for the first definition).

## Birth lessons

- Count after fetching: a web summary said toml-test had 256/512
  cases; the fetched list had 205/474.
- Third-party corpora live as a MANIFEST (sha256 + path), and so does
  the golden derived from them — a JSON golden would smuggle the
  content back in.
- "Iterative" is not "linear": measure time at depth 100 000, not only
  survival (the builder walked the frame stack per token).
- A gate that COUNTS hides a missing diagnostic; assert "carries a
  diagnostic", not "not sana" (unterminated strings had none).
- `tabula_dispersa` refuses a zero-length key; TOML's `""` lives
  outside the index.
- A gate that stays green under a plant is mute for that change
  (`aedilis` never saw `toml/fontes`; `generata` did).
- Retiring a library means finding every place that VENDORS it: silex
  projects and briar's corpus carried `lib/toml.c`.
- An `Xar` is segmented: never hand `xar_obtinere(x, 0)` on as a flat
  array (briar's suggestions, Q13).

## Currere

```
./toml/compile_probationes.sh              # omnes
./toml/compile_probationes.sh registrum    # filtrum substringae
./toml/tomllib_aurum.sh [-silvestre]       # aurea tomllib (Q1)
./toml/computus.sh <f.toml> [-machina]     # mensura (Q11b)
./toml/arbor.sh <f.toml> [-sedes]          # STML (Q11b)
```

0 sanum / 1 fractae / **2 = NULLA CURSA**. Log: `build/test_logs/toml.log`.
Gate `toml` in pythonica (`silva.porta('toml')`).
