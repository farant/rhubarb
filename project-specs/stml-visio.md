# STML — Visio Longi Temporis

Consolidated 2026-08-10. ONE document for every STML design idea across
three generations of design, with an honest status on each. Sources:

- **G1** — "XML Macro System" notes (Fran, pre-STML; previously
  unwritten, recovered 2026-08-10 — **verbatim archive:
  `archivum-xml-macro-system.md`**): Lisp-style macros over XML,
  selectors, the `%` mutation family, pipelines, stylesheets.
- **G2** — "Complete XML-Based Language Definition" (Fran, pre-STML;
  recovered 2026-08-10 — **verbatim archive:
  `archivum-lingua-homoiconica-xml.md`**): the full homoiconic
  C-targeting language — `@` variables, `:`/`::` parameters, arity
  markers, infix/postfix, fragments, transclusion-with-pipe.
- **G3a** — `../stml-parser/STML_SPEC.md` (TypeScript era): STML proper.
- **G3b** — `include/stml.h` + `lib/stml.c` (rhubarb, C89): the LIVING
  implementation, substrate of canon/natura/vitrea/silex.
- **G4** — the semantic stack grown on it in rhubarb: canon (schema),
  natura (kinds), censuses (individua), necessitudines (relations).

## Status vocabulary (every idea below carries one)

| status | meaning |
|---|---|
| **VIVIT** | shipped; the pointer says where |
| **RESERVATUM** | syntax/semantics written down, deliberately unimplemented — the `&name;` lesson: reservations PAY (it waited years, then resolved a live collision by having been written) |
| **PARCATUM** | parked in the tabularium with a ledger ID and an explicit trigger |
| **ALIBI VIVIT** | realized by DECOMPOSITION into another organ — the central discovery of 2026-08-10: G1/G2 did not die, they shipped in pieces, each in its right layer |
| **MORTUUM** | deliberately dead, with the why recorded |

## 1. The lineage thesis

G1/G2 wanted ONE homoiconic medium holding code, data, queries, and
transformations. The house instead **split it by layer** and shipped
nearly all of it:

- CSS-selectors-over-ASTs → **silva** (`selecta.sh`, legati quaestio)
- `%` mutation verbs → **tools** (`renominare.sh`, `emendare.sh`) —
  transforms produce new files; files stay truth
- transclusion + capture operators → **STML itself** (G3)
- `(>` forward capture → also **TOLERA pins** in the C annotation layer
- `<#>` anonymous documentation fragment → the universal **`<nota>`**
  doctrine in natura ("every kind-tag admits a curatorial margin")
