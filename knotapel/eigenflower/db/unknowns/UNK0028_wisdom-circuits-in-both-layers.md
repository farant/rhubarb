---
id: UNK0028
title: "How wisdom circuits that span both fluency and syllogism layers should be implemented"
status: open
source: "notes/03-transparency-and-wisdom-circuits.md"
related: []
---

The transparency document identifies the "most interesting case" as patterns requiring BOTH fluency and logic to express — specifically analogy, where "the fluency layer parses the four terms, the syllogism layer encodes the proportionality relation A:B::C:D, and the wisdom circuit IS the proportionality structure itself, which spans both layers." But spanning two architecturally distinct layers (one trained ANN, one compiled tensor logic graph) is architecturally challenging. How does a single "circuit" span a statistical and an exact layer? What data structures cross the boundary? How is the circuit's behavior verified when half of it is approximate? This may require a third architectural component — neither pure fluency nor pure logic — that mediates between them.
