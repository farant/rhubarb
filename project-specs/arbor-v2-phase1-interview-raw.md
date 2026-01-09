# Arbor v2 Phase 1 - Simulation Interview Raw Q&A

This interview was conducted after a second 3-hour implementation simulation of Phase 1 (Token Infrastructure + Lexer + Layered Expansion) that surfaced complications #18-27.

---

## #undef and Expansion Model

**Q: Complication #25: #undef breaks the 'collect all defines, then expand' model. The simulation proposed order-sensitive expansion where we update the macro table AS we process tokens. Does this feel right, or do you want a different approach?**

A: Probably segment-based, which would to some degree be calculated in an order-sensitive way is the right approach.

---

## Macro Body Storage

**Q: Complication #18: Macro body storage. The simulation decided to store macro bodies as ArborLexema* (not Arbor2Token*) since provenance is added at expansion time. But this means the macro body itself doesn't track where it was defined - that info is in Arbor2MacroDef.via_file/linea_def. Is this separation acceptable or should the body tokens carry their own provenance?**

A: I think that we want to keep track of where and how macros were defined in the source file(s). But this data is not always available because some macro definitions might be supplied through the API by library user, in which case it would not have this information.

---

## API-Injected Macros

**Q: So macro bodies should track their definition location when available, but handle API-injected macros gracefully (no source location). For API-injected macros, what should the provenance say when expanded tokens trace back to them?**

A: I think in some cases the user providing them through the API would also provide what header file / include path to associate them with. In which case this would be shown. Otherwise API or something is good.

---

## Arguments and Newlines

**Q: Complication #20: Argument collection for function-like macros. The simulation included NOVA_LINEA tokens in collected arguments. But this means multi-line macro invocations like FOO(a,\n    b) will have newline tokens in the argument. When stringifying (#) or pasting (##), should these newlines be preserved, collapsed to space, or stripped?**

A: Preserve in collection, normalize per-operation:
- Keep NOVA_LINEA in the argument token list during collection (preserves source structure)
- For normal substitution: tokens substitute with their trivia intact
- For stringify: collapse whitespace/newline runs to single space (per standard)
- For paste: paste token values, ignore intervening trivia/newlines

---

## Invalid Token Paste

**Q: Complication #22/#23: Token pasting edge cases. If ## creates an invalid token (like pasting '>' with '>' to get '>>' which is valid, vs pasting 'int' with '3' to get 'int3'), the standard says it's undefined behavior. What should arbor2 do?**

A: Best-effort lex.

---

## Stringification Precision

**Q: Complication #24: Stringification (#). The standard requires preserving argument whitespace and escaping quotes/backslashes. The simulation noted this is complex. How precise does arbor2's stringify need to be?**

