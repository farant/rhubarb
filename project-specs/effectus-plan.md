# effectus plan — what a script touches (fabrica slice 4)

> **For agentic workers:** execute task by task with the executing-plans
> discipline (test first, run red, implement, run green, plant, commit),
> INLINE on main, ONE TASK PER TURN with Fran's approval, no subagents.
> Several tasks OPEN with a reading or a measurement; a result outside
> the named branches is shown to Fran before work continues. Steps use
> checkbox syntax. Written 2026-10-05 from `effectus-spec.md`
> (de2d3621; interview Q1–Q11 in `effectus-interview.md`; A1–A5 decided
> = every recommendation). On approval: one opus per task in a park;
> `silva.commissio(opus=ID)` closes each.

**Goal:** every tracked `.sh` gets a static EFFECT SUMMARY - each read,
write, exec, source, listing, test and environment read, with its
evaluated path - in a language-neutral STML dialect judged by a canon;
an interposer oracle proves the summary covers what bash actually does;
exemplar lint rules run over it; fabrica keys verdicts on it (genus
`effectus`), so a bash read like `$(cat flag.txt)` makes a gate STALUM
without waiting for an audit; `fontationes` retires.

**Architecture:** the fontationes evaluator (`crusta_fontationes.c`)
moves into `crusta_effectus.c` and grows from two site kinds to eight.
It emits an `StmlNodus` tree in dialect `effectus` whose site elements
carry materia's uniform position view (`sedes`, `octeti`), so the
existing exemplar machinery (`materia_exemplaria`) and `<tolera>`
excuses work on it unchanged. Command semantics are data
(`crusta/effectus_mandata.stml`). The oracle is a macOS dylib
(interposition) plus a normalizer that emits the SAME dialect, so
comparison is set arithmetic. fabrica's genus `effectus` calls
`bin/effectus` through the existing seam slot (today `fontationes`).

**Tech stack:** C89 in Latin (`latina.h`), `stml.h`, `canon.h`,
`materia_exemplaria.h`, crusta (`crusta_arbor_parsare`,
`crusta_verbum_staticum`, `crusta_imperium_titulus`); credo gates in
crusta's runner; bash for wrappers and the oracle driver; macOS dyld
interposition (oracle only).

**Spec:** `project-specs/effectus-spec.md` (§0 measurements, §I
framing + soundness rule + slice 2, §II dialect, §III command table,
§IV analyzer, §V lint, §VI census + oracle, §VII fabrica genus, §VIII
order, §IX review focus, §X done, §XI decided, §XIII plan-time
corrections v2).

## Global constraints

- C89 under `tools/vexilla.sh` flags. Latin identifiers; latina macros
  are forbidden identifiers (`nomen`, `casus`, `per`, `duplex`,
  `vacuum`, `interior` …); single capitals and every Roman numeral
  0–3999 are macros. `chorda` is not NUL-terminated; `i32`/`i64` are
  UNSIGNED. Names use `titulus`, never `nomen`.
- **Attribute and element names inside the dialect are data, not
  identifiers** - but `per` is also an attribute name in §II.3 and a
  latina macro: in C it only ever appears inside string literals
  (`"per"`), never as an identifier.
- Lines ≤ 72 columns; `./silva/formator.sh -vitia` per touched file.
- No `<word` tag openings in C comments or string literals (examen
  lexicon); no `dir/*` inside block comments (`-Wcomment`).
- New identifier words: `git add -N` new files, then
  `./oratio/vocabula.sh -nova` must say NOVA 0 (glossary entries are
  fine, Fran 10-03).
- POSIX `.c` files include `postulata_posix.h` FIRST.
- Commit only through `silva.commissio(msg, viae, portae)` with
  explicit paths; never Fran's files (FAQ.md, gesta/annales/*). Long
  commits launched detached and polled. **Freeze the tree while gates
  run**; draft the next task in the scratchpad.
- Every new gate is born red; every fix is proven by a PLANTED FAULT
  that compiles, red predicted before the run, removed after.
- **Soundness rule (spec §I):** no flow-insensitive fact removes a path
  from the fabrica key. A step that would do so is a defect, whatever
  the test says.
