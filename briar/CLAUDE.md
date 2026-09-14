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
  -struere [-iterum] | -arbor | -partes | -amalgama | -html | -visio |
  -app | -versio | -h] [-f <radix>] [-icon <via>] x.thistle [args…]`
  (`-icon` only with `-app`); shebang form recognizes ONE reserved
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
- **`-app`** (spec §4.8, plan 7) = `<t>.app` beside the thistle. Builds
  as a run does, then `briar_fasciculum_consilium` (pure: identity
  `org.rhubarb.briar.<t>` with `_`→`-`, or `<briar identitas=…>` judged
  by `fasciculum_identitas_valida`; name = `<fenestra titulus>` else
  `<t>`; `<briar versio=…>`, absent = fasciculum's 1.0; icon `-icon`
  (cwd) > `<briar icon=…>` (thistle's dir) > embedded
  `briar/icon/app-icon-transparent.png`) and `briar_fasciculum_scribere`
  (icones `.icns` under `<domus>/fasciculum/`; an existing bundle is
  replaced ONLY when its Info.plist identity matches, via
  `filum_arborem_delere`; a foreign bundle or a plain file is refused and
  left untouched). The `<briar>` element is read ONLY here; the fabrica
  ignores every element but `fenestra`. Fumus XIII (headless) and XVI
  (`-agere`, Launch Services).
- **Decoding lives ONLY in `tools/briar.c`** (`_fasciculum_facere`,
  stb_image via `lib/imago.c`): briar modules take an `Imago`, and no
  briar gate links a decoder (`nm`: icones/imago_opus/imago_png need no
  symbol from imago.o). `briar_struere.sh` links `build/imago.o` and
  `build/capsula_icon_briar.c` (`tools/briar_icon_capsula.sh`, which
  regenerates when the table does not NAME the icon path, not only on
  timestamps).
- Corpus: `-f` > ascent from cwd (disk, `-versio` says `(discus)`) >
  embedded. Key: embedded = corpus stamp + flags + bytes (before any
  parse); disk = closure-content hash + flags + bytes (after fabrica).
- Run = fabricate → write project if `bin/<t>` absent → `/bin/sh
  aedificare.sh` via `processus_exsequi` (10 min deadline; clang output
  to stderr on failure) → `processus_transformare` into `bin/<t>`.
  `-probatio` execs `bin/probatio_<t>` or `probare.sh` (compiles+execs).
- Gate: `./tools/briar_fumus.sh` (pythonica `briar-fumus`) — installed
  binary, from outside the repo, fake HOME, thirteen headless stages incl. the plant
  `adversa/probatio_rubra.thistle` (must fail), a refusal, the amalgams
  of salve + derivatum + fragmenta compiled by their own banner line
  and run, fragmenta with `-probatio` and `-partes`, and the `#line`
  truth (a broken fragment line must be named by clang); XIII = `-app`
  (bundle, icon counts 10/7, own replaced, foreign refused). `-agere`
  adds XIV–XVI: the vitrea window and the spectator driven by
  `bin/manus`, and the `.app` opened by Launch Services and ATTACHED with
  `manus adhaerere` (first run 2026-09-14, green, 39 s).
- Plain build scripts list the closure's `lib/*.c` AND `lib/*.m`
  explicitly and add the Cocoa/Security/WebKit frameworks when a `.m`
  is present (silex's plain generators take briar's closure; passed
  NIHIL they still emit silex's own glob text). Fixed 2026-09-05 after
  `salutatio.thistle` (native window, `fenestra_macos.m`) failed to
  link and a pure-libc script (empty closure) failed on the glob;
  fumus stage X builds salutatio without opening it.
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
- `briar_contextus` (v1.6): fragments and transclusion. `<c! id="x">`
  is a FRAGMENT (never a root, never compiled alone); a line reading
  `<<#x>>` inside any C region (probatio too) weaves it in, its lines
  prefixed with the reference line's indentation, depth-first. Runs
  BETWEEN nexus and silva (`briar_contexere`): a root with `<<#x>>` is
  not C. Result on the record: `contextus` (woven text), `lineae`
  (thistle line per woven line), `est_fragmentum`. Refusals with the
  thistle line: undefined id, cycle (`#a -> #b -> #a`), duplicate id,
  `methodus=`/`munus=` on a fragment, invalid id, a malformed `<<#`
  line. Unused fragment = no error, `-partes` says `non adhibitum`.
  Reserved: mid-line references, template fragments `<#@x>`,
  continuation `pars=`, html/js/css fragments, tree-level tokens
- `briar_fabrica`: inventory → main rule → unit partition through
  silva's tree + symbol table → generated files → closure → silex
  scripts → SHA-256 key; `briar_fabricam_scribere` puts a project on
  disk (`briar_directoria_creare` walks paths; scripts chmod 0755).
  Partition: `include/<t>_regiones.h` = guard + latina.h + implicit
  stdio/stdlib/string + every region directive + type units +
  prototypes; `fontes/<t>_regiones.c` = objects + definitions; the
  `principale` unit alone in `fontes/<t>.c`; the probatio region
  (woven) in `probationes/probatio_<t>.c`; all `#line`-mapped PER RUN
  of the line table — a fragment inside a function body opens its own
  `#line`, so clang names the fragment's thistle line (fumus stage IX
  proves it with `adversa/fragmentum_erratum.thistle`)
