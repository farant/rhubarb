---
id: UNK0016
title: "How to handle Aquinas's 'distinguo' mode — namespace resolution for polysemous terms"
status: open
source: "notes/03-transparency-and-wisdom-circuits.md"
related: []
---

Aquinas's "distinguo" (I distinguish) is a reasoning mode where a term is shown to have multiple senses, and the argument's validity depends on which sense is intended. The architecture describes this as "namespace resolution — the same word maps to different proposition nodes depending on sense." But implementing this is nontrivial. How does the system know that a term is polysemous? How are the senses enumerated and stored? When a new argument uses a polysemous term, how does the system determine which sense is intended? This is essentially the word sense disambiguation problem, which is hard even in modern NLP. In the hybrid architecture, is this the fluency layer's job (linguistic context determines sense) or the syllogism layer's job (logical context determines sense) or both?
