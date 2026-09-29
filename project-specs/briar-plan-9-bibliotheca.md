# briar plan 9 — bibliotheca: sharing plain C between thistles

> **DONE 2026-09-29.** T1 f0159f9d (measurement, no re-slice) · T2
> b5f36b04 (members) · T3 890dbf92 + 5e876bc2 (derivation, build, user
> surface) · T4 af1fbb8a (key, amalgam, `-partes`, fumus XXII) · T5
> records. Answers lapide feature-requests/015. As built: spec §3.5.

> **For agentic workers:** execute task by task with the executing-plans
> discipline (test first, run red, implement, run green, plant, commit),
> INLINE on main, ONE TASK PER TURN with Fran's approval, no subagents.
> Task 1 is a MEASUREMENT that may re-slice Tasks 2–5; a re-slice is
> shown to Fran before work continues. Steps use checkbox syntax.
> Written 2026-09-29 from briar-spec.md §3.5 (v1.11, approved by Fran the
> same day; spec commit `dff8408f` + the B6 cycle note).

**Goal:** `<bibliotheca via="textus.thistle"/>` in a thistle makes the
public plain-C functions and types of `textus.thistle` (and, transitively,
of its own bibliothecae) callable from every C region of the importing
thistle — its `principale`, its other regions and its `-probatio` — with
no `#include` written by hand (lapide feature-requests/015).

**Architecture:** the fabrica already partitions every thistle into
`include/<t>_regiones.h` (directives, types, prototypes) and
`fontes/<t>_regiones.c` (bodies except `principale`). A build gains
*membra*: each imported thistle is read through the same arbor → nexus
→ contextus → silva pipeline, its PLAIN C regions (no `methodus`, no
`munus="probatio"`, no `id=` fragments) are partitioned under its own
title, its public names join the derivation table pointing at its
generated `<m>_regiones.h`, and its `_regiones.c` joins the build line.
Cycles are refused; each member is linked once; the cache key covers
every member's bytes.

**Tech stack:** C89 in Latin (`latina.h`), briar modules
(`briar/fontes/`), silva (parse + semantica), silex (closure, project
writing), credo gates, `tools/briar_fumus.sh`.

**Spec:** `project-specs/briar-spec.md` §3.5 (B1–B6, words, mechanism,
rules, refusals, AUDIENDA, the cycle note), §9 (reserved), §4.1–4.3
(cache, generated units, main rule), §3.4 (fragments — NOT touched).

## Global constraints

- C89 under `tools/vexilla.sh` flags. Latin identifiers; latina macros
  are forbidden identifiers (`nomen`, `casus`, `per`, …); single
  capitals and every Roman numeral 0–3999 are macros. `chorda` is not
  NUL-terminated; `i32`/`i64` are UNSIGNED (a countdown from `-I` is
  `s32`).
- Lines ≤ 72 columns. Formatter: record `./silva/formator.sh -vitia`
  per touched file at HEAD first; commit with NO new divergence
  (files CONFORMIS at HEAD may take `-scribere` whole; others: format a
  copy and splice only the new regions back).
- **No `<word` tag openings in C comments or string literals** — the
  examen tag scanner evokes them. In C, write "elementum bibliothecae",
  never the element's angle-bracket form.
- New identifier words: `git add -N` new files, then
  `./oratio/vocabula.sh -nova` must say NOVA 0 before committing
  (glossary line or rename otherwise). Validate ALL string lengths
  before writing ANY file in a scripted edit (partial writes bit this
  week).
- Commit only through `silva.commissio(msg, viae, portae)` with explicit
  paths; never Fran's files (`FAQ.md`, `gesta/annales/*`,
  `silva/grammatica/c89-formatted.stml`). Gates: `briar` + `briar-fumus`
  per task (commissio adds owed ones). Never run a build while a
  `commissio_umbra` is snapshotting.
- `briar/MUTATIONES.md` gets an `## inedita` line in the task that
  changes what users see (Task 3 at the latest; FRANGIT only if an
  existing thistle stops building — none should). When it changes:
  rebake (`./briar/compile_probationes.sh`, `./tools/briar_struere.sh`,
  `./tools/briar_spectator_struere.sh`) BEFORE the commit and again
  after it.
