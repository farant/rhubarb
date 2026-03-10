---
id: PT0053
title: "Recursive nesting of H-Nets for linguistic hierarchy"
status: untested
source: ["H-Net paper (Hwang, Wang, Gu, 2025)"]
applicable_to: []
related: [PT0005, PT0052]
---

The main network in H-Net can itself be another H-Net, creating arbitrary-depth hierarchies: characters -> morphemes -> words -> clauses -> sentences. A 2-stage H-Net achieves better performance than 1-stage at comparable compute.

The first stage of a 2-stage H-Net discovers character+space boundaries. The second stage groups multi-word phrases like "the backbone." This emergent behavior validates the approach without linguistic priors.

For eigenflower: Latin morphology is rich (declensions, conjugations). A multi-stage H-Net could naturally discover Latin morphological boundaries at stage 1, then word/phrase boundaries at stage 2, then clause/sentence boundaries at stage 3. This mirrors natural linguistic hierarchy and could model syllogistic structure at different abstraction levels.

The nesting could also model premise-level vs argument-level structure: stage 1 handles words, stage 2 handles propositions, stage 3 handles syllogisms.
