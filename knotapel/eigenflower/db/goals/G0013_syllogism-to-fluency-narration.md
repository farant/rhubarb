---
id: G0013
title: "Build syllogism-to-fluency narration interface"
status: open
priority: high
depends_on: [G0007, G0010]
blocks: [G0014]
related: []
---

## Description

Design and implement the protocol by which the syllogism layer's logical outputs are narrated back into natural language by the fluency layer. This is the "channeling out" mechanism — how abstract logical conclusions get concretized into readable text.

The syllogism layer produces logical states:
- "From premises P1 and P2, conclusion C follows by Barbara"
- "Query Q has no derivation in the compiled knowledge"
- "Premises P1 and P3 are contradictory"

The fluency layer must narrate these into coherent Latin or English text. This is closer to template-constrained generation than free generation — the logical structure constrains WHAT needs to be said, the fluency layer handles HOW to say it.

## Why It Matters

The DKC insight applies here via the parity-lock theorem (D92) and activation capability (D50): the readout mechanism controls what the circuit can express. If the narration interface bottlenecks the syllogism layer's output, the system's effective reasoning capability is reduced even if the syllogism layer is correct internally.

The narration problem is also where the system's transparency becomes visible to the user. If the narration accurately reflects the derivation chain (citing each step, naming the syllogistic form used), the user can verify the reasoning. If it summarizes or paraphrases, transparency is lost.

## What "Achieved" Looks Like

- A defined protocol for syllogism -> fluency communication
- Derivation chains are narrated as step-by-step reasoning in natural language
- Each step cites the syllogistic form used and the premises involved
- "No derivation found" is expressed clearly, not confabulated around
- Contradictions are reported accurately
- Output is coherent in both Latin and English
- The narration preserves the logical structure (a reader could reconstruct the formal derivation from the text)
