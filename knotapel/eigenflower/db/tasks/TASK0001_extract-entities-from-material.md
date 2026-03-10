---
id: TASK0001
title: "Extract entities from material"
class: extract
trigger: "After new material is produced — experiments, research, brainstorming sessions, new papers"
inputs:
  - "New material (notes, experiment results, paper extractions, brainstorming transcripts)"
  - "Existing db records (so agents can check for duplicates/updates)"
method: agent_fan_out
agent_count: 8
outputs:
  - "New records in P, G, LIM, Q, UNK, PT, HYP, CON, REF tables"
  - "Updated records where new material bears on existing entities"
related: []
---

# Extract Entities from Material

## Purpose
Decompose new material into granular database records. This is the primary way the database grows. Each agent owns a table and works through the material with its specific lens.

## When to Run
- After completing an experiment (creates RES, updates Q/HYP/PT, may create COMP/UNK/CON)
- After reading new papers (creates P, PT, CON, REF records)
- After brainstorming/analysis sessions (creates HYP, CON, Q, UNK records)
- After any session that produces notes files or other written material

## Procedure

### Step 1: Identify the material
Collect all new files produced since last extraction pass. Could be:
- `notes/*.md` files
- `extractions/*.md` files
- Experiment result files
- Conversation transcripts with key ideas

### Step 2: Launch agents in parallel
Each agent gets:
- Its table's schema file (from `db/<table>/schema.md`)
- The new material to process
- A clear mandate: "you own <table>. Look for new records to create and existing records to update."

### Agent Roster (adapt as needed — not all agents needed every time)

| Agent | Table | Lens |
|-------|-------|------|
| Papers agent | P | Individual papers with metadata |
| Goals agent | G | Objectives, both explicit and implicit |
| Limitations agent | LIM | Chosen and discovered constraints |
| Q+UNK agent | Q, UNK | Questions and vaguer unknowns |
| PT agent | PT | Potential techniques suggested by material |
| HYP agent | HYP | Testable claims and theoretical hypotheses |
| CON agent | CON | Connections between otherwise unrelated things |
| REF agent | REF | External knowledge (DKC demos, theorems, texts) |

### Step 3: Review outputs
Spot-check a few records from each agent for:
- Schema compliance (frontmatter fields present and correct)
- No duplicates of existing records
- Reasonable content (not hallucinated or overly generic)

## First Run (2026-02-27)
Initial population from existing material:
- Source: 5 extraction files, 5 crossover files, 3 notes files, CLAUDE.md, knotapel briefing
- Result: 329 records across 9 tables
- P: 14, G: 28, LIM: 27, Q: 29, UNK: 30, PT: 73, HYP: 12, CON: 71, REF: 45
- Empty tables (correct — populated from actual work): COMP, DEC, EXP, FR, MOD, MILE, RES, VT

## Notes
- Agents write to different folders so no conflicts — safe to run all in parallel
- The `related` field may be left as `[]` on first pass — TASK0002 handles cross-referencing
- Not every agent is needed every time. After a single experiment, you might only need Q+UNK, COMP, RES, and CON agents
- After a brainstorming session, you might only need HYP, CON, Q, and PT agents
