# Unknowns (UNK) Schema

## Frontmatter Fields

| Field | Required | Type | Description |
|-------|----------|------|-------------|
| id | yes | string | `UNK####` — unique identifier |
| title | yes | string | What's unknown — can be vague |
| status | yes | enum | `open` / `resolved` / `matured_to_Q` |
| matured_to | no | string | Q ID if this unknown was articulated into a specific question |
| resolved_by | no | list | IDs of records that resolved this |
| source | no | string | Where this unknown was identified |
| related | no | list | IDs of related records (any type) |

## Body
Description of the gap in knowledge, why it might matter, what area it affects.

## Example Frontmatter
```yaml
---
id: UNK0001
title: "How much of the Summa's reasoning is actually syllogistic vs analogical"
status: open
source: "notes/02-architecture-insight.md"
related: [G0001, G0004]
---
```
