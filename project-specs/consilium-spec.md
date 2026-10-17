# Consilium — effectus ut data, consilium ante actum (plan / apply)

*Born 2026-10-16 in the quinta worktree, from Fran's question after the
U5 (b) commit was refused twice: "jobs that generate side effects but as
data, and a layer that analyzes the side effects and reasons how to
execute them - batching, ordering ... is there a case for a general
purpose plan/apply library?" This spec answers yes, names the pieces,
maps what the house already has onto them, and proposes one pilot.
Nothing here is built yet.*

*Discipline note: PULL, not completeness. The library is extracted from
two or three real consumers (§VIII); a field, verb or algorithm enters
only when a consumer needs it. The effect vocabulary in §III is a
starting proposal to be tested against the existing dialects ON PAPER
(slice S1) before any C is written.*

Status words: **EXSTAT** (exists in the tree, cited), **PROPOSITUM**
(proposed here), **APERTUM** (open question for Fran).

## I. The question, and why now

Today jobs execute directly and fabrica judges afterwards. The house
already writes effects down in four dialects (§II), but none of them is
a PROPOSAL made before acting, so nothing can reason across jobs before
something happens. Three cases from one day (2026-10-16) show the cost:

1. **Format made binaries stale, the commit refused.** `silva.commissio`
   formats changed functions (`_formam_praeparare`, inside
   `_formam_custodire`, pythonica/silva.py:3001), then notices that
   `bin/aedilis`/`bin/fabrica` were built from the pre-format bytes and
   refuses: "FORMA artificia installata stala fecit ... Aedifica iterum
   et committe iterum". The formatter's effect ("rewrites these source
   bytes") was invisible until after the fact. As data, a planner
   derives "these sources feed bin/aedilis and bin/fabrica - schedule
   their rebuild before the gates".
2. **The same pipeline answers the same situation two ways.** The gate
   preflight `_instrumenta_portarum_sanare` (silva.py:2889) HEALS stale
   tree binaries before the gates (in the tree, never installing),
   while the format guard REFUSES. One rule over effect data would make
   one decision.
3. **A write conflict found by reasoning, not by construction.** Plain
   `bin/aedilis X` writes `build/aedilis/<basename>/`; seven `arbor.c`
   and eight `computus.c` tools share that directory, and building
   `briar/instrumenta/fabrica.c` would overwrite bin/fabrica's own
   manifest (unitates-codicis-spec §VII semis, U5 b). Two jobs, one
   output path: a planner sees this statically. (Fixed for the gate by
   `bin/aedilis --exitus`, f4b23f95 - a workaround, not a rule.)

A fourth is a policy kept in memory instead of data: "never install to
~/.bin from a worktree" (MEMORY.md, worktree-quinta). As effect data it
is a capability rule (§V): in a worktree, refuse any plan containing a
write whose scope is home.

## II. What exists (EXSTAT)

### II.1 Four effect dialects

| dialect | kind | where | vocabulary |
|---|---|---|---|
| effectus | STATIC, scripts | effectus.canon (crusta `crusta_effectus`) | processus, lectio, scriptura, exsecutio, fontatio, enumeratio, probatio, ambitus_lectio, ignotum; attributes via, forma, resolutio, classis |
| lectiones | OBSERVED, any house tool | include/lectiones.h:20-27 | L read, A asked-absent, X exists, D directory enumerated, S written/deleted/moved, E environment variable read |
| fabrica actio | DECLARED, build actions | aedificatio.canon; lib/fabrica_declarationes.c | ingressus (res x clavis: plagula plagulae directorium instrumentum repositorium x contentum nomina clausura provenientia identitas effectus commissum), exitus (regeneratio relatio ignota verdictum), vestigium (permitted write area), praecondicio |
| portae rationes | DERIVED, gate coverage | include/portae.h (`PortaeRatioGenus`, 35b8d726) | CURSOR TEGIT EFFECTUS SUITA NOMINAT CURRIT CURRIT_ALIENUM INSTRUMENTUM IGNOTA |

