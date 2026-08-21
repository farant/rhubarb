# Arbor ⇄ STML — Visio (serialisatio canonica parsurae)

Consolidated 2026-08-19 (same design session as mutatio-visio.md).
Status: **PROXIMUM OPUS** — Fran decreed this the next project after
compaction. Ledger: parcum 01M0DMSTEJ (carries the conversation
notes). This document is the fresh session's primary onboarding
artifact; §7 is the re-entry protocol.

## 1. Quid

Silva's runtime parsura projects to a CANONICAL STML document and
deserializes back. This is not a new direction — it is the **dormant
half of the fidelity oracle**, committed in the silva VISIO since
birth and never built:

> code → parse → emit == code, **AND code → STML → load → emit == code**

And the data model anticipated it structurally (silva-spec-v2.md):
line ~119/442 — the registry layout drives "node genus, doubling as
selector/STML tag … selector tags, and STML serialization are all
table-driven from the same layout"; line ~507 — "STML-based Speculum
+ silva's STML output = one cross-layer query surface." **One
vocabulary (the registrum coctum), three consumers**: quaestio
selectors, the future pattern language (mutatio-visio §3), and this
serialization. The .c file remains truth; both directions of the
bridge produce PROJECTIONS (doctrine: files are truth).

## 2. Quod aperit

1. **The fourth oracle kind — representational honesty.** Anything
   in the parsura that cannot survive tree→STML→tree is by
   definition hidden state: reachable by C code, invisible to the
   database. The round-trip mechanically AUDITS the "code is a
   database" pin (expect it to flush out fields — candidate suspects:
   origo chain details, caecatio tails, conditional-region
   bookkeeping). Byte-roundtrip = completeness; relexation =
   classification; metamorphic = relations; serialization =
   representation.
2. **Canon judgment of trees.** The C89 grammar registry projects to
   a generated canon (the natura→canones lineage) — tree documents
   become judgeable; `silex iudicare` could judge serialized trees.
3. **The pattern-language substrate.** This phase DEFINES the
   genus↔tag / locus↔element mapping that mutatio-visio's patterns
   consume. Once trees have canonical STML form, a pattern is
   literally "a tree with holes" — same notation for instance and
   pattern. Sequences BEFORE the pattern engine.
4. **Speculum's front door.** STML→tree→emit = authoring code in
   STML form. The deliberate entrance to the language project, not a
   drift (see §4 doctrine boundary).
5. **Interchange + storage.** Solarium's database; silex volumina
   holding parsed trees; legati returning subtree STML across the
   tool boundary; tooling in other substrates.
6. **Golden trees in probationes.** Tier-1 AST assertions gain a
   hand-writable literal notation (expected tree as STML fixture)
   instead of accessor navigation.

## 3. The three elaborated directions (Fran, 2026-08-19)

**3a. The explorer (astexplorer.net-shaped).** Source one side, live
tree the other — the tool by which humans (and agents) LEARN the
queryable vocabulary: what genera exist, what loci they carry, how
they map to real code. Substrate is ready end-to-end: atrium
(window+webview one call), imperium (steering), manus (testing),
legati resident as the warm backend. The signature interaction
(click node ↔ highlight source span) is exactly
`silva_nodus_extensionem_lineis`. **Pedagogical bonus**: the explorer
makes macro-collapse VISIBLE — click inside a CREDO expansion and
every body token highlights the same invocation span. The
lying-geometry class (geometria_fida, venationes 2026-08-19) becomes
something a human sees in one glance; the tool teaches the
expanded-tree-with-provenance mental model faster than prose. (Check
laboratorium sketch "fons oculus" — the instinct may already have a
proto.)

**3b. Compressed nodes = the component lens completed.** At full
granularity the tree is verbose; large repeating structures are
often a pure function of a few variables. A component-with-slots is
a **bidirectional lens** over a parameterized tree shape, and this
direction completes its third mode:
- *match* (instance → bindings) — the pattern engine;
- *generate* (bindings → instance) — the template engine;
- *compress* (recognize instance ≡ component(bindings), store the
  compact form) — THIS.
The compress mode is machine-discoverable: census machinery can MINE
candidate components from the corpus (anti-unification — "this
40-node shape appears 31×, fully explained by 2 variables" =
nominated component). Components need not be invented; the corpus
nominates them. From here grows the compact DSL for common code
patterns.

