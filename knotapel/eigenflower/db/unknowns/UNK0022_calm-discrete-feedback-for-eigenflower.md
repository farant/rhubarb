---
id: UNK0022
title: "How CALM's discrete feedback mechanism applies to eigenflower's exact arithmetic requirement"
status: open
source: "notes/01-analysis.md"
related: []
---

The crossover analysis notes that Connection 8 (CALM discrete feedback <-> exact arithmetic requirement) is "more insightful than it looks." Both CALM and DKC discover that you must "ground" back to the discrete/exact representation for rigorous computation — continuous is good for exploration, discrete is necessary for correctness. But it is unclear how this insight translates to eigenflower's architecture. Does the fluency layer (continuous/approximate) periodically "ground" its representations through the syllogism layer (discrete/exact)? How often? At what granularity? Is this a per-sentence operation, per-argument, or per-article? The frequency and mechanism of grounding affects both correctness and performance.
