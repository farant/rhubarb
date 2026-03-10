---
id: UNK0020
title: "What the right embedding approach is for Latin morphology"
status: open
source: "notes/01-analysis.md"
related: []
---

Latin has rich inflectional morphology (nouns decline in 5 declensions x 6 cases x 2 numbers = 60 forms; verbs conjugate in 4 conjugations x 6 tenses x 3 moods x 2 voices x 6 persons = hundreds of forms). Standard tokenizers (BPE, SentencePiece) handle this poorly because they split words at arbitrary subword boundaries that don't align with morphological structure. H-Net's tokenizer-free (byte-level) approach eliminates the tokenizer problem entirely but may not efficiently encode the morphological regularity. A morphologically-aware approach (stem + inflection tokens) could be more parameter-efficient for Latin but requires linguistic knowledge baked into the tokenizer. Which approach is best for a small model on a Latin-heavy corpus is genuinely unclear.
