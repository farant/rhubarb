# GESTA K3 SPEC v1 — ACTIONES + PROCESSUS

2026-07-16. **SUPERSEDED by gesta-k3-spec-v2.md (the implementation
source) — kept as the explorer target of record.** Original header:
**SPEC v1 — explorer target, NOT the build source.** Three
explorers (TS Processes deep-read; gesta/tabularium seam audit; live
smaragda mining) shoot at this; spec-v2 is the implementation source.
Interview record: gesta-k3-interrogatio.md (14 decisions, all locked).
Oracle: ../bun-libraries/smaragda.ts Actions 3180–3673 (read),
Processes 4655–5431 (explorer 1's charter).

## I. WHAT K3 IS

Two capabilities on the K1/K2 kernel, one new kernel primitive:

1. **ACTIONES** — business logic as data: stored recipes with gated
   preconditions, typed arguments, and an atomic bundle of side
   effects drawn from a closed verb vocabulary. Full provenance:
   every execution (and refusal) is an event on the action's own res.
2. **PROCESSUS** — orchestration as data: lanes of steps
   (opus/actio/porta) advancing eagerly as their tasks complete,
   instances being ordinary res linked to their tasks via K2 membra.
3. **New primitive: the multi-event atomic bundle** — N events, one
   transaction, N annales lines, folds after. Everything else
   composes from K1/K2 machinery.

Machinula reducers = separate named milestone (park renamed at K3
seal). The tier-2 seam is named here (§VII) so tier-1 forecloses
nothing.

## II. DECISIONS LOCKED (interrogatio)

| # | Decision |
|---|----------|
| 1 | Scope: actiones + processus; machinula reducers = own milestone |
| 2 | Gate BLOCKS (preconditions refuse), effects RECORD (custodia) |
| 3 | Execution = actio-facta event on the action's res; NO side tables; TS input table subsumed |
| 4 | Closed verb list, 1:1 onto event kinds |
| 5 | Tokens: $arg.X, $res.X, $nunc, $novus.N (creatio 'ut' binding) |
| 6 | species:"actio", opes/argumenta/effectus, whole-JSON, emendatio-in-place |
| 7 | Affordances: gesta_actiones_rei + res-tool 'actiones:' line |
| 8 | Mechanical failure: rollback whole bundle + actio-recusata event (gate refusals too) |
| 9 | Steps v1: opus-gradus, actio-gradus, porta-gradus (fetch/ramus parked) |
| 10 | Advancement: eager, triggered by status events on membra of running instances; hook at SCRIBERE time (replicare never re-fires) |
| 11 | Seed v3: opus genus + claudere-cum-decreto + ritus-signaculi |
| 12 | MCP: one new tool agere |
| 13 | Recipes are code: no MCP definition surface |
| 14 | Full ritual: 3 explorers → spec-v2 |

## III. ACTIONES

### Definition (genus document, species:"actio")

```json
{
  "titulus": "claudere-cum-decreto",
  "species": "actio",
  "opes": [
    {"titulus": "parcum", "genus": "parcum",
     "status_necessarius": "tractum"}
  ],
  "argumenta": [
    {"titulus": "cur", "typus": "textus", "necessarium": true}
  ],
  "effectus": [
    {"verbum": "creatio", "ut": "decretum_novum",
     "datum": {"genus": "decretum", "titulus": "$arg.cur"}},
    {"verbum": "nota", "res": "$res.parcum",
     "datum": {"textus": "clausum: $arg.cur"}},
    {"verbum": "status", "res": "$res.parcum",
     "datum": {"novus": "clausum"}}
  ]
}
```

(This exact document is seed v3's flagship — approved in interview
preview. Note for spec-v2: consider a fourth effect linking
$novus.decretum_novum to $res.parcum via membrum verbs — the nexus
sugar decomposes into creatio+membrum-additum effects.)

- `opes[]`: named bindings — {titulus, genus, status_necessarius?}.
  TS ActionResourceDef.
- `argumenta[]`: {titulus, typus (K2 four), necessarium} — validated
  by the K2 type validator.
- `effectus[]`: ordered; each {verbum, res?, datum, ut?}.
- Defined via ordinary definitio-generis; amended via
  emendatio-generis (whole-replace). No actionReducer port (D8
  pattern, third time).

### Verb slate (sanctioned; 1:1 onto event kinds)

creatio (may carry 'ut'), mutatio, remotio, status, nota,
membrum-additum, membrum-remotum. New verbs = vocabulary ceremony.

### Substitution

- `$arg.X` — argument value (whole-token keeps JSON type;
  embedded-in-string coerces, TS 3376–3391 parity)
- `$res.X` — bound ops res_id
- `$novus.N` — res_id created by an earlier creatio effect with
  ut:"N" (oracle improvement; forward references = mechanical
  failure)
- `$nunc` — the bundle's creatum timestamp (write-time, not
  fold-time — no wall-clock in folds law untouched)