- The analyzer spawns nothing; gates spawn nothing except the oracle
  tool's own `-probare` (C14: goldens for fixed fixtures, live house by
  hand).

## Review Focus

1. **A site class the analyzer never emits** (e.g. `<<<` mistaken for
   a read, a glob inside quotes treated as a listing) → silently wrong
   summary. Test: T3 one fixture case per §IV.2 row, each with its
   CONTRARY (quoted glob = no enumeratio; `/dev/null` = no scriptura).
2. **Positions that do not match crusta's** → excuses never apply, or
   apply to the wrong site. Test: T6 Step 4 (an excuse above
   `sera_vetus () {` removes exactly its three sites; moved one line
   down, it removes none and is reported dead).
3. **The table and the oracle agreeing because they share the table**
   (spec §IX.2) → named limit; T5 Step 6 checks the rows it can
   against Homebrew coreutils run under the interposer.
4. **A flow-insensitive fact shrinking the key** → stale verdict.
   Test: T7 plant `build/vexilla.sigillum` edited under RECENS →
   STALUM.
5. **Judge-time cost** → the toml key derivation must stay inside the
   2 s budget. Measured in T7 Step 7 against fontationes' time.

## File structure

| file | responsibility |
|---|---|
| `effectus.canon` (NEW, root), `canones.registrum` | T1 dialect canon; `<effectus>` registry line |
| `crusta/probationes/fixa/effectus/*.stml` (NEW) | T1 hand summaries; T5 oracle goldens |
| `crusta/probationes/probatio_crusta_effectus_dialectus.c` (NEW) | T1 canon gate (+ T2 table section) |
| `crusta/effectus_mandata.stml` (NEW) | T2 command table |
| `crusta/instrumenta/mandata_census.c` (NEW) | T2 precise command census |
| `crusta/fontes/crusta_effectus.{h,c}` (NEW) | T3 analyzer |
| `crusta/probationes/probatio_crusta_effectus.c` (NEW) | T3 site classes, attributes, totality over the house |
| `crusta/instrumenta/effectus.c` (NEW), `crusta/effectus.sh` (NEW) | T3 CLI (`-census` in T6) |
| `crusta/fontes/crusta_fontationes.{h,c}` | T4 becomes a projection |
| `crusta/instrumenta/interpositio.c` (NEW), `crusta/effectus_oraculum.sh` (NEW) | T5 oracle |
| `crusta/lintrum/effectus-*.stml` (NEW), `crusta/fontes/crusta_facies.{h,c}` | T6 rules + summary route |
| `include/fabrica.h`, `lib/fabrica.c`, `tools/fabrica.c` | T7 genus `effectus`, `catenae`; T8 remove `fontationes` |
| `toml/aedificatio.stml`, `tools/iudicium_fumus.sh` | T7 switch + plants |
| `crusta/fontes/crusta_effectus.worklog.md` (NEW), spec §XII, MEMORY, ledger | per task; T8 closes |

---

### Task T1: the dialect and its canon

**Opens with a reading:** `aedificatio.canon` and `fabrica.canon`
whole (the house dialect-canon idiom: `elementum`, `attributum genus=`
`electio`, `liberum`), and `canon_iudicare` usage in one existing gate
(`lib/silex.c` reads `aedificatio.canon`). No code before it.

**Files:** `effectus.canon`, `canones.registrum` (one line
`<effectus>\teffectus.canon`), `crusta/probationes/fixa/effectus/
{catena,cursor,omnia}.stml`, `crusta/probationes/
probatio_crusta_effectus_dialectus.c`.

- [ ] **Step 1: Hand summaries first** (the contract before the
  emitter). Three documents:
  - `omnia.stml` - one of every element of spec §II.2 with every
    attribute of §II.3 in at least one value;
  - `cursor.stml` - the toml runner fragment of spec §II.1 written
    out fully (two `processus`);
  - `catena.stml` - a pipeline `cat a | sort > b` (a lectio, a pure
    command, a scriptura) - the smallest real summary.
  Positions use materia's view: `sedes="L:C-L:C" octeti="B-B"` (spec
  §XIII.1), not `linea`/`initium`/`finis`.
