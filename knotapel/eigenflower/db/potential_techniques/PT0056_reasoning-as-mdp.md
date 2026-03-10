---
id: PT0056
title: "Formalize syllogistic reasoning as MDP"
status: untested
source: ["Early Experience paper (Zhang et al., 2025)"]
applicable_to: []
related: [PT0027, PT0055]
---

Formalize syllogistic reasoning as a Markov Decision Process: M = (S, A, T, R, gamma, rho_0).

- States S = sets of known propositions
- Actions A = application of inference rules (Barbara, Celarent, Darii, Ferio, etc.)
- Transitions T = new propositions derived from applying rule to current state
- Reward R = (optional) logical consistency, derivation of target conclusion
- rho_0 = initial premises

Even if R (reward) is unknown, T (transition dynamics) can be learned from interaction via IWM. The environment's response (what propositions follow from applying a rule) IS the supervision signal.

For eigenflower: this formalization connects syllogistic reasoning to the reinforcement learning literature. The Early Experience paper's IWM and SR methods apply directly to this MDP. The "early experience" of exploring alternative logical moves and observing consequences teaches the model logical dynamics.

The MDP formalization also connects to the "walk" metaphor in the hybrid architecture: the fluency layer navigates the MDP, choosing which inference rules to apply.
