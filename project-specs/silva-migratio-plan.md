# Silva → materia (phase 5) — plan

> Inline on main, one task per turn with Fran's approval, no subagents.
> Spec: `project-specs/silva-migratio-spec.md` (decisions D1–D5 DECIDED
> 2026-09-24). Ledger: decree …XX0BZ, park …FE9E "phase 5" intra region
> "c89 parser"; one `opus` per task intra the park, closed by
> `silva.commissio(opus=ID)`. Narrative: `materia/phase-log.md`
> (INTENTIO with this plan's commit, RELATIO at each step boundary).

> **STATUS.** T1 DONE 2026-09-24: `silva/m3_probare.sh`, gate
> `silva-m3`, 6/6 in ~65 s (bars and sources in spec §2). Two
> corrections to T1's text as written, both below in place: pins are
> FIXED or LIVE (a live corpus rising is not a failure), and plant 3
> goes in the emitter (a byte dropped from an INPUT fixture still
> round-trips). T2 DONE 2026-09-24: `oraculum_silvae` pinned at
> `7a4847b0` + live, 390/390 identical over the shim corpus, pinned
> 10 s / live `-legere` 29 s; as-built notes under T2. T3 DONE
> 2026-09-24: gate `oraculum-silvae`, 468 files in 40 s, green with an
> empty dispares file; the shim gate now runs `-stml` (it had run one
> oracle of three). T4 DONE 2026-09-25: replay inventory 01M3B6KRAH —
> silva owes nothing; five items cross INTO silva at T10 (CR moved
> there from T9; STML compression = a decision before code). T5 DONE
> 2026-09-25: consumer inventory 01M3B7A8DG — 10 functions but ~894
> field sites; nodes/values pass by typedef, 78 token-field sites need
> an accessor (47 of them `origo`). Surface written above T7. T6
> SPLIT (Fran). T6a DONE 2026-09-25: silva links materia's substrate
> (`silva/materia_substratum.sh`, 23 build sites); found and fixed
> renominare never relinking on object changes. T6b DONE 2026-09-25:
> `silva/fontes/silva_frons.{h,c}` (shim now a 372-line driver),
> lexicon in `silva/fontes`, amalgam unchanged (frons excluded until
> T13); shim 402 + 401/401/401, oracle clean. T7 SPLIT (Fran). T7a
> DONE 2026-09-25: ~430 token-field sites on accessors (321 driver,
> ~43 hand, 63 inside macro arguments the census could not see);
> zero direct uses by two angles; oracle, M3, shim unchanged; last
> amalgam regeneration before the freeze. T7b DONE 2026-09-25:
> SilvaToken IS MateriaToken + C89 tail; oracle clean over 470, M3
> 6/6, shim unchanged; lexing bytes −29%; `genus`'s type (enum → s32)
> needed `silva_token_genus` at 33 sites; amalgam FROZEN until T13.
> T8 DONE 2026-09-25: `silva_nodus.h` is a facade (typedefs + macro
> aliases; `SilvaNodus` a MACRO because officina uses the struct tag
> against both live and frozen headers); ~7,000 sites untouched; S27
> ("no parent at construction") fell harmlessly — commissio re-parents
> everything; the five origin query families stay in silva, unifying
> them with materia's uncus is desideratum 01M3BHHJQA (step 5).
> T9 DONE 2026-09-25: silva's writer delegates to `materia_scribere`
> with C89 hooks over silva's own tokens (no conversion); registry and
> `SilvaScriptura` are typedefs; materia gained `reinserenda_finire`
> (trailing reinserts — a header guard's `#endif` — were unreachable on
> materia's whole-file path, which the shim had never exercised).
> T10 MEASURED + RE-SLICED 2026-09-25 (Fran, decree …MQF): T10a–d.
> T10a DONE 2026-09-25: materia arbor writer/reader sessions, public
> cursor (gap re-seek), template definitions, re-pointing, the
> value-position join materia never did, comparator hook.
> T10b DONE 2026-09-25: shim parity over ALL 11,711 top-level nodes
> (found and fixed a read-side extent bug, 20 nodes); C89 hooks over
> silva's own tail proven byte-identical, then silva's node writer and
> reader switched; materia's parent policy in silva_commissio; CR/NUL/
> escape fixed at node level with tests, P7 measured inert.
> T10c step 1 DONE 2026-09-25: parsura writer/reader on the sessions,
> compression a C89 post-pass; silva_arbor.c 7,078 → 3,832; oracle stml
> identical over 470 on the first run; wish 01M32TA81Z closes. Step 2
> (retire frons conversion + shim) next.
> T10c DONE 2026-09-25: step 2 retired the frons CONVERSION (silva_frons
> is now only the C89 hooks over silva's own tail) and the shim + gate
> `materia-shim` (it would compare silva with itself; the oracle covers
> bytes, STML, reading and comparison against pinned silva).
> T10d DONE 2026-09-25: `silva_arbor_aequalis` = materia's walk + one
> C89 hook (standard, longitudo, continuations, missing tail); the
> owed "full-trivia mode" was a misread (materia always compared trivia
> in full); materia gained `materia_arbor_lexemata_aequalia_fronte` for
> the parsura comparator's loose tokens; porta M1 widened 281 → 492
> subtrees (M3 pin moved). T10 complete; T11 (annotationes) next.
> T11 DONE 2026-09-25 (Fran chose (C) after the measurement): the
> per-comment kernel (decoration, `<`+letter anchor, parse) is materia's
> (`materia_annotationem_legere`, `MateriaDecoratio`); collection from
> C89 sources, unit attachment and nid identities stay in silva. Parity
> 162/162 annotations over 1,545 files; crusta diagnostics identical
> over 250 .sh. T12 (quaestio) next.

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
is green**: silva suite, shim (until T10c), oracle gate (T3), M3 runner (T1),
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
(today's numbers). Exit 0 all at pin · 1 a bar short, a FIXED bar off
its pin in either direction, or a bar's probatio failing · 2 NOTHING
RAN (a bar script missing, a count line not found — never read as
zero). *As built:* pins are FIXED (frozen corpus: exact) or LIVE
(`lib/*.c`, haruspex's headers: all pass and count ≥ floor; a rise is
printed as a note — a new library must not break the gate).

**Red.** Plant 1: pin one bar at N+1 → exit 1 naming it. Plant 2:
rename a bar's count line in the parse pattern → exit 2, not a pass.
Plant 3 (compiles): in the LIVE emitter, skip the EOF token's trivia
in `silva_scribere_fontem` → plain-C short, exit 1. *As built:* the
first text said "drop a byte from a roundtrip fixture" — that cannot
go red (the fixture is the input; its round trip is still exact).
Measured: 77/78 plain, 182/182 latin (a final newline rides on the
last token, not EOF); hospes unmoved (it judges the committed amalgam).

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

*As built* (worklog `materia/instrumenta/oraculum_silvae.worklog.md`):
live headers are `silva_contextus.h`, `silva_c89_oraculum.h`,
`silva_scribere.h`, `silva_arbor.h`, `silva_c89_semantica.h`,
`silva_tabulae_c89.h` (not `silva_parsare.h`); `errores` =
`parsura->numerus_errorum`; default parse carries the latina.h
lexicon, `-nudum` = the shim's bare mode; semantics hashed in INDEX
order; `-legere` compares the pinned and live documents BOTH loaded by
the live reader (a fresh parse vs a document was never measured
equal); isolation = every symbol the pinned tool needs is defined by
the amalgam or on a libc list, planted with a house call; one arena
per file (`vacare` retained 4 GB after `lib/biblia_dr.c`: 8 min 47 s
→ 29 s).

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

*As built* (worklog `materia/instrumenta/oraculum_silvae.worklog.md`):
corpus 468 (the roundtrip fixtures live at the repo root,
`probationes/fixa/roundtrip/`); the gate builds silva's objects itself
before judging; plus a POSITIVE control (a named divergence passes).
STML texts measured byte-equal (shim `-stml` 399/399), so the `stml`
column stays. Found: the registered `materia-shim` gate had run
without `-stml` — byte oracle only; fixed in pythonica's gate table.

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

*As built* (inventory 01M3B6KRAH, 15 rows × 4 lenses: `modulus
substrati`, `directio`, `replicatum`, `causa`): the 10 silva commits
owe NOTHING — two touch substrate modules, `064635d2` (format pass:
`differre_git` 150/150 symbols cosmetic) and `1373a3e8` (arbor
refactor: none of its symbols exist in materia). The debt runs the
OTHER way: four materia writer fixes silva lacks (CR, NUL, the escape
ladder of wish 01M32TA81Z, crusta's P7 guard) and one silva feature
materia lacks (STML compression). All five land in T10; CR moved
there from T9 (the bug is in projection, not emission). Rows for the
"other direction" were not in the plan's text — they are the same
class ("what must cross at a switch") and were found by the
duplicate check on filing.

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

### Task 6 — SPLIT by its measurement (Fran, 2026-09-25)

The measure step found that silva's runner and 22 silva tools compile
and link EVERY `silva/fontes/*.c` (by design — `tools/silva_fontes_
generare.sh`: a new module is never silently missed), while their
generated dependency lists cover `lib/` only. So `silva_frons.c` in
`silva/fontes` would break 23 builds. Materia joining silva's build is
unavoidable (T7 cannot happen without it) — it gets its own commit,
with no behaviour change, so any red is a build fault and nothing else.

**T6a — materia joins silva's build.** One shared definition of the
substrate modules silva links (token, nodus, scribere, arbor,
arbor_aequalitas, lexicon), used by the silva runner and every tool
that links all of `silva/fontes`; the shim runner's `silva/build/*.o`
glob excludes those objects (else it links them twice). Gate: every
affected tool builds and runs, silva suite, shim, oracle, M3 — all
unchanged.

*T6a as built:* helper `silva/materia_substratum.sh` (six modules,
header guard over include/ and materia/fontes, equal-second recompiles)
sourced by the silva runner (step 2a, `FRACTA:` on failure), the 20
tools, and `tools/silex_struere.sh`; `amalgamare.sh` needs nothing yet
(the amalgamator's own sources do not glob fontes — what goes INTO the
amalgam is T6b's). Shim excludes `materia_*.o` from its silva/build
glob; the live oracle keeps them. Every tool binary verified by `nm` to
contain materia — which found `renominare` relinking only on its OWN
source/headers: an object rebuilt from a changed silva `.c` never
reached the binary. Fixed (relink when any object is not older).
`suitae probationum`: the silva runner now covers `materia/fontes/*`.

**T6b — the promotion** (below, as first written). Split measured
2026-09-25: ~1,050 production lines (token tail, conversion, 12
hooks, `FRONS_C89`, `_nodum_radicis`) · ~380 driver · 25 dead
(`_radix_silvae`, never called — dropped). The global `SHIM` becomes
an opaque context passed through the hooks' `datum`:

    nomen structura SilvaFrons SilvaFrons;              /* opaca */
    SilvaFrons*   silva_frons_creare (Piscina*, constans SilvaExpansio*);
    MateriaValor  silva_frons_valorem_convertere (SilvaFrons*, SilvaValor);
    MateriaNodus* silva_frons_nodum_convertere   (SilvaFrons*, SilvaNodus*);
    vacuum silva_frons_scripturam_parare (SilvaFrons*, MateriaScripturaConsilium*);
    b32    silva_frons_arborem_parare    (SilvaFrons*, MateriaArborConsilium*);
    i32    silva_frons_lexemata_numerus  (constans SilvaFrons*);

The generated lexicon moves to `silva/fontes/silva_lexicon_c89.{h,c}`.

### Task 6b: `silva_frons` — promote the shim

*As built* (worklog `silva/fontes/silva_frons.worklog.md`): as below,
plus — every silva/fontes build site gained `-I materia/fontes` and a
header guard over it; the amalgam excludes `silva_frons.c` and
`silva_lexicon_c89.c` until T13 (regenerated byte-identical); the shim
driver compiles under the HOUSE flags (its remission's reason moved
into silva_frons); two API additions (`silva_frons_causa`,
`silva_frons_nodus_radicis`); the generator moved to
`silva/instrumenta/`. A plant in the macro attribute showed only the
TEXT comparison guards frontend attributes (round trip and comparator
agree with themselves) — after T13 that falls to the oracle's `stml`
column.

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

### The facade surface (measured by T5, 2026-09-25)

Inventory 'phasis V: consumptores silvae' (01M3B7A8DG, 24 files outside
silva/ and materia/: 16 with uses, 8 naming types only). Two halves,
and the spec's "~24 functions" had the weight on the wrong one:

**Functions: 10**, in 9 files (nexus, cross-checked by word grep — the
same 9 files). Same-named in materia: `silva_valor_lista_obtinere` (48
sites), `silva_valor_lista_numerus` (45), `silva_nodus_liberi` (1).
Facade or `silva_frons` must supply: `silva_token_radix` (12 — the
origin chain's root, C89 data), `silva_nodus_extensionem` (8),
`silva_nodus_extensionem_lineis` (4), `silva_valor_extensionem` (1)
(materia has `materia_tractus_nodi`/`_lexematis` — same question,
different shape: measure at T8), `silva_commentarium_ducens` (1,
compendium — C89 leading comment), `silva_quaestio_compilare` /
`_exsequi` (1 each, legatus — T12).

**Direct field access: ~894 sites** (`renominare -membrum` dry runs,
semantic). Node and value fields are NAME-IDENTICAL in materia
(`genus numerus_locorum loci pater`; `genus datum.{nodus token lista
index}`) — 176 + 482 sites pass through `nomen Materia… Silva…;`
untouched, given `SILVA_VALOR_*` aliased to `MATERIA_VALOR_*` (same
order; materia appends REFERENTIA). **Token fields are NOT** — 236
sites, of which **78 need an accessor or an edit**:

| silva field | consumer sites | in materia |
|---|---:|---|
| `origo` | 47 (saltuarius_origo 20, officina.c 9, officina_indicium 9, saltuarius_liber 8, legatus 1) | absent — frontend tail |
| `longitudo` | 16 | absent (= `valor.mensura`?) |
| `spatia_ante` / `_post` | 12 (saltuarius_liber) | `MateriaToken**` + `numerus_*`, not `Xar*` |
| `initium_lineae` | 3 | a bit in `vexilla` |
| `standard`, `scissurae` | 0 | absent |

plus `genus`: an enum in silva, a lexicon index in materia
(`lexicon_c89` is generated from silva_token.h's enum — confirm the
numbering is identical before relying on it). Silva's OWN uses of
these fields are not counted here (T7's measure step).

**Build modes** (lens `aedificatio`): briar, saltuarius, aedilis
compile against the AMALGAM, oratio against live headers, officina
both — amalgam consumers feel a switch when the amalgam regenerates.

### Task 7 — SPLIT by its measurement (Fran, 2026-09-25)

Measured (`renominare -membrum` over silva's 156 files + T5): ~1,300
compatible token-field uses (`valor genus linea columna byte_offset
fons_index`) pass a typedef untouched; **~408 do not** — 330 inside
silva (31 files; silva_arbor.c 74, silva_token.c 35, semantica 31) +
78 outside: `origo` 142+47, `spatia_ante/_post` 91+12, `longitudo`
33+16 (NOT `valor.mensura`: a token holding a line continuation has
source bytes its value lacks), `initium_lineae`/`scissurae`/`standard`
64. Favourable structure: ONE allocator (`silva_token.c`, no token
ever held by value — materia's tail-after-token model fits) and ONE
trivia builder (`silva_lexema.c`); everything else reads.

**T7a — the accessor seam, no representation change.** Accessors
(`silva_token_origo` → `SilvaOrigo*`, `_longitudo`, `_standard`,
`_scissurae`, `_initium_lineae`, trivia `_ante_numerus`/`_ante`/
`_post_numerus`/`_post`, setters for the ~30 writes); every one of the
~408 sites rewritten onto them — a Python driver (sites from the
semantic census, base expression by balanced back-scan, four shapes)
applied through one `silva.Refactio`; unclassifiable sites refused and
listed, done by hand. Nets: `-Werror`, a SECOND census reporting zero
direct uses outside the accessor bodies, oracle + M3 byte-for-byte
unchanged. The amalgam regenerates normally (accessors join
`silva.h`) — its consumers move onto the accessors in this task.

**T7b — the representation swap behind the seam.** `SilvaToken =
MateriaToken` + C89 tail (`origo longitudo standard scissurae`); the
allocator calls `materia_token_creare`; the lexer hands trivia over as
finished arrays. Decisions (Fran): the amalgam is FROZEN at T7a's
state from T7b until T13 (its consumers use only accessors, which the
frozen amalgam implements over the old representation; hospes keeps
judging the committed amalgam; silva/CLAUDE.md's "regenerate after
every fontes edit" suspended with a note); the frontend's own token
tail (`SilvaFronsCauda`) merges with silva's at T9/T10, when the
writers consume silva's tokens directly — not at T7b.

### Task 7a/7b: token (original text)

C89 data (origin, expansion identity, …) leaves `SilvaToken` for the
frontend tail (`silva_frons`); `SilvaToken` = `MateriaToken`. Expect
the widest field-access edit of the step (lexema, expandere, glr
read token fields) — the measure decides whether accessors or direct
tail access.

### Task 8: nodus

`SilvaNodus` = `MateriaNodus`; the five nodus query families listed as
unported in 2026-09 (~4.2k lines with quaestio) are measured here and
either ported onto `MateriaNodus` inside silva or scheduled into T12.

*As built (2026-09-25).* Measure: ~7,000 identifier sites (SilvaValor
1,695, SilvaNodus 1,684, silva_valor_nihil 863, silva_nodus_ponere 432,
…) — all pass by alias, no re-slice. Facade: `SilvaValor`,
`SilvaValorGenus`, `SilvaLocusSpecies`, `SilvaListaProspectus` are
typedefs; `SilvaNodus`, the 11 `SILVA_VALOR_*`/`SILVA_LOCUS_*` constants
and 14 core functions are `#define`s (nexus follows the expansion, so
callers show under materia's names). silva_nodus.c 990 → 646 lines. The
one semantic change — materia parents at construction, silva's S27
never did — is invisible after commissio (it re-parents every reachable
node; nothing reads `pater` earlier); three S27 assertions rewritten.
The query families are "ported onto MateriaNodus inside silva" in the
facade sense only: their unification with `MateriaOrigoUncus` /
`materia_tractus_nodi` changes extents consumers see, so it is
desideratum 01M3BHHJQA, a step-5 candidate — not T12 (quaestio is a
different module). The non-canonical-arm parent policy (silva NIHIL vs
materia's reader parenting the whole tree, named in silva_frons.c)
stays owed to T10.

### Task 9: scribere

`silva_scribere` = `materia_scribere` + the frontend hooks. Byte
emission only. *(T4 moved the CR fix to T10: the bug is in the STML
projection — `_octetum_exuere`/`cr` has 10 sites in `materia_arbor.c`
and 0 in `materia_scribere.c`.)*

*As built (2026-09-25).* Measure: 72 call sites of the three entry
points (48 `fontem`), all kept by signature; consumers outside silva
are the shim, the oracle and saltuarius (frozen amalgam). The two
walkers were the same algorithm; the differences were all hooks
(origin chain, extent lookup — silva also searches by CONTAINMENT for
stringified arguments, the frons hook does not — and continuation
emission) plus fontem's C89 reinsert collection and EOF. Found: the
shim compared only `silva_scribere_valorem`, so materia had never
written a whole file; a reinsert after the last tree token had no way
out → `MateriaScripturaConsilium.reinserenda_finire` (additive, default
off). Registry types became typedefs (identical layout), so the writer
takes silva's registry without conversion. "Frontend hooks" live in
`silva_scribere.c`, reading silva's tail by accessors; the frons keeps
its own hooks for CONVERTED tokens until T10 retires conversion.

### Task 10: arbor + aequalitas

The big one: `silva_arbor` 7,063 → `materia_arbor` 3,965 + frontend
sections (`_parsura_*` functions — the layer boundary measured
2026-08-27 — move to `silva_frons`); `silva_arbor_aequalitas` →
`materia_arbor_aequalitas` with the phase-5 hook named at the B1 port
(frontend tail excluded from FIDELITAS). If the measure says two
commits, arbor first, aequalitas second; both green.

**Owed here by the T4 inventory** ('phasis V: replicatio silvae'):
- **Materia writer fixes silva lacks** — arrive with the switch, each
  with its OWN test and, if the corpus reaches it, a named oracle
  divergence: CR (`cr` attribute; a C comment holding `\r`), NUL
  (`nul`; a C file holding a NUL byte), the closing-sequence escape
  ladder (silva_arbor.c:985 still refuses; closes wish 01M32TA81Z; a
  C comment holding `</lex-commentarium>`), and crusta's P7
  mixed-edge-newline guard (whether C89 reaches it: MEASURE).
- **Retire the frons conversion (from T9).** silva's writer now
  reads silva's own tokens; the frons still converts to materia tokens
  with its own tail (`SilvaFronsCauda`) for the STML path, with a
  second set of hooks — one of which (`_extentum_quaerere`) lacks
  silva's containment fallback for stringified arguments. When the
  STML writer/reader consume silva's tokens directly, the tails merge
  and the duplicate hooks go.
- **Parent policy for non-canonical AMBIGUUS arms — DECISION
  (named in silva_frons.c, re-confirmed at T8):** silva's commissio
  leaves arm roots with `pater` NIHIL; materia's reader parents the
  whole tree. Once silva reads through materia's reader, one policy
  wins (78 header files diverged on this when the frons mirrored
  silva).
- **A silva feature materia lacks — DECISION before code:** the
  whole-file writer's STML COMPRESSION (spaces `<<#@post/ante-spatia>>`,
  macro leaves `@m-`, parameter families; census
  `SilvaArborCensusCompressionis`). materia has none, and the oracle's
  `stml` column hashes exactly this output. Port it into materia (STML
  templates are not C-specific — a general writer option) or keep it
  as a C89 post-pass in `silva_frons`. Measure the compressed vs
  uncompressed document size on the corpus first.

*Measured (2026-09-25) — more than 2× the estimate; re-slice proposed
to Fran before code.*

- **Shape of silva_arbor.c (7,078):** vocabulary ~430 · node writer
  ~1,740 · reader + position fixups ~1,810 · whole-file `parsura`
  writer/reader (sources, directives, regions, gaps, anchors) ~2,170 ·
  STML compression ~1,000 (macro leaves ~435, parameter families
  ~485, space templates ~80). Roughly 3,700 lines have a materia
  twin; ~3,300 are C89 (origin/extent/continuations, the parsura
  document, compression).
- **Consumers:** none outside silva/materia. Inside: arbor tool,
  hospes, canon coquere, the exemplaria/canon suites (exact STML),
  oracle, shim. The oracle's `stml` column is the gate.
- **The shim's STML parity covered only each file's FIRST top-level
  node** (`silva_frons_nodus_radicis`). Node-level parity over whole
  files is unproven — the T9 lesson again (the byte shim never ran
  `fontem`).
- **materia's arbor writer/reader are one-shot** (`scribere_nodum`,
  `proicere_nodum`, `legere`); silva's parsura writer runs ONE writer
  session over the document (pass I counts the whole tree, then each
  top-level node, directive reinsert and the EOF tail becomes its own
  element with its own anchor). The lexeme seam
  (`materia_arbor_lexema_scribere/legere`) is only reachable from
  inside hooks. → materia needs a SESSION API (writer and reader).
- **Compression:** materia has the space templates; it lacks macro
  leaves and parameter families, both POST-PASSES over the finished
  STML tree (they need the writer's piscina/intern and its sedes pairs
  to re-point substituted elements). Parameter templates are C89
  grammar, hard-coded; leaves are a general hoisting mechanism keyed on
  `<expansio>`. Size on lib/*.c (182 files): compressed 466 MB vs
  uncompressed 522 MB — saving 10.7% overall, median 15.3% per file
  (4–23%); documents are ~34× source either way.
- **Comparator:** silva's = materia's + three C89 token fields
  (`standard`, `longitudo` under FIDELITAS, continuations) — materia's
  header already names the missing "frons comparison hook". Whole-file
  comparison (`parsurae_aequales` + regions, ~330) is C89.
- **Arm-parent policy, by experiment:** commissio changed to parent
  non-canonical arm roots (materia's policy) → oracle clean over 470
  (all five columns), M3 6/6, officina 15/15, silva 53/54 — the only
  failure was probatio_silva_commissio's assertions pinning the old
  policy. Nothing else observes it. Reverted.
- **Owed writer fixes:** the corpus reaches none (0 files with CR,
  NUL, mixed endings; the one `</lex-` is `</lex-int>` inside a
  comment, not a comment's closing sequence). Each needs its own test.

*Re-sliced (Fran, 2026-09-25; decree …MQF: compression stays a C89
post-pass in silva; materia's parent policy is adopted).*

**T10a — materia arbor SESSIONS (substrate only).** Measured need:
silva's parsura writer/reader drive ONE session per document; materia
exposes only one-shot entries. Everything required already exists
inside materia_arbor.c — T10a exposes it, and the one-shot entries are
re-expressed on it (their output byte-identical: every materia client
suite + shim are the proof).
- Writer: `materia_arbor_scriptor_creare(piscina, consilium, &causa)`,
  `_scriptor_numerare(sc, valor)` (pass I, whole document, before any
  writing), `_scriptor_fontem_ponere(sc, fons)` (default source: `f`
  omitted when equal — silva sets fons_princeps AFTER pass I),
  `materia_arbor_nodum_scribere(sc, nodus)` → element,
  `materia_arbor_lexema_scribere` (exists), `_templa_spatiorum_scribere
  (sc, involucrum)` (the definitions of the `<<#@post/ante-spatia>>`
  calls materia already writes), `_scriptor_repungere(sc, tabula)`
  (compression's element substitutions re-point the value/element
  pairs; key = old element pointer bytes), `_scriptor_finire(sc,
  involucrum, textum)` → reference custody, census, text with value
  positions (`sedes_valorum` — materia collected the pairs but never
  joined them: the join is ported from silva).
- Reader: `materia_arbor_lector_creare(piscina, intern, consilium,
  vitium)`, `_lector_fontem_ponere`, `materia_arbor_fragmentum_aperire`,
  `materia_arbor_nodum_legere`, `materia_arbor_lexema_legere` (exists),
  `_lector_finire` (references). Positions: public `MateriaArborFixura`
  + `_fixura_initiare(f, consilium, lacunae)`, `_fixura_ancoram_legere(f,
  elementum)` (anchor attributes + gap-index RE-SEEK, silva's T7 fix),
  `_positiones_nodi/_lexematis`. Envelope validation (tag, grammar,
  seal, template expansion) stays with the client that owns the
  envelope.
- Comparator: `materia_arbor_aequalis_fronte(a, b, modus, frons,
  differentia)` with a token-comparison hook (C89: standard,
  longitudo, continuations); `materia_arbor_aequalis` = no hook.

*As built (T10a).* As designed. Also found: silva's comparator compares
TRIVIA tokens with the full token comparison (positions under
FIDELITAS, source, provenance, standard, continuations); materia
compares trivia by genus+value only. **T10d owes** a full-trivia mode
in materia's comparator (or the hook extended to trivia) before
`silva_arbor_aequalis` can delegate. **→ WRONG (found at T10d):** read
from the header prose, not the code — materia's trivia comparison has
always been the full token comparison, hook included. Nothing was owed.

*As built (T10b).* The shim's parity became per-node (11,711 nodes) and
gained two columns: silva's own subtree round-trip, and the new path
(materia + C89 hooks over silva's tail) against the old silva writer.
Found on the way: read tokens never found their invocation's extent
(frons searched a list only conversion fills) — 20 nodes lost
`initium_lineae`; fixed in the frons, pinned. The new hooks live in
silva_frons.c (`silva_frons_arborem_silvae_parare`); read extents live in
the frons context, so subtree reads no longer need an expansion (silva's
old reader refused 101 nodes). Parent policy: 806 arm divergences →
silva_commissio adopted materia's; 0. Writer fixes at node level: CR,
NUL, escape — each a round-trip test; P7 measured inert for C89 (no
lexeme value carries a newline at its edge). **Owed to T10c:** the
parsura path still uses the old internals (still refuses the escape
case — wish 01M32TA81Z closes there); the old converted-tail hooks and
the shim's frons columns retire with conversion.

*As built (T10d).* `silva_arbor_aequalis` delegates to
`materia_arbor_aequalis_fronte` with a static C89 hook (`standard`,
`longitudo` under FIDELITAS, `scissurae/*`, and `lexema/cauda` for a
token lacking the C89 tail); the modus and differentia types are
typedefs of materia's. `parsurae_aequales` stays in silva (regions,
branches, directive laminas), its nodes through `silva_arbor_aequalis`
and its loose tokens (laminas, EOF token) through the new
`materia_arbor_lexemata_aequalia_fronte` — decided (A) over a private
token copy (Fran, 2026-09-25). silva_arbor_aequalitas.c 1,026 → 638.
First-named field may differ from old silva when several diverge (hook
runs after materia's fields); every plant diverges in one field and
passed unchanged; new hook plants reddened by an always-accepting hook.
Porta M1 now probes EVERY subtree of its genera (281 → 492, same time)
in place of the retired shim's all-node sweep; M3 pin moved with cause.
Gates: silva 54/54, materia 14/14, oracle 470 clean, M3 6/6.

### Task 11: annotationes

`silva_annotationes` → `materia_annotationes`; STML-in-comment
annotations (`<tolera>`, `<contractus>`) read by examen and legatus
are the consumers to watch.

*Measured (2026-09-25).* Same name, different jobs. silva's collector
(911) reads C89-only sources — token stream, invocation roots (origin
chain), preprocessor-consumed directive lines, the EOF tail — and
attaches to TOP-LEVEL UNITS (SUPRA/INTERIOR/PLAGULA), which examen's
contractus pass, the nid index and hospes read; it also extracts nid
identities and mint offsets (~360). materia's (410) descends the tree
and attaches to the OWNING node + scope, with a one-string prefix. On
C89 it would find nothing (first byte `/`; a `/*` prefix leaves `*/`),
and descent cannot see directive/invocation/EOF comments at all.
Options put to Fran: (A) full switch with a comment-source hook, (B) no
change, (C) shared per-comment kernel.

*As built (T11, option C).* materia: `MateriaDecoratio` (opening,
closing, continuation mark), `materia_annotationem_legere`, anchor =
`<` + letter (silva's rule; over 250 .sh one comment changes class,
unreported either way), `materia_annotationes_decoratione_colligere`
(the old entry wraps it) — which also closes the css block-comment gap
the excusatio spec named (a prefix-only tolera parsed with `*/` in its
cause). silva: block/line decorations declared, collector calls the
kernel; collection, attachment and nids unchanged (`materia_annotationes`
joins `silva/materia_substratum.sh`). Proof: a scratch dump of every
annotation field over a HEAD snapshot (1,545 C files, 162 annotations)
identical before/after, planted (continuation off → 28 rows);
tools/diagnostica over all .sh identical; silva 54/54, materia 14/14,
crusta 16/16, identitates -porta verified. `textus` now starts at the
anchor in both (parse still over the whole purged body, so error lines
stay exact).

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
learned. *Owed from T7b:* `tools/amalgama_excludenda_generare.sh` and
`tools/amalgama_ligare.sh` pass `silva/fontes` without `materia/fontes`
— they only run inside silva's frozen `amalgamare.sh`; fix them when
the amalgam regenerates here.

*Owed from T10 (2026-09-25):* the amalgam that thaws here must now
carry `silva_frons.c` (silva_arbor.c and silva_scribere's callers
depend on it; `fontes_politica.sh radices()` still EXCLUDES it since
T6b) and the materia substrate silva links (token, nodus, scribere,
arbor, arbor_aequalitas, lexicon, annotationes (T11) —
`silva/materia_substratum.sh`).
Amalgam consumers (briar, saltuarius, aedilis — consumer inventory
…WW34 rows still ignotum on 'per facadem transit') meet all of T7–T10
at once here.

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
