---
id: P0006
title: "The Free Transformer"
authors: [Francois Fleuret]
year: 2025
arxiv_id: "2510.17558"
extraction_group: B
relevance: high
key_techniques: [conditional_vae, binary_latent_variables, split_decoder, learned_constant_query, free_bits, information_bottleneck]
summary: "Extends decoder-only Transformers by conditioning on learned discrete latent variables Z (2^16 possible values per position). Only 3% overhead. Massive improvements on code/math: +44% HumanEval, +28% MBPP at 1.5B scale."
related: []
---

## Key Contributions

The Free Transformer conditions autoregressive generation on learned latent random variables Z. During generation, Z is sampled from a uniform prior over 2^H values (H=16 = 65,536 possible values). During training, Z is inferred by an encoder (one non-causal Transformer block sharing computation with the decoder). The key insight: standard autoregressive models waste capacity re-inferring latent structure (topic, sentiment, logical mode) from the token stream. Making these decisions explicit and up-front dramatically improves reasoning tasks.

## Key Techniques

- **Latent variables condition generation**: Sample Z once, then generate conditioned on Z. The model decides global properties before generating tokens.
- **Split decoder architecture**: First L/2 layers process tokens causally as normal. Z injected at midpoint via keys/values. Second L/2 layers condition on Z's influence.
- **Learned constant query zeta**: Encoder uses a single trained embedding (replicated across positions) as queries, decoder activations as keys/values. Prevents token copying, forces extraction of GLOBAL properties.
- **Binary mapper**: H independent Bernoulli bits -> one-hot vector of dim 2^H. Gradient pass-through via sigmoid monotonicity. No Gumbel-Softmax needed.
- **Free bits**: KL per position thresholded at kappa. Prevents common VAE collapse where Z becomes meaningless. kappa controls information rate (1/64 to 8 bits per token).
- **Minimal overhead**: ~3.1-3.6% more compute and memory. One extra non-causal block + two linear layers.

## Specific Relevance to Eigenflower

- **Syllogism compilation via Z**: Z could explicitly represent the LOGICAL MODE -- syllogistic form, middle term, whether this is premise/conclusion. Compiling a syllogism means finding the Z that produces it. 2^16 = 65k possible "logical modes."
- **Binary Z as syllogism address**: A syllogism library could assign specific Z values to specific forms (Barbara, Celarent, Darii, Ferio, etc.).
- **kappa as reasoning vs creativity dial**: Lower kappa = more creative. Higher kappa = more deterministic/logical. Built-in control.
- **Decompile existing models**: The encoder extracts latent structure from text. If trained on output of existing model, reveals what latent decisions the model makes implicitly.
- **Coin-flip example**: Motivating example perfectly parallels syllogistic reasoning -- once you know the logical mode (Z), the tokens follow simply.

## Key Results

- At 1.5B: HumanEval +44%, MBPP +28%, GSM8K +12% over baseline
- At 8B with 1T tokens: MBPP +6%, HumanEval +11%
- Only 3% overhead for these gains
- Sweet spot: 1/2 to 1 bit per token information rate

## Key Equations

- P(S) = integral P(S|Z=z)P(Z=z)dz (conditional VAE)
- KL clipping: L_KL = sum max(D_KL(Q(Z_t|S) || P(Z_t)), kappa)
- Binary mapper: Y_{t,d} + G_{t,d} - detach(G_{t,d}) (straight-through)
- kappa = log(2)/k for k bits per token

## Implementation Notes

- Models tested: 1.5B (28 layers, d=1536) and 8B (32 layers, d=4096), Llama-3-style
- H = 16 in all experiments (2^16 = 65,536 latent values per position)
- Training: 32 H100s for 12 hours (1.5B), 256 H100s for 24 hours (8B)
- Encoder evaluated only during training and KV cache pre-filling. During generation, only decoder runs.
- Validated sweet spot: kappa = log(2)/2 for 1/2 bit per token
- At 4 bits (kappa = 4*log(2)), encoder encodes ENTIRE sequence in Z -- downstream collapses
