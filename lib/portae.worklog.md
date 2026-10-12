# portae worklog

## 2026-10-10 - T1: declarations reader (portae-declaratae plan)

`portae_declaratas_legere(sutura, piscina, &causa)`: every candidate file
the sutura enumerates is read; a cheap prefilter (`chorda_continet` on
"<porta") skips files without declarations; the rest go through crusta
(`crusta_arbor_parsare`, CRUSTA_BASH) and `materia_annotationes_colligere`
with the crusta lexicon ratum and "#" - so a `<porta>` inside a quoted
string or plain prose is never a declaration (tested). Elements named
`porta` in an annotation's document become PortaDeclarata; vocabulary is
CLOSED (titulus manu causa tegit currit). After ALL files: registry
checks (title outside PORTAE, alien runner, duplicate title, PORTAE key
with no annotated runner). Refusal returns NIHIL with every cause, one
line each, `via:linea: ...`.

Bug caught by writing the cross-file test: the file loop originally ran
`while numerus == 0`, i.e. stopped reading files after the first problem
- "all causes" held only within one file. The guard was meant only for a
broken foundation (lexicon, enumeration) - now an explicit flag. Plant
(old condition back) turns the new section red.

Traps hit: `registrum` is a latina macro (register) - field renamed
`tabula_portarum` (my blunt rename then also rewrote a test's EXPECTED
string - the test failed for the test's fault; a debug imprimere showed
it). Wrote a private substring helper twice before noticing
`chorda_continet` exists - replaced.

Dogfooding (principle …Z2Y555, checklist lapide FR-008 "scriba"):
- building cause messages piecewise with chorda_aedificator
  (appendere_chorda / literis / sprintf of a line number) is the main
  verbosity - FR-008's pool formatter with a `%C` verb for chorda would
  turn `_causa(a, &n, via, linea, "titulus '", t, "' bis declaratus")` into
  one format call;
- compile_tests.sh's -I list says it mirrors aedilis.stml <inclusa> but is
  a hand-kept SUBSET (7 of ~30 dirs): lib/portae.c needed -Icrusta/fontes
  added by hand. Derive it from aedilis.stml (one source) - candidate.

## 2026-10-10 - T2 derivation (portae_operturas_derivare)

- **effectus and silva are sutura callbacks, not calls.** crusta_effectus_derivare
  reads the disk itself (follows sources), and "which programs does a test
  launch" needs silva lexemes (macros expanded, comments gone). Both stay out
  of the pure core: `sutura->effectus(cursor)` and
  `sutura->litterae_vocationum(fons)`; NIHIL = unknown -> a NAMED entry in
  `opertura->ignota`, never a refusal. The CLI (T3, tools/fabrica.c) wires
  them, as fabrica already does for `clausura_c`.
- **The enumerator is now ALL tracked paths** (git ls-files); only `.sh` are read
  for annotations. Derivation needs the tree for existence checks, suite test
  listing and binary -> entry-source mapping.
- **Rules for effectus sites:** lectio/exsecutio/fontatio/probatio/enumeratio
  only; classis arbor; resolutio plena; `.` skipped; any element with
  `custodia` (the processus AND the guarded site in the parent) is skipped
  with its subtree, following effectus's own doctrine that guarded builders
  are outside the verdict chain. Partial prefixes such as `lib/` from
  `for src in lib/*` are excluded on purpose: aedilis closures cover lib
  precisely, and Python OMISSA dropped include/lib too.
- **Compiled sources:** a `.c` site goes into `fontes_c` so T3 can match its
  closure. A `.c` glob site is expanded over the tree. This is how the old
  `fontes` mode (the `$X_DIR/sub/*.c` regex) gets ported. **-I dirs are still a
  gap**: the effectus clang entry does not record -I, so the annotation's
  `tegit` carries them (T4 will show which rows need it).
- **Glob matcher:** this uses `directorium_titulus_congruit` (iter_directoria).
  There `*` crosses `/`, which matches Python fnmatch, so the shadow
  comparison stays honest. It has no `[...]` classes. A path-glob routine
  living in the directory iterator is a dogfooding find: path globs want
  their own home, next to the FR-008 scriba "sorted glob".
