---
id: KIT0002
title: "Lempel-Ziv complexity scorer"
status: planned
tier: 0
input: "String/byte sequence"
output: "Complexity score (integer)"
depends_on: []
effort: small
source: ["Intelligence at the Edge of Chaos paper (Zhang et al., ICLR 2025)"]
migrated_from: PT0016
applicable_to: [G0003]
related: [KIT0011]
---

## Description

Lempel-Ziv complexity measures the number of distinct subpatterns in a sequence. It is a simple string scanning algorithm: walk the string left to right, count how many times you encounter a substring not seen before. Higher count = more complex = more information-dense.

For eigenflower: score passages from Bible and Summa to curate training data. Select the most information-rich sections. Summa's syllogistic articles likely have different complexity profiles than narrative passages in the Bible.

Can be combined with compression complexity (deflate ratio) for a two-metric scoring system. Both correlate with downstream model performance (r=0.44 to r=0.73 depending on task).

## Algorithm

1. Initialize vocabulary V = {}, complexity C = 0
2. Scan string left to right, extending current substring w
3. When w is not in V: add w to V, increment C, reset w
4. Return C

~150 LOC. Standard algorithm, no dependencies.
