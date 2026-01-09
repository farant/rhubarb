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
