---
id: TASK0005
title: "Generate steering views"
class: aggregate
trigger: "On demand, before deciding what to work on next"
inputs:
  - "All db records with populated related/depends_on/blocks fields"
method: hybrid
outputs:
  - "Ripeness map across all MODs"
  - "Critical path analysis per goal"
  - "Ready queue (what can be worked on now)"
  - "Blocking analysis (what's the bottleneck)"
  - "Orphan scan (forgotten records)"
  - "Branch health (per-module trends)"
related: [TASK0002]
---

# Generate Steering Views

## Purpose
Answer the two core questions: "What should we work on next?" and "What do we need to do the best experiment possible?" These views are generated from the database, never maintained by hand.

## When to Run
- Before deciding what to work on next
- After a major extraction/cross-reference pass
- Periodically to check project health

## Views

### 1. Ready Queue (script) — IMPLEMENTED
`bun tools/show-ready-queue-of-actionable-items.ts`

Finds all records with status open/untested/proposed whose `depends_on` are all resolved. Output: grouped by table, sorted by ID.

### 2. Critical Path (script) — IMPLEMENTED
`bun tools/show-dependency-chain-for-goal.ts <GOAL_ID>`

Walks `depends_on` recursively from a target goal. Shows unresolved dependencies as a tree and identifies ready leaf nodes (things that can be worked on now to advance the goal).

### 3. Blocking Analysis (script) — IMPLEMENTED
`bun tools/show-blocking-bottleneck-analysis.ts`

Counts how many times each record appears in `depends_on`, `blocks`, `impacts`, `blocked_by` fields across the entire database. Top 30 bottlenecks ranked by count, showing what each blocks.

### 4. Ripeness Map (script + LLM)
For each MOD, assess:
- How many of its MILEs are close to completion?
- When was it last advanced (most recent EXP)?
- What just became unblocked due to work elsewhere?
Script computes the metrics, LLM interprets and ranks by ripeness.

### 5. Orphan Scan (script) — IMPLEMENTED
`bun tools/find-orphan-records-with-no-inbound-references.ts`
`bun tools/find-orphan-records-with-no-inbound-references.ts <table>` (filter by table)

Finds records with no inbound references — nothing points to them in any frontmatter field or body text. Potentially forgotten items. Grouped by table.

### 6. Connection Distance (LLM)
Find CON records that span the most distant goals/modules. These represent cross-pollination opportunities — areas where work in one domain might unlock another.

### 7. Branch Health (script)
Per module, compute over recent experiments:
- VT creation rate (are we validating techniques?)
- COMP creation rate (are we hitting walls?)
- Q resolution rate (are questions getting answered?)
- UNK growth vs resolution (is fog clearing or thickening?)
Output: per-module health dashboard.

### 8. Phase Detection (script)
Track ratios of record types created over time:
- High PT creation = exploration phase
- High VT creation = convergence phase
- High COMP creation = hitting walls, maybe pivot needed
- High CON creation = synthesis phase
Output: current phase assessment.

## Additional Tools
- `bun tools/count-records-per-table.ts` — quick database overview (record counts per table)
- `bun tools/search-records-by-text-in-title-or-body.ts <term>` — full-text search across all records
- `bun tools/show-record-detail-by-id.ts <ID>` — drill into any record from the views above
- `bun tools/find-all-references-to-id.ts <ID>` — see everything that references a particular record

## Implementation Path
Views 1, 2, 3, 5 are IMPLEMENTED as Bun scripts. Views 4, 6, 7, 8 still need implementation — start with 7 (Branch Health) and 8 (Phase Detection) as they're pure script, then 4 (Ripeness Map) and 6 (Connection Distance) which need LLM interpretation.
