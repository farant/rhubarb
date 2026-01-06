# arbor_ce_lxxxix - Interview Brainstorm

## Q: What feature are you thinking about building?

**A:** i want to make our own c parsing library

---

## Q: What's the primary use case driving this? Is it for syntax highlighting, code navigation, refactoring tools, or something else entirely?

**A:** Syntax highlighting, Code intelligence, Static analysis, Code generation, could be other uses as well, but i think having the code itself be a data format we can work with in a flexible way is important

## Q: Do you need to handle the full C89 spec including preprocessor directives, or a subset?

**A:** Full C89 + preprocessor

---

## Q: For preprocessor handling, should macro expansion happen during parsing, or do you want to preserve macros as unexpanded AST nodes?

**A:** probably keep both pieces of information somehow so you can have stuff kind of like source maps in some cases? i think you would have n expansions since macros can be nested but have something like a TokenOrigin data structure that tells you which n expansion the code came from? because basically you would have like a macro expansion tree right? i think you would want to keep all that data in some layer even if you end up with the canonical final ast because you might want to know that layer 3 of 5 uses a particular macro, etc

## Q: How should the parser handle your Latin macros (si, alioquin, redde, etc.)? Are these pre-expanded or should the parser understand them?

**A:** i think we will treat them as normal typedefs for now but at some point i think making them keywords that can be used even without a latina.h file could be useful as an optional configuration

---

## Q: Your macro expansion tree idea is interesting. Should each AST node carry its full expansion provenance, or should there be a separate side-table mapping source locations to expansion chains?

**A:** i could see inline on nodes or a side table. at some point we are probably going to have something like css selectors for querying ast nodes/substrings of the code. honestly i use pretty simple macros so it's not going to probably be used very much any time soon, but want to plan ahead for ways to make the c parser as useful as possible

## Q: For code intelligence (go-to-def, find-refs), do you need cross-file analysis or is single-file parsing sufficient for your initial use cases?

**A:** it will probably have a full project index

---

## Q: You mentioned CSS-like selectors for AST querying. Are you thinking something like tree-sitter queries, XPath-style navigation, or a custom DSL?

**A:** i'm thinking literally like css selectors except instead of tag names you have node types, use attribute syntax, maybe some other kind of special syntax for other search cases if necessary (like searching by text string or something). these means that i would like tag names to be lowercase-kebab-case if possible

## Q: For error recovery during parsing (incomplete or invalid code), what behavior do you need?

**A:** from what i understand it seems like treesitter has the approach here? so i would want to understand better but probably try to emulate their strategy

---

## Q: Tree-sitter uses GLR parsing with an explicit grammar DSL. Their error recovery inserts ERROR nodes and uses heuristics to resume. Do you want to write your own recursive descent parser, or use a grammar-driven approach?

**A:** Hand-written RD

## Q: What's your expected codebase size for indexing? This affects data structure choices.

**A:** i think it will probably be stuff where all the c files fit into memory, but rhubarb is probably at least in the 50-500 file range at this point

---

## Q: For incremental re-parsing when a file changes, do you need to re-parse only the changed function/block, or is full file re-parse acceptable?

**A:** Full file re-parse

## Q: Should the AST be mutable after parsing (for refactoring/transformations) or immutable (safer, enables sharing)?

**A:** i think it's okay to have the parsed tree be immutable but we might want to clone it/ do transformations? i think there might be a css selector/xml (or stml) based syntax for doing transformations on the ast

---

## Q: For AST transformations with CSS-like selectors, are you imagining something like 'match pattern → produce replacement', similar to sed/awk but structural?

**A:** pattern -> template but the template might be in a <tag> dsl that lets you have more flexibility about transformations. probably have special tags like <%this> that indicated some kind of mutation syntax on the matched "this" ast sub graph. so you can do <%append>...</append>, <%prepend></prepend> <%replace selector="...">...</replace> etc

## Q: What representation should the AST use for source text? This affects pretty-printing fidelity.

**A:** i think we will use string interning to some degree (we have a string internment library already) but pretty printing based on some specific formatting rules i like will definitely be one of the use cases. so we want to be able to do code -> parse -> new-code and have code==new-code, so whatever information we need to track to make that possible should be tracked

---

## Q: For roundtrip fidelity (code → parse → code == original), you need to preserve whitespace and comments. Where should these attach?

**A:** i could see leading/trailing or both could be fine. i'm not sure what the tradeoffs and considerations are here

## Q: Your transformation DSL with <%append>, <%prepend>, etc - should this operate on the AST level or the token/text level?

**A:** i think you would definitely be able to do ast, and i can see it being useful to drop into token in some cases so maybe both?

---

## Q: For trivia attachment: leading means comments before 'if' attach to the if-statement. Trailing means comments after '}' attach to that brace. 'Both' is most flexible but more complex. Given you want roundtrip, 'both' is safest. Any preference based on your comment style?

**A:** Both (safe default)

## Q: How should the parser handle platform-specific constructs you don't use (e.g., __attribute__, __asm__, MSVC extensions)?

**A:** Parse as opaque

---

## Q: For the project index (cross-file symbol resolution), how should symbol visibility work? C has complex rules with static, extern, tentative definitions.

