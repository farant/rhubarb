---
id: UNK0012
title: "How to route domain-specific inputs through the correct wisdom circuit"
status: open
source: "notes/03-transparency-and-wisdom-circuits.md"
related: []
---

The wisdom circuits architecture requires a routing mechanism: given an input, determine which abstract circuit(s) apply. The text says "the routing problem — which wisdom circuit to activate for a given input — is itself a pattern recognition task that probably lives in the fluency layer or the interface" and compares it to "Orchid's data-dependent kernel selection." But this is gesturing at the problem, not solving it. How many wisdom circuits are there? How are they indexed? What happens when multiple circuits apply? What happens when none apply? Is routing learned (trained from examples of correct routing) or specified (hard-coded rules)? The routing mechanism is potentially as complex as the circuits themselves.
