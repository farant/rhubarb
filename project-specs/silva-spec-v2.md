# Silva — Specification v2 (codebase-grounded)

*Supersedes silva-spec.md's open questions (§12) with answers from four parallel
codebase explorations (2026-07-02): lapifex generator internals, arbor2 GLR merge
machinery, arbor2 token/lexer/expander layer, and v1-downstream/vendored-libs/fixtures
inventories. The v1 spec's decisions all stand; this document grounds them in the
actual code, resolves the open questions, adds newly discovered decisions, and turns
the plan into a concrete work inventory. Read silva-spec.md first for vision,
requirements, and the decision record; read this for how it maps onto what exists.*

---

## 1. Headline findings that shape the plan

1. **Conflict-preserving tables already work.** Lapifex's table builder appends all
   actions into multi-action cells and only *annotates* conflicts
   (`lapifex_generare.c:2242,2302-2328`; dedup at `:2108` rejects only identical
   entries; `conflictus_detegere` `:2132` marks `conflictus_intentus`). The action
   lookup already returns all matching actions (`lapifex_actiones_quaerere` `:2339`).
   Silva needs **no generator changes** for conflicts — only a grammar that contains
   them.
2. **Table baking does not exist.** No code emits tables as C source — only debug
   dumps. Tables are rebuilt at runtime (~7s) and cached in file statics
   (`lapifex_c89_grammatica.c:3557`). The baking module is **pure new construction**
   (mechanical: all table fields are POD s32/enums).
3. **The tagged-value refactor has exactly one logic site.** The only place the GLR
   engine *inspects* a value is the merge-vs-fork equality test at
   `lapifex_glr.c:451`. Everything else is mechanical copying through
   `LapifexGSSNodus.valor`, `GSSVia.valori[]`, `LapifexSignum.valor`,
   `fructus.valori`, and the callback typedef.
4. **Localized AMBIGUUS packing requires the tagged value first.** Lapifex's GLR
   forks same-state/different-value into separate frontier nodes and returns N whole
   parses (`glr.c:466-486`, ACCEPT collection `:557-604`); consumers take
   `valori[0]` and drop the rest. A GSS node's value must be able to *be* a set of
   alternatives — expressible only as a tagged-struct variant. This same whole-parse
   model caused the phantom-fork segfaults that forced the token-level segment loop
   (`lapifex_c89_grammatica.worklog.md:134-143`).
5. **Arbor2's merge is ~50 portable lines; its couplings are things we redesign
   anyway.** `_mergere_compatibiles` (`arbor2_glr.c:1184-1233`): same-state pairwise
   scan → shallow-equal ? union-predecessors : wrap-AMBIGUUS → tombstone. Its two
   delegated policies — `_nodi_aequales` (`:1095`, deliberately shallow: same genus =
   equal except TERNARIUS — a known correctness knob) and `_creare_nodum_ambiguum`
   (`:1123`, always tags TYPEDEF_IGNOTUM, >2-way ambiguity nests) — are exactly what
   the tagged-value redesign replaces. `furca_id` and `punctum_salutis` on GSS nodes
   are **dead fields**; do not port.
6. **Arbor2 already implements silva's conditional design.** `Arbor2CondRamus`
   (`arbor2_glr.h:263-278`) + `_colligere_conditionale` (`arbor2_glr.c:8328-8614`) +
   `_parsere_ramus` (`:8279-8323`, recursive whole-branch sub-parse into
   CONDITIONALIS with rami, nested conditionals recurse naturally) is the localized
   sub-parse pattern, working today — outside the GSS, touching no state numbers or
   value types. It ports essentially unchanged. The evaluator is invoked there
   (`:8378,8389,8526`) and keeps all branches (no pruning).
7. **The #define/#undef leak is root-caused — and it's a lexer-contract bug.**
   `arbor2_expandere.c:1997` treats "line start" as "has leading trivia," but the
   newline ending a line lives in the *previous* token's `spatia_post`
   (`arbor2_lexema.c:869-923`), so a column-1 directive after a non-blank line fails
   the guard and leaks through as ordinary tokens. Line-start is non-local in the
   arbor2 token contract. **Silva requirement: line-start is a first-class local
   token field.**
8. **The Lexema/Token two-type split is what let provenance die.** Trivia lives on
   `Arbor2Lexema` (`arbor2_lexema.h:184-194`), provenance on the `Arbor2Token`
   wrapper (`arbor2_token.h:75-79`); the M3 bridge extracted bare Lexema* because
   the GLR stores `(s64)lexema` — provenance built by the expander, discarded at the
   parser boundary. Silva's unified Token makes that impossible by construction.
9. **The trivia graveyard reduces to one enforceable invariant.** Seventeen
   catalogued v1 fixes (arbor_syntaxis.worklog.md) are all instances of two failure
   classes (LOSS: consumed delimiter's trivia discarded; DUPLICATION: parent and
   child capture the same run). See §5.3 for the formal statement.
