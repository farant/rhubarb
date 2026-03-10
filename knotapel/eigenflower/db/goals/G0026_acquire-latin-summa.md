---
id: G0026
title: "Acquire Latin Summa Theologica text"
status: open
priority: medium
depends_on: []
blocks: [G0003]
related: []
---

## Description

Obtain the complete Latin text of the Summa Theologica for inclusion in the training corpus. The CLAUDE.md notes this as a TODO: "Latin Summa text still needed." The English Summa is already present in the corpus folder, but the Latin original is required for training a bilingual fluency model and for accurate syllogism extraction (the Latin is the authoritative text).

## Why It Matters

The Latin Summa is not optional — it is the primary text. The English translations are derivative. For accurate extraction of logical structure, the Latin text is essential because:

- Aquinas's Latin is technically precise in a way translations often blur
- Syllogistic terms have specific Latin formulations that map directly to logical operators
- The structural markers (videtur quod, sed contra, respondeo dicendum) are in Latin
- Training the fluency layer on the original Latin produces a model that can work with the authoritative text

Additionally, Latin-English parallel text is valuable for training bilingual capabilities — the same logical content expressed in two languages provides natural alignment data.

## What "Achieved" Looks Like

- Complete Latin Summa Theologica text obtained (all parts: Prima Pars, Prima Secundae, Secunda Secundae, Tertia Pars, supplements)
- Text cleaned and formatted consistently
- Placed in corpus/ directory alongside existing texts
- Validated against known article counts per part
- Encoding verified (UTF-8, Latin characters correct)
