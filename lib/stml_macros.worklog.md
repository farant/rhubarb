# stml_macros worklog

## 2026-08-25 — birth (T1-T6 of stml-macros-plan.md, one session)

Design lineage: stml-visio.md §9.1 (decreed pins) →
stml-macros-interview.md (four rounds, raw) → stml-macros-spec.md →
stml-macros-plan.md. Consumer: arbor (T7, pending).

What shipped: `stml_expandere` — parameterized-fragment expansion
as a pure tree→tree projection. Definitions (`<#id attr="@loculus">`)
collected in document order and dropped from the content view;
calls (`<<#id p="v">>`) splice filled body clones; six loud vitia;
provenance side table in splice order; document-order strata with
the resolution ceiling = containing definition's collection index
(strictly decreasing → termination by construction).

Discoveries worth keeping:

- **The transclusion lexer already carried arguments** — the whole
  interior lands in `valor`, so the call syntax needed ONE lexer
  change (quote-aware `>>` scan, T1) and zero writer changes.
- **`stml_duplicare` existed** (public deep clone) — the engine is
  built entirely on the public stml.h surface, which doubles as
  proof that surface is sufficient.
- **Shallow duplicare leaves `liberi` NIHIL** — the walk creates
  the child Xar itself. Easy to forget; will bite the next person
  who clones-then-appends.
- **Grammar unity forced threading** (T5): one `&@nomen;` scanner
  serves collection judgment AND fill substitution. Substituting
  transclusion valors uniformly means an inner call's arguments
  are substituted BEFORE parsing — cross-strata argument threading
  fell out free, superseding the plan's "inner args stay literal"
  clause. Undeclared references still fail loud at collection
  (fixture c: LOCULUS_IGNOTUS; fixture c2 pins threading).
- **Nested fragments are OPAQUE both ways** — quoted definition
  material: not scanned at collection, cloned verbatim in fills.
  Their `&@` refers to their own future loculi.
- **Nota order = splice order** (spec: "ordine splicis"), achieved
  by appending the nota BEFORE the fill and setting `nodus` after —
  legal because Xar cell pointers are stable across appends.
- **The `nomen` landmine fired (7th recorded)**: used it as a
  parameter name; it is the `typedef` macro. `titulus` per the
  standing substitution.
- **Unknown-entity asymmetry is doctrine**, not a bug: `&@x;`
  outside a body roundtrips as `&amp;@x;` ("circuitus mutat,
  sensus non" — stml.h strictum). Fixtures must compare against
  the original's own write, not a byte literal.

V1 non-goals (reserved with landing spots in the spec §6):
children-args/spread, query fragments, cross-document libraries,
selector resolution, canon gating of loculi. Call detection reads
the ORIGINAL valor — a transclusion whose valor only becomes
`#`-leading after substitution stays a passthrough node (contrived;
noted, not handled).

Gates at birth: probatio_stml_macros 92/92 (20 fixture groups, all
born red first); root suite 141/141; formator CONFORMIS on all
three files; no amalgam duty until T7 makes silva a consumer.

## 2026-08-26 — Template-space split (`#@`): the first consumer immediately corrected the design

T7 (arbor slice) began with the planned "loader-first, macro-free
passthrough" step and instantly hit a wall the plan had not seen:
**arbor documents are not macro-free.** Every corpus document with
ambigui carries `<#lexN>` + `<<#lexN>>` — the shared-lexeme identity
machinery ("identitas res est, duplicatio mentiretur"). Blanket
`stml_expandere` would have (a) DROPPED each `<#lexN>` definition —
which sits at the first USE site, i.e. it IS content — deleting the
lexeme, and (b) replaced identity pointers with clones.

Root cause, named with Fran: one surface, two opposite semantics.
Bare transclusion is ALIASING (Nelson's transclusion: one object,
many windows — identity is the payload); a macro call is
INSTANTIATION (fresh copy, divergent under arguments). The
interview's "zero-arg call = plain transclusion resolution, same
apparatus" conflated them — plausible because our pure-projection
design makes macros reference-like at the FILE level; the difference
only appears at the TREE level when a consumer asks "same object?".
Arbor is the first consumer that asks.

Fix (Fran chose the sigil): template-space ids begin `@` —
`<#@post-spatia>` defines, `<<#@post-spatia n="1">>` instantiates;
the `@` is PART of the interned id. Content fragments and their
transclusions pass through the engine untouched. Surgery was two
predicates (`_est_definitio` requires leading `@`;
`_est_vocatio` requires `#@`) + a one-`si` lexer extension
(`_tok_legere_fragmentum_id` accepts leading `@`). Nested-opacity
consequence, intended: a CONTENT fragment inside a template body is
now scanned/filled like ordinary content; only nested TEMPLATE
definitions stay quoted.

Fixtures: three born-red passthrough groups (content fragments
survive; `<<#solus>>` without definition is NOT a vitium; mixed
template+content doc — the arbor case itself); all prior fixtures
re-spelled `#@` (engine one day old — cheapest re-spell there will
ever be). Corpus pins in probatio_silva_arbor_plagula /
probatio_silva_canon_corpus bumped 154→155: first silva-suite run
since lib/stml_macros.c was born; the gate found the new file and
judged it 155/155 pure.

## 2026-08-26 (later) — ante-spatia + folia macronum shipped; the corpus found the cap bug

Two more arbor shapes on the v1 engine (see silva/phase-log.md same
date): the ante mirror, and the m-leaf compressor (repeated
expansio-carrying lexeme elements hoisted to `<#@m-<macro>>` head
definitions, zero-arg calls, first-occurrence MOVED as definition
body, sedes paria repointed via parallel-subtree map). Engine
unchanged — both shapes ride T1-T6 machinery as-is; nested calls in
definition bodies (m-bodies contain `<<#@post-spatia>>`) worked by
construction via document-order strata.

Trap recorded: id-collision suffixes need a PER-NAME COUNTER, not a
capped scan — grammar tables invoke one macro 100+ times with
distinct arguments; the capped loop reused `-99` → GEMINUM in 3 of
155 files. The corpus gate caught it within the hour; the unit
fixtures never could (no fixture has 100 shapes of one macro).
