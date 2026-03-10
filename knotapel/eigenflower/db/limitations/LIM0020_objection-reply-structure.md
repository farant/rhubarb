---
id: LIM0020
title: "Objection-reply structure requires representing deliberately wrong syllogisms"
type: discovered
source: "notes/02-architecture-insight.md"
impacts: []
related: [LIM0018, LIM0015]
---

The Summa's format includes objections (videtur quod) that are deliberately WRONG syllogisms — arguments that seem valid but contain errors that Aquinas then identifies and corrects in his replies (ad primum, ad secundum, etc.). The syllogism graph needs to represent these.

From 02-architecture-insight.md: "Where does the Summa's structure of objection-reply fit? The objections are deliberately WRONG syllogisms. Does the graph store them as such? Does it store the error type?"

This is more than a data modeling question — it's a fundamental architectural issue:
- Objections are valid-LOOKING syllogisms. They have the form of real syllogisms but contain specific errors (equivocation, false premise, invalid figure, etc.).
- Aquinas's replies identify the specific error and explain why the objection fails.
- A complete model of the Summa must represent not just valid reasoning but also the taxonomy of errors.

Implications:
- The syllogism graph needs a way to mark edges/nodes as "asserted as objection" vs "asserted as conclusion."
- Error types (equivocation, false premise, etc.) could be metadata on objection nodes.
- The distinguo (distinguishing senses of a term) is the most common way Aquinas resolves objections — this connects to namespace resolution (LIM0018's modes of reasoning).
- This is actually a feature, not just a complication: a system that can explain WHY an argument fails is more useful than one that only produces valid conclusions.
- Phase 1 could start by storing only the respondeo (valid arguments) and add objection-reply structure later.
