---
id: KIT0016
title: "Precomputed embeddings cache"
status: planned
tier: 3
input: "Bible + Summa corpus, chosen encoder"
output: "Serialized embedding tensors (binary arrays)"
depends_on: []
effort: medium
source: ["MicroDiT paper (Sehwag et al., 2024)"]
migrated_from: PT0042
applicable_to: [G0001, G0010]
related: [KIT0003, KIT0004]
---

## Description

Precompute text embeddings for the entire Bible + Summa corpus once and store as fixed tensors. Training then operates purely on embeddings, never touching raw text. This amortizes encoding cost across all training experiments.

For eigenflower's iterative development process (many training experiments on the same corpus), this is a significant time saver. The embedding cost is paid once regardless of how many experiments run.

## Implementation

1. Choose encoder (byte-level H-Net would make this unnecessary, but traditional tokenizer + embedding layer needs this)
2. Run encoder once over entire corpus
3. Save results as binary arrays (simple format: header with dimensions + raw doubles/floats)
4. Training loop loads precomputed arrays instead of raw text

## API Sketch

```c
nomen structura Incorporatio {
    longus dimensiones;
    longus numerus;
    duplex *data;          /* numerus x dimensiones */
};

integer incorporationes_scribere(constans character *via, constans Incorporatio *inc);
integer incorporationes_legere(constans character *via, Incorporatio *inc);
```

Straightforward I/O. Format should be simple enough to read without dependencies.
