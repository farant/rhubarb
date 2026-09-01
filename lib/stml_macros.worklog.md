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

## 2026-08-31 — EXEMPLAR/PER/TRANSPARENTIA: gradus II built (loose mode + the bridge)

The §2-§5 surface of stml-exemplaria-spec.md, in the engine
(interleaved: EXEMPLAR resolves during the one expansion walk,
document level only — inside a fill context all three CAPS raise
vitium XIII, the named @arg door). 51/51 unit fixtures
(probatio_stml_exemplaria.c); all prior suites + plagula gate +
materia + amalgam green.

**THE FINDING: "partial tree = content above" was FALSE until
ADNEXIO PRAECOX.** The walk attached a child to its parent only
after the child's whole subtree finished, so the expanded root's
spine above the current position was unlinked — every floating
match over radix_expansa saw an empty tree (15/51 red, all
zero-match shaped). Fix: _expandere_nodum now attaches the shallow
duplicate BEFORE walking its children (extra param parens_novus;
same final tree, only intermediate visibility changed; failure
paths discard the whole expansion so partial attachment is
harmless). The strata law now falls out of the implementation for
real, not just in prose.

**Two lexer blessings in lib/stml.c** (the `.`-tag / `@`-attr
precedent, both narrow):
- `*` as a COMPLETE tag name (`<*/>` wildcard) — only when NOT
  followed by a name character; attributes never reach it.
- `$` leading an ATTRIBUTE NAME (`<p $n/>` node capture) — only
  when followed by a name-initial character; the sigil becomes part
  of the name. Trap met on the way: writing `<*/>` inside a C
  comment ends the comment at `*/`.

**Implementation laws set at the edges** (spec-consistent, decided
here, worklog is their record):
- Loose child matching is GREEDY ordered subsequence — each pattern
  child consumes the FIRST matching effective candidate child; no
  backtracking across choices. Failed trials truncate their
  ligamina (caller-truncates discipline, as the strict matcher).
- Effective children (content view): content fragments DISSOLVE
  in place; TRANSCLUSIO skipped (identity counted at definition);
  attribute-elements skipped; TRANSPARENTIA-declared tags and
  @-space skipped whole. Floating descent uses the same skips.
- Candidate attr matching reads INLINE attributa only (capere dual
  view not consulted); a="" (valor NIHIL) never satisfies a value
  capture — written = required means a present, valued attribute.
- Hard vs soft: XII-XVII and XIX are HARD (walk stops — the
  transactional reading of unum/optional); XVIII
  (EXITUS_NON_CONSUMPTUS) is the end-guard, soft.
- PER = the bridge literally: ligamina → StmlMacroArgumentum map →
  _liberos_expandere over the inline body (stratum+1), or over a
  definition body for voca="#@f" (with the full loculus judgments +
  a provenance nota per instantiation, exactly like a call). Node
  captures splice as CLONES (PER is the copy consumer).
- modus="primum" truncates the relation to one row; absent modus =
  omnia; matching runs over the EXPANDED (content) view, so
  template-compressed material matches in its resolved form.

## 2026-08-31 — L1: the first derived report document (exemplaria step 4)

`silva/probationes/fixa/exemplaria/conditio_nihilum.stml` — four
lines of STML: TRANSPARENTIA + a two-stage chain (all <conditio/>
wrappers, then NULL identifiers within each row via de=) + a
<relatum> built by PER. The harness
(probatio_silva_exemplaria_lint.c) concatenates each corpus file's
arbor document with the lint text and runs ONE stml_expandere —
the whole lint is a single pure call, as the interleaved decree
promised. Differential gate: per-file site-count equality against
an independent silva-side walk. **GREEN over 233/233 files (78
plain + 155 latinized), 3,639 sites both ways, apparatus gate
clean, invariant asserted (zero EXEMPLAR/PER/TRANSPARENTIA
elements survive into any expanded tree).**

**The gate's first catch was a SEMANTICS question, not a bug.**
One file diverged by one (lib/stml.c, lint 147 / oracle 146). Not
shared-token identity (the suppression trace printed nothing) —
NESTED conditio scopes: a NULL inside a TERNARY's conditio locus
inside an enclosing condition. The lint's per-row semantics
legitimately observe it twice (it IS inside two conditions — two
rows of $conditiones contain the element); the oracle had counted
per distinct token. Mirror fix: count `+= altitudo` (one per
enclosing conditio scope) at a token's first appearance.
Measured: exactly one such site in the whole corpus, genus
ternarius (named by the walk itself, not guessed). The probe
lineage repeats: the differential disagreement was worth more
than the count — it forced the lint's meaning to be stated
precisely.