10. **The vendored set is seven libraries, not five.** chorda needs
    chorda_aedificator; tabula_dispersa needs friatio (three functions of it). There
    is a chorda ⇄ chorda_aedificator cycle at the .c level → amalgamate
    declarations-first-then-bodies. Zero `##`/`#` operators in any vendored body, so
    a token-aware renamer is feasible. latina.h primitives are the substrate and are
    **not** renamed.

## 2. Answers to spec-v1 §12

1. **Lapifex reuse boundary.** Carry `lapifex_generare.c` (grammar reader,
   FIRST/FOLLOW, LALR builder `lapifex_collectio_lalr_construere` `:1644`,
   conflict-preserving table build) with two additive extensions: per-production
   annotation fields + reader parsing (the production loop at `:265-319`), and the
   new baking/emission module. Carry `lapifex_glr.c`'s architecture (GSS, worklist
   reduction, path enumeration) but rewrite the value+ambiguity layer. Carry the
   adaptator's genus→terminal map, updating its s64 packing. Drop
   `lapifex_parsere.c` (single-path LR, can't handle conflicts) and
   `lapifex_lexere.c` (C89 path uses arbor2_lexema). `lapifex_c89_grammatica.c` is a
   template to replace: the grammar string is liftable reference; the 200-case
   switch is what annotations delete; `0xDEAD`/`PerClausula`/segment-loop are
   artifacts to design out.
2. **Merge port shape.** Port the two-pass control structure (frontier scan +
   `reducenda` drain, merge once per token on the shift frontier only) and the
   ~50-line merge algorithm. Supply new policies: structural equality over tagged
   values (replacing shallow `_nodi_aequales`) and an AMBIGUUS value variant holding
   a flat alternatives list (replacing nested node wrapping). Keep arbor2's *second*
   ambiguity site — accept-reconciliation with a preference ladder
   (`arbor2_glr.c:7604-7665`) — as a distinct, explicit policy. Inherit the
   hardest-won lesson: **the reduce dispatch exists in exactly one function, called
   from both passes** (the dual-ordinarius bug class, worklog:2616-2626 — which
   generated construction eliminates entirely).
3. **Token contract.** Keep from arbor2: the `OrigoGenus` taxonomy
   (FONS/EXPANSIO/PASTA/CHORDA/API — complete), always-allocated provenance, pasta
   dual-parent capture. Discard: the two-type split (one `SilvaToken` carries text,
   position, trivia, standard-tag, AND provenance), the layer_index duplication, the
   first-member-aliasing pasta downcast (make provenance an explicit tagged union),
   and the invocation-vs-def-site location ambiguity (carry both: invocation site on
   the expansion origin, def-site on the macro body). Add: `initium_lineae` (b32,
   set at lex time) so line-start is local. Provenance chains are
   **immutable once created** — v1's praeparator hung by mutating a shared origin
   chain (arbor_praeparator worklog 2026-01-09).
4. **Vendored surfaces.** Seven libs; concatenation: latina.h → piscina → friatio →
   chorda_aedificator → chorda → tabula_dispersa → xar → internamentum, with all
   declarations hoisted first (breaks the chorda⇄aedificator cycle). Rename by exact
   identifier-token match (never substring — Latin prose in comments collides
   constantly; lowercase `chorda` is a substring of every chorda_* symbol), skipping
   comments and string literals. Only two globals exist (file-static internamentum
   singletons). Friatio: amalgamate only fnv1a/fnv1a_literis/djb2.
5. **Annotation syntax.** Grounded proposal: extend `<productio>` with attributes
   read via the existing `stml_attributum_capere` path — `genus="si-sententia"`
   (node genus, doubling as selector/STML tag), slot mapping for RHS positions
   (child slots, token slots, list-accumulation role), and a `manu="..."` escape
   hatch naming a hand-written constructor for irregular productions. Precedent: the
   unimplemented `<symbola>`/`<actio>` design in lapifex-spec-v2.md:50-69. Exact
   attribute grammar is simulation ③'s deliverable. Production identity must stop
   being purely positional (insertion-order index) — annotations give productions
   stable names, killing the renumbering hazard (worklog:100).
6. **Test fixture inventory.** Port: probationes/fixa/roundtrip/ (78 files — real
   sources incl. cursor.c/sectio.h/tractator.c/latina.h + per-construct
   micro-fixtures that map one-to-one onto the trivia graveyard), fixa/arbor_index/
   (5 files, incl. the cross-TU and static-visibility cases). Carry as
   don't-regress bars: arbor2 expander 134 + lexer 41 + lapifex-integration 48 +
   file-roundtrip 62 assertions; lapifex c89 suites (112/120/148/73) as grammar
   parity references. Full per-generation counts in the downstream report
   (foundation ~611, v1 ~862, gen2 ~2,888, gen3 ~750). Green-field new bars:
   conditionals (zero existing coverage anywhere — "tests were adjusted to avoid
   conditional directives"), a consecutive-column-1-directives leak regression test,
   deep multi-layer provenance, pasta dual-parent walking, segment historical
   accuracy.
7. **v1 downstream mining list.** arbor_quaestio: the selector data model
   (simplex/composita/sequentia) and `_iterare_liberos` (`:628-1026`) — the
   allocation-free uniform child-enumeration primitive that IS the generic node
   interface, to be regenerated table-driven from grammar annotations; the pseudo
   implementations (:calls/:uses/:defines/:returns) as reference; the catalogued
   bugs (mid-chain captures discarded, duplicate results, dead attribute paths,
   non-functional custom pseudos) as things to design out, not port. arbor_formator:
   the trivia envelope pattern and its hazards (hardcoded "enum" `:704` broke Latin
   roundtrip; silent `/* ? */` fallbacks — silva fails loud). arbor_typus: the
   three-namespace discipline (typedef/tag/ordinary scopes; note tag namespace was a
   stub — silva implements it for real). arbor_index: the genuine two-pass build
   (`:171-212`) and linker-style decl/def merge; fix the TU-isolation bug (statics
   leak into the global table `:582` and lookups never consult per-file tables) and
   position-search that never descends into expressions. Cross-cutting: the
   tagged-union walker footgun (two independent crashes from reading the wrong
   union arm) → **child access only through one safe accessor**.

## 3. Revised architecture notes (deltas from spec v1)

### 3.1 Engine assembly
The runtime is lapifex_glr's skeleton (GSS + worklist + path enumeration + Tomita
frontier double-buffer) with arbor2's control refinements (two-pass per token with a
`reducenda` drain; merge pass over the shift frontier) and the ported merge. The
tagged `SilvaValor` replaces s64 at the six mechanical sites plus the one decision
site; the AMBIGUUS variant holds a flat alternatives list plus the canonical index
(spec-v1 §5.2). Fixed caps to revisit consciously: `LAPIFEX_MAXIMUS_DEXTRUM=32` (RHS
length), arbor2's `MAX_GSS_PATHS=64`/`MAX_POP_DEPTH=16`. Generated GOTO tables get a
completeness validation pass (missing GOTO = infinite loop, not clean failure —
arbor2 worklog:284,1938).

