# Silva Simulatio ① — Post-Simulation Interview (Raw Q&A)

*2026-07-02. Interview on the five open questions from silva-simulatio-1.md plus the
malformed-file robustness topic Fran raised at invocation:*

> "one that i think gets kicked sometimes that should be addressed early on (and this
> might be something we have already designed well for, just want to make sure it's
> surfaced) is handling malformed files. the parser should not crash on badly formed
> files, i think treesitter is probably the best comp here."

---

## Q: K&R function definitions (`int f(a) int a; { }`) are legal C89 and broke the boundary finder's `)`-before-`{` heuristic (S16). What's silva's K&R support level?

**A:** Full parse, tagged obsolescent. K&R declarators go in the grammar (they're in
the standard's Annex A), parse to real nodes, and carry a standard-tag so lint can
flag them — same mechanism as C99 tagging. "Full C89" means full; the phase-2
external corpus will contain them.

## Q: S15 found that all-branches conditionals fork macro state per arm, so expansion output can't be a flat token stream. What shape are layers?

**A:** Region nodes inline. A layer's stream contains CONDITIONALIS region nodes
holding per-arm sub-streams (each with arm-scoped journal events). One traversal
structure; the expander's regions ARE the parser's localized sub-parse units.
Consumers that want flat iteration get a flattening iterator with a configuration
argument.

## Q: Hidesets (S12) put a set of interned macro names on every expanded token's provenance. How should they be represented?

**A:** Immutable shared-tail lists. A hideset is an immutable linked list; extending
= one new cons cell pointing at the parent's list. O(1) extension, massive
structural sharing, arena-friendly, membership scan O(depth) where depth is tiny.
The classic Prosser-implementation shape.

## Q: S13 redefined a layer as one full C89 expansion generation (args pre-expanded within the step), which is C89-conformant but compresses layer counts vs the naive strict-layer picture. Accept?

**A:** Accept: layer = rescan generation. C89-conformant expansion; stringify correct
by construction; every intermediate token still recorded in provenance chains —
nothing is lost, it's recorded on chains rather than as separate layers.

---

## Q: Intra-token line continuations (S8) — `#def\<newline>ine` is legal C89; the sim designed `scissurae` splice-offset metadata for byte-exact roundtrip. Scope for 1.0?

**A:** Full scissurae in 1.0. Lexer splices while scanning, tokens carry splice
offsets, roundtrip is byte-exact even for pathological input. One field plus lexer
logic; doing it now means the token contract never changes. The maximalist rule
applied literally.

## Q: GLR error recovery mechanism — tree-sitter keeps error-recovery forks alive in the GLR (skip/insert with costs, min-cost survives); silva's spec said boundary resync. Which?

**A:** Hybrid: resync now, cost-ready GSS. 1.0 ships boundary resync (discard to
`;`/`}`/line-start directive, error node with skipped tokens). BUT the GSS and
action dispatch are designed so recovery actions are just another fork kind with a
cost field — tree-sitter-style recovery stays structurally open as a post-substrate
upgrade. Maximalist data model, pragmatic implementation order.

## Q: Lexer totality — should EVERY byte sequence (binary garbage, NULs, invalid bytes, truncation) lex to a token stream that roundtrips byte-exactly?

**A:** Total + byte-exact always. The lexer is a total function: unknown bytes →
OCTETUS_IGNOTUS tokens, truncation → IMPERFECTUM genera, NULs fine (chorda is
length-based). Property test: ANY input roundtrips byte-identically through
lex→emit. Crash-freedom starts here; garbage in, garbage faithfully back out,
diagnostics alongside.

---

## Q: Preprocessor malformations — unterminated #if, orphan #else/#endif, garbage after directives, unterminated macro invocations. Policy?

**A:** Region-repair, everything parses. Unterminated #if → region closed at EOF
with est_imperfectum flag; orphan #else/#endif → error directive nodes in place;
garbage after a directive → directive node with error payload; unterminated
invocation → name treated as plain identifier + diagnostic (arg collection is
EOF-bounded). Every input produces a complete tree + diagnostics list. The
tree-sitter posture applied to the preprocessor.

## Q: Crash-freedom fuzzing (distinct from the deferred correctness fuzzing) — random/mutated bytes, assert no-crash/no-hang/no-byte-loss. When does it start?

**A:** From the walking skeleton. A small harness in silva/probationes: corpus files
+ byte-mutations + pure-random buffers through lex→expand→parse→emit, asserting
never-crash, bounded-time (caps as assertions), lex-level byte fidelity. Runs with
the test suite. Correctness fuzzing stays deferred as previously decided — this is
a different tool guarding a different property.

## Q: Resource bounds for hostile/pathological input (nested-region bombs, macro bombs, 100MB files) given long-lived solarium and possible MCP hosting?

**A:** Caps + cancellation hook. Configurable hard caps (include depth, region
nesting, expansion generations, GSS frontier, total tokens) that truncate
gracefully — IMPERFECTUM/error nodes + diagnostics, partial results always
returned. Plus a tree-sitter-style optional progress callback in SilvaContextus so
an interactive host can cancel a long parse cleanly. Deterministic (no timers),
single-threaded, embeddable.

---

## Q: Any final thoughts — anything else in the "gets kicked down the road" category, or discomfort with the sim's design changes?

**A:** i think that is great. there is just one extra use case that i would like to
document that is probably implicit or maybe mentioned somewhere but i missed it,
which is that at some point we will probably want to have "prettier" or
auto-formatting type stuff that will let you format code to specific styles. i
imagine that that can be built as a layer on what we have, but if it that relevant
at all to how we handle whitespace, etc. just want to make sure we are anticipating
that use case in case it was slipped through the cracks at all

**Resolution recorded:** reformatting = the roundtrip pipeline with generated trivia
substituted for stored trivia. Core requirement it places on the data model, now
explicit: **comments are content, whitespace is style** — comment trivia is
preserved and repositioned via the attachment metadata (own-line/end-of-line/etc.),
whitespace trivia is discardable and regenerable from style rules. Style profiles
are data (indent, brace placement, alignment), with silva's opinionated style as the
default profile — "specific styles," plural, upgrades the v1-spec single-style
stance. The formatter may also *generate* scissurae (line continuations) for long
macro definitions per style. New testable properties alongside the roundtrip
oracle: idempotence (format∘format = format) and semantic preservation
(parse(format(x)) ≡ parse(x)). Range formatting (solarium selection) is tractable
because the single-owner invariant localizes trivia regeneration to nodes in range.
Nothing in the current design forecloses any of this.