Diagnosis seams kept, env-gated: LINT_INDAGO=1 prints suppressed
shared tokens, nested-scope sites (with genus name), and
first-use coordinates. Note for readers: an expanded token's
b/linea are DEF-SITE coordinates (latina.h line 45 for every
NIHIL) — useless for locating the source site; that is the §1.3
extent-provenance obligation talking.

v1 status per spec §10: L0 identity ✓ (step 1), engine surface +
vitia fixtures ✓ (step 2), L1 derivation ✓ (this). REMAINING:
canon vocabulary for the CAPS builtins so canon judges the lint
documents (§6) — the one open §10 item. CATENA is now unblocked
(its precondition — the lint in explicit-name form — exists).

## 2026-08-31 — CATENA shipped (the first post-v1 increment)

The full arc landed in one session: four residues ratified at build
start (tee RESERVED vitium-loud; wrapper carries de=/output= under
EXEMPLAR's law while links carry neither but may carry modus=; `<(>`
is permitted decoration, never load-bearing; canon CAPS row
same-pattern), then engine, fixtures, canon, and the differential
gate. Commits: 88fe15d7 (decree) → 23bdc532 (engine) → a77b0055
(fixtures, 96/96) → 9b46902f (canon, 385/385) → 5359c5e2 (THE GATE:
conditio_nihilum_catena.stml byte-identical relatum vs the named
form, 233/233 plagulae, 3691 sites — which now include the CATENA
engine's own conditionals, the corpus having grown under the lint).

Build findings worth keeping:

**The conveyance refinement crystallized during the design trace,
before code.** "Link expands first" cannot mean running the normal
expansion path over literal links: a literal EXEMPLAR link must be
used RAW (like a document-level EXEMPLAR's forma — patterns are
never macro-expanded), while a macro-PRODUCED link gets full
instantiation semantics (loculus substitution, nested calls — that
is what instantiation means; fill-vs-quote is a distinction the
language already had). Consequence: ctx->applicatio wraps ONLY the
invocation expansion in the link collector, and the interception's
conveyance check (EXEMPLAR/PER/TRANSPARENTIA/CATENA fall through to
plain-element expansion when applicatio > 0) is the entire vitium-
XIII refinement. Literal links never touch the flag.

**`<(>` in chain position nests.** The anonymous capturing fragment
auto-closes at parent close, so the second `<(>` sits INSIDE the
first — but recursive fragment dissolve preserves link order
exactly, so Fran's original sketch spelling works verbatim with
zero new lexer machinery. The gate document uses it deliberately.

**One engine subtlety: pointer threading makes the desugaring the
mental model only.** Stage N's congruentiae Xar is handed to stage
N+1 as its fons; no minted names exist, "intermediates consumed by
construction" is literally true, and the tail relation alone is
registered under the wrapper's output= — so the XVIII end-guard
concentrates on the tail exactly as the dead-code argument wanted.
_exemplar_nucleus (extracted from _exemplar_implere) is the shared
stage core; the only behavioral delta of the refactor is vitium
ORDER (de= resolution now precedes modus/forma checks) — no fixture
pinned the old order.

**Canon lesson (the double-count trap):** a collected-vitia judge
cannot mirror the engine's first-vitium-wins shape. The static
emptiness rule ("no links could bind output") double-fired when a
link was malformed (flagged links don't increment the counter) —
fixed with an ante-guard: emptiness clamat only when the link walk
added no vitia of its own. Same lesson as the orphan rule's "bis
clamare numerum vitiorum mentiretur".

**Inner empty CATENA = identity.** Under flattening, a nested empty
chain contributes zero links and is algebraically harmless (only
the top-level total is judged at runtime); the static canon rule
catches the literal-empty-body authoring error instead. Runtime
stays algebraic, statics stay strict — a clean division.

Latina bite of the session: `nomen` as a parameter name (the #1
forbidden identifier, expands to typedef) — censor hook caught it
at write time, renamed to `titulus`.

NEXT: lint II (an examen codex re-expressed as a pattern document,
differentially gated) — now judged by the extended CAPS vocabulary
for free. CATENA's open sub-questions live in the interview file:
the tee (reserved), capture threading across links (second pull on
V.4 relational extension).

## 2026-08-31 — Lint II shipped: the first REAL codex as a pattern
## document (evening, same session as CATENA)

Codex 87 (OBSOLETUM_ADHIBITUM — usleep, the one-entry `<gradus>`
table in silva/fontes/systema_posix.h) re-expressed as
fixa/exemplaria/obsoletum_usleep.stml: ONE TRANSPARENTIA row (L1's
origin wrappers + regio-cruda), ONE EXEMPLAR
(`<lex-identificator $n>usleep</>`), one relatum carrying
pro="nanosleep". Gate = probatio_silva_exemplaria_obsoleta
(commit ffcc6837): GREEN ON THE FIRST FULL RUN — 155 lib files
(1/1 sites, tcp_posix.c) + 6 named probationes files (26/26
sites), zero divergence, zero fracturae, zero recusae.

What made it a step up from L1: the oracle is the SHIPPING
ANALYZER — the examen recipe copied verbatim into the probatio
(lexicon compose + capita praebita + oraculum + bis-analysis post
recanonicare), diagnostics filtered to the codex. ONE parse feeds
BOTH judges (arbor writes the lint view from the same parsura
semantica judges) — same tree, two independent judgments.

The scoping worry dissolved as scouted: system headers never enter
the tree, omitted #ifdef arms are `<regio-cruda>` (TRANSPARENTIA
skips whole), and the minimal transparency needed NO
divergence-driven additions. The apparatus path (apparatus_parsare)
would have been a FALSE-GREEN trap — it composes no systema, so
codex 87 never fires on it; the recipe copy is load-bearing, not
ceremony. Cost: ~0.3s/file × 161 (examen binary parity).

Corpus decision recorded: roundtrip tier OMITTED (lib already
supplies broad zero-site agreement; 78 recipe runs for nothing
this codex can see); probationes tier = a FIXED six-file index
(loud rot: a moved file = FRACTA, never silent).

Survey residue recorded in the interview file: codices 69/70
(unused var/param) are ANTIJOIN-shaped — the relational door's
pull, independently confirmed; codex 83 wants prefix matching (a
pattern-vocabulary door); codex 86 falls out free after 87 but has
zero live sites.

## 2026-08-31 — DIRIBITIO shipped (the pure-plane switch, night of
## the same day as CATENA and lint II)

The four-question decree round (name DIRIBITIO — the sorting of
ballots; chain-plane only; the conservative semantics bundle;
corpus differential gate) → engine → 41 fixtures (137/137) →
canon (401/401) → gate, in one pass. Commits 0c4d159b (decree) →
36b6045a (engine) → e02a7bf8 (fixtures + EST revision) → e368e68b
(canon) → ae55eb75 (THE GATE: lint II as a SELF-GUARDED chain —
DIRIBITIO whose condition is the usleep pattern, arm re-collects
with capture — byte-identical relatum vs the plain document,
161/161, zero divergence).

**The build's one real discovery: the pinned `<@est=>` spelling
cannot parse.** Attr-elements carry tree children ONLY in
invocation-argument position (modus I); everywhere else the §6.3
text-only law is enforced AT PARSE TIME (lib/stml.c
_attributa_elementa_probare), and generic STML cannot special-case
CASUS without a layering violation. The 2026-08-26 "zero new
syntax" claim was false for structured conditions. Fran's call:
**`<EST>` CAPS element** (`<CASUS><EST>pattern</EST>command
</CASUS>`), zero parser change, BACKPORTED to the fill-plane CASUS
door's pinned spelling; migrating live COMMUTATIO's value-form
est= attribute to <EST> recorded as a named mechanical increment.

Engine shape: `_mandatum_exsequi` = the shared pure-command
executor (EXEMPLAR via nucleus, CATENA via corpus walk, DIRIBITIO
via recursion — the closed class in one dispatcher, with the
est_conditio flag enforcing bare-condition law);
`_mandata_colligere` = the est/arm collector (fragments dissolve,
templates instantiate under applicatio — composability for free);
`_diribitio_processare` = the arm walk (form-checks ALL arms,
evaluates until first-congruent wins, ORDINARIUS-last mirrored
from COMMUTATIO, no-arm = empty relation out). Document-level
wrapper law identical to CATENA's. Vitium XXI.

Canon: `_mandatum_forma_iudicare` (static link-form judge for the
command class, conditio flag for the modus ban) +
`_diribitio_bracchia_iudicare`; DIRIBITIO also joined
`_catena_nexus_iudicare`'s link vocabulary — it was falling to
the alien-child vitium otherwise. Both lint probatios' invariata
walkers gained DIRIBITIO.

The pure-command class is now CLOSED AND LIVE: EXEMPLAR composes
by CATENA (AND) and dispatches by DIRIBITIO (OR), all three
conveyable, all three link-eligible, all three canon-judged.
Open doors unchanged: per-row routing, alternation, capture flow,
guard-narrows-arm, the tee.

## 2026-08-31 — THE EXTENSION LAW (the relational door's first
## slice, deep night)

Decree db57e9f8 (two four-question rounds) → build 9d8c0e7e.
Rows are now TUPLES: `_ligamina_hereditare` merges the source
row's captures into every row a per-fons match produces
(inherited first, own after; interned-pointer collision =
vitium XXII, loud — correlation belongs to the `&@n;` bridge,
join-by-naming stays a named door). The hook lives in
`_exemplar_nucleus`'s fons loop, so de= chaining, CATENA stages,
DIRIBITIO conditions AND arms all inherit uniformly. PER's map
widened FOR FREE — `_ligamina_ad_argumenta` already iterates all
ligamina, so accumulated captures reach reports with zero new
code (the fixture splices stage-1's t beside stage-2's k).

THE RE-VERIFICATION WAS THE POINT: all seven stml suites, canon,
the L1 gate (3772/3772 sites — the corpus now contains the
extension code itself), lint II (27/27), and every byte-identical
differential (catena, diribitio) stayed green. Carry-forward is
observably invisible to every existing document — no current doc
reuses capture names across stages, and richer PER maps only
matter when referenced.

Numbering note: XXII = CAPTURA_COLLISA (this law); SINE will
mint XXIII (the interview build-shape line said XXII for SINE —
superseded here).

NEXT: SINE itself (bare-pattern body, bridge-filled, row-subtree
scope, pure filter) + vitium XXIII + canon row + the codex 69
gate over fixa/examinis + lib.

## 2026-08-31 (later) — SINE shipped + the three matcher refinements the codex 69 gate forced

SINE (the antijoin) landed exactly as decreed: `_sine_processare`
on the chain plane — body = one bare pattern, `_ligamina_ad_argumenta`
+ `_liberos_expandere` under portatio fill the `&@n;` bridge PER ROW,
`_exemplar_petere` (floating) runs the filled pattern inside the
row's radix subtree, zero matches = row kept UNCHANGED. Vitium
XXIII covers engine attrs on SINE, malformed body, and fons NIHIL
(SINE as first link of a document CATENA without de= — document-wide
absence stays a named door, refused loudly). A SINE link IS lawful
as first link when de=/a DIRIBITIO condition feeds the chain.

USAGE TRAP (found by my own fixture): a SINE right after a
narrowing stage tests absence inside the NARROWED subtree. If the
absence lives beside the match (declaration vs sibling uses), the
capturing stage must keep the SCOPE element as its pattern root —
`<s><d n="$n"/></s>`, not `<d n="$n"/>`. "Scope-rows" is a
discipline the document author carries.

THE BIG FINDING — the codex 69 differential's first run produced
865 false positives across lib, and every one traced to a single
insight: **the bridge promotes captured document material into the
pattern plane, and document material carries representation-layer
baggage that hand-written patterns never had.** Three baggage kinds,
three refinements:

1. SYMMETRIC BLINDNESS: `_laxa_liberos_congruere` filtered only the
   CANDIDATE's children through `_liberi_effectivi`; spliced trivia
   children (`<post>`) in the pattern demanded matches no candidate
   could supply (transparent tags never survive candidate-side).
   Pattern children now go through the same filter. 858 of the 865
   died here.
2. `TRANSPARENTIA attributa=`: captured lexemes carry provenance
   attributes (b/linea/columna/f/def-*) which the attribute law
   ("littera: adsit et aequet") turned into constraints — "an
   identifier AT ITS OWN BYTE", unsatisfiable by any use. The
   document now declares attribute blindness the way it declares
   tag blindness; empty attributa= = vitium XIX; canon checks the
   same form.
3. CONTENT-ALIAS RESOLUTION: `<<#lexN>>` shared-lexeme transclusions
   (ambiguus arms, expansion strata) resolve to their fragment's
   content inside `_liberi_effectivi` (lazy id→node table over the
   expanded root, depth cap XVI). Sharing is representation, not
   content. Sites like `sizeof(x)`-as-only-use (expression arm holds
   x via alias) now witness uses correctly. Enumeration through
   aliases in petere is NOT done — a named door, taken only when a
   case demands it.

After the three: exact agreement, 206 files, zero divergence —
examinis 1/1, lib 0/0 — with L1 (3772 sites) and lint II byte
gates INTACT. The residue classes (second same-shape declaration
per block, ordinatio/initiated-pointer shapes, shadowing) are
measurably absent from the corpus; the pinned divergentes==0 will
announce their arrival.

NEXT: no decreed increment — the relational arc (rows-as-tuples →
extension law → SINE → codex 69 gate) is COMPLETE. Open doors live
in the interview file (tee, per-row routing, alternation,
join-by-naming, document-scope SINE, uncorrelated subtract,
COMMUTATIO est= migration, codices 83/86, petere-through-alias).

## 2026-08-31 (night III) — INDAGO shipped: commands resolve into trace comments

Fran's shape, built same evening: `<INDAGO/>` (bare CAPS element,
document-declared — zero API change, erasable) makes every consumed
command leave a TOMBSTONE COMMENT at its site. One scriptor per
top-level command (`_mandatum_vestigatum` at the interception);
inner links/arms append to the same comment. Format, live-verified
before pinning:

  <!--indago CATENA $improbata (radix); 1 EXEMPLAR punctum
      radix->3 (capturae: i); 2 SINE probatio 3->1 (necati 2);
      exitus 1-->
  <!--indago DIRIBITIO $v (de $th 2): CASUS 1 conditio 0
      praeteritum; ORDINARIUS SUMPTUM; bracchium; EXEMPLAR
      punctum 2->2; exitus 2-->
  <!--indago PER $improbata: ordines 1-->

