# Group E: Knowledge Structures & Learning from Experience

Date: 2026-02-26

---

## Paper 1: Inverse Knowledge Search over Verifiable Reasoning

**Full title:** Inverse Knowledge Search over Verifiable Reasoning: Synthesizing a Scientific Encyclopedia from a Long Chains-of-Thought Knowledge Base

**Authors:** Yu Li, Yuan Huang, Tao Wang, Caiyu Fan, Xiansheng Cai, Sihan Hu, Xinzijian Liu, Cheng Shi, Mingjun Xu, Zhen Wang, Yan Wang, Xiangqi Jin, Tianhan Zhang, Linfeng Zhang, Lei Wang, Youjin Deng, Pan Zhang, Weijie Sun, Xinyu Li, Weinan E, Linfeng Zhang, Zhiyuan Yao, Kun Chen

**arXiv:** 2510.26854v3 (Jan 2026)

**Venue/affiliation:** Lanzhou Center for Theoretical Physics, Institute of Theoretical Physics CAS, DP Technology, ENS Paris, various Chinese universities

### Core Technique

The paper addresses what the authors call the "dark matter" of human knowledge -- the derivational reasoning chains that connect facts but are omitted from compressed scientific writing (textbooks, wikis, papers). They build a pipeline with three main components:

1. **Socrates Agent** -- generates ~3 million first-principles questions using a curriculum of ~200 courses, each with ~200 topics, each spawning ~100 prompts. Questions are in two categories: "What and Why" (reductionist derivations from first principles) and "How" (application prompts). Multiple independent LLMs ("Solvers") generate Long Chain-of-Thought (LCoT) answers for each question, and cross-model consensus filters out unreliable ones.

2. **Brainstorm Search Engine** -- performs "inverse knowledge search": given a target concept (endpoint), it retrieves all LCoT derivation chains in the knowledge base that touch that concept. This inverts the typical search paradigm (which finds conclusions matching keywords) to instead find *reasoning pathways* that lead to or through a concept. Query expansion + ranking by cross-disciplinary relevance.

3. **Plato Agent** -- a synthesis LLM that takes the retrieved LCoT chains and narrates them into a coherent encyclopedia article. The LLM's task is shifted from "generation" to "narration" -- weaving pre-verified reasoning chains rather than generating claims from scratch. This dramatically reduces hallucination (~50% reduction vs baseline LLM).

The result is SciencePedia: ~200,000 fine-grained encyclopedia entries with emergent cross-disciplinary connections, organized via a keyword graph clustered using modularity belief propagation (MODBP) into a 21-level hierarchy.

### Relevant Ideas

- **Endpoint-driven reductionist knowledge generation** (Sec 3.1, p6): Instead of forward-reasoning from axioms (hard to guarantee completeness), start from known endpoints (theorems, facts, concepts) and derive them backward to first principles. This is a Socratic inversion. *For eigenflower: this maps directly to compiling syllogisms. Start from known theological conclusions in the Summa and derive backward to premises. The "endpoint" is the conclusion of a syllogism; the derivation IS the syllogism.*

- **Multi-level derivation from the same endpoint** (Sec 3.1, p6): Each endpoint is derived from multiple abstraction levels (high-school, undergraduate, graduate). This forces the model to articulate connections between layers. *For eigenflower: derive the same Thomistic conclusion at multiple levels -- from natural reason, from Scripture, from the Fathers. This creates richer weight structures.*

- **Curriculum scaffolding for systematic coverage** (Sec 3.2, p7): ~200 courses x ~200 topics x ~100 prompts = ~4M questions, filtered to ~3M. The curriculum ensures no domain gaps. *For eigenflower: the Summa already provides a curriculum (Questions organized into Parts and Treatises). The Bible has books/chapters. Use the existing structure as scaffolding rather than generating one.*

- **Two prompt categories: "What and Why" vs "How"** (Sec 3.2, p7-8): Reductionist prompts ask for derivations; application prompts ask for practical use. *For eigenflower: "What and Why" = the syllogistic structure (premises -> conclusion); "How" = pastoral/practical application. Both are present in the Summa (the respondeo + the replies to objections).*