- Every new gate is born red; every fix is proven by a PLANTED FAULT
  that compiles, whose red is predicted before the run, and removed.
- Never edit `../lapide-v2`. Cite `feature-requests/015` in commits.

## Review Focus

1. **An imported file changes, the importer does not** → the next run
   rebuilds (the key must hold every member's bytes — the stale-cache
   class fixed 2026-09-29, c452f4b5). Test: Task 4 Step 1.
2. **The same `staticus` helper name in importer and member** → builds
   (C statics are private) and `-amalgama` still compiles alone (the
   Q13 `Gradus` class). Test: Task 3 Step 1 fixture + Task 4 Step 3.
3. **The member is a vitrea app** (has the window element and `methodus`
   regions) → only its plain regions are shared; the importer builds.
   Test: Task 3 Step 1 (`fixa/bibliotheca/vitrea_membrum.thistle`).
4. **A compile error inside a member** → clang's message names the
   MEMBER's thistle line, not a generated file. Test: Task 3 Step 6.
5. **The importer is run from another directory** (shebang
   `./instrumenta/recensio.thistle` from the repo root, or `cd /tmp;
   /abs/path/recensio.thistle`) → `via=` resolves against the importer's
   directory, never the cwd. Test: Task 2 Step 1.

## File structure

| file | responsibility |
|---|---|
| `briar/fontes/briar_plagulae.h/.c` (NEW) | collect a document's members transitively: read each `via=`, parse it (arbor → nexus → contextus → silva), visited set, cycle and refusal checks (T2) |
| `briar/probationes/probatio_briar_plagulae.c` (NEW) | member collection gate (T2) |
| `briar/probationes/fixa/bibliotheca/*.thistle` (NEW) | fixtures: `radix`, `media`, `folium`, `circulus_a/b`, `sine_c`, `vitrea_membrum`, `duplex` (T2–T3) |
| `briar/fontes/briar_silva.c/.h` | members' public names join derivation, pointing at `<m>_regiones.h`; the second parse sees member headers (T3; how is Task 1's answer) |
| `briar/fontes/briar_fabrica.c/.h` | partition members' plain regions; generate `<m>_regiones.h/.c`; members on the build and test lines; duplicate public name refusal (T3) |
| `tools/briar.c` | call member collection before silva; members' bytes into the key (T3–T4) |
| `briar/fontes/briar_amalgama.c` | members' units in the amalgam; private rename sees all members (T4) |
| `briar/probationes/probatio_briar_fabrica.c`, `probatio_briar_amalgama.c` | generation and amalgam cases (T3–T4) |
| `tools/briar_fumus.sh` | stage XXII: run, `-probatio`, key moves with a member, amalgam alone (T4) |
| `briar/MUTATIONES.md`, `-dialectus` line, `briar/CLAUDE.md`, spec §3.5 "as built", `briar/fontes/briar.worklog.md`, `briar/probationes/fixa/FONTES.md` | records (T3–T5) |

---

### Task 1: Measure — the five AUDIENDA plus the parse question (no product code)

**Files:** none committed except the plan's "Executed" note and a
worklog entry. Probes live in the scratchpad.

- [ ] **Step 1: the partition on a non-root file (AUDIENDA 1).** Write a
  scratch driver (scratchpad, linked like `briar/probationes/*`) that
  runs `briar_arbor_parsare` → `briar_nexus_texere` → `briar_contexere`
  → `briar_silvam_texere` on `briar/probationes/fixa/thistle/
  punctum.thistle` and then `briar_fabricare` with `via_thistle` set to
  a DIFFERENT path. Record: does anything assume the fabricated file is
  the command-line file (titles, `#line` paths, `f.titulus`)? Expected:
  titles come from `via_thistle`; confirm by reading the generated
  `_regiones.h` `#line` lines.
- [ ] **Step 2: the member's own derived headers (AUDIENDA 2).** Using
  `fragmenta.thistle` (its regions derive `chorda.h`), print which
  headers its generated `_regiones.c` includes. Record whether a
  member's `_regiones.c` compiles alone (`clang -c` with the project's
  `-I`), i.e. whether it carries its own derivations.
