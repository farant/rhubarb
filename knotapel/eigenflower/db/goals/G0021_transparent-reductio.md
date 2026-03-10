---
id: G0021
title: "Build transparent reductio ad absurdum"
status: open
priority: low
depends_on: [G0018, G0007]
blocks: [G0023]
related: []
---

## Description

Design and implement reductio ad absurdum reasoning within the syllogism graph. Reductio shows that denying a conclusion produces a contradiction — it is backward chaining through the syllogism graph with negation.

The mechanism: assume the negation of the target conclusion, then forward-chain from that assumption. If a contradiction is reached (P and not-P both derivable), the original conclusion is established.

This requires the syllogism graph to handle negation correctly and to detect contradictions efficiently.

## Why It Matters

Reductio is one of Aquinas's five reasoning modes and it has a naturally transparent structure within the existing syllogism graph architecture. It is essentially backward chaining with a twist — you are searching for a contradiction rather than a derivation. The transparency comes from the fact that every step in the reductio chain is a valid syllogistic inference, and the contradiction is an exact logical fact (not a fuzzy judgment).

Aquinas uses reductio frequently, especially in the objection-reply structure. Many replies to objections work by showing that the objection's premise leads to an absurd or contradictory conclusion.

## What "Achieved" Looks Like

- Negation handling in the syllogism graph (proposition nodes can be negated)
- Contradiction detection: given a set of premises, find if any pair (P, not-P) is derivable
- Reductio procedure: assume not-C, forward-chain, detect contradiction, conclude C
- Test cases from Summa objection-reply pairs
- Integration with the derivation narration (G0013) — reductio chains are narrated clearly
- Performance: reductio search terminates in bounded time (no infinite loops from circular dependencies)
