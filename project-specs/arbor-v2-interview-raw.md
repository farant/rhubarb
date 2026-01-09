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
