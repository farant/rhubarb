# Hypotheses (HYP) Schema

## Frontmatter Fields

| Field | Required | Type | Description |
|-------|----------|------|-------------|
| id | yes | string | `HYP####` — unique identifier |
| title | yes | string | The claim, stated clearly |
| status | yes | enum | `proposed` / `testing` / `confirmed` / `refuted` |
| evidence_for | no | list | IDs supporting this hypothesis |
| evidence_against | no | list | IDs contradicting this hypothesis |
| testable_by | no | list | IDs of experiments or approaches that could test this |
| source | no | string | Where this hypothesis originated |
| related | no | list | IDs of related records (any type) |

## Body
Full statement of the hypothesis, reasoning behind it, what confirmation/refutation would look like, implications if true.

## Example Frontmatter
```yaml
---
id: HYP0001
title: "Circuit sharing of abstract algebraic patterns is the mechanism behind LLM generalization"
status: proposed
source: "notes/03-transparency-and-wisdom-circuits.md"
evidence_for: [REF0010, P0003]
related: [G0008, UNK0005]
---
```