### Execution pipeline (gesta_agere)

1. Load def (genus datum, species must be actio) — else refuse.
2. GATE: every ops binding exists, genus matches, status_necessarius
   matches; every argumentum present + typed. Failure → NO effects;
   write actio-recusata {ligamina, argumenta, causa} on the action
   res; return refusal.
3. Build substitution context.
4. Execute effectus in ONE atomic bundle (§V): each verb becomes one
   gesta event (validation record-don't-block per K1 — custodia
   notae land INSIDE the bundle).
5. Mechanical failure mid-bundle (unknown genus in creatio, bad
   $novus forward ref, allocation) → roll back entire bundle → write
   actio-recusata → return refusal.
6. Success → append actio-facta {ligamina, argumenta,
   eventus: [event ids]} to the action res — same bundle or
   immediately after (explorer 2: decide; leaning same-bundle, so
   facta is atomic with its effects).

### Affordances

`gesta_actiones_rei(mundus, res_id, piscina)` → actions whose opes
slot 0..n could bind this res in its current status. Tenant: res tool
appends `actiones:` line when non-empty.

## IV. PROCESSUS

### Definition (genus document, species:"processus")

```json
{
  "titulus": "ritus-signaculi",
  "species": "processus",
  "opes": [ {"titulus": "propositum", "genus": "parcum"} ],
  "gradus": [
    {"titulus": "interrogatio", "ordo": "consilium",
     "genus_gradus": "opus", "positio": 0},
    {"titulus": "spec", "ordo": "consilium",
     "genus_gradus": "opus", "positio": 1},
    {"titulus": "exploratores", "ordo": "consilium",
     "genus_gradus": "opus", "positio": 2},
    {"titulus": "frusta", "ordo": "aedificatio",
     "genus_gradus": "opus", "positio": 0,
     "post": ["spec"]},
    {"titulus": "porta-sigilli", "ordo": "sigillum",
     "genus_gradus": "porta", "positio": 0,
     "post": ["exploratores", "frusta"]},
    {"titulus": "sigillare", "ordo": "sigillum",
     "genus_gradus": "actio", "positio": 1,
     "actio": "claudere-cum-decreto",
     "ligamina": {"parcum": "$ops.propositum"},
     "argumenta": {"cur": "sigillum: $ops.propositum"}}
  ]
}
```

(Shape PROVISIONAL — explorer 1 refines against TS step encoding:
lane ordering semantics, cross-lane 'post' dependencies, whether
in-lane position alone sequences steps. The ritual definition above
is also seed-v3 content pending that refinement.)

- ordo = lane (named). Steps in one ordo run in positio sequence;
  ordines run concurrently; `post` names explicit cross-lane deps.
- genus_gradus: opus (spawn opus res, wait for perfectum), actio
  (execute action), porta (barrier: completes when all `post` steps
  complete).

### Instance model

- Instance = ordinary res, genus = the processus genus titulus.
  Started via agere {processus, ligamina}.
