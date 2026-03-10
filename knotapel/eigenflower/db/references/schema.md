# References (REF) Schema

## Frontmatter Fields

| Field | Required | Type | Description |
|-------|----------|------|-------------|
| id | yes | string | `REF####` — unique identifier |
| title | yes | string | Reference name |
| type | yes | enum | `dkc_demo` / `dkc_theorem` / `dkc_proof` / `textbook` / `web` / `corpus` / `other` |
| source_project | no | string | E.g., `knotapel`, `external` |
| demo_number | no | number | If DKC demo, the demo number (e.g., 92) |
| theorem_id | no | string | If DKC theorem, the ID from atlas (e.g., "P04") |
| url | no | string | URL if web resource |
| summary | yes | string | 1-2 sentence summary of what this reference establishes |
| related | no | list | IDs of related records (any type) |

## Body
Longer description of the reference, key results, relevance to eigenflower.

## Example Frontmatter
```yaml
---
id: REF0001
title: "DKC Demo 92 — Parity-Lock Theorem"
type: dkc_theorem
source_project: knotapel
demo_number: 92
theorem_id: "P04"
summary: "+/-q encoding is structurally locked to XOR/XNOR; AND/OR/MAJ impossible. Proven formally."
related: [P0001, CON0001, G0001]
---
```
