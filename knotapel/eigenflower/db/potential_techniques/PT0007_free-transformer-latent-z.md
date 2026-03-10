---
id: PT0007
title: "Free Transformer latent Z for syllogistic mode selection"
status: untested
source: ["Free Transformer paper (Fleuret, 2025)"]
applicable_to: []
related: [PT0006, PT0008]
---

Extend a decoder-only Transformer by conditioning on learned latent random variables Z. Z is sampled from a uniform prior over 2^H values (H=16, so 65,536 possible values). During training, Z is inferred by an encoder (a single non-causal Transformer block). The binary mapper trick encodes H independent Bernoulli bits as a one-hot vector.

Key insight: standard autoregressive models waste capacity re-inferring latent structure (topic, sentiment, logical mode) from the token stream. The Free Transformer makes these decisions explicit and up-front. Only ~3% compute/memory overhead.

For eigenflower: Z could explicitly represent the LOGICAL MODE -- what kind of syllogism, what the middle term is, whether this is premise/conclusion. With H=16 you have 65k possible "logical modes." A syllogism library could assign specific Z values to specific syllogistic forms (Barbara, Celarent, Darii, Ferio, etc.). Compiling a new syllogism means training the decoder to produce it when given that Z.

Split decoder: first L/2 layers process tokens normally, then Z is injected at the midpoint. Encoder uses a LEARNED CONSTANT QUERY (prevents copying individual tokens, forces extraction of global properties). kappa controls information rate (sweet spot: 1/2 to 1 bit per token).

Massive improvements on code/math tasks: +44% HumanEval, +28% MBPP at 1.5B params. These are planning/reasoning tasks where up-front latent decisions help most.

Caveat: tested only at 1.5B+ scale with 47B+ tokens. May not work with very small models or corpora. Training "often unstable" per conclusion.
