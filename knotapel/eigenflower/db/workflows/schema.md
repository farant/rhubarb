# Workflows (WF) Schema

Multi-step processes that chain tasks together with decision gates, branching, and human/agent checkpoints. Like Smaragda processes — sagas with lanes and stages.

## Frontmatter Fields

| Field | Required | Type | Description |
|-------|----------|------|-------------|
| id | yes | string | `WF####` — unique identifier |
| title | yes | string | Workflow name |
| trigger | no | string | When this workflow is initiated |
| participants | no | list | Who's involved (e.g., `fran`, `team_lead`, `agents`) |
| estimated_stages | no | number | How many stages in the typical run |
| related | no | list | IDs of related records |

## Body

### Stage Format
Each stage in the workflow body should include:

```
### Stage N: <name>
- **Actor**: who does this (fran / team_lead / agent / script)
- **Input**: what this stage needs
- **Action**: what happens
- **Output**: what this stage produces
- **Decision gate**: [if applicable] what decision is made here and by whom
- **Branches**: [if applicable] what paths are available based on the decision
- **Tasks**: [if applicable] which TASK records are invoked
```

### Decision Gates
Stages where a human or team lead reviews output and decides the next step. These are explicit pauses — the workflow doesn't proceed automatically past a gate.

### Branching
Some stages have conditional paths. Document all paths and the criteria for choosing each one.
