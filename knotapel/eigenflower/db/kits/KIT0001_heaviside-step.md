---
id: KIT0001
title: "Heaviside step function"
status: planned
tier: 0
input: "Real number x, threshold t"
output: "1 if x >= t, 0 otherwise"
depends_on: []
effort: trivial
source: []
applicable_to: [G0005]
related: [KIT0003]
---

## Description

The Heaviside step function H(x) is the nonlinearity in tensor logic: SP[s,p] = H(SM[s,m] * MP[m,p]). Without it, syllogism compilation is just matrix multiply with no activation.

Configurable threshold allows experimentation with soft vs hard boundaries. For exact Boolean computation (forward DKC), threshold is typically 0.5 or 1.

## API Sketch

```c
integer gradus_caelestis(duplex x, duplex limen);
```

~20 LOC. Trivial but necessary as a named, tested component.