- [ ] **Step 2: The failing gate.** `probatio_crusta_effectus_dialectus.c`:
  read `effectus.canon` (`canon_legere`), judge each fixture
  (`canon_iudicare`) → zero vitia; registry lookup by root element
  (`canon_registrum_quaerere_radice`) → `effectus.canon`. Run
  `./crusta/compile_probationes.sh effectus_dialectus` → red (canon
  file missing).
- [ ] **Step 3: Write `effectus.canon`**: root `effectus` (attrs
  `lingua` electio `bash`, `radix` via, `registrum-sigillum`
  textus); `processus` (attr `radix`, liberum of every site element);
  the eight site elements with their attributes; closed `electio`
  lists for `forma`, `resolutio`, `classis`, `per`, `scripta_in_ambitu`,
  `assignatum`; `sedes` and `octeti` necessarium on every site. Every
  element and attribute gets a `nota` (Latin) saying what it means
  language-neutrally (spec §II.2 last paragraph).
- [ ] **Step 4: Green**, then **plants** (each compiles, red predicted):
  (a) `classis="aliena"` in `omnia.stml` → one vitium naming the
  attribute; (b) a `lectio` without `sedes` → one vitium; (c) the
  registry line removed → lookup assertion red. Restore each.
- [ ] **Step 5: Commit** (`effectus.canon`, `canones.registrum`, the
  fixtures, the gate; gate `crusta`).

### Task T2: the command table, seeded by a precise census