- [ ] **Step 3: plain vs `methodus` in `inv.app` (AUDIENDA 3).** Read
  `_inventarium_colligere` (briar_fabrica.c ~l.280–330) and run the
  fabrica on `salve_vitreum.thistle`: list which regions land in
  `inv.app`. Record the exact predicate a "plain C region" needs
  (expected: `c` region, not `id=`, not `munus="probatio"`, no
  `methodus` attribute).
- [ ] **Step 4: THE PARSE QUESTION.** A region that uses a member's
  typedef (`TextusEventum e;`) must parse. Probe two ways and record
  which works: (a) first parse tolerates the unknown type (as with a
  corpus type before derivation — check `r->silva` diagnostics for a
  one-region file using an undeclared typedef); (b) the second parse
  needs the header's TEXT: find where silva resolves `#include "x.h"`
  for the second parse (`silex_clausuram_e_contentis(piscina, fons,
  …)` in `_parsare`) and whether an in-memory header can be supplied
  (a `SilexFons` overlay, or prepending the member's `_regiones.h` text
  to the prelude instead of an `#include` line). Decision rule: prefer
  prepending the generated header TEXT into the prelude (no silex
  change) if `praeludium` line accounting stays exact; otherwise a fons
  overlay, which touches `lib/silex.c` and owes the silex gates — show
  Fran before choosing it.
- [ ] **Step 5: cost (AUDIENDA 5).** Time `briar -struere` on the lapide
  pair today if reachable read-only (`../lapide-v2/instrumenta/
  textus.thistle`, ~3,800 lines) — copy to the scratchpad, never edit
  lapide. Record build time of textus alone; Task 4 compares.
- [ ] **Step 6: report and re-slice.** Write the numbers and the Step 4
  decision into this plan's "Executed" note under Task 1 and into
  `briar/fontes/briar.worklog.md`. If Step 4 needs `lib/silex.c`, or if
  Step 3 shows `methodus` regions cannot be separated cleanly, re-slice
  Tasks 3–4 and show Fran. Commit the notes:
  `silva.commissio(msg, ['project-specs/briar-plan-9-bibliotheca.md',
  'briar/fontes/briar.worklog.md'], [])`.

**Executed 2026-09-29 — no re-slice; two additions to Task 3.**
Measured:
1. **Non-root file (AUDIENDA 1):** no root assumption — `f.titulus`
   and every `#line` path come from `optiones->via_thistle`
   (`briar_fabrica_titulus`). BUT `briar_fabricare` REFUSES a file with
   neither `principale` nor the window element ("nihil currendum"), and
   a pure-library thistle is a natural member → members need a
   PARTITION-ONLY entry (inventory → partition of plain regions →
   `_caput_fingere` / `_corpus_fingere`), not the whole fabrica. Added
   to Task 3.
2. **Member's own derivations (AUDIENDA 2):** self-contained. The
   generated `_regiones.h` carries the file's derived includes
   (`chorda.h` for fragmenta); `_regiones.c` includes only that header
   and compiles ALONE with the project's `-I`.
3. **Plain predicate (AUDIENDA 3):** `inv.app` DOES hold `methodus`
   regions (only fragments and `munus="probatio"` are excluded). A
   member's plain region = `c`, not `id=`, not probatio, NO `methodus`
   attribute.
4. **Parse question — option A holds, no silex change.** The first
   parse TOLERATES an unknown type (scratch `sonda_ignotus`: silva
   accepts, clang reports). Derivation already collects BOTH implicit
   functions (`est_implicitum`) and unknown type names
   (`EXAMEN_CODEX_TYPUS_NOMINATUS_IGNOTUS`) through `_caput_addere` —
   members' public names only need to join that symbol table
   (symbol → `<m>_regiones.h`); the existing ambiguity check then
   refuses a member name that collides with a corpus symbol, located.
   The SECOND parse cannot `#include` a generated header (silva
   resolves includes through the corpus `SilexFons` only;
   `briar_silva_parsare`'s extra parameter is `excludere`, not an
   overlay) → `_parsare` writes the member header's TEXT into the
   prelude instead of an `#include` line, WITHOUT `#line` (silva rejects
   it): `_lineam_appendere` gains a "no line" mode (via NIHIL). Scratch
   `sonda_praeludium` confirmed silva parses guard + include + typedef +
   prototype + uses in one text. Prelude text is also the path silva's
   semantics handles best (quaestio …RYPY8DFZ: includes via the MAIN
   source misreport implicit calls; via the prelude they do not). The
   compile-side `_regiones.h` keeps a normal `#include
   "<m>_regiones.h"` (the file exists in the project). Added to Task 3.
