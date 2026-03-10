# Limitations (LIM) Schema

## Frontmatter Fields

| Field | Required | Type | Description |
|-------|----------|------|-------------|
| id | yes | string | `LIM####` — unique identifier |
| title | yes | string | Short description of the limitation |
| type | yes | enum | `chosen` (self-imposed) / `discovered` (found through work) |
| source | no | string | Where this limitation comes from (e.g., "CLAUDE.md", "EXP0015", "hardware") |
| impacts | no | list | IDs of records this limitation constrains |
| related | no | list | IDs of related records (any type) |

## Body
Full description of the limitation, why it exists (if chosen), how it was discovered (if discovered), and what it implies for the project.

## Example Frontmatter
```yaml
---
id: LIM0001
title: "Training time limited to 30-60 minutes on M2"
type: chosen
source: "hardware/time constraint"
impacts: [G0002, PT0005]
related: [P0007]
---
```