**Opens with a measurement:** `crusta/instrumenta/mandata_census.c`
walks every tracked `.sh` (outside `oracula/`) through
`crusta_arbor_parsare` + `crusta_imperium_titulus`, separating
(i) titles defined as functions anywhere in the scope chain, (ii)
builtins/reserved, (iii) externals; prints sites and files per
external title. Branch: the spec §0 heuristic numbers are within ~20%
for the top 20 → proceed; otherwise show Fran the corrected table first
(the spec's §0 gets the precise numbers either way).

**Files:** `crusta/instrumenta/mandata_census.c`, a wrapper line in
`crusta/effectus.sh` (`-mandata`), `crusta/effectus_mandata.stml`,
`effectus.canon` (table section `mandata`), the T1 gate.

- [ ] **Step 1: Census tool + run.** Record top-60 externals in spec
  §XII (dated).
- [ ] **Step 2: Table rows for every external with ≥ 5 sites**, by the
  grammar of spec §III (`argumenta` / `primum` / `ultimum` / `reliqua`
  roles, `optiones_cum_valore`, `optio_lectio`, `purum`, `ignotum` +
  `causa`). Each row's semantics checked against the command's
  `--help`/man page, not memory (cite in the row's `nota` where an
  option matters: `grep -f`, `sed -i`, `sort -o`, `cp -r`, `find`).
- [ ] **Step 3: Canon section + gate.** The table judged clean; plants:
  a role value outside the electio; a duplicate `titulus` (the gate
  asserts uniqueness itself - canon cannot say it, as aedificatio's
  duplicate-title rule).
- [ ] **Step 4: Commit** (gate `crusta`).

### Task T3: the analyzer

**Opens with a reading:** `crusta_fontationes.c` whole (1,910 lines)
+ its worklog; list in the worklog which functions MOVE unchanged
(scope collection, fixpoint, value evaluation, cwd), which GENERALIZE
(site collection), which are NEW (redirections, tests, globs, table
interpretation, env reads, attributes, STML emission).

**Interfaces (`crusta/fontes/crusta_effectus.h`):**

```c
/* crusta_effectus.h - summarium effectuum scripti (effectus-spec)
 *
 * Quae scriptum (et omnia quae fontat aut exsequitur) legit,
 * scribit, exsequitur, fontat, enumerat, probat, et quas variabiles
 * ambitus legit - via cuiusque aestimata. Dialectus 'effectus'
 * (effectus.canon), lingua neutra: lintrum et fabrica idem
 * vocabularium legunt quod silva (C) olim emittet. */
#ifndef CRUSTA_EFFECTUS_H
#define CRUSTA_EFFECTUS_H
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "stml.h"

/* Summarium derivare. radix: via ABSOLUTA arboris; scriptum:
 * relativa aut absoluta; mandata: tabula IAM LECTA (NIHIL =
 * crusta/effectus_mandata.stml legitur). Reddit radicem
 * <effectus>; NIHIL = memoria deficit aut scriptum illegibile
 * (causa_out). Totalis: plagula fontata illegibilis aut parsura
 * non sana = situs <ignotum> cum causa, numquam NIHIL. */
StmlNodus*
crusta_effectus_derivare (
               Piscina*  piscina,
    InternamentumChorda* intern,
    constans character*  radix,
    constans character*  scriptum,
            StmlNodus*   mandata,
    constans character** causa_out);

#endif
```

**Files:** `crusta/fontes/crusta_effectus.{h,c}`,
`crusta/probationes/probatio_crusta_effectus.c`,
`crusta/instrumenta/effectus.c`, `crusta/effectus.sh` (wrapper on the
`fontationes.sh` model: `cursor_instrumentum_struere`, exec).

- [ ] **Step 1: Failing tests, one case per spec §IV.2 row** in a fake
  tree under `crusta/build/effectus_fixa.<pid>` (fontationes' test
  idiom): each asserts the element, `via`, `forma`, `resolutio`,
  `classis`, `per`/`mandatum`, and its CONTRARY (quoted `"*.c"` = no
  enumeratio; `> /dev/null` and `2>&1` = no scriptura; `<<<` = no
  lectio; `X=1 cmd` not a definition; `$?`/`$#` no ambitus_lectio; a
  scope function `sera_capere` is descended, not `ignotum`; an
  executed script's scope does not see the caller's variables).
  Plus `scripta_in_ambitu` both ways (cursor_communis shape: written
  line 58, read line 102 → verum; read only → falsum) and the cwd rule
  of spec §IV.3 (catena `cd`, top-level `cd` before the site, `cd`
  inside `if` ignored). Run red (header missing).
- [ ] **Step 2: Move + generalize** per the T3 reading; emit STML with
  `sedes`/`octeti` taken from the owning crusta node (materia's
  writer computes them - reuse, never recompute).
- [ ] **Step 3: Green.** Then the house: every tracked `.sh` summarized
  (`crusta_effectus_derivare` in a loop, like `probatio_crusta_corpus`)
  → zero NIHIL, every summary judged clean by `effectus.canon` (the
  drift guard: an element the analyzer invents fails here).
- [ ] **Step 4: Plants:** (a) redirection `>` classified as lectio →
  named cases red; (b) the scope fixpoint cut to one pass →
  cursor_communis-shape case red; (c) `sedes` omitted on `probatio`
  → canon drift guard red.
- [ ] **Step 5: CLI** `./crusta/effectus.sh <script>` prints the
  summary; run on `toml/compile_probationes.sh`, eyeball against spec
  §0's 62 observed effects, note gaps in the worklog (T5 makes this
  mechanical).
- [ ] **Step 6: Commit** (gate `crusta`; portae_debitae decides more).

### Task T4: fontationes as a projection

**Files:** `crusta/fontes/crusta_fontationes.{h,c}`.

- [ ] **Step 1:** `crusta_fontationes_derivare` reimplemented as: derive
  the summary, keep `fontatio` + `exsecutio` sites, map `classis` →
  `CrustaFontatioGenus` (arbor+fontatio/script = FASCICULUS,
  instrumentum_domus = INSTRUMENTUM, build exsecutio = PRODUCTUM,
  externa fontatio = EXTERNUM, resolutio nulla = IRRESOLUTUM), same
  sort and uniqueness. Header and output format unchanged.
- [ ] **Step 2:** `probatio_crusta_fontationes` UNCHANGED and green;
  `./crusta/fontationes.sh toml/compile_probationes.sh` output
  byte-identical to before (saved in the scratchpad first).
- [ ] **Step 3:** delete the duplicated evaluator code; `wc -l` before
  and after in the worklog.
