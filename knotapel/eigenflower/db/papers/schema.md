# Papers (P) Schema

## Frontmatter Fields

| Field | Required | Type | Description |
|-------|----------|------|-------------|
| id | yes | string | `P####` — unique identifier |
| title | yes | string | Full paper title |
| authors | yes | list | Author names |
| year | yes | number | Publication year |
| arxiv_id | no | string | ArXiv identifier (e.g., `2501.00817`) |
| url | no | string | URL to paper |
| extraction_group | no | string | Which extraction group (A/B/C/D/E) |
| relevance | yes | enum | `high` / `medium` / `low` — relevance to eigenflower |
| key_techniques | no | list | Main techniques/concepts from the paper |
| summary | yes | string | 1-3 sentence summary |
| related | no | list | IDs of related records (any type) |

## Status Values
Papers don't have a status — they exist or they don't.

## Example Frontmatter
```yaml
---
id: P0001
title: "Tensor Logic for Neural Networks"
authors: [Pedro Domingos]
year: 2024
arxiv_id: "2407.xxxxx"
relevance: high
key_techniques: [tensor_logic, boolean_tensors, syllogism_compilation]
summary: "Shows syllogisms compile directly to tensor equations — Barbara is SP[s,p] = H(SM[s,m] * MP[m,p])."
related: [G0001, REF0003, PT0001]
---
```