- **Cross-model answer validation** (Sec 3.3, p8): Multiple distinct LLMs from different providers solve the same question. If they disagree, the answer is discarded. Success rate drops from ~70% (undergrad) to ~50% (graduate). *For eigenflower: relevant for corpus generation phase. Could use Claude + Gemini + local model to cross-validate syllogism extractions. But more importantly, this is a filtering mechanism -- only keep extractions where multiple models agree on the logical structure.*

- **Prompt sanitization** (Sec 3.3, p8): Before reasoning, a distinct LLM screens the question for flawed assumptions. Filters ~5% of auto-generated problems. *For eigenflower: when auto-generating questions from the Summa, pre-filter for questions that have verifiable logical structure.*

- **Verifiable endpoint design** (Sec 3.3, p8): Questions are biased toward objectively verifiable answers (numerical, symbolic, multiple-choice). *For eigenflower: syllogisms ARE verifiable endpoints -- the logical structure can be mechanically checked (valid syllogistic form, correct middle term, etc.). This is a huge advantage of working with scholastic logic.*

- **LCoT as a fundamentally new data distribution** (Sec 2.1, p5): The paper argues that RLVR (Reinforcement Learning from Verifiable Rewards, as in DeepSeek-R1) creates a new distribution p_LLM that generates long reasoning chains absent from the internet corpus p_Internet. The key equation: p_LLM(LCoT|Q) >> p_Base(LCoT|Q) ~ 0. *For eigenflower: this suggests that training on reasoning chains (syllogisms) rather than compressed conclusions (catechism-style facts) would produce a qualitatively different kind of model. The Summa's explicit syllogistic structure IS an uncompressed reasoning corpus.*

- **Inverse knowledge search -- concept to provenance** (Sec 4.1, p10): Rather than searching for "what is X," search for "what reasoning chains pass through X." This surfaces non-obvious connections. *For eigenflower: given a theological concept, find all syllogisms in the corpus that use it as a premise, conclusion, or middle term. This would reveal the logical dependency graph of theology.*

- **Knowledge graph emerges from content, not pre-defined** (Sec 5.4, p14): The directed keyword graph (120K nodes, 7454 base communities, 21-level hierarchy) emerges from analyzing which concepts reference each other in synthesized articles. MODBP clustering reveals structure. *For eigenflower: the syllogism dependency graph would emerge naturally from which conclusions serve as premises for other syllogisms. No need to pre-define the graph -- it emerges from the logical structure.*

- **Modularity belief propagation for hierarchical clustering** (Sec 5.4, p14): MODBP recursively partitions the keyword graph until subgraphs are indistinguishable from random. Yields a tree of coarse-to-fine scientific areas. *For eigenflower: use similar community detection on the syllogism dependency graph to discover the natural hierarchical structure of theological knowledge. Would reveal which treatises are most interconnected.*

- **Narration vs generation** (Sec 4.2, p10): The Plato agent's task is explicitly "narration" not "generation" -- weaving pre-verified chains into coherent text. This is the key insight for reducing hallucination. *For eigenflower: inference should be "narration over compiled syllogisms" not "generation from compressed weights." The model should be combining and selecting from its compiled knowledge, not confabulating.*

- **Two-tiered structure: coarse topics + fine-grained pages** (Sec 5.1, p12): ~200 courses -> ~200 topics each -> ~10 keywords each -> ~200K pages. The hierarchy is: course > topic > keyword > page. *For eigenflower: a natural two-tiered structure already exists: Part > Question > Article > Objection/Reply in the Summa. Book > Chapter > Verse in the Bible.*

- **Recursive expansion from existing content** (Sec 5.1, p13): New keywords can be extracted from existing encyclopedia pages, enabling recursive deepening. *For eigenflower: once the model can reason about syllogisms, it could potentially derive NEW syllogisms not explicitly in the training corpus, using the compiled rules of inference.*

- **Formalization of knowledge network as future work** (Sec 6, p15): The authors note that connections between concepts are currently implicit in natural language LCoT chains. They propose using mathematical logic to parse chains into a formal graph. *For eigenflower: this is EXACTLY what syllogism compilation achieves. Formalizing the reasoning chains into explicit logical structures (subject-predicate-middle term, figure, mood) is the eigenflower goal.*

