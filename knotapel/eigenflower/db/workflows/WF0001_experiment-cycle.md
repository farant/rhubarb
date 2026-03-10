---
id: WF0001
title: "Experiment cycle — plan, execute, harvest, steer"
trigger: "Whenever we decide to run an experiment"
participants: [fran, team_lead, agents]
estimated_stages: 7
related: [TASK0001, TASK0002, TASK0004, TASK0005]
---

# Experiment Cycle

The core loop. Plan an experiment, execute it, harvest the database updates, then use the updated database to decide what's next.

## Stage 1: Select Experiment
- **Actor**: fran + team_lead
- **Input**: steering views from TASK0005 (or intuition if views aren't generated yet)
- **Action**: Review what's ripe, what's blocked, what's high-value. Pick the next experiment.
- **Output**: Chosen experiment target — which Q to answer, which PT to test, which HYP to probe
- **Decision gate**: Fran approves the experiment choice

## Stage 2: Design Experiment
- **Actor**: team_lead (with fran input)
- **Input**: The target Q/PT/HYP, relevant P/REF/LIM records, prior EXP records in the same area
- **Action**: Design the experiment — what to build, what to measure, what constitutes success/failure, known risks from COMP records
- **Output**: Experiment plan (could be a notes file or inline)
- **Decision gate**: Fran approves the plan or adjusts scope

## Stage 3: Execute Experiment
- **Actor**: team_lead + fran
- **Input**: Experiment plan
- **Action**: Build the thing. Write C89 code, run it, collect results.
- **Output**: Code, results, findings

## Stage 4: Debrief — Entity Extraction
- **Actor**: agents (via TASK0004)
- **Input**: Experiment results/findings
- **Action**: Fan out debrief agents to decompose results into database records
- **Output**: New/updated EXP, RES, Q, UNK, COMP, CON, HYP, PT/VT records
- **Tasks**: TASK0004

## Stage 5: Cross-Reference
- **Actor**: agents (via TASK0002)
- **Input**: New records from Stage 4
- **Action**: Link new records to existing database, populate related fields
- **Output**: Updated relationship graph
- **Tasks**: TASK0002

## Stage 6: Update Steering Views
- **Actor**: scripts + agents (via TASK0005)
- **Input**: Updated database
- **Action**: Regenerate ready queue, critical path, ripeness map
- **Output**: Fresh steering views
- **Tasks**: TASK0005

## Stage 7: Decide Next
- **Actor**: fran + team_lead
- **Input**: Updated steering views, intuition, interest
- **Action**: Look at what's ripe, what just unblocked, what connections emerged. Pick the next experiment.
- **Output**: Go to Stage 1 of next cycle
- **Decision gate**: Fran picks direction

## Notes
- Stages 4-6 can be compressed or skipped for small experiments
- Stage 7 → Stage 1 is the loop — this workflow never really ends, it cycles
- The quality of Stage 1 improves as the database grows — more data = better steering
- Early cycles may skip Stage 6 if the database is too sparse for useful views