- `briar_fasciculum` (§4.8): `briar_fasciculum_consilium` (identity,
  name, version, icon source, `.app` path from the nexus, no disk) and
  `briar_fasciculum_scribere` (decoded `Imago` → `.icns` → bundle; own
  replaced, foreign refused); fixtures `app_vestitum.thistle` (LINE-PINNED,
  `<briar>` on line 8), `adversa/app_{identitas_prava,briar_duplex}`
- `briar_computus`: bench twin; instruments `./briar/arbor.sh`,
  `./briar/computus.sh`, **`./briar/fabrica.sh <x.thistle> <dir>`**
  (then `cd <dir> && ./aedificare.sh && ./probare.sh` — the only clang
  run there is, by hand; a clang error names the `.thistle` line)

## Gates (`./briar/compile_probationes.sh [filter]`; exit 2 = NOTHING RAN)
registrum · lexema · arbor · stml · canon · totalitas · computus · nexus
· silva · fabrica · imperium · amalgama · contextus · facies · spectator ·
fasciculum (sixteen files;
every one born red by a planted fault — see the worklog). Goldens:
`fixa/contextus/fragmenta.contextus` (`BRIAR_CONTEXTUS_SCRIBERE=1`),
`fixa/fabrica/fragmenta/`. Fixtures: `probationes/fixa/thistle/` (+ `adversa/`),
inventory in `fixa/FONTES.md`; computus golden `fixa/computus/basis.tsv`
(`COMPUTUS_SCRIBERE=1` + a named cause); fabrica goldens
`fixa/fabrica/<t>/` = the generated files byte for byte
(`BRIAR_FABRICA_SCRIBERE=1` + a named cause, inspect before committing;
the vitrea build scripts are asserted structurally, their lists come
from the corpus); amalgam golden `fixa/amalgama/gamma.c` over the
SYNTHETIC fabrica `fixa/amalgama/fabrica/` (`BRIAR_AMALGAMA_SCRIBERE=1`
+ a named cause; real-corpus amalgams are asserted structurally, they
would churn with every lib edit); facies goldens `fixa/facies/`
(`BRIAR_FACIES_SCRIBERE=1` + a named cause) rendered with TEST CHROME,
so editing the real CSS can never move one. The runner links silex's closure, `vendor/sqlite3.c`
and the silva amalgam as one object (cold ~21 s once; warm suite ~9 s).

## Laws
- Column 0 is the only place briar looks. Prose needs no escaping.
- **The facies page reads with JavaScript off.** Fragment links, use
  sites, own-symbol anchors, the woven view and its gutter are markup
  and anchors. The ONE script is the declaration panel. Break that and
  the page is no longer what §4.6 says it is.
- **The chrome is files, never C string literals** (`briar/facies/`,
  carried in a capsula). briar splices; it does not contain CSS or JS.
- **HTML in prose is TEXT**, through `briar/facies/md-html-facies.stml`
  — md's projection minus `<crudum!>` in three arms, held to exactly
  that difference by a drift gate. HTML lives in regions.
- **The spectator is briar with a window** (`tools/briar_spectator.c`,
  `bin/briar-spectator`; `briar -visio` execs it). It repeats NO
  rendering — same `briar_faciem_fingere`, same chrome, so its page and
  `-html`'s are identical BY CONSTRUCTION. Identity holds *given the
  same corpus source*: inside the tree briar reads the DISK corpus, the
  spectator always the EMBEDDED one, so the fabrica key differs there.
- **`briar_optiones_plagulae` is the one place options are built.**
  Two binaries building them separately drifted (absolute vs raw path)
  and no in-process test could see it — the fumus caught it.
- **The bridge's brain is a LIBRARY, not a handler**: `briar_symbolum`
  answers title+header → definition as a pure function over the corpus,
  so the gate hits it with no window anywhere. The parse cache lives on
  a caller-held cursor, keyed by path.
- `-html` on a REFUSED thistle still writes the page (F4) and exits 0:
  the render succeeded, the cause is on stderr AND in the margin. Every
  other action exits 1 on refusal.
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
- **Fragments** (`fragmenta.thistle` is the model): `<c! id="summa">`
  defines, a line `<<#summa>>` transcludes; the reference stands alone
  on its line and carries its indentation; a fragment used twice
  appears twice; fragments may reference fragments; the probatio may
  reference any fragment. A fragment's `#include` lines reach the
  generated header through the roots that weave it.
- A script needs NO `#include` for house headers (see `derivatum.thistle`);
  a bare object-like macro or enum constant with no function from its
  header nearby is the one thing derivation misses — include it yourself.
  A name ANY region declares is never derived (pass one runs for all
  regions first): a house header may reuse a script's type name.
- Worktree rules (until merged): `./silva/scribe.sh` not `~/.bin/scribe`;
  no ledger residents; commit with `silva.commissio_umbra(..., ['briar'])`;
  run the oratio suite once before a commit that touches pythonica.