- **Cross-disciplinary links as emergent property** (Sec 5.2, p13): When you store reasoning chains rather than conclusions, cross-domain connections emerge automatically because the same mathematical/logical principles appear in chains from different fields. *For eigenflower: theology and philosophy share premises (natural law, causation, essence/existence). Storing syllogisms would automatically reveal where theology depends on philosophy and vice versa.*

### Relevance to Eigenflower Goals

| Goal | Relevance | How |
|------|-----------|-----|
| 1. Train in 30-60 min on M2 | LOW | Paper uses large LLMs (not specified but implied frontier models). However, the *curriculum scaffolding* approach reduces the problem size -- you don't need a huge model if your training data is already structured as reasoning chains. |
| 2. Small corpus (Bible + Summa) | HIGH | The Summa IS an uncompressed knowledge base -- it already contains explicit derivational chains (the respondeo sections are essentially LCoTs). The Bible + Summa is a pre-structured corpus that maps perfectly to their framework. |
| 3. Compile syllogisms to weights | VERY HIGH | The endpoint-driven reductionist strategy maps directly to syllogism compilation. Each Article in the Summa is an endpoint; the respondeo is the derivation chain. The "inverse knowledge search" is equivalent to querying the syllogism graph. |
| 4. Dynamic weight recompilation | MEDIUM | The recursive expansion idea (new keywords from existing pages) suggests a pattern: compiled knowledge enables generating new knowledge that can then be compiled. Dynamic recompilation = adding new LCoT chains to the knowledge base. |
| 5. Performant inference | MEDIUM | The narration-over-retrieval pattern means inference is constrained retrieval + synthesis rather than unconstrained generation. This is inherently more efficient and more reliable. |
| 6. Diffusion text generation | LOW | No direct connection. |
| 7. Build from scratch | MEDIUM | The curriculum scaffolding is directly implementable. The Socratic question generation pipeline could be built as a C89 tool. |
| 8. Decompile existing models | MEDIUM | The "dark matter" thesis implies that existing models have compressed away the reasoning chains. Decompilation would mean reconstructing these chains from the model's weights -- essentially the inverse of what eigenflower does (compiling chains TO weights). |

### Implementation Notes

- The paper does NOT release training details for the individual LLMs used. The framework is about the pipeline, not the models.
- SciencePedia homepage: sciencepedia.bohrium.com
- They use "multiple independent solver models" from "different providers" but don't specify which.
- The keyword graph has 120,226 nodes and is clustered with MODBP.
- ~200K encyclopedia entries produced.
- Evaluation uses GPT-5 as judge for factual error rates.
- No code repository mentioned.
- The pipeline is embarrassingly parallel (each question is independent), which is good for batch processing.

### Combinability

- **With syllogism compilation (eigenflower core):** The endpoint-driven reductionist strategy IS syllogism extraction. Use their curriculum scaffolding on the Summa's structure (Parts/Questions/Articles) to systematically extract every syllogism. Cross-validate extractions using multiple models.
- **With Paper 2 (Early Experience):** The "early experience" self-reflection mechanism could be used during syllogism extraction -- the model proposes a syllogistic parse, observes the consequences (does it chain correctly with known syllogisms?), and learns from the feedback without explicit reward.
- **With knowledge graph methods:** The emergent keyword graph maps to a syllogism dependency graph. MODBP clustering would reveal the natural structure of theological reasoning.
- **With RAG approaches:** The Brainstorm Search Engine is essentially RAG over reasoning chains rather than over conclusions. For inference, retrieve relevant syllogisms and narrate/combine them.

---

## Paper 2: Agent Learning via Early Experience

**Full title:** Agent Learning via Early Experience

**Authors:** Kai Zhang, Xiangchao Chen, Bo Liu, Tianci Xue, Zeyi Liao, Zhihan Liu, Xiyao Wang, Yuting Ning, Zhaorun Chen, Xiaohan Fu, Jian Xie, Yuxuan Sun, Boyu Gou, Qi Qi, Zihang Meng, Jianwei Yang, Ning Zhang, Xian Li, Ashish Shah, Dat Huynh, Hengduo Li, Zi Yang, Sara Cao, Lawrence Jang, Shuyan Zhou, Jiacheng Zhu, Huan Sun, Jason Weston, Yu Su, Yifan Wu

