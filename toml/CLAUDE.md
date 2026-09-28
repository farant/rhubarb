# toml — TOML 1.0 as a materia client (seventh)

**Status: building** (plan `project-specs/toml-arbor-plan.md`, Q1–Q10 + Q11a done;
Q7 was re-sliced: Q7a scalars, Q7b tables; Q8 oracle, Q9 tomllib, Q10 API, Q11a totality done).
Spec: `project-specs/toml-arbor-spec.md` (decisions T1–T11).

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
to 9; both follow TOML 1.0). The runner regenerates
`build/aurum_silvestre.txt` when absent or older than the manifest.

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
scalar → NIHIL. TRANSITION until Q12: `include/toml.h` (old lib) has
the same name — the runner puts `toml/fontes` first; never link both.

## Totality (Q11a)

`probatio_toml_totalitas`: every generated input (random bytes, 1/40
mutation, truncation over toml-test valid + house, CRLF conversions,
nests of five shapes × open/closed × 1..1000) never crashes, always
emits back byte for byte, an unclean parse always carries a diagnostic,
and cooking with/without the parse report yields the same diagnostics.
Depth 100 000: parse and cooking are linear and live (a running bracket
count; an iterative work stack). materia's limits are PINNED as
`RUIT_CUM(SIGSEGV)` — emission and STML projection at 100 000 (park
…FAD8); promote to NON_RUIT when materia is fixed. `toml_coquere` skips
materia's quadratic derivation when the parse is clean. A failing input
is written to `build/totalitas_fractum.toml`.

## Currere

```
./toml/compile_probationes.sh              # omnes
./toml/compile_probationes.sh registrum    # filtrum substringae
./toml/tomllib_aurum.sh [-silvestre]       # aurea tomllib (Q1)
```

0 sanum / 1 fractae / **2 = NULLA CURSA**. Log: `build/test_logs/toml.log`.
Gate `toml` in pythonica (`silva.porta('toml')`).
