# Kits (KIT) Schema

A KIT is a reusable building block — a bounded set of functionality with a clear input/output contract, no experimental uncertainty, and deterministic implementation. KITs are the eigenflower equivalent of rhubarb's `lib/` libraries: each one does one thing, has tests, and composes with others.

The key distinction from PT (Potential Techniques): a PT has experimental uncertainty ("will this work?"). A KIT has implementation effort only ("this clearly works, just build it").

## Frontmatter Fields

| Field | Required | Type | Description |
|-------|----------|------|-------------|
| id | yes | string | `KIT####` — unique identifier |
| title | yes | string | Short name for the building block |
| status | yes | enum | `planned` / `in_progress` / `complete` / `blocked` |
| tier | yes | enum | `0` / `1` / `2` / `3` / `4` — dependency tier (0 = no deps, foundational) |
| input | yes | string | What it takes in |
| output | yes | string | What it produces |
| depends_on | no | list | KIT or other IDs this depends on |
| blocks | no | list | IDs that depend on this |
| migrated_from | no | string | PT ID if this was promoted from a potential technique |
| source | no | list | Where the algorithm/design comes from (P IDs, REF IDs, textbooks) |
| effort | no | enum | `trivial` / `small` / `medium` / `large` |
| applicable_to | no | list | G IDs this kit serves |
| related | no | list | IDs of related records (any type) |

## Body
Description of what the kit does, how the algorithm works, known caveats, API sketch if relevant.

## Example Frontmatter
```yaml
---
id: KIT0001
title: "Heaviside step function"
status: planned
tier: 0
input: "Real number x, threshold t"
output: "1 if x >= t, 0 otherwise"
depends_on: []
effort: trivial
source: []
applicable_to: [G0005]
related: [KIT0003]
---
```
