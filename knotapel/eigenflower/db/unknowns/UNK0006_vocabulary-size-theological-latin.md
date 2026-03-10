---
id: UNK0006
title: "What the actual vocabulary size requirement is for theological Latin + English"
status: open
source: "notes/01-analysis.md"
related: []
---

The analysis notes that "Latin theological text has a much larger vocabulary" than the grid puzzles TRM handles, but doesn't quantify it. Latin's inflectional morphology means each lemma generates many surface forms (amo, amas, amat, amamus, amatis, amant = 6 forms from one verb, and that's just present active indicative). The Summa uses specialized philosophical/theological vocabulary on top of general Latin. Practical questions: how many unique tokens appear in the combined corpus (Bible + Summa, Latin + English)? How does a byte-level / H-Net tokenizer-free approach change the effective vocabulary size? What fraction of model parameters must be devoted to embeddings vs computation? These numbers directly determine the minimum viable model size.