5. **Cost (AUDIENDA 5):** lapide `textus.thistle` is 7,756 lines (not
   ~3,800), 2 C regions, no `methodus`: cold build 1.0 s, warm run
   0.55 s — paid on EVERY run, since briar parses before computing the
   key. As a member it adds ~0.5 s per importer run (vs a subprocess +
   full log replay per call today). Named, not planned: key from bytes
   BEFORE parsing.

Found on the way (not this plan): the partition emits a region's
`#ifndef … #endif` block twice (directives group + the unit spanning
it), so a typedef inside it is lost (ledger parcum …VF42V). Sourcing
`tools/vexilla.sh` from zsh OUTSIDE the tree (BASH_SOURCE empty) takes
`.` as root and writes a stray `build/vexilla.sigillum` there — removed.

### Task 2: Members — collect, resolve, refuse (briar_plagulae)

**Files:** create `briar/fontes/briar_plagulae.h/.c`,
`briar/probationes/probatio_briar_plagulae.c`, fixtures under
`briar/probationes/fixa/bibliotheca/`; modify the briar test runner's
list if it is not a glob (check `briar/compile_probationes.sh`).

**Interfaces (proposed; Task 1 may rename):**
- Produces:

```c
/* membrum aedificationis: plagula per elementum bibliothecae adducta */
nomen structura {
    constans character* via;          /* absoluta, normalizata */
    constans character* titulus;      /* basis sine ".thistle" */
                 chorda  octeti;       /* in clavem (Task 4) */
          MateriaNodus* documentum;
                   Xar* nexus;         /* BriarNexusRes, silva texta */
    constans character* via_importatoris;
                    i32  linea_elementi;
} BriarMembrum;

/* elementa bibliothecae documenti radicis, TRANSITIVE: membra ordine
 * post-ordinis (folia primum), quodque semel; radix ipsa non membrum.
 * FALSUM: causa + via + linea positae (refutationes par. 3.5). */
b32
briar_membra_colligere (
                      Piscina* piscina,
          constans character* via_radicis,
                          Xar* nexus_radicis,
         InternamentumChorda* intern,
         constans SilexFons* fons,
                         Xar** membra,        /* BriarMembrum */
                       chorda* causa,
         constans character** via_causae,
                          i32* linea_causae);
```

- [ ] **Step 1: fixtures.** `radix.thistle` imports `media.thistle`,
  which imports `folium.thistle` (`via="folium.thistle"`) and
  `sub/ramus.thistle` via `../folium.thistle`-style relative paths;
  `circulus_a` ↔ `circulus_b`; `sine_c.thistle` (prose only);
  `radix_absens.thistle` names a missing file; `radix_non.thistle`
  names `folium.md`. Each fixture's prose states what it proves.
- [ ] **Step 2: the failing gate.** `probatio_briar_plagulae.c`:
  collecting from `radix` gives `[folium, media]` in that order, folium
  once although reached twice (diamond: radix → media → folium and
  radix → folium); run twice with cwd `briar/` and cwd `/tmp`: same
  absolute vias (Review Focus 5); `circulus_a` refused with causa
  containing `circulus_a.thistle -> circulus_b.thistle ->
  circulus_a.thistle` and `partem communem in plagulam tertiam move`;
  missing file, non-`.thistle`, and `sine_c` refused with the §3.5
  wordings and the ELEMENT's line in the importer. Run
  `./briar/compile_probationes.sh plagulae` → red (link error: the
  module does not exist yet — then stub the header+functions returning
  FALSUM so it COMPILES red).
