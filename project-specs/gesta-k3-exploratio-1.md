# K3 EXPLORATIO 1 — oraculum (TS Processes 4655-5428)
# Relatio exploratoris verbatim, 2026-07-16. Fons pro spec-v2.

HEADLINE: the oracle has NO general cross-lane dependency mechanism
and NO `post` field. Spec-v1 §IV invents `post`, `opes`/`ligamina`,
and `$ops` param templating — none exist in the oracle. Advancement
is triggered ONLY by task completion, not a generic status hook.

## 1. Instance storage
- Instance = ordinary res, genus_id = process genus (createRes, 4982);
  def via getProcessDef(res.genus_id) (5405). Matches spec-v1.
- Process genera = res under META_GENUS_ID, meta.kind="process" (4909).
- Step statuses live ENTIRELY in the instance res's own stream, folded
  by processInstanceReducer (4807-4879) into state.steps (4741-4749).
  Events: step_activated, step_completed, step_failed, step_skipped,
  step_task_created, step_action_executed. NO side tables, NO membra.
- Cached in _processCache keyed id\0branch (5011, 5024); invalidation
  at append (594-609) with a subtle cold-cache staleness trap.

## 2. Step/lane encoding
ProcessStepDef (4688-4711) COMPLETE fields: name, type, lane, position,
action_name/action_params/action_resource_bindings (action),
task_title/description/priority/target_agent_type (task),
fetch_source/fetch_into (fetch; fetch_into DEAD - never read),
gate_conditions: string[] (gate), branch_condition/map/default (branch).
- Sequencing = (lane, position) ONLY. In-lane strictly sequential.
- ONLY cross-lane primitive: gate_conditions (ALL-OF list of step
  names, 5288-5292). NO post/depends_on anywhere.
- ProcessLaneDef.position NEVER read (lanes grouped from step.lane
  strings, 5260-5265). Lane order decorative.
- Defs append-only events (process_lane_defined/process_step_defined/
  process_trigger_defined); evolveProcessGenus = last-writer-wins per
  step name (4788). No whole-replace.

## 3. Advancement algorithm (_advanceProcess 5248-5377)
1. status != running -> return.
2. Group by lane, sort by position.
3. Per lane walk: terminal -> continue; ACTIVE -> break (lane parked);
   else next step:
   - gate: ALL conditions completed? no -> gate_evaluated{false} +
     break (re-emitted EVERY pass - noise); yes -> step_activated +
     gate_evaluated{true} + step_completed, continue.
   - action: step_activated, execute SYNCHRONOUSLY, fail -> break.
   - task: step_activated + spawn, then BREAK (lane waits).
   - fetch/branch: synchronous, continue.
4. madeProgress -> RECURSE (fixpoint 5346-5349).
5. No-progress pass: allDone -> process_completed; anyFailed &&
   !anyActive && !allDone -> process_failed (5355-5376).
TRIGGERS: startProcess (4992) and completeTask -> 
_checkProcessTaskCompletion -> _advanceProcess (4602, 5423). NOTHING
ELSE. Only task_step waits; only completeTask resumes.
Fixpoint RE-MATERIALIZES the fold after every micro-append
(5313/5321/5328/5335) - assumes each append committed/visible.
OPPOSITE of what our atomic bundle wants (in-memory threading).

## 4. task_step + completeTask hook
_executeTaskStep (5160-5183): createTask with context_res_ids=
[process_id], associated_res_id=instance.context_res_id; then
setAttribute step_name + lane_name on task; then step_task_created
{step_name, task_id} on instance (fold stores steps[].task_id).
Linkage = task attributes + stored task_id. NOT relationship_member.
_checkProcessTaskCompletion (5381-5428): parse task context_res_ids ->
for each: genus meta.kind=process? instance running? find step where
task_id matches AND status active -> step_completed{result} ->
_advanceProcess -> break.

## 5. action_step (_executeActionStep 5108-5158)
findActionByName; bindings from action_resource_bindings with EXACTLY
ONE substitution: literal "$context.res_id" -> instance context_res_id
(5129-5130); everything else verbatim. action_params passed VERBATIM -
no substitution (5137). executeAction runs the action's own gate +
internal token resolution. error -> step_failed; success ->
step_action_executed{action_taken_id} + step_completed.

## 6. fetch/branch (deferred - what they actually are)
- fetch (5185-5203): materialize context_res_id, read ONE attribute
  state[fetch_source], step_completed{result: value}. fetch_into dead.
- branch (5205-5244): read state[branch_condition] off context res,
  target = branch_map[value] ?? branch_default (step NAME); skip every
  step in SAME LANE strictly between branch and target; forward-only.
Both depend entirely on reading fields off the single context_res_id.
Park as a PAIR (branch needs fetch's field-read substrate).

## 7. Failure/cancel
- Failed step: lane-local break; other lanes continue. Terminal status.
- process_failed: anyFailed && !anyActive && !allDone (5366-5376).
- Blocked gate re-emits gate_evaluated{false} every advancement pass.
- cancelProcess (4998-5008): process_cancelled event only; spawned
  tasks ORPHAN (no cascade).
- Instance statuses: running/completed/FAILED/cancelled (4754).

## 8. Cron boundary
Section 5431+. ProcessTriggerDef (4718-4724) stored but INERT - engine
never consumes triggers; cron subsystem reads them. Out of K3 scope.

## SPEC V1 BREAKS
B1: `post` DOES NOT EXIST. Only gate_conditions joins lanes. Ordinary
    steps CANNOT wait on foreign lanes. Spec-v2 must choose: (a) gate-
    only model, rewrite ritus-signaculi routing joins through portae;
    (b) deliberately EXTEND oracle with post (own it in ledger).
B2: Advancement trigger = completeTask ONLY in oracle. Our
    status-on-membrum hook is a GENERALIZATION to own, not a port.
    Needs step->opus resolution (oracle stores task_id; membra role
    can serve, but that's B3).
B3: Instance<->task linkage = attributes (context_res_ids + step_name
    + stored task_id), NOT membra. Our membra choice = deliberate
    divergence, and B2's trigger depends on it.
B4: Process opes/ligamina + $ops templating DO NOT EXIST. startProcess
    takes single context_res_id; action-step bindings support only
    literal "$context.res_id"; params never substituted. Spec-v2 must
    BUILD named process bindings + token resolver as a K3 FEATURE -
    ritus-signaculi cannot run without it.
B5: Spec-v1 drops failed/cancelled instance states (oracle has 4).
    AND the key undefined interaction: a refused actio-gradus (gesta
    actions write actio-recusata, never throw) - does it fail the
    step, stall the lane, or record-and-continue? MUST DECIDE.
B6: Lane position decorative in oracle (concurrent, unordered).
B7: Porta events: passed gate = 3 events; blocked gate re-emits noise
    every pass. Spec-v2: decide porta event vocabulary + dedup (our
    eager trigger amplifies the noise vector).
LOAD-BEARING FOR E2: oracle fixpoint re-reads committed state after
every append; atomic bundle wants one in-memory fold threaded through
the fixpoint, all events in one bundle, NO mid-bundle disk re-reads.
