---
id: TASK0004
title: "Experiment debrief — post-experiment entity extraction"
class: extract
trigger: "After completing any experiment"
inputs:
  - "Experiment results/findings"
  - "Existing db records"
method: agent_fan_out
agent_count: 4-6
outputs:
  - "EXP record (what was done, what happened)"
  - "RES records (referenceable findings)"
  - "Updated Q records (answered questions)"
  - "Updated HYP records (confirmed/refuted hypotheses)"
  - "Updated PT records (validated → VT, or killed)"
  - "New COMP records (complications discovered)"
  - "New UNK records (new unknowns surfaced)"
  - "New Q records (new questions raised)"
  - "New CON records (unexpected connections found)"
related: [TASK0001]
---

# Experiment Debrief

## Purpose
The primary yield of each experiment is not the code or the numbers — it's the database updates. This task ensures every experiment is properly decomposed into the records that enable steering.

## When to Run
After every experiment, before starting the next one.

## Procedure

### Step 1: Write up findings
Produce a findings document (can be a notes file or experiment log) capturing:
- What was attempted
- What happened
- What worked / didn't work
- What was surprising
- What questions were answered
- What new questions arose

### Step 2: Launch debrief agents
Focused subset of TASK0001 agents, plus experiment-specific ones:

| Agent | Creates/Updates | Lens |
|-------|----------------|------|
| EXP agent | EXP | Create the experiment record with metadata and summary |
| RES agent | RES | Extract referenceable empirical findings |
| Status agent | Q, HYP, PT | Update statuses: Q→answered, HYP→confirmed/refuted, PT→validated/killed |
| Discovery agent | COMP, UNK, Q | Create new complications, unknowns, and questions that emerged |
| CON agent | CON | Look for connections to OTHER modules/goals that weren't the experiment's focus |

### Step 3: Review the delta
Look at what changed in the database. The diff IS the experiment's value. Ask:
- Did this experiment advance any goals?
- Did it unblock anything?
- Did the complications it revealed change priorities?
- Did the connections it surfaced create new opportunities?

## Key Principle
The CON agent in the debrief is the most important one for the maximalist approach. An experiment aimed at goal A might produce a side result that unblocks goal M. The CON agent's job is to find these cross-pollination moments.