- Spawned opera = res of genus opus, LINKED to the instance via K2
  membra: instance --pars "<gradus titulus>"--> opus. (Processes
  become K2 nexus machinery's second consumer.)
- Instance stream records advancement: gradus-incohatus {gradus},
  gradus-perfectus {gradus}, plus actio-gradus firings referencing
  actio-facta ids. Instance state = its own progress ledger; the
  linked opera tell their own stories (event-sourcing norm: streams
  narrate locally, folds join).
- Instance status machine: currens → perfectus | omissus.

### Advancement engine

Trigger: inside gesta_scribere, AFTER folds, if the event was a
status change on a res that is a membrum of a res whose genus is
species:processus and status currens (one membra lookup + one genus
check). Then:

1. Mark the completed step (gradus-perfectus event).
2. Fixpoint: spawn every step whose predecessors (in-lane positio +
   `post`) are all complete — opus steps create+link opera, actio
   steps execute recipes (which may complete further steps — bounded
   by step count, cycle-safe because steps complete at most once).
3. All steps complete → instance status perfectus.
4. All spawned events ride atomic bundles.

Explorer 2 charter: exact hook placement (recursion into
gesta_scribere from within gesta_scribere — re-entrancy!), and
whether advancement shares the triggering event's transaction or
follows it.

## V. NEW KERNEL PRIMITIVE — the atomic bundle

gesta_scribere is one-event-one-transaction. K3 needs
gesta_fascis_scribere (N events, one transaction): validations first
(record-don't-block notes queued as events IN the bundle), N inserts,
N annales appends before COMMIT (crash ⇒ annales superset, verificare
names it — same law as K1), folds once after. Explorer 2: refactor
shape of _scribere_crudum; re-entrancy for advancement.

## VI. TENANT (tabularium)

- **Seed v3**: opus genus {titulus req, assignatum, prioritas,
  effectus attrs; pendens→susceptum→perfectum|omissum + direct
  pendens→perfectum} + claudere-cum-decreto + ritus-signaculi.
  Idempotence: genus/res absent-check as seed v1/v2.
- **agere tool**: {actio?|processus?, ligamina (JSON obj:
  titulus→res_id/titulus), argumenta (JSON obj)}. Returns facta
  summary (events written) or recusata causa. Response includes
  the K2-style passive surfaces.
- **res tool**: + actiones: line (affordances).
- **Doctrine v3**: grows to teach agere + the recipes-are-code rule.
  (First doctrine growth since K1.1 — deliberate.)
- **Definitions NOT via MCP** (decision 13).

## VII. MACHINULA SEAM (named, deferred)

- Action tier-2: an action may in future carry `fons` (C89 source as
  tessellae) instead of `effectus` — the executor dispatches on which
  field is present. K3 refuses fons-bearing actions with a named
  error.
- Genus reducers: `reducer` field (K1-reserved, "ordinarius") is that
  milestone's seam.
- Process tier-2: ramus-gradus (conditionals) lands with machinula.

## VIII. DIVERGENCE LEDGER (v1 — explorers extend)

| D | divergence | why |
|---|---|---|
| D1 | No input/action_taken tables — execution is an event | db = projection law; events carry actor/origo already |
| D2 | No actionReducer/processReducer — whole-JSON defs | D8 pattern, third application |
| D3 | Effects record-don't-block inside gated bundle (TS throws) | two-phase posture, decision 2 |
| D4 | $novus creata refs (TS cannot reference created res) | flagship recipe requires it |
| D5 | create_log/create_error verbs collapsed into nota | K2 D4 — custodia/notae are the error stream |
| D6 | Steps fetch/ramus deferred | context-flow + conditionals = tier-2 adjacent |
| D7 | Process↔task linkage via K2 membra, not bespoke tables | one link machinery (K2 decision echoed) |

## IX. EXPLORER CHARTERS (ritual step 3)

**E1 — oracle (TS Processes 4655–5431 + cron boundary):** instance
storage model; step/lane/position encoding as data; the advancement
algorithm verbatim (_checkProcessTaskCompletion + whatever else);
gate semantics (all-of? any-of?); context flow (what fetch_step
actually does — what we're deferring); failure/cancel semantics;
how Publication-like defs are stored; anything that breaks §IV.

**E2 — our seams (gesta.{h,c}, tabularium.c):** atomic-bundle
refactor of _scribere_crudum (transaction scope, annales ordering,
sordidae, eager plicare — exactly what moves); advancement hook
re-entrancy (scribere→advancement→scribere recursion or queue?);
membra reuse costs; affordance query plan; agere handler + seed v3
mechanics; chunk boundaries that keep suites green mid-milestone.

**E3 — live system mining (claude_ai_smaragda MCP):** describe 3-4
process genera incl. Demo Exploration + Publication (step encodings
in the wild); one completed + one running instance walked; task usage
patterns (591 completed — spawned by what?); which of the 9 actions /
16 process genera actually get USED vs rot (checkout_v4 lesson,
generalized); features we planned that reality demotes or promotes.

## X. PRELIMINARY CHUNKS (spec-v2 finalizes)

A: atomic bundle + actio machinery (gate, substitution, verbs,
   facta/recusata, affordance query) + goldens.
B: processus machinery (defs, instances, membra linkage, advancement
   engine, porta) + goldens.
C: tenant (seed v3, agere, res line, doctrine v3) + live deploy +
   first ritual instance + seal bars.

## XI. SEAL BARS (draft)

Suites green; examen ACCIPE; annales==acta; refold==standing (incl.
advancement replay-safety proven); flagship action executed against
the LIVE store closing a real park with a real decretum atomically;
ritus-signaculi instance RUNNING with K4 as its propositum; ledger
bar: the K3 seal itself performed via agere.
