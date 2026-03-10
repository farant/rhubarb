---
id: P0013
title: "Inverse Knowledge Search over Verifiable Reasoning: Synthesizing a Scientific Encyclopedia from a Long Chains-of-Thought Knowledge Base"
authors: [Yu Li, Yuan Huang, Tao Wang, Caiyu Fan, Xiansheng Cai, Sihan Hu, Xinzijian Liu, Cheng Shi, Mingjun Xu, Zhen Wang, Yan Wang, Xiangqi Jin, Tianhan Zhang, Linfeng Zhang, Lei Wang, Youjin Deng, Pan Zhang, Weijie Sun, Xinyu Li, Weinan E, Linfeng Zhang, Zhiyuan Yao, Kun Chen]
year: 2026
arxiv_id: "2510.26854"
extraction_group: E
relevance: high
key_techniques: [inverse_knowledge_search, endpoint_driven_derivation, curriculum_scaffolding, cross_model_validation, socratic_question_generation, narration_over_generation, modbp_clustering, knowledge_graph_emergence]
summary: "Builds SciencePedia (~200K entries) by inverting the knowledge search paradigm: given a concept, retrieve all reasoning chains that pass through it. Three components: Socrates Agent (systematic question generation), Brainstorm Search Engine (inverse retrieval), Plato Agent (narration over verified chains). ~50% hallucination reduction vs baseline LLM."
related: []
---

## Key Contributions

Addresses the "dark matter" of human knowledge -- derivational reasoning chains connecting facts that are omitted from compressed scientific writing. Three-component pipeline: (1) Socrates Agent generates ~3M first-principles questions via curriculum scaffolding (~200 courses x ~200 topics x ~100 prompts), (2) Brainstorm Search Engine performs "inverse knowledge search" retrieving all reasoning chains touching a concept, (3) Plato Agent narrates verified chains into coherent encyclopedia articles. Result: SciencePedia with ~200K entries and emergent cross-disciplinary connections.

## Key Techniques

- **Endpoint-driven reductionist derivation**: Start from known conclusions, derive backward to first principles. The Socratic inversion. Maps directly to compiling syllogisms: start from conclusion, derive premises.
- **Multi-level derivation**: Same endpoint derived at multiple abstraction levels (high-school, undergrad, graduate). Forces articulation of cross-layer connections.
- **Curriculum scaffolding**: ~200 courses x ~200 topics x ~100 prompts = systematic coverage. No domain gaps.
- **Two prompt categories**: "What and Why" (reductionist derivations) and "How" (applications). Maps to syllogistic structure and pastoral application.
- **Cross-model validation**: Multiple distinct LLMs solve same question. Disagreement = discard. 70% success (undergrad) to 50% (graduate).
- **Inverse knowledge search**: Given concept, find all derivation chains passing through it. Surfaces non-obvious connections.
- **Narration vs generation**: Plato Agent weaves pre-verified chains rather than generating claims. ~50% hallucination reduction.
- **Emergent knowledge graph**: Directed keyword graph (120K nodes, 7454 communities, 21-level hierarchy) emerges from content analysis. MODBP clustering reveals structure without predefined ontology.
- **LCoT as new data distribution**: RLVR creates distribution generating long reasoning chains absent from internet corpus. Training on reasoning chains (not compressed conclusions) produces qualitatively different models.

## Specific Relevance to Eigenflower

- **Summa IS an uncompressed knowledge base**: The respondeo sections are essentially Long Chain-of-Thought derivations. The Summa already provides curriculum scaffolding (Parts > Questions > Articles).
- **Syllogism dependency graph**: Which conclusions serve as premises elsewhere would emerge naturally, revealing logical dependency structure of theology.
- **Narration over compiled syllogisms**: Inference should combine/select from compiled knowledge, not confabulate from compressed weights.
- **Cross-disciplinary emergence**: Storing syllogisms automatically reveals where theology depends on philosophy and vice versa.
- **Formalization as eigenflower goal**: Paper notes parsing chains into formal logic graph as future work -- this is EXACTLY what syllogism compilation achieves.

## Key Results

- ~200K fine-grained encyclopedia entries produced
- 120,226 keyword nodes in emergent knowledge graph
- 21-level hierarchical structure via MODBP clustering
- ~50% hallucination reduction vs baseline LLM (Plato narration approach)
- Cross-model validation success: 70% (undergrad) to 50% (graduate)

## Implementation Notes

- No code repository released
- SciencePedia: sciencepedia.bohrium.com
- Pipeline is embarrassingly parallel (each question independent)
- Uses "multiple independent solver models from different providers" (unspecified)
- Evaluation uses GPT-5 as judge for factual error rates
- The curriculum scaffolding approach is directly implementable as a C89 tool
- MODBP clustering could be applied to syllogism dependency graphs
