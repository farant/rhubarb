---
id: TASK0002
title: "Cross-reference pass — populate related fields"
class: quality
trigger: "After TASK0001 extraction, or periodically as database grows"
inputs:
  - "All db records across all tables"
method: agent_fan_out
agent_count: 4-6
outputs:
  - "Updated `related` fields across all records"
  - "Updated `depends_on`, `blocks`, `impacts`, `connects` fields where applicable"
  - "Report of new connections discovered during cross-referencing"
related: [TASK0001]
---

# Cross-Reference Pass

## Purpose
After entity extraction, most `related` fields are empty. This task reads across tables and populates the relational links. It's what turns a collection of records into a connected graph.

## When to Run
- After every TASK0001 extraction pass
- Periodically (every 10-20 new records) as maintenance
- Before running TASK0005 (aggregate steering views) since views depend on the link graph

## Procedure

### Step 1: Launch cross-reference agents
Each agent owns a subset of tables and reads ALL tables to find links.

| Agent | Owns | Looks For |
|-------|------|-----------|
| Goals + Limitations agent | G, LIM | Which LIMs constrain which Gs? Which Gs depend on each other? |
| Questions + Unknowns agent | Q, UNK | Which Qs relate to which Gs, Ps, PTs? Which UNKs relate to which Qs? |
| Techniques agent | PT, VT | Which PTs come from which Ps? Which PTs serve which Gs? Which PTs relate to which Qs? |
| Connections + References agent | CON, REF | Link CON `connects` fields to actual record IDs. Link REFs to Ps, Gs, PTs. |
| Hypotheses agent | HYP | Link `evidence_for`/`evidence_against` to specific RES, EXP, REF records. Link `testable_by` to specific PTs or EXPs. |

### Step 2: Validate links
Quick check that all referenced IDs actually exist. Use tools:
- `bun tools/find-all-references-to-id.ts <ID>` — verify that references point to real records and see the full inbound reference graph for any ID
- `bun tools/find-orphan-records-with-no-inbound-references.ts` — find records that nothing points to (may indicate missed cross-references)
- `bun tools/find-orphan-records-with-no-inbound-references.ts <table>` — filter orphan scan to a specific table

### Step 3: Report new connections
During cross-referencing, agents often discover connections that weren't in any CON record. These should be created as new CON records.

## Notes
- This is where the database becomes queryable — the links ARE the query paths
- Cross-referencing is idempotent — running it twice won't create duplicates, just fill in anything missed
- First priority: link PTs to Ps (source papers), link Gs to each other (dependency graph), link Qs to Gs (what goal does this question serve)