The effectus canon already chose a language-neutral vocabulary on
purpose ("ut silva (C) idem olim emittat: fopen(p, 'r') lectio est",
effectus.canon header) - the right instinct for the shared record.

### II.2 Plan/apply in miniature

- **silva/renominare.sh**: plan by default, `-scribere` applies
  (renominare.sh:8). A plan of edits across files.
- **bin/fabrica sanare**: heals "ordine dependentiae" (tools/fabrica.c:
  1200, 1419), producers before readers, reports `praeparata`; refuses
  a write outside the declared vestigium ("scripsit extra vestigium",
  lib/fabrica_sanare.c:228) - the observed-vs-declared check of §VI.
- **silva.commissio**: a fixed pipeline of phases (silva.py:3421-3480):
  forma, lint, fabrica, unci, portae debitae, ordo portarum, instrumenta
  portarum, gates, git commit. Ordering by cost and kind already exists
  for gates (`portae_ordinare`, praeiudicia first).
- **silva.fusio_probare** (silva.py:3563): both tiers, continues past
  red (`pergere`), names all failures - an executor mode.
- **sanare installata**: rebuilds and installs all 21 house binaries;
  "install only those that changed" is the unfinished U5 item.

## III. The effect record (PROPOSITUM)

One record, many emitters. Fields (names provisional, APERTUM):

| field | meaning | example |
|---|---|---|
| verbum | what happens | legere, scribere, delere, movere, exsequi, instituere, rete, committere |
| res | the thing | a path, a binary, a git ref, a ledger record, a URL |
| scopus | where it lives | arbor (the tree), aedificatio (build/ under the tree), domus (~ outside the tree), externum (network, other repos) |
| auctor | which step proposes it | `forma`, `fabrica:corpus_silicis`, `porta:radix` |
| causa | why, in words | "fons tools/aedilis.c rescriptus" |
| ante / post | content hash before / after, if known | sigillum |
| idempotens, reversibile | properties a planner may rely on | verum / falsum / ignotum |

Mapping test for slice S1 (to be completed on paper; failures mean the
record is wrong):

| source | maps to |
|---|---|
| effectus `lectio via=X` | verbum legere, res X, scopus from path |
| effectus `scriptura` | scribere |
| effectus `exsecutio` | exsequi (res = program) |
| effectus `ignotum` | the BLIND case: a step whose effects are unknown (portae IGNOTA): owed, never trusted |
| lectiones `S` | scribere/delere/movere (the letter does not distinguish - gap) |
| lectiones `E` | legere, res = environment variable |
| fabrica `ingressus` | legere (clavis = how the planner keys it) |
| fabrica `exitus` | scribere (provenientia = regeneratio etc.) |
| fabrica `vestigium` | the PERMITTED write set: a policy, not an effect |

Primary form: STML with a canon (`consilium.canon`), so plans are files
one can read, diff and commit; a C struct mirrors it in the library.
APERTUM: whether the record extends `effectus.canon` (one dialect) or
is a new dialect that effectus translates into.

## IV. The plan: a pure core (PROPOSITUM)

A plan = steps, each with proposed effects and explicit `sequitur`
edges; the core is PURE (no I/O): questions about the world (does X
exist? its hash? which actions produce it?) go through a sutura, as in
lib/unitates.c and lib/portae.c - credo tests run on an in-memory world.

Algorithms, each a candidate library function:

- **ordo**: data edges (A writes what B reads) + explicit edges ->
  topological waves; a cycle is NAMED with its members, never "cycle".
- **conflictus**: two steps writing one resource in a wave (or at all,
  unless declared ordered) - the arbor case of §I.3.
- **deduplicatio**: the same effect proposed by several steps runs once
  (heal bin/fabrica once even if three steps need it fresh).
- **fasciculi**: batch effects of one kind (compile objects once for many
  programs; cheap checks first - the praeiudicium tier, generalized).
- **sumptus**: cost estimate per step from history
  (build/portae/tempora.tsv, mensura cursus), critical path.
