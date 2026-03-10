---
id: UNK0011
title: "Whether the fluency layer needs retraining when new terms are introduced"
status: open
source: "notes/02-architecture-insight.md"
related: []
---

The architecture claims the fluency layer "only needs retraining if the VOCABULARY changes (new terms it hasn't seen before), not when new logical relationships are added." But how often does vocabulary change in practice? If a user wants to add syllogisms about new concepts not in the Bible or Summa, the fluency layer encounters unknown terms. With a byte-level / H-Net tokenizer-free approach, the model can always REPRESENT new terms (any string of bytes), but can it UNDERSTAND them well enough to parse and generate correctly? This is the difference between having a character encoding for "quidditas" and knowing that "quidditas" means "essence/whatness" in Thomistic philosophy. The boundary between vocabulary-change-requiring-retraining and logical-change-not-requiring-retraining may be blurrier than assumed.
