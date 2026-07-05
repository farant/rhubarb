# Silva — positio (fresh-eyes description, landscape, and the seven opportunities)

*2026-07-04, post-M2-close conversation with Fran. Captured per the
raw-transcripts-precede-specs rule: the reasoning here feeds the roadmap
addendum in silva/phase-log.md ("recensio positionis"). Useful as onboarding
material and as the seed of any future public README.*

## I. What silva is, from scratch

**Silva is a lossless C parser that treats source code as a database instead
of a compilation input.**

A normal compiler front end is a demolition pipeline: the preprocessor
expands macros and discards the original text, conditional compilation
discards every `#if` branch it didn't take, the parser drops whitespace and
comments, and by the time you have a tree the file that produced it is
unrecoverable. Silva inverts that. Its load-bearing invariant: **nothing is
ever thrown away.**

- **Any bytes in, tree out, always** — real C89, half-written code, garbage.
  Re-emitting the tree reproduces the input byte-for-byte; unparseable
  regions become ERROR nodes that still hold their bytes. Enforced by test,
  permanently (fuzz + corpus + monorepo sweep).
- **The preprocessor is a recorded story, not a destructive pass.** Every
  expansion generation is a retained layer; every token carries a provenance
  chain (this `if` came from that `si`, defined in latina.h:26, invoked
  here). Pasting, stringification, includes keep their genealogy.
- **Both arms of every `#if` are retained** — compiled-out code exists in
  the tree (86% of real regions thread as structure; the rest as bytes).
- **Ambiguity is honest.** `foo * bar;` forks; both readings live in the
  tree; a canonical one is marked; when the oracle later learns the typedef
  the choice flips IN PLACE, no reparse.

Mechanically: an annotated STML grammar → generated conflict-preserving LALR
tables + tree constructors (every conflict cell individually named and
pinned — an unexplained conflict fails the build) + a GLR runtime.
Deliverable: SQLite-style single generated silva.c + vanilla-C89 silva.h,
zero dependencies, all internals prefix-renamed. The ~55 Latin genus tags
(DECISUS) are the permanent vocabulary that becomes the query language.

Current state at M2 close: the ENTIRE rhubarb monorepo (724 files) parses
with real include resolution at ZERO error nodes, byte-exact, ambient error
badge in the daily driver (saltuarius). What it deliberately isn't: a
compiler (no codegen/sema), a C99+ parser (robustness-lexed, lint-tagged
later), a style checker (conformance = future lint queries).

## II. The landscape

Existing C tooling splits into three families, each of which gave up
something silva refuses to give up:

1. **Compiler front ends (Clang/libclang, GCC, EDG)** — semantic gold;
   surface expendable. AST is not a fidelity object (no comments, no
   re-emission, no untaken arms), needs build context, and libclang means
   carrying LLVM. GCC won't be a library on principle.
2. **Editor-grade robust parsers (tree-sitter)** — silva's posture ancestor
   (any bytes → tree, never crash) but it does NOT run the preprocessor:
   macro-heavy C gets structurally wrong trees, typedefs are guessed.
3. **Source-document tools (srcML, Coccinelle, comby)** — fidelity without
   real parsing (unexpanded markup / deliberately heuristic C).

**Silva's position: an essentially unoccupied point in this triangle** — it
parses the EXPANDED stream (compiler-correct, oracle-resolved trees) while
every token chains back to layer-0 bytes (document-exact emission, layers in
between queryable). Compiler-correct trees AND document-exact surface in one
data model.

Sharper comps: **Roslyn** (C# full-fidelity trees with trivia — the model
silva imports; "C never got its Roslyn" BECAUSE of the preprocessor, which
silva makes a first-class recorded dimension); rust-analyzer/rowan and
SwiftSyntax (same model, macro-free languages); **TypeChef/SuperC**
(academic all-configuration parsing — the only prior art for both-arms;
research artifacts, not fidelity emitters); **Elkhound/Elsa** (GLR-with-
ambiguity-nodes precedent, dead); **CodeQL/Semgrep** (where quaestio is
headed; build-dependent / tree-sitter-limited respectively); distribution
comp = SQLite/stb/Lua, different species from libclang.

