# briar — orientation

*You are reading this because you touched a file in `briar/`.*

briar reads `.thistle` files (literate C89 scripts: markdown prose,
STML config tags, raw `<c!>` regions, a `#!` line). Plan 1 built the
PARSER as a materia client; plan 2 the silva inner kind and the
headless FABRICA (thistle → silex project directory, no clang in the
suite); plan 3 builds the binary. Spec: `project-specs/briar-spec.md`
(v1.2); plans `briar-plan-1-parsator.md` (done),
`briar-plan-2-fabrica.md` (done), `briar-plan-3-binarium.md` (next).
Findings: `fontes/briar.worklog.md`.

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
- `briar_silva`: the FOURTH inner kind, its own unit (the silva amalgam
  header and `stml.h` cannot share a translation unit — `briar_nexus.h`
  includes neither, it forward-declares the tags): every `<c!>` region
  parsed by silva WITH expansion, prelude `#include "latina.h"` (+
  `internuntius.h` + `briar_tractator_exemplar` for `methodus=`),
  headers by TEXT from the silex corpus, never disk; result in
  `res->silva` (`BriarSilva {piscina, parsura, semantica}`,
  `briar_silvam_solvere` frees); `briar_nexus_linea_silvae` maps lines
- `briar_fabrica`: inventory → main rule → unit partition through
  silva's tree + symbol table → generated files → closure → silex
  scripts → SHA-256 key; `briar_fabricam_scribere` puts a project on
  disk (`briar_directoria_creare` walks paths; scripts chmod 0755).
  Partition: `include/<t>_regiones.h` = guard + latina.h + implicit
  stdio/stdlib/string + every region directive + type units +
  prototypes; `fontes/<t>_regiones.c` = objects + definitions; the
  `principale` unit alone in `fontes/<t>.c`; the probatio region
  verbatim in `probationes/probatio_<t>.c`; all `#line`-mapped
- `briar_computus`: bench twin; instruments `./briar/arbor.sh`,
  `./briar/computus.sh`, **`./briar/fabrica.sh <x.thistle> <dir>`**
  (then `cd <dir> && ./aedificare.sh && ./probare.sh` — the only clang
  run there is, by hand; a clang error names the `.thistle` line)

## Gates (`./briar/compile_probationes.sh [filter]`; exit 2 = NOTHING RAN)
registrum · lexema · arbor · stml · canon · totalitas · computus · nexus
· silva · fabrica (ten files; every one born red by a planted fault —
see the worklog). Fixtures: `probationes/fixa/thistle/` (+ `adversa/`),
inventory in `fixa/FONTES.md`; computus golden `fixa/computus/basis.tsv`
(`COMPUTUS_SCRIBERE=1` + a named cause); fabrica goldens
`fixa/fabrica/<t>/` = the generated files byte for byte
(`BRIAR_FABRICA_SCRIBERE=1` + a named cause, inspect before committing;
the vitrea build scripts are asserted structurally, their lists come
from the corpus). The runner links silex's closure, `vendor/sqlite3.c`
and the silva amalgam as one object (cold ~21 s once; warm suite ~9 s).

## Laws
- Column 0 is the only place briar looks. Prose needs no escaping.
- A raw region ends only at a column-0 `</name>`; an STML element at
  its column-0 close or its own self-closing line.
- Every byte belongs to exactly one token; derived tokens (fons 1) hold
  none. Emission is byte-exact by construction.
- Never a second grammar for attributes: the open tag is rewritten to
  `<name attrs/>` and given to stml.
- Fabrica limits (plan 2): a file-scope object is private to
  `_regiones.c` (share through functions); probatio helpers must be
  `interior` (`-Wmissing-prototypes`); comments BETWEEN top-level units
  are dropped; one region each of html/js/css; a `methodus=` function
  must match `JsonValor* f(JsonValor*, Piscina*, vacuum*, chorda*)`
  (checked structurally before clang); refusals name the `.thistle`
  line and write nothing.
- Silva's commit root is a LIST of units for a clean parse — walk
  values, never `silva_nodus_liberi` on `radix.datum.nodus`.
- Worktree rules (until merged): `./silva/scribe.sh` not `~/.bin/scribe`;
  no ledger residents; commit with `silva.commissio_umbra(..., ['briar'])`;
  run the oratio suite once before a commit that touches pythonica.
