---
id: UNK0008
title: "What the syllogism-to-fluency narration protocol should look like"
status: open
source: "notes/02-architecture-insight.md"
related: []
---

The syllogism layer produces logical states: derivation chains, "no derivation found," contradictions. The fluency layer must narrate these into coherent natural language text. The architecture suggests this is "closer to template filling than free generation," but this is an assumption that hasn't been tested. Thomistic arguments can be complex multi-step chains involving distinctions, analogies, and cross-references to other questions. Whether template filling can handle this complexity, or whether something closer to structured generation (conditioned on the logical derivation) is needed, is unknown. The narration protocol also needs to handle the case where the logical conclusion is correct but the natural language expression is awkward or misleading.
