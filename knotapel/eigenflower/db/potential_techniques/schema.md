# Potential Techniques (PT) Schema

## Frontmatter Fields

| Field | Required | Type | Description |
|-------|----------|------|-------------|
| id | yes | string | `PT####` — unique identifier |
| title | yes | string | Technique name |
| status | yes | enum | `untested` / `testing` / `validated` / `killed` / `promoted_to_kit` |
| source | no | list | Where this technique comes from (P IDs, REF IDs, brainstorming) |
| validated_as | no | string | VT ID if promoted to validated |
| promoted_to | no | string | KIT ID if promoted to a reusable building block |
| killed_by | no | string | COMP ID if killed |
| applicable_to | no | list | G IDs or MOD IDs this technique could serve |
| related | no | list | IDs of related records (any type) |

## Body
Description of the technique, how it would work, what it requires, known caveats from the source material.

## Example Frontmatter
```yaml
---
id: PT0001
title: "Kernel ridge regression for closed-form syllogism compilation"
status: untested
source: [P0002]
applicable_to: [G0001]
related: [REF0005, Q0003]
---
```
