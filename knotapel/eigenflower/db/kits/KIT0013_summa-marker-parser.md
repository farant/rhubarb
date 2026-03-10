---
id: KIT0013
title: "Summa Theologica structural marker parser"
status: planned
tier: 0
input: "Summa article text (Latin)"
output: "Structured sections (objections, sed contra, respondeo, replies)"
depends_on: []
effort: small
source: ["Architecture Insight notes", "Inverse Knowledge Search paper"]
migrated_from: PT0032
applicable_to: [G0012, G0005]
related: [KIT0012, KIT0014]
---

## Description

The Summa Theologica has highly consistent structural markers across 3,000+ articles. A deterministic parser exploits these for segmentation:

- **"Videtur quod..."** (It seems that...) = objection = premise to be refuted
- **"Sed contra..."** (On the contrary...) = counter-authority
- **"Respondeo dicendum quod..."** (I answer that...) = the actual argument
- **"Ad primum/secundum/tertium..."** = replies to specific objections

The structural markers are the "curriculum scaffolding" that the Inverse Knowledge Search paper recommends -- except the Summa already provides it naturally.

The respondeo sections are essentially Long Chains-of-Thought (LCoTs). This is the "dark matter" that the Inverse Knowledge Search paper says is missing from compressed scientific writing -- but it's already present in the Summa.

## Implementation

Simple state machine parser. Walk the text, match markers, emit structured sections. No ambiguity -- the markers are formulaic across the entire Summa.

## API Sketch

```c
nomen enumeratio SectioGenus {
    SECTIO_OBIECTIO,
    SECTIO_SED_CONTRA,
    SECTIO_RESPONDEO,
    SECTIO_AD_OBIECTIONEM,
    SECTIO_TITULUS
};

nomen structura SummaArticulus {
    Chorda titulus;
    Chorda *obiectiones;
    longus obiectiones_numerus;
    Chorda sed_contra;
    Chorda respondeo;
    Chorda *ad_obiectiones;
    longus ad_obiectiones_numerus;
};

integer summam_parsare(constans character *textus, longus longitudo, SummaArticulus *articulus);
```

~300 LOC. State machine with zero ambiguity.
