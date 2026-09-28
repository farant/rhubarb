# toml — TOML 1.0 as a materia client (seventh)

**Status: building** (plan `project-specs/toml-arbor-plan.md`, Q1–Q5 done).
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

## Currere

```
./toml/compile_probationes.sh              # omnes
./toml/compile_probationes.sh registrum    # filtrum substringae
./toml/tomllib_aurum.sh [-silvestre]       # aurea tomllib (Q1)
```

0 sanum / 1 fractae / **2 = NULLA CURSA**. Log: `build/test_logs/toml.log`.
Gate `toml` in pythonica (`silva.porta('toml')`).
