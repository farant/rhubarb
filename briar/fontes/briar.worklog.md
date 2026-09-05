# briar — worklog

## 2026-09-04 — plan 1 (parser) sealed

Ten tasks, nine gates, every gate seen red by a plant before it counted.
Findings, as measured:

- **Extent of an STML element** = column-0 `</name>` or a single
  self-closing line (spec §2 amended). The lexeme-walk design was
  dropped: the column-0 law already governs every other boundary. The
  one edge it costs, a self-closing element spanning lines
  (`<fenestra\n titulus=…/>`), becomes `elementum-non-clausum` — named,
  not silent.
- **Fence law** toggles only in the lexer's prose state; inside a raw
  region the arbor scans for the close and never classifies content
  lines, so a ``` inside C never toggles anything. The crude lexer
  (per-line, no region state) DOES toggle on a ``` inside a region: it
  is a classification gate, not the arbor.
- **FIDELIS comparator passed on all 14 fixtures on first run**, CRLF
  included: materia's `_cursorem_promovere` counts `\n` inside token
  values, so briar needs no LINEA genus and no frons hook. The `cr`
  attribute carried the `\r` offsets through the projection.
- **The byte law is structure-independent.** Task 3's plant (the
  close never found) went red on seven structural assertions and stayed
  green on every emission check. Consequence for plants: a fault that
  only moves structure cannot be caught by a byte gate, and a plant
  for a byte gate must drop or move bytes (Task 7's plan-suggested
  guard removal was replaced by "drop the final unterminated line",
  which the truncation class caught in 22 cases).
