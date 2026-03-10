---
id: P0014
title: "Agent Learning via Early Experience"
authors: [Kai Zhang, Xiangchao Chen, Bo Liu, Tianci Xue, Zeyi Liao, Zhihan Liu, Xiyao Wang, Yuting Ning, Zhaorun Chen, Xiaohan Fu, Jian Xie, Yuxuan Sun, Boyu Gou, Qi Qi, Zihang Meng, Jianwei Yang, Ning Zhang, Xian Li, Ashish Shah, Dat Huynh, Hengduo Li, Zi Yang, Sara Cao, Lawrence Jang, Shuyan Zhou, Jiacheng Zhu, Huan Sun, Jason Weston, Yu Su, Yifan Wu]
year: 2025
arxiv_id: "2510.08558"
extraction_group: E
relevance: high
key_techniques: [implicit_world_modeling, self_reflection, reward_free_learning, early_experience_paradigm, contrastive_alternatives, two_stage_training, data_amplification]
summary: "Introduces 'early experience' as a middle ground between imitation learning and RL. Two methods: Implicit World Modeling (predict next-states from alternatives) and Self-Reflection (explain WHY expert action is better). Works at 3B scale. 1/8 expert data matches full imitation learning."
related: []
---

## Key Contributions

The "early experience" paradigm: an agent's own exploratory actions and their resulting environment states serve as supervision WITHOUT any reward function. Two methods: (1) IWM (Implicit World Modeling) -- predict next-states from (state, action) pairs including agent's own alternatives; (2) SR (Self-Reflection) -- generate chain-of-thought explanations of WHY expert action beats alternatives, grounded in observed consequences. Both consistently outperform pure imitation learning across 8 environments and 3 model sizes.

## Key Techniques

- **Implicit World Modeling (IWM)**: From expert trajectories, at each state propose K alternative actions, execute them, train to predict resulting next-states. Standard next-token prediction loss on state-transition data. Two-stage: world-model pre-training (1 epoch), then supervised fine-tuning on expert data. Same total compute as imitation learning.
- **Self-Reflection (SR)**: For each expert state, propose alternatives, execute them. LLM generates chain-of-thought WHY expert action was better, grounded in actual observed transitions. Train on (state, explanation, correct_action) triples.
- **Data amplification**: K alternative actions per expert state multiply training data by ~K. 21K expert pairs x 9 alternatives = 189K IWM triples.
- **Two-stage pipeline**: First world-model pre-training, then supervised fine-tuning. Gives model transition dynamics before specific content.
- **Reward-free bootstraps RL**: Early-experience warm-started models reach HIGHER RL ceilings. Gap sometimes GROWS during RL training.
- **1/8 data efficiency**: On WebShop, early experience with 1/8 demonstrations surpasses imitation learning on full dataset.
- **STaR-style baselines FAIL**: Ungrounded rationales (not executed in environment) actually degrade performance. Grounding in observed consequences is essential.
- **Long CoT at inference doesn't substitute**: Making model think longer at inference provides modest gains but can't match early experience compiled into weights.

## Specific Relevance to Eigenflower

- **IWM IS syllogism compilation**: Train on (premises, inference-rule) -> (conclusion) triples. The logical transition dynamics ARE the world model. Two-stage: Stage 1 = logical dynamics (IWM), Stage 2 = theological content (SFT).
- **Contrastive syllogism training**: For each valid syllogism, generate K=3-4 invalid variants (wrong figure, undistributed middle, illicit process). Model learns from the contrast.
- **Self-reflection for logic**: Generate explanations WHY valid syllogisms are valid and invalid ones are invalid, grounded in specific logical errors. Teaches reasoning principles, not pattern matching.
- **Data efficiency**: 1/8 expert data matches full imitation learning. Small corpus of high-quality syllogisms + early experience augmentation could match training on much larger shallow corpus.
- **LoRA for dynamic recompilation**: Compile base logical rules into main weights. New syllogisms as LoRA adapters. Adapters compose rather than overwrite.
- **Critical warning**: Don't just generate explanations -- GROUND them in actual logical consequences. "If you used this invalid form, HERE is the contradiction" >> "this form is invalid because [hallucinated reason]."

## Key Results

- Consistent improvement over imitation learning across 8 environments, 3 model sizes (3B, 7B, 8B)
- 1/8 expert data with early experience >= full imitation learning (WebShop)
- IWM + RL warm-start reaches higher RL ceilings than imitation + RL
- SR best at K=2-4 alternatives (non-monotonic). IWM improves steadily with larger K.
- Out-of-domain generalization improved, sometimes MORE than in-domain
- 70B with LoRA shows same pattern as smaller full-finetune models

## Key Equations

- IWM loss: L_IWM = -sum log p_theta(s'|s,a)
- SR loss: L_SR = -sum log p_theta(c, a_expert | s) where c = chain-of-thought explanation
- MDP: M = (S, A, T, R, gamma, rho_0) -- R may be unknown, T learned from interaction

## Implementation Notes

- Hardware: at most 8 H100 GPUs (for 8 environments x 3 model sizes)
- Training: LlamaFactory, batch 16, lr 1e-5, 2 epochs
- IWM: 8 non-expert + 1 expert = 9 alternatives per state, rollout data ~10x expert data
- SR: up to 3 unique alternatives, temperature 1.0 for diversity
- Evaluation: max prompt 4096, max response 1024, temperature 0.4
- Models: Llama-3.2-3B, Qwen-2.5-7B, Llama-3.1-8B, Llama-3.3-70B (LoRA)
- RL integration: GRPO algorithm with Verl-Agent hyperparameters

## Combinability with Eigenflower Pipeline

1. Corpus prep (from P0013): Summa structure as curriculum, extract syllogistic structure
2. Data augmentation: K=3-4 invalid variants per valid syllogism
3. Stage 1 (IWM): train on logical transition data
4. Stage 2 (SFT): fine-tune on actual Summa syllogisms in context
5. Stage 3 (optional RL): syllogism validity checker as reward signal
6. Dynamic update: new syllogisms via LoRA adapters
