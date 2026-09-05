# briar — orientation

*You are reading this because you touched a file in `briar/`.*

briar reads `.thistle` files (literate C89 scripts: markdown prose,
STML config tags, raw `<c!>` regions, a `#!` line). Plan 1 built the
PARSER as a materia client; plan 2 the silva inner kind and the
headless FABRICA (thistle → silex project directory, no clang in the
suite); plan 3 the BINARY: `~/.bin/briar`, `#!/usr/bin/env briar`,
`./x.thistle [args]` builds once under `~/.rhubarb/briar/<t>-<key>/`
and becomes the program. Spec: `project-specs/briar-spec.md` (v1.3);
plans 1–3 done (`briar-plan-{1-parsator,2-fabrica,3-binarium}.md`).
Findings: `fontes/briar.worklog.md`.

## The binary
- Build: `./briar/compile_probationes.sh registrum` (objects) then
  `./tools/briar_struere.sh` → `bin/briar` + `~/.bin/briar`. The corpus
  object is SHARED with silex (`tools/corpus_infixum.sh`); a stale
  binary lies green — rebuild after touching lib/ or briar/.
- Flags (hand-parsed, `briar_imperium`, gated): `briar [-probatio |
  -struere [-iterum] | -arbor | -partes | -amalgama | -versio | -h]
  [-f <radix>] x.thistle [args…]`; shebang form recognizes ONE reserved
  first argument after the file (`./x.thistle -probatio`), `--` ends
  flags.
- **`-amalgama`** = the escape hatch: ONE file `<t>.c` beside the
  thistle (+ `probatio_<t>.c` if a probatio region exists) that clang
  compiles ALONE — line 2 of the file's banner is the exact compile
  line. Headers in dependency order (`postulata_posix.h` first), the
  regions header, lib sources with EVERY static renamed per file
  (`#define x x_<stem>` / `#undef`, lists = the `lib/` rows of
  `corpus.symbola.tsv`), regions, main; local includes blanked, `#line
  1 "<via>"` per file. v1 refuses vitrea, `vendor/`, `.m` (spec §9).
  Never overwrites a file it did not write (banner check).
- Corpus: `-f` > ascent from cwd (disk, `-versio` says `(discus)`) >
  embedded. Key: embedded = corpus stamp + flags + bytes (before any
  parse); disk = closure-content hash + flags + bytes (after fabrica).
- Run = fabricate → write project if `bin/<t>` absent → `/bin/sh
  aedificare.sh` via `processus_exsequi` (10 min deadline; clang output
  to stderr on failure) → `processus_transformare` into `bin/<t>`.
  `-probatio` execs `bin/probatio_<t>` or `probare.sh` (compiles+execs).
- Gate: `./tools/briar_fumus.sh` (pythonica `briar-fumus`) — installed
  binary, from outside the repo, fake HOME, seven stages incl. the plant
  `adversa/probatio_rubra.thistle` (must fail), a refusal, and the
  amalgams of salve + derivatum compiled by their own banner line and
  run; `-agere` opens the vitrea window and drives it with `bin/manus`
  (by hand).
- Numbers 2026-09-05: salve cold 0.42 s (disk) / 0.68 s (embedded,
  shebang), hit 9 ms; vitrea cold 1.97 s; briar 10.5 MB.

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
  `briar_silvam_solvere` frees); `briar_nexus_linea_silvae` maps lines.
  **Includes are DERIVED** (house headers only): pass one collects
  implicit symbols + unknown types, `corpus.symbola.tsv` (baked from
  the identifier index by `tools/corpus_infixum.sh`; the runner
  regenerates it) maps them to headers, pass two parses with them;
  `res->silva->capita_derivata`, written first into the generated
  header and the probatio unit; `-partes` lists them as `derivatum`
- `briar_amalgama`: fabrica fructus → `<t>.c` [+ `probatio_<t>.c`]
  (`briar_amalgamare`), `briar_amalgama_scribere` (banner-guarded
  overwrite), `briar_amalgama_inclusio_localis` (silex's include-line
  rule, public for the gate). No parsing: static lists come from the
  table, header order from the closure texts
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
· silva · fabrica · amalgama (eleven files; every one born red by a
planted fault — see the worklog). Fixtures: `probationes/fixa/thistle/` (+ `adversa/`),
inventory in `fixa/FONTES.md`; computus golden `fixa/computus/basis.tsv`
(`COMPUTUS_SCRIBERE=1` + a named cause); fabrica goldens
`fixa/fabrica/<t>/` = the generated files byte for byte
(`BRIAR_FABRICA_SCRIBERE=1` + a named cause, inspect before committing;
the vitrea build scripts are asserted structurally, their lists come
from the corpus); amalgam golden `fixa/amalgama/gamma.c` over the
SYNTHETIC fabrica `fixa/amalgama/fabrica/` (`BRIAR_AMALGAMA_SCRIBERE=1`
+ a named cause; real-corpus amalgams are asserted structurally, they
would churn with every lib edit). The runner links silex's closure, `vendor/sqlite3.c`
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
- A script needs NO `#include` for house headers (see `derivatum.thistle`);
  a bare object-like macro or enum constant with no function from its
  header nearby is the one thing derivation misses — include it yourself.
  A name ANY region declares is never derived (pass one runs for all
  regions first): a house header may reuse a script's type name.
- Worktree rules (until merged): `./silva/scribe.sh` not `~/.bin/scribe`;
  no ledger residents; commit with `silva.commissio_umbra(..., ['briar'])`;
  run the oratio suite once before a commit that touches pythonica.
