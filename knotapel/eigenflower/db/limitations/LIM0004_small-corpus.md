---
id: LIM0004
title: "Small corpus: Bible (multilingual) + Summa Theologica"
type: chosen
source: "CLAUDE.md"
impacts: []
related: [LIM0001, LIM0017, LIM0021]
---

The training corpus is deliberately restricted to:
- Douay-Rheims Bible (English, ~5MB)
- Greek Septuagint (~4.5MB)
- Latin Vulgate (~4.4MB)
- Summa Theologica (Latin and English, multiple parts)

Total corpus is roughly 20-25MB of text. This is tiny by modern LLM standards (GPT-3 trained on ~570GB).

Why this is chosen, not just accepted:
- The Edge of Chaos paper's key insight: data COMPLEXITY matters more than data QUANTITY. The Bible and Summa are among the most informationally dense texts in existence — millennia of editorial refinement, highly structured argumentation, rich cross-referencing.
- The Inverse Knowledge Search paper notes the Summa is already an "uncompressed reasoning corpus" — it doesn't hide its derivations like most text does.
- The 1/8 data efficiency result from the Early Experience paper suggests small high-quality corpora can be competitive.

Implications:
- Vocabulary coverage is bounded by these texts. The model won't know words outside theological/biblical/philosophical Latin and English.
- The model is a specialist, not a generalist. It reasons about theology, not about weather or sports.
- Multilingual alignment (Latin/English/Greek) provides implicit structural signal — the same content in three languages gives the model three views of the same underlying meaning.
- The Summa's structured format (videtur quod, sed contra, respondeo) provides rich parsing signal that partially compensates for small corpus size.
