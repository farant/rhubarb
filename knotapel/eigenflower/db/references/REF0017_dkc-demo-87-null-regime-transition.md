---
id: REF0017
title: "DKC Demo 87 — Null Dispensability Regime Transition"
type: dkc_demo
source_project: knotapel
demo_number: 87
summary: "Null entries are critical at finite groups (z8) but dispensable at infinite groups (z12). Regime transition: zeta_4=75% null, zeta_8=37.5%, zeta_12=3%. The 'right' amount of data depends on the algebraic context."
related: []
---

## Description

Demo 87 tested null indispensability at zeta_12 (infinite group) and discovered a regime transition. At finite groups (z8), null entries are indispensable -- removing them collapses capacity. At infinite groups (z12), nulls are redundant because the infinite catalog provides sufficient directional coverage without them. The null fraction decreases monotonically: zeta_4=75%, zeta_8=37.5%, zeta_12=3%.

## Relevance to Eigenflower

- Predicts that BitNet's zero-weight fraction should decrease as model size increases -- matching the DKC trend
- Demonstrates that data efficiency is regime-dependent: structured environments need less data than unconstrained ones
- Parallels Paper 2's (Early Experience) 1/8 data efficiency result: less data with right algebraic structure beats more data without
- Relevant to Eigenflower's corpus sizing: a small but algebraically rich corpus (Bible + Summa) may suffice if it has the right structural properties