**arXiv:** 2510.08558v2 (Oct 2025)

**Venue/affiliation:** Meta Superintelligence Labs, FAIR at Meta, The Ohio State University

### Core Technique

The paper introduces the "early experience" paradigm as a middle ground between imitation learning (requires expert demonstrations, doesn't scale) and reinforcement learning (requires reward signals, often unavailable). The key insight: an agent's own exploratory actions and their resulting environment states can serve as supervision signals WITHOUT any reward function.

Two concrete methods are proposed:

1. **Implicit World Modeling (IWM):** Starting from expert trajectories, at each state the agent proposes K alternative actions (sampled from its own policy). These alternatives are actually executed in the environment, yielding next-states. The model is then trained to predict these next-states given (state, action) pairs, using standard next-token prediction loss. This teaches the model the environment's transition dynamics implicitly. Two-stage: first train on world-model data (1 epoch), then fine-tune on expert demonstrations. Same total compute budget as imitation learning.

2. **Self-Reflection (SR):** For each expert state, the agent proposes alternatives and executes them. An LLM then generates chain-of-thought explanations of WHY the expert action was better than each alternative, grounded in the actual observed state transitions. The model is trained to produce these reflective explanations followed by the correct action. This teaches generalizable decision principles rather than rote action patterns.

Both methods are evaluated across 8 diverse environments (ALFWorld, ScienceWorld, TravelPlanner, BFCLv3, Tau-Bench, SearchQA, WebShop, WebArena-Lite) with 3 model sizes (Llama-3.2-3B, Qwen-2.5-7B, Llama-3.1-8B). Both consistently outperform pure imitation learning.

### Relevant Ideas

- **Learning from consequences without reward** (Sec 1, p1-2): The fundamental insight is that you can learn from observing what happens when you take actions, without anyone telling you if those actions were good or bad. The environment's response IS the signal. *For eigenflower: when compiling syllogisms to weights, "consequences" = logical validity. If you compile a syllogism and it produces contradictions with other compiled syllogisms, that IS feedback. No external reward needed.*

- **Implicit world modeling as auxiliary prediction task** (Sec 4.2, p6): Train the model to predict next-states from (state, action) pairs. The training objective is L_IWM = -sum log p_theta(s'|s,a). This is standard next-token prediction but on state-transition data rather than static text. Uses the SAME model parameters for both world modeling and action prediction. *For eigenflower: train the model to predict what follows logically from a set of premises. Given (premises, inference-rule), predict (conclusion). This IS syllogistic reasoning as a next-token prediction task.*

- **Self-reflection as contrastive learning** (Sec 4.3, p6-7): The model learns WHY the expert action is better by comparing outcomes. The chain-of-thought explanation becomes part of the training target: L_SR = -sum log p_theta(c, a_expert | s). The explanation c is generated by a separate LLM comparing the expert next-state with alternative next-states. *For eigenflower: generate explanations for WHY a valid syllogism is valid and WHY an invalid one is invalid. Train on (premises, explanation-of-validity, conclusion) triples. This teaches logical reasoning principles, not just pattern matching.*

- **Branching factor K and data amplification** (Sec 4.1, p5; Sec 6.3, p12): For each expert state, sample K alternative actions. This multiplies the training data by ~K. For ALFWorld: 21,031 expert pairs x 9 alternatives = 189,279 IWM triples. IWM improves steadily with larger K; SR is non-monotonic (best at K=2-4). *For eigenflower: for each correct syllogism, generate K incorrect variants (wrong middle term, wrong figure, illicit major/minor, etc.). This gives the model contrastive examples. K=2-4 seems like a sweet spot for self-reflection.*

- **Two-stage training pipeline** (Sec 4.2, p6): First world-model pre-training (1 epoch on rollout data), then supervised fine-tuning on expert data. Same total compute. *For eigenflower: first stage = train on (premises, rule) -> (conclusion) prediction (world modeling); second stage = train on complete syllogisms with their Summa context. This gives the model logical transition dynamics before theological content.*

- **Works with small models** (Sec 5.1, p8; Sec 6.4, p13): Tested on 3B, 7B, 8B, and 70B models. Benefits persist at ALL scales. Even 3B models show consistent gains. The 70B model uses LoRA for efficiency. *For eigenflower: this is encouraging. If the paradigm works at 3B, it might work at sub-1B scales needed for M2 MacBook training. The key is that early experience amplifies whatever capacity the model has.*

- **Reward-free learning that bootstraps RL** (Sec 5.4, p11): When verifiable rewards later become available, models warm-started with early experience reach HIGHER RL ceilings than those started from imitation learning. The performance gap sometimes GROWS during RL training. *For eigenflower: this suggests a pipeline: (1) compile syllogisms as early experience (reward-free), (2) later add verification rewards (logical validity checking). The two stages compound rather than substitute.*

- **Out-of-domain generalization** (Sec 5.3, p10): Early experience improves OOD performance, sometimes even MORE than in-domain. IWM helps most when environment dynamics are stable; SR helps when tool/argument distributions shift. *For eigenflower: a model trained on syllogisms from the Summa should generalize to syllogistic reasoning on new texts (e.g., Church Fathers, papal encyclicals). IWM would help with the logical structure (stable); SR would help with new vocabulary/domains (shifting).*

- **Exploration of the state space beyond expert demonstrations** (Sec 4, p5): The core innovation is that the agent explores states the expert never visited. This reduces distribution shift -- the model has seen what happens when it makes mistakes. *For eigenflower: don't just train on valid syllogisms. Have the model attempt syllogistic parsing, observe what goes wrong (invalid forms, ambiguous premises), and learn from those failures.*

- **Interaction traces as natural-language supervision** (Sec 4.2, p6): In their framework, states are text (DOM trees, tool outputs, error messages). The world model predicts text given text. No special state encoding needed. *For eigenflower: logical states (sets of propositions, syllogistic forms) can be represented as text. The world model predicts "what follows logically" in natural language. This means the architecture can be a standard language model.*

- **Self-reflection prompt template** (Sec 4.3, p7): The paper provides an explicit prompt template for generating self-reflections. The key structure: (1) analyze the situation, (2) compare possible actions and why each may be less optimal, (3) justify the expert action grounded in expected outcome, (4) highlight relevant constraints. *For eigenflower: adapt this for syllogistic reasoning: (1) state the premises, (2) compare possible conclusions and why each invalid one fails, (3) justify the valid conclusion grounded in the rules of inference, (4) highlight the specific logical principles at work.*

- **Data efficiency: 1/8 expert data matches full imitation learning** (Sec 6.2, p12): On WebShop, early experience with 1/8 of the demonstrations surpasses imitation learning on the full dataset. *For eigenflower: this suggests that a relatively small corpus of high-quality syllogisms + early experience augmentation could match training on a much larger but shallower corpus.*

- **IWM and SR serve different functions** (Sec 5.2, p9-10): IWM learns environment dynamics (what HAPPENS); SR learns decision principles (WHY to choose). IWM is better for structured/predictable environments; SR is better for reasoning-heavy tasks. *For eigenflower: IWM = learning logical entailment patterns; SR = learning WHY certain logical moves are correct. Both are needed. IWM for the "mechanical" validity checking, SR for understanding the semantic content.*

- **Training hyperparameters** (Appendix B, p20): Batch size 16, learning rate 1e-5, 2 epochs, using LlamaFactory. Max prompt length 4096, max response 1024, temperature 0.4 for evaluation. For IWM, 8 non-expert actions sampled per state plus the expert action = 9 alternatives. For SR, up to 3 unique alternative actions, temperature 1.0 for diversity. *For eigenflower: concrete starting points for hyperparameters. The small batch size and low epoch count are promising for MacBook training.*

- **LoRA for large model efficiency** (Sec 6.4, p13): 70B models use LoRA (parameter-efficient fine-tuning) with the same rank and update steps. IWM LoRA adapters are continued into the supervised stage. *For eigenflower: even if the base model is modest, LoRA-style adaptation could enable dynamic weight recompilation (goal 4) -- add new syllogisms by training small adapter weights rather than retraining the whole model.*

- **Performance scales with model size AND early experience** (Sec 6.4, p13): Early experience checkpoints consistently occupy the top performance curve across 3B/8B/70B. The supervision complements rather than substitutes for model capacity. *For eigenflower: even a small model benefits from structured early experience. The syllogism corpus doesn't need to compensate for model size -- it amplifies whatever capacity exists.*

- **STaR-style baselines fail** (Sec 6.1, Table 4, p11-12): STaR (Self-Taught Reasoner) generates rationales for expert actions but WITHOUT executing alternatives in the environment. These ungrounded rationales actually DEGRADE performance when fine-tuned on. The key difference: early experience rationales are grounded in observed consequences; STaR rationales are hallucinated. *For eigenflower: this is a critical warning. Don't just generate explanations of syllogisms -- ground them in actual logical consequences. "If you used this invalid form, HERE is the contradiction that results" is much more useful than "this form is invalid because [hallucinated reason]."*

- **Long CoT at inference doesn't help as much as grounded training** (Sec 6.1, Table 4, p11): Simply making the model think longer at inference (Long CoT) provides modest gains over the prompt baseline but fails to approach early experience performance. On imitation-learning models, Long CoT actually HURTS (WebShop: 47.3 -> 0.0; ALFWorld: 80.5 -> 25.8). *For eigenflower: reasoning at inference time is not a substitute for having the right knowledge compiled into weights. This supports the compilation approach over a pure prompt-engineering approach.*

- **MDP formalization** (Sec 3, p4): The environment is formalized as M = (S, A, T, R, gamma, rho_0). States S are text, actions A are discrete choices, T is the transition function. Even though R (reward) may be unknown, T can be learned from interaction. *For eigenflower: formalize syllogistic reasoning as an MDP. States = sets of known propositions; Actions = application of inference rules; Transitions = new propositions derived; Reward = (optional) logical consistency.*

### Relevance to Eigenflower Goals

| Goal | Relevance | How |
|------|-----------|-----|
| 1. Train in 30-60 min on M2 | HIGH | Works at 3B scale with batch size 16, 2 epochs, learning rate 1e-5. The data amplification (K alternatives per state) means you get more training signal from less data. LoRA makes even larger models feasible. |
| 2. Small corpus (Bible + Summa) | VERY HIGH | Data efficiency is a core result -- 1/8 expert data matches full imitation learning. The small corpus is AMPLIFIED by generating alternatives and their consequences. |
| 3. Compile syllogisms to weights | VERY HIGH | IWM is literally "compile transition dynamics into weights." Syllogistic inference rules ARE transition dynamics (premises + rule -> conclusion). The two-stage pipeline (world model then expert data) maps perfectly. |
| 4. Dynamic weight recompilation | HIGH | LoRA adapters for incremental learning. New syllogisms could be compiled as adapter weights. The warm-start property means new compilations build on old ones rather than overwriting. |
| 5. Performant inference | MEDIUM | Compiled knowledge in weights means inference doesn't need long reasoning chains or retrieval. The model has internalized the dynamics. |
| 6. Diffusion text generation | LOW | No direct connection. |
| 7. Build from scratch | HIGH | The method is architecture-agnostic (tested on Llama and Qwen). The training loop is standard SFT with augmented data. Implementable from scratch. |
| 8. Decompile existing models | MEDIUM | The IWM objective (predict next-state from state+action) could be used as a probe: give an existing model (state, action) pairs and see what transitions it predicts. This reveals what "world model" is implicit in its weights. |

### Implementation Notes

- **Hardware:** "at most 8 H100 GPUs for training and evaluation" -- but this is for 8 environments x 3 model sizes. A single environment on a single small model would be far less.
- **Training framework:** LlamaFactory (open source)
- **Models tested:** Llama-3.2-3B, Qwen-2.5-7B, Llama-3.1-8B, Llama-3.3-70B (LoRA)
- **Hyperparameters:** batch size 16, lr 1e-5, 2 epochs
- **IWM data ratio:** rollout data is ~10x expert data (189K vs 21K for ALFWorld)
- **SR data construction:** prompt the same policy model at temperature 1.0 for up to 3 unique alternatives per state
- **Evaluation:** max prompt 4096 tokens, max response 1024 tokens, temperature 0.4
- **RL integration:** GRPO algorithm with default Verl-Agent hyperparameters
- **No code repository mentioned explicitly** but LlamaFactory is the training framework

### Combinability

- **With Paper 1 (Inverse Knowledge Search):** Use the curriculum scaffolding from Paper 1 to generate the "expert trajectories" (correct syllogistic derivations from the Summa). Then apply early experience augmentation from Paper 2: at each step of the derivation, propose alternative logical moves, observe their consequences, and learn from the comparison. The curriculum provides WHAT to learn; early experience provides HOW to learn it efficiently.

- **With syllogism compilation (eigenflower core):** The two-stage pipeline maps perfectly:
  - Stage 1 (IWM): Train on (premises, inference-rule) -> (conclusion) triples and their incorrect variants. This teaches the model logical dynamics.
  - Stage 2 (SFT): Fine-tune on actual Summa syllogisms in context. This teaches theological content.
  - Stage 3 (optional RL): If you have a syllogism validity checker, use it as reward signal to further refine.

- **With LoRA for dynamic recompilation:** Compile base logical rules into the main weights. Compile specific theological content as LoRA adapters. To "record a new syllogism," train a small adapter. To combine domains, merge adapters. This gives goal 4 (dynamic weight recompilation) for free.

- **With small model architectures:** The data efficiency results suggest that a model far below 3B might work if the training data is sufficiently structured. The Summa's explicit syllogistic form + early experience augmentation could make a sub-1B model surprisingly capable within the theological domain.

---

## Cross-Paper Synthesis

### The Eigenflower Training Pipeline (combining both papers)

A concrete pipeline emerges from combining these two papers:

**Phase 1: Corpus Preparation (from Paper 1)**
1. Use the Summa's structure as curriculum scaffolding (Parts > Questions > Articles)
2. For each Article, extract the syllogistic structure: premises, middle terms, conclusions
3. Cross-validate extractions using multiple models (Claude, Gemini, local)
4. Build a syllogism dependency graph (which conclusions serve as premises elsewhere)
5. Cluster the graph (MODBP or simpler) to discover natural theological structure

**Phase 2: Data Augmentation (from Paper 2)**
6. For each valid syllogism, generate K=3-4 invalid variants (wrong figure, illicit process, undistributed middle, etc.)
7. For IWM: create (premises, rule, variant-conclusion) triples showing what WOULD follow under different logical moves
8. For SR: generate chain-of-thought explanations of WHY valid syllogisms are valid and invalid ones are invalid, grounded in the specific logical errors

**Phase 3: Two-Stage Training (from Paper 2)**
9. Stage 1 -- World Model: train on logical transition data (IWM triples), 1 epoch
10. Stage 2 -- Expert Data: fine-tune on actual Summa syllogisms in context, remaining epochs
11. (Optional) Stage 3 -- RL: use syllogism validity checker as reward signal

**Phase 4: Knowledge Retrieval (from Paper 1)**
12. At inference, use inverse knowledge search: given a query concept, retrieve all compiled syllogisms that touch it
13. The model narrates/combines retrieved syllogisms rather than generating from scratch
14. New syllogisms can be dynamically compiled via LoRA adapters

### Key Insight

Paper 1 tells us WHAT kind of training data produces reliable knowledge models (uncompressed reasoning chains with verifiable endpoints -- i.e., syllogisms).

Paper 2 tells us HOW to train efficiently on small data (amplify expert demonstrations with exploratory alternatives and their consequences -- i.e., valid vs invalid syllogistic forms).

Together they describe an approach to building a small, from-scratch model that:
- Has explicit, verifiable logical structure (not black-box pattern matching)
- Learns efficiently from a small corpus (the Summa is ~3.5M words, tiny by LLM standards)
- Can be incrementally updated (LoRA adapters for new syllogisms)
- Produces reliable inference (narration over compiled knowledge, not unconstrained generation)

### What Neither Paper Addresses

- Actual architecture design for small models (both assume existing LLM architectures)
- Training time on consumer hardware (both use server GPUs)
- Diffusion-based text generation
- How to EXTRACT syllogisms from natural text (they assume structured data already exists)
- Multilingual training (Paper 1 mentions language parity as an advantage but doesn't detail it)
- The Latin-specific aspects of the corpus

These gaps represent where eigenflower needs original research or ideas from other papers.
