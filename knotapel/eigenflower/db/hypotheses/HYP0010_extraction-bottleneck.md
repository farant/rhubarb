---
id: HYP0010
title: "The extraction problem (parsing syllogisms FROM natural language) is the real bottleneck, not compilation TO weights"
status: proposed
source: "notes/01-analysis.md"
evidence_for: []
evidence_against: []
testable_by: []
related: []
---

## Claim

Everyone in the paper extractions talks about compiling syllogisms TO weights, but actually parsing syllogisms FROM the Summa's Latin prose is its own hard problem and is the real bottleneck for the eigenflower project.

The compilation side has strong mathematical grounding: tensor logic (Domingos), kernel ridge regression (Hilbert Space paper), GPTQ-as-CVP, and DKC's 109 demos. Given a syllogism in structured form, compiling it to weights is a solved or nearly-solved problem.

The extraction side is much less developed. The Summa is structured (sed contra, respondeo, etc.) but the actual logical structure within each article is implicit. "Respondeo dicendum quod" introduces the argument but doesn't label its premises, middle terms, and conclusions. Extracting the syllogistic skeleton from Latin prose requires:
1. Parsing medieval Latin grammar
2. Identifying propositions within prose
3. Recognizing which propositions serve as premises and which as conclusions
4. Identifying the middle terms that connect them
5. Determining which syllogistic figure is being used

None of the 14 papers address this concretely.

## Evidence For

- **Analysis Part 1 "What's Missing" section:** "No one addressed the extraction problem concretely." This gap is explicitly called out.
- **The Summa's implicit logical structure:** The structural markers (videtur quod, sed contra, respondeo) identify SECTIONS but not the syllogistic form within each section.
- **Compilation is well-grounded:** The tensor logic equation SP[s,p] = H(SM[s,m] * MP[m,p]) works. Kernel ridge regression gives closed-form operators. GPTQ-as-CVP handles quantization. The compilation pipeline has solid mathematical foundations.
- **The architecture insight (02):** Identifies the fluency layer's role as solving the extraction problem — parsing Latin prose into logical structure. This is the fluency layer's hardest job.
- **Medieval Latin complexity:** Latin has free word order, heavy use of subjunctive and ablative constructions, extensive use of technical philosophical vocabulary with precise but context-dependent meaning.

## Evidence Against

- The Summa's rigid article structure gives the parser a huge head start compared to parsing arbitrary text. The problem may be easier than it appears because the genre conventions are so strong.
- Semi-automated extraction (fluency layer proposes, human verifies) reduces the problem to something more tractable than full automation.
- Existing NLP tools for Latin (while imperfect) have improved significantly and could provide a starting point.
- The extraction problem may not be a bottleneck if the initial scope is limited to clearly formal syllogisms (of which there may be enough to start with).

## What Would Confirm It

- Attempting to parse syllogistic structure from 100 Summa articles and finding that the success rate is low (<50%) even with the structural markers.
- Showing that the fluency layer (once built) struggles with the parsing task despite being trained on the Summa.
- Demonstrating that compilation works perfectly but the system is bottlenecked by the quality of extracted syllogisms.

## What Would Refute It

- Building a relatively simple parser that exploits the Summa's structural markers and achieves >80% extraction accuracy on clearly syllogistic articles.
- Showing that the compilation side has unexpected difficulties that make it as hard as or harder than extraction.
- Finding that a deterministic parser (lapifex-style) can handle the Summa's Latin without needing a trained fluency layer.

## Implications If True

- The build order should prioritize extraction capabilities alongside (not after) the syllogism layer.
- The fluency layer is not just a "translator" — it is the critical path component.
- Semi-automated extraction with human verification may be the practical approach for the initial corpus.
- The Summa's structural markers are a key engineering asset that should be exploited maximally.
- The deterministic syllogism parser (identified as a next step in the vision document) is high-priority.
