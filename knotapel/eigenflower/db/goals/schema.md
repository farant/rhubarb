# Goals (G) Schema

## Frontmatter Fields

| Field | Required | Type | Description |
|-------|----------|------|-------------|
| id | yes | string | `G####` — unique identifier |
| title | yes | string | Short goal name |
| status | yes | enum | `open` / `in_progress` / `achieved` / `deferred` |
| priority | yes | enum | `high` / `medium` / `low` |
| frames | no | list | FR IDs — which frames this goal belongs to |
| depends_on | no | list | IDs this goal depends on |
| blocks | no | list | IDs that depend on this goal |
| related | no | list | IDs of related records (any type) |

## Body
Description of the goal, why it matters, what "achieved" looks like.

## Example Frontmatter
```yaml
---
id: G0001
title: "Compile syllogisms to ANN weights"
status: open
priority: high
depends_on: []
blocks: [G0003, G0005]
related: [P0001, P0002, PT0001, PT0002]
---
```
