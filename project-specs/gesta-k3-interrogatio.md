# GESTA K3 — INTERROGATIO (raw Q&A)

2026-07-16. Interview for K3 (actiones + processus). 14 questions, 4
rounds. Preceded by a live field trip through the running TS smaragda
(claude_ai_smaragda MCP): the Book/checkout action anatomy, the
checkout_v2/v3/v4 versioning wart, the running Publication process
(3 lanes concurrent, gate/task/fetch/action steps), 16 process genera,
591 completed tasks. Oracle sections read before questioning: Actions
(smaragda.ts 3180–3673 complete: types, actionReducer, defineActionGenus,
recordInput, _substituteParams, _executeSideEffects, executeAction,
query helpers). Processes section (4655–5431) deliberately left for
explorer 1.

## Round 1 — scope + doctrine

**Q1. Scope.** Three separable strands: actions / processes+lanes /
machinula reducers.
**A: ACTIONS + PROCESSES** (the gradus ladder's K3). Machinula reducers
= own NAMED milestone with own interview (officina-grade ritual:
sandbox flags, source-digest fold versioning, vindex). Tier-1 names the
tier-2 seam.

**Q2. Validation posture.** Gate (preconditions) vs effects (writes).
**A: GATE BLOCKS, EFFECTS RECORD.** Preconditions refuse (a gate's
purpose is that the recipe must not run — refusing is the recipe
declining, not the log lying). Once gated in, effects follow house law:
machina violations land WITH custodia notae. An action that starts
always finishes.

**Q3. Provenance.** TS uses relational side tables (input,
action_taken) outside the event log.
**A: EXECUTION = EVENT.** 'actio-facta' on the action definition's own
res stream: {ligamina, argumenta, eventus:[ids]}. The action res is its
own execution ledger — replayable, annales-covered. TS input table
SUBSUMED (our events already carry actor/origo). No side tables ever
(db stays a projection).

**Q4. Side-effect vocabulary.** Closed verbs vs open event templates.
**A: CLOSED VERBS → EVENTS 1:1.** Sanctioned list mapping directly onto
event kinds; create_log/create_error collapse into nota (D4 — no Error
genus); create_task = creatio of opus. Closed = analyzable (affordances,
tooling). New verbs sanctioned deliberately like link verbs.

## Round 2 — action mechanics

**Q5. Substitution tokens.**
**A: LATIN SLATE + CREATA REFS.** $arg.X, $res.X (no '.id' suffix),
$nunc; PLUS the oracle-improvement: creatio effect may carry
'ut':"nomen" binding the fresh ULID → later effects reference
$novus.nomen. TS structurally cannot create-then-reference; our
flagship recipe needs it day one.

**Q6. Definition form + naming.**
**A: species:"actio" — OPES / ARGUMENTA / EFFECTUS.** Whole-JSON genus
document, definitio/emendatio-generis unchanged (kills the checkout_v4
wart — one action corrected in place, history in annales). Approved
preview = the flagship recipe verbatim (claudere-cum-decreto, spec §).

**Q7. Affordances.**
**A: KERNEL QUERY + RES LINE.** gesta_actiones_rei(res_id) matches
species:actio genera against the res's genus+status; surfaced as an
'actiones:' line in the tabularii res tool (K2 passive-surface
pattern).

**Q8. Mechanical failure mid-recipe** (vs machina violations, Q2).
**A: ROLLBACK + ACTIO-RECUSATA EVENT.** Whole bundle rolls back
(atomicity is the point), then one actio-recusata event on the action
res records {ligamina, argumenta, causa}. Gate-refusals write the same
event. The attempt ledger is complete; the log holds no half-recipes.

## Round 3 — processes

**Q9. Step types v1.**
**A: THREE — opus-gradus, actio-gradus, porta-gradus.** fetch
(context-flow model) and ramus (conditionals = tier-2 territory)
deferred as named parks.

**Q10. Advancement engine.**
**A: EAGER, NARROWLY TRIGGERED.** After a status event lands on a res
that is a MEMBRUM of a running process instance (K2 membra index = one
lookup), the engine advances: step completed, next steps spawned
atomically. No polling; non-process writes pay one indexed lookup.
NOTE: hook lives at SCRIBERE time, not fold time — replicare replays
advancement events as data, never re-fires the engine.

**Q11. Seed v3.**
**A: OPUS + FLAGSHIP ACTION + RITUAL PROCESS.** opus genus (ends
goldens-only exile), claudere-cum-decreto (approved preview verbatim),
and RITUS-SIGNACULI — the milestone ritual (interrogatio → spec →
exploratores → frusta → sigillum) as a process definition. K4's ritual
runs as K3's first live instance.

**Q12. MCP surface.**
**A: ONE TOOL — AGERE** {actio?|processus?, ligamina?, argumenta?}.
Execution + process instantiation. Everything else rides existing
grammar: task claim/complete = gerere status on opus (auto-advances via
the eager engine); discovery = res affordance line + quaerere.

## Round 4 — governance + ritual

**Q13. Who may define recipes?**
**A: RECIPES ARE CODE.** Definitions arrive via seed (committed) or
launcher pipe only — NOT via MCP. Agents execute freely, cannot mint
recipes mid-session (self-modifying automation deserves its own future
decision). Same posture as tools themselves.

**Q14. Ritual weight.**
**A: FULL — spec v1 → 3 EXPLORERS → spec-v2.** E1: TS Processes section
deep-read (4655–5431 unread; advancement internals, context model).
E2: gesta/tabularium seam audit (multi-event transactions!, hook
placement, membra reuse, agere shape). E3: LIVE smaragda mining (16
process genera, Demo Exploration instances, usage patterns, what
rotted).

## Carried into spec v1

Verb slate (proposed, spec §): creatio (cum 'ut'), mutatio, remotio,
status, nota, membrum-additum — the full existing event vocabulary,
each 1:1. Multi-event atomic transaction = the ONE new kernel
capability. Machinula seam named: effectus-list is tier-1; a future
'fons' field (C89 source) is tier-2; genus 'reducer' field already
reserved since K1.
