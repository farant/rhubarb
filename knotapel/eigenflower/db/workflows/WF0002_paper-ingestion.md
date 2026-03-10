---
id: WF0002
title: "Paper ingestion — read, extract, connect"
trigger: "When new papers are found or recommended"
participants: [team_lead, agents]
estimated_stages: 5
related: [TASK0001, TASK0002, TASK0006]
---

# Paper Ingestion

Process for absorbing new papers into the database so their knowledge becomes available for steering.

## Stage 1: Discover / Receive Paper
- **Actor**: fran or team_lead or researcher
- **Input**: Paper URL, recommendation, or search result
- **Action**: Download PDF to `pdfs/`, note the source
- **Output**: PDF in `pdfs/`, basic metadata noted

## Stage 2: Extract Paper Summary
- **Actor**: single agent
- **Input**: PDF or paper text
- **Action**: Read the paper, produce an extraction document in `extractions/` following the format of existing extraction files. Key contributions, techniques, eigenflower relevance, key equations.
- **Output**: Extraction file

## Stage 3: Entity Extraction
- **Actor**: agents (subset of TASK0001)
- **Input**: Extraction file
- **Action**: Create P record for the paper. Create PT records for techniques. Create CON records for connections to existing knowledge. Create REF records if the paper references relevant external results.
- **Output**: New P, PT, CON, REF records
- **Tasks**: TASK0001 (subset — P, PT, CON, REF agents only)

## Stage 4: Gap Detection
- **Actor**: single agent (TASK0006)
- **Input**: New extraction + existing Q/UNK records
- **Action**: Does this paper surface new unknowns? New questions? Does it answer existing questions?
- **Output**: New/updated Q and UNK records
- **Tasks**: TASK0006

## Stage 5: Cross-Reference
- **Actor**: agents (TASK0002)
- **Input**: New records from Stages 3-4
- **Action**: Link new records to existing database
- **Output**: Updated relationship graph
- **Tasks**: TASK0002
- **Decision gate**: Team lead reviews connections — any of these warrant changing priorities?

## Notes
- Can be batched: ingest multiple papers at once, running Stage 2 in parallel
- The extraction format should match the existing group-X extraction files for consistency
- Stage 5 is where the real value often appears — a new paper might connect to a stale module and make it ripe
