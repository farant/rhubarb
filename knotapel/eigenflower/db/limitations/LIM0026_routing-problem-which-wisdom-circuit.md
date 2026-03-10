---
id: LIM0026
title: "The routing problem — which wisdom circuit to activate for a given input"
type: discovered
source: "notes/03-transparency-and-wisdom-circuits.md"
impacts: []
related: [LIM0025, LIM0027]
---

If the architecture has multiple wisdom circuits (syllogistic, analogical, authoritative, reductio, distinguo), the system needs to determine which circuit(s) to activate for a given input. This routing problem is itself a pattern recognition task.

From 03-transparency-and-wisdom-circuits.md: "The routing problem — which wisdom circuit to activate for a given input — is itself a pattern recognition task that probably lives in the fluency layer or the interface."

This is analogous to Orchid's data-dependent kernel selection (the input determines which processing circuit to use) and to the Free Transformer's latent Z variable (which conditions generation on a logical mode).

The problem has layers:
- Detecting that reasoning is needed at all (vs simple factual recall or generation)
- Identifying which mode of reasoning is appropriate (deduction, analogy, authority, reductio, distinguo)
- Formulating the correct input for that circuit (e.g., identifying the four terms in an analogy)
- Potentially combining multiple circuits for complex arguments that use multiple reasoning modes

Implications:
- The routing itself might need to be learned (part of the fluency layer's training) or could be partially rule-based (the Summa's structural markers indicate reasoning mode).
- Misrouting — sending a deductive query to the analogical circuit or vice versa — produces wrong answers. The routing must be reliable.
- This is an area where the fluency layer and syllogism layer are most tightly coupled, making the interface design (LIM0027) critical.
- The Summa's structured format is a huge advantage: "Videtur quod" signals objection-mode, "Respondeo" signals conclusion-mode, etc.
