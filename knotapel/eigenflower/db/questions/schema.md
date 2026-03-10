# Questions (Q) Schema

## Frontmatter Fields

| Field | Required | Type | Description |
|-------|----------|------|-------------|
| id | yes | string | `Q####` — unique identifier |
| title | yes | string | The question, clearly articulated |
| status | yes | enum | `open` / `answered` / `superseded` |
| answer | no | string | Brief answer (if status is `answered`) |
| answered_by | no | list | IDs of records that answered this (EXP, RES, etc.) |
| source | no | string | Where/how this question arose |
| related | no | list | IDs of related records (any type) |

## Body
Context for the question — why it matters, what answering it would unlock, any partial knowledge.

## Example Frontmatter
```yaml
---
id: Q0001
title: "Does Barbara compile to a working tensor equation in exact Z[zeta_8] arithmetic?"
status: open
source: "notes/01-analysis.md"
related: [G0001, P0001, PT0001]
---
```
