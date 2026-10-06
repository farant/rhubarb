# Visiones extractae — stml-visio.md

*Extracted and classified 2026-10-03 from `project-specs/stml-visio.md`
("STML — Visio Longi Temporis", consolidated 2026-08-10, sections added
through 2026-09-03; last commit 2026-09-05). A consolidation by Claude of
Fran's notes and design conversations across three generations of design
(G1 "XML Macro System" and G2 "Complete XML-Based Language Definition",
both Fran's, pre-STML; G3 the TypeScript and C STML; G4 the semantic
stack on top). Claims the source attributes to Fran are marked (Fran);
§11 is "recorded verbatim in substance from Fran". One claim per bullet,
worded as close to the source as possible; `Lnn` = source line. The last
section is my commentary, kept apart.*

*Marking: put `[+]` on the claims to keep for the distillate.*

*Tags (see `docs/taxonomiae.html`): `{PRINCIPIUM}` prime condition,
`{PRINCIPIUM+intervention, split}` a condition with its cure attached,
`{OCCASIO}` opportunity, `{INDICIUM}` clue, `{REGULA}` commitment,
`{VISIO-OPERIS}` project vision, `{DECRETUM}` load-bearing decision,
`{SUPELLEX}` furniture, `{nulla:…}` fits no tier (ratio, exemplum,
quaestio). `> Mn` = the item(s) it is evidence for; `> Rn` = the
project-wide regula it bears on (R1 1.0 and done, R2 C89, R3 no
dependencies).*

## I. One document, five statuses

- [ ] M1 One document for every STML design idea across three generations of design, with an honest status on each (L3-4)  {SUPELLEX}
- [ ] M2 Five statuses: VIVIT (shipped), RESERVATUM (written down, deliberately unbuilt), PARCATUM (parked with a ledger id and a trigger), ALIBI VIVIT (realized by decomposition into another organ), MORTUUM (deliberately dead, with the why) (L21-29)  {DECRETUM}
- [ ] M3 "Reservations PAY": `&name;` waited years, then resolved a live collision by having been written (L26)  {PRINCIPIUM > M48}

## II. The lineage thesis

- [ ] M4 G1/G2 wanted ONE homoiconic medium holding code, data, queries and transformations; the house instead split it by layer and shipped nearly all of it (L33-35)  {DECRETUM}
  - [ ] M4.1 CSS selectors over ASTs became silva (selecta, legati quaestio) (L37)  {nulla:exemplum > M4}
  - [ ] M4.2 the `%` mutation verbs became tools (renominare, emendare): transforms produce new files, files stay truth (L38-39)  {nulla:exemplum > M4}
  - [ ] M4.3 transclusion and capture operators became STML itself; `(>` also became TOLERA pins in C; `<#>` became natura's universal `<nota>` margin (L40-43)  {nulla:exemplum > M4}
- [ ] M5 The one bet G2 lost: C89 is the better *notation* for code; the macro system's real value was always the STRUCTURE operations (L44-46)  {DECRETUM > R2}
- [ ] M6 "Take the *meaning* of a construct, refuse its *embedding* where it would make documents compute. Documents mean what they say on the page." (L48-50)  {REGULA}

## III. The sigil registry

