---
id: LIM0019
title: "Boolean tensor logic only handles demonstrations, not degrees of certainty"
type: discovered
source: "notes/02-architecture-insight.md"
impacts: []
related: [LIM0018, LIM0013]
---

Aquinas distinguishes between demonstrations (certain), probable arguments, and arguments from authority. A pure Boolean tensor logic (SP[s,p] = H(SM[s,m] * MP[m,p])) only handles the first kind — conclusions that follow with certainty from premises.

From 02-architecture-insight.md: "Can the syllogism layer handle degrees of certainty? Aquinas distinguishes between demonstrations (certain), probable arguments, and arguments from authority. A pure Boolean tensor logic only handles the first."

Theological reasoning frequently involves graded certainty:
- "This is de fide" (of faith, certain)
- "This is theologically certain" (follows necessarily from de fide premises)
- "This is the common opinion" (probable, widely held)
- "This is a pious opinion" (not certain, but edifying)
- Arguments from authority carry weight proportional to the authority's standing

Implications:
- The syllogism layer in its initial tensor-logic form can only handle binary true/false conclusions.
- Extending to graded certainty might require weighted edges in the syllogism graph, or a separate "confidence layer."
- The DKC mechanism works with exact values — introducing probability/confidence means leaving the exact arithmetic world.
- One approach: keep the syllogism layer binary (a conclusion either follows or it doesn't) but annotate the PREMISES with certainty grades from an external source. The graph traversal then inherits the minimum certainty along its path.
- This limitation is acceptable for Phase 1 (demonstrations only) but must be addressed for a complete system.
