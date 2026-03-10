---
id: KIT0012
title: "Syllogistic form validator"
status: planned
tier: 0
input: "Claimed syllogism (propositions, figure, mood)"
output: "Valid/invalid with error message"
depends_on: []
effort: small
source: []
applicable_to: [G0005, G0004]
related: [KIT0003, KIT0013]
---

## Description

Validates that a syllogism is formally correct according to Aristotelian logic. Given three propositions (major premise, minor premise, conclusion) with figure and mood annotations, checks all validity rules.

## Rules to check

- **Figure**: which term is middle (4 figures)
- **Mood**: which proposition types (A, E, I, O) for each of the 3 propositions
- **Distribution**: middle term must be distributed at least once
- **Quality**: negative premise requires negative conclusion
- **Quantity**: particular premise requires particular conclusion
- **Existential import**: two universal premises cannot yield a particular conclusion (without existential assumption)

24 valid moods across 4 figures (15 unconditionally valid + 9 with existential import).

## API Sketch

```c
nomen structura Syllogismus {
    character figura;         /* 1, 2, 3, 4 */
    character modus[3];       /* e.g. "AAA" for Barbara */
    character *maior;         /* major premise text */
    character *minor;         /* minor premise text */
    character *conclusio;     /* conclusion text */
};

integer syllogismum_validare(constans Syllogismus *s, character *error, longus error_max);
```

~200 LOC. Purely deterministic logic rules, no ambiguity.