**3c. Fragments for shared structure — a CORRECTNESS requirement,
not a compression nicety.** The serialized tree is a DAG wearing
tree clothes: expansion bodies, AMBIGUUS arms, origo chains,
caecatio tails all SHARE structure. Duplicating shared substructure
lies about identity — the sharing-vs-collapse distinction
(geometria_fida's hinge) appearing a third time. STML fragments +
transclusion are the native mechanism: a macro's expansion body as
`<#fragment>`, transcluded per invocation — honest AND compact for
exactly the macro-branches case. This is the first real PULL on
transclusion resolution in C (stml-visio: RESERVATUM, adopt on
pull). The serialization phase is thereby a genuine exerciser of
dormant STML features — fragments-with-arguments, transclusion
resolution — each against a real consumer.

## 4. Doctrine boundary (keep crisp)

Compressed forms in PROJECTIONS are lawful computed views (the
pipe-views precedent: computed views in generated projections only).
The moment a compact form is AUTHORED as truth, expansion becomes
the reader's job and the document is a LANGUAGE — which is fine,
because that is not a data document: that is speculum, entered
deliberately. **Code documents may be languages; censuses may not.**
The "no expression language in the data plane" law is not violated
anywhere in this design, but this is the exact border where confusion
would start — name it in every downstream spec.

## 5. Design questions (open, deliberately)

- **Scope staging**: subtree first (nodes + tokens + trivia —
  round-trip gate over subtrees of the existing roundtrip corpus),
  full parsura second (directivae, non-taken regions, strata,
  provenance chains — needs §3c fragments).
- **References**: document-space `#id` anchors for shared structure
  (sigil registry ready; `#` = document-space per decretum
  01KZPEXT74).
- **Trivia**: raw (`!`) tags for byte fidelity of comments/spacing.
- **The reader as generated code**: the deserializer is a candidate
  for GENERATION from the registry (canon_coquere lineage) rather
  than hand-writing — decide at the seam.
- **Size**: projection for interchange/tests first; bulk storage
  (silex) posture later.
- **Naming**: the dialect and tool need names (the genus-tag
  vocabulary is registry-given; the document envelope is not).

## 6. Staging proposal

- **M1 — subtree serialization**: SilvaNodus/SilvaValor (+ tokens +
  trivia) → STML → back; gate = tree-equality round-trip over
  roundtrip-corpus subtrees + the representational audit (log every
  field that doesn't survive). No provenance chains yet (tokens
  serialize their lexical fields; origo deferred to M2).
- **M2 — full parsura**: directives, conditional regions, strata,
  origo chains via fragments/`#id` (§3c); gate = the VISIO oracle
  itself, code→STML→load→emit == code over the corpus.
  - *2026-08-21 — §2 SHIPPED and the gate is GREEN* (78/78 files
    byte-exact; M1's subtree gate rose to 281/281 both oracles).
    **`strata` were deliberately dropped from the document**: layers
    are OUTPUTS of expansion, so a document meant to be edited must
    not contain them — a document holding an edited file beside an
    unedited stratum could lie. Name reserved, nothing owed. Origin
    is **nested inside the token**, not pooled. See
    `arbor-parsura-spec.md`. Remaining in M2: latinized corpus tier,
    amalgam integration, canon projection.
- **M3 — the explorer** (§3a) on M1's output; laboratorium-first.
- **M4 — components/mining door** (§3b) — opens toward mutatio-visio
  and speculum; not before M2's gate is green.

## 7. Re-entry protocol (fresh session, post-compact)

1. This document, then stml-visio.md (§2 sigil registry + §6),
   then mutatio-visio.md (the sibling — §3 pattern vocabulary
   dependency, §9 simulatio findings).
2. silva/CLAUDE.md protocol as always (phase-log tail; spec-v2 §9
   node/registry section before touching representation).
3. Surfaces at the seam: silva_nodus.h (the sum type), include/stml.h
   (writer/reader surface — NB silva VENDORS its own stml copy;
   amalgamare choreography applies), silva_tabulae.h (registrum
   coctum — the shared vocabulary), silva_scribere.h (emission
   contracts; the API-origin deferral note).
4. House method: interview first (raw transcript archived), spec,
   then plan. Candidate opening interview question: which downstream
   direction (pattern substrate / explorer / speculum / storage)
   drives M1's scope choices.
5. A simulatio of M1 against silva_nodus.h + stml.h before the spec
   is written would repeat the technique that settled mutatio's
   architecture in one session.
