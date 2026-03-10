---
id: WF0003
title: "Brainstorm harvest — capture and decompose ideas"
trigger: "After brainstorming or analysis sessions"
participants: [fran, team_lead, agents]
estimated_stages: 4
related: [TASK0001, TASK0006]
---

# Brainstorm Harvest

Process for capturing ideas from brainstorming sessions (like the one that produced wisdom circuits, hybrid architecture, etc.) and decomposing them into database records.

## Stage 1: Brainstorm
- **Actor**: fran + team_lead (or fran + explorer in team mode)
- **Input**: A topic, a question, a hunch, a connection noticed
- **Action**: Free-form discussion. Follow the thread wherever it goes.
- **Output**: Notes file in `notes/` capturing the key ideas

## Stage 2: Entity Extraction
- **Actor**: agents (TASK0001)
- **Input**: Notes file
- **Action**: Decompose into records. Brainstorming typically produces: HYP (new hypotheses), CON (new connections), Q/UNK (new questions and unknowns), PT (new potential techniques), G (new goals if the brainstorm opens a new direction)
- **Output**: New records across multiple tables
- **Tasks**: TASK0001 (subset — HYP, CON, Q, UNK, PT agents; possibly G)

## Stage 3: Gap Detection
- **Actor**: single agent (TASK0006)
- **Input**: Notes file + new records from Stage 2
- **Action**: What did the brainstorm assume but not verify? What blind spots are visible from outside?
- **Output**: New UNK records for undocumented unknowns
- **Tasks**: TASK0006

## Stage 4: Review and Connect
- **Actor**: fran + team_lead
- **Input**: All new records from Stages 2-3
- **Action**: Review the decomposition. Anything missing? Anything over-generated? Any connections to existing modules that the agents missed?
- **Output**: Cleaned records, manual CON records if needed
- **Decision gate**: Fran reviews — does this brainstorm warrant a new MOD? New G records? Priority shifts?

## Notes
- Brainstorming is one of the three input channels (along with experiments and research)
- The most common output is HYP and CON records — ideas and connections
- Don't force structure on the brainstorm itself (Stage 1). Structure comes in Stage 2.
- The review in Stage 4 is important — brainstorm extraction agents tend to over-generate