**A:** i think full c semantics? at some point we might want to use this for linting so if something is breaking rules in some way it would be good to know.

## Q: Should type information be resolved during parsing, or kept as-is and resolved lazily during queries?

**A:** i'm not sure which one is most practical with the macro expansion approach we are taking so no strong opinion

---

## Q: With macro expansion tracking, two-pass is practical: first pass expands macros + builds symbol table, second pass resolves types with full context. Does that sound reasonable?

**A:** Two-pass sounds good

## Q: What node types do you consider most important to get right first? This helps prioritize the grammar.

**A:** Functions + structs, Expressions

---

## Q: For CSS-selector queries, what pseudo-selectors beyond :first-child, :nth-child might be useful for code? I'm thinking :has-return, :calls(fn), :modifies(var)?

**A:** Both + custom (Standard + semantic + user-defined pseudo-selectors)

## Q: Should queries be able to capture and return matched nodes, like regex capture groups?

**A:** capturing with something like $name sounds great

---

## Q: For the library API, do you want a streaming/callback style (parser calls you per-node) or a tree-building style (parse returns complete AST)?

**A:** Both modes

## Q: How should the parser allocate memory? This affects lifetime management.

**A:** i think whatever combination of long term and temporary piscinas make sense

---

## Q: What should the library be called? Following your Latin naming convention.

**A:** arbor_ce_lxxxix

## Q: Should this library depend on your existing string interning (internamentum) or be standalone?

**A:** i would say to use internamentum but if it uses internamentum it would also use the chorda string type probably

---

## Q: For go-to-definition in the editor, should the parser provide byte offsets or line/column positions?

**A:** probably both cached? honestly i can think of a world where part of the output of this is like a unity build or messing with translation units so it should definitely be aware of files but it will also be able to have enough metadata to compose things intelligently

## Q: How precise should error messages be? Full gcc-style with carets and context, or simpler?

**A:** i would say have utility to print in formatted way that takes error object as input and return error objects?

---

## Q: You mentioned unity builds - should the library have a concept of 'project' that manages multiple translation units and their relationships?

**A:** it should probably have a concept of a project or at least some kind of "filetree" structure that includes files and their relative paths to each other, but such file trees can probably be constructed and it can be run on single files with an implicit project of just that file? there would be utilities to easily make filetree/projects based on the contents of some folders or whatever

## Q: For the index/project, should it persist to disk or be rebuilt on each run?

**A:** i would say memory only for now and if performance is an issue we can work on caching

---

## Q: What output formats do you envision beyond in-memory AST? JSON, STML, or custom binary for tooling?

**A:** stml would be a logical output format, i think the main consumer for now will be widgets in the app, but at least one output format could be good as a form of pre caching. but would want round trip of code -> parse -> output -> load -> new-code where code == new-code to be possible

## Q: Should the pretty-printer be configurable (brace style, indent, etc) or enforce your single preferred style?

**A:** it will probably only be my style but that style might refine over time

---

## Q: For code -> STML -> code roundtrip, the STML needs to preserve trivia (whitespace/comments). Should the STML representation mirror the AST exactly, or be a simplified form?

**A:** i would say one-to-one, maybe configurable to not keep that level. but we can have trivia attributes or tags right?

## Q: Are there any specific C89 features you find particularly tricky that we should discuss handling strategy for?

**A:** i think we will just do our best to make it cover all these cases. obviously we are focused on c89 and maybe support some c99 stuff that is common but certainly nothing beyond that

---

## Q: Which C99 features would you want to support? These are common ones that are hard to avoid.

**A:** // comments, Designated initializers, Variable-length arrays, Mixed declarations, we would support these but definitely want to be able to flag them or query for them somehow because one of the use cases would be automated linting to make sure that we are not using any c99 features

## Q: Final question: What's the first concrete use case you'd want to build with this library?

**A:** current coloratio i believe is for the sputnik language. having that for c as well would be great. i think code formatter could be pretty useful too.

---

## Q: A few more details: should the parser track which C standard each construct belongs to, enabling queries like 'find all C99-only usages'?

**A:** Both (Nodes tagged + lint rules that use the tags)

## Q: For syntax highlighting, should the parser output semantic tokens (function-call vs variable-reference) or just syntactic (identifier)?

**A:** Semantic

---

## Q: Last few questions. For the recursive descent parser, do you want it to be re-entrant (parseable from multiple threads) or single-threaded is fine?

**A:** i think single-threaded is probably fine

## Q: Any concerns about the scope of this project? It's quite ambitious.

**A:** i would like to approach this as building up a series of modules that we can thoroughly unit test as we go. so one unit tested module at a time if possible. (obviously at a certain point you have to iterate on the core functionality composing together all the modules but i think organizing around modules that can be tested independently and composed is important)

---

## Q: What do you see as the core modules? I'm thinking: lexer, preprocessor, parser, type-resolver, index, query-engine, formatter, stml-serializer. Any additions or different breakdown?

**A:** that makes sense to me. if we need to go more granular we can but we can probably figure that out as we go

## Q: Any final thoughts, edge cases, or concerns before I write up the spec?

**A:** i would say ready for spec, but please create a -brainstorm.md file that has a raw literal transcription of our conversation before creating your -spec.md file
