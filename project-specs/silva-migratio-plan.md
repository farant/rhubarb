# Silva → materia (phase 5) — plan

> Inline on main, one task per turn with Fran's approval, no subagents.
> Spec: `project-specs/silva-migratio-spec.md` (decisions D1–D5 DECIDED
> 2026-09-24). Ledger: decree …XX0BZ, park …FE9E "phase 5" intra region
> "c89 parser"; one `opus` per task intra the park, closed by
> `silva.commissio(opus=ID)`. Narrative: `materia/phase-log.md`
> (INTENTIO with this plan's commit, RELATIO at each step boundary).

> **Shape.** Step 0 (T1–T5) is written in full: it changes no silva
> code. Steps 1–4 (T6–T13) are written at task level, each with its
> gate; every task opens with a measurement that may resize it. Steps
> 5–6 (T14–T19) are SKETCHES, re-issued as a plan B at the step-4
> boundary — their bodies depend on what steps 1–4 find (the
> css-arbor-plan lesson: bodies written early go stale).

**Global constraints.** C89 in Latin via latina.h, house flags
(`tools/vexilla.sh`); `i32`/`i64` UNSIGNED; chorda not NUL-terminated;
new C files via `scribe`; gates born red by a plant THAT COMPILES;
worklog first, gates last; formator `-scribere` then `-vitia`; words via
`./oratio/quaere.sh` then `./oratio/vocabula.sh -nova`; never
`silva.Editio` on a `.sh` (edit runners with perl or by hand); every
new gate registered in pythonica's gate table AND as a row of the
'suitae probationum' inventory (lenses `currit binaria` / `tegit
viae`), or `portae_debitae` cannot owe it. **Every commit from T6 on
is green**: silva suite, shim, oracle gate (T3), M3 runner (T1),
consumer suites (officina, briar, saltuarius) and — when a materia
file moves — every materia client suite (css, md, oratio, html,
crusta). After a lib/ touch: rebake briar, briar-spectator, silex.

**Freeze discipline until T13.** silva/fontes changes only through
this plan; each commit that touches a substrate module states its
replay verdict in the phase-log (rule from the 09-01 audit). Intended
divergences from the oracle are NAMED in the dispares file (T3), never
counted away.

## Review focus

Failure modes the spec implies that no single task's tests would
catch; each is pinned in the task named.

1. **The oracle judging itself** — the live side accidentally linking
   the pinned amalgam, or the pinned side linking live objects.
   Pinned in T2 (`nm` check: the oracle binary defines no symbol from
   `silva/build/*.o`, links nothing but the extracted amalgam).
2. **A named divergence that stopped diverging** — a dispares row
   that no longer differs hides the next real divergence at the same
   file. Pinned in T3: a stale row is RED, like crusta's oracle.
3. **STML files written before phase 5 that no longer read** — the
   house has committed STML (canon fixtures, arbor documents);
   migration must read pinned-silva STML. Pinned in T3: the live
   reader reads every oracle STML file and the comparator agrees.
4. **Consumer walking a node field the facade renamed** — briar's
   `_errorem_quaerere` (`->loci`, `numerus_locorum`), compendium
   (`->pater`). A facade of functions does not cover field access.
   Pinned in T5 (lens `campi directi`) and checked row by row in each
   switch task.
5. **A header-only facade edit that rebuilds nothing** — runners
   track `.c` mtimes; a switch that changes only `silva_token.h` can
   report green against stale objects. Pinned in T7: the task's gate
   run starts from removed objects (`silva/build`, `materia/build`,
   consumer builds), stated in the commit message.

---

## Step 0 — measure and freeze (no silva change)

### Task 1: the M3 bars, re-measured, behind one runner

**Why.** Spec §2: the bars are dated 2026-08 and the suite has grown
(57 probatio files, gate 54/54). The acceptance gate of T13 must be
one command whose numbers are today's.

**Measure first** (worklog `materia/phase-log.md` + this plan's
status header): for each bar find the script and the number it
prints today. Candidates from the 09-24 survey — CONFIRM each:

| bar (2026-08) | candidate source |
|---|---|
| subtree round trip 281/281 | `silva/probationes/probatio_silva_arbor_circuitus.c` (PORTA M1) |
| plain-C 78/78 | `probatio_silva_fidelitas.c` (corpus `probationes/fixa/roundtrip`; pins moved 155→156 on 09-01) |
| latinized 154/154 | `probatio_silva_arbor_plagula.c` (PORTA M2) or `aequivalentia.sh` (156/156) |
| hospes 39/39 | `silva/amalgamare.sh` (hospes stage; phase-log reads 24/24 later) |
| adversarial 24/24 | `probatio_silva_arbor_parsura.c` T7b "casus adversarii" |
| haruspex 243 TUs | `silva/haruspex.sh` |

A bar whose source cannot be found is reported to Fran, not
re-invented.

**Files:** create `silva/m3_probare.sh`; modify
`pythonica/silva.py` (gate `silva-m3`); inventory row in 'suitae
probationum' (`tegit viae`: `silva/fontes/`, `materia/fontes/`).

**Contract.** Runs the six bars from their own scripts (no copy of
their logic), parses each script's own count line, prints

    m3 subtree      281/281
    m3 plain-c       78/78
    ...
    VERDICTUM: M3 6/6

and compares each count to a pin table at the top of the runner
(today's numbers). Exit 0 all at pin · 1 a bar short OR above pin (a
rise moves the pin by hand, like html's oracle) · 2 NOTHING RAN (a bar
script missing, a count line not found — never read as zero).

**Red.** Plant 1: pin one bar at N+1 → exit 1 naming it. Plant 2:
rename a bar's count line in the parse pattern → exit 2, not a pass.
Plant 3 (compiles): a one-byte drop in one roundtrip fixture read by
the plain-C bar → that bar short, exit 1.

**Done when** the runner is green at today's numbers, the three plants
went red, the spec §2 bar table is updated in place with dates and
script names, and the M3 wall time is recorded.

### Task 2: `oraculum_silvae` — the frozen oracle as a program

**Why.** Spec §3: mid-migration "compare against silva" compares the
new code to itself. The oracle is silva at a pinned commit, built from
its amalgam, run as a separate process.

**Files:** create `materia/instrumenta/oraculum_silvae.c` (via
`scribe`), `materia/oraculum_silvae_struere.sh`,
`materia/instrumenta/oraculum_silvae.worklog.md`.

**The pin.** One line at the top of the build script:
`PIGNUS=<commit>` = HEAD when T2 starts (today `7a4847b0`). Every
commit before T6 carries the same silva, so the choice is not
delicate; after T6 it must never move forward. The script extracts
`git show $PIGNUS:silva/amalgama/silva.{c,h}` into
`build/oraculum_silvae/<PIGNUS>/`, compiles silva.c once (3.6 MB
object, cached by pin), and links the tool. Changing the pin is a
commit that says why.

**One source, two builds.** The tool source includes

    #ifdef ORACULUM_PIGNUS
    #include "silva.h"                /* amalgama ad pignus */
    #else
    #include "silva_parsare.h"        /* silva viva: capita fontium */
    #include "silva_scribere.h"
    #include "silva_arbor.h"
    #include "silva_c89_semantica.h"
    #endif

The pinned build (`-DORACULUM_PIGNUS`, links ONLY the extracted
amalgam object) is `build/oraculum_silvae/<PIGNUS>/oraculum`. The live
build (links `silva/build/*.o` with the shim's exclusion list + house
libs) is `build/oraculum_silvae/vivum`. The live build is a CONSUMER of
the facade from T7 on: the facade must keep these four headers'
names working, and this tool proves it every run.

**Output.** `oraculum [-stml <dir>] [-legere <dir>] <plagula>...`,
one line per file, tab-separated, deterministic:

    <via>  octeti=<n>  circuitus=idem|dispar  emissio=<h>  stml=<h>  errores=<n>  semantica=<h>|-

- `circuitus` = `silva_scribere_fontem(silva_parsare(x)) == x`;
- `emissio` = FNV-1a 64 over the emitted bytes, 16 hex (hash written
  in the tool: the pinned build may link nothing but the amalgam);
- `stml` = FNV-1a over `silva_arbor_scribere_parsuram`'s text;
  `-stml <dir>` also writes it to `<dir>/<via con / → %>.stml`;
- `errores` = count of ERROR nodes;
- `semantica` = FNV-1a over the sorted lines `titulus<TAB>genus` of
  the symbols `silva_c89_semantica_analysare` finds; `-` when analysis
  refuses (the refusal is then the value compared);
- `-legere <dir>` (live build only): read each file's pinned STML
  with `silva_arbor_legere_parsuram` and print
  `comparator=aequales|<first divergent field>` via
  `silva_arbor_parsurae_aequales` against the live parse.

**Red / checks.** (a) Isolation (Review focus 1): the build script
asserts the pinned link line names exactly two objects (the tool's
and the amalgam's), and `nm -u` on the tool's object lists only
`silva.h` API and libc. (b) On 5 files (one plain, one latinized, one with a
macro-expanded region, one syntax error, `lib/piscina.c`) the two
builds print identical lines. (c) Plant (compiles): in the LIVE
`silva_scribere.c` drop the last trivium of a comment → live line
`circuitus=dispar`, pinned line unchanged — the two processes are
really different programs. Remove the plant.

**Done when** both builds print identical lines over the 5 files, the
plant separated them, and the worklog records the pin, the build time
and the per-file cost.

### Task 3: the oracle gate — live silva vs pinned silva

**Files:** create `materia/oraculum_probare.sh`,
`materia/oraculum_silvae.dispares`; modify `pythonica/silva.py` (gate
`oraculum-silvae`), inventory row in 'suitae probationum' (`tegit
viae`: `silva/fontes/`, `materia/fontes/`, `silva/grammatica/`).

**Corpus** = the shim's corpus (400) ∪ the M3 corpora T1 named, one
sorted list, deduplicated, printed as a count before any comparison.

**Contract.** Build both (T2), run pinned with `-stml`, run live with
`-legere`, compare column by column. The dispares file is the named
exceptions:

    # via <TAB> columna <TAB> causa (commissum aut ledger id)

Exit 0 when every difference is named and every named row still
differs · 1 an unnamed difference (prints via, column, both values,
and for `stml` the comparator's first divergent field) OR a named
row that no longer differs (STALE — Review focus 2) · 2 NOTHING RAN
(either binary missing, corpus empty, a line count mismatch).

**Measure at birth.** Whether silva's and materia's STML TEXTS agree
byte for byte on the corpus (the shim can print both). If they do not,
the `stml` hash column cannot survive the T9/T10 switches; it is then
demoted to the comparator column only, and that decision is recorded
at the top of the dispares file and in the phase-log.

**Red.** Plant 1 (compiles, live `silva_scribere.c`): drop one byte on
emission of a line-comment → exit 1 naming files. Plant 2: add a
dispares row for a file that agrees → exit 1 "STALE". Plant 3: corrupt
the pinned build path → exit 2.

**Done when** the gate is green with an EMPTY dispares file at birth
(the pin is today's silva), the three plants went red, the wall time
is recorded, and `portae_debitae.sh silva/fontes/silva_scribere.c`
lists `oraculum-silvae`.

### Task 4: replay inventory

**Data, no C.** Inventory 'phasis V: replicatio silvae' (`ordo_genus:
commissum`) intra the park's region; rows = every commit touching
`silva/fontes` since the fork (2026-08-27; ten on 09-24: `160c680f
5b6b049b 40beb200 99e25047 05315f0f d514c5a3 064635d2 e4546b8e
1d062a18 1373a3e8`) + one row `CR` (latent CR-comment bug, materia B6
`cr` attribute). Lenses: `modulus substrati` (ita-non: does the diff
touch token/nodus/scribere/arbor/aequalitas/annotationes?),
`replicatum` (ita / non-debitum / debitum), `causa` (textus). Fill by
reading each diff (`git show --stat` then the substrate hunks).

**Done when** every row has all three cells and the `debitum` rows
(expected: CR only) are each linked to the switch task that pays them
(CR → T9).

### Task 5: consumer inventory — the facade surface

**Data + one measurement.** Inventory 'phasis V: consumptores silvae'
(`ordo_genus: via`); rows = files OUTSIDE `silva/` and `materia/`
that use the substrate. Find them with the tools, never grep:
`./silva/nexus.sh <fn>` for each substrate function, and
`./silva/selecta.sh` for member access through `SilvaNodus*` /
`SilvaToken*` (`->loci`, `->pater`, `->numerus_locorum`, …). Include
amalgams that embed silva callers (`officina/amalgama/officina.c`).

Lenses: `functiones` (textus: the substrate functions used),
`campi directi` (textus: fields accessed; empty = none), `per facadem
transit` (ita-non, filled by T7–T11), `suita` (textus: the suite that
proves the file).

**Produces for T7.** The facade surface: the exact list of substrate
functions and fields used outside silva, written into this plan under
T7 before T7 starts (spec §2 estimates ~24 functions).

**Done when** the inventory renders, every row names its suite, and
the surface list is in the plan.

---

## Step 1 — the C89 frontend becomes production code

### Task 6: `silva_frons` — promote the shim

**Measure first:** which parts of `materia/instrumenta/shim_c89.c`
(1,462 lines) are conversion + frontend hooks (production) and which
are gate driver (comparison, reporting). Record the split in the
worklog before moving a line.

**Files:** create `silva/fontes/silva_frons.{h,c}` (via `scribe`) =
the token tail (`cauda lexematis`), silva→materia conversion, the
registered hooks (origin write/read, sections `<fontes>` /
`<regio-*>`, extent lookup, reinserenda collector); move
`materia/probationes/lexicon_c89.{h,c}` → `silva/fontes/` (its own
header says phase V moves it) with its generator's output path, and
point css/html/materia runners at the new path; `shim_c89.c` shrinks
to the driver; silva's runner compiles `materia/fontes` objects;
amalgamare's excludenda manifest lists `silva_frons` until T13 (gate 0
of `amalgamare.sh` must stay green).

**Gate:** shim 400/400 unchanged (now exercising production code),
oracle gate green (no silva emission changed), M3 runner green,
materia client suites green (lexicon moved). Plant: break one hook's
origin write in `silva_frons.c` → shim red.

**Done when** the shim contains no conversion code and all gates are
green.

---

## Step 2 — switch modules, one per commit, leaves first

Every switch task has the same frame:

1. **Measure**: `./silva/nexus.sh` / `selecta.sh` census of the
   module's API and field uses inside silva, in consumers (T5 rows),
   and in the amalgam manifest. If the count is more than ~2× the
   estimate, stop and re-slice with Fran.
2. **Facade**: the module's silva header keeps its NAME and becomes
   `#include "materia_<m>.h"` + `nomen Materia… Silva…;` + thin
   wrappers or macros for the surface T5 listed. silva's `.c` copy is
   deleted in the same commit (not at T13) unless a frontend part
   stays, which moves to `silva_frons`.
3. **Gates, from removed objects** (Review focus 5): silva suite,
   shim, oracle gate, M3 runner, consumer suites (officina/legatus,
   briar, saltuarius), materia client suites if materia changed.
4. **Consumer rows** of T5 touched by the module get `per facadem
   transit = ita`.
5. Phase-log line; replay verdict.

### Task 7: token

C89 data (origin, expansion identity, …) leaves `SilvaToken` for the
frontend tail (`silva_frons`); `SilvaToken` = `MateriaToken`. Expect
the widest field-access edit of the step (lexema, expandere, glr
read token fields) — the measure decides whether accessors or direct
tail access.

### Task 8: nodus

`SilvaNodus` = `MateriaNodus`; the five nodus query families listed as
unported in 2026-09 (~4.2k lines with quaestio) are measured here and
either ported onto `MateriaNodus` inside silva or scheduled into T12.

### Task 9: scribere (+ the CR fix)

`silva_scribere` = `materia_scribere` + the frontend hooks. The latent
CR-comment bug is FIXED here, on purpose, with its own test
(a C file whose comment holds `\r`: STML round trip keeps it); its
oracle divergence is NAMED in the dispares file with this commit. The
T4 row `CR` → `replicatum = ita`.

### Task 10: arbor + aequalitas

The big one: `silva_arbor` 7,063 → `materia_arbor` 3,965 + frontend
sections (`_parsura_*` functions — the layer boundary measured
2026-08-27 — move to `silva_frons`); `silva_arbor_aequalitas` →
`materia_arbor_aequalitas` with the phase-5 hook named at the B1 port
(frontend tail excluded from FIDELITAS). If the measure says two
commits, arbor first, aequalitas second; both green.

### Task 11: annotationes

`silva_annotationes` → `materia_annotationes`; STML-in-comment
annotations (`<tolera>`, `<contractus>`) read by examen and legatus
are the consumers to watch.

---

## Step 3 — quaestio

### Task 12: `silva_quaestio` onto `MateriaNodus` (D3a)

Port `silva_quaestio` (1,881) and whatever of the nodus query
families T8 scheduled; it stays in silva. Gates as step 2, plus
`probatio_silva_quaestio*`, `legati quaestio` over
`silva/quaestiones.stml` (every named query returns the same rows
before and after — record the before rows at the task's start).

---

## Step 4 — delete and seal

### Task 13: seal — M3 acceptance, freeze ends

Remaining substrate copies deleted; amalgam regenerated (`silva.h` =
frontend + facade + the materia modules it needs; excludenda cleaned
of `silva_frons`); **M3 runner green at T1's pins; oracle gate green
with only named divergences**. The shim is retired (it now compares
materia with itself); its corpus lives on in the oracle gate. Freeze
notice removed from `silva/CLAUDE.md`; `materia/CLAUDE.md` and the
materia-spec §7/§10 marked as-built; phase-log RELATIO; park note.
Then re-issue T14–T19 as `silva-migratio-plan-B.md` from what was
learned.

---

## Step 5 (5.x) — the payoff (SKETCH, re-issued after T13)

- **T14** C89 registry DECLARATION (`silva/grammatica/c89.registrum.stml`
  like crusta's) → `coquere`; declared diagnostics and seals apply to
  C89; `lexicon_c89` generated from it.
- **T15** GLR records the death token (all heads dead) + expected kinds
  if cheap; emits through `emissa`: primary "hic exspectatur", relata
  "hic coepit" at the unit start; examen, legati, briar print it via
  `lib/excerptum`. Closes briar bugs/001; the clang relay (desideratum
  …SQ9) becomes the second opinion.
- **T16** bugs/009 (`va_arg(va, character*)`) and **T17** bugs/010
  (`__attribute__((…))`) as ordinary frontend fixes, each a named
  oracle divergence.
- **T18** `latina.h` description line (silva's embedded copy and
  `probatio_silva_contextus` pin move together).

## Step 6 — cleanup (SKETCH)

- **T19** LR toolkit's home decided; duplicated code deleted; MAP,
  census, MEMORY updated; lapifex landmine (MG4) re-checked; park
  …FE9E and decree …XX0BZ closed with pointers.
