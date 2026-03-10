---
id: LIM0012
title: "Latin-first codebase (identifiers, comments, messages)"
type: chosen
source: "CLAUDE.md (rhubarb project)"
impacts: []
related: [LIM0006]
---

All code is written in Latin using the latina.h macro header. This includes variable names, function names, struct names, comments, error messages, and test descriptions. A few technical terms (like "offset") are left untranslated where Latin equivalents would be unclear.

Why:
- Latin is a sacred language and a universal language that has persisted for thousands of years.
- Building things intended to last thousands of years should use a language with that kind of track record.
- Acts as a natural filter — developers who engage with Latin-language code are self-selected for the kind of long-term, careful thinking the project values.
- Consistent with the Catholic civilization-building mission.

Implications:
- Development is slower because naming requires Latin vocabulary knowledge (or lookup).
- Claude's Latin is generally better than the developer's, which helps.
- Reserved word conflicts: "nomen" is a macro for "typedef" and cannot be used as a variable name. Use "titulis" or similar alternatives.
- Reading the code requires at least basic Latin comprehension, which limits the contributor pool (intentionally).
- The latina.h header maps all C89 keywords to Latin equivalents (si/if, dum/while, redde/return, etc.).
