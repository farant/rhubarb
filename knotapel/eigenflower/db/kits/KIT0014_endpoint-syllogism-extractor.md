---
id: KIT0014
title: "Endpoint-driven syllogism extractor"
status: planned
tier: 3
input: "Summa article (parsed by KIT0013), endpoint/conclusion"
output: "Proposed syllogistic structure (major, minor, conclusion, figure, mood)"
depends_on: [KIT0013, KIT0012]
effort: medium
source: ["Inverse Knowledge Search paper (Li et al., 2026)"]
migrated_from: PT0033
applicable_to: [G0005, G0012]
related: [KIT0015]
---

## Description

Instead of forward-reasoning from axioms, start from known endpoints (theological conclusions in the Summa) and derive backward to premises. Each Article's conclusion is an endpoint. The respondeo provides the derivation chain. Extract the syllogistic structure by working backward from conclusion to premises.

Each endpoint can be derived from multiple abstraction levels (from natural reason, from Scripture, from the Fathers), creating richer weight structures.

The syllogism dependency graph emerges naturally: which conclusions serve as premises for other syllogisms. No need to pre-define the graph -- it emerges from the logical structure.

## Process

1. KIT0013 parses article into sections
2. Identify conclusion from respondeo
3. Work backward: what premises support this conclusion?
4. Match against syllogistic figures/moods
5. KIT0012 validates the extracted form
6. Optionally: KIT0015 cross-validates with external models

This KIT handles step 2-4. Steps 3-4 may require fluency layer assistance for natural language understanding, making this a quasi-KIT -- the procedure is deterministic but quality depends on the NLP component.
