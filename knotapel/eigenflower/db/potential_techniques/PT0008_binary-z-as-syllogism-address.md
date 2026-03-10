---
id: PT0008
title: "Binary Z as syllogism address space"
status: untested
source: ["Free Transformer paper (Fleuret, 2025)", "Architecture Insight notes"]
applicable_to: []
related: [PT0007]
---

Use the Free Transformer's binary latent variable Z as an explicit address space for compiled syllogisms. With H=16 bits, you get 2^16 = 65,536 possible addresses. Each syllogistic form (Barbara, Celarent, etc.) gets a specific Z value. "Compiling" a new syllogism means training the decoder to produce correct instantiated text when given that Z value + premises.

kappa controls how much information flows through Z. Lower kappa = less structure = more creative output. Higher kappa = more structure = more deterministic/logical output. This is a built-in "reasoning vs creativity" dial.

The parity-lock theorem from DKC (D92) is relevant: encoding IS a binary latent variable that determines the entire accessible function space. D107's constructive phase formula is literally a 3-bit binary mapper (writhe parity + 2 nesting bits -> Z/4Z phase).

At inference: select Z for known syllogism (deterministic), or sample from uniform prior for exploratory reasoning. The encoder is only needed during training and KV cache pre-filling.

This technique bridges the Free Transformer's theoretical framework with eigenflower's concrete goal of addressable compiled knowledge.
