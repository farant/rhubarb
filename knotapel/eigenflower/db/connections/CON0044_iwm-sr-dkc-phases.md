---
id: CON0044
title: "IWM (world modeling) and SR (self-reflection) map to DKC's natural research phases"
connects:
  - "Paper 2 dual methods: IWM predicts next-states, SR explains WHY expert action is better"
  - "DKC catalog building phase (D1-29) + mechanism discovery phase (D85-109)"
type: structural_parallel
strength: moderate
source: "extractions/group-e-knotapel-relevance.md, Connection 5"
---

DKC's research program naturally decomposed into IWM-like and SR-like phases:

IWM phase (D1-29, D45-50): Building the bracket catalog IS implicit world modeling. Each (braid_word, bracket_value) pair is a (state, action)->next_state triple. D10's transition from symbolic to numerical was "compress world model into efficient representation." D29's transition to exact Z[zeta_8] was "upgrade to lossless world model."

SR phase (D85-109): The mechanism investigation arc is self-reflection. D89 tested 8 hypotheses for WHY deep entries outperform shallow. D90 discovered axis cancellation. D92 proved WHY +/-q encoding locks to parity. D93 explains WHY AND is easy and XOR is hard. D108's Dual-Channel Theorem explains WHY certain values compute parity and others cannot.

Paper 2 found IWM and SR serve different functions: IWM learns environment dynamics (what HAPPENS), SR learns decision principles (WHY). DKC's "what happens" lives in catalogs and exhaustive search. The "why" lives in theorems: Parity-Lock, Complement-Blindness, Barrington-Radical, Dual-Channel, Z/4Z Axis-Alignment.

Paper 2 found IWM helps most when dynamics are stable (logical entailment = maximally stable). SR helps when distributions shift (new algebras, encodings, activations). DKC confirms: bracket computation (IWM-learnable) is universal, but capacity implications (SR-learnable) shift between solvable/non-solvable (D94), finite/infinite (D80), different encodings (D92).