- [ ] **Step 4: Plant:** the classis map swaps PRODUCTUM and
  INSTRUMENTUM → fontationes gate red. Restore. Commit (gates
  `crusta`, `iudicium-fumus`, `toml` - the genus still calls it).

### Task T5: the oracle

**Opens with names/placement confirmed** (A2 decided: house C89 under
`crusta/instrumenta/`), and with a reading of how a macOS-only tool is
fenced in the house (platform header; examen's POSIX lexicon has no
`dyld` interpose section - the section attribute and `getprogname`
may need an `oracula/`-style exclusion: if examen or the Latin lint
refuses them, show Fran before choosing).

**Files:** `crusta/instrumenta/interpositio.c`,
`crusta/effectus_oraculum.sh`, `crusta/probationes/fixa/effectus/
oraculum/` (fixture scripts + goldens).

- [ ] **Step 1: Interposer** = the spike (spec §0) in house C: open,
  openat, stat, lstat, access, fstatat, faccessat, opendir, execve;
  ARGV lines; shebang redirect to `/opt/homebrew/bin/bash`; a first
  line per pid with cwd and PATH; one `write()` per line (O_APPEND).
- [ ] **Step 2: Normalizer** (in `crusta/instrumenta/effectus.c`,
  mode `-observata <log>`): log → dialect `effectus` with
  `per="observatum"`, filters of spec §VI.2, house-binary pids
  dropped, argv of table commands interpreted by the SAME table,
  read-before-write per pid listed to `build/effectus/ante_scripta.tsv`.
- [ ] **Step 3: Comparison** (`-comparare <static> <observed>`): every
  observed site covered by a static site of the same kind (path equal,
  glob match, prefix) → exit 0; else each uncovered one printed by
  name, exit 1.
- [ ] **Step 4: Fixture goldens**: 8–10 small scripts (one per site
  class + a sourced lib + a `#!/bin/bash` child + a read-before-write)
  run by `effectus_oraculum.sh -scribere`; the gate
  (`probatio_crusta_effectus` section) compares static summaries to
  the committed goldens, spawning nothing.
- [ ] **Step 5: Live**: `effectus_oraculum.sh -domus
  toml/compile_probationes.sh registrum` → zero uncovered, or each
  fixed in the analyzer with a T3-style case first. Record counts in
  spec §XII.
- [ ] **Step 6: Table check where possible** (Review Focus 3): for
  commands Homebrew coreutils ships (`gcat`, `ghead`, `gsort`, `gwc`,
  `gcut`, `gtail`…), run a fixture under the interposer with the
  g-variant and compare its observed reads with the table row's claim.
  Rows that cannot be checked are listed in the worklog as unverified.
- [ ] **Step 7: Plants:** the shebang redirect disabled → the child
  fixture's sites uncovered, red; a static case dropped → the
  comparison names it. Commit.

### Task T6: lint rules, excuses, census, verdict chains

**Opens with a reading:** `crusta_facies.c` whole (how rules are read,
annotations collected, excuses applied by owning-node range, dead
excuses judged), and `tools/diagnostica` call path for `.sh`.

**Files:** `crusta/lintrum/effectus-{irresolutum,build-sine-domino,
mandatum-ignotum}.stml`, `crusta/fontes/crusta_facies.{h,c}`,
`crusta/instrumenta/effectus.c` (`-census`), `tools/sera.sh`,
`tools/vexilla.sh` (excuses), `include/fabrica.h` / `tools/fabrica.c`
(`catenae`).

- [ ] **Step 1: `bin/fabrica catenae`**: scripts reachable from every
  `iudicium` action's `fontationes`/`effectus` ingressus (both genera
  until T8), one path per line. Test in `probatio_fabrica` style.
- [ ] **Step 2: Summary route in facies** (spec §XIII.2): for a `.sh`
  in a chain, take its sites from the chain ROOT's summary (scope
  context of the caller); else summarize it alone. Sites are grouped
  per `plagula` into one document per file (exemplaria: one document =
  one plagula); rules `effectus-*` run over that document; excuses from
  the file's own crusta annotations, unchanged machinery. Gravitas
  `erratum` in chains, `monitum` elsewhere (A4).
