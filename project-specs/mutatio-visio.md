# Mutatio Arborum — Visio (verbs, patterns, components)

Consolidated 2026-08-19 from a design conversation (Fran + Claude,
same session that shipped the provenance queries, the lexicon-typedef
preload, the ambiguity census, and the differre pairing fix — the
insights below lean on all four). Status: **RESERVATUM** — semantics
written down deliberately ahead of implementation, per stml-visio
doctrine 7 ("reservations are cheap and pay compound interest").
Sibling of stml-visio.md §6's "STML transform tool" entry; ledger res
carries the trigger.

**Nothing here is data-plane.** Everything below lives in the TOOL
plane (stml-visio doctrine 8: computation enters as a tool; documents
mean what they say on the page). Pattern/transform documents are DATA
consumed by a tool; the tool computes.

## 1. Why mutation is approachable (the modulated thesis)

The preprocessor was someone else's language: ISO handed silva an
adversarial string-rewriting system where nothing worked until all of
it worked (hidesets, pasting, recursion) — two predecessor parsers
died in that completeness trap. Mutation is OUR language: a **closed
vocabulary of decreed verbs**, each total and useful alone, grown on
demand. Verb one ships the day it exists; verb seven waits for a
consumer. The risk profile inverts. (Third appearance of the house
pattern: canon refuses expression languages for judged dialects,
natura binds by closed relation vocabulary, mutation grows by decreed
verbs. Closed vocabulary + judgment + deliberate growth.)

## 2. The verb vocabulary (gradus I)

Each verb = a named transformation type with a **checkable series
contract** — the formator generalized (its 17 rules share one
contract: token series preserved exactly; the paranoid applier
refuses anything outside it). Verb-specific contracts:

- *renominare*: series identical except IDENTIFICATOR valors old→new
  at exactly the claimed sites (verb one — ALREADY SHIPS in byte-edit
  form, ./silva/renominare.sh).
- *tolle-nodum*: series = original minus one contiguous subsequence.
- *insere*: original series with one splice whose content matches the
  template's series.
- *involve / substitue* (G1 `%wrap` / `%replace-with`): analogous.

Arbitrary transformation can only be TESTED; a closed verb can be
VERIFIED — the applier proves the edit stayed inside the verb's
meaning before a byte lands. G1's `%` family provides the ancestral
names (stml-visio §2: `%remove/%replace-with/%append/%wrap/...`,
ALIBI VIVIT as tools; exactly one verb — augmentation — was admitted
to the data plane).

**Universal gates on every verb, transaction-level:**
1. *differre-exactness*: differre(original, mutated) shows EXACTLY
   the intended pairs changed, nothing collateral (the 2026-08-19
   pairing fix — normalized seals + ordinal lists — is load-bearing
   here; it was fixed just in time to be this gate).
2. *reparse-identity*: the emitted mutant reparses to a tree equal to
   the mutant tree (mutation's roundtrip — the relexation property's
   big sibling).

**Division of labor with the formator**: mutation produces CONTENT,
the formator produces FORM. Synthesized nodes get minimal trivia;
`formator -scribere` normalizes afterward. The mutation engine never
needs a pretty-printer — half the traditional difficulty deleted by
composition. (Comment INHERITANCE on deletion remains a real design
question — comments are content, per the silva pin.)

**Oracle column**: each verb records whether it can invalidate
parse-relevant knowledge (rename/delete a typedef changes what
downstream text means). Most verbs are knowledge-neutral; knowing
which aren't is cheap at design time, expensive later.

**Provenance interlocks** (the 2026-08-19 queries as safety guards):
verbs refuse targets whose tokens are not source-level
(silva_token_est_fons) or whose subtree geometry collapses
(silva_nodus_geometria_fida). Macro-body interiors are unjudgeable
and unmutable from use-sites BY PRINCIPLE — macro-body formatting or
mutation happens at the definition site, a different code path.
AMBIGUUS regions: match canonical spine only, never command inside
(the formator's cut, inherited; no :lectiones for mutation until
something real pulls).

## 3. Pattern semantics (gradus II) — nested match with bindings

SilvaNodus IS a sum type (genus = discriminant into the registry,
loci = per-genus payload; generated accessors = projections; the
SilvaValor mini-sum underneath). ts-pattern-style nested structural
match maps directly — and silva can judge patterns AT LOAD against
the registry (does genus vocatio have locus functio? NODUS-species?),
the canon philosophy applied: malformed pattern = loud load-time
vitium, never a silent non-match.

The declarative transformation = **nested binding scopes with
commands interspersed at the matched positions**:

