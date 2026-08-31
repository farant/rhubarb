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

## 2026-08-26 (eve) — argument forms (spec §6.1, v1.5 step 2)

The engine argument map generalized to both call spellings. One
session-worth of design compressed into the map:

- `StmlMacroArgumentum` grew `arbores` (Xar of EXPANDED StmlNodus*;
  NIHIL = scalar). One map, both forms feed it, the body never
  distinguishes — the §6.1 equivalence lives exactly where the
  decree said it would.
- Consumption: the maximal run of argument-elements after a call is
  collected and SKIPPED via the new `_liberos_expandere` +
  `saltus` out-param protocol (the shared child loop, factored from
  the document walk, the body fill, and subtree-argument expansion —
  three call sites, one loop). Argument-elements never reach the
  expanded tree.
- Classification (the design-forcing case was fixture 6.1g):
  zero children = tombstone → consumed, absent, NOT in the map
  (NON_IMPLETUS fires for declared slots — §6.3's delete verb);
  one TEXT child = scalar UNLESS its whole text is `&@x;` naming a
  SUBTREE argument of the caller — then the forest THREADS
  (classification after reference consideration; without this rule
  forests could not cross strata and the scalar path would fire
  vitium VII on a legitimate hand-off);
  else = subtree, children expanded with the CALLER's context into
  an ephemeral wrapper element (stml_elementum_creare — it creates
  the liberi Xar; stml_liberum_addere refuses NIHIL liberi, so the
  known duplicare-superficialiter trap does not bite here).
- Fill: whole-text `&@n;` in child position → forest splice, fresh
  stml_duplicare per site (stored forest immutable). String
  positions (interpolated text / attr / transclusion valor) naming
  a subtree → vitium VII ARGUMENTUM_ARBOREUM via
  `_chordam_substituere` (gained a nodus param for the line); the
  literal stays and the END GUARD in stml_expandere fails
  successus — soft-set vitia can never pass silently. Splice into a
  parent-bound attr-element's children = also VII (text-only law).
- Vitium VIII ARGUMENTUM_GEMINUM: same name twice, any combination
  (inline dup check added to _argumenta_parsare too). Never
  last-wins.
- Nota-ordering caveat recorded in spec: calls inside argument
  subtrees register notas before the outer call's nota. Unpinned;
  revisit if a consumer cares.
- Trap: `_argumenta_parsare`'s new duplicate check called
  `_argumentum_invenire` before its definition — implicit-decl
  error. Forward declaration added. (The censor hook's one-line
  summary said "latina macro in name position" but the actual
  effusor output was the implicit call — always run the effusor
  before believing the hook's one-liner.)

All seven born-red fixtures went green on the first full compile.

## 2026-08-26 (nocte) — COMMUTATIO + optional slots (spec §6.2, v1.5 step 3)

First CAPS-Latin builtin shipped: value-driven arm selection in
definition bodies, plus the hidden contract change it forces —
optional slots `p="@p?"` with flow-narrowing.

Shape: `_commutationem_perscrutari` (collection: shape validation +
narrowing scope) and `_commutationem_implere` (fill: first-match +
arm splice). COMMUTATIO/CASUS/ORDINARIUS never reach the expanded
tree — structure machinery, same class as argument-elements. Both
sides skip comments between arms.

Decisions locked (spec §6.2 build block has the full list):
- `de` = exactly one whole `&@x;` ref, declared; it is the
  SCRUTINIZING reference and therefore exempt from narrowing —
  the one place an optional slot may be touched presence-agnostic.
  Handled by intercepting COMMUTATIO in `_corpus_perscrutari`
  BEFORE the generic attr scan (which would have fired XI on de
  itself — the subtle bit of the whole build).
- est AND non-nihil arms establish presence (est only matches
  present values); nihil and ORDINARIUS do not — ORDINARIUS runs
  for both present and absent, so refs to the optional slot there
  are XI at collection. Fill-safety falls out by construction: an
  optional ref only executes inside an arm whose selection proved
  presence, so `_chordam_substituere`'s defensive path stays
  defensive.
- Narrowing = `angustati` stack (interned chorda*) threaded through
  `_corpus_perscrutari`/`_chordam_perscrutari`; push/pop around
  presence-establishing arms; `_in_angustatis` pointer-compare.
- Subtree value meets est arm at fill = vitium VII (forest vs
  literal comparison); non-nihil BEFORE est arms matches first
  legally — presence-switching on forests works.
- Loculi list upgraded chorda* → StmlMacroLoculus {titulus,
  optionalis}; NON_IMPLETUS skips optional; tombstone composes
  (explicitly absent → nihil arm).

Vitia IX/X/XI minted. Seven new fixture blocks born red (three via
unknown enum identifiers), all green on first full compile — third
time today. The engine's seams (child-loop interception, one
substitution grammar, first-vitium-wins) are absorbing new verbs
without resistance, which is what the T1-T6 shape promised.

## 2026-08-27 — DISTRIBUTIO built (`stml_distribuere`, ronda IV → §6.4)

The second content projection: legere → expandere → distribuere →
consumer. Deliberately a SEPARATE entry point beside stml_expandere
— the engine's contract stays template-space (`@`) only. Mirror
shapes: StmlDistributioResultus / vitia BENE-MIXTA-MEMORIA /
first-wins / ctx struct, all copied from the expansion idiom.

Shape of the walk: `_dist_nodum` clones (shallow + fresh children
for elements, deep for the rest), then `_dist_dissolvere` decides:
no items → append; mixed → MIXTA loud; else each item becomes a
wrapper-typed element and the result RECURSES through
`_dist_dissolvere` itself — that recursion is what makes nested
anonymous lists flatten with the same type (the degenerate-PER
rhyme fell out of the structure rather than being coded).

Decisions worth remembering:
- item-wins is implemented as SUPPRESSION: wrapper attrs/prefix
  attr-elements named by the item are simply not copied; the item's
  own material rides along verbatim. The tombstone needs ZERO
  special code here — it suppresses by name like any override and
  stays in the output as explicit absence (§6.3 semantics do the
  rest via capere).
- distributed elements take the ITEM's trivia + linea (position
  identity) and the wrapper's type/attrs; item clausura flag wins.