- writing C in XML → **actual C89 + silva** giving structural
  affordances (the one bet G2 lost: C89 is the better *notation*; the
  macro system's real value was always the STRUCTURE operations)

The recurring pattern, and the standing rule for future adoption: take
the *meaning* of a construct, refuse its *embedding* where it would make
documents compute. **Documents mean what they say on the page.**

## 2. Sigil registry

The complete sigil grammar across all generations. The organizing
insight (2026-08-10): sigils answer *"what is this element's
relationship to something that exists elsewhere?"* — and the three
reference sigils carve three spaces:

**`#` = document-space · `&nomen;` = world-space · `.` = kind-space**

| sigil | meaning | status |
|---|---|---|
| `!` suffix | raw content tag | **VIVIT** (C + TS; C adds raw line capture `<tag! (>`) |
| `\` suffix | multiline content tag — newlines are CONTENT + dedent at parse (common indentation = trivia; relative indent survives); the opt-out from flow-default text. Glued to the name (`<versus\>` — "should jump out", Fran 2026-08-24); composes raw-first only (`<tag!\>`, `\!` refused). v1 text-only; INLINE CHILDREN are the v2 differentiator from `!` (raw can never have children, multiline eventually can) | **PARCATUM** — stml-trivia-spec.md §1.5, approved 2026-08-24 (parcum 01M0T5XYC3) |
| `(>` / `((>` … | forward capture, arity = paren count | **VIVIT** (C: `captio_numerus`; TS per spec; G2 origin) |
| `<) tag>` | backward capture | **VIVIT** (C + TS) |
| `<= tag =>` | sandwich capture | **VIVIT** (C + TS) |
| `<#>` `<#id>` | fragments (anonymous/named) | **VIVIT** (C + TS) |
| `<#id (>` `<# (>` | capturing fragments — fragment captures following siblings (forward direction ONLY) | **VIVIT** (C) |
| `<(>` | anonymous forward-capturing fragment (sugar for `<# (>`); the WRITER normalizes it to `<# (>` on serialization — read-side sugar, canonical write-side form | **VIVIT** (C) |
| `</>` | anonymous close (author's form preserved byte-wise) | **VIVIT** (C) |
| `<<selector>>` | transclusion | **VIVIT-as-node** in C (`STML_NODUS_TRANSCLUSIO` — parsed, carried); **RESOLUTION = RESERVATUM** (TS resolves with CSS selectors; C consumers don't yet) |
| `<<x \| transform>>` | transclusion with pipe-transform (G1) | **RESERVATUM** — earmarked as the textual form of computed VIEWS in generated projections ("`&c;` with augmentations applied"); never in truth-files |
| `<#@id p="@n">` / `<<#@id p="v">>` | TEMPLATE-space fragment definition / call (stml macros v1). The `@` is PART of the id. Doctrine (decretum 2026-08-26, e284bbcb): **transclusio = alias** (content-space, consumer-resolved, identity — Nelson's meaning, arbor lexN), **templum = instantiatio** (engine-resolved fresh copy). Forced by the first consumer: one surface carried both semantics and blanket expansion would have deleted arbor's shared-lexeme first uses | **VIVIT** (C: engine lib/stml_macros.c T1-T6 + arbor parsura slice T7 2026-08-26; canon judges call resolution, treats definition bodies as quoted material — loculus gating reserved, macros spec §6) |
| `&@n;` | loculus reference inside template bodies (text + attribute positions, whole or interpolated); literal outside definitions (unknown-entity rule) | **VIVIT** (C, template-space; `&@...;` spread forms refused loud — **permanently DEAD** since the 2026-08-26 argument-form decree: named argument-elements superseded anonymous children) |
| `<@attr=>` | attribute-element (`@`-prefix + `=`-suffix tag), value = its CHILDREN. TWO bindings, position-disjoint: PREFIX of a child list = the parent's attribute (text-only children — attributes stay string-valued; BASE-format repair for the unrepresentable `"`); immediately following a call = the call's argument (subtree values legal there, engine-map only). Elsewhere = loud vitium | **VIVIT** 2026-08-26 (macros spec §6.1 + §6.3, BOTH bindings built same day): base grammar in lib/stml.c (lexer + position law post-captures + capere dual lookup + writer + canon dual view; empty = TOMBSTONE, explicit absence); engine argument map in lib/stml_macros.c (block scalars ≡ inline, subtree args + forest splice + cross-strata threading, vitia VII ARBOREUM + VIII GEMINUM) |
| `&@n.slot.slot;` / `&@n.slot!;` | SLOT PROJECTION on a subtree argument: each path segment names a child wrapper element and the reference yields its CONTENT (text = scalar, else forest — never the wrapper itself; same-tag sibling choice is a pattern's job via `de="@n"`, never a path's). The `!` suffix (the raw-content sigil, same glued position as `<tag!>`) reads text + raw leaves as BYTES, allowed on any projection as an assertion, structure under it = vitium VII. No implicit element→text rule. Also legal as `de="@n.slot"` scope (door 5) and PER source | **AEDIFICATUR** 2026-09-03 (md-arbor-spec.md §6.1, ratified with Fran at B1 start; spread forms `&@...x;` stay DEAD) |
| CAPS-Latin tags (`<COMMUTATIO>`, `<CASUS>`, `<ORDINARIUS>`) | BUILTIN-VERB grammar for the template layer — data tags stay lowercase kebab, builtins SCREAM. First verb: arm selection in definition bodies; cases = literals + presence-tests ONLY (the no-expression-language line), richer cases arrive as patterns | **VIVIT** 2026-08-26 (macros spec §6.2, built same day): first-match arms in lib/stml_macros.c, optional slots `@p?` + flow-narrowing at collection (LOCULUS_NON_ANGUSTATUS), exhaustiveness CASUS_NULLUS, shape COMMUTATIO_MALFORMATA; de = whole-ref only, est = literal only; doc-level passes through; pattern cases reserved via `<@est=>` block form on builtins |
| `#id` | document-internal identity/anchor | **VIVIT** as the ruled MEANING of `#` (decretum 01KZPEXT74: `#` belongs to document-space — fragments, CSS/URL-fragment convention, future sententiae anchors) |
| `&nomen;` | entity reference — named singleton, deduplicated, "semantically a wikilink" (STML_SPEC reservation) | **VIVIT in attribute-space** (2026-08-10, f8c6dcc: natura individua — identity, citation, canon-validated, `;` in ALL positions); **RESERVATUM in prose-space** (the wikilink layer for sententiae/forum/notae — rides the unknown-entity-literal rule until resolution exists) |
| `.genus` | kind reference (vocabulary) | **VIVIT** (canon signa; resolves against generated canons) |
| `<.species>` | kind-MINTING element — `.` in tag-name position, part of the NAME (creation site rhymes with citation site, as `<#intro>`/`#intro`) | **VIVIT** (2026-08-10, arbor porphyriana: lexer accepts one leading dot in tag names only, never attributes; canon genus `titulus` mirrors the rule; natura `.genus`/`.species`/`.cultivar`). Open registry: any future `.`-tag self-announces as kind-minting. Cautions: (a) selector engine reads leading `.` as class/label (selectio.c) — dotted elements are selector-UNADDRESSABLE until a real need forces an escape; (b) annotation scanner requires `<`+letter, so dotted tags in C comments are deliberately invisible to it; (c) reconcile with `.ns::label` (below) before any C label-namespace work — two claimants on a leading dot in different positions |
| `##prefix` | UUID/NanoID git-style prefix selector | **VIVIT** (TS); **DEEST** in C (no uuid autogen in C either — adopt only on real pull) |
| `.ns::label` | label namespaces | **VIVIT** (TS); C has labels API without namespaces |
| `%` family | operations on what exists elsewhere (G1: `%remove/%replace-with/%append/%wrap/%set-attr…`) | **ALIBI VIVIT** as tools (renominare/emendare); **PARCATUM** as data-plane augmentation — see §5 |
| `<% &x;>` | augmentation (single lawful data-plane verb of the `%` family) | **VIVIT** — W3 SHIPPED 2026-08-10 (87b5944 grammar, 320aab8 judgment, e7a41ab migration); strict at birth (`<%>`/`<% laika>`/`<% .canis>` = vitium, unclosed = TAG_NON_CLAUSUM); additive-only mechanical (AUGMENTUM_PUGNANS XV); W2 collision tripwire remains (spec librarium-spec.md §3, COLLISA = XVI) |
| `<-->` + `$var` | selector combinator blocks + pattern extraction (G1) | **ALIBI VIVIT** (silva selectors; TOLERA forward-capture); remainder feeds the future transform tool (§6) |
| `@x` | value interpolation (G2) | **MORTUUM** as data-plane computation — data documents do not compute. The `@` CHARACTER is reborn as the template-space marker (`#@id` ids, `@n` declarations, `&@n;` refs — see the template row above): expansion is a pure load-time projection, never a document that computes |
| `:` / `::` | parameter definition vs invocation (G1/G2) | syntax **MORTUUM**; the DISCIPLINE (define-side and use-side must be visually distinct) **VIVIT** as doctrine — it is the ancestor of the loud tripartition (§5) |
| `<{> <}>`, infix `</ op />`, postfix `<) op>` | expression notation in XML (G2) | **MORTUUM** — C89 is the notation for code; nothing mourns this |
| `$:` positional args, `<@$1/>` | macro plumbing (G2) | **MORTUUM** with the macro plane |

## 3. The two entity policies, reconciled

The generations disagree about `&` — deliberately, and the disagreement
composes:

- **TS spec**: NO character entities at all; `&name;` RESERVED for
  entity references; `&` never escaped or rewritten.
- **C lib/stml.c** (measured from include/stml.h): the five XML
  entities (`&lt; &gt; &amp; &quot; &apos;`) processed **in TEXT
  only**, symmetric read/write; **attribute values RAW both ways**;
  raw (`!`) content verbatim; **unknown entities pass through as
  literal text, deliberately** (measured 2026-08-06).

This layering is exactly right for the 2026-08-10 adoption:
individuum references live in **attribute values** (raw — the canon
layer reads `&carl_linnaeus;` as plain bytes and validates form), and
future **prose-position** references ride the unknown-entity-literal
rule safely until a resolver exists. The `;` terminator is what makes
prose-safe `&` possible at all: `AT&T` stays literal because no `;`
closes it (real corpus case, organizatio). Hence the ruling: **`;` in
all positions** — one grammar between attributes and text.

## 4. The semantic stack (G4) — all VIVIT, pointers only

Each layer is its own project with its own docs; this section exists so
the vision doc shows where STML ideas GROW rather than duplicating them.

- **canon** (`lib/canon.c`, canon.canon self-hosting): closed
  vocabulary, cardinality, datatypes, uniqueness, in-document citatio
  keyref with `intra=` scoping, FINES, canon infixus, SIGNA
  (identitas/referentia attribute genera — the `&`/`.` machinery).
  Law: **canon LIMITS, never DEMANDS; NO expression language, ever.**
- **natura** (`lib/natura.c`, 35 models): kinds with inheritance,
  properties/parts/actions/machines, **necessitudines** (first-class
  relation kinds: conversum, scriptiones, families) and **frames**
  (`<termini>`/`terminus munus=` on act-genera). Detail:
  natura/METAMODULUS.md §8-9, natura.worklog.md.
- **projection**: natura → generated canons → typed C readers
  (natura_canones, canon_coquere) → semina.census (dictionary
  individua as generated data — the nascent EXPORT SURFACE, §5).
- **censuses**: instance documents (rhubarb.census et al.), judged by
  generated canons; census_orbi reachability lint; signum-with-via
  (numbers carry their re-measurement command).

## 5. PARCATUM — the cross-document arc (the big outstanding design)

Ledger **01KZPA16E4** (+ decreta 01KZPEXT74, 01KZP4B8KY context).
**Trigger: the SECOND census that wants to cite a library individuum.**
Build nothing before it fires. Consensed design, all from 2026-08-10:

1. **Global uniqueness of individua** within the DECLARED world (local
   document + consumed libraries). Collision = loud vitium, never
   shadowing/rebinding — a suppositum cannot be equivocal (kinds keep
   per-model uniqueness + the dormant domains door; identifiers are
   not vocabulary). Collision pressure is a feature: registrar-grade
   naming (`&carl_linnaeus;`, never `&carl;`).
2. **The loud tripartition** — every failure direction is a vitium:
   creation `<x nomen="&y;">` (error if `&y;` exists anywhere in the
   declared world) / augmentation `<% &y;>` (error if it does NOT
   exist) / citation `&y;` (error if unresolvable).
3. **Augmentation** = the ONE lawful data-plane verb of G1's `%`
   family (`%append`-shaped). Additive-only, mechanically defined via
   **multiplex**: adding to a multiplex member is augmentation; a
   second value on a single-valued member is a conflict-vitium.
   Identity-addressed only (never selector-addressed — that is the
   transform tool's mode, §6). Because one verb is lawful, the verb
   name vanishes: sigil alone carries it. The delta is the augmenting
   document's OVERLAY (its claims, carried with `fons=`/`certitudo=`
   — the epistemic qualifiers already exist); never pushed upstream.
   Corrections go to the owning library.
4. **Export surface** = what the projection emits (semina.census
   graduating into "librarium" = canon + exports). External
   consumption = vendored, stamped manifest (renovare posture;
   attestatio doctrine: a projection handed out stops being a
   projection). No ambient document-to-document web — documents cite
   themselves and NAMED libraries; becoming citable means becoming a
   library.
5. **Reading story** (the CSS lesson, inverted): identity-addressing
   kills the "what matches?" question, additive-only kills the
   cascade — the computed view is a UNION with in-band provenance.
   The inspector ("fold view": base + every augmentation, each claim
   tagged with document + fons) is an apparatus-shaped projection the
   architecture already knows how to grow.

## 6. RESERVATUM / FUTURUM — everything else on the books

- **Transclusion resolution in C** — the node type exists; a consumer
  with a selector engine does not. Adopt on pull. **The pull may have
  arrived 2026-08-19: `arbor-stml-visio.md`** (silva parsura ⇄
  canonical STML — the dormant half of silva's fidelity oracle,
  decreed NEXT PROJECT) needs fragments + transclusion for shared
  structure (expansion bodies, ambiguity arms — a DAG in tree
  clothes; duplication would lie about identity).
- **STML transform tool** — G1's stylesheet system (selector + `%`
  verbs + pipeline) as a TOOL in the renominare/emendare lineage:
  "XSLT done right" for corpus-wide STML migrations (35 models through
  a format change will want this someday). Never document-embedded.
  **Design mass banked 2026-08-19: `mutatio-visio.md`** — decreed-verb
  vocabulary with per-verb series contracts, nested registry-judged
  patterns (quantified binding scopes as transactions, non-linear
  captures, commands at bound positions), slotted components with
  signatures, the quaestio pseudo-bridge (patterns project to boolean
  = user-defined pseudo-selectors; path-shaped patterns translate to
  1D chains), twin engines over silva AND stml trees, one semantics.
- **`<TEXTUS>` — flattening verb** (2026-09-03, md B1 review): a CAPS closed
  verb in the fill plane, DOM textContent semantics (all text and raw
  bytes of the forest in document order), explicit by construction.
  Reserved with the aggregates' pattern (door 8). Pull: the first
  consumer needing flattened text of STRUCTURED content (anchor ids,
  a search index). Not md: paths + the parser's derived tokens cover it,
  and dialect questions (does a break contribute a newline? alt text?)
  are the parser's, not the engine's.
- **Pipe-views** `<<&x; | cum-augmentis>>` — computed views in
  GENERATED projections only (INDEX pages, inspectors).
- **Parameterized fragments** (G2: `<#f attr="@hole">` +
  `<<#f attr="v">>`) — future FRAME TEMPLATES: when act-frame number
  four rhymes with one through three, this is declared-once for frame
  shapes. YAGNI until then (venditio is one frame).
- **Entity references in prose** — the wikilink layer (§3); wants the
  cross-document arc first (resolution needs the declared world).
- **Signa in `.genera`** — desideratum 01KZEJABR0 (documents-only
  today).
- **UUID/NanoID + `##` in C** — TS has it; adopt only when something
  pulls (tabularium ULIDs cover most identity needs today).

## 7. MORTUA — with their reasons, so they stay dead

- **The homoiconic code plane** (G2's expression syntax, infix XML,
  positional macro args): C89 + silva won. The value was structure
  operations, which shipped elsewhere.
- **defmacro / compile-eval / gensym in the data plane**: a document
  that computes is a document whose meaning is not on the page. House
  law (canon: NO expression language) is the standing form of this
  refusal. The tool plane (§6) is where computation lives.
- **Selector-addressed mutation in data documents**: a selector can
  match zero-or-forty and drifts as documents evolve — the
  verdict-depends-on-invisible-state failure. Identity-addressing only.
- **Override/cascade semantics for augmentation**: local override is a
  fork of truth wearing a patch's clothes. Additive-only; conflicts
  are vitia; corrections go upstream.
- **`#` as world-entity sigil** (the 2026-08-07 trichotomy's choice):
  collided with STML fragments AND the web's fragment-identifier
  convention. Retired 2026-08-10 (f8c6dcc) with a migration tripwire
  (old sigil = loud vitium, proven by planted fault).

## 8. Doctrine distillate (the laws that govern all future adoption)

1. **Files are truth; graphs, views, canons cocti are projections.**
2. **No expression language in the data plane, ever.**
3. **One parser** — every reading of a corpus goes through the same
   parse (xmllint's retirement; the three-parses lesson).
4. **Byte fidelity** — non-pretty write is byte-exact; parse metadata
   (linea) never reaches the emitter.
5. **Canon limits, never demands** (ontological necessity ≠
   documentary obligation).
6. **Every reference failure is LOUD** — no fallback chains, no silent
   literals, no rebinding. Scope is stated, never searched.
7. **Reservations are cheap and pay compound interest** — write the
   syntax down with its semantics even (especially) when refusing to
   build it. `&name;` waited years and then settled a live design
   argument in one paragraph.
8. **Adopt meanings, not embeddings** — when raiding G1/G2 (or any
   future notes), the construct's semantics may enter the data plane
   only if it keeps documents non-computing; otherwise it enters as a
   tool.

## 9. The 2026-08-25 conversation — control plane, substrate, pipes, SGML mining

One session's design talk (Fran + Fable, the day formatter decrees
V/VI shipped). Raw notes: forum 01M0WY4G8Z, desideratum 01M0VNSZC6,
parcum 01M0T5XYC3. Nothing here is implemented; everything below
passed the §8 laws at the door.

### 9.1 Macros — the §6 "parameterized fragments" trigger is firing

**STATUS 2026-08-26: VIVIT-partial.** Engine SHIPPED (lib/stml_macros.c,
spec project-specs/stml-macros-spec.md, T1-T6 2026-08-25) + the arbor
consumer slice (T7: parsura loader expands, emitter authors
`<#@post-spatia n="@n">`, 78+155 byte-gate holds through pretty).
Template-space `#@` decree: spec §2.0 (transclusio=alias,
templum=instantiatio). Query fragments (below) remain the v2 pull —
the pattern-cardinality thesis (parcum 01M0XJQAVP notas) is their
design seed. Cross-document libraries banked on the same parcum.
**The v2 pattern language is now MOSTLY DESIGNED**: four informal
interview rounds (EXEMPLAR/PER, `$`-captures, bridge-as-scoping,
sigil-typed output=, the type ladder, wildcards/quantifiers,
DISTRIBUTIO) preserved in `stml-exemplaria-interview.md` — assemble
the v2 spec FROM that file after a short formal interview on its
open list; do not re-invent.

Fran: macros = "fragments if they could take arguments"; work starts
imminently. Decisions bound in conversation:

- **Tree-level substitution with provenance** (origo-style, recorded
  per expansion), never string-level — the correction of SGML
  entities and the C preprocessor (the disease silva was built to
  cure), not an imitation.
- **Expansion in strata** (silva's expansion-generations model): a
  stratum sees only earlier strata. Termination by construction;
  intermediate layers stay queryable.
- **Query fragments** (a fragment filled with query matches at
  expansion time) subsume conditional inclusion (SGML
  INCLUDE/IGNORE) — with the arms-retained doctrine, not SGML's
  discard.

### 9.2 The declarative control plane (RESERVATUM — trigger: first lint slice)

STML as a Datalog-flavored control plane: a heartbeat collects data
AS TAGS; the document (macros + query fragments) derives; the result
tree is a PLAN carrying effect verbs; a dumb executor performs it.

- Expansion PURE (tree→tree); effects at the rim only. **Canon
  judges the plan before execution** — schema-validated side
  effects, the property no make/shell system has.
- **Datalog, not Prolog**: stratified, terminating, no backtracking.
- Effects = decreed verbs; `processus_exsequi` is the exec seam;
  tabularium K3's RECEPTA SUNT CODEX ("agere exsequitur, numquam
  definit") is this doctrine already in law.
- Convergent organs: fabrica (declared artifacts + content hashes =
  the incrementality layer), metrum (threshold crossings emit tags),
  tabularium agere/processus.
- **First consumer decreed: declarative lint + autofix** (query
  match + canon judgment + mutatio template) — satisfies
  mutatio-visio's walking-skeleton trigger. silex/vitrea templates
  second.

### 9.3 The substrate thesis

Every parser projecting its AST into STML (arbor for C89; web
lexers next) makes detailed file state just another tag input to
the plane. Depth is managed two ways: **named patterns as the API
boundary** (macro-compressed queries — arbor's format changed twice
in ONE DAY and only interface-consumers survived; the speculum seam
lesson generalized) and **decreed shallow projections** (census,
indices, differre deltas — you don't always query the deep tree).
Patterns judged against the SOURCE canon catch dead queries
statically. Prior art: CodeQL; our differentiator is the byte-exact
round trip — the plane can FIX, not just find. Protect the
**pattern/constructor duality** when the 2D query language lands:
one vocabulary that both matches and builds (Prolog-term style)
makes autofix definitional.

### 9.4 The stml CLI and structured pipes (PARCATUM — 01M0VNSZC6)

One `stml` tool with subcommands (format, quaerere, mutare,
emittere, legere, formare, spargere/colligere, iudicare). The pipe
thesis: **"PowerShell pipes you can cat"** — record = top-level
element, one per line in a COMPACT writer mode (the formatter's
width machinery inverted); `genus=` as the pipeline's type
discipline; `iudicare` as a canon FIREWALL STAGE mid-pipe (typed
pipelines — novel vs PowerShell/jq/Nushell); grep's exit contract
(0 matched / 1 clean no-match / 2 refusal, loud). Selector syntax
survives as sugar desugaring to patterns — ONE engine underneath.
Decreed risks: never become a shell (zsh stays conductor); **the
stream framing contract is the API** — Eskil-first, half a page,
decided before any subcommand ships.

### 9.5 SGML mining report (meanings, not embeddings — law 8 applied)

- **SHORTREF/USEMAP** (context-sensitive surface sugar → markup):
  adopt as a PROJECTION layer (the sententiae one-way-parse
  doctrine), never in the parser.
- **Architectural forms**: already reborn as natura + canon; the
  actionable residue is **query-by-genus** in the 2D language.
- **LINK (LPDs)** — declared source→result mappings: reborn as
  mutatio patterns judged against source AND result canons
  (transformation typechecking).
- **Marked sections**: subsumed by query fragments; TEMP's
  provisional-text instinct noted for document lifecycles.
- **Content-model +/− exceptions and the `&` connector**: canon
  vocabulary candidates — free for us (post-parse judgment) where
  SGML paid in parse ambiguity.
- **#CURRENT** (inherit-from-previous attributes): only ever tamed
  as canon-DECLARED inheritance; ambient statefulness refused.
- **CONCUR**: refused — multiple projections over one truth is the
  house answer to overlapping hierarchies.
- **RCDATA**: a possible fourth rung of the kind ladder if
  entities-in-raw ever pulls.
- **Comments transparent to capture COUNTING** (Fran's M4
  directive): comments stay NODES (the silva pin holds); they stop
  consuming captee slots — the fluxus-transparency precedent
  applied to the parser's captee collection.

## 10. The strangler doctrine (2026-08-27 conversation) — trajectory, not project

Fran's frame: STML enters a codebase as a vine on the tree —
structure matching the code (arbor today: code is truth, the
projection byte-faithful and erasable) — and over time becomes the
source of truth, "the code kind of the negative space around the
stml," until high-level transformations of the codebase are
operations in STML over the library of transformational macros and
their compositions. Recorded with consequences; the primitives all
live in the v2 interview items (stml-exemplaria-interview.md Round
V) — nothing here adds v2 scope.

### 10.1 The vine grows per-region, and truth is earned by a gate

The inversion of authority is never an event and never a policy:

- A region whose STML form is a macro call with arguments is
  ALREADY effectively STML-truth (edit the call, regenerate); a
  raw-AST region is still code-truth. The boundary moves as the
  emitter learns forms — the compression measurements (parametrum
  family 81-87% coverage, top-3 skeletons 83%) are the vine's
  growth chart. "Code as negative space" reads precisely: the
  residue no named form captures is where the real decisions live
  — compression as comprehension.
- A region is STML-truth IFF regeneration reproduces the live code
  (roundtrip + differre) modulo DECLARED overrides. Fail the gate →
  loudly revert to code-truth. Authority is a verdict, continuously
  re-earned, never an assertion — the two-truths transition failure
  that kills classic strangler migrations dissolves.
- TWO transformation regimes, one moving boundary: mutation verbs
  (mutatio-visio — surgical, verb-contracted, differre-exactness)
  for the code-truth residue; REGENERATION (edit the definition or
  the call, re-expand) for strangled regions. The strangler
  progression IS the migration of regions from the first regime to
  the second.
- Cross-cutting dividend: when the cut points are named forms, an
  aspect edit is ONE definition edit + regeneration — the AOP
  pointcut, except the pointcut is the form's definition site, not
  a regex over someone else's code.
- REMOVABILITY is the safety property the classic pattern lacks:
  any region can expand-and-freeze back to plain code-truth at any
  time. The vine is peelable because projection is the primitive
  and both directions are gated. In-house cautionary tale: lapifex
  (codegen that outlived its understanding, still load-bearing
  under nuntium_schema). Escaped here because the generator is
  DATA under canon, not a program, and erasability is doctrine.

Worked examples (from the conversation, compressed):

- "Log request/response normalized at every API call site; harvest
  a mock library": EXEMPLAR relation over call sites → insere verb
  (unstrangled regions) or one-line form edit (strangled);
  harvested structures land as STML under a response canon → the
  mock corpus is JUDGED; re-harvest drift = canon vitia =
  consumer-driven contract testing falling out for free.
- "Schema → scaffold UI/DB/code, except named overrides": schema =
  one relation feeding per-dialect template libraries (the
  lingua-franca doctrine); overrides = item-wins + tombstone
  generalized from attributes to REGIONS; override anchoring = the
  sententiae anchor problem (authored address + sigillum, CAUTIO
  on unresolved) already solved once; hand-edit drift caught by
  regenerate-and-differre — the gate classic codegen cannot have
  because classic codegen does not roundtrip.

### 10.2 The META-strangler (Fran): semantics over structure

The same pattern recurses INSIDE STML: the semantic story (natura
genera, censuses, the Porphyrian taxonomy — §4's G4 stack) grows
over the structural story exactly as the structural story grew
over code. Today natura/census DESCRIBE what exists (projection);
the trajectory ends with the semantic description as the ADEQUATE
BASIS for generating the lower levels — "describing things
semantically based on what they are and the logic of different
kinds of things and their relationships." Because everything is
tags, the tower is HOMOGENEOUS: the same EXEMPLAR/macro/canon
machinery operates at every story — no per-level machinery. (The
property Lisp had, without judgment; here every story carries its
judge.) Three stories, one relationship repeated: truth-by-gate,
projection down, conformance up.

### 10.3 Known-unknowns linting (the strength Fran named)

A semantic document that recognizes its own loose ends and "errors
with known unknowns" where it is semantically underspecified:

- NEW VITIUM CLASS: **INCOMPLETUM**, distinct from MALFORMATUM —
  the document is well-formed but inadequate relative to its
  declared ambitions. Computable only against a CLOSURE
  declaration (closed-world per kind: "these are ALL the
  casus/genera/arms" — the registry as the closure surface;
  §6.2's exhaustiveness vitium and canon FINES are the existing
  embryos; stratification keeps the negation lawful).
- ADEQUACY IS CONSUMER-RELATIVE (teleological): a semantic
  document is adequate FOR a generator iff the generator's
  demanded slots are all satisfiable — the generator itself
  enumerates the holes. "Cannot scaffold the DB: these three
  kinds lack a key" beats generating garbage.
- The natural OUTPUT of an INCOMPLETUM vitium is a QUAESTIO: the
  document files its own interview questions (tabularium as the
  known-unknowns queue; the /interview culture is the manual form
  of this lint, mechanized — the document interviews its author).
  Lived precedent: the 2026-08-10 necessitudinum burn-down (263
  sedes solutae → 0, worked genus by genus off a machine-produced
  hole list) was exactly this loop, run by hand.

### 10.4 Simulatio: baubellarium (2026-08-27) — trajectory findings

Same-day pretend-SaaS exercise (full record + interview-feeding
items: stml-exemplaria-interview.md, Simulatio section). The
findings that bind THIS doctrine:

- **Instance migration belongs in the adequacy loop**: "optional
  property" is ambiguous over EXISTING rows (absent = none? or
  unknown?); the answer is a decreed migratio document (declared
  meaning-of-absence, canon-judged, consumed by the reader
  generator) that INCOMPLETUM can DEMAND like any other hole. The
  banked corpus-migration tool (mutatio §6) is the engine; the
  wiring is what is missing.
- **PRAETERITUM = the third verdict class**: an override complete
  against an OLD schema — not MALFORMATUM, not INCOMPLETUM,
  *behind*. The simulated shipped bug (#412) was a PRAETERITUM
  defect: the same override flagged advisory-stale in the morning
  shipped the $0.00 bug in the afternoon. Ergo staleness verdicts
  must be able to GATE, which opens the severity-policy question
  (which verdict classes block which pipelines, decreed where?).
  The wished-for affordance — diff an override against the fresh
  generated default — composes from differre + generation-to-
  scratch; wants a single verb, needs no new engine.
- **Lintability in dynamic dialects is MANUFACTURED, not
  analyzed**: raw field reads in JS are unjudgeable without
  dataflow; the strangler answer is generated typed accessors +
  a structural perimeter lint (no raw schema'd-data access
  outside accessor modules). The vine need not cover the JS —
  only the SEAMS where hand code touches schema'd data.
- **L4 policies are COMPOSITES**: a generation decision (loud
  accessors: absent field = error channel, never a default) + 
  perimeter patterns, bundled as ONE named unit with shared
  provenance — apparatus-shaped ("policy = a thing you enable,
  not five rules you remember").
- **Version skew = staleness at deployment granularity**: shipped
  bundles frozen at schema vN talking to a v(N+k) API — same
  phenomenon as PRAETERITUM, one level up. Home: fabrica stamps
  (bundle carries its schema version at build) + a census-style
  health-as-exit-code CI gate.
- **Greenfield adequacy = the spec interview mechanized** (§10.3
  confirmed on contact): scaffolding a new genus is answering the
  generators' enumerated known-unknowns; overrides declared AT
  BIRTH beat retrofitted ones.
- **Machine-driven scaffolding = tabularium generalized**: genus-
  declared lifecycles (natura actions/machines) generate status
  columns, transition guards, per-state UI affordances, API verbs
  — status flows + affordatae actiones is tabularium's living
  architecture; the house has been building the business-app
  generator all along.
- **Adequacy ≠ omniscience** (honest limit, keep it in the
  doctrine): closure catches STRUCTURAL holes (unreachable
  states, undeclared transitions, unsatisfied consumer demands);
  a forgotten business case is only catchable if some consumer
  demands it. Semantic omissions still need domain review.

The staleness findings (PRAETERITUM, version skew, relation
freshness, instance migration) are plausibly ONE relation worn at
four granularities — "X was adequate against Y at version N; Y
moved" — suggesting a single version-stamped adequacy algebra
rather than four ad-hoc checks. Unify before building any of them
separately.

### 10.5 Vocabulary epistemics (same conversation, the deeper cut)

Fran's observation: the whole edifice rests on a shared dialect as
stable as possible (the 1.0-and-done echo) — yet a document cannot
tell you, from itself, whether its vocabulary matches yours. Skew
is SYMMETRIC IGNORANCE: when there is a mismatch, neither party
knows who moved. Version stamps presuppose a trusted stamper and a
shared version authority.

- **The house edge**: vocabularies are themselves DOCUMENTS (canon,
  natura genera) — de jure comparison is canon-diff, decidable
  whenever both declarations are in hand.
- **Fran's primitive: the corpus-vocabulary diff.** Census a corpus
  into its DE FACTO vocabulary (the T7.2 value-masked shape-
  signature probe is the embryo; a usage census = an INFERRED loose
  canon), then diff: de facto vs de facto (two corpora), or de
  facto vs de jure (drift in both directions — usage beyond
  declaration, and dead declared vocabulary nothing uses).
- **Output is FOUR-QUADRANT, never a version delta**: shared-same /
  shared-SHIFTED (same tag, different shape — the dangerous
  quadrant) / only-A / only-B.
- **Doctrine: compare USAGE, never stamps** — the compare-output-
  never-timestamps reflex generalized to vocabularies. fabrica
  stamps remain the cheap declarative layer; the corpus diff is
  the audit that need not believe them.
- **Long-horizon**: version numbers assume linear time and a
  central authority; over generations, lineage BRANCHES (forks,
  dialects-of-dialects). The primitive that survives branching is
  the pairwise diff, from which lineage is RECONSTRUCTED —
  vocabulary stemmatics (Lachmannian recensio: manuscript kinship
  inferred from shared variants, because no manuscript carries a
  trustworthy stamp).
- **The stability reconciliation**: 1.0-and-done is correct for
  LOGIC (programs do not rot); a vocabulary is a BOUNDARY between
  parties and cannot be unilaterally frozen. Its stability is
  manufactured by making every departure loud, cheap, and
  accounted — and the change DISCIPLINE is the 1.0-and-done
  artifact. The vocabulary is what it superintends.
- **Position in the algebra**: the vocabulary delta is the ATOM of
  §10.4's staleness algebra — regeneration scheduling, migration
  demands, override review (PRAETERITUM), and bundle skew are the
  same delta observed at four granularities.
- **Generation confirmed as the center of gravity** (Fran's
  question; the simulation's evidence agrees): generation is the
  metabolism, lint/mutation the immune system — and generation is
  what AMPLIFIES vocabulary deltas into everything downstream,
  which is why vocabulary change is the master question of the
  trajectory.

## 11. The 2026-09-03 steer (md_arbor spec review) — parsers as ingress, apps as projections

Recorded verbatim in substance from Fran, at the point where the
markdown parser's html transform became the first GENERATOR over the
command layer:

- **All of the command/macro work is EXPLORATORY DESIGN.** STML is past
  the seed-concept phase; the vision needs experimentation and may need
  syntax or flow adjustments. Conceptual coherence is what Fran is
  confident in; USABILITY is the open test — "technically feasible but
  horrible to use" sends a piece back to the drawing board. Composition
  is expected to be THE way larger ergonomic units are built from
  smaller ones, and "macros and fragments as is are not yet ideal
  composition units" is a live possibility: changes are welcome when
  they are conceptually coherent.
- **Many "input → STML AST" parsers, reachable through `<PARSE>`** (or
  whatever its syntax becomes): materia clients (css, md, html, c89) and
  also lighter ones — natural language / plain text into a tree,
  regex-shaped parses that turn a string into words or characters. The
  suspicion: with everything a tree, LOGIC can be implemented as tag
  manipulation (pattern, dispatch, composition) as needed. Much
  experimentation to see how that works in practice.
- **Apps as projections.** The aggressive end of composition: lay out an
  app's requirements as semantic tags, wrap them in `<php>` or `<c89>`
  (or such), and through LAYERS of exactly the operations above obtain
  an implemented app — the app is a projection of the STML document,
  its library, and the STML operations. This needs units like
  `<http-route>` / `<<#@http-route>>` that expand through many layers
  into a concrete implementation while being contextually inflected
  not only by parent tags but by tags ELSEWHERE in the app (the
  cross-document/relational door, §6 and exemplaria §8.6).
- **Three additions ratified at the same review** (from the md_arbor
  experiment; all extend existing reservations): dotted SLOT
  PROJECTIONS on subtree arguments (`&@n.gradus;` scalar,
  `&@n.inlinea;` forest — the dotted forms the engine reserved); PER
  over a FOREST with delegation (`<PER de="@n.bloci"
  voca="#@md-nodus"/>` — door 5 with a path + existing `voca=`; the
  descent guard is satisfied by construction, so self-call becomes a
  CHECK); optional `<CASUS tag="x">` (a literal test on a subtree
  scrutinee's root tag — inside THE LINE).
- **Observation logged:** the no-expression line has so far pushed work
  to where the domain itself puts it (decoding and link resolution into
  the parser; parent context like list tightness into parse-time
  marks). Termination by strata + descent = a stratified term-rewriting
  system; expressiveness is bought by decreed projections, never by
  expressions.
