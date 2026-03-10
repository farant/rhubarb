---
id: LIM0005
title: "Build from scratch — no off-the-shelf models"
type: chosen
source: "CLAUDE.md"
impacts: []
related: [LIM0006, LIM0007, LIM0011]
---

The project explicitly rejects using pre-trained models, fine-tuning existing models, or building on top of any existing ML framework. Everything is built from scratch in C89.

Why:
- Understanding. The human needs to understand all the code. Taking an off-the-shelf model means inheriting opaque complexity that can't be reasoned about.
- Transparency. The goal (LIM0011) is DKC-level transparency — knowing what every weight does. This is impossible with a model you didn't build.
- Independence. No supply chain risk. No dependency rot. No framework updates breaking things. The same code will work a billion times without wearing out.
- Long-term thinking. The rhubarb project philosophy is building things that last thousands of years. Dependencies are entropy. Own code is stable.

Implications:
- Must implement matrix multiply, backpropagation, optimizer (AdamW), data loading, tokenization, attention mechanisms, etc. from scratch in C89.
- Development is slower initially but produces a fully understood, fully owned system.
- Every architectural choice is deliberate, not inherited from a framework's defaults.
- The "decompile existing models" goal is about understanding mechanisms to build deliberately, not about taking shortcuts.
