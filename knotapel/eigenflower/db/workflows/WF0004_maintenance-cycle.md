---
id: WF0004
title: "Maintenance cycle — validate, clean, aggregate"
trigger: "Periodically (every 10-20 new records), or before major planning sessions"
participants: [team_lead, agents, scripts]
estimated_stages: 4
related: [TASK0002, TASK0003, TASK0005, TASK0006]
---

# Maintenance Cycle

Periodic health check on the database. Ensures schema compliance, populates missing links, surfaces forgotten records, and generates fresh steering views.

## Stage 1: Schema Validation
- **Actor**: script + agent (TASK0003)
- **Input**: All records, all schemas
- **Action**: Validate every record against its schema. Fix anomalies.
- **Output**: Clean records, anomaly report (if any remain unresolved)
- **Tasks**: TASK0003

## Stage 2: Cross-Reference Refresh
- **Actor**: agents (TASK0002)
- **Input**: All records
- **Action**: Full cross-reference pass. Populate missing `related` fields. Check for broken references.
- **Output**: Fully linked database
- **Tasks**: TASK0002

## Stage 3: Gap Detection
- **Actor**: single agent (TASK0006)
- **Input**: Full database + recent notes/experiment results
- **Action**: Fresh-eyes pass looking for undocumented unknowns, implicit questions, blind spots
- **Output**: New UNK/Q records
- **Tasks**: TASK0006

## Stage 4: Generate Steering Views
- **Actor**: scripts + agent (TASK0005)
- **Input**: Clean, fully linked database
- **Action**: Generate all steering views: ready queue, critical path, ripeness map, orphan scan, etc.
- **Output**: Fresh steering views
- **Tasks**: TASK0005
- **Decision gate**: Fran + team_lead review views. Any priority changes? Any stale modules to revisit? Any orphans to address or prune?

## Notes
- This is the "defrag" equivalent for the database
- Stage 4 output is the primary input for WF0001 Stage 1 (experiment selection)
- Don't skip Stage 3 — gap detection on a mature database surfaces things early extraction missed
- Frequency: roughly every 10-20 new records, or whenever steering feels uncertain
