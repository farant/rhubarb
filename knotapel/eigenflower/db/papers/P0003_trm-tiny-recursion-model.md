---
id: P0003
title: "Less is More: Recursive Reasoning with Tiny Networks"
authors: [Alexia Jolicoeur-Martineau]
year: 2025
arxiv_id: "2510.04871"
extraction_group: A
relevance: high
key_techniques: [recursive_refinement, deep_supervision, tiny_network, ema_weights, latent_chain_of_thought, adaptive_halting]
summary: "A single 2-layer 7M-parameter network recursively refines answers via latent chain-of-thought. 42 effective layers via deep supervision. Beats billion-parameter LLMs on ARC-AGI reasoning with 4x fewer parameters than its predecessor HRM."
related: []
---

## Key Contributions

TRM (Tiny Recursion Model) reasons by recursively refining its answer with a single tiny network (2 layers, ~5-7M parameters). Given input x, maintains two features: proposed solution y and latent reasoning feature z. Recursively updates z given (x, y, z), then updates y given (y, z). Deep supervision enables 42 effective layers without backpropagating through all of them.

## Key Techniques

- **Recursive refinement with one tiny network**: A single 2-layer transformer applied repeatedly. Each step: z = net(x, y, z) then y = net(y, z). The entire algorithm.
- **Deep supervision = virtual depth**: By reusing latent features across supervision steps (without gradients), effective depth = T*(n+1)*n_layers. With T=3, n=6, 2 layers: 42 effective layers.
- **Two features (y and z)**: y = current answer (decodable), z = latent reasoning state (chain-of-thought in latent space). Model needs BOTH -- y remembers the solution, z remembers how it got there.
- **Less is more**: 2 layers beats 4 layers. Single network beats two separate networks. Increasing capacity DECREASES generalization with scarce data.
- **EMA for stability**: Exponential Moving Average (decay=0.999) prevents sharp collapse. Improves 79.9% to 87.4%.
- **Attention-free variant**: For fixed-size tasks, MLP replaces self-attention and works better (87.4% vs 74.7% on Sudoku).

## Specific Relevance to Eigenflower

- **Tiny model, huge reasoning**: 7M params at FP16 = ~14MB. At 1.58 bits (BitNet) = ~1.4MB. 16 forward passes through 2 layers = trivially fast on CPU.
- **Small-sample learning**: Designed for ~1000 examples (Sudoku training set). Bible+Summa is larger than TRM's training sets.
- **Recursive refinement IS syllogistic reasoning**: Each recursion applies the same logical rules to refine the answer. A syllogism compiled as a weight pattern would, when recursively applied, perform multi-step inference. z accumulates the logical state.
- **Train in 30-60 min on M2**: 7M parameters, plausibly under 60 minutes for small corpus.

## Key Results

- 7M params achieves 45% on ARC-AGI-1, beating Deepseek R1 (671B, 15.8%), Claude 3.7 (28.6%), o3-mini (34.5%)
- 87.4% on Sudoku-Extreme with 2 layers + n=6 recursions
- 4 layers with n=3 recursions (same compute): only 79.5%
- Single network (87.4%) beats two separate networks (82.4%)
- MoE HURTS, weight tying HURTS, Deep Equilibrium Models HURT

## Key Equations

- Forward: z_{t+1} = f(x, y_t, z_t); y_{t+1} = g(y_t, z_t)
- Effective depth: D_eff = T * (n + 1) * n_layers
- Optimal config: T=3, n=6, 2 layers (D_eff = 42)

## Implementation Notes

- Architecture: 2-layer transformer, hidden dim 512, RMSNorm, SwiGLU, rotary embeddings
- Training: AdamW, lr=1e-4, batch 768, N_sup=16 supervision steps, EMA=0.999
- Parameters: 5M (MLP variant) to 7M (attention variant)
- Hardware: Single L40S for Sudoku (<36 hours), 4xH100 for ARC-AGI (~3 days)
- Supervised learning, not generative -- extending to generation is future work
- Heavy data augmentation critical: 1000 shuffles per Sudoku, 8 dihedral for Maze
