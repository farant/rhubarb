---
id: REF0035
title: "DKC Demo 10 — Numerical Engine (50x Compression Pivot)"
type: dkc_demo
source_project: knotapel
demo_number: 10
summary: "The numerical pivot: Poly (808 bytes/entry) replaced by Cx (16 bytes/entry), a 50x compression. Expensive computation operates on the compressed representation. This transition from symbolic to numerical enabled the scale needed for DKC discovery."
related: []
---

## Description

Demo 10 transitioned from symbolic Laurent polynomial computation (Poly type, 808 bytes per entry) to numerical complex number evaluation (Cx type, 16 bytes per entry) -- a 50x compression. All subsequent demos through D28 used this numerical engine, and the compression enabled the scale of computation necessary for the forward DKC discovery (D29).

## Relevance to Eigenflower

- A concrete instance of H-Net's hierarchical compression: the expensive computation operates on the compressed representation
- The compression pipeline (braid word -> TL algebra -> bracket value -> Boolean function) is a natural hierarchical chunking sequence
- The 50x compression ratio demonstrates that algebraic structure enables dramatic representation efficiency
- Parallels CALM's representational compression (K tokens -> one vector)