- [ ] **Step 3: Rules + fixtures**, each with a positive and a negative
  fixture (cursor_communis shape = `build-sine-domino` silent;
  read-only build file = fires).
- [ ] **Step 4: Excuses** in the toml chain: one above
  `sera_vetus () {` (Review Focus 2 test: moved one line → dead and
  reported), `sera_tenens`, `sera_dimittere`; vexilla's stamp. Each
  excuse's causa says WHY (the inventory row).
- [ ] **Step 5: Census** `./crusta/effectus.sh -census` →
  `build/effectus/census.tsv` (spec §VI.1); counts per kind/class/
  resolution/rule into spec §XII; ledger rows only for decisions
  (excuse classes, table additions).
- [ ] **Step 6: Plants:** an excuse removed → commit lint blocks on
  the toml chain (erratum) while the same site in a non-chain script
  only warns. Commit.

### Task T7: fabrica genus `effectus`

**Files:** `include/fabrica.h` (seam slot `effectus`), `lib/fabrica.c`
(`_genus_effectus` beside `_genus_fontationes`), `tools/fabrica.c`
(calls `crusta/effectus.sh -clavis <via>`: a deterministic line
format of the digest rows of spec §VII), `toml/aedificatio.stml`,
`tools/iudicium_fumus.sh`, `aedificatio.canon` (`effectus` genus
value).

- [ ] **Step 1: Key lines.** `effectus.sh -clavis` prints, per site:
  kind, class, path, and the digest RULE (bytes / provenance /
  owner / names / existence / env / IGNOTUM / excused) - fabrica
  digests per rule; IGNOTUM lines make the action IGNOTUM naming the
  site. Read-and-written build files: bytes (soundness rule).
- [ ] **Step 2: Tests in `probatio_fabrica`** for each digest rule,
  incl. absence (`probatio` of a missing file: creating it → STALUM).
- [ ] **Step 3: `toml/aedificatio.stml`**: `genus="fontationes"` →
  `genus="effectus"`.
- [ ] **Step 4: iudicium-fumus** - the porta script switches genus;
  new plants: `$(cat flag.txt)` edited → STALUM directly (the AUD case
  stays, now as "audit still congruent"); `[ -f absent ]` created →
  STALUM; a read-and-written build state file edited → STALUM.
- [ ] **Step 5: P2/P7 kept green** (script edit → IGNOTUM/STALUM;
  `source "$NESCIO"` → IGNOTUM naming the site).
- [ ] **Step 6: Plant** the soundness rule broken (read-and-written
  build file skipped) → the state-file plant goes RECENS: red.
- [ ] **Step 7: Measure** judge time RECENS (target < 2 s) and key
  churn over the next commits (spec §IX.3); record. Commit (gates
  `fabrica`, `iudicium-fumus`, `toml`, `crusta`).

### Task T8: retire `fontationes`, close

- [ ] **Step 1:** grep-free check that no declaration uses
  `genus="fontationes"` (`bin/fabrica` declarations dump); remove the
  genus, the seam slot, `crusta/fontationes.sh`,
  `crusta/instrumenta/fontationes.c`; keep `crusta_fontationes.{h,c}`
  only if a caller remains (else fold its test into
  `probatio_crusta_effectus`).
- [ ] **Step 2:** `aedificatio.canon` drops the value; a declaration
  using it now fails the canon (plant = the old toml line restored →
  red).
- [ ] **Step 3:** spec §XII As built; `crusta_effectus.worklog.md`;
  crusta/CLAUDE.md section; MEMORY; ledger (park closed, slice 2 filed
  as a desideratum under region 'flow analysis' with the
  `ante_scripta.tsv` list and the census's unresolved count). Commit.

## Not in this plan (stated)

- Ordering / control flow (slice 2, spec §I).
- A C emitter (silva) - vocabulary only.
- Migrating other gates to verdict actions (…J6HF) - effectus makes it
  cheaper; the batch job stays separate.
- Effects inside external commands beyond the table; `eval`, `xargs`,
  `find -exec` bodies remain `ignotum`.