- [ ] **Step 3: implement.** Read each element (`BRIAR_NEXUS_STML` with
  `briar_nexus_titulus_est(r, "bibliotheca")`, attribute `via`),
  resolve with `via_iungere` against `via_directorium(importer)` +
  `via_normalizare` (absolute `via=` kept), check `.thistle` suffix and
  existence, parse the member through the same four calls as
  `tools/briar.c` (arbor, nexus, contextus, silva), recurse with a
  visited set (`TabulaDispersa` keyed by absolute via) and a path stack
  for the cycle message; refuse a member with no plain C region (Task 1
  Step 3's predicate).
- [ ] **Step 4: green, then plant.** Plant: drop the visited-set check →
  the diamond case links folium twice → predicted 1 red (order/count
  assertion). Remove the plant.
- [ ] **Step 5: commit** with gates `briar`, `briar-fumus` (no user
  change yet: members are collected by nobody). Message cites
  feature-requests/015 and the fixtures.

**Executed 2026-09-29.** `briar_plagulae.{h,c}` (`briar_membra_colligere`,
`BriarMembrum`, `BriarMembraCausa`): depth-first walk with an in-progress
stack (cycle, chain named with its remedy) and a finished set (each
member once), post-order output; via resolved against the importer's
directory (`via_directorium` + `via_iungere` + `via_normalizare`); refusals
at the element's line (no via, not `.thistle`, missing, no plain C region,
a member region's own parse error). Gate `probatio_briar_plagulae` 44/44
(diamond + `sub/..` path, same absolute vias from cwd `/tmp`, cycle, four
refusals); plant (both "already a member" checks removed) → exactly the
predicted 1 red (`xar_numerus(membra) == III`). The test's `chdir` is an
examen SUSPECTUM (lexicon lacks it; verdict ACCIPE) - left, not worth a
judge change. Deviation: the recursion is depth-first over imports (depth
= import chain, tiny), not the house's iterative frame stack.

### Task 3: Derive, partition, build (the user-visible task)

**Files:** `briar/fontes/briar_silva.c/.h`, `briar/fontes/briar_fabrica.c/.h`,
`tools/briar.c`, `briar/probationes/probatio_briar_fabrica.c`,
fixtures `vitrea_membrum.thistle`, `duplex_a/b.thistle`,
`membrum_mendosum.thistle`; `briar/MUTATIONES.md`,
`briar/fontes/briar_dialectus.c`.

- [ ] **Step 1: failing fabrica cases.** Fabricate `radix.thistle`:
  generated units include `include/folium_regiones.h`,
  `fontes/folium_regiones.c`, same for `media`; `radix`'s generated
  header includes `folium_regiones.h` ONLY if a radix region uses a
  folium name (derivation, not blanket include); the build line (the
  `aedificare.sh` content) lists `fontes/folium_regiones.c` and
  `fontes/media_regiones.c` exactly once each; the probatio line too.
  `vitrea_membrum.thistle` as a member: its `methodus` regions and
  window element contribute nothing (no `internuntius` in the
  importer's closure). `duplex_a` imports `duplex_b` and both define
  `s32 commune (vacuum)` (public) → refused with both thistle lines;
  both define `staticus s32 adiutor (vacuum)` → builds.
- [ ] **Step 2: run red** (`./briar/compile_probationes.sh fabrica`).
- [ ] **Step 3a (from Task 1): partition-only entry.** A fabrica
  function that runs inventory → plain-region partition (predicate:
  `c`, not `id=`, not `munus="probatio"`, no `methodus`) →
  `_caput_fingere` / `_corpus_fingere` for a member, WITHOUT the main
  rule (a library thistle has no `principale`), returning its two
  units, its exported names and its derived headers.
- [ ] **Step 3b (from Task 1): member header text in the prelude.**
  `_lineam_appendere` gains a no-`#line` mode (via NIHIL); `_parsare`
  writes each derived MEMBER header's text (that mode) into the
  prelude in place of its `#include` line, counting its lines into
  `r->praeludium`. Corpus headers keep `#include`.
- [ ] **Step 3: implement per Task 1 Step 4's decision:** members'
  public names (from their partitions' prototypes/typedefs, excluding
  `principale`) enter `_symbola_legere`'s table as symbol → header
  `<m>_regiones.h`; the second parse sees member header text; the
  fabrica partitions each member's plain regions under its title and
  appends its units to `f.genitae`; build and probatio lines list
  member `_regiones.c`; duplicate-public check over importer + members
  before generation.
- [ ] **Step 4: green; plant:** remove one member `.c` from the build
  line → predicted: fabrica assertion red AND the fumus link fails.
- [ ] **Step 5: end to end.** `./bin/briar briar/probationes/fixa/
  bibliotheca/radix.thistle` prints the folium-computed value;
  `-probatio` passes calling a folium function from the test region.
- [ ] **Step 6: error mapping (Review Focus 4).** `membrum_mendosum`
  (imported by a one-line importer) holds a type error at a known line:
  clang's first error names `membrum_mendosum.thistle:<line>`.
- [ ] **Step 7: user surface.** `## inedita` line (element, semantics,
  feature-requests/015); `-dialectus` line naming the element and the
  `staticus` = private rule; rebake before commit; commit with gates
  `briar`, `briar-fumus`; rebake after.

**Executed 2026-09-29 (two commits: 890dbf92 derivation layer, then the
build + surface).** As in the worklog: members finished after their
dependencies with only their transitive members visible; public names
into the derivation table; own unguarded parse text once each,
dependencies first; partition-only entry; fabrica `_cum_membris` (units,
build lines once each, closure with member includes stripped, duplicate
public names refused before clang at the root's line); tools/briar.c and
the spectator wired (the spectator renders without members on refusal,
F4). End to end: radix 28, its `-probatio` green, same from /tmp,
vitrea member 42 (plain region only, no internuntius), two members'
private `adiutor` builds (5), a member's type error is reported at
`membrum_mendosum.thistle:11`. Gates: plagulae 64/64, fabrica 294/294;
plants: substitution off → red; guard restored → only the structural
check red (my "semantics blind in #ifndef" hypothesis was false — a
foreign-file diagnostic was being counted); a member source dropped
from the lines → the predicted 2 reds. Deviations from the plan text:
member headers carry only PUBLIC prototypes (statics move to the
member's body — found by the first run); a duplicate member TITLE is
refused (generated file names would collide). Not yet: cache key (T4),
amalgam (T4), `-partes` (T4), macros from members (not derived — §9's
bare-macro limitation applies).

### Task 4: Cache key, amalgam, `-partes`, fumus

**Files:** `tools/briar.c`, `briar/fontes/briar_amalgama.c`,
`briar/probationes/probatio_briar_amalgama.c`, `tools/briar_fumus.sh`.

- [ ] **Step 1: key (Review Focus 1), failing first.** Fumus stage
  XXII: build `radix.thistle` (copied into the fumus area with its
  members), note the cache dir, append a comment line to `folium.thistle`
  ONLY, run again: cache dir differs. Run red against Task 3's build
  (members not in the key yet).
- [ ] **Step 2: implement** — members' `octeti` (post-order, each
  length-prefixed) enter the stamp in `tools/briar.c` next to
  `briar_aedificatio_fontes`.
- [ ] **Step 3: amalgam (Review Focus 2).** `probatio_briar_amalgama`:
  amalgamate `duplex_a` (both files carry `staticus s32 adiutor`): the
  output compiles ALONE (`clang` with the house flags) and runs; the
  private-rename pass sees every member's publics (the `Gradus` rule).
  Red first, then implement in `briar_amalgama.c`.
- [ ] **Step 4: `-partes`** lists `bibliotheca <via>: membra N,
  nomina publica M` per member; one fumus grep.
- [ ] **Step 5: fumus stage XXII complete:** run, `-probatio`, key moves
  with a member, `-amalgama` alone compiles + runs, `-partes` line.
  Plant: stamp without members → XXII red at the key check. Commit
  with gates `briar`, `briar-fumus`.

**Executed 2026-09-29.** Measured first: editing folium only left
radix's cache dir unchanged and radix printed the stale 28 (true value
72, shown by `-struere -iterum`); `-amalgama radix` exited 0 but the
file did not compile alone (no member units at all); `-partes` already
showed derived member headers. No re-slice. Built: `briar_membra_stampa`
(briar_plagulae) appends ` membra <16 hex>` = SHA-256 over each member's
title, `\n<length>\n`, bytes, post-order, and returns the stamp
UNCHANGED with no members (no existing cache moves); tools/briar.c
calls it after `fontes briar`. `briar_silva_nomina_statica` (one walk
with the public list, `statica` flag) fills `BriarMembrum.statica`
(static functions AND variables). `briar_amalgamare_cum_membris`:
member headers after the corpus headers and before the root's header,
member bodies after the corpus sources and before the root's regions,
each once; EVERY static of a member renamed `s_<member>` around its
body (no skip list, so the Gradus class cannot recur for members);
`briar_amalgamare` without members on a fabrica that has them REFUSES
(names the member) instead of writing a broken file. `-partes`:
`<via relative to the root dir>\tbibliotheca:membra N, nomina publica
M`. Fumus XXII (17 s whole fumus): run 28, `-probatio`, comment in
folium moves the cache dir, arithmetic change prints 72 without
`-iterum`, amalgams of radix and statica_a compile alone and print 28
/ 5, radix's amalgam test green, three `-partes` lines. Gates:
plagulae 82/82, amalgama 273/273, briar 20/20, fumus sanum. Plants:
(A) stamp without the length prefix -> my FIRST test stayed green
(`ab|c` vs `a|bc`: the next member's title separates them); the test
now uses `X|mediaZ` vs `Xmedia|Z`, which collide without the prefix ->
exactly that one red; (B) no member static renames -> 9 reds (predicted
8; the 9th is `_paria_probare`'s "at least one #define"); (C) briar
built stamping without members -> fumus XXII red at the key check.
Deviations: the amalgam compile-alone check lives in fumus (the unit
gate stays structural, like the corpus fixtures); statica_b/c gained a
same-named `interior` variable `basis` (output still 5). Not done: the
root's OWN statics are still not renamed in the amalgam (pre-existing;
a root static equal to a member's public would collide there - the
normal build usually refuses it too).

### Task 5: Records and seal

- [ ] **Step 1:** spec §3.5 "As built" (deltas from B1–B6, Task 1's
  numbers, the parse decision); status PROPOSITUM → DECISUS rows that
  held.
- [ ] **Step 2:** `briar/CLAUDE.md` (the element, members, the rules),
  `briar/probationes/fixa/FONTES.md` (every fixture), worklog.
- [ ] **Step 3:** this plan's header: DONE + commits; ledger: note on
  the lapide round (feature-requests/015 answered by commit X); memory
  `briar-project.md` line.
- [ ] **Step 4:** commit records; rebake if MUTATIONES moved.

**Executed 2026-09-29.** Spec: §3.5 heading DECISUS (built), an "As
built" block (AUDIENDA answers, the deltas: members finished after
their dependencies with only their own imports visible, unguarded
once-each parse text, public-only member headers, the duplicate-title
refusal, refusal wordings as coded, key, amalgam, `-partes`, what is
not derived), §4.1 stamp additions, P10 in the plan list.
`briar/CLAUDE.md`: a Bibliothecae bullet, the key paragraph (the
"before any parse" claim was no longer true — every run parses before
the key), `briar_plagulae` in the map, a law on member headers; its
gate list said sixteen files and named 16 of the TWENTY that exist
(`bibliotheca`, `dialectus`, `mutationes`, `plagulae` were missing) —
corrected. FONTES.md: the Task 4 uses. Ledger note on the lapide round.

## Spec coverage (self-review against §3.5)

B1 element → T2 (read) + T3 (effect) · B2 word → T2 fixtures · B3 plain
C only → T1 S3 predicate, T3 S1 vitrea member · B4 derivation → T1 S4,
T3 S3 · B5 relative via → T2 S2 (cwd twice) · B6 transitive + cycle
refusal with "partem communem" → T2 S2 · rules: members' principale /
probatio / methodus / fragments never leave → T3 S1 · importer's
`-probatio` sees imports → T3 S5 · duplicate public refused before clang
→ T3 S1 · key holds every member → T4 S1–2 · amalgam privates → T4 S3 ·
`-partes` → T4 S4 · refusals (five wordings) → T2 S2 + T3 S1 ·
AUDIENDA 1–5 → T1 S1–S5 · cycle note → T2 S2 message.
