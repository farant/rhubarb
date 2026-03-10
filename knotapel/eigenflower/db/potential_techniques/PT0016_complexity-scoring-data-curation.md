---
id: PT0016
title: "Complexity scoring for training data curation"
status: promoted_to_kit
promoted_to: KIT0002
source: ["Intelligence at the Edge of Chaos paper (Zhang et al., ICLR 2025)"]
applicable_to: []
related: [PT0015]
---

Use information-theoretic complexity measures to score and curate training data, selecting passages at the optimal complexity level for representation learning. Measures include:

- Lempel-Ziv complexity (string scanning algorithm, simple to implement)
- Compression complexity (compression ratio)
- Lyapunov exponent (sensitivity to initial conditions)
- Krylov complexity (operator growth in Hilbert space)

All four correlate with downstream performance (r=0.44 to r=0.73 depending on task).

For eigenflower: score passages from Bible and Summa. Select the most information-rich sections for training. The Summa's syllogistic articles likely have different complexity profiles than narrative passages in the Bible. Curating by complexity could maximize the value of the small corpus.

Implementation: Lempel-Ziv is a simple string scanning algorithm, trivially implementable in C89. Compression complexity via deflate/zlib is also straightforward.

The temporal structure of data matters -- randomly shuffling temporal order degrades performance. Preserve sequential/argumentative structure when preparing training data.
