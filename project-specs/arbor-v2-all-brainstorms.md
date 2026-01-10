

#arbor-v2-brainstorm.md
# Arbor v2 Interview - Raw Q&A

## Expansion / Parser-Preprocessor Interface

**Q: How do you envision the parser querying the preprocessor mid-parse for expansion lookahead?**

A: There would be some kind of data structure that the "macro collection" pass generates that can be used when parsing layers so the GLR can say "do we know for sure that this token is a macro identifier yes/no" (doesn't mean it's not a macro, could be from an implicitly included or unavailable file) and then "what kind of tokens does this macro expand to" so the GLR parser can lookahead and make better predictions about what kinds of paths to explore.

---

## GLR Fork Handling

**Q: When the GLR forks due to ambiguity and later discovers the correct interpretation via macro expansion, what happens to the 'wrong' fork?**

A: We could let go of wrong forks once we are sure that they are not usable/valid. Since we are using a stack arena I'm not sure if there would be any kind of immediate cleanup though.

---

## Typedef Discovery Timing

**Q: For the 'typedef discovery through expansion' problem - when should typedefs become 'known' to the parser?**

A: There would be a typedef collection pass first of all, before any parsing, but the typedefs may not be complete because of implicit includes or macro expansion. So another module is kind of the macro expansion layer which is about tokenization (plus a minimal amount of parsing for macro arguments). So:
- Macro collection pass
- Iterative macro expansion based on collected macros of all text/token layers
- Typedef collection pass
- Best effort / GLR parsing of each of the text/token layers

---

## Layer Links / Cross-Layer Traversal

**Q: How do you envision storing/linking parallel parse trees? If layer 0 has `FOO(x)` and layer 1 has `x + 1`, how do queries traverse between layers?**

A: It's all tokens and trivia. Every macro expands to new tokens, so the expanded tokens would probably link back to the original "macro" token(s).

---

## Ambiguity Representation

**Q: What's your tolerance for ambiguity in the final result? What should an ambiguous node look like?**

A: The three kinds of "ambiguous" nodes we would have would be:
- Error nodes
- "Probably a typedef" node
- "Probably a macro" node