- Commands-at-positions make the pattern THE ADDRESS SPACE for edits
  — the match-one-thing-edit-another bug class becomes
  unrepresentable.
- **Scope quantifiers** (where tree-matching exceeds ts-pattern):
  `unum` (exactly one, else fail), `primum`, `omnia` (fan out;
  commands run per binding), optional (bind-or-absent, interior
  commands skipped). Default on fan-out failure: abort ALL siblings
  (refusal-loud).
- **Pattern = transaction**: required sub-pattern fails → whole match
  fails → zero edits land. (Emendatio-applier semantics at pattern
  granularity; R11's two-span detector was the precedent.)
- **Non-linear captures**: `$a` twice = matches must be EQUAL (token
  series equality — the differre normalized seal is the equality
  relation). ts-pattern lacks this; term rewriting lives on it;
  refactoring patterns want it constantly. In from day one.
- **Bindings carry SUBTREES WITH TRIVIA**, never strings — replanting
  a captured expression replants its comments; preservation falls out
  of capture semantics instead of needing rules.

The 2D form is not sugar over 1D — it adds expressive power quaestio
deliberately refused (see §4): multi-branch BINDING and LOCUS
addressing ("in the functio locus, this; in argumenta, that" —
descendant combinators forget which locus they passed through).

## 4. Relationship to quaestio (find vs shape; the pseudo-bridge)

quaestio's grammar is a PATH language: one spine, side-branches
testable via :habet but boolean-only — the header says it exactly:
"capturae intra selectores nidificatos numquam ligantur (neutrales)".
Right call for a find-language; structurally insufficient for
transformation (whose whole job is multi-position binding).

Division of labor:
- **quaestio = outer finder** (axes: siblings, descendants, semantic
  pseudos like :vocat — cheap over the forest) → candidate subjects.
- **pattern = inner shape** (destructure, quantify, bind, command).
- The pattern engine INHERITS quaestio's decisions: registry-driven
  loud judgment, the user-pseudo table as guard vocabulary, canonical
  spine ambiguity policy.

**Bidirectional translation (the sub-language theorem):**
- Every 1D selector chain embeds trivially into the pattern language
  (a chain is a degenerate, spine-only tree).
- A pattern that happens to be path-shaped translates DOWN to a 1D
  selector string (compilable to the existing engine).
- Any pattern — path-shaped or not — projects to a BOOLEAN and can
  therefore be registered as a **user-defined pseudo-selector**: the
  existing silva_quaestio_registrare mechanism, but with the pseudo
  implemented by a compiled pattern (STML data) instead of a
  hand-written C function. Inside a 1D chain the pattern-pseudo is
  boolean (bindings unavailable) — CONSISTENT with quaestio's
  existing capture-neutrality rule, so the degradation semantics are
  already house law. Full binding mode = pattern engine as driver.

Net effect: the component library backfills the 1D language through
the pseudo mechanism with ZERO changes to quaestio's grammar.

## 5. Components with slots (gradus III) — the reuse layer

Named patterns as reusable functions / "web components". The house
has invented this three times at lower fidelity — the signal it wants
to exist:
1. pseudo-classes = canned components, one string slot, C-implemented
   (:vocat encapsulates vocatio/functio/folium lore);
2. quaestiones.stml + legati quaestio {nomen, argumenta} = named
   queries, STRING-parameterized ("selector re-typatus promotionem in
   bibliothecam meret" — the component instinct at v0);
3. this: slots accepting PATTERNS — structural, higher-order,
   binding-preserving composition.

Design commitments:
- **Signatures**: declared slots (in) + declared EXPORTED bindings
  (out); internal captures private by default — two components both
  binding $x internally must not collide. A component is a function.
- **Canon-judged library**: component definitions are an STML dialect
  under a canon — slot arity, accepted species per slot, missing
  required slot = load vitium; unknown component = loud.
- **Compile-time flattening**: components expand into flat pattern IR
  (quaestio-chain-style); no runtime call semantics. Recursion =
  loud vitium, not a stack overflow.
- **Lineage**: parameterized fragments (`<#f attr=...>` +
  transclusion-with-arguments) are the RESERVATUM syntax neighborhood
  (stml-visio §6 "frame templates"); `$var` extraction is explicitly
  earmarked "feeds the future transform tool" (§2 sigil registry).
  G1's macro system returns in the TOOL plane, where computation is
  lawful — defmacro stays MORTUUM in the data plane.

## 6. Twin engines, one semantics

The same pattern/verb/component semantics wants TWO substrates:
- **silva trees** (C89 transformation — this document's main thread);
- **stml trees** (the §6 stml-visio corpus-migration tool — 35 natura
  models through a format change).
Design once as SEMANTICS, implement twice as engines. Meanings are
durable; embeddings are per-plane (doctrine 8).

## 7. Sequencing (walking skeleton before DSL)

House culture: C-level mutation API first (clone-spine, replace-node,
insert-into-lista, synthesize-token — likely reusing the GLR
fork/prospectus machinery, which is already structural sharing:
append-only stores, views with own mensura, copy-on-divergence),
exercised by ONE real consumer end-to-end with all gates, BEFORE any
selector/template syntax. Candidate first consumers, each forcing a
different API corner:
- the parked annotation-layer inserter (01KXTX7FA3/01KYB3NNR4):
  pure insert, gentlest contract (comments are trivia — code series
  untouched);
- prototype synchronization (definition signature edited → prototype
  follows): first COMPOSED verb, exercises query+verb pairing;
- "thread a parameter through a function and its call sites": the
  structural refactor span-editing genuinely cannot fake.

Surface syntax (sigils for bindings/quantifiers/verbs) is an STML
design decision made against the sigil registry in stml-visio.md —
that conversation happens at spec time, interview-first, raw
transcript archived (the G1/G2 recovery lesson).

## 8. Aperta (undecided, deliberately)

- Comment inheritance on node deletion (content, not trivia — needs
  a decree).
- Fan-out transaction granularity beyond the default
  (per-file? per-match? the default is abort-all).
- Whether pattern-pseudos may appear inside :habet (nesting depth of
  the bridge).
- The mutation API's shape: edit-script vs builder vs
  persistent-update-returning-new-root (the Eskil question — let the
  first consumer decide).
- memoriae_index-class knowledge invalidation policing: static verb
  column vs dynamic oracle-diff check after mutation.

## 9. Simulatio I — prototypa_synchronizare (2026-08-19, same session)

Pretend-implementation of verb one against real seams (house
technique; findings grounded in header reads, unverified by
compilation — strong hypotheses that shape the spec, to be confirmed
at build time).

**The architecture discovered (supersedes §8's open API question):**
mutant emission = `silva_scribere_nodum` (structural, position-free —
its header literally says "ingressus transformationum") to RENDER the
replacement subtree, then a byte SPLICE into the original file at the
old subtree's span. `silva_scribere_fontem` is offset-partitioned
FIDELITY machinery, structurally hostile to mutant trees (copied
offsets collide in its stratum-0 dedup) — never the mutant path.
Route A (tree) and Route B (bytes) compose instead of competing:
trees render and match; bytes land. The one new primitive =
**emendatio-with-content** (the formator's paranoid applier
generalized from whitespace-only to verb-contracted payloads;
descending application, span validation, whole-file refusal all
transfer).

**Findings:**
1. Committed trees immutable by contract (S27/S32) → verbs BUILD
   replacement subtrees (clone-spine helper: fresh creare +
   re-ponere; write-once becomes construction CHECKING; prospectus
   sharing = the GLR fork mechanics reused).
2. Token sharing across positions is structurally FORBIDDEN (trivia
   single-ownership + scribere_fontem offset dedup) — copying is
   law. Provenance fork: copy-as-FONS (cheap skeleton) vs
   **ORIGO_MUTATIO** (mutation as provenance event = generation N+1,
   unifying with the expansion-layer model; blocked only by the
   emitter's NAMED deferral "PASTA/CHORDA/API in catena = fractura
   clara" — a labeled dig site, not a surprise).
3. Payloads arrive TRIVIA-STRIPPED (copied trivia carries the source
   site's indentation); the formator restores house form — the §2
   division of labor held on first contact.
4. Contracts SHRINK on contact: the verb collapsed from "sync the
   prototype head" to "replace the parameter-list span, content
   token-matching the definition's" — specifiers legitimately differ
   between definition and prototype. Expect per-verb narrowing.
5. Verb one needs NO pattern language, NO DSL, NO components — plain
   C against existing seams (vista + prototype shape-test for find;
   clone helper; splice; gates). The walking-skeleton doctrine
   demonstrated, not just asserted.
6. Gates wire up from existing parts: differre (one
   MUTATA[substantiva] expected, rest immotae — the 2026-08-19
   pairing fix load-bearing immediately), reparse-identity (the
   puritas probatio's token collector promotable to shared helper),
   formator last.

**Interview questions generated:** ORIGO_MUTATIO in v1 or after?
emendatio-with-content extracted as shared instrumentum or grown in
place? verb one = prototype-sync or the narrower
parameter-list-replace it collapsed into?