- item children are MOVED, wrapper prefix attr-elements are CLONED
  per item (a shared node cannot have N parents).
- comments among items pass through positionally; TEXT among items
  = MIXTA (post-parse, inter-element whitespace is trivia, so a
  text node there is real content).
- MEASURED at close: arbor corpus has ZERO anonymous fragments
  (352KB doc, 0 hits vs thousands of named @-forms) — distribution
  is identity over real content today; no corpus pin, the gate
  follows the first consumer. (Also measured: my first "0 hits" was
  an empty file from a stale cwd — assert the sweep's own coverage
  before believing its zero.)

Fixture set: 12 blocks incl. the flattening rhyme, tombstone-delete,
wrapper-prefix carry-down, and both passthrough classes. One fixture
bug found by the suite: I authored `</#f>` as a closer — fragment
closers never carry the id; the "0 vitia" received bytes were the
parser faithfully preserving my malformed input.

## 2026-08-31 — stml_congruere_strictum: the matcher extraction (exemplaria build step 1)

`_par_congruere` + its three helpers promoted VERBATIM out of
silva_arbor.c into the engine (spec: stml-exemplaria-spec.md §4/§9
step 1). Extraction moves, never improves: logic byte-for-byte, only
ownership renamed — `scriptor` dissolved into explicit
`piscina` + `intern` params; `ParCaptura` → public `StmlCaptura`;
`FoliumPar` (par path) → public `StmlCongruentiaPar`. One behavioral
widening: `paria` may now be NIHIL (pair recording skipped) — the
arbor caller passes its Xar as before, so its path is unchanged.

Contract carried in the header: capturae appended in discovery
order; repeated loculus = forests-equal (regula V — non-linear
captures ARE equality); pairs appended on success, the CALLER
truncates on failure (xar_truncare to its own mark) — the matcher
never rolls back.

Gates: silva arbor 5/5 (the plagula gate — 78+155 octetim exactae +
compression presence — green through the engine matcher, which is
step 1's definition of done), root stml + canon suites green, silva
canon corpus 2/2, amalgam VERIFICATUM (standalone + hospes 40/40 +
nm-intersection 0). Generated amalgam manifests UNCHANGED — the new
symbol is used from birth, so the excludenda harvest never saw it as
dead.

Unification note for step 2: `_congruentia_referentia_tota`
duplicates `_loculum_invenire`'s whole-reference grammar — the two
copies of that law now at least live in ONE file (the arbor comment
used to say "lex una, sedes altera"). Unify when loose mode lands,
not before.
