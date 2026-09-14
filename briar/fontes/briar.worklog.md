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

## 2026-09-05 — derived includes (house headers only)

`derivatum.thistle` has no `#include` at all and builds. Notes:

- **Two passes, not a fixed point**: pass one with the prelude alone,
  collect `est_implicitum` symbols and `TYPUS_NOMINATUS_IGNOTUS`
  diagnostics (the diagnostic's node extent IS the type name), look
  them up, pass two with the headers. Headers pull their own
  dependencies through the closure, so a third pass never found
  anything in the fixtures.
- **The table is the identifier index's `include/*.h` rows**
  (`build/nexus.tsv`, `sedes` only): 1,854 functions, 914 macros, 716
  typedefs, 1,501 enum constants, 11 variables; two ambiguous symbols
  (`Capitulum`, `Liber`). `./silva/nexus.sh -renovare` is incremental
  (~1 s), so regenerating on every stale header is cheap.
- **Function-like macros derive for free**: an unexpanded
  `CREDO_AEQUALIS_I32(...)` parses as a call to an undeclared function
  and lands in the implicit set. Object-like macros do not.
- **The parse prelude now carries the stdio/stdlib/string trio**, as
  the generated header does; `praeludium` is 4 (+ derived headers, + 2
  for a `methodus=` region). Tests that pinned 1 and 3 moved.
- **The sort's indices were `i32`**: examen's domesticum caught
  `j >= ZEPHYRUM` always true on the unsigned type (third time this
  project); `s32` indices with casts at the xar calls.
- **`silva.Editio` for the fabrica edit** (Fran's suggestion): the
  token-anchored replace matched every site the formatter had
  re-aligned, in one pass; examen + differre reported in the same
  call. One rough edge: its own reformat left a few statements with
  odd indentation (the pre-commit formatter fixes them, `-vitia` shows
  them) — cosmetic.
- **Open silva question found on the way (filed on the ledger):** in
  pass one, when a header arrives through the REGION'S OWN `#include`
  (not the prelude), the call in the first statement of a function
  body is marked implicit while calls in initializers and later
  statements resolve — e.g. `piscina_destruere` (piscina.h:37) after
  `Piscina* p = piscina_generare_dynamicum(...)` (piscina.h:22);
  `credo_aperire` in every probatio fixture. Pass two, with the same
  header additionally in the prelude, resolves everything with an
  IDENTICAL token count (285 for the probe), so the declaration was in
  the stream both times. `chorda.h` in an app region did not show it.
  Not chased: the derivation is correct either way, and a header the
  region includes itself is now never derived (`_regio_includit`), so
  the generated header stays clean. Repro: scratch thistle with
  `#include "piscina.h"` + the two calls, `briar -partes` shows
  `piscina.h derivatum`.

## 2026-09-05 — `-amalgama`: one file, clang alone

`./salve.thistle -amalgama` writes `salve.c` + `probatio_salve.c`
beside the thistle; line 2 of each is its compile line, and the fumus
runs exactly that line (stage VII). Module `briar_amalgama`, gate
`probatio_briar_amalgama` (born red by dropping the `#undef` emission:
golden + three named asserts + the pair check all red). Findings:

- **Three deviations from the banked design, each measured first.**
  System includes are NOT hoisted: `lib/filum.c` has `<io.h>` under
  `_WIN32` and `lib/machina_posix.c` `<uuid/uuid.h>` under a
  condition — hoisting would include them unconditionally. In place
  is harmless (guards) and only `postulata_posix.h` has an order
  constraint (codex 85), so it is forced first. Every static is
  renamed, not only colliders — a user region can collide with a lib
  static too, and the identifier index gives the whole list for free.
  The vitrea form is refused in v1: the design assumed `capsula_<t>.c`
  text exists, but `tools/capsula_generare.c` is a tool with everything
  in `main` (763 lines: toml, glob, flatura), nothing in-process;
  named in spec §9 with the four ingredients.
- **The index does not record linkage.** "Static" = a depth-0
  `lib/*.c` sedes row (functio, variabile, typedef, constans, macro)
  whose name has no `include/*.h` row; sound because a public function
  needs a header prototype under `-Wmissing-prototypes`. 4,614 rows;
  the member-name risk (a static renamed while a header struct member
  shares the name) measured 0. Private struct TAGS in `.c` files are
  not indexed — none collided in the fixtures. Objective-C files have
  no index rows at all (nexus parses C89), another reason the vitrea
  amalgam waits.
- **`corpus.symbola.tsv` now has two row kinds** in one file; the
  derivation reader (`_symbola_legere`) had to learn to skip the
  `lib/` rows — without that, every public function (rows in both
  `include/x.h` and `lib/x.c`) became AMBIGUOUS and the derivatum
  golden went red. The fabrica gate is the guard.
- **The ludus merge exposed a derivation bug** (fabrica golden red on
  `punctum`): `include/mandatum.h` now defines `Punctum`, and
  `punctum.thistle` defines its own; the probatio region's unknown
  type `Punctum` looked up to `mandatum.h` → `#include "mandatum.h"`
  → duplicate typedef at compile time. Fix: pass one for ALL regions
  first, then derivation skips any depth-0 non-implicit symbol of any
  region (its own declarations + the headers it includes itself — all
  reach every unit through `<t>_regiones.h`). Assertion added to the
  silva gate. Lesson: a fixture's type name can be shadowed by a house
  header written LATER; the golden is what noticed.
- **Closure order is BFS from the seeds**, so `beta.c` preceded
  `alpha.c` (gamma derives only `beta.h`). Sources now follow their
  header's dependency position — low-level libs first, readable.
- **`re.sub(r'\bundef\b', …)` renamed inside string literals and
  comments** (`"#undef "` → `"#sublatio "`): the gate had been run
  BEFORE the rename and stayed "green" in my head. Renaming by regex
  is the training-prior trap the memory names; `silva.Editio` or
  `renominare.sh` next time. Caught by rerunning the gate.
- **`git checkout <untracked file>` restores nothing** — the planted
  fault would have stayed in the new module; the `|| true` hid the
  error. Check the plant is GONE (`grep -c PLANTA` = 0), never assume.
- **Numbers**: salve.c 5,974 lines / derivatum.c 5,988 (closure of
  10 files); gamma.c 172 lines; 23 static wrappers in salve's closure;
  amalgama gate 0.3 s; fumus 7 stages green; `bin/briar` rebuilt from
  the branch (corpus stamp 177eab66).

## 2026-09-05 — plan 4: contextus (fragments and transclusion)

`<c! id="summa">` defines a chunk, a line `<<#summa>>` weaves it in;
the compiler still names the chunk's own thistle line. Unit
`briar_contextus` between nexus and silva; spec §3.4 (v1.6). Findings:

- **Every sigil already existed.** `<#id>` fragment, `<<#id>>`
  transclusion = alias (the 2026-08-26 decree), `#` = document space.
  briar is the first C consumer resolving `<<#id>>` to text; the
  registry row in stml-visio.md says so. No new syntax was minted.
- **The weave must precede silva** — a root with `<<#x>>` is not C —
  and silva must not see `#line` (it refuses them), so the woven text
  carries a side table (thistle line per woven line) and the fabrica
  emits `#line` at every run break, inside function bodies too. The
  `-partes`-visible proof: `fragmenta_regiones.c` has four runs inside
  `summare` (13, 9, 16, 41).
- **The plan's line numbers were guesses; the file's are law.** The
  fixture was written first, `grep -n` gave the truth (capita at 20,
  not 19), and the gate pins those. A reflow of `fragmenta.thistle`
  reds the gate rather than lying.
- **First plant was a compile error, not a red gate**: removing the
  prefix prepend left `_lineam_habet_signum` unused → `-Werror`. A
  plant that does not compile proves nothing; the second plant
  (prefix → empty string) went red on four assertions.
- **Fixture facts beat plan facts twice more.** `fragmenta.thistle`
  cannot show derivation through a fragment: both roots pull
  `piscina.h` through their own includes (chorda.h and credo.h both
  include it). A second fixture with no includes at all
  (`fragmenta_derivata.thistle`) does.
- **A latent derivation asymmetry** from the morning's fix: the
  "declared by any region" exclusion counted the probatio's includes,
  which never reach the generated header. Only app regions' directives
  flow to every unit; the probatio is now excluded from the set. No
  older fixture had a probatio including a header the app needed.
- **A contextus refusal on a root was overwritten** by silva parsing
  the unwoven text ("parsura fracta"): roots that carry a refusal are
  no longer parsed; the fabrica checks `linea_erroris` before anything
  else and skips fragments before the "non parsata" check.
- **The hand tool is a fourth pipeline** (`briar/instrumenta/fabrica.c`)
  and needed the weave too; the grep for call sites had not covered
  `briar/instrumenta/`. Pipelines that must weave: tools/briar.c, the
  hand tool, three test helpers.
- **Pure-libc scripts do not build** — found by the `#line`-truth
  stage: `fragmentum_erratum.thistle` without any house include has an
  empty closure, no `lib/`, and the plain `aedificare.sh` dies on the
  literal `lib/*.c` before the type error. Ledger quaestio; fix =
  explicit lists in the plain ordo (silex's projects never hit it).
  The fixture includes chorda.h meanwhile.
- **By hand**: `./bin/fragmenta` → `salve, fragmenta: 6`, probatio
  green; a copy with line 9 broken made clang say
  `fragmenta_fractum.thistle:9:15` — and `:35` for the unused
  parameter that followed, both true. Numbers: contextus gate 100
  assertions 0.2 s; fragmenta amalgam 6,006 lines; fumus nine stages.

## 2026-09-05 — plain scripts list the closure (salutatio.thistle)

Fran ran `project-specs/exempla/salutatio.thistle` (the ludus
session's native-window example) and it failed at link time: every
`fenestra_*` and `tabula_pixelorum_*` symbol undefined. Cause: a
plain program's `aedificare.sh` compiled `lib/*.c` and linked no
frameworks, so `lib/fenestra_macos.m`, present in the closure, was
never compiled. The same glob had bitten the other way an hour
earlier (empty closure → literal `lib/*.c` → clang dies). One fix for
both: `silex_ordinem_fingere` / `_probandi_fingere` take a closure;
NIHIL = silex's old text (silex's goldens untouched, `probatio_silex`
green), a closure = explicit `.c` and `.m` list through the vitrea
ordo's own `_ordinem_fontium_appendere` plus the framework triple when
a `.m` is present. Notes:

- **Two failures, one hole.** A glob stands in for a list the tool
  already has. The vitrea ordo had learned this in plan 2 ("numquam
  globus"); the plain ordo had not, because silex's own projects never
  meet an empty or Objective-C closure.
- **briar's plain script goldens went structural** (each closure lib
  file exactly once, no glob, frameworks iff `.m`), as the vitrea ones
  already were: explicit lists churn with the include graph.
- **The fumus builds salutatio (`-struere`) without opening it** —
  stage X; the window stays Fran's `-agere` territory. Stage IX's
  fixture is pure libc again, so the empty-closure path is proven by
  the same run.
- Over-linking three frameworks for any `.m` is harmless; exact
  derivation is Fabrica's reservation (01KZYN4VPZ).
- **Two more holes under the same link.** `fenestra.h` has two
  implementation files; silex's closure stopped at the first suffix
  twin. The header already declares the second for aedilis
  (`<aedilis corpus="lib/fenestra_textus.c"/>` in a comment), so silex
  now reads that annotation after the conventional twin, deduplicated
  by path (`motus.h` annotates its own twin — a first draft collected
  it twice and the link reported 94 duplicate symbols). Then
  `fenestra_textus.c` pulled `fons.c` and `utf8.c`, whose own header
  includes read `"../include/x.h"`: the scanner produced
  `include/../include/fons.h` and silex's `novum -vitrea` scaffold
  could not write it. The two sources now include `"x.h"` and the
  scanner strips the prefix. silex's closure test gained the fenestra
  case (both implementors, both transitive libs, each once).
- **The ludus session's commit had named three briar defects**
  (8d5c85a9): the `.m` + frameworks hole, the second implementation
  file, and "a function used as a value is not derived". Two are
  closed here; the third is real — silva marks only CALLS as implicit,
  a bare function name passed as a value raises nothing — and joins
  the bare-macro case in spec §9. Reading other sessions' commit
  messages before debugging would have saved the first hour.

## 2026-09-11 — facies (consilium V, T1-T4): quid pagina docuit

**Numeri.** `salutatio2.thistle` (415 lineae) -> 70.107 octeti paginae
sui sufficientis, 0,07 s. Fragmenta XIII, transclusiones XV, contexta
II, symbola derivata CXV notata, capita XIV in insula. Porta facies
currit sine clang, sine fenestra, sine rete.

**Quod pagina INVENIT primo cursu vero.** `salutatio2.thistle` tabulam
capitum MANU scriptam fert et ipsa monet: *"if the two ever disagree
the table is wrong."* Discrepant. Tabula XIII nominat; briar XIV
derivat. Deest `motus.h` — plagula `constans Motus* motus` linea CCLXVII
adhibet. Charta ergo munus suum primo die praestitit: pretium quod
prosa manu reddebat, pagina automatice reddit et errare non potest.

**Culpa plantata quae TEGMEN nominavit, non successum.** Aside
regionis sublatum -> assertio UNA rubra sola (aurum). Causae
assertiones omnes viridis manebant quia aside PAGINAE (causa fabricae)
aside REGIONIS geminabat: `circulus.html` causam eandem BIS ferebat.
Emendatio: causa fabricae SUBSIDIARIA est. Eadem planta postea XIII
assertiones rubras dedit. LECTIO: planta quae parum rubet defectum
tegminis nuntiat; numerus rubrorum legendus est, non solum color.

**Vestis ut parametrum.** Aurea corporis paginae veste PROBATORIA
(notae solae) redduntur. Sine hoc CSS quodque emendatum aurea moveret
et nemo post mensem sciret an mutatio significans esset.

**Evasio prosae (F6).** HTML crudum in prosa transibat - non vitium
briaris sed mos markdown, tribus bracchiis in md/html/md-html.stml.
Proiectio propria (`briar/facies/md-html-facies.stml`) ea evadit.
Ratio non securitas primum sed COHAERENTIA: forma HTML bloci columna 0
IAM recusat, ergo transitus inlineum acceptum et bloccum refutatum
faceret. Porta derivae exemplaria duo comparat.

**Semita quae CODICE non eguit.** Par (symbolum -> caput)
briar_silva IAM computabat et abiciebat; insula id servat. Similiter
md nomen tagi IAM scandit (TAGI_CRUDI, LXII TAGI_BLOCORUM) et abicit -
ergo politica per tagum EXPOSITIO erit, non extensio parsatoris
(par. 9). Bis eadem forma: quod quaeris saepe iam computatum est.

**Lectio crustae.** `tools/briar.c` recusationem ante actionem
sistebat, ergo modulus legem F4 servabat sed BINARIUM eam frangebat.
Fumus id invenit, non porta: porta modulum solum videt.

## 2026-09-11 — spectator (consilium VI): quod porta videre non poterat

**Assertio mea inutilis erat.** Identitatem octetorum inter '-html' et
spectatorem probare volui, et probationem scripsi quae plagulam eandem
BIS intra processum UNUM reddebat, optionibus IISDEM. Cum binaria vere
divergerent (via absoluta contra argv crudum in capite paginae, XXXII
octeti differentiae), assertio VIRIDIS mansit. Cursus verus rem XXX
secundis invenit.

Remedium ad CAUSAM: briar_optiones_plagulae, sedes una quam ambo
vocant - ergo deriva structuraliter impossibilis. Et probatio vera in
fumum ivit, ubi binaria DUO conferuntur. LECTIO: probatio quae unum
processum non excedit divergentiam binariorum videre non potest,
quantumvis callida sit.

**Caveat identitatis.** 'Octetim aequales' valet EODEM CORPORE. Intra
arborem briar corpus disci legit, spectator semper infixum, ergo
stampa fabricae (et clavis in capite) differunt. Area fumi extra
repositorium iacet, ubi ambo idem legunt - ideo ibi probatur.

**Planta quae mentitur.** Cursorem symbolorum probare volui planta quae
clavem tabulae 'planta' faceret - sed quia et lectio et insertio eandem
clavem fictam ferebant, mores fere servabantur: UNA assertio rubra.
Planta secunda (lex gemelli fracta: omne caput -> lib/piscina.c) QUINQUE
dedit. Non omnis planta aequalis est: planta quae duo loca simul mutat
se ipsam celare potest.

**Quod cursus verus docuit et porta non.** Directorium '/x' fictum quod
parentem crearet directorium VERUM 'x' reliquit (briar_directoria_creare
viam TOTAM creat, non parentem solum). Et silex monitionem 'latina.h
sine implementatione' bis clamabat donec silex_monitiones_tacere
adderetur - quod briar ipse iam faciebat, et ego oblitus eram.

**Gradus tres scripti (S4).** facies.js nunc pontem quaerit, deinde
insulam, deinde tacet. Plagulae tres EAEDEM spectatori et navigatori
serviunt - id est quod vestem in plagulas ponere (F3) meruit.

## 2026-09-14 — fasciculus, opus III: consilium purum (plan 7)

`briar_fasciculum_consilium` ex nexu et via plagulae decernit
identitatem (A4), nomen et versionem (A5), fontem iconis (A7) et viam
fasciculi iuxta plagulam (A3), sine disco. Elementum `briar`, in spec
par. 2 nominatum sed numquam aedificatum, hic primum legitur
(identitas, versio, icon; titulus manet non aedificatus). Fabrica
elementa praeter fenestram tacite praeterit (`briar_fabrica.c:323`),
ergo elementum briar aedificationem non turbat.

**Regula una.** Identitas ordinaria = `org.rhubarb.briar.` + nomen
plagulae cum `_` in `-`; `briar_fabrica_titulus` characteres extra
[A-Za-z0-9_] iam in `_` vertit, ergo ordinaria semper valida est.
Attributum per `fasciculum_identitas_valida` (opus I) iudicatur: regula
non duplicatur.

**Lineae.** Recusatio lineam tagi fert (elementum STML: `linea_initium`
EST linea tagi). Elementum iteratum in SECUNDO recusatur et primum
nominat.

**Tres correctiones plani.**
- Nullae uncinae angulares in chordis aut commentariis C (lex scansoris:
  tag in commentario aut chorda evocatur): nuntius "elementum briar
  iteratum", commentaria "elementa briar et fenestra".
- `Editio.inserere_ante(functio)` in plagula ubi functio commentarium
  suum non habet adiutorem SUPRA commentarium plagulae et inclusiones
  posuit: `ChordaAedificator` ignotus, examen REICE. Emendatum ancoris
  exactis ex `e.textus`. In plagula nova adiutorem cum ancora
  inclusionis ultimae insere, non per nomen functionis.
- Planta III plani (`FALSUM &&`) numerum lexematum mutat et
  `elementum_briar != elementum_briar` monitum tautologiae clang daret;
  `r == NIHIL` (numquam verum) lexematum numerum servat.

**Porta** `probatio_briar_fasciculum` (XXV assertiones): rubra contra
stipitem XX, praedicta XX; viridis XXV.

| Planta | Praedicta | Observata |
|---|---|---|
| P1 `_` manet in identitate ordinaria (consilium recusat) | 4 | 4 — lineae 79 82 83 87 |
| P2 icon attributi sine directorio plagulae | 1 | 1 — 119 |
| P3 briar iteratum non recusatum (`r == NIHIL`) | 3 | 3 — 145 146 148 |
| P4 validitas identitatis omissa | 3 | 3 — 138 139 141 |

**Clausura.** `bin/aedilis lib/fasciculum.c --partes`: plist, xml,
base64 et fasti (iam aderat); RADIX_FONTES quattuor accepit.
`tools/briar_struere.sh` omnia `briar/build/*.o` nectit, ergo binarium
post hoc opus nectibile manet (imago et icones opus IV/V exspectant).

## 2026-09-14 — fasciculus, opus IV: scriptor super Imaginem decodificatam (plan 7)

`briar_fasciculum_scribere(piscina, consilium, icon, exsecutabile,
domus, causa)`: pixela in `.icns` sub `domus/fasciculum/t.icns`
(icones, setum Apple, ad centrum recisum), planum fasciculi
(`fasciculum_reddere`), deinde lex A6 super viam exsistentem, deinde
`fasciculum_scribere`.

**Lex A6, ordine.** (1) via exsistens quae directorium non est, aut
directorium quod `fasciculum_legere` recusat: "exsistit neque
fasciculus est", nihil tangitur; (2) fasciculus cuius
CFBundleIdentifier differt: "fasciculus alienus", intactus; (3)
proprius: `filum_arborem_delere` (opus II, lstat), deinde scriptio.
fasciculum ipsum numquam delet: sine deletione plagula stala superest
(planta P2 id probat).

**stb_image extra portas.** `bin/aedilis lib/icones.c --partes`
`lib/imago.c` nominat, quia caput `imago_opus.h` caput `imago.h`
includit; sed `nm -u` super icones.o, imago_opus.o, imago_png.o contra
symbola in imago.o definita NIHIL commune dat. Ergo RADIX_FONTES
imago_opus, imago_png, icones SOLA accepit. Clausura capitum non est
clausura symbolorum.

**Porta** (XXV assertiones novae, III apparatus; summa L): rubra contra
stipitem XVII, praedicta XVII (I 9, II 3, III 4, IV 1); viridis L.

| Planta | Praedicta | Observata |
|---|---|---|
| P1 comparatio identitatis caeca (`vetus.identitas` bis) | 3 | 3 — lineae 238 240 244 |
| P2 deletio omissa (`filum_directorium_existit` pro deletione) | 1 | 1 — 226 (plagula stala superest) |
| P3 plagula plana ut absens (`filum_directorium_existit` bis) | 1 | 1 — 252 (scriptio adhuc deficit; causa SOLA rubet, ideo causa asseritur) |

**Periculum nominatum.** Fasciculus proprius semiscriptus (fasciculum
non revertit) exsecutabile legibile non habet, ergo `fasciculum_legere`
eum recusat et scriptor "neque fasciculus" dicit: usor manu delet.
Acceptum (plan 7, periculum I).

## 2026-09-14 — fasciculus, opus V: vexilla, icon infixus, binarium (plan 7)

`-app` (actio) et `-icon <via>` (ante plagulam, iuxta `-f`) in
`briar_imperium`. `-icon` sine `-app` recusatur POST sectionem II (forma
shebang `./x.thistle -app` actionem ibi demum dat); argumentum sequens
quod '-' incipit aut '.thistle' continet = "-icon sine via". Porta
imperii +XII: rubra VIII praedicta VIII; planta (custodia `!= APP` in
`== VERSIO`, lexemata aequalia) II praedicta II.

Icon ordinarius `briar/icon/default-thistle.jpg` (Frani, MXXIV px, 386
KB) in capsula PROPRIA (`tools/briar_icon_capsula.sh`; spectator eam non
vehit). Decodificatio SOLUM in `tools/briar.c` (`_fasciculum_facere`):
capsula -> `imago_caricare_ex_memoria`, aut via iconis ->
`imago_caricare_ex_file`; `build/imago.o` radicis in `briar_struere.sh`
nectitur, nulla porta briar stb_image nectit (A1).

**Mensurata manu (Fran spectavit).**
- `bin/briar` 10.716.968 -> 11.292.264 octeti (+575 KB: JPEG infixum et
  stb_image).
- `briar -app kalendarium.thistle` (in arbore, corpus disci): 3 s;
  identitas `org.rhubarb.briar.kalendarium`, CFBundleName = titulus
  fenestrae; exsecutabile arm64 +x; `.icns` 4,7 MB, iconutil X plagulas
  reddit; icon MXXIV px oculo inspectus (colores et situs recti).
- Applicatio vitrea IN FASCICULO currit et fenestram aperit: assets per
  capsulam in binarium compilata sunt, ergo exsecutabile copiatum sibi
  sufficit.
- `-app` iterum: plagula stala in Resources evanuit (proprius
  repositus), identitas eadem.
- `-icon fons_256.png`: `.icns` 210 KB, iconutil VII plagulas (16,
  16@2x, 32, 32@2x, 128, 128@2x, 256) - numerus in plano ratiocinatus
  nunc MENSURATUS.
- **A8: icon novus in Dock ET Finder statim apparuit** (applicatione
  clausa, fasciculo reposito, iterum aperta). Nulla mutatio temporis
  necessaria; nihil additum.

## 2026-09-14 — icon ordinarius: PNG pellucidum (Fran)

Fran `app-icon-transparent.png` (MXXIV px, RGBA, 1,9 MB) pro
`default-thistle.jpg` elegit: eadem pictura, charta circumcisa, margo
pellucidus. JPEG e repositorio sublatum (in bd0e1831 manet).

**Custodia capsulae viam confert.** `briar_icon_capsula.sh` recentiam
per tempora SOLA iudicabat: via iconis mutata ad plagulam tabula
vetustiorem tabulam priorem tacite servaret. Nunc etiam `grep -qF
"$ICON"` in tabula (tabula viam fili nominat). Probatum: struere I
regeneravit (tabula JPEG nominabat; nunc PNG II, JPEG 0), struere II
omisit.

**Mensurata.** `bin/briar` 11.292.264 -> 12.827.864 octeti (+1,5 MB:
PNG maius JPEG). `briar -app kalendarium.thistle`: `.icns` 4,6 MB,
iconutil X plagulas, alpha in MXXIV et XXXII px; 16 et 32 px (ic04/ic05,
ARGB praemultiplicatum) oculo inspecta sine strepitu. Fran Dock et
Finder spectavit: recte, sine quadrato albo.

## 2026-09-14 — fasciculus, opus VI: fumus XIII et XVI, sigillum (plan 7)

**XIII (sine fenestra).** `./salve_vitreum.thistle -app` in area extra
repositorium (corpus infixum, HOME fictum): identitas
`org.rhubarb.briar.salve-vitreum` (plutil), exsecutabile +x, icon
infixus X plagulae (iconutil NUMERATUR), `-app` iterum proprium reponit
(`vetus.txt` evanescit), `-icon fons_256.png` VII plagulae, fasciculus
alienus (`fasciculum_instrumentum`, `org.aliud.alienum`) recusatur et
identitate intactus manet. Fumus sine `-agere`: 12 s.

**Calibratio.** Planta operis IV (`filum_arborem_delere` in
`filum_directorium_existit`) + registrum + struere: fumus rc 1 ad
"plagula stala superest - fasciculus non repositus" (gradus XIII);
plagula restituta (octetis aequalis), aedificata iterum: sanum.

**`-agere` PRIMUM cursum (39 s, sanum post duas emendationes).**
- XV (spectator, plan 6) numquam cucurrerat. `manus incipere` `-portus
  N` addit, et `briar-spectator` argumentum primum sine '-' pro plagula
  sumebat: 'N' plagula legebatur, "plagula non lecta", portus numquam
  apertus. Emendatio in `tools/briar_spectator.c`: valores `-portus` et
  `-radix` praetereuntur. Prima coniectura (cwd diversus) FALSA erat -
  via absoluta addita nihil mutavit; plagula `.effusio` sessionis manus
  errorem verum nominavit. Via absoluta in fumo manet (innocua).
- XVI (fasciculus per Launch Services), MENSURATUM: `open -n X.app
  --args -vivum -portus 18765 -retro` portum in atrio aperit (~1 s;
  stdout per `open --stdout`). Planum `manus -s P affordantiae` dicebat:
  FALSUM - `-s` sessionem manus nominat, et applicatio a manu non
  genita "sessio ignota" est; `manus adhaerere P` prius. `finire` super
  sessione adhaesa sessionem SOLAM claudit, applicatio vivit: processus
  per viam fasciculi necatur.

**Mensurae sigilli.** `bin/briar` 12.827.864 octeti; `briar-spectator`
20.108.560, reaedificatus: corpus eius infixum post opera I-V renovandum
erat, ne identitas octetorum gradus XII falleret.


## 2026-09-14 — visio in applicatione, opus III (plan 8)

- **The page in every vitrea build.** For a vitrea fructus `tools/briar.c`
  reads the chrome (`briar_vestem_legere`, as `-html` does) and folds its
  hash into the key through `briar_stampa_vestita`: stampa + "\nfacies " +
  16 hex of a SHA-256 over the four chrome files, each prefixed by its
  length, so bytes MOVING from facies.css to facies.js change the key too
  (the gate proves both directions). Only when the project is written
  (binary absent, or `-iterum`) is the page rendered — `briar_faciem_fingere`
  with the same options as `-html` (`briar_optiones_plagulae`) — and added
  by `briar_visionem_addere`: a genita `assets/<t>.visio.html` plus the name
  spliced into the toml `_files` list before its FIRST `]` (the list
  `_toml_fingere` writes). A cache hit renders nothing: cold 2.36 s, hit
  0.050 s, same directory.
- **The generated main** sets `figura.visio = "<t>.visio.html";` before the
  flags and prints `[<t>] visio aperta` when `atrium_gressus` returns
  `ATRIUM_ACTUM_VISIO`. Golden `fixa/fabrica/salve_vitreum/fontes/salve_vitreum.c`
  regenerated with that cause: one file changed, the diff exactly those
  two places.
- **Names.** The plan's `briar_stampam_vestire` became `briar_stampa_vestita`,
  a noun phrase beside `briar_stampa_clausurae`; the test helper is
  `_vestem_probationis` (`probatoriam` is IGNOTUM and would have been a new
  word for the lint).
- **Gates.** Facies: `_paginam_fingere` split into `_fructum_fingere` +
  `_vestem_probationis` — proof of no behaviour change: the count stayed
  133 and every golden stayed green. The visio block +15 (148), born red
  7/7 against stubs; plants `partes[1] = vestis->scriptum` -> exactly 1 red
  (a != c) and a toml `.visio.htm` -> exactly 1 red. Fabrica: +2 structural
  (257), red 2 -> template -> 1 red (DIFFERT) -> regenerated -> green.
- **Measured.** salve_vitreum page 20,416 B, binary 417,152 B; kalendarium
  page 167,397 B, binary 601,936 B; `bin/briar` 12,828,120 B. The two-dot
  asset name goes through capsula_generare fine. `manus clavis
  Cmd+Shift+v` against salve_vitreum -> `[salve_vitreum] visio aperta` with
  the REAL page. Fran looked at kalendarium: Visio in its menu, the
  literate page in a second window, the symbol panel's island, closing it
  leaves the calendar running — all good.
- **Known.** A project written by the dev instrument `./briar/fabrica.sh`
  gets `figura.visio` but no page (the instrument renders none), so its
  Visio window shows the capsula 404. Every project briar writes has it.