Decisions that shaped it: comment-not-element is LOAD-BEARING
(traces must be invisible to the semantics they describe BY
NATURE — the attributa= bug was the cost of visibility with
forgotten blindness); PER traces uniformly, including inside
<relatum> (bytes differ only when the document opts in — that IS
the feature); DIRIBITIO conditions report the VERDICT COUNT only
(scriptor suspended around the condition's _mandatum_exsequi —
boolean tests don't narrate their stages; lift the chain out if
you need its story); skipped/unreached arms are absent from the
trace (evaluated and taken arms only). Counts were already
computed at every stage — the whole feature is chorda_aedificator
formatting plus one stml_commentum_creare per command.

Vitium XXIV = INDAGO non nudum (any attribute / non-comment
child). Under portatio INDAGO is inert like every command.
Commands BEFORE the element in document order are untraced
(contentum-supra law; put INDAGO first).

197/197; all seven suites + canon + L1 + lint II + codex 69 gate
green through the traced engine — zero behavior change without
the element (the 189 pre-existing exact-output pins are the
proof).

## 2026-08-31 (night III, later) — the descendant axis `<**>`

Existential descent shipped exactly as decreed. Implementation
notes worth keeping: the lexer's `'*' SOLUS` carve-out became a
star-RUN (the pattern-space name family; `<**>` previously
MIS-PARSED as `<*` + text `*>`, so no document could contain it —
compat free). Form errors are SETUP-TIME (`_descensus_probare`
at nucleus/SINE body collection → vitium XXV) so matching stays
a pure b32 — a malformed `<**>` can never masquerade as a silent
non-match. `_alicubi_congruere` mirrors petere's walk exactly
(same skips, no alias-descent) with early exit; the try-at-node
failure path truncates ligamina and WALKS ON, which is what makes
"first COMPATIBLE descendant" (and deep regula-V self-joins)
fall out with zero extra code. The partition in
`_laxa_liberos_congruere` leaves the cursor untouched — the
positional-composition fixture pair (`<**>` inside `<*>` in a
cursor slot: matches after `a`, refuses before) pins that the
composition, not the primitive, carries position.

The gate-doc unification measured IDENTICAL (1/1, 0/0) with the
residue class shifted: first-declaration-per-block-any-shape
(was: per-shape). Canon needed NOTHING — pattern bodies are
quoted, and `<**>` in content is just an odd element name, same
as `<*>` always was.
