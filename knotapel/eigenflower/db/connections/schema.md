# Connections (CON) Schema

## Frontmatter Fields

| Field | Required | Type | Description |
|-------|----------|------|-------------|
| id | yes | string | `CON####` — unique identifier |
| title | yes | string | Short description of what connects |
| connects | yes | list | IDs of the 2+ records being connected |
| type | yes | enum | `mathematical_identity` / `analogy` / `technique_transfer` / `structural_parallel` / `dependency` / `cross_pollination` |
| strength | yes | enum | `strong` / `moderate` / `speculative` |
| source | no | string | Where this connection was noticed |
| related | no | list | IDs of related records (any type) |

## Body
Explanation of the connection — what links these things, why it matters, what it implies. For mathematical identities, show the correspondence. For technique transfers, explain what carries over and what doesn't.

## Example Frontmatter
```yaml
---
id: CON0001
title: "DKC forward computation IS tensor logic over Boolean tensors"
connects: [REF0001, P0001]
type: mathematical_identity
strength: strong
source: "extractions/group-d-knotapel-relevance.md"
related: [G0001, HYP0002]
---
```
