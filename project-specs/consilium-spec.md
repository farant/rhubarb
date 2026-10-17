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
