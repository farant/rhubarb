---
id: LIM0006
title: "C89 only — no modern C, no C++, no other languages"
type: chosen
source: "CLAUDE.md (rhubarb project)"
impacts: []
related: [LIM0005, LIM0007]
---

All code is written in ANSI C (C89/C90). Not C99, not C11, not C++, not Python, not anything else.

Why:
- C89 is the most portable, most stable, most universally supported language standard in existence. Code written in C89 today will compile and run identically in 100 years.
- It's turing complete — everything that can be computed can be computed in C89.
- Influenced by Casey Muratori / Handmade Network and Eskil Steenberg philosophy: own your tools, understand everything, no abstraction layers you don't control.
- Strict compiler flags (-pedantic -Wall -Wextra -Werror -Wconversion etc.) catch problems early and enforce discipline.

Implications:
- No variable-length arrays (VLA) in the C99 sense — use the xar library instead.
- No inline functions in the standard sense — use macros or static functions.
- No mixed declarations and code — all declarations at top of block.
- No // comments — use /* */ only.
- No stdint.h guaranteed — may need platform-specific type definitions.
- No complex.h or tgmath.h — implement any needed math from scratch.
- The latina.h header provides Latin macros for all C89 keywords, and all code is written using these Latin identifiers.