- **String building:** reasons are assembled with a local
  `_ratio(piscina, ante, a, medium, b, post)` helper. That is today's stand-in
  for the planned checked formatter: ledger region "sprintf and printf
  replacement" (01M3AZRFAH7A5C9S1HPGHNS05F, `chorda_formare` with `%C` for
  chorda). Adopt it here once it lands. Also found that
  `chorda_aedificator_appendere_i32` exists, so T1's `sprintf(":%u")` into a
  fixed buffer is gone and the file no longer includes stdio.h.
- **Plant P5 find:** the "suite directory only, not subdirectories" check stayed
  GREEN with the check removed. `sub/probatio_x.c` already fails the
  `probatio_` prefix test, so the fixture never exercised the rule. The
  fixture is now `probatio_veteres/probatio_x.c`. Six plants (custodia,
  resolutio, globus expansion, probationes/ exclusion, subdirectory,
  dedupe) all went red, then green.
- **Divergence from Python, on purpose:** Python's `currit` only scanned tests
  that nexus showed calling system/processus_*. C asks the sutura about every
  suite test and leaves the cost (prefiltering) to the CLI.
- **Not yet run on the real tree.** Fixtures mirror the census shapes. The first
  real run is the T3 CLI, and the full comparison is T4.

## 2026-10-12 - T3 owed gates + CLI