A: The standard for stringify (#):
- Each sequence of whitespace (including newlines) in the argument becomes a single space - it does NOT preserve original whitespace
- " and \ characters in the argument get escaped (\" and \\)
- Leading and trailing whitespace of the argument is removed

So "simplified" actually IS the standard behavior.

---

## Circular Include Detection

**Q: Complication #26: Circular include detection. The simulation proposed tracking processed files in a hash table. Should this be per-parse (reset each arbor2_parsere call) or persistent across calls on the same context?**

A: Probably configurable? But don't have a strong opinion about it.

---

## Typedef Detection Errors

**Q: Complication #27: Typedef detection from context includes is heuristic-based (pattern matching, not full parsing). The simulation accepted this for header scanning. But what if the heuristic gets it wrong - false positive (thinks X is typedef, isn't) or false negative (misses a typedef)?**

A: The API does let the user explicitly declare typedef. My understanding is that it is a technique the GLR uses to decide which paths it follows. I think there are cases where it is ambiguous what is correct which is fine, but we don't want to make mistakes in cases where it is unambiguous what is correct.

---

## Line Continuation Handling

**Q: On the line continuation issue (Complication #16/17): the simulation deferred this since the base lexer doesn't handle backslash-newline. For Phase 1, is it acceptable to have line continuation as a known limitation, or should we address it now?**

A: I think we would want to handle this properly. Although there would probably be some kind of trivia / metadata or something so that you can regenerate the backslash from the token stream if you are doing roundtrip parsing -> re-output without changes.

---

## Line Continuation Implementation

**Q: So line continuation should be handled with metadata preserved for roundtrip. Where should this happen: the base lexer (modifying arbor_lexema.c) or the arbor2 wrapper? The base lexer is shared with arbor v1.**

A: I am not attached to keeping the base lexer. If it is cleaner to fork and create an arbor v2 lexer that is not shared then that's what we should do.

---

## Segment Visibility

**Q: On the segment-based expansion model you chose for #undef: conceptually, a segment is 'the macro state from this point until the next #define/#undef'. Should segments be explicit data structures that users can query, or implementation detail hidden behind the layer abstraction?**

A: I would expect that you would want to make this queryable in some way.

---

## Query Granularity

**Q: For querying macro state at a position: should this be 'what macros were defined at token X' or 'what was the macro table state at source line Y'? Token-based is precise but requires the parsed token; line-based works before parsing.**

A: I would say token-based or line-based probably but ultimately is the granularity line-based or can you have #undef as just one thing among others on a line?

(Interviewer note: Directives are line-terminated in C, so line-based granularity is sufficient.)

---

## Segment Merging

**Q: One more question on segments: when a segment has no #define/#undef changes, should it still be a distinct segment object, or merge with the previous segment?**

A: Only on changes. Segments only created when #define/#undef occurs. Most files have 1 or few segments.

---

## Lexer Fork vs Wrapper

**Q: The simulation found that the arbor2 lexer wrapper needs to track 'previous line number' to compute the gap for NOVA_LINEA generation. Is there any other state the wrapper needs to maintain beyond: base lexer, buffered token, pending newline count, previous line?**

A: It's hard to say, but I would say probably we don't want the arbor2 lexer to be a wrapper. I imagine we might want to just take the existing code in a new file and then start modifying as a v2 lexer if I understand what you're asking correctly.

---

## Confirm Lexer Fork

**Q: Forking the lexer: so arbor2_lexema.c would be a copy of arbor_lexema.c with modifications for NOVA_LINEA, line continuation handling, and any other v2-specific changes. The original arbor_lexema.c stays untouched for v1. Confirm this is the direction?**

A: Yes, full fork.

---

## Token Paste Provenance

**Q: On provenance for pasted tokens (##): the simulation proposed Arbor2OrigoPasta with sinister/dexter pointers to both source tokens. The 'primary' provenance chain follows sinister (left token). Is this arbitrary, or is there a better choice?**

A: I think you could have a primary chain but you would want to link to the chain from both tokens right? Left is fine for "primary" if that is applicable.

---

## Null vs Always Allocate

**Q: On the memory allocation pattern: the simulation decided source tokens (layer 0) have origo_token=NULL and origo_meta=NULL to save allocations. But this means checking for NULL before accessing provenance. Alternative: always allocate origo_meta, just mark it as ARBOR2_ORIGO_FONS. What's your preference?**

A: Always allocate.

---

## Summary of Key Decisions

| Topic | Decision |
|-------|----------|
| #undef model | Segment-based, segments created only on #define/#undef changes |
| Macro body provenance | Track when available; API-injected can specify associated file or use 'API' marker |
| NOVA_LINEA in args | Preserve in collection, normalize per-operation (collapse for #, ignore for ##) |
| Invalid ## paste | Best-effort lex the concatenated text |
| # stringify | Standard behavior (collapse whitespace, escape quotes/backslashes, trim) |
| Include tracking | Configurable per-parse vs persistent |
| Typedef errors | API override available; avoid unambiguous mistakes |
| Line continuation | Handle properly with metadata for roundtrip |
| Lexer approach | Full fork of arbor_lexema.c to arbor2_lexema.c |
| Segment query | Make queryable (line-based granularity sufficient) |
| Paste provenance | Link both tokens; left is primary chain |
| origo_meta allocation | Always allocate (no NULL checks) |
