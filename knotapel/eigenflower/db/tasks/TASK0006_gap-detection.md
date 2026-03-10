---
id: TASK0006
title: "Gap detection — find undocumented unknowns"
class: quality
trigger: "After reading new material, or periodically"
inputs:
  - "Any material (notes, experiment results, transcripts)"
  - "Existing UNK and Q records"
method: single_agent
outputs:
  - "New UNK records for previously unnoticed gaps"
  - "New Q records for implicit questions"
related: [TASK0001]
---

# Gap Detection

## Purpose
Explicitly look for things we DON'T know that we don't know. An agent reads material and asks: "what's uncertain here, what's assumed but not verified, what's left unsaid?"

## When to Run
- After any brainstorming session
- After reading new papers
- Periodically on existing notes/records as a fresh-eyes pass

## Procedure

### Step 1: Agent reads material
Give the agent:
- The material to analyze
- The existing Q and UNK records (so it doesn't duplicate — use `bun tools/list-all-records-in-table.ts questions` and `bun tools/list-all-records-in-table.ts unknowns` for current inventory)
- Use `bun tools/search-records-by-text-in-title-or-body.ts <term>` to check if a potential gap is already captured before creating a new record
- The prompt: "Identify all undocumented unknowns. What's uncertain? What's assumed? What's in a blind spot? What hasn't been tested? What could go wrong that nobody has mentioned?"

### Step 2: Agent creates UNK/Q records
New unknowns go to `db/unknowns/`, new questions to `db/questions/`.

### Step 3: Review
Gap detection agents tend to over-generate. Review and delete records that are too vague or not actually relevant.

### Step 4: Structural gap detection
Run `bun tools/find-orphan-records-with-no-inbound-references.ts` to find records nothing points to. Orphans may indicate:
- A gap in cross-referencing (run TASK0002)
- A genuinely isolated idea that needs connecting to the graph
- Something that was extracted but doesn't actually belong

## Key Value
This is the task that makes the database smarter than any individual. People (and LLMs) naturally focus on what they DO know. This task forces attention to the gaps. It's most valuable when run by a different agent than the one that produced the material — fresh perspective catches what familiarity misses.
