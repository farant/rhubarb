---
id: PT0033
title: "Endpoint-driven reductionist syllogism extraction"
status: promoted_to_kit
promoted_to: KIT0014
source: ["Inverse Knowledge Search paper (Li et al., 2026)"]
applicable_to: []
related: [PT0032, PT0034]
---

Instead of forward-reasoning from axioms (hard to guarantee completeness), start from known endpoints (theological conclusions in the Summa) and derive backward to premises. The "endpoint" is the conclusion of a syllogism; the derivation IS the syllogism.

Each endpoint can be derived from multiple abstraction levels (from natural reason, from Scripture, from the Fathers), creating richer weight structures.

For eigenflower: systematically walk through Summa articles. Each Article's conclusion is an endpoint. The respondeo provides the derivation chain. Extract the syllogistic structure by working backward from conclusion to premises.

Cross-validate extractions using multiple models (Claude, Gemini, local) -- only keep extractions where multiple models agree on the logical structure. Pre-filter auto-generated questions for those with verifiable logical structure.

The syllogism dependency graph emerges naturally: which conclusions serve as premises for other syllogisms. No need to pre-define the graph -- it emerges from the logical structure. MODBP clustering on this graph would reveal natural hierarchical structure of theological knowledge.
