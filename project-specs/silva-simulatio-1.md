# Silva Simulatio ① — Token Contract + Expander Vertical

*Implementation simulation (2026-07-02), ~2.5 simulated hours. Complications numbered
S1+. Full session log in conversation; this file is the durable register. Follows the
arbor2 precedent (two sims, complications #1–27) that preceded phase 1.*

## Pre-sim research findings (verbatim code extraction)

- **R1 — arbor2's recursion guard is dead across layers.** `exp->macro_stack` is
  pushed/popped entirely within one `_expandere_macro` call (`arbor2_expandere.c:
  1672-1677, 1960-1964`), which never recurses — body re-expansion is deferred to the
  next layer, where the stack is empty. No token carries any non-expandable mark.
  `#define A A` and mutual recursion are bounded ONLY by the 100-iteration fixpoint
  cap (`:2232-2247`), producing up to 100 junk layers.
- **R2 — TabulaDispersa has no clone.** `Arbor2Segmentum.macros` says "snapshot" but
  no clone primitive exists anywhere in tabula_dispersa.h; the field stores the live
  table (confirming the expander report's `:632-635` caveat). COW segments as
  spec'd in silva-spec-v2 §3.2 would require a new hash-table primitive.

## Complication register

- **S1 — Trivia tokens inside the unified token.** If trivia are themselves
  SilvaTokens (Xar* spatia_ante/post), trivia tokens must not recurse (their own
  spatia = NIHIL, origo inherited/FONS). Macro-body comments survive in the body
  slice and re-emerge on expansion — needs an explicit rule for whether expanded
  output carries body trivia (decision: yes, body slice verbatim; stringify
  collapses per standard).
- **S2 — Provenance as explicit tagged union.** SilvaOrigo { genus; union { fons,
  expansio, pasta, chorda, api } } replaces first-member-aliasing downcasts. Easy.
- **S3 — File identity table.** Tokens carry a fons_index into a per-context
  SilvaFons table (path, source text ref) instead of chorda* via_file per token.
  API-injected macro bodies get synthetic fons entries. Def-site preserved because
  macro bodies are slices of the real file token stream; invocation site lives on
  the expansio origo.
- **S4 — initium_lineae is LOGICAL line start.** A token after a CONTINUATIO
  newline is not line-start (a `#` there is not a directive — matches C89 phase-2
  splicing semantics). Computed during trivia attachment.
- **S5 — Tokens are immutable and shared across layers; layer_index dropped.**
  Pass-through tokens are the same object in layer N and N+1. Layer membership =
  which layer Xar holds the pointer; expansion generation = provenance chain depth.
  Kills arbor2's meta/Layer duplication.
- **S6 — Trivia ownership by separate attachment pass.** Lexer emits a raw
  interleaved stream; a rule-driven attachment pass assigns spatia_ante/post
  (trailing = same-line trivia through first newline; leading = rest). Ownership
  becomes a testable pure function instead of scanner side-effects; unifies arbor2's
  two divergent ~120-line scanners.
- **S7 — CRLF: NOVA_LINEA valor is the verbatim terminator** ("\n" or "\r\n"), one
  token either way (arbor2 behavior kept).
- **S8 — Intra-token line continuations (THE lexer complication).** C89 splices
  `\<newline>` BEFORE tokenization: `#def\ <nl> ine` is a legal `define`;
  identifiers/literals/comment-markers can span continuations. Trivia-only
  CONTINUATIO (arbor2's model) cannot represent these. Decision: lexer splices
  while scanning; tokens whose text crossed a continuation carry an optional
  `scissurae` Xar (byte offsets of splice points, NIHIL for ~all tokens) so
  roundtrip re-inserts `\<nl>` exactly. Maximalist rule: model must not foreclose,
  even though the rhubarb corpus never does this.
- **S9 — Unterminated literals parse through.** New genus LITERAL_IMPERFECTUM to
  end of line + diagnostic; lexing continues (arbor2 silently `frange`d with no
  error genus). Also fix leading-dot floats (`.5`).
- **S10 — Journal replaces COW segments (design change to spec-v2 §3.2).** Forced
  by R2. Macro state history = append-only journal of events
  { fons, linea, genus(+define/−undef), def*, conditio_id }. `macros_ad_lineam`
  replays into a caller-arena scratch table. Segments become derived views;
  historically accurate (arbor2's never was); conditional-scoped events fall out
  free via conditio_id.
- **S11 — Directive extent via initium_lineae.** Directive = tokens from a
  line-start `#` to the next line-start token. Logical-line semantics means
  continued directives work correctly with zero extra code. (The arbor2 leak class
  is unrepresentable.)
- **S12 — Hidesets (caecatio) replace the dead macro stack (design change).**
  Per R1, strict layers need Prosser-style blue paint: each expanded token carries
  a hideset (interned macro names it must never expand as — parent's set + the
  macro just expanded), stored on the expansio origo (layer-0 tokens never have
  one). `#define A A` fixpoints at layer 1; mutual recursion terminates; the
  100-cap becomes an assertion, not semantics.
- **S13 — Argument pre-expansion order (semantic decision).** Pure strict layers
  substitute unexpanded args and let them expand in later layers — diverges from
  C89 (args fully expanded before substitution, except #/## operands) on
  self-referential edge cases, because post-substitution tokens inherit the
  invoking macro's hideset. Decision: each layer step performs one full C89
  expansion generation — args pre-expanded (recursively within the step, with
  their own hidesets) before substitution; `#`/`##` operands taken unexpanded per
  standard. Stringify correctness falls out. "Layer" = one rescan generation, not
  one naive substitution pass.
- **S14 — Journal events keyed by (fons, linea), not linea.** Full includes
  interleave lines from multiple files; `macros_ad_lineam` needs file context.
- **S15 — Layers are trees, not flat streams (design change).** All-branches
  conditionals with macro-affecting directives inside arms mean macro state forks
  per arm. Expansion must process arm bodies per-arm (arm-scoped journal events,
  condition-coupled); the layer stream contains CONDITIONALIS region nodes whose
  arms hold their own expanded sub-streams. Elegant convergence: expander regions
  ARE the parser's localized sub-parse units — one region structure serves both.
- **S16 — Boundary finder vs K&R definitions.** The `)`-before-`{` heuristic fails
  on `int f(a) int a; { }` (legal C89). Decision: extend heuristic (after `)` at
  depth 0, tolerate declaration-shaped tokens before `{`); add adversarial
  fixtures; note as standing pressure toward grammar-driven TU post-substrate.
- **S17 — Query APIs take a caller piscina.** Replay tables and other derived
  views allocate from a piscina parameter (scratch vs result is the caller's
  choice); rhubarb convention anyway.

## Design changes to fold back into silva-spec-v2

1. §3.2: COW segment snapshots → **event journal** (S10, S14); segments derived.
2. §3.2/expander: add **hidesets on expansio origo** (S12) — new correctness
   requirement with its own test family (direct/mutual/self-in-arg recursion).
3. §3.2: define layer step as **one full C89 expansion generation** with arg
   pre-expansion (S13); document the rejected pure-strict-layer divergence.
4. §2.3/§3.1: **layers are trees with CONDITIONALIS region nodes** (S15); region
   structure shared between expander and parser sub-parse units.
5. Token contract: add `scissurae` (S8), `fons_index` + SilvaFons table (S3),
   logical-line `initium_lineae` (S4), immutable shared tokens with layer_index
   removed (S5), LITERAL_IMPERFECTUM (S9).
6. Lexer plan: single trivia scanner + separate attachment pass (S6).
7. New vendored-lib need: either a tabula_dispersa iterator-based copy helper in
   silva's amalgamated internals, or (chosen) no clone at all — journal replay
   builds fresh tables (R2 resolved without new primitives).

## Open questions for Fran — ANSWERED 2026-07-02

*Interview transcript: silva-simulatio-1-interview.md. Resolutions folded into
silva-spec-v2.md §8.*

1. K&R: **full parse, tagged obsolescent** (standard-tag mechanism, lint flags it).
2. Layers-as-trees: **region nodes inline**; flattening iterator for flat consumers.
3. Hidesets: **immutable shared-tail lists** (Prosser shape, O(1) extend, sharing).
4. Layer semantics: **accepted — layer = rescan generation** (C89-conformant).
5. Continuations: **full scissurae in 1.0**.

The interview also settled the malformed-files posture (total lexer, pp
region-repair, hybrid cost-ready GLR recovery, caps + cancellation hook,
crash-freedom fuzzing from the walking skeleton) and recorded reformatting as a
first-class use case (comments are content, whitespace is style; style profiles as
data). See spec-v2 §8.2–8.3.

## What got EASIER than spec'd

- Directive detection: one flag (initium_lineae) eliminates the entire leak class
  and handles continued directives free (S11).
- Stringify: arg tokens are naturally unexpanded at `#` time in the layer model.
- Segments: journal is simpler than COW and more capable (S10).
- No tabula_dispersa clone primitive needed after all (R2 + S10).

## Verdict

No blockers. Two spec-level design changes (journal, hidesets), one structural
enrichment (layers as trees), one deferred-scope question (K&R). The unified token
contract absorbed every complication thrown at it — nothing surfaced that pressures
the tagged-value or single-owner-trivia decisions. Recommend folding changes into
spec-v2 and proceeding to Simulation ③ (annotated grammar → generation) next, since
S15's region nodes touch the construction layer.
