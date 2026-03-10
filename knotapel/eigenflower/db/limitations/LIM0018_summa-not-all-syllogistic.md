---
id: LIM0018
title: "Only ~30% of Summa reasoning may be strictly syllogistic"
type: discovered
source: "notes/02-architecture-insight.md"
impacts: []
related: [LIM0015, LIM0019, LIM0020]
---

A significant open question from the architecture insight: how much of the Summa's reasoning is actually syllogistic vs analogical, authoritative, or other modes? If only ~30% is formal syllogisms, the syllogism graph is sparse and the fluency layer is doing heavy lifting for the other 70%.

From 02-architecture-insight.md: "How much of the Summa's reasoning is actually syllogistic vs analogical/authoritative/other? If only 30% is formal syllogisms, the graph is sparse and the fluency layer is doing a lot of heavy lifting for the other 70%."

Aquinas uses multiple reasoning modes extensively (documented in 03-transparency-and-wisdom-circuits.md):
- Demonstration (syllogistic) — the clear case
- Analogy — proportionality reasoning (A:B::C:D)
- Argument from authority — weighted citation graph
- Reductio ad absurdum — backward chaining with negation
- Distinguo — namespace resolution / disambiguation

Implications:
- The syllogism layer needs to be extended beyond pure syllogistic reasoning eventually.
- In the near term, even a sparse graph is valuable — syllogistic reasoning is the highest-certainty mode and the most verifiable.
- The 70% non-syllogistic content still provides training data for the fluency layer.
- The "wisdom circuits" framework (note 03) proposes that each reasoning mode gets its own transparent architectural component, not just one monolithic syllogism engine.
- This limitation argues for an incremental approach: get syllogistic reasoning working first (it's the clearest case), then extend to other modes.
