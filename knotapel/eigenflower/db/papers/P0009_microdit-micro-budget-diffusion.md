---
id: P0009
title: "Stretching Each Dollar: Diffusion Training from Scratch on a Micro-Budget"
authors: [Vikash Sehwag, Xianghao Kong, Jingtao Li, Michael Spranger, Lingjuan Lyu]
year: 2024
arxiv_id: "2407.15811"
extraction_group: C
relevance: high
key_techniques: [deferred_masking, patch_mixer, mixture_of_experts, layer_wise_scaling, micro_budget_training, two_phase_resolution, classifier_free_guidance]
summary: "Trains competitive text-to-image diffusion from scratch for $1,890 (2.6 days on 8xH100). Key innovation: deferred masking with patch-mixer processes all patches first, then masks 75% for the backbone. 1.16B sparse MoE achieves 12.7 FID on COCO."
related: []
---

## Key Contributions

MicroDiT trains a competitive diffusion model on a micro-budget by introducing "deferred masking": instead of naively dropping input patches (which degrades quality), first process ALL patches through a lightweight "patch-mixer" (4-block transformer, <10% of backbone), THEN mask 75% before the main backbone. Unmasked patches retain global context. Combined with MoE, layer-wise scaling, and synthetic data augmentation.

## Key Techniques

- **Deferred masking**: Small patch-mixer processes ALL patches first, then mask 75% for expensive backbone. At 75% masking: deferred achieves FID 5.03 vs naive masking's 16.5. During inference: no masking, no patch-mixer (training-only overhead).
- **Masking as training cost reduction**: 75% masking gives 4x training speedup independent of model size. Masking > downscaling for equivalent compute budgets.
- **Patch-mixer**: 4 transformer blocks with residual connections, <10% backbone parameters.
- **Mixture-of-Experts**: 8-expert MoE with expert-choice routing in alternating blocks. Increases capacity without proportional FLOP increase.
- **Layer-wise scaling**: Linearly increase hidden dimension from early to later layers. Deeper layers = more complex features = more parameters.
- **Two-phase training**: Phase 1: 256x256 with 75% masking + unmasked finetuning. Phase 2: 512x512 same approach.
- **Classifier-free guidance**: 10% caption dropout during training. At inference: F_hat = F_uncond + w*(F_cond - F_uncond).
- **Precomputed embeddings**: Text+image embeddings computed once, amortized over training runs.

## Specific Relevance to Eigenflower

- **Diffusion text generation**: Entire pipeline adaptable for text. Patches -> tokens. Image VAE -> text autoencoder. Deferred masking -> token masking (exactly BERT-style). The patch-mixer insight transfers: process all tokens through lightweight mixer first, then mask.
- **Training cost**: 3.45e20 FLOPs for 1.16B model. Eigenflower at ~200M, ~5M samples = ~8e18 FLOPs. With masking + low precision on M2: potentially hours range. At 50-100M params: 30-60 minute sweet spot.
- **Synthetic data**: 15M synthetic + 22M real images. Could augment Bible/Summa with synthetic theological text.
- **Precomputed embeddings**: Compute Bible embeddings once, store, never touch raw text during training.

## Key Results

- 12.7 FID on COCO (competitive with SD 1.5 at 11.18 FID, which cost 4800 A100-days)
- Total cost: $1,890 (2.6 days on 8xH100)
- Deferred masking at 75%: FID 5.03 vs naive masking 16.5 (CIFAR ablation)
- 4-channel VAE outperforms 16-channel for micro-budget (lower latent dim better with limited budget)
- SwiGLU > GELU for feedforward layers
- Random patch masking > block masking

## Key Equations

- Diffusion loss: L = E[||F_theta(x + noise) - x||^2]
- Noise schedule: ln(sigma) ~ N(P_mean, P_std), tuned to (-0.6, 1.2)
- CFG: F_hat = F_unconditional + w * (F_conditional - F_unconditional)

## Implementation Notes

- 1.16B sparse model (MicroDiT), DiT-Xl/2 backbone with 8-expert MoE
- Patch size: 2 (256 patches per 256x256 image)
- Training: AdamW, cosine LR, weight decay 0.01, EMA 0.995 (last 10K steps)
- Phase 1: 250K masked + 30K unmasked steps (256x256)
- Phase 2: 50K masked + 5K unmasked steps (512x512)
- Dataset: 37M images (22M real + 15M synthetic), all open-source
- Code: github.com/SonyResearch/micro_diffusion
- VAE: 4-channel from SDXL (pretrained). Text encoder: CLIP ViT-H/14 (pretrained).

## Feasibility Estimates for Eigenflower

- 50-100M parameter model is the sweet spot for 30-60 min training on M2 with masking + low precision
- M2 at ~3.6 TFLOPS FP16: 200M model = ~25 days at 100%. With masking (4x) + low precision (2x) = ~3 days. At 50M params: hours.
