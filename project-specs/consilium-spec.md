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
