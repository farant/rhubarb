---
id: WF0005
title: "Module recognition — identify and formalize emerging modules"
trigger: "When a cluster of related records suggests a coherent area of work"
participants: [fran, team_lead]
estimated_stages: 4
related: []
---

# Module Recognition

Process for recognizing that a set of goals, questions, techniques, and experiments have coalesced into a coherent module — and formalizing it so the ripeness view can track it.

## Stage 1: Notice the Pattern
- **Actor**: fran or team_lead
- **Input**: Database state, steering views, intuition
- **Action**: Recognize that several records cluster around a coherent theme. Could be triggered by:
  - Multiple Qs and PTs pointing at the same area
  - A CON record linking distant records through a common concern
  - A brainstorm that reveals a coherent sub-problem
  - Repeated COMPs in the same area suggesting a real module boundary
- **Output**: Informal description of the emerging module

## Stage 2: Formalize the Module
- **Actor**: team_lead
- **Input**: Cluster description
- **Action**: Create a MOD record with:
  - Title and description
  - Which Gs it serves
  - Which existing records belong to it (Qs, PTs, UNKs, etc.)
  - Current status assessment
- **Output**: MOD record in `db/modules/`

## Stage 3: Define Milestones
- **Actor**: fran + team_lead
- **Input**: MOD record
- **Action**: Define 2-5 MILE records for this module. What are the meaningful checkpoints? What does "ripe to advance" look like? What does "done enough for now" look like?
- **Output**: MILE records in `db/milestones/`
- **Decision gate**: Fran approves milestones

## Stage 4: Assign a Frame
- **Actor**: fran + team_lead
- **Input**: MOD record
- **Action**: Which FR does this module belong to? Does it need a new frame? Does it span multiple frames?
- **Output**: Updated MOD record with frame assignment, possibly new FR record
- **Decision gate**: Fran approves frame assignment

## Notes
- Modules are RECOGNIZED, not planned. They emerge from the work.
- Don't force module creation — if the records don't naturally cluster, there's no module yet
- The ripeness view (TASK0005) only works for modules that have milestones. Defining milestones is what makes a module steerable.
- A module can split (you realize it's actually two things) or merge (you realize two modules are the same thing). Update records accordingly.
- Early in the project, most work won't belong to formal modules. That's fine. Modules emerge as patterns clarify.
