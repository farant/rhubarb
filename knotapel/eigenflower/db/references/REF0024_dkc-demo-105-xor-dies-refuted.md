---
id: REF0024
title: "DKC Demo 105 — 'XOR Dies at N>=7' REFUTED"
type: dkc_demo
source_project: knotapel
demo_number: 105
summary: "'XOR dies at N>=7' was a k-regime artifact, not a fundamental limit. W_{8,0} achieves XOR8=22266 at k=4096. The macrame principle: branching interaction reverses between cross-block and within-block regimes."
related: []
---

## Description

Demo 105 refuted the earlier belief that XOR computation dies at N>=7 inputs. The limitation was a k-regime artifact -- using insufficient activation resolution (low k) created an artificial ceiling. At k=4096, W_{8,0} achieves XOR8=22266 solutions. The macrame principle explains that branching interaction between module blocks reverses behavior between cross-block (dim 5+4) and within-block (dim 14+14) regimes.

## Relevance to Eigenflower

- Demonstrates the importance of activation resolution for unlocking computational capacity -- parallels the finding that sector count k is a fundamental scaling axis
- The k/2^N ratio controls XOR viability (k-regime theory) -- relevant to activation function design in DKC-inspired architectures
- Shows that apparent fundamental limits can be k-regime artifacts -- caution against premature conclusions about model capacity