- **differentia**: diff two plans ("what changed in what we are about
  to do") - review before outward actions.

## V. Policy as data: capabilities (PROPOSITUM)

Rules over effect records, verdict per effect: LICET / ROGANDUM (needs
approval) / VETITUM, always with the rule's name - the `<iudex>`
pattern of unitates.stml generalized from files to effects. First
rules, each already a habit or a memory today:

- in a worktree, `scopus=domus` writes are VETITUM (installs from
  worktrees);
- `verbum=rete` and `verbum=committere` (push) are ROGANDUM;
- writes to a committed GENERATUM file only by its fabrica action;
- `vestigium`: a step writing outside its declared area is VETITUM.

APERTUM: where policies live (unitates.stml, a new consilium.stml, or
per consumer) and whether fabrica-visio §VII item 6 ("capabilities")
is the same question.

## VI. Executor and journal (later)

Apply waves through `processus` async (no threads by design), observe
with lectiones, compare observed with proposed: more than proposed is
a violation (the general form of "scripsit extra vestigium"); modes
stop-at-first-red and continue-and-name-all (`fusio_probare` PERGIT);
dry run = print the plan. Journal: append-only plan + verdicts +
results, for resuming a half-applied plan and for timings (diurna /
annales are relatives). Most of this exists inside fabrica; extracting
it is where desideratum …DCF4G1 ("fabrica sutura vera as a library")
lands. Built last.

## VII. Pilot: silva.commissio (PROPOSITUM)

Why commissio: its phases are a fixed plan today (silva.py:3421-3480),
it is where the pain is, and §I's cases are its test cases.

Shape of the pilot:

1. Each phase PROPOSES before acting: `forma` -> "scribere tools/aedilis.c
   (only these functions)"; `fabrica` -> "artifacts X Y stale, actions A
   B would rewrite them"; `portae` -> "run gates G (their instruments:
   bin/x)".
2. The planner derives: sources rewritten by `forma` feed binaries B
   (fabrica already answers "who is built from this source") -> add
   "heal B in the tree" before the gates; dedupe with the gate
   preflight's heals; order by data.
3. Policy: worktree -> heals only through tree-local fabrica actions
   (never an `*_struere.sh` that installs); refuse with the rule's name
   otherwise.
4. Test cases = §I.1 (format then rebuild, no refusal) and the second
   refusal of the same day: an annotation edit in tools/natura_canones.h
   made bin/natura_canones stale, its generator refused, everything
   downstream was IGNOTUM - the planner proposes "heal
   bin/natura_canones, then regenerate canones_cocti" as one plan.

Where it lives: the planning core in C (pythonica stays a thin wrapper -
MEMORY "pythonica = thin wrapper"); commissio calls it. APERTUM: whether
the pilot starts inside fabrica (it already knows producers and
readers) or as a separate lib that fabrica feeds.

## VIII. Second consumers (rule of three)

Only what two consumers both need enters the library:

- **sanare installata**: plan = "these binaries changed since their
  installed copies" -> install only those; policy: only in main.
- **silva/renominare.sh**: edits across files as a plan; preview =
  `differentia`; apply atomically.
- Later: tabularium `expeditio` batches; app editors (scriba, pictor)
  where reversible effects give undo/redo.

## IX. Slices (PROPOSITUM)

- **S1** effect record + canon on paper: complete the §III mapping for
  all four dialects; record what does not fit. No C.
- **S2** pure library: plan, ordo, conflictus, deduplicatio, policy
  verdicts; credo suite on an in-memory sutura; born red by plants.
- **S3** commissio pilot (§VII); its two 2026-10-16 refusals become
  passing cases.
- **S4** second consumer (sanare installata or renominare); move into
  the library only what both use.
- **S5** executor and journal, extracted from fabrica.

## X. Open questions (APERTUM, for Fran)

1. Names: `consilium` (plan) and/or `propositum` (proposal) for the
   library; `effectus` is taken by the script dialect.
2. One dialect or two: extend effectus.canon, or a new record that
   effectus translates into (§III)?
3. Inside fabrica or beside it (§VII)?
4. Where policies live (§V) - and whether this IS fabrica-visio §VII's
   "capabilities" and "gates and debts in one graph".
5. How far the pilot goes: commissio phases only, or also fusio_probare.

## AUDIENDA (not verified)

- The §III mapping is sketched for a handful of elements, not completed;
  lectiones `S` folds write/delete/move into one letter - whether the
  planner needs them apart is untested.
- That fabrica can answer "which binaries are built from source X" in
  one query was assumed from `_stala_celeria` and `quid`/clausura uses,
  not measured as an API.
- No timing for planning itself; the claim that a pure planner adds
  negligible time to commissio is a belief.
- Whether static effects of scripts (crusta) are precise enough to plan
  on, versus only to owe gates, was not measured; blind steps must stay
  blind (owed, never trusted).

## XI. S1 result: the four dialects mapped (2026-10-16)

Read in full: effectus.canon (all nine elements), include/lectiones.h,
aedificatio.canon (actio, ingressus, exitus, vestigium, praecondicio),
include/portae.h (`PortaeRatioGenus`). Finding first: **effectus
already carries most per-effect fields, under the same names** - the
§III sketch was too thin and too new. The record should GROW OUT OF
effectus, not beside it.

### XI.1 Record v0.2 (per effect)

| field | values | from |
|---|---|---|
| verbum | legere, inspicere, enumerare, scribere, delere, movere, exsequi, fontare, ambitum_legere, instituere, rete, committere | effectus elements (lectio, probatio, enumeratio, scriptura, exsecutio, fontatio, ambitus_lectio); lectiones L/X/D/S/E; the last three NEW (no dialect has them) |
| res + forma | path; via / globus / praefixum | effectus `via`, `forma` |
| classis (scope) | arbor, build, instrumentum_domus, domus, externa, systema, temporaria | effectus `classis`; `domus` NEW (split out of externa, §XI.3 M5) |
| certitudo | plena / partialis / nulla | effectus `resolutio`; `ignotum` element = a BLIND step (portae IGNOTA) |
| cognitio (how we know) | declaratum / derivatum / observatum / propositum | fabrica (declared), crusta + portae (derived), lectiones (observed), the planner's input (proposed); effectus `per=observatum` is one case of it |
| per (mechanism) | redirectio, aedificium, mandatum | effectus `per` (minus observatum, which moved to cognitio) |
| condicio | the guard under which the effect happens | effectus `custodia` ('[ -x P ] \|\| S') - generalized |
| exstat | verum / falsum, for reads | lectiones `A` (asked, ABSENT): a dependency on ABSENCE - creating the file changes the step |
| clavis (reads) | contentum, nomina, clausura, provenientia, identitas, effectus, commissum | fabrica `ingressus clavis` - HOW staleness of this input is judged; optional |
| provenientia (writes) | regeneratio, relatio, ignota, verdictum | fabrica `exitus` - reproducible / reported by the binary / unknown / a gate verdict; gives `idempotens` for free (regeneratio) |
| locus | plagula, sedes, octeti | effectus (where the claim is grounded in source) |
| auctor, causa | step title; words | portae `ratio` strings, unitates causae habit |

### XI.2 Step-level fields (per step, not per effect)

| field | from |
|---|---|
| genus: generator / formatio / institutio / iudicium | fabrica `actio genus` |
| sequitur (explicit order) | fabrica `praecondicio` |
| vestigium (PERMITTED writes) - a POLICY, not an effect | fabrica `vestigium` (+ `communis`) |
| sumptus, memorabilis, celer (verification cost and mode) | fabrica attributes; build/portae/tempora.tsv |
| caecus (blind: effects unknown) | effectus `ignotum`, portae IGNOTA |

### XI.3 Misfits (what the record must decide)

- **M1 intra-step data flow.** effectus `scripta_in_ambitu` (a read of
  something the same process wrote) and `ambitus_lectio assignatum` are
  NOT inputs. The planner must drop reads satisfied inside the step, or
  every generator looks like it depends on its own output.
- **M2 write kinds folded.** lectiones `S` = written, deleted or moved;
  effectus `scriptura` = '>' '>>' rm alike. Conflict detection needs at
  least scribere vs delere (two deletes do not conflict; write + delete
  do). Options: new letters in lectiones (W/R/M), or keep folded and
  treat any two as a conflict. APERTUM.
- **M3 absence as input** (lectiones `A`): the record needs `exstat`;
  a plan that CREATES a file another step probed as absent must order
  them (or report the change in behaviour).
- **M4 portae reasons are relations, not effects.** CURSOR, SUITA,
  CURRIT, EFFECTUS are cognitio x verbum on the gate's own effects;
  TEGIT is a hand-declared read (cognitio declaratum, certitudo
  partialis); NOMINAT a possible read; INSTRUMENTUM and CURRIT_ALIENUM
  are OWNERSHIP of a program by a step - a step-level relation the
  record lacks ("possessor"). Needed by the policy for runners (U5 c).
- **M5 home vs elsewhere.** effectus `externa` = any absolute path
  outside the tree (effectus-spec.md:202), so ~/.bin and ~/.rhubarb are
  lumped with /usr/local or another repo. The worktree policy needs
  `domus` apart.
- **M6 the outward verbs are missing everywhere.** No dialect records
  network access, git commit/push or ledger writes; crusta sees `git`
  only as an external command (ignotum or mandatum). These are exactly
  the effects an approval policy cares most about. Emitters for them
  will have to be declarations (cognitio declaratum), not analysis.
- **M7 certainty is per effect, blindness per step.** A partial effect
  (glob, unresolved variable) is still useful; a blind step is owed and
  never trusted. Keep both levels.

### XI.4 What this suggests for §X questions 1-2

- **Q2 (one dialect or two):** EXTEND effectus (a version 2: the NEW
  verbs, `domus`, `cognitio`, `exstat`, `clavis`, `provenientia`,
  `condicio`; `per=observatum` moves to cognitio) for the per-effect
  record, and add a SMALL new dialect for plans - steps (genus,
  sequitur, vestigium, possessor, sumptus, caecus) whose children are
  effectus records. One vocabulary for effects, a second only for what
  is genuinely about steps and plans.
- **Q1 (names):** keep `effectus` for the record (already approved by
  Fran 2026-10-05, effectus.canon header); `consilium` for the plan
  dialect and the library.
- Neither is decided here.

## XII. S2 built (2026-10-16): lib/consilium (pure core)

include/consilium.h + lib/consilium.c + probationes/probatio_consilium.c
(8 credo sections). The record of §XI.1 as a C struct
(`ConsiliumEffectus`: verbum, res, forma, classis incl. DOMUS,
certitudo, cognitio, exstat, causa); steps (`ConsiliumGradus`: titulus =
identity, genus, effectus, sequitur, vestigium, auctores, caecus,
sumptus_ms). No sutura was needed: everything the core reasons about is
in the plan; world questions belong to S3's expansion.

- **ordo**: data arcs (writer -> reader, absence probes included, M3),
  explicit `sequitur`, blind steps as walls; Kahn waves in insertion
  order; a cycle is returned NAMED (its members in arc order), unknown
  `sequitur` targets as refusals. Every arc carries its reason ("A
  scribit R quod legitur", "absentia: ...", "sequitur expresse",
  "murus gradus caeci").
- **M1** implemented: a step's read of what it writes itself is not an
  input. **M2** decided for the API: delete + delete is not a conflict;
  any other pair of writers to one resource without an ordering path is.
- **res matching**: path equal; prefix; glob via
  `directorium_titulus_congruit` (same semantics as portae); an
  enumeration of a directory covers everything under it; pattern vs
  pattern conservatively by literal prefixes.
- **deduplicatio**: same title = same step (proposers collected in
  `auctores`); same effect twice = once.
- **politica**: `ConsiliumRegula` {titulus, verbum|OMNIA, classis|OMNIA,
  exemplar, sententia, causa}; first match wins, an explicit LICET
  stops; a step's `vestigium` forbids writes outside it ("vestigium").
- **effusio**: human text or TSV (UNDA ARCUS CYCLUS RECUSATIO
  CONFLICTUS IUDICIUM).

Tests pin the arbor collision of §I.3 as a conflict and the worktree
rule of §V as a VETITUM verdict. Born red three times (M1 removed,
delete+delete counted, wall removed), each at its own assertion. Not in
S2 (by PULL): STML reading/writing of plans, a CLI, cost/critical path,
plan diff - S3 needs the first two.

## XIII. S3 built (2026-10-16): dialect, CLI, commissio pilot

**S3a** `consilium.canon` (registered `<consilium>`): `<regula>` and
`<gradus>` children; `<gradus>` holds `<effectus>` (the record's names,
values as in effectus where shared: `partialis` in STML maps to
CONSILIUM_EX_PARTE), `<sequitur>`, `<vestigium>`. Reader
`consilium_legere` validates every value against the C enumerations
and names EVERY error with its line ("linea 5: verbum 'volare' ignotum
(nota: legere ...)"); the test caught a real gap - unknown attributes on
the root were not checked.

**S3b** `bin/fabrica consilium [-machina] VIA` (tools/fabrica_consilium.c):
waves, arcs with reasons, cycle, refusals, conflicts, verdicts, SUMMA.
Exit 0 = may run as is; 1 = not without a decision (conflict, cycle,
refusal, VETITUM or ROGANDUM); 2 = unreadable.

**S3c** pilot in `silva._formam_custodire`: when formatting makes house
binaries stale, commissio no longer refuses outright. It writes the plan
(build/consilium/forma.stml: `forma` writes the formatted sources;
`sana X` reads them - conservatively all of them - and writes X; `portae`
execute X), has bin/fabrica judge it, and if allowed runs the heals
WAVE BY WAVE, batched (one `bin/fabrica sanare` per wave, tree-local,
never installing), re-judges, and continues. If the planner says no, it
refuses as before but with the rule named. Policy written by commissio:
`institutio_rogata` (any install needs approval - commissio never
installs on its own) and, in a git worktree (`--git-dir` differs from
`--git-common-dir`), `arbor_secundaria` (home writes VETITUM).

The first real run taught the planner a fact: THE JUDGE CANNOT HEAL
ITSELF - `bin/fabrica sanare bin/fabrica ...` refuses ("iudex ipse non
recens"). The plan now gives bin/fabrica its own step (genus `iudex`,
built by tools/fabrica_struere.sh, which installs nothing) and every
other heal `sequitur` it: forma -> sana bin/fabrica -> sana bin/aedilis
-> portae. End-to-end (format-only edit of lib/aedilis.c after building
both binaries from the misformatted bytes): "forma: 2 artificia stala
per consilium in arbore sanata (bin/fabrica, bin/aedilis)", both RECENS,
the source back to its original bytes. A second run had hit the known
false "scripsit extra vestigium ~/.bin/mensor" while another session
installed (ledger …VJPN3WT, ictus added).

Tests: credo section IX (reader, good and bad documents); pythonica:
plan order (judge first), install ROGANDA, worktree home VETITA, main
without the worktree rule, scope classes; a plant dropping the
judge-first edge went red.

Not yet (S3d): the SECOND refusal case of 2026-10-16 - a tool edit
made the tool binary stale, its GENERATOR refused, everything
downstream IGNOTUM (`_fabricam_exigere`). Same shape: heal the tool,
then regenerate, as one plan.

## XIV. S3d built (2026-10-16): the generator blocked by a stale tool

The second refusal of the day, reproduced: a comment edit in
tools/natura_canones.h made bin/natura_canones stale; `iudicare -plenus
-tacta` reported 37 IGNOTUM canons ("generator fractus ... bin/
natura_canones STALUS") and bin/natura_canones itself NOT AT ALL.

Two layers, both needed:

1. **Declarations (root cause, ledger …HTQC94).** `canones_cocti`
   ran bin/natura_canones without declaring it (only the tool's
   manifest, its sources, was an input), so fabrica could not know
   "heal natura first". Now `canones_cocti` and `glossae_pagina` declare
   `<praecondicio actio="natura"/>` + `<ingressus genus="instrumentum_domus"
   via="bin/natura_*"/>` - the existing `lectores_cocti` pattern. After
   it, `bin/fabrica sanare -siccum` plans wave I `natura`, wave II
   `canones_cocti`.
2. **Facts for the planner from fabrica, not Python.** `sanare -siccum
   -machina` now also prints `EXITUS actio via` for every output of
   every planned action (tools/fabrica.c); commissio reads actions,
   outputs and waves from there instead of parsing aedificatio.stml.

`silva._consilium_producentium`: from that dry run, PRODUCERS = planned
actions whose outputs are all uncommitted and tree-local (bin/, build/);
actions writing committed artifacts are NOT in the plan (re-judgment
finds them fresh when the bytes do not change; otherwise the old refusal
stands - no silent additions to a commit). The plan (build/consilium/
producentes.stml, same policy as §XIII) is judged and run with the S3c
executor; then `_fabricam_exigere` re-judges once (`_producentes`). It
is the LAST resort before the refusal: the older rules (regenerate
committed artifacts inside the commit, ignore uncommitted ones) run
first and unchanged. End to end: "producentes stali per consilium in
arbore sanati (bin/natura_examen ...)", then 37 RECENS, commit proceeds.

Tests (pythonica): producers healed, the committed regenerator and a
home install left out; committed outputs = no producer; three existing
call-order tests now ignore the read-only dry-run line. A plant
dropping the committed-output filter went red.

## XV. S4 built (2026-10-17): the second consumer - bin/fabrica sanare

Consumer two is fabrica's own `sanare`, in C, through the library API
(no document). In a git worktree (`.git` is a file), after the judge
stage (judges fresh, so the plan is complete) and BEFORE anything runs,
`_sanare_consilium` (tools/fabrica.c) takes fabrica's dry-run plan,
builds a consilium (one step per planned action, its outputs as
effects - `~` and outside-the-tree paths as `instituere`, scope from
the path), and judges it with the worktree rule. Any VETITUM = exit 2,
nothing done, every blocked install named. In the main tree nothing is
judged (installing is its ritual) - zero cost there.

Before: "never `bin/fabrica sanare installata` from a worktree" lived in
memory and discipline. Now (sandbox HOME): "consilium in arbore
secundaria VETITUM - nihil actum ... VETITUM: institutio_briar
instituere ~/.bin/briar <- arbor_secundaria ...", real ~/.bin untouched;
tree-local heals (`sanare bin/praeco`) unaffected; `-siccum` unaffected.

Rule of three, applied: what BOTH consumers need moved into the
library - `consilium_classis_viae` (path -> scope; `~` and home =
DOMUS, `bin/` = INSTRUMENTUM_DOMUS, `build/` and `.../build/` = BUILD)
and `consilium_regula_arboris_secundariae`. pythonica's `_classis_viae`
is now a duplicate of the C function (left for S5: commissio's plan
documents could omit `classis` and let the reader infer it).

Tests: credo section X (scopes, rule); fabrica-fumus LX (a world with an
install action into a temporary HOME: worktree refuses and writes
nothing, main tree is not judged). Plant: the check disabled ->
LX red with "scriptum 1" - the worktree run DID write into the home.

## XVI. S5 plan (2026-10-17): executor and journal

Read in full before planning: lib/fabrica_sanare.c (the executor) and
commissio's `_consilium_sanare` (pythonica/silva.py, a 60-line Python
copy of the generic half - against "pythonica = thin wrapper").

**Generic half of fabrica's loop** (moves to lib/consilium): waves in
order; SISTERE ('post fracturam: nova non incipiuntur') or PERGERE
(name every failure - gradus members today); dependents of a failed
step OMISSUM ('dependentia fracta: X'); observed vs proposed (snapshot
before/after, a write outside the declared footprint fails the step);
in a simultaneous wave, a write is pinned on the member whose book
recorded it, a write claimed by nobody breaks the wave ('scriptor
ignotus'); timings, progress. **fabrica's own half** (stays, as sutura
callbacks in S5d): freshness before/after, auditus, verdicts from logs,
gradus areas, saved read-records of passes.

Slices (Fran 2026-10-17: recommendations on 1 and 2 accepted):

- **S5a** executor core `consilium_exsequi` - pure, credo on an
  in-memory world. Sutura: `agere` (exit code, tail, duration, the
  step's own recorded writes), `observare` (snapshot, optional),
  `incipit`/`notare` (journal, progress). Refuses BEFORE anything runs:
  cycle, unknown `sequitur`, conflict, any non-LICET verdict (approval
  of ROGANDUM is the caller's; no consumer runs one today). Serial in
  S5a: one snapshot pair per step makes attribution exact; the
  wave-wide unknown-writer rule arrives with simultaneous waves (S5d).
- **S5b** journal `build/consilium/acta/<titulus>.tsv`, append-only,
  keyed by the plan's hash AND a tree fingerprint (HEAD + status hash):
  `-resumere` skips steps journaled ACTUM only when both match - the
  "interrupted, nothing changed" case (2026-10-16 laptop sleep). After
  a code fix, resume refuses ('arbor mutata post initium'); freshness
  (fabrica) is the tool for that case, not the journal.
- **S5c** consumer one: `<mandatum><argumentum>..` in the dialect (an
  argv as data, judgeable by policy); `bin/fabrica consilium -agere
  [-pergere] [-resumere] VIA`; commissio's Python loop deleted; the
  reader infers `classis` from `res` when absent (pythonica's
  `_classis_viae` deleted). A judge rebuilding itself mid-plan is safe:
  tools/fabrica_struere.sh does rm then cp (the running image keeps
  its inode).
- **S5d** consumer two: fabrica `sanare`'s wave loop onto the executor;
  `agere_simul` becomes a sutura capability; fabrica-fumus + probatio
  fabrica prove behavior unchanged. Riskiest, last.

What does NOT improve: speed (serial until S5d); blind steps stay
blind; observation costs a snapshot pair per step. Observation sees the
tree (snapshot) and house tools anywhere (lectiones books); a bare `cp`
out of the tree is invisible UNLESS the consumer's `observare` also
lists the out-of-tree directories policy cares about (`~/.bin` - one
directory listing; the 2026-10-13 silex clobber).

## XVII. S5a built (2026-10-17): the executor core

`include/consilium.h` EXSECUTIO: `consilium_exsequi(c, regulae,
sutura, modus, piscina, &causa)` -> Xar of `ConsiliumActus` (step,
wave, ACTUM/FRACTUM/OMISSUM, exit code, duration, cause, observed
writes, writes beyond the proposal), or NIHIL + every refusal named
(cycle, unknown sequitur, conflict, non-LICET verdict) with nothing
run. Sutura: `agere`, `observare` (optional snapshot), `incipit`,
`notare`. `consilium_observata_comparare` (snapshot diff),
`consilium_acta_scribere` (human / `ACTUS`, `EXTRA`, `SUMMA` TSV).

Decided while building: dependents follow every arc - data, explicit
`sequitur`, the blind wall; extra writes fail only a step that
promised (not blind, writers PLENA). credo XI-XV on an in-memory
world; four plants red. No consumer yet - S5b (journal) and S5c
(`bin/fabrica consilium -agere`, commissio) are next.

## XVIII. S5b built (2026-10-17): journal and resume

`consilium_sigillum(c, regulae)` (SHA-256 hex of a length-prefixed
canonical form: steps, effects, sequitur, vestigium, rules - not
authors, costs, causes); diary lines `consilium_diarium_caput`
(CONSILIUM seal fingerprint novum|resumptum), `_incepti` (INCEPIT),
`_actus` (ACTUS, EXTRA, ARBOR after every step);
`consilium_diarium_resumere(textus, seal, fingerprint)` -> done steps,
or FALSUM naming why (empty, plan changed, tree changed since the last
ARBOR line, malformed line N). `consilium_exsequi` gained `facta`:
done steps are not run (ACTUM 'ex actis'), their dependents proceed.

Fingerprint (Fran 2026-10-17): HEAD + source status, build/ and
ignored output excluded - computed by the consumer (git), compared by
the library. Still pure: the file itself is S5c's (`-agere
-resumere`). credo XVI-XVIII; four plants red.
