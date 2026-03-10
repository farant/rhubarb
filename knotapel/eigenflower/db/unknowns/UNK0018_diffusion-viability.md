---
id: UNK0018
title: "Whether diffusion text generation is viable for this application"
status: open
source: "CLAUDE.md"
related: []
---

The CLAUDE.md mentions "we might explore diffusion text generation" as a possibility. There is a research file (research/mercury-2.md) on this topic. But diffusion models for text are relatively new and less proven than autoregressive approaches. Key unknowns: (1) does diffusion generation work well for structured/logical text where coherence is critical, or is it better suited to creative/descriptive text? (2) can diffusion models be made small enough (50-100M params) and fast enough (70 tok/s) for eigenflower's constraints? (3) does diffusion generation's ability to revise all tokens simultaneously help with logical coherence (plausible) or hurt it (harder to maintain consistent logical structure)? This is more of a "keep an eye on the literature" item than a near-term design decision.