(For typedefs and macros that we can infer from positioning/other context that we don't have the definition of)

---

## Typedef/Macro Inference Heuristics

**Q: What heuristics for 'probably a typedef/macro' inference?**

A: Only positions that are valid locations for a type would be valid for typedefs. Typedefs would take precedence for the same identifier in a "type" position. If it's something like `MY_TYPE(x)` with an argument then clearly that can't be a typedef and must be a macro. If it is identifier with arguments in a position that cannot be a function call then it would be marked as a macro definition. Otherwise any other area where there is an identifier that there is not otherwise allowed to be a macro, or locations where there is incomplete/invalid syntax and then a macro that might be expanding to the missing syntax, we would consider to be a "probably macro".

---

## Transformation Flow

**Q: When a user wants to do a code transformation (e.g., rename a function), which tree do they operate on?**

A: You would modify the AST. But the AST has token slots and each token slot has trivia. So if you are changing the "function name" token in a function node in the AST you don't necessarily lose any formatting or trivia.

---

## Trivia Fate on Deletion

**Q: If a transformation deletes a node entirely, what happens to its trivia?**

A: You would probably keep leading trivia of first token and trailing trivia of last token. It might be worthwhile to have the ability to do "floating trivia" tokens - you might want to attach orphaned trivia to the trailing trivia of preceding token or leading trivia of following token.

Comments should not necessarily be handled as trivia - they should probably be nodes in their own right because we will want to be able to introspect and query for comments. But since comments can be interspersed within the tokens of other kinds of AST nodes they would probably either exist as a "peer" to AST nodes or a child to AST nodes with a location within that node's token slots.

---

## Nested Macro Expansion Chain

**Q: For nested macros (`A` expands to `B(x)`, `B(x)` expands to `x+1`), does the final `x` link to B's invocation, A's invocation, or both?**

A: Full chain.

---

## Comment Attachment

**Q: For `/* doc */ int foo;` - is the comment a peer before the declaration, or a child of it?**

A: This particular case could go either way, but probably because it is not inserted between any tokens of the statement technically this would be its own peer node to the declaration. You would want to have extra metadata or structure associated with comments. Comments on their own line are not the same as comments at the end of a line or at the beginning of a line or interspersed between tokens on a line. Multiline comments are different nodes/have a different attribute than single line comments.

---

## Comment Metadata

**Q: For comment metadata (own-line vs end-of-line vs inline vs multiline) - explicit fields or derived?**

A: Explicit fields. You would also want to probably have heuristics to explicitly attach comments to other nodes:
- If there is a comment on the end of a line, attach to preceding AST node
- If there is a comment without whitespace and then a line following (no blank line), attach to following AST node
- If there is a comment with a blank line above and below, that comment is a special kind of freestanding type

---

## Chain Size / Performance

**Q: Full expansion chains could get large. What's your memory/performance expectation?**

A: We want to solve the problem in a complete way first and then think about optimization. Lazy initialization could be a problem but I don't necessarily want to do optimization that shoots us in the foot for querying before we know the cost/need.

---

## GLR Memory Model

**Q: For the arena allocator model, how do you envision handling fork/merge?**

A: If it was technically feasible I think copy-on-write could make sense. But I could also see separate-from-fork being good. I haven't implemented a GLR parser before so I don't know the best technique/tradeoffs.

---

## Doc Comments

**Q: What about doc comment conventions like `/** ... */`? Special handling?**

A: We don't handle `//` so we only have C89 type comments. I could see special handling cases for `/* ====== \n .... \n ====== */` type comments. But this is an area where it would be nice to have some extensibility because you can imagine wanting to have extra patterns or even patterns based on parsing the body of the comment.

---

## Incremental Re-expansion

**Q: What triggers re-expansion? If file A includes B and B changes?**

A: From a parsing perspective we only care about typedefs and macros in other files. If a typedef or macro we are using changes, that could trigger a reparse. Or if a macro is defined that matches one of our "maybe this is typedef" or "maybe this is a macro" nodes.

It might be worthwhile to have an "identifier table" for each file so we can quickly see if we should reparse given new/updated macros.

---

## Identifier Table Tracking

**Q: How granular should identifier tracking be?**

A: Some kind of lookup / list that has an array of pointers to tokens or ASTs. Probably don't need full usage graph. But keeping track of all identifiers not just ambiguous ones is probably worthwhile.

---

## Comment Extensibility

**Q: For extensible comment parsing - plugin system or grammar extension?**

A: Not necessarily even a user extensible thing at this point, just from our own perspective not making any decisions that make it super hard for us to expand the types of comments going forward in our own parsing code.

---

## Index Timing

**Q: Should identifier index be built during parse or as post-parse pass?**

A: Fine to build during parse because it seems relatively coupled with parsing anyway.

---

## C89 Strictness

**Q: How strict on C89 vs extensions?**

A: I don't want the parser to completely fail to parse files with C99 constructs but definitely should flag violations. Definitely not doing anything past C99. Could allow for some common extensions. In our codebase we use things like `long long`.

In a perfect world we would also be looking at Eskil Steenberg's Dependable C at https://dependablec.org/ and figure out rules based on that document and flag things that are not in that either.

---

## Dependable C Integration

**Q: Should parser enforce Dependable C rules or just provide info for lint?**

A: Parser annotates (accepts but marks violations for lint layer to report).

---

## Layer Granularity

**Q: At what granularity do you want expansion layers?**

A: Layer 0 is source. Layer 1 is running all macro expansions on source. Layer 2 is running all macro expansions on layer 1, and so on until there are no more macro expansions - then that is your complete layers. These layers can then be tokenized with trivia. And then tokenized+source text layers with trivia is the input to the C parsing layer.

---

## Expansion Order

**Q: For iterative expansion, how do you handle macros that expand to other macro invocations?**

A: Strict layers - Layer 1 only expands what was in layer 0, new macro calls wait for layer 2.

---

## Query Layer Selection

**Q: How should the query API handle different layers?**

A: Just use different layers as appropriate for the use case, that's the strength of having them as available data. For queries you always use fully expanded layer except if the query includes macros in which case you would want to query across all layers.

---

## Macro Query Results

**Q: For queries involving macros across layers - what should results include?**

A: Include all the occurrences of FOO across all layers because no FOO / macro node can persist across multiple layers since each layer expands all its macros.

---

## Error Recovery

**Q: How far should GLR try to recover from unparseable input?**

A: Recovery + error node - Always recover, insert error node containing skipped tokens.

---

## Error Classification

**Q: Should parser attempt to classify what kind of construct failed?**

A: Best-effort classification. I could see including failed alternatives in particular parsing modes designed for a text editor but otherwise don't think they're necessary to keep.

---

## Memory Layout

**Q: What's your instinct for memory layout - single arena or per-type arenas?**

A: Single arena.

---

## Output Modes

**Q: What modes for different use cases?**

A: Presets + custom - Named presets (analysis, editor, transform) plus custom flag combinations.

---

## Cross-Layer Disambiguation

**Q: Should expanded layer info help resolve source-layer ambiguity?**

A: GLR branches can use "macro lookahead" to see what the expansion of the macro looks like, if they do have that definition (which of course we might not).

---

## Lookahead Depth for Function-like Macros

**Q: How deep should lookahead go for function-like macros with arguments?**

A: As far as I understand the arguments to macros are just tokens, so it seems like lookahead could do the expansion with tokens as much as that is helpful to the GLR deciding what branches to create.

---

## Project Scope

**Q: Single-file analysis or multi-file projects?**

A: It could be used for single files, file includes, whole project, or snippets. Configuration would be "here are extra folder or file paths that are being included." You could also include macro/typedef definitions that are implicitly a part of the snippet but not available to the parser otherwise.

It should resolve includes if it can, but if it can't it considers that to be an unresolved include. You might also create macro/typedef definitions for standard libraries so those definitions are generally available.

There could be suggestions where if it sees a FILE macro/typedef it says "this code might need to include ..." as extra diagnostics.

---

## Standard Library Definitions

**Q: Should stdlib definitions be shipped with parser as data files or compiled in?**

A: Compiled-in defaults, and then the code invoking the parser would also be able to pass in extra data when it calls our API if they choose to.

---

## Header Hints

**Q: Simple lookup table or header dependency modeling?**

A: Probably a header graph, although usage-based suggestions for C standard library identifiers could be useful too.

---

## GLR Implementation

**Q: Implement from scratch, adapt existing, or use parser generator?**

A: From scratch.

---

## Trivia Contents

**Q: Since comments are first-class nodes, what remains as 'trivia'?**

A: Probably whitespace and line info. Preprocessor directives would probably exist as their own nodes.

---

## Preprocessor Directive Node Position

**Q: How do PP directives fit into the AST?**

A: Interleaved siblings. I could see them persisting across layers because you might want to query them in relation to fully expanded code. Conditional compilation macros like #if would not be in the final layer, probably just layer 0.

---

## GSS Structure

**Q: Preference for how parse states and stack nodes should be represented?**

A: Need to research.

---

## Conditional Code Handling

**Q: If `#ifdef FOO ... #endif` and FOO is undefined, should that code appear in expanded layers?**

A: You would probably parse all possible conditional expansions and then when users query they would be able to set which conditional expansions they want to be true. That way you can query for function usage across other code paths.

---

## Initial Implementation Scope

**Q: Full C89 grammar from day one or start with subset?**

A: Full C89.

---

## Conditional Explosion

**Q: With nested #if/#elif, how to handle combinatorial explosion?**

A: Would need to know which conditions lead to which expansions because you would have certain configurations coupled based on the same condition. Would also want to keep track of any expansions that had typedef or macro definitions that might affect compilation elsewhere.

Otherwise you could safely parse them once usually without having to reparse everything. Obviously there are edge cases with syntax that we would need to think about. In theory could treat them just as a macro expansion since it is essentially a node expanding to more tokens.

---

## Project Status

**Q: Timeline/priority - rewrite, evolution, or experiment?**

A: Parallel experiment.

---

## Code Sharing

**Q: Share infrastructure with arbor or keep independent?**

A: Share infrastructure - Reuse piscina, chorda, xar, etc. - just new parser logic.

---

## Naming

**Q: Any naming thoughts?**

A: Probably just arbor-v2.


#arbor-v2-phase1-brainstorm.md
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


#arbor-v2-simulation-brainstorm.md
# Arbor v2 Simulation Interview - Raw Q&A

This interview was conducted after a 3-hour implementation simulation that surfaced 16+ complications in the arbor-v2 design.

---

## Include Processing

**Q: The simulation revealed tension between 'Layer 0 = original source' and 'build macro table from includes'. Should include resolution happen BEFORE layer 0 (so layer 0 is the combined translation unit), or should layer 0 literally be the main file with #include directives as tokens?**

A: I think resolving includes as much as possible makes sense before layer 0 but I don't know if you 100% merge the tokens - I think keep track of which file tokens came from. Might also be a conceptual difference between "incidental includes" which are just for typedefs and macro definitions and files meant to be fully parsed by user API invocation.

---

## Include Types

**Q: So there's a distinction between 'learning includes' (just extract macros/typedefs) vs 'full includes' (merge tokens). How should the API express this? Should it be automatic based on angle brackets vs quotes, or explicit configuration?**

A: I think when you invoke the API you are invoking it with a file, an array of files, or folders/globs. But maybe also have an "includes" option with a folder or files.

---

## Provenance Depth

**Q: When a macro in the 'target file' expands to tokens, those tokens need provenance. The simulation showed provenance chains can get complex with nested macros. Should provenance track just the immediate expansion, or the full chain A→B→C, or let the user choose?**

A: Well tokens could kind of be like a linked list where you can walk backwards and get the data based on previous tokens previous token, etc. I am not set necessarily on any implementation approach but the point is that you should be able to take any token and have a process to view whole chain / introspect as needed.

---

## Argument Expansion

**Q: For function-like macros like A(B) where B is also a macro, the simulation found ambiguity. In layer N, do we expand A with literal 'B' as the argument (B expands in layer N+1), or do we somehow coordinate? This affects how many layers a nested expansion takes.**

A: So either you would consider expanding macro arguments to count as a step or you would expand multiple macros in one layer as a special case for macro arguments where you expand arguments and then expand macro being invoked.

---

## Paste Provenance

**Q: Token pasting (##) creates new tokens that didn't exist in source. What's the provenance of `A ## B` → `AB`? It came from two parent tokens. Should pasted tokens have special 'merged' provenance, or just point to the macro that did the pasting?**

A: Merged provenance.

---

## Line Handling

**Q: The current arbor lexer doesn't emit NEWLINE tokens - newlines are in trivia. Directive boundaries need line info. Should arbor2 modify the lexer to emit newlines as tokens, use line number comparisons, or something else?**

A: Add NEWLINE token.

---

## Code Reuse

**Q: The simulation found the existing praeparator is tightly coupled - it does directive parsing AND expansion in one loop. For arbor2, should we extract directive parsing into reusable functions, fork the praeparator code, or write fresh?**

A: It's okay if we do a fresh implementation and have v1 as reference. The main thing that I think would definitely be worth using is all the unit test / integration test examples to make sure v2 can pass all the tests v1 does.

---

## Unknown Macros

**Q: When a macro is used but not defined (missing include), current arbor can still parse with 'probably macro' heuristics. For arbor2's layered model, should undefined macro invocations stay as-is across layers (never expand), or become special 'unknown macro' nodes?**

A: Yes, you would have explicit unknown macro and unknown typedef nodes I think that are based on heuristics. Typedef taking precedence over macro if it is ambiguous. This is kind of like error nodes where you want to be able to parse everything even with missing information.

---

## GLR + Unknown

**Q: The GLR parser needs to query the macro table mid-parse for 'expansion lookahead'. If a token MIGHT be a macro but we're not sure (unknown macro node), should GLR fork on both interpretations, or commit to one based on position heuristics?**

A: I think fork both. Obviously some identifiers would only be valid as macros due to their position so it would either be a macro or an error. But for unknown macros the paths that would be considered since it can be expanded might be something like "maybe it is this token", "maybe it is this collection of tokens" etc. where the expanded token itself would be missing some information. Like we could parse the macro as if it is a type definition or variable name but then we don't necessarily know what that actually would be without the definition. But a common case especially for us is "is this macro where I expect to see an if keyword or continue keyword or whatever, I will test if that's what it is".

---

## Keyword Macros

**Q: You mentioned keyword macros specifically ('si' might be a macro expanding to 'if'). Should arbor2 have special handling for 'keyword macro' patterns - where an identifier is known/suspected to expand to a keyword - or treat all macros uniformly?**

A: There will be the option for the user to have macro definitions (and typedef definitions) that they pass into the API when they are parsing. There would probably be built-in macro definitions that can be passed in this way for everything in latina.h. So most of the time GLR will actually have the macro definition for "si" or "alioquin" available to it. Otherwise I think treat it normally. But obviously when GLR is trying to explore what the definition of a macro is the paths it searches will depend on the surrounding context/position of the macro keyword which would prioritize certain searches more than others as a heuristic.

---

## Builtin Definitions

**Q: For the 'built-in latina.h definitions' - should these be compiled into arbor2 as data, loaded from a file at runtime, or provided via API like user definitions?**

A: This will probably be compiled-in data but there will be an API where the user could pass them in itself. So it might be a curried version of the main API, it might be a variable that has the data you can load from the library, or it might be an option you can set to include latina.h (or toggle to exclude them, etc).

---

## Fork Memory

**Q: The simulation found that piscina mark/reset could work for GLR fork handling - checkpoint before fork, reset if fork fails. But if a fork succeeds, its allocations stay. With many forks, memory could grow. Is this acceptable, or should we have fork-specific allocation pools?**

A: I think single arena with growth is fine if that is practical.

---

## Conditional Parse

**Q: When parsing conditional compilation (#if/#ifdef), the spec says parse ALL branches. But what if different branches have conflicting syntax (one has valid code, one has garbage)? Should we require each branch to parse independently, or allow one branch to be error nodes while another succeeds?**

A: Well you are basically forking the code, the question is just how localized can the code be. The use case of this (when we are not providing explicit arguments for the ifdef so there is a canonical resolve) is to be able to search for structures in the code and not miss things because they are behind an ifdef you didn't think of. So I would say some ifdefs are mutually exclusive (if/else) and some are necessarily linked if they are for example based on the same condition (which also gives them all mutual exclusion with anything that has mutual exclusion with any of their set). So I think you would try to figure out the minimum number of "parsing forks" that covers all their cases. If forks are totally independent of each other, we want to avoid combinatorial explosion. So instead of being complete to the whole codebase/translation units (unless it has typedef definitions or macro definitions in it) maybe the fork could be node-based?

---

## Condition Query

**Q: So conditional forks are 'node-based' - each #ifdef creates a local fork in the AST rather than a whole-file fork. When querying, how do you specify which condition configuration you want? A list of defined macros, or something more structured?**

A: You would have an API to query with certain conditions being defined/true (or false) and you could query with those conditions or maybe with a "wildcard" that would search across all ifdef. And maybe the wildcard would be the default behavior because I think it's most intuitive that if you give it a file or whatever it searches the whole file.

---

## Tree Sync

**Q: The simulation touched on the 'source tree vs semantic AST' duality. For roundtrip formatting, you serialize from source tree. But if the user modifies the semantic AST (rename a function), how does that propagate to source tree for serialization?**

A: Well honestly the semantic AST where each node has its constituent tokens and their trivia, in theory you should be able to (and probably should for testing purposes) do the roundtrip from the AST not the original source. So I think you would just be able to manipulate values or nodes and keep or create new trivia, whatever, then stringify it.

---

## One vs Two Trees

**Q: If AST nodes contain their tokens+trivia and you can roundtrip from AST directly, do we still need a separate 'source tree' structure? Or is the AST sufficient for both semantic queries AND roundtrip, just with different traversal patterns?**

A: Even if technically you can recreate the source tree for every layer from the AST on demand somehow, I think it is still helpful to have the source view already composed and available for all the layers. And this is not necessarily a string, it could be an array of tokens with their trivia.

---

## Error Granularity

**Q: For error recovery in GLR, when parsing fails and we create error nodes with skipped tokens - should recovery only happen at 'safe' boundaries (semicolon, close brace), or try to recover mid-expression with partial nodes?**

A: Configurable.

---

## Index Scope

**Q: The spec mentions building an identifier index during parse. Should this index be per-layer (separate index for each expansion layer) or unified across all layers (one index with layer annotations)?**

A: I think it would be a unified index because conceptually identifier resolution happens on a unified layer right?

---

## Incremental Update

**Q: When we parse and find what looks like a typedef usage but we don't have the definition, we create an 'unknown typedef' node. Later, if we get the definition (via incremental update or user providing it), should the AST auto-update, or is it a full re-parse?**

A: Well it seems like we would be able to do a pretty targeted update since we are not changing the type of any node just change attributes on nodes that match a particular query but whatever is practical.

---

## Paste/Stringify

**Q: The simulation showed complexity around ## (token pasting) and # (stringification). These are expansion-time operations. Should arbor2 fully implement these in the expansion layer, or defer them somehow since they're relatively rare in practice?**

A: I think as long as we could mark tokens as having multiple pasted tokens as their "ancestor" it should be fine? It seems like if we are thinking about things properly as token streams it should be manageable.

---

## API Shape

**Q: For the API, should arbor2_parse return a single 'result' struct containing everything (all layers, AST, indexes, errors), or should it return an opaque handle that you query for different parts?**

A: I think builder pattern could be good if appropriate. I think it would also generally return an opaque handle, although there might be a special API to get the complete result struct.

---

## Parse Modes

**Q: The existing arbor has PROCESSARE, PRESERVARE, and HYBRID modes. Should arbor2 have similar modes, or does the layered model eliminate the need for modes since you can just query different layers?**

A: Layers replace modes.

---

## Layer Compute

**Q: For performance, should we lazily compute layers (only expand to layer N when requested) or eagerly compute all layers up front? Lazy saves work if you only need layer 0, eager is simpler.**

A: I'm not sure when we would only need layer 0 since expanding layers means macro expansion which generally we need to do to semantically parse code completely so I think it would eagerly parse all layers.

---

## Comment Attach

**Q: Comments are first-class nodes with position metadata (own-line, end-of-line, etc). Should arbor2 automatically attach comments to adjacent code nodes using heuristics, or leave attachment as a separate query/operation?**

A: Auto-attach.

---

## Dependable C

**Q: The spec mentions 'Dependable C' annotations for flagging violations. Should these be computed during parse, as a post-parse pass, or on-demand when queried?**

A: I think post parse pass or on demand query is fine. Usually you would be looking for violations across all the parsed code.

---

## Test Strategy

**Q: For testing arbor2, should we create new test files from scratch, port existing arbor tests to verify parity, or use the existing roundtrip test files as a compatibility suite?**

A: We can build up new tests as we are developing to match the module decomposition and order of development. But at some point we should cover all the test cases as far as C snippets and round trip files that arbor v1 is testing against.

---

## First Milestone

**Q: Last question: The simulation surfaced 16+ complications. What's your priority for the first implementation milestone? Core expansion layers working, or GLR parser skeleton, or something else?**

A: I think we will try to think of it as decoupled layers that have an interface between each other and build/unit test layers one at a time as much as possible.