- **Examen caught a real bug at birth:** `_clausuram_quaerere` returned
  `i32` (unsigned) with a `-1` sentinel — four domestica ("sentinella
  negativa in functione insignata", "comparatio vana") on the first
  scribe. An unterminated region would have indexed past the line
  table. Fixed to `s32` before the first test ran.
- **Two plants were inert, both instructive:** (1) leaving the `!` in
  the rewritten open tag parses fine — `<c! a="x"/>` is LEGAL STML (a
  raw self-closing element with attributes); (2) skipping the rewrite
  entirely is a compile error, not a runtime red (unused function under
  `-Werror`). The real plant: emit `>` instead of `/>` → unclosed
  element → four reds.
- **STML accepts `bad=` (attribute without value) as an empty value**;
  the nexus fixture for a malformed open tag uses an unterminated quote
  instead, which STML refuses at 1:10.
- **Canon** loaded by path (shared `<arbor>` root, as css/md); seal
  pinned `9a77c465`; derivation guard covers 5 genera, 14 loci, 8
  lexeme tags, the envelope, and the reverse direction. The attribute
  nodes of the envelope rule are direct `liberi` of the `<elementum>`
  node in the parsed canon, as assumed.
- **Computus golden born** with cause "nativitas portae": salve 507
  bytes / 12 tokens / 6 nodes; salve_vitreum 1315 / 20 / 10. The
  parse of the larger fixture is 0.05 ms; the STML write+read 0.2 ms.
- **Environment finding (worktree):** the pythonica gate depends on
  `oratio/build` objects (`vocabula.sh` inside `probatio_silva.py`);
  a fresh tree must run `./oratio/compile_probationes.sh` once before
  any commit that touches `pythonica/silva.py`, or the shadow gate
  reports `signum absens` with a traceback in its acta.
- `silva.arbor(via)` dispatch for `.thistle` is not wired in pythonica
  (no consumer yet); `./briar/arbor.sh` is the instrument.

## 2026-09-05 — plan 2 (fabrica): silva in the nexus, silex promotion, headless fabrica

Six tasks, executed inline in the worktree (commits 4cc10d97 → T6).
Gates 10/10 (`silva` 39 assertions, `fabrica` 149). Findings worth
keeping, in the order they bit:

- **The silva amalgam header and `stml.h` cannot share a translation
  unit**: `silva/amalgama/silva.h` carries its own copy of stml's
  enums (`STML_NODUS_*`, `STML_ERROR_*`) for the arbor projection, and
  they are not guarded. So the silva inner kind lives in its own unit
  `briar_silva.c/.h`, and `briar_nexus.h` includes NEITHER — it
  forward-declares `structura StmlNodus;` / `structura BriarSilva;`
  (stml.h names its struct tag, so a tag reference is enough) and
  consumers include what they dereference. The plan had put the silva
  fields on `BriarNexusRes` directly; they now sit in `res->silva`
  (`BriarSilva {piscina, parsura, semantica}`).
- **A broken parse's commit root is a LIST value, and a clean one's
  too**: `parsura->commissio->radix` for `regio.c` is
  `SILVA_VALOR_LISTA` of units. `silva_nodus_liberi` on
  `radix.datum.nodus` read the list as a node → SIGSEGV in the error
  walk. Both walkers (error search, unit partition) now accept LISTA
  or NODUS.
- **Extent end is exclusive**: `silva_nodus_extensionem` maximum =
  byte after the last token; the gate's "main unit ends with `}`"
  assertion confirmed it on the first run.
- **Prelude byte count** = `chorda_aedificator_longitudo` after the
  prelude appends; units with `minimum < praeludium_octeti` (the
  exemplar declaration) are skipped.
- **`c_fractum`**: a line of `@@@` as the first line of a unit gives an
  ERROR node whose `silva_nodus_extensionem_lineis` start IS that line
  — the thistle mapping asserted exactly.
- **Sentinel in an unsigned again**: `briar_silvam_texere` returned
  `i32` with `-I`; examen's domesticum flagged `< ZEPHYRUM` as always
  false in the test. It returns `s32`. Second time this session (plan
  1 had it in `_clausuram_quaerere`).
- **`lib/*.c` inside a block comment** ends the comment (`-Wcomment`,
  the memory's warning made real in `silex.h`). Written in words.
- **Generated header goes to `include/`**, not `fontes/`: the probatio
  unit compiles with `-Iinclude` only; `fontes/salve_regiones.h` was
  unreachable from `probationes/`. The spec's §4.2 row is amended.
- **Implicit standard trio** in the generated header (`stdio.h`,
  `stdlib.h`, `string.h` after `latina.h`): the plain fixture calls
  `imprimere` without including stdio, and a thistle file is a
  script. Decision recorded in the spec.
- **Name collision on disk**: the hand tool's binary is
  `briar/build/fabrica` (as `build/arbor`), and the gate's write area
  was `briar/build/fabrica/` — `stat` says "not a directory". Area
  renamed `fabrica_area/`.
- **`filum_directorium_creare_si_necesse` creates ONE level**;
  `briar_directoria_creare` walks the path. Public, since the test's
  golden writer needs it too.
- **First-bake numbers (spec §10)**: the stateless vitrea app pulls
  NO `volumen`, hence no sqlite, no `vendor/`: closure 71 files (34
  `lib/*.c`), `./aedificare.sh` cold 2.1 s (capsula_generare + app +
  three frameworks), binary 364,176 bytes, `probare.sh` green. The
  plain fixtures: `salve, munde`, `summa 3`, probationes green. The
  `#line` maps compile — a clang error names the `.thistle` line.
- **Golden format**: directories of generated files under
  `fixa/fabrica/<t>/` compared byte for byte; the vitrea build SCRIPTS
  are asserted structurally instead (their file lists are computed
  from the corpus and would churn on every lib change).
- **`commissio_umbra` refuses a directory as a via** ("plagulae post
  photographiam mutatae aut extra eam") — list the golden files.
- **Runner cold cost**: sqlite (`-O2`) + the 3.5 MB amalgam compile in
  ~21 s once; warm suite 8.7 s for 10 gates. `silex_clausuram_*`
  prints "monitio - nulla implementatio pro latina.h" to stderr per
  header-only file — noise in every log; a silex quiet flag is a
  named future.
- **zsh does not word-split `$F`** in the Bash tool (memory
  `zsh-no-word-split`): `formator.sh $F` saw one argument.

## 2026-09-05 — plan 3 (binarium): the first shebang run

Five tasks (9cb3bb50, 39a03a8a, T4, T5). `./salve.thistle` from a
directory outside the repository printed `salve, munde`. Findings:

- **`build/probatio_*.o` leftovers broke `silex_struere.sh`'s link**
  (`capsula_speculi_hospes` undefined — a per-app capsula only the
  probatio links). Fourteen of them sit in main's `build/` too, from
  before d55b9096 moved test objects to `build/probationes/`; silex's
  build script had not been run since. The link now skips
  `probatio_*.o`; the worktree's leftovers were deleted.
- **Verifying a script that installs into `~/.bin`** from a worktree:
  `HOME=<scratch> ./tools/silex_struere.sh` — the corpus block, the
  link and the install all ran, and the real `~/.bin/silex` kept its
  mtime.
- **`tools/briar.c` cannot include both `briar_silva.h` and
  `materia_arbor.h`** (the amalgam/stml enum collision of plan 2): the
  STML projection moved into `briar_proiectio.c`, whose header is
  stml-free.
- **`-versio -f X` bug**: the flag loop returned at `-versio` before
  reading `-f`; found by hand (`bin/briar -versio -f /nonexistent`
  printed the disk corpus, exit 0). Now the loop runs to the file or
  the end, and `-versio` needs no file.
- **examen sees `va_start`/`va_end` as implicit calls** (suspecta, not
  a refusal) — stdarg macros are outside its lexicon; the variadic
  `_monere` helper passed with 5 suspecta / 0 violations.
- **Disk-corpus stamp = closure-content hash**: `filum` has no mtime
  accessor; hashing the bytes of the ~10–71 closure files after the
  fabrica costs less than a stat walk and moves the key exactly when a
  file that goes into the project changes. Inside the tree a cache hit
  therefore still parses (0.42 s cold → ~30 ms hit measured loosely);
  from outside (embedded) a hit is 9 ms.
- **Numbers**: salve cold 0.42 s (disk) / 0.68 s (embedded, shebang);
  hit 9 ms; salve_vitreum `-struere` cold 1.97 s (embedded); bin/briar
  10,461,832 bytes (silex 19,445,544); fumus 5.2 s for six stages; the
  scratch `~/.rhubarb/briar/` after four projects: 2.6 MB.
- **Corpus stamp says `SORDIDUM`** when lib/ or include/ carry
  uncommitted edits at bake time — it did during this session (the
  quiet-switch edit); the stamp is honest, rebake after committing.
- **The fumus prints `-versio` from the AREA**, not the repo root: from
  the root it says `(discus)` and looks like the wrong corpus.
- `-agere` (window + `bin/manus`) not run this session — Fran's hand.