Honest gaps vs the landscape: clang's semantic depth (typus milestone is a
gesture, not a rival), C99/C11/GNU extensions (evidence-gated), API-fed
include resolution (host does setup clang automates), no incremental
reparse, ecosystem of one. Gaps of ACCRETION, not FORECLOSURE — the
predecessors' killer was foreclosure.

One line: everyone else chose between the file that was written and the
program it means; silva holds both at once, and for C nobody had built that.

## III. The seven opportunities (rough edges where a strength is 90% built)

1. **DIAGNOSTICS FROM THE TABLES.** Error nodes count and hold bytes but
   never explain. At GLR failure the state's action row already enumerates
   the acceptable terminals — "expected one of `;` `,` `)`" is baked data,
   unread. Surface the expected-set on ERROR nodes → infidelis output, badge
   popup. Moves silva from honest counter to honest counter that explains
   itself. Small; the info exists.
2. **MINIMAL SYNTHESIZED-TRIVIA POLICY.** The Phase-5 park ("stored trivia
   wins; absent emits nothing") conflated a placeholder policy with the
   whole formatter milestone. A default (single space; newline after `;`/`}`)
   unblocks FOUR parked items at once: layer-N text views (the most
   demonstrable form of "layers are the product"), node-INSERTING
   transforms, generated-code emission, silva.h generation. ~A day.
3. **THE COMMENT MODEL HAS NO OWNER** (audit repair — the 2026-07-04
   full-log audit missed it). silva/CLAUDE.md marks comments-as-queryable
   as THE most easily smoothed-over pin; it is absent from the post-M2 map,
   and the smoothing already started: the subscriptio banner fix hand-rolled
   comment trimming in a consumer because no comment model exists to ask.
   Needs a named landing spot (quaestio-adjacent).
4. **INDUSTRIAL FUZZING ON THE AMALGAM.** The totality+roundtrip oracle is
   the strongest checkable property in the codebase and exactly the shape
   libFuzzer/AFL want (single .c, no deps, binary oracle). 400 fixed
   specimens undersell it. One harness file, occasional overnight runs,
   harvest failures as fixtures. Tree-sitter's robustness reputation is
   partly that the world fuzzes it; silva can buy the equivalent with a
   script.
5. **STANDARD-TAGGING AT RECOGNITION TIME.** The token contract has carried
   a standard-tag since Phase 1; the lint pin promises "find all C99-isms is
   a query." The LL/ULL lexer change (M2d D1) was justified as "lint tags it
   later" — but the lexer KNOWS at the moment of consumption. Tag as
   constructs are recognized (pennies per site) so the lint layer's first
   query works on day one instead of needing a retroactive annotation pass.
6. **THE INCLUDE GRAPH IS COLLECTED AND UNREAD.** Learning-mode inclusiones
   records every edge with resolution status; the VISIO's header-graph
   feature is one percursus flag from a monorepo-wide dependency dump.
   Cheapest new instrument available.
7. **INCREMENTALITY FORECLOSURE CHECK** (analysis, not construction). The
   per-segment architecture is accidentally incrementality-shaped (edit in
   one segment invalidates one segment — tree-sitter's trick). Nobody needs
   it yet; but tokens carry ABSOLUTE byte offsets and consumers are pinning
   position semantics. One page of "what would offset-shifting require"
   BEFORE more consumers calcify it — so it stays a decision, not an
   assumption. This is the exact class that killed the three predecessors.

Meta: none of these are new scope — each is a built strength missing its
last 10% (a voice, a policy, an owner, a harness, a tag write, a dump flag,
a page of analysis). Items 1, 3, 7 should not wait for a pull: they are
respectively about trust, a VISIO pin drifting, and foreclosure risk.