- [ ] M7 Sigils answer "what is this element's relationship to something that exists elsewhere?"; three reference sigils carve three spaces: `#` document-space, `&nomen;` world-space, `.` kind-space (L54-59)  {DECRETUM}
- [ ] M8 `\` multiline text: newlines are content, dedent at parse; the opt-out from flow-default text; glued to the name because it "should jump out" (Fran); inline children are the v2 differentiator from `!` (L64)  {DECRETUM}
- [ ] M9 Transclusion is carried as a node in C; resolution is the consumer's (briar's literate tangle is the first C consumer resolving `<<#x>>`) (L72)  {DECRETUM}
- [ ] M10 Pipe-transform transclusion `<<x | transform>>` is reserved as the textual form of computed views in generated projections, never in truth files (L73)  {VISIO-OPERIS}
- [ ] M11 Transclusion = alias (content-space, consumer-resolved, identity: Nelson's meaning); template = instantiation (engine-resolved fresh copy) - forced by the first consumer, where blanket expansion would have deleted arbor's shared-lexeme first uses (L74)  {DECRETUM}
- [ ] M12 Attribute-elements `<@attr=>`: two position-disjoint bindings (the parent's attribute as a child-list prefix, a call's argument right after the call); the BASE-format repair for the unrepresentable `"`; empty = tombstone (L76)  {DECRETUM}
- [ ] M13 Slot projections `&@n.slot;` yield a child wrapper's content, never the wrapper; choosing among same-tag siblings is a pattern's job, never a path's; no implicit element-to-text rule (L77)  {DECRETUM}
- [ ] M14 CAPS-Latin tags are the builtin-verb grammar: data tags stay lowercase, builtins SCREAM; cases are literals and presence tests ONLY, richer cases arrive as patterns (L78)  {DECRETUM}
- [ ] M15 `&nomen;` is "semantically a wikilink": live in attribute values (natura individua), reserved in prose until resolution exists (L80)  {VISIO-OPERIS}
- [ ] M16 `<.species>` mints a kind: the creation site rhymes with the citation site, as `<#intro>`/`#intro`; dotted elements stay selector-unaddressable until a real need forces an escape (L82)  {DECRETUM}
- [ ] M17 `##prefix` UUID selectors and `.ns::label` namespaces live in TypeScript only; adopt in C only on real pull (L83-84)  {nulla:quaestio}
- [ ] M18 `@x` value interpolation is dead as data-plane computation; the `@` character is reborn as the template-space marker, where expansion is a pure load-time projection (L88)  {DECRETUM}
- [ ] M19 The `:`/`::` syntax is dead, but its discipline (define-side and use-side visually distinct) lives as doctrine: the ancestor of the loud tripartition (L89)  {PRINCIPIUM > M25}
- [ ] M20 Expression notation in XML (`<{> <}>`, infix, postfix) is dead: "C89 is the notation for code; nothing mourns this" (L90)  {DECRETUM > R2}

## IV. The two entity policies

- [ ] M21 TypeScript STML had no character entities and reserved `&name;`; the C library decodes the five XML entities in text only, keeps attribute values raw, and passes unknown entities through as literal text, deliberately (L98-104)  {nulla:ratio > M22}
- [ ] M22 The disagreement composes: individual references live raw in attribute values, and future prose references ride the unknown-entity-literal rule until a resolver exists (L106-110)  {DECRETUM}
- [ ] M23 `;` in all positions, one grammar between attributes and text: the terminator is what keeps `AT&T` literal (L110-113)  {DECRETUM}

## V. The semantic stack

- [ ] M24 STML ideas GROW in separate organs: canon (schema), natura (kinds, relations, frames), projection to generated canons and typed C readers, censuses (instance documents); "canon LIMITS, never DEMANDS; NO expression language, ever" (L117-135)  {REGULA}

## VI. The cross-document arc (parked; trigger: the second census citing a library individual)

- [ ] M25 Global uniqueness of individuals within the declared world: a collision is a loud vitium, never shadowing; registrar-grade naming (`&carl_linnaeus;`, never `&carl;`) (L143-148)  {DECRETUM}
- [ ] M26 The loud tripartition: creating an existing name, augmenting a missing one, and citing an unresolvable one are each a vitium (L149-152)  {DECRETUM}
- [ ] M27 Augmentation is the ONE lawful data-plane verb of G1's `%` family: additive only, identity-addressed only; the delta is the augmenting document's overlay, never pushed upstream; corrections go to the owning library (L153-162)  {DECRETUM}
- [ ] M28 No ambient document-to-document web: documents cite themselves and NAMED libraries; "becoming citable means becoming a library" (L163-169)  {DECRETUM}
- [ ] M29 Identity-addressing kills "what matches?", additive-only kills the cascade: the computed view is a UNION with in-band provenance, shown by a fold-view inspector (L170-175)  {VISIO-OPERIS}

## VII. Reserved and future

- [ ] M30 An STML transform tool: G1's stylesheet system (selector + `%` verbs + pipeline) as a TOOL, "XSLT done right" for corpus-wide migrations; never document-embedded (L186-196)  {VISIO-OPERIS}
- [ ] M31 `<TEXTUS>`, a flattening verb with DOM textContent semantics, reserved until a consumer needs flattened text of structured content (L197-204)  {VISIO-OPERIS}
- [ ] M32 Parameterized fragments as future FRAME TEMPLATES, "YAGNI until then" (L207-210)  {VISIO-OPERIS}
- [ ] M33 Entity references in prose: the wikilink layer, wanting the cross-document arc first (L211-212)  {VISIO-OPERIS > M15}
- [ ] M34 md exercised the reservations: slot projections, forest PER, pattern arms and the self-call check sufficed for the full CommonMark program, with no walk verb and no join (L218-220)  {INDICIUM}
- [ ] M35 Two named, unopened doors from md: a fill mode that drops template layout trivia, and an absence sugar over the write-once law (L220-224)  {nulla:quaestio}
- [ ] M36 The fence hook: a C fence projected as a silva tree inside the md projection, one STML document across two grammars, selectable as one (L226-230)  {VISIO-OPERIS}

## VIII. The dead, with their reasons

- [ ] M37 The homoiconic code plane is dead: C89 + silva won (L234-236)  {DECRETUM > R2}
- [ ] M38 defmacro / compile-eval / gensym in the data plane are dead: "a document that computes is a document whose meaning is not on the page"; computation lives in the tool plane (L237-240)  {PRINCIPIUM}
- [ ] M39 Selector-addressed mutation in data documents is dead: a selector can match zero or forty and drifts as documents evolve - "the verdict-depends-on-invisible-state failure" (L241-243)  {PRINCIPIUM}
- [ ] M40 Override/cascade semantics are dead: "local override is a fork of truth wearing a patch's clothes" (L244-246)  {PRINCIPIUM}
- [ ] M41 `#` as the world-entity sigil was retired: it collided with STML fragments and the web's fragment identifiers; old sigil = loud vitium, proven by a planted fault (L247-250)  {nulla:exemplum > M7}

## IX. Doctrine distillate (eight laws for all future adoption)

- [ ] M42 Files are truth; graphs, views and cooked canons are projections (L254)  {REGULA}
- [ ] M43 No expression language in the data plane, ever (L255)  {REGULA}
- [ ] M44 One parser: every reading of a corpus goes through the same parse (L256-257)  {REGULA}
- [ ] M45 Byte fidelity: non-pretty write is byte-exact; parse metadata never reaches the emitter (L258-259)  {REGULA}
- [ ] M46 Canon limits, never demands (ontological necessity ≠ documentary obligation) (L260-261)  {REGULA}
- [ ] M47 Every reference failure is LOUD: no fallback chains, no silent literals, no rebinding; "scope is stated, never searched" (L262-263)  {REGULA}
- [ ] M48 Reservations are cheap and pay compound interest: write the syntax down with its semantics even (especially) when refusing to build it (L264-267)  {REGULA}
- [ ] M49 Adopt meanings, not embeddings: a raided construct enters the data plane only if documents stay non-computing; otherwise it enters as a tool (L268-271)  {REGULA > M6}

## X. The 2026-08-25 conversation: macros, control plane, substrate, pipes, SGML

- [ ] M50 Macros are "fragments if they could take arguments" (Fran) (L297)  {VISIO-OPERIS}
- [ ] M51 Tree-level substitution with provenance, never string-level: the correction of SGML entities and the C preprocessor, "the disease silva was built to cure" (L300-303)  {DECRETUM}
- [ ] M52 Expansion in strata: a stratum sees only earlier strata; termination by construction; intermediate layers stay queryable (L304-306)  {DECRETUM}
- [ ] M53 Query fragments subsume conditional inclusion (SGML INCLUDE/IGNORE), with arms retained rather than discarded (L307-310)  {VISIO-OPERIS}
- [ ] M54 STML as a Datalog-flavoured control plane: a heartbeat collects data as tags, the document derives, the result tree is a PLAN of effect verbs, a dumb executor performs it (L314-316)  {VISIO-OPERIS}
  - [ ] M54.1 expansion pure, effects at the rim; canon judges the plan before execution: "schema-validated side effects, the property no make/shell system has" (L318-320)  {OCCASIO}
  - [ ] M54.2 Datalog, not Prolog: stratified, terminating, no backtracking (L321)  {DECRETUM}
  - [ ] M54.3 first consumer decreed: declarative lint + autofix (query match + canon judgment + mutatio template) (L328-331)  {DECRETUM}
- [ ] M55 The substrate thesis: every parser projecting its AST into STML makes detailed file state just another tag input to the plane (L335-337)  {VISIO-OPERIS}
  - [ ] M55.1 depth managed by named patterns as the API boundary ("arbor's format changed twice in ONE DAY and only interface-consumers survived") and by decreed shallow projections (L337-341)  {DECRETUM}
  - [ ] M55.2 prior art is CodeQL; the differentiator is the byte-exact round trip: "the plane can FIX, not just find" (L343-345)  {OCCASIO}
  - [ ] M55.3 protect the pattern/constructor duality: one vocabulary that both matches and builds makes autofix definitional (L345-347)  {REGULA}
- [ ] M56 One `stml` tool with structured pipes, "PowerShell pipes you can cat": one record per line in a compact writer mode, `genus=` as the type discipline, `iudicare` as a canon firewall stage mid-pipe, grep's exit contract (L351-359)  {VISIO-OPERIS}
  - [ ] M56.1 decreed risks: never become a shell (zsh stays conductor); the stream framing contract is the API, decided before any subcommand ships (L360-362)  {REGULA}
- [ ] M57 SGML mined for meanings: SHORTREF as a projection layer, architectural forms reborn as natura + canon, LINK reborn as patterns judged against source and result canons, marked sections subsumed by query fragments, content-model exceptions as canon candidates, `#CURRENT` only as canon-declared inheritance, CONCUR refused, RCDATA a possible fourth rung (L366-384)  {nulla:exemplum > M49}
- [ ] M58 Comments transparent to capture counting: comments stay nodes but stop consuming captee slots (Fran) (L385-388)  {DECRETUM}

## XI. The strangler doctrine (2026-08-27): trajectory, not project

- [ ] M59 STML enters a codebase as a vine on the tree (structure matching the code) and over time becomes the source of truth, "the code kind of the negative space around the stml," until high-level transformations are operations over a library of transformational macros (Fran) (L392-398)  {VISIO-OPERIS}
- [ ] M60 The inversion of authority is never an event and never a policy: a region is STML-truth iff regeneration reproduces the live code modulo declared overrides; fail the gate and it loudly reverts to code-truth (L404-418)  {DECRETUM}
- [ ] M61 "Code as negative space": the residue no named form captures is where the real decisions live - compression as comprehension (parametrum family 81-87% coverage, top-3 skeletons 83%) (L407-413)  {INDICIUM}
- [ ] M62 Two transformation regimes on one moving boundary: mutation verbs for the code-truth residue, regeneration for strangled regions (L419-424)  {DECRETUM}
- [ ] M63 Cross-cutting dividend: an aspect edit is one definition edit + regeneration - "the AOP pointcut, except the pointcut is the form's definition site, not a regex over someone else's code" (L425-428)  {OCCASIO}
- [ ] M64 Removability is the safety property the classic strangler lacks: any region can expand-and-freeze back to plain code at any time (L429-431)  {PRINCIPIUM+intervention, split}
- [ ] M65 lapifex is the in-house cautionary tale: codegen that outlived its understanding, still load-bearing; escaped here because the generator is DATA under canon and erasability is doctrine (L432-435)  {nulla:exemplum > M64}
- [ ] M66 Worked examples: normalized logging harvested into a judged mock corpus (consumer-driven contract testing "for free"); schema → scaffold UI/DB/code with named overrides as item-wins + tombstone generalized to regions (L439-452)  {nulla:exemplum > M59}
- [ ] M67 The META-strangler (Fran): the semantic story (natura, censuses, the Porphyrian taxonomy) grows over the structural story as the structural grew over code, until semantic description is the adequate basis for generating the lower levels (L456-464)  {VISIO-OPERIS}
- [ ] M68 Because everything is tags, the tower is homogeneous: the same EXEMPLAR/macro/canon machinery at every story - "the property Lisp had, without judgment; here every story carries its judge" (L464-468)  {OCCASIO}
- [ ] M69 Known-unknowns linting: a new vitium class INCOMPLETUM (well-formed but inadequate to its declared ambitions), computable only against a closure declaration (L472-481)  {VISIO-OPERIS}
  - [ ] M69.1 adequacy is consumer-relative: the generator enumerates the holes - "cannot scaffold the DB: these three kinds lack a key" beats generating garbage (L482-486)  {DECRETUM}
  - [ ] M69.2 the natural output of INCOMPLETUM is a QUAESTIO: "the document interviews its author"; precedent: the necessitudines burn-down, 263 → 0 off a machine-produced hole list (L487-493)  {VISIO-OPERIS}
- [ ] M70 PRAETERITUM, the third verdict class: complete against an OLD schema - behind; the simulated $0.00 bug shows staleness verdicts must be able to gate (L508-514)  {DECRETUM}
- [ ] M71 Lintability in dynamic dialects is MANUFACTURED, not analyzed: generated typed accessors + a perimeter lint; the vine need only cover the seams where hand code touches schema'd data (L518-523)  {DECRETUM}
- [ ] M72 Policies are composites: "a thing you enable, not five rules you remember" (L524-528)  {REGULA}
- [ ] M73 Machine-driven scaffolding is tabularium generalized: "the house has been building the business-app generator all along" (L538-543)  {INDICIUM}
- [ ] M74 Adequacy ≠ omniscience: closure catches structural holes; a forgotten business case still needs domain review (L544-548)  {PRINCIPIUM}
- [ ] M75 The staleness findings are plausibly ONE relation at four granularities ("X was adequate against Y at version N; Y moved"): unify before building any of them separately (L550-555)  {DECRETUM}
- [ ] M76 Vocabulary skew is SYMMETRIC IGNORANCE: neither party knows who moved; version stamps presuppose a trusted stamper (Fran) (L559-564)  {PRINCIPIUM}
  - [ ] M76.1 Fran's primitive: the corpus-vocabulary diff - census a corpus into its de facto vocabulary and diff it, de facto vs de facto or vs de jure; output four quadrants, the dangerous one being same tag, different shape (L569-577)  {VISIO-OPERIS}
  - [ ] M76.2 "Compare USAGE, never stamps" (L578-581)  {REGULA}
  - [ ] M76.3 over generations lineage BRANCHES; the primitive that survives is the pairwise diff, from which lineage is reconstructed - vocabulary stemmatics (Lachmannian recensio) (L582-588)  {VISIO-OPERIS}
- [ ] M77 1.0-and-done is right for LOGIC; a vocabulary is a BOUNDARY between parties and cannot be unilaterally frozen - its stability is manufactured by making every departure loud, cheap and accounted (L589-594)  {PRINCIPIUM+intervention, split > R1}
- [ ] M78 Generation is the metabolism, lint and mutation the immune system; generation amplifies vocabulary deltas, which is why vocabulary change is the master question of the trajectory (L599-604)  {PRINCIPIUM}

## XII. The 2026-09-03 steer: parsers as ingress, apps as projections (Fran)

- [ ] M79 All of the command/macro work is EXPLORATORY DESIGN: conceptual coherence is what Fran is confident in; usability is the open test - "technically feasible but horrible to use" sends a piece back (L612-617)  {DECRETUM}
- [ ] M80 Composition is expected to be THE way larger units are built; "macros and fragments as is are not yet ideal composition units" is a live possibility (L617-620)  {nulla:quaestio}
- [ ] M81 Many "input → STML tree" parsers reachable through `<PARSE>`: materia clients and lighter ones (plain text into a tree, regex-shaped parses into words or characters); with everything a tree, logic can be tag manipulation (L621-627)  {VISIO-OPERIS}
- [ ] M82 Apps as projections: lay out an app's requirements as semantic tags, wrap them in `<php>` or `<c89>`, and through layers of these operations obtain an implemented app (L628-632)  {VISIO-OPERIS}
  - [ ] M82.1 needs units like `<http-route>` that expand through many layers, inflected by tags ELSEWHERE in the app (the cross-document door) (L632-636)  {nulla:quaestio}
- [ ] M83 The no-expression line has so far pushed work to where the domain itself puts it (decoding into the parser, list tightness into parse-time marks): a stratified term-rewriting system, expressiveness bought by decreed projections, never by expressions (L646-651)  {INDICIUM > M43}

## XIII. Since then (my commentary, not in the source)

- Status has moved since the source's last edit (2026-09-05). Now VIVIT, per `docs/stml-reference.md` and the code: `\` multiline (M8, PARCATUM in the source); the whole v2 pattern language the source calls "mostly designed" (EXEMPLAR, PER, CATENA, SINE, DIRIBITIO, TRANSPARENTIA, INDAGO, `cursus` sibling runs); slot projections (M13, AEDIFICATUR); comments transparent to capture counting (M58); `stml` with three verbs (formare, expandere, vertere) and the -probare check. Still not built from M56: the pipe subcommands (quaerere, mutare, spargere/colligere, iudicare), the compact record writer, and the framing contract.
- One reservation arrived from outside this document: the raw-closer escape `<\/t>` (clausura plan, 2026-09-21), built because Fran hit the limit while making the format describe itself.
- Still reserved or parked as the source left them: the cross-document arc (§VI, trigger not fired), pipe views (M10), `<TEXTUS>` (M31), the transform tool (M30, design in mutatio-visio.md), prose entity references (M33), INCOMPLETUM and PRAETERITUM (M69, M70), `<PARSE>` (M81), apps as projections (M82).
- Lineage with other sources: the control plane (M54) has partly arrived by another road - fabrica (spec 1b §I) is "a control plane for derived state" with a pure judge and a thin executor, though declared in STML rather than derived by it. M75's "one staleness relation at four granularities" is what fabrica's input-set digest is, at the artifact granularity. M64 (removability) and M65 (lapifex) are the strangler form of the silva-instrumenta sheet's S17-S23 (regeneratable code with retained edits). M76.2 "compare usage, never stamps" is the house reflex "staleness = compare OUTPUT".
- The STML feature page (`docs/bibliothecae/stml_facultates.html`) inventories the reserved and dead forms of §III and §VIII as rows; this sheet is where their reasons live.
