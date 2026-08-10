# STML — Visio Longi Temporis

Consolidated 2026-08-10. ONE document for every STML design idea across
three generations of design, with an honest status on each. Sources:

- **G1** — "XML Macro System" notes (Fran, pre-STML; shared 2026-08-10,
  previously unwritten in any repo): Lisp-style macros over XML,
  selectors, the `%` mutation family, pipelines, stylesheets.
- **G2** — "Complete XML-Based Language Definition" (Fran, pre-STML;
  shared 2026-08-10): the full homoiconic C-targeting language —
  `@` variables, `:`/`::` parameters, arity markers, infix/postfix,
  fragments, transclusion-with-pipe.
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
| `(>` / `((>` … | forward capture, arity = paren count | **VIVIT** (C: `captio_numerus`; TS per spec; G2 origin) |
| `<) tag>` | backward capture | **VIVIT** (C + TS) |
| `<= tag =>` | sandwich capture | **VIVIT** (C + TS) |
| `<#>` `<#id>` | fragments (anonymous/named) | **VIVIT** (C + TS); C adds capturing fragments `<# (>` and sugar `<(>` |
| `</>` | anonymous close (author's form preserved byte-wise) | **VIVIT** (C) |
| `<<selector>>` | transclusion | **VIVIT-as-node** in C (`STML_NODUS_TRANSCLUSIO` — parsed, carried); **RESOLUTION = RESERVATUM** (TS resolves with CSS selectors; C consumers don't yet) |
| `<<x \| transform>>` | transclusion with pipe-transform (G1) | **RESERVATUM** — earmarked as the textual form of computed VIEWS in generated projections ("`&c;` with augmentations applied"); never in truth-files |
| `#id` | document-internal identity/anchor | **VIVIT** as the ruled MEANING of `#` (decretum 01KZPEXT74: `#` belongs to document-space — fragments, CSS/URL-fragment convention, future sententiae anchors) |
| `&nomen;` | entity reference — named singleton, deduplicated, "semantically a wikilink" (STML_SPEC reservation) | **VIVIT in attribute-space** (2026-08-10, f8c6dcc: natura individua — identity, citation, canon-validated, `;` in ALL positions); **RESERVATUM in prose-space** (the wikilink layer for sententiae/forum/notae — rides the unknown-entity-literal rule until resolution exists) |
| `.genus` | kind reference (vocabulary) | **VIVIT** (canon signa; resolves against generated canons) |
| `##prefix` | UUID/NanoID git-style prefix selector | **VIVIT** (TS); **DEEST** in C (no uuid autogen in C either — adopt only on real pull) |
| `.ns::label` | label namespaces | **VIVIT** (TS); C has labels API without namespaces |
| `%` family | operations on what exists elsewhere (G1: `%remove/%replace-with/%append/%wrap/%set-attr…`) | **ALIBI VIVIT** as tools (renominare/emendare); **PARCATUM** as data-plane augmentation — see §5 |
| `<% &x;>` | augmentation (single lawful data-plane verb of the `%` family) | **PARCATUM** 01KZPA16E4 — see §5 |
| `<-->` + `$var` | selector combinator blocks + pattern extraction (G1) | **ALIBI VIVIT** (silva selectors; TOLERA forward-capture); remainder feeds the future transform tool (§6) |
| `@x` | value interpolation (G2) | **MORTUUM** — data documents do not compute; no successor wanted |
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
  with a selector engine does not. Adopt on pull.
- **STML transform tool** — G1's stylesheet system (selector + `%`
  verbs + pipeline) as a TOOL in the renominare/emendare lineage:
  "XSLT done right" for corpus-wide STML migrations (35 models through
  a format change will want this someday). Never document-embedded.
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
