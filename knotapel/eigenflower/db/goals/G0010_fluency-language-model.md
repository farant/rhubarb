---
id: G0010
title: "Build small fluency language model"
status: open
priority: high
depends_on: [G0009, G0003]
blocks: [G0001, G0002, G0011, G0012, G0013, G0015]
related: []
---

## Description

Train a small (~50-100M parameter) language model with ternary weights (BitNet b1.58) on the Bible + Summa corpus. The model handles fluency — parsing, understanding, and generating theological Latin and English text. It does NOT need to reason; reasoning is offloaded to the syllogism layer.

Key architectural choices to resolve:
- Ternary weights {-1, 0, +1} = affirm/deny/ignore (maps naturally to syllogistic logic)
- Possibly recursive (TRM-style) for deeper processing from fewer params
- Free Transformer's latent Z could encode mode (parsing vs generating, Latin vs English)
- H-Net tokenizer-free operation could handle Latin morphology naturally
- Vocabulary size vs embedding layer trade-off (Latin theological text has large vocabulary)

## Why It Matters

The fluency layer is one of the two essential halves of the hybrid architecture. Without it, the syllogism layer has no way to interact with natural language — it can compute logical conclusions but cannot parse input text or generate readable output.

The model does not need to be "smart" — it is a translator between human language and logical structure. This dramatically reduces the required capacity compared to a general-purpose LLM. It needs good LOCAL understanding (parse this sentence correctly) and good generation (express this conclusion clearly), but long-range reasoning is offloaded.

The TRM result (7M params recursive beats billion-param models on reasoning) suggests that even a very small model, applied recursively, can be surprisingly capable — especially when it only needs to handle fluency, not reasoning.

## What "Achieved" Looks Like

- Model trained on full Bible + Summa corpus in under 60 minutes on M2
- Generates coherent Latin text
- Generates coherent English text
- Can parse simple Latin sentences into structured representations
- Runs at 70+ tok/s inference on M2 within 1-2GB RAM
- Ternary weights throughout — integer-only inference
