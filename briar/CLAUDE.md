# briar — orientation

*You are reading this because you touched a file in `briar/`.*

briar reads `.thistle` files (literate C89 scripts: markdown prose,
STML config tags, raw `<c!>` regions, a `#!` line). Plan 1 built the
PARSER as a materia client; plan 2 builds the fabrica and the binary.
Spec: `project-specs/briar-spec.md`; plans `briar-plan-1-parsator.md`
(done), `briar-plan-2-fabrica.md` (next). Findings: `fontes/briar.worklog.md`.

## Map
- `briar_registrum` (5 genera, 14 loci) · `briar_lexicon` (8 token
  genera, prefix `briar-`, no LINEA: newlines live inside values)
- `briar_lexema`: line table, column-0 classification (tag open/close,
  `!` raw, `\` dedent, `/>` self-closing), markdown FENCE state (inside a
  fence every line is prose), token factory across lines
- `briar_arbor`: regions → tree (documentum, interpres, prosa, regio,
  elementum); always a tree; `briar_emittere` == source bytes
- `briar_stml`: projection consilium + origin hook; `briar/grammatica/
  briar.canon` (load by path; seal pinned)
- `briar_nexus`: inner trees by part identity — md for prose and
  `<md!>`, stml for elements and for raw open tags (attributes
  `methodus=`, `munus=`); `.thistle` line offsets and translated errors
- `briar_computus`: bench twin; instruments `./briar/arbor.sh`,
  `./briar/computus.sh`

## Gates (`./briar/compile_probationes.sh [filter]`; exit 2 = NOTHING RAN)
registrum · lexema · arbor · stml · canon · totalitas · computus · nexus
(eight files; every one born red by a planted fault — see the worklog).
Fixtures: `probationes/fixa/thistle/` (+ `adversa/`), inventory in
`fixa/FONTES.md`; computus golden `fixa/computus/basis.tsv`
(`COMPUTUS_SCRIBERE=1` + a named cause).

## Laws
- Column 0 is the only place briar looks. Prose needs no escaping.
- A raw region ends only at a column-0 `</name>`; an STML element at
  its column-0 close or its own self-closing line.
- Every byte belongs to exactly one token; derived tokens (fons 1) hold
  none. Emission is byte-exact by construction.
- Never a second grammar for attributes: the open tag is rewritten to
  `<name attrs/>` and given to stml.
- Worktree rules (until merged): `./silva/scribe.sh` not `~/.bin/scribe`;
  no ledger residents; commit with `silva.commissio_umbra(..., ['briar'])`;
  run the oratio suite once before a commit that touches pythonica.
