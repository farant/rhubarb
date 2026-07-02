# Silva × Speculum — Implications Notes

*2026-07-02. Written after Fran shared the Speculum v0.5.3 spec (structured-thinking
superset of C89: philosophical declarations + C89 procedure bodies; stripper/
validator/inspector toolchain). Fran's addendum: **the actual syntax will probably
be based on STML** — the v0.5.3 line-oriented format (`--` comments, `body:`
regions, column-zero rules) is not the end state. These notes record what Speculum
implies for silva's design. Principle up front: silva stays a pure C89 substrate
with no Speculum-awareness; Speculum is a *consumer* through silva's public API.*

## 1. Speculum bodies are silva's snippet-parsing use case, made concrete

A `procedure` body is a C89 fragment whose context comes from OUTSIDE C: parameter
names/types derive from the Speculum signature via the as-C type map; runtime
types (`list_t`, `map_t`, `string_t`, `optional_t`) and failure macros (`OK`,
`FAIL`, `IS_OK`, `UNWRAP`) come from speculum-runtime, not from headers the
fragment includes. This is exactly the capability the original ce/v2 brainstorms
decided years-of-context ago: **parse a snippet with an implicit project +
API-injected macros/typedefs**. Speculum's validator level 4 ("the C function as
emitted would be syntactically valid; argument count matches") is a direct silva
client: parse the body with injected context, report diagnostics, no compiler
needed. The AMBIGUUS/parse-through-missing-info machinery covers the cases where
declarations live in another .spec file the validator hasn't loaded.

## 2. STML-based Speculum syntax makes the two layers one queryable surface

If Speculum v0.6+ is STML, then: the philosophical layer parses with the existing
stml library (raw-content tags — the `!` suffix machinery from STML extensions —
are the natural home for C bodies); silva parses the embedded C; and since silva
*serializes to STML*, both layers land in one representation. The killer usage
pattern this suggests for the query surface: **cross-layer joins** — "every
procedure tagged #parsing whose body calls piscina_allocare", "every movement
whose realizing procedure's body writes to disk", "procedures whose declared
Speculum signature disagrees with what the body returns". Selector design should
anticipate querying across an STML tree that CONTAINS silva trees. (Also aligns
with smaragda-as-knowledge-graph: speculum facets/tags + silva structure in one
graph.)

## 3. speculum-strip is morally another expansion layer

Stripping (.spec → .c) is a source-to-source transformation exactly like macro
expansion: tokens in the generated C originate from tokens in the .spec (signature
translation, hyphen→underscore, as-C lookups) or pass through verbatim (body
text). Silva's provenance model generalizes upward: a token's origin chain could
extend above layer 0 to "emitted from procedure tokenize in wordfreq-tool.spec".
No design work now — the SilvaOrigo tagged union is extensible by construction,
and the fons table can carry .spec sources — but this confirms the layer/
provenance model is the right shape for the whole toolchain, not just the
preprocessor. (This is the retrospective's "speculum/latina as a real language"
appendix item arriving on schedule.)

## 4. The inspector's query list is a requirements catalog for silva's query surface

`where X` (find-refs), `c-signature of P` (type resolution), `procedures in S`
(index by container), `apprehend X` (render one declaration with structure),
`tag-census` (facet aggregation), `gaps in X` (declared-but-undefined sweep —
compare silva's unresolved-include/unknown-typedef diagnostics). When the
selector-engine interview happens, walk this list as concrete acceptance queries
for the C layer. Note Speculum identifiers allow dots and hyphens and tags are
kebab-case — congruent with silva's Latin kebab selector tags; no impedance
mismatch at the naming layer.

## 5. The `complexity` declaration is the simulation register, formalized

`discovered-while / consists-in / consequences / handled-by` is a strictly better
schema than freeform register entries — it is what sim ① and ③'s S-entries already
are, unformalized ("per modum quia": knowing through effects, which is precisely
what an implementation simulation produces). Consider: future sim registers (② and
④) written with those four parts per complication; longer term, silva's worklogs
and registers are natural early Speculum documents once the STML syntax exists.

## 6. Boundaries to hold

- Silva ships no Speculum vocabulary, no .spec awareness, no runtime coupling.
  Everything Speculum needs arrives through existing public API: SilvaContextus
  injected macros/typedefs, snippet parsing with implicit project, STML output,
  diagnostics as values.
- Speculum's runtime types reach silva as injected typedef definitions the same
  way latina.h definitions do (compiled-in optional definition sets are already a
  contextus feature — a "speculum-runtime" definition set is just another one,
  supplied by Speculum's tooling, not baked into silva).
- The current v0.5.3 body-boundary rules (column-zero `--`, permissive keyword
  detection) are line-format artifacts that the STML syntax obsoletes — silva
  should NOT design against them.