- **What "unknown" means (Fran said go ahead with my recommendation):**
  - **Blind** (`ignota`: effectus unknown, test literals unknown, `currit`
    entry that doesn't resolve): the gate is owed for EVERY changed path,
    with the named reason, but the gate does NOT cover the path. A blind
    gate would otherwise hide every INTECTUM.
  - **Unknown closure:** owed AND covering. This is Python parity, because
    Python also counted the path as covered.
  - **Binary whose name is ambiguous:** covers ALL candidates, and the reason
    says "nomen ambiguum: unus ex N". It never picks one, and it no longer
    lands in `ignota`, because two sources is not a blind spot.
  - **Binary with no source in the tree:** a NOTA. These are test fixtures
    such as `bin/inst`, and they owe nothing.
- **Refusals are structured** (PortaeRecusatio {via, linea, causa}), built
  once in `_causa` and rendered both as human text and as `RECUSATIO` TSV.
  Before this, the text was the only record, and machine output would have
  meant re-parsing our own messages.
- **TSV contract and human form live in the library**
  (`portae_debita_scribere`, `portae_declaratas_scribere`,
  `portae_campum_effugere`), so probatio_portae tests the exact bytes. The
  human form copies `silva.portae_debitae_relatio`, so
  tools/portae_debitae.sh can switch over in T5 without any reader noticing.
- **The CLI lives in tools/fabrica_portae.c**, with
  `<aedilis corpus>` in fabrica_sutura.h. The real sutura uses:
  - `git ls-files` through processus_exsequi + chorda_fissio (painless:
    FR-008 `capere` + `per_lineas` are basically these two calls);
  - effectus with the mandata table read once;
  - closures through `suturae_clausura_c` (fontes + capita).
- **TRAP: silva/amalgama/silva.h redefines the STML enums** (STML_ERROR_*),
  so it cannot share a translation unit with house stml.h, which portae.h
  pulls in. The lexer call therefore lives alone in tools/fabrica_litterae.c
  behind a house-typed header. Including silva/fontes/silva_lexema.h
  instead links a SECOND silva next to the amalgamation (40 duplicate
  symbols).
- **Measured: no macro expansion is needed for test literals.** A plain lexical
  scan of all suite tests finds a superset of Python's `arbor.sh -nudum`
  answer. The extras are silva/amalgamare.sh,
  compile_tests_fontes_generata.sh and lib/manus.c; nothing was missing. In
  the C version, test literals naming a tracked `.c` also go into fontes_c
  (Python kept only `.sh`), for example the formator roundtrip fixtures and
  lib/utf8.c read by probatio_silex. This over-covers, and T4 will show by
  how much.
- **Timings** on the real tree (3 runners temporarily annotated, then
  restored):
  - `declaratae`: 0.6 s, including effectus of compile_tests.sh with its 16
    processes.
  - `debitae` with include/credo.h changed (~350 closures): 30 s cold, 2.4 s
    warm. Python's "portae debitae" commit phase is 22 s.
  - "Cold" means the first run after every `bin/fabrica` rebuild: the
    extraction cache's key includes the binary's digest. Worth remembering
    when a commit right after a fabrica edit feels slow.
- **Gates:** probatio_portae has 11 sections / 147 assertions. fabrica fumus
  XLIV is a temporary git root with a real aedilis closure: the runner
  `cat`s lib/bib.c, so editing include/bib.h owes it. Plants VII all went
  red, then green: blind-covers, closure memo, reason dedupe, tab escape,
  closure for a non-C path, CLI closure blind, refusal exit code. The CLI
  plants ran via a function gate that rebuilds bin/fabrica around the plant.

## 2026-10-12 - T4 annotations + shadow comparison

- **What was annotated:** 51 runners carry 52 `<porta>` declarations,
  generated ONCE from the ledger by a scratchpad script that is not kept:
  - 45 PORTAE keys;
  - `aequivalentia` as registered-but-manual;
  - 6 manual rows;
  - rete_fumus and vates_fumus, which had no row.

  Each annotation sits at the end of the first header comment block. All
  51 runners pass `bash -n`, and every runner diff is pure insertion.
- **Ledger cells → annotation:** 320 entries in total.
  - 116 are already derived and NOT written (73 tegit viae, 12 tegit
    fontes, 31 currit binaria).
  - 203 are kept as `tegit`/`currit`: family globs (`crusta/fontes/*`,
    `*/probationes/*` - fixtures, grammars, worklogs in a directory),
    guarded builders, runtime inputs of binaries (fabrica.stml, canons),
    and lists effectus cannot resolve (venenum's `${fontes[@]}`,
    nexus-instrumentorum's wrapper list).
  - 1 is dropped: `lib/*.h` matches nothing.
  - A `tegit` entry that duplicates `currit` is dropped, because a currit
    `.c` covers itself.
- **Shadow comparison** (tools/portae_comparare.py, temporary, deleted in
  T5): old answer = silva.portae_debitae per path (inventory + closures,
  pinned for the run); new answer = `bin/fabrica portae tactus -omnes`.
  Over 9815 tracked paths:
  - SOLI VETERI (owed only by the old answer) = 0, so the new answer covers
    everything the ledger did;
  - SOLI NOVO started at 11973 and is now 2021.
- **Derivation fixes the comparison found** (each one had a failing test
  first, and a plant):
  1. A house tool executed by a runner (`classis="instrumentum_domus"`)
     maps to its entry source, like test-launched binaries. Builder chains
     execute bin/fabrica, bin/compilator and so on.
  2. A test literal naming ANY tracked path is a dependency (verb
     `nominat`: fixtures read, tables). Every suite test is lexed; the
     system/processus_ prefilter is gone.
  3. A `.c` site that is NOT tracked (written by the script itself, e.g.
     `tools/unci-git/.fumus_generatum.c`, `culpa.c`) gets no closure.
     Before this fix the unknown closure owed the gate on every C edit:
     ~1900 paths each for unci, scribe and saltuarius.
  4. Preference for ambiguous names: for `bin/x`, a unique candidate under
     `tools/` wins; for `sub/build/x`, a unique one under `sub/` wins.
     bin/fabrica matched three fabrica.c files, and briar/instrumenta/
     fabrica.c dragged all of briar in.
  5. **Only CONTENT sites count** (lectio, exsecutio, fontatio). `probatio`
     and `enumeratio` only inspect mtime or existence. The header
     staleness loop in silva/materia_substratum.sh
     (`[ obj -nt include/<caput>.h ]`) alone made 2070 false pairs, and
     `ls include/<caput>.h` in silva/examen.sh made more. The actual reads
     are separate lectio sites.
- **Fran decision (B), 2026-10-12: instrument ownership.**
  `<porta titulus="aedilis" instrumentum="bin/aedilis"/>`: the gate that
  tests a build tool OWNS its binary, and other gates that run it inherit
  nothing (a NOTA names the owner). An explicit `currit` still wins.
  Owners:
  - aedilis → bin/aedilis
  - fabrica-fumus → bin/fabrica
  - compilator → bin/compilator
  - mensor-suitae → bin/mensor (suite timing - same infrastructure
    pattern)

  This removed exactly the 1086 infrastructure-only pairs, plus 241 for
  mensor. Owning the same binary twice is a refusal.
- **Also:** `titulus` + `manu` together is now allowed. It means a
  registered gate that a human runs (aequivalentia needs a basis built by
  hand). T1's "titulus et manu simul" refusal is gone.
- **What the remaining 2021 new-only pairs are:** real dependencies the
  ledger never recorded.
  - A gate runs another suite's runner or a tool wrapper:
    `silva/compile_probationes.sh` from oraculum-silvae/silva-m3 (325),
    `oratio/compile_probationes.sh` from the git hooks (256),
    gesta/tabulariumd.sh (127), silva/formator.sh, arbor.sh, differre.sh,
    examen.sh and nexus.sh (~320), tools/diagnostica.sh (49),
    briar/compile_probationes.sh (49).
  - Tools whose output becomes part of the product, such as
    bin/capsula_generare (apps, 96).
  - Named fixtures, e.g. probatio_silva_exemplaria_obsoleta.c naming 47
    lib files.
  - 326 pairs with mixed origins.
- **Not done here (noted):** excubitor.sh/fabrica.tsv appear for many gates
  through silva/nexus.sh index maintenance. That is plumbing at SCRIPT
  level, which binary ownership cannot express. It's a candidate for a
  later script-level form.
- **Cost:** the new answer computes more coverage, so a commit touching
  silva/oratio/gesta sources will owe more gates than before. That is the
  price of the dependencies being real.
- **Commit trap (T4):** the silva-m3 gate (`haruspex` TU check) compiles
  EVERY lib/*.c with its own hand-written `-I` list (silva/haruspex.sh
  TU_FLAGS). lib/portae.c has included crusta headers since T1, but
  silva-m3 was not owed until the runners were annotated, so the break
  hid for three commits. Added `-I$RADIX_DIR/crusta/fontes`. This is the
  second hand-kept subset of aedilis.stml's include roots, after
  compile_tests.sh; a lib that includes a subsystem header breaks every
  such copy.

## 2026-10-12 - T5 cutover

- **silva.portae_debitae is now a thin wrapper** (≈60 lines including
  docstrings) over `bin/fabrica portae debitae -machina -portae <tsv>`.
  It writes the registry TSV with `portae_tabula()`, reads the
  DEBITUM/INTECTUM contract, and unescapes `\\ \t \n \r`.
  - A RECUSATIO, a line outside the contract, or a missing bin/fabrica is
    a SilvaError, which commissio prints as MONITUM, as before.
  - `portae_debitae_relatio` passes the human form straight through.
  - Deleted: `inventarium()`, `_textus_cellae`, `_index`, `_clausurae`,
    `_clausurae_computare`, the in-process closure memory, `AEDILIS_BIN`,
    the Python matching, `portae_sine_ordine` and its commissio notes,
    tools/inventarium_suitarum.py and tools/portae_comparare.py.
  - silva.py is net -265 lines.
  - Node/ruby check: a new wrapper would need "write a TSV, run, split
    4 columns" and nothing else.
- **Test trap:** probatio_silva swaps `silva.FABRICA_BIN` for a fake
  GLOBALLY, and earlier sections inject `ficta-*` PORTAE entries whose
  runners have no annotation. The live section points FABRICA_BIN at the
  real binary and sets the `ficta*` keys aside, restoring both. The C
  side refuses unannotated registry keys, which is correct behaviour.
  The existing `_fabrica_ficta` helper mangles backslashes (repr
  round-trip), so the contract tests use a stub that cats exact bytes.
- **New gate `portae-declaratae`** (tools/portae_declaratae_fumus.sh, 3 s):
  - I: `declaratae` over the real registry has no refusal (53 gates);
  - II: with `RHUBARB_ANNALES` pointing nowhere, portae_debitae.sh still
    owes radix for compile_tests.sh (the fresh-clone property).
  - It is owed by `*.sh`, pythonica/silva.py, and (via
    `currit=tools/fabrica.c`) the C core. Commissio itself only WARNS on
    refusals, so this gate is what blocks.
  - Plants III went red, then green: annotation deleted, unknown
    attribute, wrapper made to depend on the annals directory.
- **New runners need `git add -N` before they count:** the enumerator is
  `git ls-files`, so an untracked new runner reads as "key without
  annotation". Commit staging covers it; a dry run before staging doesn't.
- **frigida fumus stage VI** read the LIVE inventory 'suitae probationum'
  to test `frigida -inventarium`. It now reads 'bibliothecae (include)'
  (known row include/piscina.h → lib/piscina.c), since the old
  inventory is being removed (Fran 2026-10-12).
- **Measured on this commit's own diff** (10 paths, now computed in C):
  4.6 s end to end. Python's "portae debitae" commit phase was 22-26 s.