**Packing point (new decision):** start with arbor2's proven policy — pack at the
shift frontier once per token, not during the reduction fixpoint. Tomita-style
reduction-time packing (the polynomial-bound guarantee) is the documented escalation
path if frontier measurement (simulation ②, and the walking skeleton's fork case)
shows growth; earlier packing demands the structural-equality policy be airtight
first.

**Top-level segmentation (new decision):** keep an outer per-declaration slicing
loop as the parse driver — both generations converged on it independently, it
bounds GSS size structurally, and it gives per-item error recovery for free. With
localized packing the grammar-driven translation unit becomes *possible* (the
phantom-fork cause is removed); treat it as an optimization/simplification to
evaluate after the substrate, not a 1.0 requirement. The boundary finder must
handle the known cases (`)` before `{` = function definition; struct/enum bodies;
initializer braces) and is part of simulation ①.

### 3.2 Expander (fresh build, informed)
Directive dispatch keys on the new `initium_lineae` token field (kills the leak
class). Segments: snapshot semantics — arbor2's segments store a live table pointer
(`arbor2_expandere.c:632-635`) so historical queries are wrong; silva uses
copy-on-write table snapshots per segment. Conditionals in the expander from day
one: do **not** macro-expand `#ifdef`/`#ifndef` operands; expand `#if`/`#elif`
expressions with `defined()` protected *before* evaluation (the one real gap in the
evaluator — it only resolves single-integer object macros, `conditio_evaluare.c:
653-671`); collect arms as regions with CondRamus-shaped metadata; all branches
retained, evaluation recorded per-arm. Carry: include learning/full modes and
resolution order, `_detectare_typedef`'s structure, latina.h registration,
stringify/paste semantics (re-lex pasted text, ERROR token on failure). Known v2
gaps to close: intra-pass rescan reliance on the outer fixpoint, `__VA_ARGS__`
by string-match, def-site fidelity of macro-body tokens.

### 3.3 The generic node interface, concretely
v1's `_iterare_liberos` proves the shape: a uniform, allocation-free
child-enumeration primitive that every consumer (query walk, formatter, type
resolver, index, serializer) shares. In silva it is **generated from the grammar
annotations** (the same slot mappings that build nodes also describe them), so it
cannot drift from the node definitions, and the tagged-union footgun (A6: two
independent v1 crashes) is structurally impossible — consumers never touch a union
arm directly.

### 3.4 Trivia invariant (formal, from the graveyard)
Every byte of source — each trivia run and each delimiter/keyword token's own text —
is owned by exactly one node and emitted exactly once. Leading trivia belongs to the
single deepest node that *starts* at that token; trailing trivia to the single
deepest node that *ends* at it. Container nodes that own no token own no trivia.
Delimiter tokens with no natural node are absorbed as token slots (not synthesized
by the formatter) into exactly one owner; positions with no natural owner get
explicit slots (v1 needed `trivia_vacuum` for `{ }` and `trivia_post_open` for
initializer-list braces). Ownership moves by transfer (set source to NIHIL), never
by copy-then-dedup (v1's pointer-equality dedup was unsound the moment trivia was
copied). The emitter emits each node's trivia in exactly one place and never
hardcodes text the parser captured (v1's hardcoded `"enum"` broke Latin roundtrip;
its `/* ? */` fallback corrupted silently — silva fails loud on unknown genus).
Enforcement: the invariant is checked structurally in tree construction (debug
assertion: every token consumed exactly once), and the 78-file roundtrip corpus +
graveyard micro-fixtures are the regression net.

### 3.5 Amalgamator, concretely
Two-phase output: hoisted declaration block (all headers, guards and internal
includes stripped), then implementation bodies in dependency order (§2.4). Renamer
operates on lexed identifier tokens (silva's own lexer once it exists;
bootstrap-phase can use v2's), never raw text; skip comments/strings; prefix map
covers the §2.4 type/function/macro inventory; latina.h passes through unrenamed.
The seven-lib subset is fixed; friatio is trimmed to three functions. Simulation ④
validates the renamer on the real sources and compiles the result standalone.

## 4. Work inventory (carry / extend / new)

| Piece | Basis | Verdict |
|---|---|---|
| Grammar reader + LALR + conflict-preserving tables | lapifex_generare.c | **Carry** + annotation fields |
| Table/construction **baking to C source** | — | **New** (mechanical) |
| Generated AST-construction from annotations | — (precedent: unbuilt lapifex-spec-v2 design) | **New** (simulation ③ first) |
| GLR skeleton (GSS/worklist/paths/frontier) | lapifex_glr.c | **Carry structure** |
| Tagged SilvaValor + merge policies | glr.c:451 + arbor2 merge | **Rewrite layer** (one decision site + ~50-line port) |
| Accept-reconciliation preference ladder | arbor2_glr.c:7604-7665 | **Port as explicit policy** |
| Conditional sub-parse subsystem | CondRamus + _colligere_conditionale + _parsere_ramus | **Port ≈ unchanged** |
| Conditio evaluator | arbor2_conditio_evaluare.c | **Carry** + pre-expansion of #if exprs |
| Lexer | arbor2_lexema.c | **Fork** + unify trivia scanners + initium_lineae + unified Token output |
| Token model | arbor2_token.h taxonomy | **Redesign around kept taxonomy** |
| Expander | arbor2_expandere.c as reference | **Fresh** (spec v1 decision; §3.2 specifics) |
| Terminal-map adaptator | arbor2_lapifex_adaptator.c | **Carry** + tagged values |
| Roundtrip emitter | arbor_formator envelope + arbor2_scribere | **New against invariant**, mined patterns |
| Selector engine (post-1.0) | arbor_quaestio model + _iterare_liberos | **Mine rules, rebuild** on generated interface |
| Type resolver / index (post-1.0) | arbor_typus namespaces + arbor_index two-pass | **Mine structure, rebuild** (fix TU isolation, real tag namespace) |
| Amalgamator | — | **New** (C89 tool; simulation ④) |
| Test harness | compile_tests.sh + credo | **Mirror** (drop Cocoa/Security links) |

## 5. Sharpened simulation briefs (M0)

① **Token contract + expander vertical.** Includes: the unified Token layout, the
newline-ownership + `initium_lineae` rules, segment COW snapshots, the top-level
boundary finder, def-site vs invocation provenance. Exit: the 134-test semantics
expressible against the new contract; the leak test unwritable (guard is local).
② **Conditional mechanics.** Port-fit of CondRamus against the fresh expander;
operand-protection rules; frontier/memory measurement on a real nested-#ifdef
header; cross-arm brace imbalance degradation.
③ **Annotated grammar → generated construction.** The attribute grammar; slot
mapping expressiveness against the hard cases (specifiers_ordine side-bands,
extra_specifiers, list accumulation, PerClausula-shaped intermediates — all as
honest tagged variants); the manu escape hatch; generated `_iterare_liberos`.
Exit: annotation format frozen.
④ **Amalgamation.** Token-aware renamer on the seven real libs; decls-then-bodies
assembly; standalone compile under the full flag set; collision test against a
host that also links rhubarb libs.

## 6. Walking skeleton (M1), grounded

Declarations + expressions grammar (lift precedence hierarchy shape from lapifex's
as reference), annotated, baked by the new emission module; fresh expander core
(#define/#undef/#include + segments + one #ifdef through the ported conditional
path); unified tokens end-to-end; GLR with tagged values; **the genuine fork case is
`foo * bar` with `foo` unknown** — must produce a localized AMBIGUUS with canonical
reading, roundtrip byte-exact, and re-canonicalize when the typedef is injected;
byte-exact roundtrip of one real file (cursor.c — v1's original proof file);
through the amalgamator; compiled standalone with the rhubarb flag set. That
exercises every load-bearing contract in the system at least once.

## 7. Newly surfaced risks

- **Structural-equality policy under packing** — arbor2's shallow equality silently
  discarded differing subtrees (nested-ternary bug); silva packs more aggressively,
  so equality + representative selection must be first-class and tested (property:
  canonical spine always roundtrips).
- **Boundary-finder correctness** — the outer slicing loop inherits the `)`-before-
  `{` heuristic; adversarial fixtures needed (function-pointer declarators, K&R
  definitions, braces in comments/strings already excluded by lexing).
- **Renamer completeness** — identifier-token-aware is necessary but the prefix map
  must also cover struct tags used as types and macro names; the collision test in
  simulation ④ is the gate.
- Carried from v1 spec: reduce-reduce correctness, conditional frontier growth,
  annotation lock-in, the coverage mountain.

## 8. Simulation ① + interview resolutions (2026-07-02)

*Simulation ① (token contract + expander vertical — see silva-simulatio-1.md) ran
after this spec was written; its complication register (S1–S17, R1–R2) and the
post-sim interview (silva-simulatio-1-interview.md) resolve to the following. Where
this section conflicts with §3 above, this section wins.*

### 8.1 Design changes from the simulation

1. **Event journal replaces COW segments** (supersedes §3.2's snapshot language).
   Forced by fact: TabulaDispersa has no clone primitive and arbor2's "snapshot"
   stored a live pointer. Macro-state history = append-only journal of events
   `{ fons, linea, ±define, def, conditio_id }` keyed by (fons, linea) since full
   includes interleave files. `macros_ad_lineam` replays into a caller-piscina
   table; segments become derived views; conditional-scoped state falls out via
   conditio_id. Query APIs take a caller piscina throughout.
2. **Hidesets (caecatio) are load-bearing.** arbor2's macro_stack guards nothing
   across layers (push/pop inside a non-recursing call; `#define A A` was bounded
   only by the 100-iteration cap). Silva: Prosser-style hidesets on the expansio
   origo — **immutable shared-tail lists** (O(1) extension, structural sharing,
   arena-friendly). `A A` fixpoints at layer 1; the iteration cap demotes to an
   assertion. New test family: direct/mutual/self-in-arg recursion.
3. **Layer = one full C89 expansion generation.** Args pre-expanded within the step
   (with their own hidesets) before substitution; `#`/`##` operands taken raw per
   standard. C89-conformant; stringify correct by construction; every intermediate
   token still recorded on provenance chains. The naive strict-substitution layer
   model is rejected (diverges from C89 on self-referential args).
4. **Layers are trees: region nodes inline.** All-branches conditionals fork macro
   state per arm, so a layer's stream contains CONDITIONALIS region nodes holding
   per-arm expanded sub-streams with arm-scoped journal events. The expander's
   regions ARE the parser's localized sub-parse units — one structure serves both.
   Flat iteration via a flattening iterator taking a configuration argument.
5. **Token contract additions**: `scissurae` (intra-token line-continuation splice
   offsets — full support in 1.0, since C89 splices before tokenization and
   `#def\<nl>ine` is legal); `fons_index` into a per-context SilvaFons file table
   (replaces per-token via_file; synthetic entries for API-injected bodies;
   preserves def-site because macro bodies are slices of real file streams);
   `initium_lineae` = first non-trivia token of a **logical** line (CONTINUATIO-
   aware — directive detection becomes one local check and the arbor2 leak class
   is unrepresentable); tokens immutable and **shared across layers** with
   layer_index deleted (generation = provenance-chain depth).
6. **Lexer: single trivia scanner + rule-driven attachment pass** (replaces
   arbor2's two divergent scanners). Trailing = same-line trivia through first
   newline; leading = rest. Ownership is a testable pure function.

### 8.2 Robustness posture (the tree-sitter bar, from the interview)

- **Lexer totality**: every byte sequence lexes — OCTETUS_IGNOTUS for unknown
  bytes, IMPERFECTUM genera for truncation/unterminated literals, NULs fine
  (chorda is length-based). Property: ANY input roundtrips byte-identically
  through lex→emit, with diagnostics alongside.
- **Preprocessor region-repair**: unterminated #if → region closed at EOF with
  est_imperfectum; orphan #else/#endif → error directive nodes in place; garbage
  after directives → directive node with error payload; unterminated macro
  invocation → plain identifier + diagnostic (arg collection EOF-bounded). Every
  input produces a complete tree.
- **GLR recovery — hybrid**: 1.0 ships boundary resync (discard to `;`/`}`/
  line-start directive, error node with skipped tokens), but the GSS and action
  dispatch are designed so recovery actions are **just another fork kind with a
  cost field** — tree-sitter-style skip/insert cost recovery stays structurally
  open as a post-substrate upgrade. Maximalist data model, pragmatic order.
- **Caps + cancellation**: configurable hard caps (include depth, region nesting,
  expansion generations, GSS frontier, total tokens) truncating gracefully to
  IMPERFECTUM/error nodes with partial results always returned; optional
  tree-sitter-style progress/cancel callback on SilvaContextus for interactive
  hosts. Deterministic — no timers.
- **Crash-freedom fuzzing from the walking skeleton on** (distinct from
  correctness fuzzing, which stays deferred): harness of corpus mutations +
  random buffers through lex→expand→parse→emit asserting never-crash,
  bounded-time, lex-level byte fidelity. Runs with the normal test suite.

### 8.3 Scope confirmations

- **K&R definitions: full parse**, nodes tagged obsolescent via the standard-tag
  mechanism (same as C99 tagging). The boundary finder tolerates declaration-
  shaped tokens between `)` and `{`; adversarial fixtures required.
- **Reformatting is a first-class anticipated use case** (upgrades §the v1 spec's
  single-style stance): format = the roundtrip pipeline with **generated trivia
  substituted for stored trivia**. Data-model requirement now explicit: comments
  are content (preserved, repositioned via attachment metadata); whitespace is
  style (discardable, regenerable). Style profiles are data; silva's opinionated
  style is the default profile; the formatter may generate scissurae for long
  macro definitions. New properties beside the roundtrip oracle: idempotence
  (format∘format = format) and semantic preservation (parse(format(x)) ≡
  parse(x)). Range formatting is localized by the single-owner trivia invariant.

### 8.4 Updated simulation queue

Simulation ① complete. Next: ③ annotated grammar → generated construction (S15's
region nodes now touch the construction layer; the annotation format must account
for region/CONDITIONALIS nodes and the cost-ready recovery action kind), then ②
conditional sub-parse mechanics (now largely pre-validated by the arbor2 port
finding + S15, but frontier measurement still owed), then ④ amalgamation.

## 9. Simulation ③ + interview resolutions (2026-07-02)

*Simulation ③ (annotated grammar → generated construction — silva-simulatio-3.md,
interview in silva-simulatio-3-interview.md). The annotation format is FROZEN at
v0 (sample in the sim file). Where this conflicts with §2.5/§3.3, this wins.*

### 9.1 The node model (S21, decided)

Silva nodes are **uniform**: a fixed header (genus, span, fons_index,
standard-tag, pater, semantic fields) + a per-genus **tagged slot array** whose
layout is described by generated tables. No per-genus union (arbor2's union
charges every node the max-arm size — 21-field declaratio — per R5). Type safety
via generated checked accessors (`silva_binarium_sinister(n)`); children(),
selector tags, and STML serialization are all table-driven from the same layout
tables. Semantic passes write header fields, never slots. This IS the
code-as-database representation solarium needs (3D IDE: code as database + 3D
structure + literate programming — recorded from the interview).

### 9.2 Construction purity under GLR (S26–S27, correctness requirements)

GLR reductions are speculative; forks die. Therefore: (1) generated constructions
are PURE — allocate and fill only; the annotation format cannot express side
effects. Typedef/oracle registration happens in a post-accept pass per top-level
slice. (2) **pater is never assigned during construction** — a subtree shared by
two live forks would get its pater overwritten by whichever reduces last (a
latent cross-fork bug present in arbor2/lapifex today, e.g.
`lapifex_c89_grammatica.c:645-646`). pater is assigned by a post-accept fixup
walk; AMBIGUUS children take pater from the canonical spine and
re-canonicalization re-runs local fixup.

### 9.3 Annotation format v0 (frozen)

- Inline per-symbol slot mapping: `symbol@slot` (append form `@slot+`); mapping
  rides the symbol so RHS edits can't shift it (the R3 drift class).
- One-symbol production without genus = pass-through. **Any production with >1
  RHS symbols must declare `genus` and map every terminal to a token slot** — the
  generator errors on unowned tokens: the single-owner trivia invariant enforced
  at generation time (v1's 17-fix LOSS graveyard becomes unrepresentable).
- Productions sharing a genus unify slots (union; unfilled = NIHIL; kind
  conflicts = generation error). Grammar factoring (dangling-else, per_clausula's
  8 variants) disappears from the tree.
- List rules: `modus="lista-initium|lista-appendere"`; list values are ALWAYS
  lists (kills the 0xDEAD dual-representation class). Separated lists interleave
  separator tokens with elements in one ordered list.
- `manu="fn"` escape hatch (emits typed call + prototype; possibly zero users in
  the core grammar — every known-hard case dissolved into annotations).
- **`id="..."` mandatory on node productions**, unique, pass-throughs exempt —
  diagnostics, generated comments, and conflict reports name productions stably.
- Structural genera (CONDITIONALIS, COMMENTUM, DIRECTIVUM, ERROR, AMBIGUUS)
  declared in **`<genera-extra>` inside the grammar file** — one registry, one
  input, no skew. Generated emitter skeleton is exhaustive over the registry
  (fail-loud on unknown genus is structural).
- per-clausula (and shapes like it) are **real tree nodes**, not unpacked
  intermediates.

### 9.4 Generator obligations

- The generator IS the schema (STML has no validation — R4): unknown symbols,
  unowned terminals, slot-kind conflicts, genus collisions, list-mode misuse,
  missing manu implementations, duplicate ids.
- Debug builds: generated assertions — value-tag matches slot kind on every
  store; each terminal consumed exactly once per reduction.
- **Generated code is fully readable and Latin-commented** (same standards as
  hand code; comments cite production id + rule) — the read-everything culture
  extends to the parser's generated core.
- Grammar lives on disk (`grammatica/c89.stml`), not in a C string literal.
- Baked action entries carry `pretium` + RECUPERARE now (discharges §8.2's
  cost-ready obligation in the table format).
- Single generated dispatch function, called from both GLR passes
  (dual-ordinarius class dead by construction).

### 9.5 Remaining queue (superseded by §10 — see below)

Simulations ① ③ complete. Remaining: ② conditional mechanics (frontier/memory
measurement on a real nested-#ifdef header is the one owed deliverable) and ④
amalgamation (renamer on real vendored sources + standalone compile). Deferred
open items: whether to write a verification test for the arbor2 cross-fork pater
bug before freeze-and-delete; accessor-only discipline for consumers outside
silva's modules. Speculum was shared (v0.5.3, future syntax to be STML-based) —
implications recorded in silva-speculum-implications.md: Speculum procedure
bodies are the snippet-parsing-with-injected-context use case made concrete;
STML-based Speculum + silva's STML output = one cross-layer query surface;
speculum-strip is another expansion layer above the preprocessor (provenance
model generalizes); the inspector's query list is an acceptance catalog for the
selector engine; silva stays Speculum-unaware (consumer via public API only).

## 10. Simulation ② + interview resolutions (2026-07-02)

*Simulation ② (conditional sub-parse mechanics — silva-simulatio-2.md, interview
in silva-simulatio-2-interview.md). Included the owed frontier/memory measurement
as a REAL census over 113 rhubarb headers + 9 macOS SDK headers. Register entries
S33–S40 use the Speculum complexity schema.*

### 10.1 The measured verdict

Conditional fork explosion does not materialize under the localized design:
region trees are shallow (max depth 4 in the wild sample), arms sub-parse
sequentially (GSS never holds more than one arm's forks + the reconvergence
check), and all-branches token retention is PARTITIONED, not duplicated (every
token belongs to exactly one arm's sub-stream). All 8 measured brace-imbalanced
arms are the extern-C/__cplusplus pattern. 175/252 system-header arms mutate
macro state — arm-scoped journal events are the main case there, while the
primary corpus (rhubarb: 113/113 headers) contains nothing but include guards.

### 10.2 The conditional architecture (signed off)

1. **Two-track semantics**: the cpp-faithful evaluated path (conditio oracle
   against live state) drives the default expansion track — canonical parses
   agree with what a real compiler sees; the journal (events under conditio_id)
   + the region tree retain every arm for configuration-scoped and wildcard
   queries.
2. **Sub-parse contract**: arms sub-parse from the ENCLOSING GSS STATE and must
   reconverge to a common landing state. Degradation ladder: balanced +
   reconverging = full sub-parse (overwhelming case); balanced + divergent =
   bounded downstream forks per arm; imbalanced = opaque token fringe with
   diagnostic (measured: rare, tiny, always-false-for-C89).
3. **Guard transparency**: est_custos-detected include guards produce NO region
   node — the body parses as unconditional structure, the three directives stay
   as plain directive nodes, guard-ness recorded on the file/fons record with
   pointers. (Without detection, 100% of the primary corpus would be wrapped in
   meaningless whole-file conditionals.)
4. **Operand protection**: #if/#elif expressions get a protected pre-expansion
   pass (defined-operands marked no-expand, ephemeral) before evaluation;
   #ifdef/#ifndef operands are never expanded.
5. **Configurations = atom assignments + named presets**: conditions normalize
   to interned expressions with extracted atom sets; same-atom regions couple;
   in-region arms are mutually exclusive by construction. SilvaContextus holds
   user-named preset configurations ("macos", "c89-nudum") = saved atom
   assignments, serializable with project config. Wildcard remains the default
   query mode.
6. **#if 0 idiom**: literal-false regions tagged `numquam`; parsed best-effort;
   queries/diagnostics filter them by default.
7. **Arm-scoped includes**: default track processes true-arm includes normally;
   false-arm includes become conditional include nodes resolved LAZILY on the
   first configuration-scoped query that needs them, with an eager contextus
   flag ("resolvere omnes configurationes") for index-building workloads (the
   cross-configuration rename/audit use case).

### 10.3 Remaining before implementation

Simulation ④ (amalgamation + prefix-rename on real vendored sources, standalone
compile, collision test) is the last of the four M0 simulations. After it: fold
any final changes, then the walking skeleton (M1) per §6 — whose conditional
requirement should now include one est_custos guard file and one two-arm #ifdef
exercising the sub-parse contract end to end.

## 11. Simulation ④ + interview resolutions (2026-07-02) — M0 COMPLETE

*Simulation ④ (amalgamation — silva-simulatio-4.md, interview in
silva-simulatio-4-interview.md) was run as a REAL experiment: crude amalgam of
the seven vendored libraries compiled clean under the full flag set; ~40-line
token-aware renamer prototyped, applied (1,084 renames), recompiled clean;
symbol collisions vs rhubarb's own objects measured 15/44/32 → zero after
rename. Artifacts in the session scratchpad.*

### 11.1 The amalgamator, as proven and decided

1. **Manifest with two topological orders** — headers (latina → piscina →
   chorda → chorda_aedificator → friatio → tabula_dispersa → xar →
   internamentum) and bodies (independent; the chorda⇄aedificator cycle is
   body-level only). Decls-first assembly, compile-verified.
2. **Per-file function exclusion lists** in the manifest (friatio ships only
   fnv1a/fnv1a_literis/djb2); the assembler drops excluded definitions AND
   prototypes. Canonical lib sources stay untouched. Required because static
   internalization × -Werror makes unused functions build failures.
3. **Static internalization is load-bearing** (export surface = public silva_*
   API only; ~155 externals collapse to the API); **prefix-rename retained on
   top** for debugger/profiler legibility per the original vendoring decision.
   Static injection rewrites prototype and definition consistently.
4. **silva.c assembly order**: [system includes: ctype/stdio/stdlib/string]
   [silva.h verbatim] [latina.h] [renamed+static internal decls] [bodies].
5. **silva.h is vanilla C89** — plain spellings in signatures AND documentation;
   Silva-prefixed types; latina's 388 defines never cross the public boundary.
   A generated host-pollution test (host declares variables named si, per,
   character after including silva.h) enforces this permanently.
6. **Vendored include guards pass through verbatim** — the shared macro names
   double as double-inclusion protection for hosts that also use real rhubarb
   headers.
7. **Amalgamator test suite**: duplicate-static detection, nm-intersection
   collision check against rhubarb objects, the host-pollution compile,
   standalone full-flag-set compile of the assembled silva.c. Public API names
   kept ≤ 31 chars (C89 external-identifier significance).

### 11.2 M0 closed — the state of silva

All four simulations complete (① token/expander, ③ grammar/construction, ②
conditionals, ④ amalgamation), all registers resolved through interviews:
47 numbered complications + 5 research findings, every one dispositioned. Every
load-bearing contract is decided, simulation-tested, and where possible
measured or experimentally demonstrated:

- Token contract (unified, provenance-immutable, scissurae, initium_lineae)
- Expander semantics (journal, hidesets, layer = rescan generation, regions)
- Conditional architecture (two-track, GSS-state sub-parses, guards, configs)
- Construction (annotation format v0 frozen, slot-array nodes, purity + pater)
- Distribution (amalgamation mechanism DEMONSTRATED with compiling artifacts)

**Next: the walking skeleton (M1)** per §6, now with accumulated obligations:
tiny annotated grammar (declarations + expressions), fresh expander core with
one est_custos guard file and one two-arm #ifdef, unified tokens end-to-end,
generated tables + construction, the genuine fork case (`foo * bar`, unknown
`foo` → localized AMBIGUUS → canonical reading → byte-exact roundtrip →
re-canonicalize on typedef injection), post-accept pater fixup, cursor.c
roundtrip, through the amalgamator, compiled standalone, with the crash-freedom
fuzz harness attached from day one.
