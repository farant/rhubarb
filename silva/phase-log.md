# Silva — Phase Log

***Lege silva/CLAUDE.md primum*** *— it holds the VISIO (vision pins), CULTURA,
and the full re-orientation protocol. This log is step 2 of that protocol.*

*Append-only. Each phase gets an INTENTIO entry (what we're implementing, what it
proves, exit criteria) written before work starts, and a RELATIO entry (how it
went, complexities discovered — discovered-while / consists-in / consequences /
handled-by) appended when it closes. English is fine, per worklog convention.
Design record: project-specs/silva-spec.md + silva-spec-v2.md (§§8–11 are the
simulation-resolved contracts; §11.2 is the M1 obligations list). This log is the
state re-entry point for any session landing mid-implementation.*

---

## M1 — Walking Skeleton: phase sketch (plan of record, revisable)

- **Phase 0 — Scaffolding.** silva/ layout (fontes/, grammatica/, instrumenta/,
  probationes/), compile script mirroring ./compile_tests.sh (no Cocoa/Security),
  credo wired, one trivial passing test. Proves: the build loop exists.
- **Phase 1 — Token + Lexer.** silva_token.h (unified contract: trivia,
  always-allocated tagged origo, fons_index, initium_lineae, scissurae,
  standard-tag) + silva_lexema (total lexer, single trivia scanner + rule-driven
  attachment pass). Tests: carried arbor2 lexer semantics (41-assertion bar) +
  the any-input byte-fidelity property + scissurae fixtures. Proves: the
  foundation contract under the invariant.
- **Phase 2 — Expander core.** Journal (fons,linea events + conditio_id),
  hidesets (shared-tail), layer = rescan generation, #define/#undef/#include,
  region scanner with est_custos guard detection, one guard file + one two-arm
  #ifdef end-to-end, two-track evaluation via the ported conditio evaluator.
  Proves: S10/S12/S13/S15 and the §10.2 architecture in real code.
- **Phase 3 — Generator.** Lapifex fork into instrumenta/: annotation reader
  (@slot, genus, modus, manu, id), tiny grammar (declarations + expressions,
  ambiguity present), conflict-preserving LALR, the NEW baking module (readable
  Latin-commented C out), generated construction dispatch + genus registry +
  layout tables + accessors + children(). Proves: annotation format v0 executes.
- **Phase 4 — GLR runtime.** Tagged SilvaValor, GSS with two-pass control +
  reducenda drain, localized AMBIGUUS merge (ported policy, real structural
  equality), accept-reconciliation ladder, post-accept pater fixup + oracle
  registration. The genuine fork case: `foo * bar`, unknown foo → AMBIGUUS with
  canonical spine → re-canonicalize on typedef injection. Proves: S21/S26 and
  the §9 contracts under fire.
- **Phase 5 — Scribere.** Byte-exact roundtrip emitter driven by the layout
  tables; cursor.c roundtrips byte-identically; trivia single-owner assertions
  on. Proves: the whole token→tree→bytes chain.
- **Phase 6 — Amalgamator.** Manifest (two orders, function exclusions),
  static+rename assembly, vanilla-C89 silva.h, host-pollution test,
  duplicate-static + nm-collision checks, standalone full-flag compile.
  Crash-freedom fuzz harness attached. Proves: the deliverable mechanism as
  §11.1 specifies.
- **Phase 7 — Skeleton integration.** The complete §11.2 checklist in one run;
  M1 RELATIO written; decide what widens first (grammar coverage vs expander
  directive coverage).

---

## Phase 0 — Scaffolding

### INTENTIO (2026-07-02)

Create the silva/ subproject skeleton: directory layout per spec-v2 §3.1; a
compile script (probationes runner) following compile_tests.sh's glob-discovery
and incremental-object pattern but self-contained to silva/ and without the
Cocoa/Security link flags; credo available (it depends only on
latina/chorda/piscina, which silva reaches as ordinary rhubarb includes during
development — vendoring only matters at amalgamation); one
probatio_silva_salve.c that asserts something trivial and passes. Exit: 
`./silva/compile_probationes.sh` runs green on a fresh checkout.

### RELATIO (2026-07-02)

Green. `silva/compile_probationes.sh` compiles the eight rhubarb dependency
objects (the seven future-vendored libs + credo) into silva/build/, discovers
probatio_*.c by glob with substring filtering, links without frameworks, and
runs. probatio_silva_salve: 5/5 assertions (harness arithmetic + chorda reach
into the root libraries). Incremental rebuild and the filter verified.

Complexities: only one, small —
- **chorda_ex_literis argument order** — discovered-while: writing the first
  test from memory; consists-in: the constructor takes (cstr, piscina), string
  FIRST, unlike most piscina-first constructors in the codebase;
  consequences: easy recurring slip when writing silva code that constructs
  chordae; handled-by: noted here; silva's own APIs will keep piscina-first
  convention consistently.

Also settled in passing: silva development-time tests reach rhubarb's
include/ and lib/ directly (vendoring is an amalgamation-time concern only),
and clangd's unused-include warning on stdio.h in tests is noise (imprimere
expands to printf) — house pattern keeps the include.

---

## Phase 1 — Token contract + Lexer

### INTENTIO (2026-07-02)

Two chunks, tested independently:

**Chunk A — silva_token.** The unified contract from sim ① as real structs:
SilvaToken carrying genus, valor (chorda view into source), position, fons_index,
standard-tag, initium_lineae, spatia_ante/post (trivia tokens, NIHIL default),
scissurae (splice offsets, NIHIL almost always), and an EMBEDDED SilvaOrigo —
an explicit tagged union (FONS carries nothing; EXPANSIO carries corpus/def-site
+ invocatio/use-site + nomen_macro + caecatio; PASTA carries sinister/dexter;
CHORDA carries primus; API carries nomen_macro). Embedding the origo (rather
than always-allocating a separate block) preserves the never-NULL property with
better locality and one fewer allocation per token. SilvaCaecatio = immutable
shared-tail list. Constructors per origo kind; chain queries (radix,
profunditas); caecatio extend/membership. Genus enum = full C89 lexical
vocabulary (arbor2 names kept where known: IDENTIFICATOR, INTEGER, NOVA_LINEA,
BRACE_APERTA, English keyword names) + robustness genera (OCTETUS_IGNOTUS,
STRING_IMPERFECTUM, CHARACTER_IMPERFECTUM). Exit: probatio_silva_token green —
constructors, two-generation chains, pasta dual parents, shared-tail caecatio.

**Chunk B — silva_lexema.** The total lexer: fork of arbor2_lexema.c reshaped to
emit SilvaToken directly, single trivia scanner + rule-driven attachment pass
(trailing = same-line through first newline; leading = rest), logical-line
initium_lineae (CONTINUATIO-aware), intra-token splicing with scissurae capture,
CRLF-preserving NOVA_LINEA, imperfect-literal genera instead of silent stops,
leading-dot floats. Exit: probatio_silva_lexema green covering the carried
arbor2 41-assertion trivia contract semantics + the any-input byte-fidelity
property (lex→emit reproduces input byte-exactly, including on garbage) +
scissurae and imperfectum fixtures.

### Chunk A — COMPLETE (2026-07-02)

silva_token.h + silva_token.c + probatio_silva_token: 50/50 assertions green
(full suite 2/2). The contract as designed in sim ① survived implementation
intact — embedded tagged-union origo, five constructors, use-site chain walking
(radix/profunditas through two generations), pasta dual parents, shared-tail
caecatio with structural sharing verified by pointer equality, enum↔name-table
alignment spot-checked at first/middle/last entries.

Complexities:
- **i32 is unsigned in rhubarb** — discovered-while: first compile, -Wsign-
  conversion on `byte_offset = -1`; consists-in: latina's i32 = unsigned int,
  s32 = signed int (i-prefix ≠ signed as in most codebases); consequences: any
  field carrying a sentinel -1 (byte_offset, fons_index) must be s32, and this
  will recur in every silva struct designed from memory of other codebases'
  conventions; handled-by: s32 for sentinel-bearing fields; convention noted
  here for all future silva structs.
- **The nomen landmine fired despite the warning** — discovered-while: writing
  the first test file; consists-in: named a local variable `nomen` (expands to
  `typedef`), the exact thing CLAUDE.md's first critical reminder forbids —
  produced 9 cascading parse errors; consequences: latina keyword macros make
  certain natural Latin identifiers (nomen, and watch si/per/dum as
  substrings-of-nothing but whole-identifiers matter) unusable, and knowing the
  rule abstractly did not prevent the slip under flow; handled-by: renamed to
  `titulus` (per CLAUDE.md's own suggestion); grep-for-`nomen`-as-identifier
  added to the personal checklist before every silva compile.

### RELATIO (2026-07-02) — PHASE 1 COMPLETE

Both chunks green. Full suite: 3/3 probationes, 136 assertions (50 token + 81
lexer + 5 harness). silva_lexema.c (~700 lines) delivers the total lexer: single
trivia scanner + rule-driven attachment pass, logical-line initium_lineae,
effective (splice-aware) reading with scissurae capture, imperfectum genera,
OCTETUS_IGNOTUS, CRLF-verbatim newlines, leading-dot floats, and the
byte-fidelity emitter (emittere(lexare(x)) == x verified on valid C, garbage,
NULs, splices, CRLF, unterminated everything).

Complexities:
- **arbor2's lexer hardcodes the Latin keywords** — discovered-while: reading
  the ancestor before forking; consists-in: VERBA_CLAUSA_C maps si→IF,
  nomen→TYPEDEF etc. directly in the lexer — that is the "native latina keyword
  mode" the silva interview explicitly DEFERRED; consequences: a faithful fork
  would have silently contradicted the design decision (latina flows through
  the expander as compiled-in macro definitions, with provenance);
  handled-by: silva's keyword table is English-only; the latina definition set
  is Phase 2's job. Divergence-from-ancestor recorded here deliberately.
- **Escape-vs-splice disambiguation falls out of consumption order** —
  discovered-while: implementing string scanning under effective reading;
  consists-in: a raw backslash might start an escape OR a splice, and C89 says
  splices are processed first; consequences: naive escape handling would eat
  spliced newlines as escape characters; handled-by: _sumere consumes any
  splices BEFORE reading an effective character, so an effective '\\' inside a
  literal is always a genuine escape — no special casing needed anywhere. The
  effective-reading layer also handles laminated comment openers/closers
  (/*<splice>*, *<splice>/) for free.
- **Trivia stay raw; only real tokens get scissurae** — discovered-while:
  deciding what a laminated block comment's valor should be; consists-in:
  trivia are never text-compared, only re-emitted, so their valor can be the
  verbatim byte range (splices included) with no clean-text synthesis;
  consequences: roundtrip is exact by construction for trivia and the
  scissurae machinery stays confined to identifiers/literals/punctuators;
  handled-by: _trivia_creare uses raw ranges; Scansio scissurae are discarded
  for comments.
- **OCTETUS_IGNOTUS is one token per byte** — noted: a megabyte of binary
  garbage would allocate a megabyte of tokens. Acceptable at Phase 1 (the
  fidelity property is what matters); the Phase 2 caps (spec §8.2) are where
  hostile-input volume gets bounded. Run-coalescing is a later optimization if
  measurement asks for it.

Also: -Isilva/fontes added to repo compile_flags.txt so clangd resolves silva
headers (editor diagnostics were false-alarming on every silva file).

---

## Phase 2 — Expander core

### INTENTIO (2026-07-02)

Four chunks, tested independently; spec contracts: §8 (journal, hidesets, layer
= rescan generation), §10.2 (two-track conditional architecture).

**Chunk A — Journal + directives + macro definitions.** SilvaExpansio context
(fontes table, live macro table = default track, journal of SilvaEventum keyed
(fons, linea) with conditio_id reserved); directive recognition via
initium_lineae (one local check — the arbor2 leak class gets its regression
test, green); #define object-like and function-like (adjacency rule: `(` at
name's raw byte end = function-like), #undef; macro bodies as SLICES of the
source token stream (def-site provenance free, per the unified-token design);
macros_ad_lineam by journal replay into a caller piscina. Exit:
probatio_silva_expansio green incl. consecutive-column-1-directives and
continuation-spanning directives.

**Chunk B — Expansion generations.** One generation = one full C89 expansion
step: identifier lookup against live table + hideset check, function-like arg
collection (EOF-bounded), ARG PRE-EXPANSION within the step (S13), substitution
emitting tokens via silva_token_ex_expansione (corpus token + invocatio token +
extended hideset), fixpoint across generations by hideset semantics (cap
demoted to assertion). `#define A A` fixpoints at generation 1.

**Chunk C — # and ## in substitution.** Stringify (operand taken raw, whitespace
collapse, escaping) via silva_token_ex_stringificatione; paste (re-lex glued
text, best-effort, ERROR-shaped token on failure) via silva_token_ex_pasta.

**Chunk D — Includes + regions + two-track conditionals.** Learning/full
includes; region scanner with est_custos guard detection (transparent + fons
metadata); conditio evaluator port with protected pre-expansion; arm-scoped
journal events (conditio_id goes live); the skeleton's one guard file + one
two-arm #ifdef end-to-end.

### Chunk A — COMPLETE (2026-07-02)

silva_expandere.h/.c + probatio_silva_expansio: 37/37 assertions, first
compile+run (full suite 4/4, 173 assertions). The journal design (S10) is real:
SilvaEventum acta with (fons, linea) keys and conditio_id reserved;
macros_ad_lineam replays into a caller piscina — historically accurate where
arbor2's live-pointer "snapshot" never was. Directive recognition is ONE local
check (# with initium_lineae) — **the arbor2 leak regression test is written
and green**: consecutive column-1 #defines both register, laminated directives
span their logical line, stray mid-line # is untouched. Function-like detection
by raw byte adjacency (paren->byte_offset == titulus->byte_offset + longitudo)
— works through splices for free since longitudo is raw. Macro bodies are
slices of the source token stream: def-site provenance costs nothing.

Complexity: none new — Chunk A landed exactly as simulated. (The sim + spec
pipeline is visibly paying for itself: this was the module with the most
accumulated design, and it had the least friction.)

### Chunk B — COMPLETE (2026-07-02)

The expansion engine: 36/36 assertions, first compile+run (suite 5/5, 209
assertions). silva_expansio_generatio (one C89 expansion generation) +
silva_expansio_expandere (fixpoint loop, strata recorded per mutating
generation — the layer model's data). Hidesets do the load-bearing work:
`#define A A` terminates after exactly ONE mutating generation with the blue A
carrying caecatio {A}; mutual recursion (A→B→A) terminates in two with the
final token's provenance chain two deep. Arg pre-expansion per S13 (args
expanded to fixpoint within the step via _expandere_plene recursion);
invocation-with-space works; unterminated invocation degrades to plain
identifier without consuming (robustness posture); the latina flagship case —
`#define si if` — expands with genus IF and provenance radix back to the
source `si` token.

Complexities:
- **Hideset union is O(n·m) per replacement token** — discovered-while:
  implementing Prosser's hsadd for argument tokens (own caecatio ∪ HS');
  consists-in: shared-tail lists union by walking one list and consing missing
  elements onto the other — per-token cost scales with hideset depth;
  consequences: pathological deeply-nested macros pay quadratic-ish caecatio
  work; real code's depths are tiny (the sim predicted this); handled-by:
  accepted; the measurement harness watches it; note Prosser's exact
  (HS_call ∩ HS_rparen) refinement is simplified to HS_call — over-blues one
  exotic case class, recorded as a deferred exactness item for Chunk C/D era.
- **Object-like body tokens needed no union at all** — their caecatio IS HS'
  (bodies are FONS tokens with empty hidesets); only argument tokens union.
  Small pleasant asymmetry that kept the common path cheap.

### Chunk C — COMPLETE (2026-07-02)

# and ## in substitution: 23/23 assertions (suite 6/6, 232 assertions).
_substituere rewrote into a body-walker with lookahead: stringify (# param,
functio-similis only) takes the RAW argument per C89 — whitespace runs
collapse to one space, " and \ escaped, empty-argument provenance roots at the
invocation token; paste (##) takes raw operands, glues text, RE-LEXES
(operator formation works: < ## < → SINISTRORSUM), chains left-associatively
(a##b##c), handles object-like bodies and empty sides (placemarker-style), and
degrades best-effort on invalid pastes (glue relexing to two tokens emits
both, no crash). The rescan test is the satisfying one: J(A,B) pastes AB in
one generation, the NEXT generation expands AB→9, and the provenance chain
reads EXPANSIO(AB)→PASTA(A,B) — layers and paste provenance cooperating
exactly as the token contract intended.

Complexity:
- **Paste output wrapping is two-tier** — discovered-while: emitting the
  post-paste item series; consists-in: pasted tokens already carry PASTA
  origo + hideset, but surrounding tokens of multi-token operands still need
  ex_expansione wrapping — wrapping the pasta token again would bury its
  dual-parent provenance under an expansio layer; consequences: the emitter
  must distinguish freshly-pasted tokens from carried operand tokens;
  handled-by: origo.genus == PASTA check at emission (pasta tokens pass
  through, everything else wraps). Works, but it is order-sensitive — noted
  for the Chunk D review pass.

### Addendum (2026-07-02, surfaced during pre-compaction review)

- **Layer-N emission spacing is undecided (silent Chunk B decision, now
  explicit)** — discovered-while: Fran asked what I'm worried about;
  consists-in: silva_token_ex_expansione zeroes spatia_ante/post, so expanded
  tokens carry no trivia — layer-0 roundtrip is unaffected (the oracle
  passes), but rendering an EXPANDED layer as text has undefined spacing;
  consequences: none for the skeleton, but scribere (Phase 5) and layer-view
  queries will need a policy; the def-site trivia IS reachable
  (origo.datum.expansio.corpus->spatia_*) so nothing is foreclosed — merely
  undecided; handled-by: recorded here; decide when scribere exists (options:
  synthesize single spaces, borrow def-site trivia, or make layer emission a
  formatter concern).

### Chunk D — design decisions (2026-07-02, pre-implementation)

Recorded before coding so the narrowings are explicit, not silent:

1. **Journal = default track only (skeleton).** Spec §10.2 says "journal + region
   tree retain every arm"; for the skeleton the split is: taken-arm state
   mutations are applied + journaled (tagged with the arm's conditio_id);
   NON-taken arms are retained solely as raw partitioned token slices on the
   region tree (no journal events). This keeps macros_ad_lineam replay correct
   with zero configuration-awareness. Config-scoped re-evaluation of untaken
   arms = query-time work, post-M1.
2. **False arms are depth-counted but not sub-scanned.** Nested #if inside a
   non-taken arm is counted for correct #endif matching but produces no child
   region nodes. Structural scan of untaken arms deferred to the config-query
   milestone.
3. **Includes are API-fed, never filesystem-fed.** New
   silva_includendum_praebere(exp, via, textus, mensura) registers content;
   #include resolves against that table. Found → recursive directive-pass,
   reliqua spliced at the include point (its EOF dropped). Not found →
   "learning": recorded in exp->inclusiones with fons_ad = -1, processing
   continues (system headers absent by design). Depth cap 32. Only the two
   literal operand forms ("via", <via>) — macro-expanded include operands
   deferred.
4. **Guard detection = strict shape pre-scan** before the general walker:
   first non-trivia token is `#ifndef X`, next logical line `#define X`,
   matching #endif followed by nothing but EOF. Matched → no region node,
   interior processed unconditionally starting AT the #define line, guard
   recorded on SilvaFons (est_custos + custos_titulus). Re-include of a guard
   file whose custos is defined → skipped entirely (multiple-include
   optimization = correctness here, since the interior isn't reprocessed).
5. **Rename: silva_expansio_definitiones_colligere →
   silva_expansio_directivas_processare.** It now handles define/undef/
   include/conditionals; the old name understates. API honesty before the
   name calcifies (3 test call sites).
6. **arbor2 evaluator bug found at port time**: arbor2_conditio_evaluare
   computes in i64 — which latina defines as UNSIGNED long long — so
   `#if -1 < 0` is FALSE in arbor2 (unsigned comparison). Silva's port uses
   s64 throughout. Also upgrading: arbor2 could only expand single-token
   object-like macros inside #if; silva does the C89-correct pipeline
   (replace defined-operands with synthetic 0/1 → full macro expansion via
   the real expander → arithmetic eval, leftover identifiers = 0).
7. **Unmatched #elif/#else/#endif pass through to reliqua** untouched
   (byte-conservative; the parser will see them as opaque directives).
8. **Arms after the taken arm are not evaluated** (est_evaluatum FALSUM) —
   cpp semantics; their expressions may reference undefined state.

### Chunk D — COMPLETE (2026-07-02)

Two new modules + the directive-pass rewrite, first-compile green across the
board: probatio_silva_conditio 63/63, probatio_silva_regiones 84/84 (suite
8/8, 379 assertions total; all pre-Chunk-D suites unchanged — the rewritten
walker is behavior-compatible for the Chunk A cases).

What landed:
- **silva_conditio.h/.c** — the #if/#elif evaluator. Recursive descent ported
  from arbor2 with the s64 fix, plus the real pipeline: defined-operands
  substituted protected (synthetic INTEGER via ex_api, provenance = queried
  name) → FULL macro expansion via silva_expansio_expandere (function-like
  macros in conditions work: MAX(3,5)==5) → arithmetic eval, leftover
  identifiers = 0. Shift-out-of-range guarded (UB avoided). All errors →
  0 + successus FALSUM, no crash.
- **Directive pass rewritten** (silva_expansio_directivas_processare):
  guard pre-scan → _fluxum_processare walker → _regionem_processare /
  _includendum_processare. Regions: arms evaluated in order, taken arm's
  tokens recursively processed (defines journaled with the arm's
  conditio_id), untaken arms retained as raw partitioned slices,
  post-taken arms not evaluated. Nesting: taken arms produce filiae;
  untaken arms depth-counted only. #if 0 → est_numquam. EOF before #endif
  → est_imperfecta, no crash.
- **Custodes**: strict-shape guard detection; guard files produce NO region;
  est_custos + custos_titulus on SilvaFons; re-include with defined custos
  skipped entirely. Near-guard (content after #endif) correctly demoted to
  a normal region.
- **Includenda**: silva_includendum_praebere (API-fed content), splice at
  include point (included EOF dropped), unknown → learning record
  (exp->inclusiones, fons_ad -1), self-include cap 32 verified (97 tokens,
  terminates).
- **Skeleton obligation (spec-v2 §10.3) met end-to-end**: config.h guard
  file + #if MODUS==2 two-arm region + expansion → int x = 42 with
  EXPANSIO provenance, one region retained.

Complexities:
- **#if and #else lex as KEYWORDS, not identifiers** — discovered-while:
  designing the directive classifier before coding; consists-in: the lexer
  tags `if`/`else` with genus IF/ELSE even in directive position, so a
  faithful port of arbor2's classify-by-identifier-valor would NEVER match
  #if/#else (silently — they'd fall through as unknown directives);
  consequences: any directive-name comparison must special-case the two
  keyword genera; handled-by: _directivae_genus checks genus IF/ELSE before
  the identifier-valor ladder; caught at design time, so it never shipped
  broken.
- **Xar growth invalidates held slot pointers across recursion** —
  discovered-while: writing _includendum_processare (inclusio record +
  recursive include processing); consists-in: xar_addere may relocate the
  backing store, so a SilvaInclusio* held across a recursive call that
  itself appends inclusiones dangles; consequences: a
  write-after-recursion pattern would corrupt silently and only under
  nested includes; handled-by: all record fields written BEFORE recursion
  (comment in code); general rhubarb pattern worth remembering — never
  hold an Xar slot pointer across anything that can append to that Xar.
- **fons->via pointed at caller memory** — discovered-while: writing
  silva_includendum_praebere (tabula key must outlive the call);
  consists-in: silva_fons_addere stored a chorda view of the caller's
  cstring instead of copying; consequences: latent lifetime bug for any
  caller that frees/reuses the path buffer (solarium will); handled-by:
  fons_addere now duplicates the path bytes into the piscina.
- (From the design notes, confirmed in practice: the arbor2 i64-unsigned
  evaluator bug — `#if -1 < 0` false — now has a permanent regression
  test in probatio_silva_conditio.)

Debts carried forward (unchanged): compatibility-suite port, measurement
harness, Prosser HS_call∩HS_rparen exactness, pasta pass-through
order-sensitivity review (checked briefly this chunk: the PASTA-origo
emission check is exercised by the rescan tests and behaved; a dedicated
adversarial test still owed), macro-expanded #include operands, structural
scan of untaken arms, config-aware journal replay.

### RELATIO (2026-07-02) — PHASE 2 COMPLETE

All four chunks green: journal + directives (37), generations/hidesets (36),
stringify/paste (23), conditio evaluator (63) + regions/guards/includes (84).
Suite 8/8, 379 assertions. The expander core now does: #define/#undef (object,
function-like, adjacency rule), full C89 expansion generations with Prosser
hidesets, # and ## with provenance, #include (API-fed, learning mode, guard
skip), #if/#ifdef/#ifndef/#elif/#else/#endif with two-track regions, guard
transparency, and point-in-time macro state replay. Every module landed
first-or-second compile; the sim ①/② designs survived contact intact.
Next: Phase 3 — the generator fork (largest untested bet, per the standing
worry list).

### Addendum — corpus fidelitatis adopted + v1 suite harvest plan (2026-07-02)

Fran asked about the arbor v1 test suites; investigation confirmed v1 is the
functionality high-water mark (syntaxis 234 / praeparator 124 / formator 103 /
quaestio 100 assertions + typus + index — the only test corpora in existence
for silva's selector/formatter/type/index milestones). Two outcomes:

1. **probatio_silva_fidelitas added**: runs the byte-fidelity oracle
   emittere(lexare(x)) == x over ALL of probationes/fixa/roundtrip/ (the
   shared v1+v2 regression corpus — 78 files incl. latina.h, utf8.c, the
   comment_expr_* series). **78/78 fidelis on first run** — the total lexer
   holds against two generations of accumulated regressions, three phases
   before scribere. compile_probationes.sh now exports RHUBARB_RADIX so the
   test finds the corpus. Suite: 9/9, 458 assertions.
2. **Harvest plan + freeze-then-delete refinement recorded in spec-v2 §4.1**:
   Tier 1 roundtrip corpus (adopted), Tier 2 praeparator/syntaxis inputs
   (Phase 4-era), Tier 3 quaestio/formator/typus/index as post-1.0 milestone
   acceptance bars (quaestio needs English→Latin tag mapping). The
   freeze-then-delete rule is refined: engines die at substrate parity, but
   each v1 downstream SUITE is deleted only after its bar is harvested at
   its milestone — the original rule would have deleted the acceptance bars
   before their milestones existed.

---

## Phase 2.5 — Closure pass (compatibility bar + measurement)

### INTENTIO (2026-07-02)

Close Phase 2 honestly before opening Phase 3 (the biggest untested bet).
Three pieces:

1. **Port the arbor2 compatibility suites** — probatio_arbor2_lexema (41
   assertions) and probatio_arbor2_expandere (~100 assertions) as
   probatio_silva_compat_lexema / probatio_silva_compat_expansio, section
   by section against silva's API. Reading the suites surfaced three real
   gaps the port FORCES into existence (the point of carrying tests):
   - **API macro injection** (silva_macro_addere / silva_macro_functio_addere)
     — the ex_api origo existed with no public entry; compiled-in latina.h
     definitions ride on this (VISIO: layers/provenance for `si`→`if`).
   - **Variadic __VA_ARGS__ substitution** — est_variadica flag existed but
     no binding; C99-signed feature, six carried tests need it. Design:
     ELLIPSIS appends a "__VA_ARGS__" parameter; argument collection stops
     splitting at the named-parameter count so the variadic tail stays ONE
     argument with its REAL comma tokens (no synthesis).
   - **silva_expansio_prospectare** — macro lookahead (first-body-token
     genus + est_vacuum/est_recursivum), the GLR fork-prioritization oracle
     from the v2 brainstorm.
   Deliberate non-ports, recorded: typedef collection (silva puts the
   typedef oracle at the parser, Phase 4 — tests deferred there);
   filesystem include paths (silva is API-fed by design — include SEMANTICS
   tests port against praebere, path management does not).
2. **Measurement harness** — probatio_silva_mensura: lex + directive-pass +
   expand the fidelity corpus, report per-file time (clock()) and peak
   arena bytes (piscina_summa_apex_usus); totals always, per-file under
   MENSURA_VERBOSA. "Measure from day one" finally starts, with a
   pre-parser baseline.
3. **Pasta adversarial case** — PASTE(,bar) (empty left arg) is in the
   carried suite; the order-sensitivity review debt from Chunk C closes
   with it.

Also fixing while in there: praebere/API-injection duplicate their input
TEXT into the piscina (same caller-lifetime latency the via fix addressed —
token valor is a VIEW into the source buffer).

Exit: suite green with both compat files + mensura; RELATIO records any
behavior differences discovered between arbor2 and silva semantics.

### RELATIO (2026-07-02) — PHASE 2.5 COMPLETE

Suite 12/12, 626 assertions. The closure pass did exactly what it was for:
**the carried arbor2 suite caught a real cpp-fidelity bug in silva's
architecture** that all 379 fresh assertions had missed.

**The bug (the headline):** silva's two-phase design (directive pass →
expansion pass) expanded every token against the FINAL macro table. For
`#define TEMP 42 / TEMP / #undef TEMP / TEMP`, cpp expands the first TEMP
and leaves the second; silva expanded NEITHER (final table has TEMP
undefined). Redefinition mid-file was equally wrong (both uses got the
last value). arbor2 was right by interleaving; the carried tests
encoded it; my fresh tests never probed a #undef-between-uses.

**The fix — point-in-time expansion via the journal**, which S10 designed
for exactly this without knowing it: every SilvaEventum now records
`positus` = the reliqua-stream length at consumption time (works across
include splices for free, since events land in consumption order). New
`silva_expansio_expandere_reliqua` walks the stream with an event CURSOR,
building a working table incrementally — each token sees the macro state
of its own position, O(n + events) per generation. Positions are remapped
to output coordinates per generation (local copy, acta untouched).
`exp->tabula_activa` routes quaerere to the working table so NESTED
argument pre-expansion inside an invocation sees the invocation-point
state too. Plain `silva_expansio_expandere` keeps live-table semantics —
correct for argument/condition series during the directive pass; header
documents which entry is for what. Pipeline for files is now:
lexare → directivas_processare → expandere_reliqua.

**New API forced into existence by the port (as predicted):**
- silva_macro_addere / silva_macro_functio_addere — API macro injection;
  body text is duplicated into the piscina and lexed under a lazy
  synthetic fons "<api>", so injected-macro expansions carry FULL normal
  provenance (def-site tokens are real FONS tokens). This is the entry
  point compiled-in latina.h definitions will use.
- Variadic __VA_ARGS__ substitution — "..." appends a __VA_ARGS__
  parameter; argument collection now takes scissiones_maximae so the
  variadic tail stays ONE argument with its REAL comma tokens (zero
  synthesis; provenance intact). Six carried tests green, including
  ARGS() empty and __VA_ARGS__-used-twice.
- silva_expansio_prospectare — macro lookahead (first-body-token genus,
  est_vacuum, est_recursivum), the GLR fork-prioritization oracle;
  returns FALSUM for non-macros (arbor2 used an ERROR genus; silva has
  none, deliberately).
- Pasta adversarial case closed: PASTE(,bar) → bar, no crash (Chunk C's
  order-sensitivity debt).

**Carried bars:** probatio_silva_compat_lexema 40/40 (NOVA_LINEA/
CONTINUATIO in spatia, CRLF longitudo, directive pattern, BOF newlines,
genus_nomen strings — silva's names match arbor2's, tests port verbatim);
probatio_silva_compat_expansio 127/127 (every behavior section of the
arbor2 suite; typedef-collection sections deferred to Phase 4 oracle,
filesystem path management not ported by design — include SEMANTICS
ported against praebere, incl. NEW nested-include coverage a.h→b.h).

**Measurement baseline (probatio_silva_mensura, runs in suite; per-file
under MENSURA_VERBOSA):** 78 files / 75,143 B: lexare 24.6 ms
(0.335 ms/KB), directivae+expansio 4.6 ms (0.062 ms/KB), apex arenae
medius 262 KB / maximus 3.3 MB. First observation for the record:
LEXING dominates at 5× expansion cost — trivia-token allocation is the
likely driver; no action (no budget in 1.0), curve now visible.

Complexity:
- **Two-phase purity vs stream-order state** — discovered-while: porting
  the arbor2 #undef tests; consists-in: separating the directive pass
  from expansion silently changed WHEN state is read (final vs
  point-in-time) — a semantics change disguised as an architecture
  choice; consequences: default track diverged from cpp exactly where
  VISIO forbids it ("canonical parses agree with what a real compiler
  sees"); handled-by: journal positus + cursor expansion (above). The
  meta-lesson joins the register: an architectural decomposition can
  change semantics without any single component being wrong — only
  carried end-to-end tests catch that class.

Phase 2 now closes with the full carried bar. Next: Phase 3, generator
fork — first act is reading lapifex_generare.c end to end.

## Phase 3 — Generator (fork + annotations + baking + generated construction)

### INTENTIO (2026-07-02)

**The promised end-to-end read of lapifex_generare.c (2,724 lines) is done**
— the "biggest untested bet" is de-risked by direct knowledge. Findings:

- **Conflict preservation confirmed in the flesh**: actio_iam_existit dedupes
  identical actions only; different actions on one terminal coexist,
  conflictus_intentus flagged post-hoc. §4's "Carry" verdict is real.
- **Gap found: the reader cannot express EPSILON productions** — empty
  <productio> text is skipped (line 279). FIRST/FOLLOW already handle
  epsilon (habet_epsilon machinery is complete); only the reader is blind.
  Silva's C89 grammar needs epsilon. Fork adds: empty <productio/> =
  deliberate epsilon.
- **Gap found: zero validation** — a terminal as rule LHS is silently
  accepted; no duplicate checks. Matches R4 ("the generator IS the
  validator" — STML has no schema facility); silva's annotation validation
  (S19/S31) is the fix, not an afterthought.
- Known-slow spots for later measurement: statum_invenire linear-scans with
  full set compares; LALR propagation scans all transitions per item (the
  worklog's ~2s C89 table build). Dev-time only (tables are baked once) —
  optimize only if measurement demands.
- The code is textbook dragon-book, clean, ~90% mechanically forkable.

**Plan (chunks):**

- **Chunk A — fork + annotation reader + validation.** Fork into
  silva/instrumenta/ as silva_generare.{h,c}, prefix silva_gen_ (mechanical
  rename, then extend). NEW in the reader: symbol@slot / @slot+ inline
  parsing (S18), genus/id/modus/manu attributes on <productio>,
  <genera-extra> registry, epsilon productions, validation pass (S19: >1-
  symbol productions must declare genus + own every terminal; id mandatory
  with genus + unique; LHS must be non-terminal). Slot-kind UNIFICATION
  validation (S20) deferred to Chunk D where layouts are computed.
  compile_probationes.sh gains instrumenta/*.c + stml dependency (generator
  is dev-time only — NEVER amalgamated; directory is the firewall).
- **Chunk B — tiny annotated grammar, tables green.** The frozen v0 sample
  (sim ③) + a declarations/expressions grammar with deliberate ambiguity;
  verify conflict-preserving tables show the expected conflictus.
- **Chunk C — baking.** Emit tables as readable Latin-commented C source
  (static arrays; action entries carry pretium + RECUPERARE kind per §8.2).
- **Chunk D — generated construction.** Genus registry (grammar +
  genera-extra merged), slot layout tables (S20 unification + kind-conflict
  errors here), typed accessors, children(), dispatch. Debug assertions
  (S32) generated.

Naming: SilvaGen*/silva_gen_* — generator-side namespace stays visibly
distinct from the shipped parser's names.

### Chunk A — COMPLETE (2026-07-02)

Fork + annotation reader + validation: probatio_silva_generare 52/52
(suite 13/13, 678 assertions). The mechanical fork worked exactly as
sim ④'s renamer experiment predicted: perl prefix-rename of 2,724 lines
compiled clean under the full flag set with ONE manual fix (an
over-renamed include filename). silva/instrumenta/ is the dev-time
firewall — never amalgamated; compile script gained instrumenta/*.c +
stml/selectio deps + the include path (repo compile_flags.txt too).

What the fork now does beyond lapifex:
- **Annotation reader (forma v0 congelata)**: symbolum@locus inline
  parsing (atomum_scindere splits at '@', trailing '+' = appendere),
  genus/id/modus/manu attributes on <productio>, loci Xar parallel to
  dextrum, <genera-extra> registry (slots kept raw until Chunk D
  layouts). The frozen sim ③ sample grammar is the primary test fixture
  verbatim.
- **Epsilon productions**: empty <productio/> = deliberate epsilon
  (lapifex skipped them silently — reader gap found in the end-to-end
  read). Verified through FIRST (habet_epsilon) and full table build.
- **Validation pass (generator est schema, R4/S19)**: multi-symbol
  production without genus = error; terminal without @locus in a genus
  production = error (single-owner trivia invariant, enforced BEFORE any
  code runs); id mandatory with genus + unique; terminal-as-LHS = error
  (lapifex accepted it silently); manu = escape hatch, skips slot
  validation. All error paths return NIHIL, no crash.
- **Conflict preservation verified as a silva test**: E→E PLUS E | NUMBER
  yields numerus_conflictuum > 0 AND a multi-action cell (SHIFT+REDUCE on
  PLUS via silva_gen_actiones_quaerere) — the keystone recombination
  premise now has a green assertion in silva's own suite.

Complexity: none new — the read-before-fork paid for itself (both reader
gaps were known before any code was written; nothing was discovered
mid-implementation).

Next: Chunk B (declarations+expressions grammar with typedef-shaped
ambiguity, conflict expectations pinned), then C (baking), D (generated
construction + layouts + S20 unification).

### Chunk B — COMPLETE (2026-07-02)

The walking-skeleton grammar exists as a real on-disk artifact:
**silva/grammatica/sceletum.stml** (grammar on disk, not string literal —
interview decision), fully annotated in frozen v0: 9 terminals, 9 rules
(17 productions + augmented), lists via epsilon + lista-appendere,
binarium genus shared across expressio/terminus productions (the S20
unification case Chunk D will exercise), genera-extra carrying
error/ambiguus/conditionalis.

probatio_silva_grammatica 16/16 (suite 14/14, 694 assertions), reading
the file from disk via RHUBARB_RADIX. **The headline result: the LALR
table has exactly ONE conflict in 27 states, and it is precisely the
designed one** — state 7, reduce/reduce on STAR between typus-nomen
(declaration reading: "foo" is a typedef name) and factor-identificator
(expression reading: "foo" is a value). That cell IS `foo * bar;`. The
test pins it by production id, not state number, so grammar edits that
move the state keep the assertion honest. Phase 4's GLR will fork on
exactly this cell and reconverge into AMBIGUUS with a canonical spine.

Complexities: none — one test-side arithmetic slip (miscounted my own
grammar's productions), fixed on sight. The surgical-ambiguity result
(zero incidental conflicts) is worth noting as evidence the annotation
format doesn't force conflict-introducing factoring.

Next: Chunk C — baking (emit tables as readable Latin-commented C
source; action entries carry pretium + RECUPERARE kind per §8.2).

### Chunk C — INTENTIO (2026-07-02)

Baking: the first fully-NEW module (nothing to fork — lapifex never had
it; the ~7s runtime table build is what it kills). Pieces:

1. **fontes/silva_tabulae.h** (hand-written runtime contract): SilvaTab*
   structs — flat static arrays, no Xar/piscina at runtime. Action
   entries carry `pretium` (i32, 0 for normal actions) and the enum has
   SILVA_TAB_ACTIO_RECUPERARE — the §8.2 cost-ready-recovery obligation
   discharged IN THE FORMAT now; RECUPERARE rows are emitted only when
   recovery work lands. Per-state action/goto slices via offset+count
   into two flat arrays. SilvaTabProductio carries sinistrum, longitudo
   (pop count), and id (diagnostics; full construction layout tables are
   Chunk D's separate generated tables).
2. **instrumenta/silva_coquere.{h,c}**: the emitter. fprintf-based
   (dev-time tool). Emits GENERATUM header (source grammar, counts,
   conflict count), symbol table (terminal genus attribute emitted
   VERBATIM as the C enum constant — that's why the grammar stores
   "SILVA_LEX_..."), production table, per-state action rows each with a
   generated comment naming the terminal and target by PRODUCTION ID
   (R3: generated comments cannot rot). Actions sorted per state for
   stable regeneration diffs (files are committed).
3. **instrumenta/principalia/generator.c** + **silva/generare.sh**: the
   dev-time main. principalia/ subdir keeps mains OUT of the probationes
   link glob (instrumenta/*.c doesn't descend). generare.sh compiles and
   runs it: sceletum.stml → fontes/silva_tabulae_sceleti.{h,c},
   COMMITTED generated files (they are part of the eventual silva.c).
4. **probatio_silva_tabulae.c**: the honest loop — the suite compiles
   the baked files (fontes glob) and verifies them against a FRESHLY
   built in-memory table: state/production/symbol counts, per-state
   action counts, initium, and the fork cell (state with two REDUCERE
   on STAR whose valores are typus-nomen and factor-identificator, found
   by id in the baked production table).

Note carried: the generator uses -1 ($) as lookahead for reduce/accept
AND has an explicit EOF terminal (augmented production shifts it). Both
are baked verbatim; the EOF↔$ mapping is Phase 4 adaptator business.

### Chunk C — COMPLETE (2026-07-02)

Baking works end-to-end: probatio_silva_tabulae 18/18 (suite 15/15,
712 assertions). The ~7s runtime table build is now structurally dead —
tables are compile-time constants.

What landed:
- **fontes/silva_tabulae.h** — runtime contract as designed (flat static
  arrays, per-state offset+count slices, pretium field + RECUPERARE
  enum kind = §8.2 format obligation discharged).
- **instrumenta/silva_coquere.{h,c}** — the emitter. Generated output is
  genuinely readable: every action row carries a generated comment
  ("STAR: reducere P6 typus-nomen"), conflict states are marked
  [CONFLICTUS SERVATUS], terminal genus attributes emit verbatim as C
  enum constants (why the grammar stores "SILVA_LEX_..."). Actions and
  gotos sorted per state — stable regeneration diffs for committed
  files. Empty-array trap avoided with a terminator row (C89 forbids
  empty initializer lists).
- **instrumenta/principalia/generator.c + silva/generare.sh** — the
  dev-time main; principalia/ keeps mains out of the probationes link
  glob. generare.sh runs repo-relative so the GENERATUM header carries
  a stable path, not a machine-specific one (caught on first output and
  fixed — committed generated files must not embed local paths).
- **fontes/silva_tabulae_sceleti.{h,c}** — the first COMMITTED generated
  artifact: 27 states, 18 productions, 19 symbols, 1 preserved conflict.
  Compiles under the full -Werror flag set as a fontes citizen.
- **probatio_silva_tabulae** — the honest loop: baked vs freshly-built
  comparison (counts, symbols by name, productions by id/length,
  per-state action slices + offsets, initium) and the FORK CELL verified
  in baked form (two REDUCERE rows on STAR resolving to typus-nomen and
  factor-identificator, found by id in the baked production table).
  If the grammar changes without regeneration, this probatio breaks —
  the drift guard for generated artifacts.

Complexity: none structural. One catch at first output (absolute path
in the GENERATUM comment) — noted above, fixed by running generation
repo-relative.

Next: Chunk D — generated construction (genus registry from grammar +
genera-extra, S20 slot unification with kind-conflict errors, layout
tables, typed accessors, children(); debug assertions per S32). This is
the largest new piece and closes Phase 3.

### Chunk D — INTENTIO (2026-07-02)

Design decisions made at planning (recorded before code):

1. **List rules are their own validation category.** modus= productions
   yield LIST VALUES, not nodes — so S19's "multi-symbol needs genus"
   exempts them (genus on a list rule is meaningless; my Chunk B grammar
   wrongly carried one — fixed + regenerated). Their terminals must
   STILL be owned: mapped values in a lista-appendere rule are APPENDED
   (elements and separators alike — that IS the spec's "separated lists
   interleave separator tokens"). Semantics: result = the @slot+ value;
   every other mapped value appends in RHS order; unmapped non-terminal
   values are dropped.
2. **children() needs no registry** — values are tagged, so iteration
   walks the node's loci and yields NODUS values (and NODUS elements of
   LISTA values) directly. The registry serves accessors, serialization,
   and species checking; numerus_locorum is stored on the node.
3. **Species inference**: terminal@slot → TOKEN; nonterminal@slot →
   NODUS, unless the nonterminal is list-valued (any of its productions
   carries modus) → LISTA_MIXTA. genera-extra slots carry explicit
   species ("tokens:lista-token"). Same-genus same-slot species conflict
   = generation error (S20). One species enum, defined in the RUNTIME
   header (silva_nodus.h) and shared by the generator.
4. **S32 checks live in runtime helpers** (silva_nodus_ponere: tag-vs-
   species + store-once), so generated code stays thin and the checks
   can't be forgotten per-production.

Pieces: fontes/silva_nodus.{h,c} (SilvaValor tagged union, uniform
SilvaNodus — pater present but NEVER set in construction per S27,
checked store, children()); registry computation in silva_generare
(S20); emission in silva_coquere (genus enum, layout arrays, registrum,
constructio dispatch, typed accessors); regenerated sceletum tables;
probatio driving construction by hand (both readings of `foo * bar;`)
before any GLR exists.

### Chunk D — COMPLETE (2026-07-02); PHASE 3 COMPLETE

probatio_silva_constructio 44/44 first run (suite 16/16, 756
assertions). **Annotation format v0 executes**: the generated
constructor builds real trees from real tokens.

What landed:
- **fontes/silva_nodus.{h,c}** — SilvaValor (tagged: NIHIL/NODUS/TOKEN/
  LISTA/INDEX) + uniform SilvaNodus (genus, loci array, pater). S32
  checks live in silva_nodus_ponere (tag-vs-species + store-once +
  bounds) so generated code stays thin and checks can't be forgotten.
  children() needs NO registry — values are tagged, so it walks loci
  and yields NODUS values and NODUS list elements directly.
- **silva_gen_registrum_computare (S20)** — slot unification across
  same-genus productions with species inference (terminal→TOKEN,
  nonterminal→NODUS, list-valued nonterminal→LISTA_MIXTA, @slot+→
  LISTA_*); genera-extra slots parsed from "name:species"; species
  conflicts and grammar/extra genus collisions are generation errors.
- **Emission (silva_coquere)** — the generated pair now carries: genus
  enum (SILVA_SCELETUM_GENUS_BINARIUM...), layout arrays + REGISTRUM,
  checked accessors per genus×slot (silva_sceletum_binarium_sinister:
  genus + bounds checked, alien genus → nihil valor), manu prototypes,
  and silva_sceletum_construere — the dispatch that pattern-matched
  every production shape: node (create+ponere per mapping), transitus
  (pass-through), lista-initium (new list), lista-appendere (append to
  @+ value, other mapped values in RHS order), manu (call), epsilon
  (nihil). ~380 lines of generated C, all first-compile after one
  terminator signedness fix.
- **Validation amendment**: modus productions exempt from the genus
  requirement (they yield lists, not nodes); their terminals still
  need owners.
- **The probatio is the M1 dress rehearsal for Phase 4**: BOTH readings
  of `foo * bar ;` constructed by hand through the generated code —
  expression (folium→transitus→binarium→sententia-expressionis→lista)
  and declaration (typus-nominatus + declarator-monstrator →
  declaratio) — with accessors, children()==2 (tokens are not
  children), pater NIHIL everywhere (S27 verified), and S32 violations
  (wrong tag, double store, out of bounds) all rejected.

Complexity: one — the generated GENERA terminator emitted -1 into
unsigned i32 fields (caught by -Werror on the generated file itself:
the full flag set gates GENERATED code too, which is exactly the
point). Fixed in the emitter.

### RELATIO (2026-07-02) — PHASE 3 COMPLETE

All four chunks green in one run: fork+annotations (52), skeleton
grammar (16), baking (18), generated construction (44). Suite 16/16,
756 assertions. The generator pipeline is real end-to-end:
sceletum.stml → validated annotated grammar → conflict-preserving LALR
(exactly 1 conflict = the designed typedef fork) → baked
Latin-commented C tables + registry + accessors + constructor →
committed generated files the suite compiles and verifies against
fresh builds. The "largest untested bet" (standing worry #3) is
retired: lapifex's generator was read end-to-end, forked mechanically,
and extended without surprises; both known reader gaps were found
BEFORE coding. Remaining top risk is now Phase 4 alone (GLR runtime,
spec §7) — and its dress rehearsal already passes: the fork cell is in
the baked tables and both readings of `foo * bar ;` construct cleanly.
Next: Phase 4 — GSS + reducenda drain + localized AMBIGUUS merge +
post-accept pater fixup, driven by the baked sceletum tables.


## Audit — complexity sweep before Phase 4 (2026-07-02)

Fran asked, with everything warm in context, whether earlier-surfaced
complexities had all been addressed. Full re-read of this log; findings:

**Closed by this audit (suite now 17/17, 767 assertions):**
- **Pasta order-sensitivity (Chunk C debt)** — the specific adversarial
  shape finally has its dedicated test: MT(a b, c d) → a bc d, asserting
  surrounding operand tokens wrap ex_expansione while the pasted token
  passes through with PASTA provenance (sinister "b", dexter "c").
- **Crash-freedom fuzzing (§8.2: "from the walking skeleton on")** — was
  parked at Phase 6 by the sketch, but the spec text says from the
  skeleton on, running with the suite. probatio_silva_incolumitas now
  runs 400 DETERMINISTIC specimens (xorshift, fixed seed — reproducible):
  raw bytes incl. NUL + C-shaped chars, each through lexare→emittere
  byte-fidelity AND the full preprocessor (directivas + positional
  expansion) asserting no crash and termination. 400/400 fidelis.
- **Log chronology** — mid-file insertions had left sections ordered
  0,1,3,2.5,2; reordered to 0,1,2,2.5,3 (content untouched). Rule going
  forward: new phase sections are APPENDED, never inserted.
- **v1 praeparator suite (124 assertions) — CLOSED BY COVERAGE.** The
  interview bar said "the 134-test suite plus v1's preprocessor tests."
  Assessed section by section: creare/definire/undefinire → covered by
  macro_addere + expansio suites; conditionales → regiones (more
  thorough: arm retention, guards); if_expressions → conditio (more
  thorough: s64 fix, full expansion); special_operators → pasta + compat;
  macro_expansion → generatio + compat; include/addere_via → praebere
  semantics (path management host-side by design). Disposition: no
  verbatim port; Phase 7 integration includes a spot-check pass over the
  v1 file for any input STRING worth harvesting that the mapping missed.

**Confirmed parked with landing spots (no action now):**
- Layer-N emission spacing → Phase 5 (scribere), logged in Phase 2
  addendum.
- EOF↔$ terminal mapping → Phase 4 adaptator (logged in Chunk C
  INTENTIO).
- Two-track deferrals (untaken-arm structural scan, config-aware journal
  replay, lazy false-arm includes, macro-expanded #include operands) →
  post-M1 query milestone, logged in Chunk D design notes (Phase 2).
- v1 syntaxis input harvest (234) → Phase 4; quaestio/formator/typus/
  index → post-1.0 milestones (spec §4.1).

**Orphans found — now given explicit landing spots:**
- **Prosser exactness** (HS_call vs HS_call∩HS_rparen) — had no named
  landing spot. NOW: revisit at Phase 7 integration when the external
  corpus (Lua/zlib) enters; if no real-world case surfaces, document as
  a permanent conservative simplification.
- **Caps + cancellation hook (§8.2)** — Phase 1 said "the Phase 2 caps"
  but Phase 2 only delivered include-depth (32) and generation caps.
  Token-volume caps (OCTETUS_IGNOTUS one-per-byte) + the cancellation
  hook had no owner. NOW: Phase 7 obligation (they gate the §11.2
  checklist's hostile-input posture).
- **The latina.h compiled-in definition set** — Phase 1 said "Phase 2's
  job"; Phase 2 built the MECHANISM (silva_macro_addere, and `#define
  si if` flows with provenance) but not the SET. NOW: SilvaContextus
  work at Phase 7 (the set is contextus configuration, per the
  interview's compiled-in-defaults answer).

Verdict: nothing rotting. Two commitments were drifting toward "later"
that the spec said were "now" (fuzzing, the pasta test) — both closed.
Three parked items had no owner — all three now do.


## Phase 4 — GLR runtime

### INTENTIO (2026-07-02)

**What**: fontes/silva_glr.{h,c} — the parse engine per spec-v2 §3.1:
lapifex_glr's skeleton (GSS + path enumeration + worklist) + arbor2's
control refinements (two-pass per token, reducenda drain, shift-frontier
merge) + tagged SilvaValor + ported merge with REAL structural equality
+ accept-reconciliation + oracle + post-accept passes. Proves: S21/S26/
S27 and the §9 contracts under fire — the fork cell baked in state 7
finally fires at runtime.

**Sources read end-to-end before this entry** (per protocol):
lapifex_glr.c (716 lines, whole), arbor2_glr.c merge machinery
(_nodi_aequales :1094, _mergere_compatibiles :1185, accept ladder
:7604-7665, reducenda drain :7514-7690), spec-v2 §3.1/§8.2/§9,
silva_tabulae.h contract + generated sceletum pair + silva_nodus.h.

**Design decisions carried in from the reads:**
1. **Runtime is table-agnostic**: silva_glr takes SilvaTabulaCocta* +
   a constructor callback matching silva_sceletum_construere's
   signature. One dispatch, called from both passes (§9.4).
2. **SilvaValor replaces s64** at the six mechanical sites (via.valori,
   node valor, path temp, callback signature, shift value, fructus);
   the ONE decision site is the merge, where value comparison becomes
   structural equality + resolution policy.
3. **Fusion policy — arbor2's, with the lapifex hazard named**: lapifex
   fuses during the reduce pass and dedups worklist (node, production)
   pairs; a fusion that adds a predecessor to an ALREADY-PROCESSED node
   is blocked by the dedup from re-enumerating the new path — a
   missed-re-reduction hazard (classic Tomita re-reduction problem).
   arbor2 never fuses in-reduce (fresh GSS node per reduction path, all
   drain through reducenda) and packs ONCE per token at the shift
   frontier (_mergere_compatibiles). Silva adopts arbor2's policy
   (§3.1 already chose it); cost = transient duplicate frontier nodes,
   bounded for the skeleton; Tomita reduction-time packing stays the
   documented escalation path.
4. **Structural equality upgrade**: arbor2's _nodi_aequales is shallow
   + one TERNARIUS special case. Silva's uniform node model makes REAL
   structural equality generic: recursive walk over genus + loci tagged
   values; TOKEN slots compare by POINTER identity (identical parses of
   the same stream share token pointers; value equality would wrongly
   merge distinct source positions). No per-genus cases.
5. **Accept ladder ported minus grammar hacks**: multiple accepts →
   structurally equal = pick first; different = resolutor then AMBIGUUS.
   arbor2's nested-TERNARIUS preference does NOT port (grammar-specific
   patch; the maximalist answer is AMBIGUUS retention). arbor2's
   IDENTIFICATOR-only filter (incomplete parse) generalizes if needed;
   the sceletum grammar should not produce incomplete accepts.
6. **Merge never decides semantics alone — resolutor callback**: at the
   decision site (merge + accept ladder), a caller-supplied resolutor
   (given both values + the oraculum) returns A/B/unknown. Known →
   prune to winner, NO AMBIGUUS node (C's typedef "ambiguity" is symbol
   table, not grammar — a known answer collapses the fork). Unknown →
   AMBIGUUS node (registry genus: interpretationes lista + canonica
   INDEX) + registration in the fructus's ambigui list with the
   discriminating identifier. Runtime stays grammar-agnostic; the
   sceletum resolutor is hand-written for M1 (generator emission of
   resolutor scaffolding is post-M1, named path back).
7. **Post-accept passes (S26/S27)**: constructions stay pure; after
   accept, per top-level slice: (a) pater fixup walk — canonical spine
   only; off-spine AMBIGUUS interpretations keep pater NIHIL; (b)
   oracle registration (M1: API-fed only — the sceletum grammar has no
   typedef keyword; grammar-driven registration lands with the C89
   grammar, named). Re-canonicalization API: oraculum injection → walk
   registered ambigui → flip canonica IN PLACE + re-run LOCAL pater
   fixup — no reparse (§8.2).
8. **EOF↔$ adaptator seam confirmed in the baked tables**: EOF is a
   real shifted terminal (augmented production transposes it); ACCIPERE
   fires on $ = -1. Adaptator maps the stream's EOF token → terminal
   index by genus; past-end → -1.
9. **Load-time validation**: GOTO completeness pass (missing GOTO =
   infinite loop, not clean failure — arbor2 worklog:284,1938), RHS
   length vs cap, RECUPERARE actions fail loud until implemented.
   Caps (max frons, pop depth) are struct fields, not #defines —
   conscious revisit of arbor2's MAX_GSS_PATHS=64/MAX_POP_DEPTH=16.

**Chunks:**
- **A — Tables + adaptator + GSS core (unambiguous path).** Lookup over
  baked slices, load validation, adaptator, GSS with SilvaValor,
  two-pass + reducenda drain, constructor callback, single accept,
  error report (positio/terminalis/status). Tests: `int x ;`,
  `x + 1 ;`, statement lists, clean parse errors.
- **B — Fork + structural equality + frontier merge + accept ladder.**
  silva_nodus_aequales, _mergere_compatibiles port, AMBIGUUS creation,
  fork/fusion/max-frons stats. Tests: `foo * bar ;` → exactly ONE
  AMBIGUUS, localized at sententia level (not root), both
  interpretations verified through generated accessors, deterministic
  canonica; unambiguous inputs → zero AMBIGUUS.
- **C — Oraculum + resolutor + post-accept.** SilvaOraculum (typedef
  names), resolutor at merge/accept, pater fixup, ambigui registry,
  re-canonicalization API. Tests: known typedef → no AMBIGUUS; unknown
  → AMBIGUUS → inject → canonica flips in place (same node pointers),
  pater NIHIL during construction / correct after fixup / NIHIL
  off-spine.
- **D — Driver + recovery + robustness + mensura.** silva_parsare
  (lexare→directivas→expandere_reliqua→adaptator→GLR→post-accept),
  boundary-resync recovery (discard to ;/} → ERROR node with skipped
  tokens, resume), incolumitas extended through the FULL pipeline to
  trees, mensura gains parse timing + max-frons, v1 syntaxis input
  harvest (audit landing spot): skeleton-expressible inputs become
  fixtures now, the rest recorded for the C89-grammar milestone.

**Explicitly NOT Phase 4** (named, with owners): emission (Phase 5);
CONDITIONALIS region nodes threading into parse trees (Phase 7 §11.2);
per-declaration slicing driver (enters with the C89 grammar — the core
API takes token slices so the path is open); cost-driven RECUPERARE
recovery (post-substrate, §8.2 — table format already carries pretium).

**Exit criteria**: suite green including new probationes (glr,
ambiguitas, oraculum, driver/recovery); the fork case end-to-end
(parse → localized AMBIGUUS → inject typedef → re-canonicalize in
place, verified by pointer identity); fuzz through the full pipeline
never crashes; parse timings visible in mensura.


## Correctio — Xar pointer stability (2026-07-02)

The Phase 2 Chunk D complexity entry "Xar growth invalidates held slot
pointers across recursion" is FALSE — Fran asked whether it came from
reading xar.c or from assumption; verification against xar.c shows it
was assumption (imported realloc-style dynamic-array intuition; the
entry's own "would corrupt silently" reveals it was never observed).

The truth: Xar is a SEGMENTED array — "Tabula crescens sine
reallocatio" is the first line of xar.c. Growth allocates a fresh
segment into a fixed pointer table inside the struct; existing elements
NEVER move; no realloc/memmove/free exists in the file. Element
pointers from xar_obtinere/xar_addere are stable across any number of
appends — that stability IS the design.

What actually disturbs held pointers: xar_removere_cum_ultimo
(swap-remove copies last element over the removed slot), xar_mutare /
xar_invertere / xar_ordinare (move bytes between slots), and
xar_vacare / xar_truncare (slot logically dead, reused by later
appends). Appends: safe.

Corrected in all three places the lore propagated: this log (this
entry), silva_expandere.c:866 comment (the write-before-recursion
order there is clarity, not necessity), memory file. Relevance to
Phase 4: GSS predecessor Xars are appended during reduction recursion
— pointer stability across appends can be relied on; no index-based
defensive contortions needed.

Meta-lesson (same class as the phase-boundary audit): claims about a
library's semantics recorded in a complexity entry must come from its
SOURCE, not from what such a library usually does. When a recorded
rule constrains how code gets written, verify it once against the
implementation before it hardens into convention.

### Chunk A — COMPLETE (2026-07-02)

probatio_silva_glr 71/71 first run (suite 18/18). The baked sceletum
tables drive real parses: fontes/silva_glr.{h,c} — table-agnostic
engine (SilvaTabulaCocta* + constructor callback), GSS with SilvaValor,
FIFO reducenda drain, per-state-slice action/goto lookup, load-time
validation (bounds, slice sortedness, goto-for-every-reducible-LHS —
the arbor2 "missing GOTO = infinite loop" class caught at creare),
adaptator (genus→terminal; past-end → $ = -1; unknown genus → -2 clean
error). Engine is reentrant: frontier lives in parsare locals; the
struct holds only tabula/constructor/piscina/stats. Two-piscina API
shape baked in now (GSS in engine piscina, trees in caller-passed
piscina_arborum) per §3.1 — same piscina twice works fine. The accept
node's shifted EOF token is preserved in fructus.lexema_finis (its
trivia is file-tail bytes Phase 5 must emit). Tests: empty stream
(epsilon list), declaratio, monstrator, precedence structure via
tables (x + 1 * 2), parentheses, multi-statement lists, three clean
errors (positio+terminale), post-error reentrancy, S27 pater NIHIL.

**Complexity — list-append purity vs forks (RESHAPES CHUNK B):**
- discovered-while: tracing `foo * bar ;` through the baked tables to
  design the drain order.
- consists-in: the generated lista-appendere (P1) MUTATES its incoming
  lista (xar_addere into the shared Xar) and returns it. A fork that
  crosses a list-append shares the pre-fork lista via the common GSS
  predecessor: BOTH arms would append (flat double-append; the two
  state-1 results are then pointer-equal and would falsely fuse). S26
  purity is violated in spirit by the generated append. arbor2 never
  hit this: its GLR grammar had NO in-grammar list — the per-declaration
  slicing DRIVER built lists outside the parser, so ambiguity
  reconverged only at the accept ladder. The sceletum deliberately put
  the list in-grammar; §3.1's "shift-frontier packing only" is therefore
  INSUFFICIENT FOR CORRECTNESS here, not merely a memory trade-off: the
  fork must collapse into ONE AMBIGUUS value BEFORE the list-append
  reduction consumes it.
- consequences: Chunk B needs reduction-time packing of reduce-created
  nodes (same state, same wave), in addition to the shift-frontier
  merge. The FIFO drain (Chunk A, already in) makes waves
  breadth-first: for the sceletum fork both sententia-level nodes
  (state 2) are created in the same wave BEFORE either drains — arm A
  reduces P4→P2, arm B P9→P3, equal depth 2 (verified by table trace) —
  so same-wave packing suffices. LIMITATION: unequal-depth
  reconvergence would find the earlier node already drained, its value
  already consumed downstream BY VALUE COPY (SilvaValor is copied into
  loci/listae; mutation-in-place does not propagate — unlike SPPF
  pointer-packing, where late alternatives ride the shared pointer).
- handled-by: Chunk A ships the wave-aligned FIFO drain. Chunk B
  implements same-wave packing + a FAIL-LOUD guard for the
  already-drained case (diagnostic naming the state), with the upgrade
  options recorded: SPPF-style in-place node metamorphosis (clone old
  content as interpretation[0], rewrite the allocation as AMBIGUUS —
  pointer identity propagates) or Farshi re-reduction. Decide if/when
  the C89 grammar produces a real unequal-depth case (typedef
  ambiguity is statement-local; none expected in M1).

### Simulation ⑤ + interview — plan revision between Chunks A and B (2026-07-02)

Fran asked whether the metamorphosis/Farshi escalation could be
pretend-implemented to see if its downsides dissolve. Simulation ⑤
(project-specs/silva-simulatio-5.md) + 8-question interview
(silva-simulatio-5-interview.md) → resolutions in spec-v2 §12. Net:
**metamorphosis (transmutatio) moves from evidence-gated escalation into
Chunk B proper.** The three downsides dissolved under concrete design:
S32 breach → build-through-checked-constructors then ONE struct copy
(no check bypassed); retroactivity → ordering rule (structural equality
only at pack-time-per-key + post-drain frontier merge; transmutation
window = one drain); testing → one committed variant grammar with an
extra unit production (unequal-depth arms).

Revised Phase 4 sequence (supersedes the INTENTIO chunk list):
- **A½ — length-view lists** (correctness floor): SilvaValor LISTA arm
  becomes {Xar*, mensura}; copy-on-divergence append helper; commit
  normalization owed to the Chunk C walk. Chorda-pattern for values.
- **B — merge machinery**: structural equality + frontier merge +
  same-wave packing + transmutatio + index reducendorum keyed
  (status, basis) + fabrica ambigui callbacks + counters AS FRUCTUS
  FIELDS (fusiones/transmutationes/eventa_marginis_novi) +
  accessing-symbol-uniqueness validation + variant grammar probatio.
- **C — commit walk (SIMPLIFIED)**: pater fixup + list normalization +
  oracle resolution with COLLAPSE+JOURNAL (clean winner in tree,
  resolution event recorded in fructus) + re-canonicalization API.
  Resolutor-at-merge deleted from the plan; drains are semantics-free.
- **D — unchanged** (driver, recovery, incolumitas through pipeline,
  mensura, v1 syntaxis harvest).

New contracts (spec-v2 §12.2, one-liner in silva/CLAUDE.md VISIO):
node contents always via ponere; identity re-bindable until commit;
structural genera generator-REQUIRED. Evidence-gated (named, unbuilt):
action-filter pruning hook (known-typedef 2× fork cost, mensura-gated);
identity-handle refactor (if eventa_marginis_novi fire on real code).

### Chunk A½ — COMPLETE (2026-07-02): length-view lists

Suite 18/18 (probatio_silva_constructio 58/58 with the new
prospectus-listarum section, 14 assertions). The list value contract is
now speculation-safe per spec-v2 §12.1:

- **SilvaListaProspectus** {Xar* xar, i32 mensura} replaces the bare
  Xar* in the SilvaValor LISTA arm — the chorda pattern for value
  arrays. Nobody reads xar_numerus on a list value anymore; the view's
  mensura is the truth (silva_valor_lista_numerus / _obtinere are the
  only sanctioned readers, obtinere bounds-checks against the VIEW).
- **silva_valor_lista_appendere** — the pure append, ONE
  implementation: view at the repositorium's live end → in-place O(1)
  (common case); repositorium already written past my view (another
  fork appended) → copy my prefix to a fresh Xar (copy-on-divergence;
  the Xar's own count is a free version counter). Returns a NEW view;
  foreign views are never disturbed. silva_nodus_appendere rebinds the
  slot through it (nodes under construction are single-owner).
- **Emitter (silva_coquere)**: both list emission shapes
  (lista-initium, lista-appendere) now emit calls to the pure helper —
  generated construere no longer touches Xar internals at all.
  Regenerated committed tables; drift guard green.
- **Tests**: fast-path pointer identity (same xar, mensura grows, base
  view unmoved), genuine divergence (two forks appending from one base
  view → distinct repositoria, shared prefix by pointer, distinct
  tails), view bounds (NIHIL beyond mensura even when the repositorium
  is longer), non-lista inputs → nihil/zephyrum/NIHIL.

Owed onward (named): commit normalization (exact-length copy iff view
!= repositorium count) rides the Chunk C commit walk; probationes and
engine code MUST use the view readers (recorded in silva_nodus.h — the
"numquam xar_numerus in repositorio" rule).

The double-append corruption from the Chunk A complexity is now
unrepresentable: each fork's P1 gets its own view; shared prefixes are
structural, tails private, by construction.

### Chunk B — INTENTIO (2026-07-02)

Merge machinery per spec-v2 §12: the fork finally packs. Pieces, in
build order:
1. **Generator**: structural genera (ambiguus/error/conditionalis)
   REQUIRED in genera-extra (generation error if absent); coquere emits
   `silva_PREFIX_ambiguum_fabricare(piscina, interpretationes,
   canonica)` — the ONE fabrica callback (engine-side bookkeeping
   replaces ambiguo_addere: the engine tracks its own packed entries'
   interpretationes views, rebuilds via fabrica, struct-copies).
2. **Variant grammar** grammatica/sceletum_imparilis.stml (committed,
   drift-guarded): sceletum + `mandatum` unit rule on the expression
   arm → reconvergence depths 2 vs 3 → guaranteed same-wave miss →
   transmutatio exercised.
3. **Engine**: _valores_aequales (generic recursion: NODUS genus+loci,
   TOKEN pointer identity, LISTA mensura+elements via views, INDEX);
   index reducendorum keyed (status, basis) with exhaustus/est_ambiguum
   flags + engine-held interpretationes view; reduce dispatch: miss →
   create+register; hit undrained → equal ? fusio : valor-rebind wrap
   (no identity trick needed pre-drain); hit drained → equal ? fusio :
   TRANSMUTATIO (clone shares old loci array — original's loci pointer
   is replaced, so plain struct copy suffices; fabrica node struct-
   copied onto the stable allocation); drained non-NODUS target →
   cannot transmute → separate arm + counter (graceful). Frontier
   merge: state-keyed predecessor union (silva shift values are the
   SAME token pointer, so frontier value-wrap never occurs — unlike
   arbor2, whose shift nodes carried leaf AST values). Accept ladder:
   dedup equal predecessor values, wrap genuinely different roots via
   fabrica (defensively implemented; both grammars resolve pre-EOF, so
   multi-accept stays untestable here — noted). Accessing-symbol
   uniqueness in validare (needs scratch → validare gains Piscina*
   param; API is one day old, cheap to amend).
4. **Counters as product** (fructus fields): fusiones, transmutationes,
   eventa_marginis_novi, frons_maxima.
5. **probatio_silva_ambiguitas.c**: sceletum `foo * bar;` → ONE
   localized AMBIGUUS (lista NOT ambiguous), 2 interpretationes both
   accessor-verified, canonica 0, fusiones>0, transmutationes==0;
   imparilis same input → SAME tree shape but transmutationes==1 (the
   stored element pointer now shows genus ambiguus = the identity
   proof); multi-statement localization; unambiguous inputs → zero
   counters, zero AMBIGUUS.

Known-untested paths (named): three-arm ambiguo-append (no 3-way
ambiguity in either grammar — first C89 grammar case owns it);
multi-accept ladder (unreachable in both grammars).

### Chunk B — COMPLETE (2026-07-02)

probatio_silva_ambiguitas 60/60 first run; suite 19/19, 912
assertions. **The state-7 fork cell fires and packs**: `foo * bar;`
parses to ONE localized AMBIGUUS statement (root list ordinary, both
interpretations accessor-verified, canonica 0) — and on the imparilis
grammar the same input exercises TRANSMUTATIO (transmutationes == 1;
the list element pointer stored by the append now SHOWS genus
ambiguus — the identity proof is the assertion itself).

What landed:
- **Generator**: structural genera REQUIRED (generation error +
  ambiguus shape check: interpretationes:lista-* + canonica:index);
  coquere emits silva_PREFIX_ambiguum_fabricare reading the layout
  from the registry (loci by NAME, not fixed position).
- **grammatica/sceletum_imparilis.stml** committed + generated tables
  (20 symbols, 19 productions, 28 states, SAME 1 preserved conflict);
  generare.sh with no args now regenerates ALL committed grammars.
- **Engine** (silva_glr.c): _valores_aequales / _nodi_aequales
  (generic recursion; TOKEN by pointer identity; LISTA via views with
  same-repositorium fast path); index reducendorum (per-drain, keyed
  status+basis, entries by value in Xar — stable pointers); reduce
  dispatch: miss → create+register (same-status-other-basis counted
  eventa_marginis_novi); hit+equal → fusio (duplicate derivation
  discarded); hit+undrained → _compingere (GSS valor rebind, no
  identity needed); hit+drained+NODUS → _transmutare
  (clone-shares-old-loci + fabrica through checked constructors + ONE
  struct copy onto the stable allocation); hit+drained+non-NODUS →
  separate arm + transmutationes_negatae (graceful, shadowed entry).
  Frontier merge = predecessor union only (silva shift values are the
  same token pointer — validated accessing-symbol uniqueness makes
  value comparison unnecessary there). Accept ladder: collect all
  predecessor roots, dedup structurally, wrap genuine divergence via
  fabrica (defensive — unreachable in both grammars, noted).
- **Validation**: accessing-symbol uniqueness assertion (per state,
  incoming shift+goto symbols must agree — simulatio ⑤ C3, theory
  property checked from the baked rows); validare gains Piscina*.
- **Counters as product**: fusiones, transmutationes,
  transmutationes_negatae, eventa_marginis_novi, frons_maxima in
  SilvaGLRFructus; zero on unambiguous inputs (asserted).

Complexities: NONE new — the simulation ⑤ design survived contact
intact (first-compile clean under full -Werror except signature-
migration errors the diagnostics enumerated). The ordering rule is
embodied structurally: structural equality runs only at pack-per-key
and accept dedup; frontier merge never reads values; the
transmutation window is the per-drain index lifetime.

Untested paths carried (named, from INTENTIO): three-arm
ambiguo-append (first 3-way-ambiguous grammar owns it); multi-accept
root AMBIGUUS wrap (unreachable in both grammars); C89-grammar
transmutation shapes (the variant grammar is the regression net until
then).

### Chunk C — INTENTIO (2026-07-02)

The commit walk (spec-v2 §12, simplified by interview ⑤): ALL
semantics consolidate here; drains stay semantics-free. New module
fontes/silva_commissio.{h,c} (oraculum inside — split later if it
grows; interface survives a split).

- **SilvaOraculum**: typedef-name table (tabula_dispersa_chorda);
  typum_addere duplicates the key into its piscina (token valor is a
  VIEW into source — the fons->via lifetime lesson applied). Binary
  for M1 (novit/ignotum); three-valued oracle (known-NOT-a-type) is
  named for the C89 grammar.
- **SilvaResolutor** (callback, grammar-specific, hand-written for M1
  per interview): given an AMBIGUUS node + oraculum, fills
  {victor index | -1, discriminating token}. Engine/walk stay
  grammar-agnostic; the walk finds the ambiguus FORM (genus index +
  interpretationes/canonica loci) from the baked registrum BY NAME.
- **silva_committere(piscina, radix, registrum, oraculum, resolutor,
  contextus) → SilvaCommissio{radix, ambigui, resolutiones}**, one
  recursive pass:
  (a) pater fixup — child->pater set on descent; at surviving
  AMBIGUUS: internals of ALL interpretations threaded (queries descend
  alternatives), interpretation ROOTS marked by spine: canonical
  pater=ambiguum, others NIHIL; canonical walked LAST so on any shared
  subtree the spine parent wins (named limitation: pater on genuinely
  shared subtrees is single-owner by walk order — C89-grammar
  revisit).
  (b) A½ list normalization — view != repositorium count → exact-
  length copy, slot rebound (the commit walk is the sanctioned
  mutation point).
  (c) oracle resolution COLLAPSE+JOURNAL — resolutor knows → winner
  struct-copied ONTO the ambiguus allocation (identity discipline —
  observers' pointers stay valid), resolution event journaled {sedes,
  victor, genus, discriminans token (its origo = queryable position)},
  then walked as a normal node. Winner non-NODUS → fail loud, keep
  wrapper (root-lista interpretations unreachable in M1, named).
- **silva_recanonicare(commissio, oraculum, resolutor, contextus)**:
  post-commit learning does NOT collapse (trees immutable after commit
  except this sanctioned op — CLAUDE.md pin): canonica INDEX flipped
  in place + LOCAL pater re-thread (old spine root → NIHIL, new →
  ambiguum), event journaled. Returns count flipped.
- **probatio_silva_commissio.c**: pater chains (incl. both
  interpretations' internals + spine roots); hand-built diverged view
  normalized; collapse pointer-identity (the pre-commit ambiguus
  allocation shows DECLARATIO genus post-commit); journal content
  (discriminans "foo"); survive-then-recanonicare (flip 0→1 via test
  resolutor, pater re-threaded both directions); imparilis
  transmutation + collapse composition; no-oracle commit keeps
  AMBIGUUS with canonica 0.

Exit: suite green; the full §12 story runs end-to-end: parse → fork →
pack/transmute → commit → collapse-or-survive → inject → recanonicare
IN PLACE.

### Chunk C — COMPLETE (2026-07-02)

probatio_silva_commissio 60/60 (suite 20/20, 972 assertions; one
signedness fix, otherwise first-run). **The full §12 story runs
end-to-end**: parse → fork → pack/transmute → committere →
collapse-or-survive → inject → recanonicare IN PLACE.

What landed — fontes/silva_commissio.{h,c}:
- **SilvaOraculum** (typedef table; keys chorda_transcribere-copied
  into its piscina — the fons->via lifetime lesson applied
  proactively). Binary novit/ignotum; three-valued oracle named for
  the C89 grammar.
- **silva_committere** — ONE recursive walk, grammar-agnostic (ambigui
  form read from the baked registrum BY NAME): pater fixup (S27
  discharged), A½ list normalization (views → exact length; the
  hand-built diverged-view case verified), and COLLAPSE+JOURNAL:
  resolutor-known AMBIGUUS nodes get the winner struct-copied ONTO the
  wrapper allocation (identity: the pre-commit pointer shows
  DECLARATIO genus post-commit — asserted), resolution event journaled
  with the discriminating TOKEN (its origo = the queryable position).
- **Spine semantics**: internals of ALL interpretations threaded
  (queries descend alternatives); interpretation ROOTS carry spine
  membership (canonical pater=ambiguum, others NIHIL); canonical
  walked LAST (shared-subtree insurance, named limitation).
- **silva_recanonicare** — the sanctioned post-commit op: canonica
  INDEX versa in place, spine re-threaded BOTH directions (asserted),
  RECANONICATA event journaled, count returned; no-op when victor ==
  canonica (real resolutor after injection: 0 flips because the
  declaratio arm was already canonical — asserted); idempotent.
- **Resolutors hand-written in the probatio per interview** (sceletum
  + imparilis + a fictitious expression-preferring one to force a real
  flip); the walk never learns grammar semantics.
- **Composition proof**: imparilis foo*bar — the SAME allocation is
  rebound TWICE (transmutatio in the drain, then collapse at commit)
  and pointer identity holds through the whole story.

Complexity (small): **`registrum` is a latina macro (register!)** —
same trap class as `nomen`. Parameter renamed `tabularium`; recorded
here as the second entry in the forbidden-identifier list.

Phase 4 remaining: Chunk D (driver silva_parsare, boundary-resync
recovery → ERROR nodes, incolumitas through the full pipeline, mensura
parse timings, v1 syntaxis input harvest).

### Chunk D — INTENTIO (2026-07-02)

The driver + the robustness net; Phase 4 exit.

- **fontes/silva_parsare.{h,c}**: SilvaGrammatica bundle {tabula,
  tabularium, constructor, fabrica} (generated-surface handle; future
  emission of SILVA_PREFIX_GRAMMATICA named); silva_parsare(piscina,
  titulus, fons, mensura, grammatica, oraculum, resolutor, contextus)
  = the WHOLE pipeline (expansio creare → fons_addere → lexare →
  directivas → expandere_reliqua → slicing GLR → committere);
  silva_lexemata_parsare = the token-stream entry (forma silva_lexare,
  EOF ultimo).
- **THE DRIVER IS THE SLICING LOOP** (spec-v2 §3.1's plan of record —
  arriving EARLY because boundary-resync recovery wants it; supersedes
  the Phase-4-INTENTIO note that deferred slicing to the C89 grammar).
  Boundary finder for the skeleton: SEMICOLON at paren-depth 0
  (braces/function-definition cases belong to the C89 finder, named).
  Each segment + the stream's EOF token parses independently: success
  → its statements join the assembled root; failure → ERROR node
  (genus from the registrum BY NAME) holding the segment's tokens —
  per-item recovery for free, GSS bounded per statement. EVERY input
  yields a complete committed tree (§8.2's bar).
- **SilvaParsura** result: commissio (radix/ambigui/resolutiones),
  lexemata (expanded stream), expansio (journal/strata queryable),
  numerus_errorum/segmentorum, aggregated counters (sums; frons_maxima
  = max).
- **Incolumitas**: third stage — the FULL pipeline (silva_parsare) on
  every fuzz specimen; assert tree-always (successus + commissio),
  never crash. Unknown-genus tokens now degrade to ERROR nodes instead
  of parse errors.
- **Mensura**: the 78-file real-C89 corpus through the driver —
  timing (parse ms/KB) + error-node counts + max frons. The sceletum
  can't PARSE real C; the corpus becomes a RECOVERY soak (assert
  completion, count honestly).
- **v1 syntaxis harvest DONE at extraction level**:
  probationes/fixa/syntaxis_v1_corpus.txt (125 unique inputs,
  C-literal escapes verbatim, provenance header). Consumer = the C89
  grammar milestone; the M1 probatio runs the corpus through RECOVERY
  (robustness bar, not correctness bar).

Exit criteria: suite green; driver probatio (pipeline incl. a macro
through the preprocessor, mid-stream recovery, all-garbage input,
empty input, oracle collapse through the driver, corpus recovery
soak); incolumitas 400/400 through the full pipeline; mensura reports
parse timings. Phase 4 RELATIO follows.

### Chunk D — COMPLETE (2026-07-02); PHASE 4 COMPLETE

probatio_silva_parsare 43/43 first run; suite 21/21, 1015 assertions.

What landed:
- **fontes/silva_parsare.{h,c}** — SilvaGrammatica bundle +
  silva_parsare (fistula tota: expansio → lexemata → circuitus secans
  → GLR → commissio) + silva_lexemata_parsare (ingressus lexematum).
  THE DRIVER IS THE SLICING LOOP (§3.1's plan of record, arrived early
  because recovery wanted it — supersedes the Phase-4 INTENTIO
  deferral, as flagged in the Chunk D INTENTIO). Boundary finder:
  SEMICOLON at paren-depth 0 (C89 finder named). Failed segments
  become ERROR nodes (genus + loci from the registrum BY NAME, locus
  count included) holding the skipped tokens — nothing silently
  dropped. EVERY input yields a complete committed tree.
- **Preprocessor proven in the fistula**: #define T int / T x; parses
  to typus-PRIMITIVUS "int" — the macro ran through point-in-time
  expansion into the tree.
- **Incolumitas through the FULL pipeline**: 400/400 deterministic
  specimens (raw bytes + C-shaped) → lex-fidelity AND driver-to-tree,
  never crash. Unknown-genus tokens now degrade to ERROR nodes.
- **Mensura parse stage**: fistula integra 0.299 ms/KB over the
  78-file corpus; 797 ERROR nodes (the corpus is real C89 — the
  sceletum survives it by RECOVERY, honestly counted); frons max 2.
- **v1 syntaxis harvest**: probationes/fixa/syntaxis_v1_corpus.txt —
  125 unique inputs extracted verbatim (C escapes preserved,
  provenance header) so the corpus outlives v1's freeze-then-delete.
  Consumer = the C89 grammar milestone. The M1 probatio runs all 125
  through recovery: 125/125 trees, 125 with ERROR nodes (honest).

### RELATIO (2026-07-02) — PHASE 4 COMPLETE

All chunks green: A (engine, 71), A½ (length-view lists), B (packing +
transmutatio, 60), C (commissio, 60), D (driver + recovery, 43).
Suite 21/21, 1015 assertions. S21/S26/S27 and the §9/§12 contracts
now hold UNDER FIRE: the baked fork cell fires, packs at the smallest
node, transmutes when arms arrive late (identity held through
transmutatio→collapse on one allocation), commits with pater/
normalization/collapse+journal, re-canonicalizes in place, and the
driver turns ANY byte sequence into a complete committed tree with
per-statement recovery.

Mid-phase the process itself leveled up: simulation ⑤ (first
mid-phase simulation, run against SHIPPED code) reversed the
escalation plan with evidence — metamorphosis went from "distant,
gated" to "Chunk B, now" — and its design survived implementation
with ZERO new complexities. The Xar pointer-stability lore was found
FALSE by Fran's question and corrected everywhere (Correctio entry).
latina-macro identifier traps now number two (nomen, registrum).

Carried to Phase 5 (scribere): emission from committed trees (layout
tables + trivia single-owner + EOF lexema_finis trivia); layer-N
emission spacing decision (parked since Phase 2). Carried to Phase 7:
caps + cancellation, Prosser exactness, latina definition SET
(SilvaContextus), CONDITIONALIS threading into trees, v1 praeparator
spot-check. Named untested paths: three-arm ambiguo-append,
multi-accept root wrap (first 3-way/multi-accept grammar owns them).
Next: Phase 5 — Scribere (byte-exact roundtrip emitter; cursor.c
byte-identical).

### Phase 5 pre-INTENTIO notes (2026-07-02, protocol reads done early)

The Phase 5 protocol reads were done ahead of the INTENTIO (full
phase-log audit clean; spec §3.4/§6/§8.3; the v1 trivia graveyard in
arbor_syntaxis.worklog.md end-to-end; arbor2_scribere.c end-to-end).
Findings and Fran-stated requirements to fold into the INTENTIO:

1. **The expansion boundary is Phase 5's genuinely new design work.**
   arbor2/v1 only ever roundtripped by NOT expanding (PRESERVARE/HYBRID
   + synthetic-trivia hand-patches — the ugliest graveyard section).
   Silva parses the EXPANDED stream; cursor.c (the M1 bar) is saturated
   with latina macros. Emission must: (a) at EXPANSIO-origo tokens,
   emit the layer-0 INVOCATION (origo invocatio = FONS token with real
   trivia) exactly once per invocation instance; (b) re-insert
   directive LINES from the journal/regions (they never enter the
   tree; journal positus records where they stood). Object-like macros
   (all of latina) are the required case; function-like invocation
   extents (parens/commas absent from the expanded stream) are likely
   a named deferral. Candidate for a mini-simulation ⑥ before coding.
2. **Malformed content roundtrips (Fran, explicit).** ERROR nodes hold
   skipped tokens, so this is reachable — and the incolumitas fuzz
   assertion upgrades to the full-pipeline property: any bytes →
   parse → emit == original bytes. That oracle will stress directive
   re-insertion on garbage (half-open regions, stray #) relentlessly.
3. **Transformation fidelity (Fran, explicit): untouched subtrees
   byte-for-byte; touched seams minimally disrupted.** Two rules:
   (a) emission is a PURE STRUCTURAL WALK, never position-dependent —
   no byte_offset ordering, no neighbor reads (arbor2 compared
   linea/columna at emission; a moved subtree would emit wrongly);
   moved/cloned subtrees then emit their bytes wherever they land, and
   single-owner trivia localizes any disruption to the replaced node.
   (b) The missing-trivia policy is ONE decision, not three: inserted
   nodes, expanded tokens (the parked layer-N spacing item), and
   formatter output are the same problem — per §8.3, comments are
   content (always preserved), whitespace is style (default profile
   where absent, stored trivia always wins where present).
4. **Generic emission needs a slot-order guarantee.** The emitter is
   one generic loci walk (killing arbor2's 1,100-line hand switch),
   but slot layouts unify ACROSS productions of a genus — the
   generator must validate that all productions of a genus map slots
   in consistent RHS order (or nodes carry production id, which "code
   is a database" would welcome anyway).
5. **Mined from arbor2_scribere.c**: AMBIGUUS must emit the CANONICA
   (arbor2 emitted interpretation[0]); ERROR emission must be total
   (arbor2 SKIPPED error nodes — silent byte loss); unknown genus
   fails loud (§3.4); EOF lexema_finis tail trivia is owed by the
   emitter (Chunk A note).


## Phase 5 — Scribere

### INTENTIO (2026-07-02)

**What**: fontes/silva_scribere.{h,c} — the byte-exact roundtrip
emitter, closing the token→tree→bytes chain. The M1 §6 bar: cursor.c
byte-identical through the FULL pipeline (latina macros expanding and
all). Plus the supporting work: generator slot-order validation, the
driver capturing directive lines, and the property-net upgrades
(incolumitas/fidelitas/mensura).

**Protocol reads done before this entry** (pre-INTENTIO notes above,
plus): silva_token.h, silva_expandere.h, silva_nodus.h,
silva_tabulae.h registrum types, the driver's preprocessor call site.
Two discoveries from those reads reshaped the plan:
- **directivas_processare ALREADY retains directive lines** via its
  directivae_out parameter (Xar of Xar de SilvaToken*, one logical
  line per entry) — the driver just passes NIHIL today
  (silva_parsare.c:323). Directive re-insertion has its data source;
  no expander surgery needed.
- **PASTA origo carries NO invocatio** (sinister/dexter operands
  only), so a ## token's radix can land in the DEFINING file, not the
  use site. Pasta joins the named fail-loud deferrals below.

**Design decisions (carried in from the reads):**

1. **Two emission entries, two contracts.**
   - `silva_scribere_nodum` — subtree emission, PURE STRUCTURAL WALK:
     loci in layout order; TOKEN → per-token emit (reuse the
     scissurae-aware primitive from silva_lexema — the fidelity
     property already proves it); NODUS → recurse; LISTA → view
     elements in order; INDEX/NIHIL → skip. NEVER position-dependent
     (no byte_offset ordering, no neighbor reads — arbor2's
     linea/columna comparison is the named anti-pattern). This is
     what makes untouched subtrees byte-exact under transformation.
   - `silva_scribere_fontem` — file-level layer-0 reconstruction of
     ONE fons: tree + directivae + EOF lexema_finis tail trivia.
     Position-MERGE of directive lines is legitimate HERE ONLY
     (directives are layer-0 stream artifacts, not tree residents
     until CONDITIONALIS threading, Phase 7 — named).
2. **Genus-agnostic walk; three structural exceptions read from the
   registrum BY NAME** (the commissio pattern): AMBIGUUS → emit the
   CANONICA interpretation only; ERROR → emit the held token list
   (totality — recovery bytes survive); unknown genus or unexpected
   valor tag → FAIL LOUD (successus FALSUM + sedes), never skip.
3. **Expansion boundary rule (the genuinely new work).** A tree token
   with origo != FONS is not emitted as itself; the layer-0
   INVOCATION is emitted instead, ONCE per invocation instance
   (dedup: consecutive corpus tokens of one instance share the same
   invocatio pointer). Object-like macros — all of latina, all
   cursor.c needs — are the required case. NAMED fail-loud
   deferrals: function-like invocation extents (the invocation's
   parens/args are absent from the expanded stream; detectable via
   SilvaMacroDef est_functio), PASTA (no invocatio — see above),
   CHORDA/stringificatio. Candidate mechanism if the corpus forces
   function-like: the expander records invocation-extent (invocatio
   token → last consumed layer-0 token) in a side table on
   SilvaExpansio at expansion time — cheap, non-invasive to the
   immutable origo. Simulation decides.
4. **Include boundaries**: tokens whose radix fons_index != the
   target fons are skipped (their bytes belong to the included
   file's own reconstruction); the #include directive line is
   emitted in their place. Guard-skipped re-includes
   (est_praetermissa) emit their line only. Unknown includes
   (learning mode) — simulation traces whether the line lands in
   directivae_out.
5. **Simulation ⑥ opens Chunk B** (CULTURA: mid-phase, against
   shipped code). Paper-trace cursor.c + a nested-include fixture +
   fuzz shapes (est_imperfecta regions, stray CANCELLUM) through the
   real structs. Must trace: multi-token-body invocation dedup,
   include skip + line re-insertion, unknown-include path,
   guard-file re-include, EOF tail trivia placement, whether
   function-like extents are cheap enough to just build.
6. **Slot-order validation (generator).** Generic emission is
   correct only if every production of a genus maps slots in layout
   order — add generation-time validation: mapped locus indices in
   RHS order must be strictly increasing, per production; violation
   = generation error naming the production id. Path back if a real
   grammar needs divergent orders: production id on nodes ("code is
   a database" welcomes it anyway) — named, unbuilt.
7. **Missing-trivia policy — ONE decision** (closes the parked
   Phase 2 layer-N spacing item). Stored trivia always wins. In the
   roundtrip emitter, absent trivia emits NOTHING — the boundary
   rule means roundtrip never legitimately meets a zero-trivia
   token, so the fidelity oracle exposes any hole loudly. The
   style-default-whitespace hook (for transformation-inserted nodes
   and formatter output, §8.3) lands with the formatter milestone;
   layer-N VIEW emission (render an expanded layer as text) is a
   query-milestone feature riding the same hook. Owners named.
8. **Output**: chorda_aedificator (not arbor2's byte-at-a-time Xar).
   Fructus SilvaScriptura {successus, chorda, sedes/diagnostic on
   failure}.

**Chunks:**
- **A — generic tree emitter + slot-order validation.** nodum +
  fontem for directive-free/macro-free inputs; generator validation
  + regenerate both grammars; ERROR/AMBIGUUS/EOF handling; probatio:
  graveyard micro-shapes (empty list, multi-line, irregular
  whitespace, tabs), ERROR segments, `foo * bar;` emits byte-exact
  BOTH pre- and post-collapse (collapse must not change bytes),
  imparilis transmutation case; the 125-input syntaxis corpus
  through parse→emit==input (ERROR-heavy — proves totality).
- **B — layer-0 reconstruction.** Simulation ⑥ first (file in
  project-specs/ if it earns one; complexities in the schema). Then:
  driver captures directivae (SilvaParsura grows the field);
  boundary rule + dedup; include skip/insert; fail-loud deferral
  detection; **cursor.c byte-identical with latina.h praebere'd**
  (the M1 bar); latina.h reconstructs as its own fons; the 78-file
  corpus through parse→emit==input (no praebere: mostly-FONS +
  directive re-insertion + self-defined object macros; if any file
  self-defines-and-uses a function-like macro, implement extents
  rather than shrink the bar — maximalist default).
- **C — the property net + mensura.** Incolumitas stage 4: all 400
  fuzz specimens through parse→emit==input — the malformed-content
  roundtrip requirement becomes the system's strongest standing
  oracle; fidelitas probatio upgraded from lex→emit to
  parse→emit over the corpus; mensura gains emission timing.

**Exit criteria**: suite green; cursor.c byte-identical via the full
pipeline; corpus 78/78 and syntaxis 125/125 tree-fidelity; fuzz
400/400 tree-fidelity; slot-order validation active; deferrals and
policy owners logged; RELATIO written.

**Explicitly NOT Phase 5** (named, with owners): STML serialization
(silva_stml — own module, 1.0 scope); formatter + style profiles
(§8.3, post-substrate); layer-N view emission (query milestone);
CONDITIONALIS-in-tree emission (Phase 7 threading); function-like/
pasta/stringificatio boundaries IF the corpus doesn't force them
(first grammar/corpus that does owns them — fail-loud until then).

### Chunk A — COMPLETE (2026-07-02)

probatio_silva_scribere 64/64 (63 on first run — one TEST slip, not
a code bug: recanonicare requires a non-NIHIL oraculum, matching the
commissio probatio's usage). Suite 22/22, 1,084 assertions.
**The headline: the full syntaxis v1 corpus (125 inputs) roundtrips
through parse→emit BYTE-EXACT on the first run** — and since the
sceletum can't parse most real C89, that fidelity flows through
ERROR nodes: recovery totality (nothing dropped, ever) is now a
proven property, not a design intention. Fran's malformed-content
requirement has its first standing evidence.

What landed:
- **silva_lexema_emittere_in** — the per-token emission primitive
  (trivia ante + valor with scissurae reinserted + trivia post)
  extracted from the stream emitter; ONE fidelity implementation
  shared by lexer oracle and tree emitter.
- **fontes/silva_scribere.{h,c}** — SilvaScriptura fructus
  {successus, textus, causa, sedes}; the generic walk (loci in
  layout order, tagged dispatch; LISTA via views); AMBIGUUS emits
  the CANONICA interpretation only (form read from the registrum BY
  NAME, commissio pattern); ERROR needs NO special case (its single
  lista-token locus emits via the generic walk — totality gratis);
  unknown genus / corrupt ambiguus form / out-of-range canonica =
  fractura CLARA. Chunk A conservatism: origo != FONS or foreign
  fons_index = fractura naming Chunk B ("limes expansionis") — the
  probatio pins `#define T int\nT x;` as a LOUD failure today, to be
  flipped into a fidelity assertion when Chunk B lands.
- **Three entries**: scribere_valorem / scribere_nodum (pure
  structural walk — the transformation contract) /
  scribere_fontem(parsura, tabularium, fons_index) (adds EOF
  lexema_finis tail trivia; Chunk B adds directivae).
- **Generator, two validations** (both regenerated grammars pass):
  S19 extended to NON-terminals (unmapped non-terminal in a
  genus/modus production = error — emission totality; previously
  only terminals were checked, so a subtree's bytes could silently
  vanish); slot-order validation (per production, mapped locus
  indices in RHS order must be monotone non-decreasing in the
  genus layout — the guarantee that makes ONE generic emitter
  correct for every genus; divergent-order and consistent-order
  grammars both pinned in probatio_silva_generare, now 57).
- **Fixed in passing**: latent NULL-deref in the generator's S19
  error path (printed prod->id unconditionally; modus productions
  may lack id) — error message now guards id and names the symbol
  kind.
- **Byte-identity across resolution proven**: `foo * bar;` emits
  identical bytes pre-collapse, post-collapse (oracle in driver),
  AND after a recanonicare flip 0→1 — interpretations share tokens,
  so emission is independent of the canonical choice; the probatio
  asserts it rather than assumes it.
- **Subtree emission documents the attachment rule**: second
  statement of `int x;\nint y;\n` emits `int y;\n` — the first
  `\n` is the previous `;`'s trailing trivia (deepest-node-ending
  ownership), its own trailing `\n` rides along. Moved-subtree
  fidelity behaves exactly as §3.4 predicts.

Complexities: none structural — the INTENTIO design survived
first contact. Next: Chunk B (simulation ⑥ first: expansion
boundary + directive re-insertion; cursor.c is the bar).

### Simulation ⑥ — COMPLETE (2026-07-02): layer-0 reconstruction

project-specs/silva-simulatio-6.md — pretend-implementation of the
Chunk B reconstruction path, every claim verified against source
(line-referenced). Headline: **the design survives; two mechanics
turned out already built** (directive capture is total, guard lines
included — "tres directivae captae"; untaken arms already persist as
lexemata_cruda). Eight complexities in the register (C1-C8), the
load-bearing ones: C1 one-directivae-stream-for-all-fontes (filter by
fons_index); C2 untaken arms = third byte source (reinserenda queue);
C3 attachment rules make the byte partition EXACT (nothing to build —
the trivia invariant does the work); C4 boundary arm = radix +
consecutive-run dedup by pointer; C6 included files' EOF dropped but
retrievable from includenda (latina.h tail bytes). Two decisions for
Fran: C5 function-like extents (~30 lines, nothing forces them —
recommend build now) and C7 guard-predefined-custos hole (recommend
defer + loud fracture). Estimated Chunk B: ~120 lines scribere, ~10
driver, ~30 expander if extents approved.

**Decisions (Fran, same day): C5 = BUILD NOW. C7 = FIX — via the
untaken-arm model** (Fran's "out of scope but never lost" instinct):
predefined-custos guard files route through the NORMAL region walker
instead of guard transparency — #ifndef evaluates false, interior
becomes an honest lexemata_cruda arm, lines captured normally.
~3-line fix, zero new scribere code, semantically queryable. Chunk B
scope is now fixed: driver capture + boundary arm (radix/dedup/chain
check) + reinserenda queue + included-EOF lookup + extents + C7 fix
+ fixtures (flip Chunk A's fail-loud test, preprocessor_test shape,
cursor.c closure bar, latina.h fons, fuzz shapes, corpus).

### Chunk B — COMPLETE (2026-07-02): layer-0 reconstruction

Suite 22/22, 1,140 assertions (probatio_silva_scribere now 110).
**THE M1 §6 BAR PASSES: cursor.c reconstructs byte-identical through
the full pipeline with latina.h praebere'd** — the probatio verifies
the bar is not vacuous (silva_expansio_quaerere("si") non-NIHIL: the
keyword macros really expanded, and the boundary rule really walked
them back). cursor.h and latina.h ALSO reconstruct byte-exact as
their own fontes (latina.h = pure directives + EOF-tail from
includenda, sim ⑥ C6 confirmed in the flesh).

What landed (per sim ⑥, all eight complexities embodied):
- **Expander**: SilvaExtentumInvocationis + exp->extenta — layer-0
  function-like invocation slices [nomen..')'] recorded in the
  generation walker (C5, Fran: build now); the C7 fix (predefined
  custos → normal region walker, interior = honest untaken arm —
  ~3 lines as predicted) with its behavior pinned in
  probatio_silva_regiones (region shape + 2 captured lines + interior
  cruda + cpp semantics: interior #define never registers).
- **Driver**: silva_parsare_cum_expansione (caller-prepared expansio:
  praebere + macro injection — the SilvaContextus seam, Phase 7 rides
  it); silva_parsare = thin wrapper; SilvaParsura gains directivae
  (capture switched on) and fons_princeps.
- **Scribere boundary arm** (_lexema_scribere): radix via the
  invocation chain with purity check (PASTA/CHORDA/API → fractura
  clara, named deferrals); dedup by EMITTED-RANGE (fons_ultimus +
  emissum_usque) — covers multi-token bodies split across slots,
  adjacent same-macro invocations, and arg-derived tokens inside
  extents, all without pointer-set state; extent emission is
  per-token WITH reinserenda flush (directive-inside-arguments stays
  byte-exact even in fuzz shapes); expansio NIHIL (subtree entries
  without context) → fractura clara.
- **Reinserenda queue** (fontem): directive lines (fons-filtered —
  sim ⑥ C1) + untaken-arm cruda from the region tree recursively
  (C2), xar_ordinare by offset, flushed before each layer-0 unit;
  EOF for included fontes fetched from includenda (C6).
- **Probatio**: Chunk A's fail-loud test FLIPPED to fidelity
  (`#define T int\nT x;` roundtrips; subtree without context still
  fractures, with context emits "T x;"); object-like (6 shapes incl.
  multi-generation + #undef point-in-time), function-like extents
  (5 incl. macro-in-arg and double invocation), regions/directives
  (9 incl. imperfecta, orphan #endif, #pragma passthrough, unknown
  include, the preprocessor_test composite), multi-fons includes
  (guard file reconstructs, re-include praetermissa keeps both
  lines), cursor.c closure.

Complexity (one, found by the failing tests on first run):
- **fons princeps is NOT fons 0 under praebere** — discovered-while:
  first run of the include fixtures (including-file reconstruction
  compared modulus.h's bytes against the main file's); consists-in:
  silva_includendum_praebere calls fons_addere, so praebere'd files
  claim indices BEFORE the main file — every consumer assuming
  "main = 0" breaks the moment a context is prepared; consequences:
  reconstruction targeted the wrong fons silently (test caught it —
  the bytes were another file's); handled-by: SilvaParsura gains
  fons_princeps (driver records it; -1 for lexemata entry); tests
  and future consumers use it instead of 0. API lesson: indices
  that "start at zero for the obvious thing" stop being obvious the
  moment preparation precedes creation.

Remaining Phase 5: Chunk C — the property net (incolumitas stage 4:
fuzz parse→emit==input 400/400; fidelitas upgraded from lex→emit to
tree-emit over the 78-file corpus; mensura emission timings).

### Chunk C — COMPLETE (2026-07-02): the property net

First run green across the board. Suite 22/22, 1,218 assertions.
- **Fidelitas**: the corpus now runs BOTH oracles per file —
  emittere(lexare(x))==x AND scribere(parsare(x))==x. 78/78 on both.
  The tree oracle runs the WHOLE pipeline: self-defined macros
  expand (boundary rule live), unknown includes stay learning-mode
  (lines captured + reinserted), recovery ERROR nodes carry real
  C89. Pre-corpus checked: no ## or # stringify anywhere in the 78
  files, so the named deferrals cannot trip it.
- **Incolumitas stage 4 — the strongest standing oracle**: all 400
  deterministic fuzz specimens (raw bytes incl. NUL + C-shaped)
  now assert scribere(parsare(x)) == x. 400/400. Fran's
  malformed-content-roundtrips requirement is a permanent assertion,
  not a design intention. (Fixed seed = the specimens never change;
  a seed change that produces a pasta-triggering specimen would
  fail LOUDLY and force the deferral decision then.)
- **Mensura**: scribere 0.0335 ms/KB over the corpus — emission is
  ~8× cheaper than parsing (0.26 ms/KB); arbores fideles 78/78
  reported alongside. Cost curve visible from day one, as CULTURA
  demands.

### RELATIO (2026-07-02) — PHASE 5 COMPLETE

All chunks green: A (generic emitter + slot-order validation),
A-addendum (per-token primitive shared with the lexer oracle),
B (layer-0 reconstruction — THE M1 §6 BAR: cursor.c byte-identical
with latina expanding; latina.h/cursor.h as own fontes), C (property
net). Suite 22/22, 1,218 assertions (1,015 at phase start). The
token→tree→bytes chain is CLOSED: every input — real C89, fuzz
garbage, macro-heavy latina code, multi-file include closures —
parses to a committed tree and returns byte-identical.

Standing oracles after this phase: corpus 78/78 (both lex + tree),
fuzz 400/400 (tree), syntaxis corpus 125/125, cursor.c closure.
Emission is a pure structural walk (transformation contract held:
moved subtrees emit their own bytes; position consulted only at the
file-level reinserenda merge).

Phase-5-owned parked items, all closed: layer-N emission spacing
(ONE missing-trivia policy: stored trivia wins; absent emits nothing
in the roundtrip emitter; style-default hook owned by the formatter
milestone), EOF lexema_finis tail trivia (emitted; included fontes
via includenda), list normalization interplay (rode commissio,
Chunk A of Phase 4).

Named deferrals with owners (unchanged or new): pasta/chorda/
stringificatio boundaries (fail-loud detection SHIPPED; first
corpus/grammar that forces them owns them); CONDITIONALIS-in-tree
emission (Phase 7 threading; ownership rule pinned — an arm's bytes
have ONE owner, slice or tree); layer-N view emission (query
milestone, rides the missing-trivia hook); formatter + style
profiles (§8.3, post-substrate); STML serialization (1.0);
transforms-across-arms fidelity requirement (config-query milestone,
pinned 2026-07-02).

Process note: simulation ⑥ (mid-phase, against shipped code) again
REDUCED the work — two mechanics were already built, the C7 "hole"
became a 3-line fix through Fran's untaken-arm framing, and the
implementation landed with ONE new complexity (fons_princeps),
caught by tests on first run. Mid-phase simulation is now 2-for-2.

Next: Phase 6 — Amalgamator (sim ④ decided the design; the manifest
now includes the Phase 4/5 modules: silva_glr, silva_commissio,
silva_parsare, silva_scribere + generated tables). Protocol: INTENTIO
first; re-read spec-v2 §3.5/§11.1 and the sim ④ artifacts.


## Phase 6 — Amalgamator

### INTENTIO (2026-07-02)

**What**: the deliverable mechanism — silva/amalgama/silva.c +
silva.h (SQLite style: develop modular, ship generated), produced by
a dev-time tool in instrumenta/ per sim ④'s PROVEN design (S41-S47,
spec-v2 §11.1). Proves: the §11.1 deliverable as specified, with the
Phase 4/5 modules aboard (the part sim ④ never saw).

**Protocol reads done**: spec-v2 §3.5 + §11.1, sim ④ file + interview
(the compiled artifacts lived in a dead session scratchpad; the
DECISIONS survive in the files), actual fontes dependency inventory.

**Discoveries from the inventory (manifest updates vs sim ④):**
- **internamentum is OUT of the vendored set** — no fontes file uses
  it (generator/instrumenta only). Vendored: latina + piscina +
  chorda + chorda_aedificator + friatio (3 fns, via tabula_dispersa)
  + tabula_dispersa + xar. Sim ④'s 7-lib compile included it;
  today's runtime needs 6 + latina.
- **stdio is a RUNTIME dependency** (fail-loud fprintf(stderr) in
  glr/commissio/nodus/parsare) — already in sim ④'s hoisted system
  set (ctype/stdio/stdlib/string); noted that a diagnostics hook
  (replacing stderr) is a post-M1 API nicety, NOT M1.
- **NOT amalgamated**: silva_tabulae_imparilis (test grammar —
  probationes only), instrumenta/ (dev-time firewall), credo.

**Design decisions:**
1. **silva.h is HAND-WRITTEN vanilla C89, transparent structs.**
   Generation via the expander is foreclosed for now by the layer-N
   spacing gap (expanded tokens carry no trivia — the emitted header
   would be glued mush; the formatter milestone owns that). Drift is
   SELF-CORRECTING: silva.h's struct definitions are the ONLY ones
   in the amalgam TU (internal silva_*.h headers are NOT re-emitted;
   bodies compile against silva.h's types + latina's keyword macros)
   — a wrong field type/name fails the amalgam compile under
   -Werror. Transparent (not opaque+accessors) because code-is-a-
   database wants field access and the single-header property makes
   layout drift impossible between host and library. Public API
   names ≤ 31 chars (S47).
2. **Assembly order** (S43): [system includes] [silva.h verbatim]
   [latina.h] [renamed vendored decls] [vendored bodies] [silva
   bodies]. Include guards pass through verbatim (S46).
3. **Renamer**: token-aware, built on SILVA'S OWN LEXER (instrumenta
   links fontes — the bootstrap sim ④'s prototype anticipated).
   Exact-identifier match, skip comments/strings; longest-prefix
   function map (piscina_* → silva_piscina_* ...) + exact type map
   (Piscina → SilvaPiscina ...). silva_* names already prefixed —
   untouched. Vendored functions get static injection (hic_manens)
   on BOTH prototype and definition (S44); excluded functions
   (friatio beyond the 3) dropped entirely, prototypes included.
4. **Tool shape**: instrumenta/silva_amalgamare.{h,c} + principalia
   main + silva/amalgamare.sh (mirrors generare.sh). Manifest =
   explicit arrays in the tool (two orders, exclusions — the
   manifest IS code, reviewed like code). Output committed (like
   generated tables); drift guard = regenerate-and-diff in the
   script.
5. **Verification lives in the script** (compile steps): standalone
   full-flag compile of silva.c; generated host-pollution test (host
   declares si/per/character/nomen, includes silva.h, compiles
   clean); nm-intersection vs rhubarb's own .o files (zero shared
   externals beyond intended); duplicate-static detection in the
   tool itself (S41, enforced not assumed). The probatio
   (probatio_silva_amalgama) does harness-level checks: artifacts
   exist, silva.h contains no latina tokens, equivalence smoke
   (a fixture parsed by an amalgam-linked runner == modular result —
   exact mechanism decided in Chunk C).

**Chunks:**
- **A — assembler + renamer, vendored set only.** Replicates sim ④
  against TODAY'S six libs with the real tool (not a script):
  manifest arrays, two orders, exclusions, static injection, rename;
  standalone compile green. Exit: amalgama of the vendored substrate
  compiles under the full flag set.
- **B — silva modules + silva.h.** Hand-write silva.h (public API:
  token/nodus/valor/oraculum/resolutor/parsare family/cum_expansione/
  scribere/recanonicare/praebere/macro_addere + sceletum generated
  surface); silva bodies join the manifest; internal headers
  excluded; full silva.c standalone compile. Exit: the deliverable
  pair exists and compiles.
- **C — the verification net.** Host-pollution generated test;
  nm-intersection; equivalence smoke; drift guard wiring;
  amalgamare.sh + committed artifacts; probatio green in the suite.

**Exit criteria**: suite green incl. probatio_silva_amalgama;
silva/amalgama/{silva.c,silva.h} committed artifacts; all script
verifications pass; RELATIO written.

**Explicitly NOT Phase 6** (named): solarium-side compile (Phase 7 —
the real host); diagnostics hook for stderr (post-M1); silva.h
GENERATION from internal headers (formatter milestone unlocks it);
C89-grammar tables in the manifest (M2 swaps sceletum out).

### Chunk A — COMPLETE (2026-07-02): assembler + renamer, vendored set

First run green end-to-end: the tool compiled, ran, and its output
compiled standalone under the full flag set. Suite untouched (22/22).

What landed:
- **instrumenta/principalia/amalgamator.c** (~700 lines, single file
  — deliberately OUTSIDE the instrumenta/*.c globs: both build
  scripts link every instrumenta object, and the amalgamator needs
  fontes symbols the generator build doesn't provide). Manifest as
  code: header order + body order (S42) + per-file keep-lists (S44,
  friatio → 3 functions). Tokenizes with SILVA'S OWN LEXER (the
  bootstrap moment — silva_lexare + the per-token walk).
- **The unit scanner**: top-level declaration units by paren/brace
  depth (';' at depth 0, or the matching '}' of a function
  definition); classifies typedef/function/data; extracts the title
  (ident before '(' / before '='/'[' / last before ';'); detects
  already-static (STATIC genus or interior/hic_manens/staticus/
  universalis).
- **Transformations**: project includes dropped; system includes
  hoisted + deduped (4: stdlib/string/stdio/ctype); latina.h
  VERBATIM (S43); guards verbatim (S46); token-aware rename (exact
  type map + longest-prefix function map — 440 hits); static
  injection on non-static function units (264); keep-list exclusion
  (zero sha1/sha256/crc32 residue incl. their hic_manens tables);
  duplicate-DEFINITION detection across corpora (S41, enforced —
  header prototypes exempt by est_corpus/est_definitio flags).
- **silva/amalgamare.sh**: builds tool, generates
  silva/amalgama/silva.c (committed artifact, 164 KB), verification
  compile standalone.
- **Measured result: ZERO external defined symbols in the object** —
  the entire vendored substrate is internal linkage. Sim ④ measured
  155 externals pre-treatment; the target "export surface = public
  API only" is now trivially exceeded (no API yet — Chunk B adds it).

STAGE MARKER (explicit obligation): the verification compile carries
-Wno-unused-function — with no consumer in the file yet, every
static is unused by construction. Chunk B/C REMOVE the suppression
once silva bodies consume the libs, growing keep-lists to cover
whatever remains genuinely unused (S44's "zero unused" end state).

Design notes caught in my own review before compiling (would have
been -Werror/-Wcast-qual failures + one behavior bug): double
trivia-ante emission in the unrenamed path; const-dropping cast in
the duplicate registry; the duplicate check flagging legitimate
header-prototype/body-definition pairs (fixed via est_corpus +
est_definitio gating).

Next: Chunk B — silva modules join the manifest + hand-written
vanilla silva.h; then C — host-pollution test, nm-intersection,
equivalence smoke, drift guard, probatio.

### Chunk B — COMPLETE (2026-07-02): THE DELIVERABLE EXISTS

silva/amalgama/{silva.c (472 KB), silva.h} — the full substrate in
one file, compiling standalone under the full flag set. Export
surface: 91 external symbols, EVERY one silva_/SILVA_-prefixed
(zero host-collision surface); everything vendored is internal
linkage. Suite 22/22 throughout.

The design that made it work (the C89 single-definition analysis):
- **Only typedef units can conflict in one TU** — duplicate
  compatible function declarations and identical macro
  redefinitions are LEGAL. So silva.h owns the ~40 host-facing
  types (hand-translated vanilla: i8→unsigned char, i32→unsigned
  int, s32→int, b32→int, chorda→SilvaChorda...), and the
  amalgamator drops exactly those typedef units from internal
  header emission (CADENDA list). Tag-DEFINITIONS are dropped only
  where silva.h fully owns the struct (SilvaToken/SilvaValor/
  SilvaNodus/SilvaCaecatio); forward-only types (SilvaOraculum,
  SilvaExpansio — source-TAGGED for this, a 2-line fontes refactor)
  keep their internal definitions.
- **Drift is self-correcting**: silva.h's definitions are the only
  ones in the TU; internals compile against them under -Werror —
  the hand translation was verified by the amalgam compiling on
  the FIRST attempt after collision fixes. One non-verifiable
  constant (SilvaXar.segmenta[64] vs XAR_MAXIMUS_SEGMENTORUM)
  guarded by an emitted C89 negative-array static assert.
- **Scanner upgrades**: tag-def titles (structura X {...}; → X),
  function-pointer typedef titles ((*IDENT)), est_vendicata gating
  (static injection + rename only for vendored; silva API stays
  extern), NON_STATICA (piscina generare/destruere + xar
  numerus/obtinere stay public — hosts need them).

**S41 catch (the detector's first real prey)**: three cross-module
static helper collisions in silva's OWN bodies — _chordae_aequales
(token.c vs expandere.c), _est_nomen_potentiale and _chorda_figere
(expandere.c vs conditio.c) — invisible in modular builds, duplicate
definitions in one TU. Sim ④ measured zero among vendored libs but
never saw silva's bodies. Fixed by natural-Latin renames in the
less-central file (_chordae_pares; _est_nomen_conditionis;
_chordam_figere_conditionis); suite green.

Remaining: Chunk C — host-pollution generated test, scripted
nm-intersection, equivalence smoke (amalgam-linked runner == modular
result), drift-guard wiring (regenerate-and-diff), probatio, and the
-Wno-unused-function removal via keep-list tightening (S44 end
state).

### Chunk C — COMPLETE (2026-07-02): the verification net

Suite 23/23, 1,229 assertions (probatio_silva_amalgama 11).

- **S44 END STATE REACHED**: the -Wno-unused-function stage marker
  is GONE. The exclusion fixpoint converged in four
  compiler-harvested rounds (93 → 11 → 7 → 2 → 0): ~110 unused
  vendored functions named in per-file excludenda lists, statics
  orphaned by drops included. friatio ships exactly ONE function
  (fnv1a — djb2 and fnv1a_literis died with the dropped
  tabula_*_literis chain). silva.c: 371 KB, compiles standalone
  under the FULL flag set, zero suppressions.
- **hospes.c — the first true host** (instrumenta/principalia/,
  vanilla C89, includes ONLY silva.h): declares and USES variables
  named si/per/character/nomen/structura/redde/dum/vacuum (S43
  pollution proof — one leaked define and it doesn't build), then
  runs 8 equivalence fixtures through the amalgam — object-like +
  function-like expansion, regions, garbage recovery, praebere'd
  include via the public API. 8/8 fideles, byte-exact. Compiled and
  RUN by amalgamare.sh on every regeneration.
- **nm-intersection scripted**: amalgam externals ∩ rhubarb's own
  objects = 0, enforced in amalgamare.sh (sim ④'s measurement is
  now a permanent gate).
- **probatio_silva_amalgama** (in the ordinary suite): the REAL
  pollution check — lexes latina.h, harvests its 388 #define names,
  lexes silva.h, asserts ZERO identifiers match (comments exempt as
  trivia — Latin prose stays welcome); silva.c artifact properties
  (XAR drift assert present, zero surviving project includes,
  statics present, sizes sane).

### RELATIO (2026-07-02) — PHASE 6 COMPLETE

**The deliverable mechanism is REAL and GUARDED.** silva/amalgama/
{silva.c 371 KB, silva.h} — committed artifacts, regenerated by
amalgamare.sh which enforces on every run: standalone full-severity
compile, host-pollution + 8-fixture equivalence via hospes, zero
nm-intersection. The suite guards the artifacts' properties
independently (23/23, 1,229).

Sim ④'s design survived intact, extended by three things it never
saw: the CADENDA type-ownership rule (only typedefs conflict in one
TU — silva.h owns host-facing types, internal headers keep the
rest), silva's own modules in the manifest (S41 caught three real
cross-module static collisions), and the exclusion fixpoint at full
scale (four rounds, compiler-harvested).

Named onward: solarium-side compile of the amalgam = Phase 7 (the
real host next door); stderr diagnostics hook (post-M1); generated
per-genus accessors + genus enum in the public surface (M2 with the
C89 grammar — the sceletum's generated surface stays internal
except tables + constructor + fabrica); silva.h GENERATION from
internal headers (formatter milestone). M2 note: new silva code
consuming a vendored function currently excluded = delete its
excludenda entry (the compile clamat).

M1 remaining: Phase 7 — skeleton integration (§11.2 checklist,
owned debts: caps+cancellation, Prosser exactness, latina SET via
SilvaContextus, CONDITIONALIS threading, v1 praeparator spot-check,
solarium corpus dimension). Then the M1 RELATIO and the
widen-first decision.

### Requirement pinned — transforms across conditional arms (2026-07-02)

Fran, thinking about selector queries/replacements: they must apply
across ALL blocks, including arms mutually exclusive in any compiled
binary. This is the wildcard-conditional VISIO pin, but the TRANSFORM
half adds a requirement the config-query milestone should inherit
explicitly rather than rediscover:

- Untaken-arm parsing (deferred: config-query milestone / Phase 7
  CONDITIONALIS threading) must produce trees at the SAME fidelity
  standard as the taken arm — full trivia, re-emittable,
  transformable — because once a transform touches an arm, its
  lexemata_cruda slice is STALE and emission must come from the
  arm's tree.
- Ownership rule for the migration: an arm's bytes have exactly ONE
  owner at any time — reinserenda slice (unparsed, Phase 5 interim)
  or tree (threaded, Phase 7+) — never both. Chunk B's reinserenda
  design is the interim owner and forecloses nothing: the generic
  emitter already walks CONDITIONALIS rami (lista-nodus) the day
  they exist; arm-directive lines will need token slots on ramus
  nodes (Phase 7 genera-extra design item).


## Audit — phase boundary sweep before Phase 7 (2026-07-03)

Full phase-log re-read per protocol (the practice that found four
drifts at the Phase 4 boundary). Findings this time: every parked
item has a named owner; three drifts/notes and one API seam:

1. **CONDITIONALIS threading carries a citation drift.** Phase 4/5
   entries attribute it to "§11.2", but §11.2/§6's M1 definition
   never requires in-tree threading — it requires the two-arm #ifdef
   through the EXPANDER path (done, Phase 2). The obligation is real
   (spec §8.1.4 "layers are trees: region nodes inline" + the
   one-owner pin above), but its Phase-7 placement is a phase-log
   decision, not a spec checklist item. Kept in Phase 7 deliberately
   (see INTENTIO decision 4) at minimal scope; recorded so the
   "§11.2 says so" justification doesn't harden.
2. **Prosser's landing spot renamed itself**: the pre-Phase-4 audit
   said "external corpus (Lua/zlib)"; the external corpus became
   solarium (ring 1.5, Fran 2026-07-02). Same disposition, real name.
3. **Integration target CHANGED (2026-07-02, tessera decision)**:
   "solarium-side compile of the amalgam" is REPLACED as the
   real-host proof by saltuarius (the silva explorer TUI on the
   tessera library — project-specs/tessera-spec.md), which consumes
   amalgama/silva.h only. Solarium remains as CORPUS (ring 1.5).
   Phase 6's "Named onward" entry predates this change.
4. **contextus naming seam (source-verified)**: the resolutor's
   closure parameter is named `contextus` across silva_parsare.h /
   silva_commissio.h — it will collide with SilvaContextus the day
   the object exists. The API is one phase old; rename it in Phase 7
   Chunk A before the collision is real.

**§11.2 checklist status**: every item green with a pointer — tiny
annotated grammar (P3), expander with guard file + two-arm #ifdef
(P2), unified tokens (P1), tables + construction (P3), the fork case
end-to-end incl. re-canonicalization (P4), pater fixup (P4), cursor.c
roundtrip (P5), through the amalgamator + standalone compile (P6),
crash-freedom fuzz attached (P2.5→P5). Remaining before the M1
RELATIO: exactly the Phase-7-owned debts.

**Corpus evidence gathered at the boundary** (grep census over
../solarium): 148 .c/.h files; **ZERO ## paste usage anywhere** — the
pasta boundary deferral will NOT be forced by this corpus;
function-like macro definitions present in ~46 files (extents already
built, Phase 5 C5); dense local-include graph (praebere at scale, or
learning mode — roundtrip holds either way); stringify usage unknown
(the Chunk C survey reports exactly).


## Phase 7 — Skeleton integration (M1 closure)

### INTENTIO (2026-07-03)

**What**: close M1. The owed debts (caps + cancellation, SilvaContextus
+ latina definition SET, CONDITIONALIS threading, Prosser disposition,
v1 praeparator spot-check) + solarium as corpus, then the M1 RELATIO
and the widen-first decision. Proves: the substrate is hostile-input
safe, host-configurable, and holds against a real foreign codebase.

**Protocol reads done**: full phase-log audit (above), spec-v2 §6 +
§8.2 + §10 + §11.2, silva/CLAUDE.md, source verification
(silva_parsare.h contextus params, silva_expandere.h cap inventory,
solarium grep census).

**Design decisions:**

1. **SilvaContextus = the long-lived host object** (spec §8.2 caps +
   cancel; §10.2.5 presets later): {caps struct, cancellation
   callback + stride, definition-set loading}. It RIDES
   silva_parsare_cum_expansione — the seam Phase 5 built for it —
   via a new entry silva_parsare_cum_contextu (or contextus param
   added to cum_expansione; decide at the API sketch). M1 scope:
   caps + cancellation + definition sets. Named config presets =
   config-query milestone (named). The resolutor's vacuum* closure
   parameter is RENAMED (datum_resolutoris) across the API to free
   the name — one phase old, cheap now, expensive after M2.
2. **Caps degrade EXPANSION, never totality.** On token-volume cap,
   the remaining reliqua flows through UNEXPANDED (raw layer-0
   tokens, diagnostic recorded) — every capped parse still yields a
   complete committed tree and byte-exact roundtrip; the fuzz oracle
   holds UNDER caps (adversarial fixtures: exponential expansion
   #define A B B... chain, deep region nesting). Cap set: total
   expanded tokens, GSS frontier, region nesting depth, include
   depth (exists, 32), expansion generations (assertion → cap).
   Cancellation: deterministic count-based checkpoints (no timers)
   in the three unbounded loops — expansion fixpoint, GLR drain,
   slicing loop; cancelled → partial results + successus FALSUM +
   causa, terminal state honest.
3. **latina definition SET = compiled-in, literally.** The
   amalgamator already reads latina.h; it additionally emits its
   text as data (silva_latina_textus, ~4 KB static string) and a
   contextus API praeberes it (the cursor.c closure path, now one
   call). Modular builds read the same header from disk. This
   honors the interview's "compiled-in defaults" answer without a
   second source of truth — the emitted data IS the vendored header.
   (Reviewable: if embedding-as-data feels wrong, the fallback is
   caller-passes-text; the API shape is identical.)
4. **CONDITIONALIS threading — minimal scope, simulation ⑦ first.**
   Genera-extra design: conditionalis node (rami:lista-nodus) +
   ramus node (directive-line token slots, contentum). Taken ramus
   holds parsed content; untaken rami hold their lexemata_cruda as
   token payloads (the ERROR-node pattern — bytes in the tree,
   structure later). One-owner migration in scribere_fontem:
   threaded regions leave the reinserenda queue (arm bytes emit from
   the tree; assert no double-emission). Untaken-arm SUB-PARSING
   stays at the config-query milestone (the pinned fidelity
   requirement above governs it when it lands). THE HAZARD sim ⑦
   must trace before any code: regions are layer-0 offset-based,
   the tree is built from the EXPANDED stream via the slicing
   driver — regions that straddle statement boundaries, sit inside
   a statement, or interleave with macro expansion are the cases
   that decide whether threading happens at the slicing layer, the
   commit walk, or a post-commit pass.
5. **Solarium enters as corpus via a SURVEY first, probatio second.**
   instrumenta/principalia/lustrum.c (the census tool): every
   solarium .c/.h through parse→emit==input with praebere'd local
   closure where resolvable; reports per-file fidelity, fracture
   causes, ERROR-node counts, timings. THEN probatio_silva_solarium
   pins the honest assertable number. Solarium is NOT vendored
   (lives next door, evolves — recorded decision): the probatio
   SKIPS green-with-a-loud-note when ../solarium is absent.
   If the survey surfaces stringify-in-use → implement the CHORDA
   boundary rather than shrink the bar (maximalist default, the
   Phase 5 function-like precedent). ## is already ruled out by
   the boundary census.
6. **Prosser disposition by evidence**: revisit HS_call∩HS_rparen
   against the solarium survey; if no real-world case surfaces,
   document as a PERMANENT conservative simplification (header note
   in silva_expandere.h + a phase-log close), not a dangling debt.
7. **v1 praeparator spot-check**: read the v1 suite file for input
   STRINGS the Phase-4-audit coverage mapping missed; harvest
   anything found into fixtures; close the item either way.
8. **Amalgam discipline applies all phase**: new public API
   (SilvaContextus family) → extend hand-written silva.h; new
   vendored consumption → delete excludenda entries; amalgamare.sh
   after every fontes edit.

**Chunks:**
- **A — SilvaContextus + caps + cancellation.** The object, the
  renamed resolutor param, cap enforcement with graceful raw-flow
  degradation, cancellation checkpoints, latina SET loading, the
  cursor.c closure test rewritten through the contextus path,
  adversarial cap fixtures (exponential expansion, deep nesting,
  capped-parse roundtrip assertions). silva.h extended;
  probatio_silva_contextus.
- **B — CONDITIONALIS threading.** Simulation ⑦ (region→tree
  mapping traced through real structs; file in project-specs/ if it
  earns one), then: genera-extra shapes in both grammars +
  regenerate, threading at the layer sim ⑦ selects, scribere
  one-owner migration, fixtures (two-arm, nested, straddling,
  est_imperfecta, region-inside-statement), fuzz + corpus + closure
  oracles stay green (the migration must not move a single byte).
- **C — solarium corpus + closure sweep.** lustrum survey + report;
  probatio_silva_solarium (honest number pinned); Prosser
  disposition; v1 praeparator spot-check; mensura at solarium scale;
  stringify decision iff the survey forces it.

**Exit criteria**: suite green incl. new probationes; capped/cancelled
parses still roundtrip byte-exact; cursor.c closure passes through
the contextus path; CONDITIONALIS nodes in committed trees with
one-owner emission (zero byte drift across the migration); solarium
fidelity number pinned honestly; Prosser + praeparator dispositioned
in writing; amalgam regenerated with silva.h extended; Phase 7
RELATIO; then **M1 RELATIO as its own entry** — §11.2 walked
item-by-item with pointers, and the widen-first decision (grammar
coverage vs expander directive coverage vs query surface) made with
its first real customer input: saltuarius v0 wants the preprocessor/
layer story (DONE) + file navigation now, tree views at M2, selector
queries at the query milestone.

**Explicitly NOT Phase 7** (named, with owners): untaken-arm
sub-parsing + transforms-across-arms fidelity + named config presets
+ wildcard configuration queries (config-query milestone); stderr
diagnostics hook (post-M1 API nicety); C89 grammar + boundary-finder
upgrade + three-valued oracle + 3-way ambiguity paths (M2);
formatter/style profiles (§8.3), STML serialization (1.0), layer-N
view emission (query milestone); solarium-BUILD compile (replaced by
saltuarius as real-host proof — tessera project, 2026-07-02
decision); saltuarius itself (after tessera exists).

### Chunk A — COMPLETE (2026-07-03): SilvaContextus + fines + intermissio

Suite 24/24, 1,310 assertions (probatio_silva_contextus 81, FIRST
RUN). Amalgam regenerated + all gates green (standalone full
severity, hospes 8/8, nm-intersectio 0, double-run IDEMPOTENT);
silva.c now 434 KB (latina datum + contextus aboard).

What landed:
- **fontes/silva_contextus.{h,c}** — the long-lived host object:
  SilvaFines {lexemata, generationes, profunditas_includendi,
  profunditas_regionum, frons} (0 = unlimited; generous defaults
  ALWAYS on — tree-sitter posture, Fran-approved), SilvaPergereFunctio
  (documented MONOTONE: once false, always false), lexica + praebenda
  as copied SilvaContextusPlagula recipes. Contextus is a RECIPE:
  each parse creates a fresh expansio and applies it
  (_contextum_applicare in the driver — contextus.c stays a dumb
  config holder; lower layers never see the type, they get raw
  fields copied down).
- **Naming: definition set = LEXICON** (silva_contextus_
  lexicon_addere — "definitiones_addere" was 35 chars, over the S47
  ≤31 bar; a lexicon is exactly what latina.h is). Lexicon events get
  positus CLAMPED to 0 after processing (they were relative to the
  lexicon's own reliqua stream; clamping makes them active from
  main-stream position 0 regardless of stray non-directive content).
- **Caps degrade expansion, never totality** (the INTENTIO principle,
  now proven by assertion): volume/generation caps checked at
  generation boundaries (_generationem_licere — one helper, both
  fixpoint loops); capped stream flows on UNEXPANSED; parse + emit
  proceed; roundtrip stays byte-exact under EVERY cap (asserted per
  cap in the probatio). Region depth: _regionem_processare got a
  depth-counter wrapper (all return paths covered) + est_ultra_modum
  regions where NO arm evaluates — whole region retained cruda,
  recursion stops. **This closed a REAL pre-existing hole: a
  2000-deep #if nest recursed 2000 frames before (stack overflow on
  hostile input); now capped at 64, byte-exact, asserted.** Include
  depth: constant 32 → configurable field, same behavior. GLR:
  limen_frontis (default 4096) → est_ultra_limen = clean segment
  error → ERROR node (totality); counted in
  parsura->segmenta_ultra_limen.
- **Cancellation**: checkpoints in the three unbounded loops —
  expansion fixpoint (per generation), GLR drain (per passus_pergendi
  tokens, default 1024), slicing loop (per segment). On cancel the
  driver folds the ENTIRE remaining tail into ONE ERROR node and
  stops — tree still complete, roundtrip still exact (asserted).
  DIVERGENCE from INTENTIO wording, deliberate: SilvaParsura.successus
  stays VERUM under cancellation (the tree IS complete);
  est_intermissa carries the truth. GLR-fructus-level est_error is
  where "failure" lives, per contract.
- **latina compiled-in, literally**: amalgamator step 0 emits
  fontes/silva_latina_datum.{h,c} (byte array — C89's 509-char string
  literal limit forbids one literal) from include/latina.h on every
  amalgamare.sh run; the file is a fontes citizen (modular builds +
  amalgam both compile it); silva_contextus_latinam_addere praeberes
  it as a lexicon. Probatio asserts the compiled array ==
  include/latina.h ON DISK (drift guard both directions). Provenance:
  si→if chains land in fons "latina.h" as real def-site tokens.
- **Rename executed**: resolutor closure param contextus →
  datum_resolutoris across commissio/parsare (fontes + silva.h);
  the SilvaContextus name is now free and taken.
- **New parsura product fields**: est_intermissa, expansio_decisa,
  fines_tactae, segmenta_ultra_limen (+ mirrored in silva.h with the
  four new type defs, contextus API, latina externs,
  silva_parsare_cum_contextu; CADENDA_TYPEDEF += 4, CADENDA_DEFINITIO
  += SilvaContextus).
- **cursor.c closure THROUGH the contextus path** (M1 bar, new
  route): latina via the COMPILED lexicon (not praebere), cursor.h/
  color.h via contextus praebenda; `#include "latina.h"` stays an
  unknown include (line captured + reinserted) while the macros are
  already active from position 0 — byte-exact, quaerere("si")
  non-vacuous. The Phase 5 praebere-path test remains alongside
  (both routes pinned).

Complexities: none structural — the INTENTIO design survived first
contact (81 assertions first run; the only mid-writing corrections
were two hand-counted string lengths swapped for strlen before ever
compiling). Note for M2: expansion volume is checked at generation
BOUNDARIES — a single generation can still amplify up to
stream × max-body-length before the check fires; acceptable
(deterministic, bounded by source shape), documented here.

Next: Chunk B — CONDITIONALIS threading, simulation ⑦ first
(region→tree mapping through the real structs; the
straddling/statement-interior cases decide the threading layer).

### Simulation ⑦ — COMPLETE (2026-07-03): CONDITIONALIS threading

project-specs/silva-simulatio-7.md — pretend-implementation against
shipped code, every claim line-referenced. Mid-phase simulation now
3-for-3 at reducing work. Headline: **threading is a PRE-COMMIT
DRIVER PASS** (none of the INTENTIO's three guessed layers) — the
driver already owns the radix between slicing and committere (the
ERROR-node precedent); the commissio walk is generic over loci
(silva_commissio.c:130) so pater/normalization come free; the
emitter's fons filters make threading FONS-UNIFORM with zero special
casing (included-file regions thread identically). **Zero engine
changes** — expander +~15 lines, driver pass ~150-200, scribere ~30.

The straddling hazard DISSOLVES into detection + degrade: alignment
= every radix element overlapping a region falls fully inside the
taken arm's body (layer-0 extents via the Phase 5 _radix_probata
machinery); intra-statement/straddling/multi-fons/pasta-opaque/
tail-ERROR cases simply fail detection and the region stays
slice-owned (today's proven behavior IS the fallback). Threading is
opt-in per region.

Register C1-C8; decisions (Fran, all recommendations): **β ownership
inversion** (regions own their structural directive lines ALWAYS;
capture calls removed; directivae = non-region directives only —
one-owner extended to lines, no filtering ever); **genus split**
ramus-sumptus/ramus-omissus (selector tags free; est_numquam
journal-side, named); **conditio_id:index locus** on both ramus
genera (tree↔journal↔regions join pinned now); **counters**
regiones_textae/regiones_omissae on SilvaParsura (solarium
degradation rate in Chunk C = config-query design input).
Node shapes: conditionalis {rami:lista-nodus, finis:lista-token};
ramus-sumptus {directiva:lista-token, contentum:lista-nodus,
conditio_id:index}; ramus-omissus {directiva:lista-token,
cruda:lista-token, conditio_id:index}. Layout order == byte order
throughout — the generic emitter needs zero new cases.

### Chunk B — COMPLETE (2026-07-03): CONDITIONALIS threading

Suite 25/25, 1,390 assertions (probatio_silva_conditionalis 74,
FIRST RUN). Amalgam 458 KB, all gates green. **Conditional regions
are now TREE CITIZENS** — and the standing oracles (corpus 78, fuzz
400, syntaxis 125, both cursor closures) rode through the entire
migration byte-exact: zero drift, as the sim demanded.

What landed, exactly per simulation ⑦'s fixed scope:
- **Expander (~15 lines as estimated)**: SilvaRamus gains
  corpus_initium/corpus_finis (exclusive end = next directive's
  offset; imperfecta = open 0x7FFFFFFF); SilvaRegio gains
  directiva_finis (the #endif line, stored at the ENDIF exit) +
  est_texta. **β executed**: the two _directivam_capere calls in the
  region walker are GONE — regions own their structural lines;
  directivae_out = non-region directives only (guard-file lines
  stay captured — guard files have no region). The unthreaded-region
  fallback: _regiones_colligere in scribere now collects rami
  directiva + directiva_finis + cruda from the region tree (texta
  regions skipped, filiae always visited — a degraded inner region
  inside a threaded outer keeps its reinserenda ownership and the
  offset-flush machinery interleaves it correctly during tree
  emission).
- **Generator**: NECESSARIA extended to 5 (ramus-sumptus/-omissus
  REQUIRED, spec §12.2 contract); shape validation rewritten
  data-driven (10 FormaExigenda rows replace the one-off ambiguus
  block); both grammars' genera-extra extended + regenerated (same
  states, same 1 preserved conflict — genera-extra don't touch
  productions).
- **The weaving pass (driver, pre-commit — sim ⑦ C4)**: _texere +
  _regiones_texere (region tree depth-first, filiae first) +
  _regionem_texere. Detection: per-element layer-0 extents
  (_extentum_valoris — EXPANSIO chains walked to invocation radix;
  PASTA/CHORDA → opaque; mixed fontes → opaque; opaque intersecting
  = conservative degrade), full-containment test against the taken
  arm's corpus bounds, straddle = degrade (regiones_omissae++).
  Rebuild: grouped elements → contentum, conditionalis inserted at
  first-grouped ordinal (or first-after-region same-fons position,
  or end), foreign-fons elements stay siblings. Forms found in the
  registrum BY NAME (commissio pattern). Commissio then walks the
  woven nodes generically — pater fixup verified by assertion
  (ramus->pater == conditionalis).
- **probatio_silva_conditionalis (74)**: basic two-arm (structure +
  conditio_id joins + one-owner: directivae count 0 for pure-region
  input), #else-taken via #if 0, empty taken arm, nesting (inner
  conditionalis as outer contentum element), intra-statement +
  straddle degradations (counters asserted), defines-only region
  (zero grouped; #define line stays in directivae and reinserts
  INSIDE the conditionalis span byte-exactly), imperfecta (finis
  empty), include-inside-arm (foreign elements outside contentum;
  BOTH fontes reconstruct byte-exact — sim ⑦ C7 fons-uniformity in
  the flesh), three-arm elif (omissus/sumptus/omissus).
- **Deliberate test revisions** (logged per β decision):
  probatio_silva_regiones C7 case now asserts lines on the region
  (ramus->directiva + directiva_finis) and directivae == 0;
  probatio_silva_contextus region-cap case now asserts the woven
  radix (1 conditionalis element, textae 3) — the cap and the
  threading compose.

Complexities: none structural — sim ⑦'s design survived contact
(the only mid-writing corrections were the recurring i32-unsigned
sentinel trap in new struct fields, caught by -Werror before any
run). Named onward: est_numquam as queryable genus (journal-side
for now); untaken-arm sub-parsing at taken-arm fidelity
(config-query milestone, pinned requirement governs);
extent-computation is per-region O(n·m) (recompute per region —
mensura watches; solarium in Chunk C is the scale test).

Next: Chunk C — lustrum survey over solarium, probatio_silva_
solarium (honest number pinned; skip-green if ../solarium absent),
Prosser disposition, v1 praeparator spot-check, mensura at scale.

### Chunk C — COMPLETE (2026-07-03): solarium corpus + closure sweep

Suite 26/26, 1,404 assertions (+probatio_silva_solarium). Amalgam
regenerated, gates green.

**THE HEADLINE: 148/148 solarium files roundtrip byte-exact through
the full pipeline** — 2.3 MB of real plain-C89 (zero latina, the
first genuinely foreign corpus), largest file 836 KB (main.c), with
the header closure praebere'd through a SINGLE reusable
SilvaContextus (the Chunk A object's first real consumer). 1.35
ms/KB whole-pipeline; zero caps tripped; 41,011 ERROR nodes across
44,676 segments = the recovery soak counted honestly (the sceletum
grammar doesn't pretend to understand C89 — bytes survive anyway).

- **instrumenta/principalia/lustrum.c + silva/lustrum.sh** — the
  census tool (a lustrum is a Roman census): per-file fidelity with
  first-divergence diagnostics, ERROR/segment/region/inclusion
  counters, timings, and the PROSSER SCAN (unbalanced-paren
  function-like macro bodies = the precondition for the
  HS_call∩HS_rparen divergence class).
- **The stringify deferral FIRED and was IMPLEMENTED, not shrunk**
  (the INTENTIO pre-decided it): 11 files infideles on first run,
  all one cause — solarium's CHECK macros use #c stringification.
  The CHORDA boundary landed: _radix_probata chases
  origo.datum.stringificatio.primus (the RAW operand token, which
  lies INSIDE the invocation extent), and extent lookup falls back
  to CONTAINMENT (_extentum_continens — CHORDA radices are argument
  tokens, not name tokens, so pointer-keyed lookup misses). The
  weave's extent walk got the same hop (stringify statements no
  longer opaque). Second run: 148/148. In-suite fixtures added to
  probatio_silva_scribere (5: solo-#x body, multi-token arg,
  CHECK-shaped mid-body, empty arg, #x between body tokens). Pasta
  (##) remains the LAST fail-loud boundary deferral — zero uses in
  solarium, still owned by the first corpus that forces it.
- **PROSSER DISPOSITION CLOSED — permanent conservative
  simplification**: the lustrum scan found ZERO unbalanced-paren
  function-like macros in 2.3 MB of real code; the divergence class
  is unreachable in this corpus. Documented permanently in
  silva_expandere.h (with evidence citation); the pre-Phase-4
  audit's orphan is retired.
- **Real-world threading rate measured** (config-query milestone
  design input, sim ⑦ C8): 56 regiones textae / 9 omissae — 86% of
  real conditional regions align with statement boundaries and
  thread; the 9 degradations are intra-statement/straddling shapes
  (the sub-parse machinery's future justification, now with a
  number).
- **v1 praeparator spot-check CLOSED** (the Phase-4 audit's last
  open disposition): 35 input strings reviewed; harvest = indented
  directives (`  #ifdef` — v1 tested, silva never had), #endif
  without trailing newline at EOF, `#if !defined(X)` — all three
  now fixtures in probatio_silva_conditionalis (messis v1 section),
  all green. Everything else maps to existing coverage.
- **probatio_silva_solarium** — the corpus bar IN THE SUITE: all
  files byte-exact (RATIO asserted — fideles == numerus — not raw
  counts, since the corpus evolves), textae > 0. Solarium absent →
  loud note + green (not vendored, lives next door — recorded
  decision honored).

Complexities: none new — the one surprise (stringify-in-use) was a
pre-decided branch, and its fix used two existing mechanisms
(origo chase + extent table) with ~45 new lines.

### RELATIO (2026-07-03) — PHASE 7 COMPLETE

All three chunks green: A (SilvaContextus + fines + intermissio,
81), B (CONDITIONALIS threading, 74), C (solarium corpus + closure
sweep). Suite 26/26, 1,404 assertions (1,229 at phase start).
Amalgam 458+ KB, all gates, every regeneration idempotent.

**The owed debts are all paid, each with its receipt:**
- Caps + cancellation (§8.2): five configurable fines, generous
  defaults always on, degrading EXPANSION never TOTALITY — roundtrip
  asserted under every cap; a real stack-overflow hole (unbounded
  region recursion) found and closed on the way.
- SilvaContextus + latina SET: the long-lived host recipe; latina
  compiled-in literally (amalgamator-emitted data, drift-guarded
  against disk); the resolutor's `contextus` name freed by rename
  BEFORE the collision existed.
- CONDITIONALIS threading: regions are tree citizens (86% thread
  rate on real code), one-owner extended to directive lines (β),
  conditio_id join pinned, zero engine changes, zero byte drift.
- Prosser exactness: DISPOSITIONED as permanent conservative
  simplification, with corpus evidence (zero unbalanced-paren
  macros in 2.3 MB).
- v1 praeparator spot-check: CLOSED with three harvested fixtures
  (indented directives the notable catch).
- Solarium as corpus: 148/148 byte-exact IN THE SUITE — and it
  forced (and received) the stringify boundary, per the maximalist
  default. Pasta (##) is now the only fail-loud boundary left,
  still unforced by any real corpus.

Process notes: simulation ⑦ ran mid-phase and reduced the work for
the third consecutive time (zero engine changes was ITS finding);
the i32-unsigned sentinel trap fired for the third time (new struct
fields carrying -1 — it is now the codebase's most reliable slip);
the contextus object found its first real consumer (lustrum +
solarium probatio) within one phase of being built.

M1 remaining: the M1 RELATIO itself (§11.2 walked item-by-item)
and the widen-first decision — which now has a customer
(saltuarius: preprocessor/layer story DONE and proven at corpus
scale; tree views want M2's C89 grammar; selector queries want the
query milestone).


## M1 — WALKING SKELETON: RELATIO (2026-07-03)

**M1 IS CLOSED.** Seven phases (0-7), four mid-course simulations
(④ pre-M1 counted separately; ⑤⑥⑦ mid-phase, 3-for-3 at reducing
work), 26 probationes, 1,404 assertions, and a verification-gated
deliverable a real host consumes. The spec-v2 §6/§11.2 obligations,
item by item with receipts:

- **Tiny annotated grammar (declarations + expressions, ambiguity
  present)** — grammatica/sceletum.stml, frozen annotation v0,
  EXACTLY one preserved conflict = the designed typedef fork
  (Phase 3; probatio_silva_grammatica pins it by production id).
- **Fresh expander core; one est_custos guard file + one two-arm
  #ifdef end-to-end** — Phase 2 (probatio_silva_regiones); grown
  since into two-track regions, point-in-time expansion (the
  carried-suite catch), guards, includes, #/##, variadic, caps.
- **Unified tokens end-to-end** — SilvaToken with embedded tagged
  origo everywhere from lexer to emitter (Phase 1; the Lexema/Token
  split that killed provenance in lapifex is structurally
  unrepresentable).
- **Generated tables + construction** — baked Latin-commented
  committed artifacts, registrum + accessors + constructor +
  fabrica, drift-guarded (Phase 3; probatio_silva_tabulae).
- **The genuine fork case** — `foo * bar` with unknown foo:
  localized AMBIGUUS with canonical spine, byte-exact roundtrip,
  re-canonicalize IN PLACE on typedef injection, identity held
  through transmutatio→collapse on one allocation (Phases 4-5;
  probationes ambiguitas/commissio/scribere).
- **Post-accept pater fixup** — the commissio walk (Phase 4 Chunk C;
  S27 discharged, spine semantics pinned).
- **cursor.c roundtrip byte-identical** — through the FULL pipeline
  with latina genuinely expanding, via TWO routes: praebere (Phase
  5) and the compiled-in lexicon (Phase 7 Chunk A).
- **Through the amalgamator, compiled standalone** — silva/amalgama/
  {silva.c, silva.h}, full flag set, zero suppressions, hospes
  host-pollution + equivalence, nm-intersection 0, idempotent
  regeneration (Phase 6).
- **Crash-freedom fuzz harness attached** — 400 deterministic
  specimens through the whole pipeline INCLUDING byte-exact
  re-emission, running in the suite since Phase 2.5-era audit
  (probatio_silva_incolumitas).
- **Beyond the checklist, accumulated en route**: recovery totality
  (any bytes → complete committed tree → identical bytes), the
  malformed-roundtrip permanent oracle, conditional regions as tree
  citizens (86% real-world thread rate), hostile-input caps with
  graceful degradation, host cancellation, a long-lived contextus
  recipe object, the stringify boundary, and a 148/148 byte-exact
  foreign corpus IN THE SUITE.

**What M1 deliberately does not claim**: the sceletum grammar does
not understand C89 (41k ERROR nodes over solarium is recovery, not
comprehension — the honest counter says so); untaken arms carry
bytes, not structure; pasta (##) boundary emission stays fail-loud
(no corpus forces it); selectors/queries/formatter/STML are their
own milestones.

**Why this one lived where three died** (the record should say it
plainly): every load-bearing contract was simulated before it was
built, every narrowing was written down with a named owner, every
generation's tests were carried, and the phase-boundary audits
caught drift while it was still cheap. The maximalist data model —
provenance never optional, all arms retained, values tagged,
structural genera required — is what made Phases 5-7 additive
instead of archaeological.

**WIDEN-FIRST DECISION (Fran, 2026-07-03): tessera → saltuarius
v0.** Silva rests at M1 while its real customer gets built: the
tessera TUI library (project-specs/tessera-spec.md, interview
done), then saltuarius v0 on the preprocessor/layer story that is
already corpus-proven (miller columns + origo chains + layers +
regions). Real usage generates API pressure BEFORE M2 freezes more
surface — the same reasoning that made saltuarius the host proof.
M2 (C89 grammar; the 125-input syntaxis corpus waits as its
acceptance bar) begins when saltuarius demands tree views.
Re-entry: tessera-spec v2 (agent codebase exploration) is the
named next act on that track.


## Post-M1 addendum — the amalgamator became shared machinery
## (2026-07-03, executed as tessera Phase C step 1)

Silva's amalgamator was RESTRUCTURED, gated, and is now two-project
infrastructure (full record: tessera/phase-log.md Phase C RELATIO;
this addendum is the silva-side pointer so a silva session isn't
surprised by its own tooling):

- **Mechanism** (~700 lines: unit scanner, renamer, static
  injection, S41 dedup, CADENDA, system-include hoisting, optional
  latina-datum emission, assembly) now lives in
  **instrumenta/silva_amalgama.{h,c}** behind an AmalgamaManifestum
  struct; it compiles in the ordinary instrumenta glob.
- **instrumenta/principalia/amalgamator.c is now manifest + main
  only** (~310 lines) — the S44 maintenance rules are UNCHANGED
  (new vendored use → delete its excludenda entry; new public API →
  extend hand-written silva.h).
- **THE EXTRACTION GATE HELD**: silva.c and both
  silva_latina_datum files regenerated BYTE-IDENTICAL (md5); suite
  26/26; all amalgam gates green. amalgamare.sh's verification
  object renamed amalgama_verificatio.o (the old name collided with
  the new mechanism object in build/ — 102 duplicate symbols in
  probatio links until renamed).
- Tessera's amalgamare.sh compiles the mechanism + silva's lexer
  from silva/ (dev-time bootstrap dependency, same class as the
  generator's stml dependency).
- **Convention lesson for silva too (tessera Phase C)**: the latina
  macro list of forbidden identifiers grew — `magnitudo` (sizeof)
  and `exire` (exit) joined nomen/registrum; exire had silently
  macro-renamed a struct FIELD in every modular TU and only
  amalgamation exposed it. A latina-collision check on hand-written
  vanilla headers (the probatio_silva_amalgama pattern) is the
  named guard class.

## SILVA ADDITIONES I — INTENTIO (2026-07-03)

**The first post-M1 reopening, and the widen-first plan paying
out**: saltuarius (the first true silva.h consumer) generated an
8-item API-pressure ledger at spec time
(project-specs/saltuarius-spec-v2.md §2). This package ships the
declaration-tier items — #1 public lexing, #7 convenience
helpers, #8 arena telemetry — and unblocks saltuarius Phase B
(layer-0 coloring). Engine-tier items (#2 fons_via, #3 strata
retention, #4/5/6 vistas) are ADDITIONES II, explicitly not here.

**Simulation VIII findings (4-for-4 on sims reducing work)**:
(a) all eight #1/#7 functions are ALREADY extern in the shipped
amalgam — zero manifest edits for them; (b) NO GATE type-checks a
silva.h declaration (standalone compiles without silva.h; C links
without mangling) — a wrong-typed declaration ships silently and
corrupts in the first host; the ONLY guard is a host call;
(c) lexed tokens ALIAS the source text ("fons NON copiatur") —
the iterator-titulus bug class, must be documented AT the
declaration; (d) the renderer wants silva_lexare_cruda (raw
interleaved stream: comments/whitespace as items), hit-testing
wants silva_lexare (real lexemata, origo attached) — both ship;
(e) the pollution guard lexes silva.h ITSELF — new declarations
are latina-checked automatically.

**Scope** (decisions: cruda yes / hospes-rule / both telemetry):
1. **silva.h +10 declarations** (vanilla, ≤31 chars, param names
   checked against latina defines): silva_lexare +
   silva_lexare_cruda (with the LIFETIME CONTRACT in the doc
   comment: tokens alias fons; fons_index 0 licet for standalone
   use), silva_token_radix / _profunditas / _est_fons,
   silva_nodus_liberi, silva_lexema_genus_nomen,
   silva_origo_genus_nomen, silva_piscina_summa_usus,
   silva_piscina_summa_apex_usus (memoriae_index → size_t).
2. **Manifest**: EXCLUDENDA_PISCINAE −2 (both summa accessors —
   ONE shared array covers header+corpus rows), NON_STATICA +2.
   Renames automatic via piscina_→silva_piscina_.
3. **Hospes +~25 lines calling ALL TEN** — the type-drift guard.
   **NEW STANDING RULE: every function silva.h gains must be
   CALLED by hospes in the same change.** Named debt: 27 of the
   pre-existing 34 declarations lack hospes calls — backfill
   opportunistically, not here.

**Gates**: amalgamare.sh full (standalone full-severity, hospes,
nm-intersectio 0 — silva.c legitimately CHANGES; byte-identity
was the refactor's gate, not this one's), silva suite 26/26
(pollution guard auto-adapts), tessera + saltuarius suites as
cross-project sanity.

**Exit criteria**: all gates green; hospes exercises all ten new
functions with content assertions (lexare on a tiny fons: token
count + a genus + a valor; cruda: trivia present in-stream;
telemetry: usus > 0, apex >= usus); spec-v2 §2 ledger marked
(#1/#7/#8 SHIPPED); memory updated; saltuarius Phase B unblocked.

### SILVA ADDITIONES I — RELATIO (2026-07-03): SHIPPED

**Ledger items #1, #7, #8 are public.** silva.h +10 declarations
(lexare, lexare_cruda with the tokens-alias-fons lifetime contract
in the doc comment, token_radix/profunditas/est_fons,
nodus_liberi, lexema/origo_genus_nomen, piscina_summa_usus/
apex_usus), manifest −2/+2 (telemetry resurrection), hospes 8/8 →
14/14 with content assertions on every new function. All gates
green (standalone full-severity, hospes, nm-intersectio 0); silva
26/26, saltuarius 6/6, tessera 5/5. ~1 hour, as the simulation
estimated. **Saltuarius Phase B is unblocked.**

**Simulation VIII correction (recorded for honesty and for the
next maintainer's peace of mind)**: the sim claimed NO gate
type-checks silva.h declarations. WRONG — and the error was
instructive: the amalgam embeds silva.h VERBATIM as its caput, so
the standalone compile checks every declaration against every
definition in the same TU. It proved itself immediately: six
conflicting-type errors on the first regeneration caught (a) the
i32-unsigned slip IN REVERSE (declared profunditas as int; i32 is
unsigned — the codebase's most reliable slip now bites in both
directions) and (b) missing const on both telemetry params
(definitions take constans SilvaPiscina*). The embedded-caput
design is a self-checking contract — the same property that made
tessera's macro redefinitions self-guarding.

**The hospes rule survives anyway, re-founded**: the type gate is
free, but hospes calls check RUNTIME behavior (lexare really
returns 5 lexemata; trivia really interleave in cruda; apex >=
usus) and serve as the canonical usage examples for each
declaration. The standing rule stands: new silva.h function =
hospes call, same change. (Named debt unchanged: 27 pre-existing
declarations lack calls.)

One design nuance surfaced writing the hospes liberi check: a
sceleton segment's loci are LEXEMATA, so silva_nodus_liberi
legitimately returns an EMPTY xar at M1 — the call type-checks
and the xar is valid; child-bearing trees arrive with M2's
grammar. Noted so nobody mistakes empty-liberi for a bug.

## SILVA ADDITIONES II — INTENTIO (2026-07-03)

**The Phase C unblock package**: ledger items #2-#6
(saltuarius-spec-v2 §2). Shaped by Simulatio IX, whose findings
dissolved every open decision:
(a) STRATA RETENTION IS FREE — generation Xars are arena-allocated
unconditionally (silva_expandere.c:2311, exitus in exp->piscina);
strata_out merely records pointers the gubernator currently drops.
The measured parse cost already includes every generation →
retention is ALWAYS-ON (VISIO: layers are the product) at zero
marginal cost. Engine change = parsare.c:956 NIHIL→&strata +
parsura->strata field. NB strata[ultimum] ALIASES
parsura->lexemata (layer-numbering note for hosts).
(b) exp->rami is ALREADY FLAT (conditio_id = index+1) — the ramus
vista is index-addressed reads; the REGION TREE stays internal.
(c) Extents are BYTE OFFSETS (corpus_initium =
primum_corporis->byte_offset, expandere.c:786) — the ledger's
arm-extent caveat resolves to the best case (hosts dim by byte
range; no token walking).
(d) SilvaParsura is transparent — the strata field must mirror in
silva.h and the embedded-caput gate TYPE-CHECKS the mirror free
(Simulatio VIII's discovery, now working for us).

**Scope**:
1. **#3 strata**: SilvaParsura += `SilvaXar* strata` (internal +
   silva.h mirror, same position); gubernator passes &strata;
   NIHIL when the stream never mutated (document).
2. **#2 fontes**: silva_fontes_numerus(exp) +
   silva_fons_via(exp, fons_index) → const SilvaChorda* (NIHIL si
   extra fines; interned via, synthetic fontes return their
   titulus).
3. **#4 inclusiones**: SilvaInclusioVista {via, fons_ex, fons_ad,
   est_praetermissa} + numerus/vista pair (by-value xar walk).
4. **#5 rami**: SilvaRamusVista {genus, est_sumptum, est_numquam,
   corpus_initium, corpus_finis, fons_index, linea} +
   numerus/vista pair over the FLAT exp->rami (index =
   conditio_id - 1; fons_index/linea from ramus->regio).
5. **#6 macros**: SilvaMacroVista {titulus, est_functio,
   fons_index, linea} + numerus/vista pair filtering exp->acta
   DEFINITIO events (journal semantics: definitions AS OCCURRED,
   #undef does not erase history — the journal is the product;
   O(acta) per call accepted at v1 scale, documented).
6. **hospes calls for ALL new functions** (standing rule; the
   multi-fons fixture grows #define/#if arms so strata and every
   vista have non-trivial content to assert).

**Gates**: amalgamare.sh full; silva/tessera/saltuarius suites;
spec-v2 ledger marked SHIPPED; memory updated. Exit: Phase C (the
magic) unblocked.

### SILVA ADDITIONES II — RELATIO (2026-07-03): SHIPPED

**Ledger #2-#6 public; PHASE C UNBLOCKED.** Exactly as Simulatio
IX shaped it:
- **#3 strata**: parsura->strata retained (gubernator NIHIL →
  &strata; field appended internal + silva.h mirror — the
  embedded-caput gate type-checked the mirror as designed). FREE
  as the sim proved; always-on; hospes asserts strata[ultimum] ==
  parsura->lexemata THROUGH THE PUBLIC API (the aliasing is now a
  pinned contract, not an accident).
- **#2 fontes**: silva_fontes_numerus + silva_fons_via (bounds →
  NIHIL).
- **#4/#5/#6 vistas**: SilvaInclusioVista / SilvaRamusVista
  (over the FLAT exp->rami; byte-offset extents; fons_index/linea
  via ->regio) / SilvaMacroVista (acta DEFINITIO filter, journal
  semantics — #undef erases nothing). Types owned by silva.h,
  internal typedefs CADENDA-dropped; SilvaRamusGenus enum went
  public alongside.
- **hospes 14/14 → 19/19** (standing rule): strata + aliasing,
  fons_via incl. out-of-range, rami vista on the #if 0/#else
  fixture (numquam + sumptum + byte extents ordered), GEMINARE
  macro vista (functio, titulus bytes), empty-inclusiones
  behavior. All content assertions, all green first run after one
  regeneration.

Gates: amalgamare full (standalone/hospes/nm), silva 26/26,
saltuarius 10/10, tessera 5/5. silva.c 469,923 octeti.

Zero complexities — the first additiones package with NO
first-regeneration type errors (Simulatio IX read the shapes with
its own eyes instead of trusting reports; the lesson from VIII's
i32-slip compounding).

**Next: saltuarius Phase C — the magic** (layer flips with
token-identity cursor, origo popup, Enter-to-jump; all silva.h
prerequisites now exist).

---

## M2 — GRAMMATICA C89: phase sketch (plan of record, revisable)
## (2026-07-03)

**The pull fired as designed.** M1's close set the condition —
"M2 begins when saltuarius demands tree views" — and it was met
2026-07-03: saltuarius v0 SHIPPED (daily-driver bar passed), and
Fran, living with the Tab TOC pane, called the switch: "work on
extending c89 support." The first tree view M2 feeds back is the
TOC's FUNCTIONES section.

**What M2 stands on (all shipped, all gated)**: the full
preprocessor substrate (conditionals as tree citizens, strata,
origo), the generator pipeline (annotated stml → conflict-
preserving LALR → baked Latin-commented tables → generated
construction), the GLR runtime (tagged values, localized AMBIGUUS
with canonical spine, accept-reconciliation, post-accept oracle
registration, re-canonicalization IN PLACE), scribere byte-exact
emission, segments with recovery totality, and the amalgam gates.
**The engine risk is spent. M2 is grammar AUTHORING**: ~115
annotated rules where every terminal carries a named locus
(trivia single-owner enforced at generation) and every node genus
is a permanent name.

**The territory map**: lib/lapifex_c89_grammatica.c — 4,565
lines, 108 rules, the previous generation's complete C89 grammar
(phases 1-6b). Not reusable (no annotations, no trivia loci,
first-parse-wins ambiguity); as a MAP it removes all exploration:
every precedence tier, rule ordering, and conflict is already
charted once. Translation-and-upgrade, not discovery.

**Phases**:

- **M2.0 — NOMINA (the thousand-year decision, FIRST).** The
  full Latin vocabulary: node genera (kebab-case tags — these
  become the quaestio selector surface and solarium's query
  language FOREVER; Eskil's api-first rule applies to this
  vocabulary more than to any C signature) + locus names per
  production + the terminal set completion. Deliverable: a
  genera-c89 vocabulary document, REVIEWED WITH FRAN (his Latin,
  his query language), before any rule is authored. Also decided
  here: c89 grammar file strategy (proposal: grammatica/c89.stml
  FRESH with its own baked SILVA_C89_* symbols; sceletum.stml
  stays FROZEN as the generator's test fixture; saltuarius
  switches grammars when M2d lands — revisable). Proves: nothing;
  names everything.
- **M2a — Expressiones (~40 rules).** The precedence ladder
  comma → 11 assignments → ?: → logical/bitwise tiers →
  equality/relational/shifts → additive/multiplicative → cast →
  unary (both sizeof forms) → postfix (call/index/./->/++/--) →
  primary. Named forks arrive: cast-vs-call `(t)(x)`,
  sizeof(typus) vs sizeof expressio — each gets an EXPLICIT
  policy (fork + AMBIGUUS or declared preference), never
  first-parse-wins. Bars: lapifex expression suite semantics
  (25 tests/120 assertions, carried); the expression half of the
  v1 syntaxis harvest; byte-exact roundtrip of every fixture
  THROUGH THE TREE. Proves: annotation v0 + AMBIGUUS machinery
  at real-grammar width.
- **M2b — Declarationes + typi (~45 rules).** Order-free
  declaration specifiers (arbor2's specifiers_ordine lesson),
  init-declarator lists, the declarator recursion (pointers with
  qualifiers, arrays, params, FUNCTION POINTERS — spec §7's named
  adversarial-fixture risk), abstract declarators, brace-nested
  initializers, struct/union/enum with bitfields (arbor2 phase
  3.5 lesson). THE ORACLE DANCE AT SCALE: typedef declarations
  register at segment commit and feed subsequent segments;
  within-segment unknowns stay localized AMBIGUUS (both readings
  retained — the wildcard-query pin WANTS them); frons_maxima /
  fusiones counters become the fork-explosion watchdog with
  measured ceilings. Proves: the M1 fork case was the general
  mechanism, not a demo.
- **M2c — Sententiae + functiones (~30 rules).** Compound blocks
  (C89 declarations-before-statements), si/alioquin with the
  dangling-else conflict resolved by DECLARED preference in the
  conflict-preserving tables, loops, switch, goto/labels; then
  function definitions — ANSI *and K&R parameter style* — and
  translation-unit-as-list, at which point the segment boundary
  heuristic ()-before-{, spec §7) faces its adversarial fixtures.
  Full v1 syntaxis harvest completes here (125 inputs / 234
  assertions re-asserted against silva genera). Proves: whole
  real files acquire structure.
- **M2d — The acceptance wall.** The honest counter flips
  meaning: M1 recorded 41k ERROR nodes over solarium as
  RECOVERY; M2d drives that number toward zero as COMPREHENSION
  (the headline metric, tracked in the harness like parse
  time/arena bytes). Tier-1 roundtrip corpus (78 files) byte-
  exact through real trees; solarium 148/148 held; a
  parse-the-monorepo sweep; carried lapifex integration bar
  green; **saltuarius grows the FUNCTIONES section** (structura's
  fourth section walking declaration vistas — the API-pressure
  loop closes where it started). Post-M2d: quaestio selectors /
  formator / typus+index stay their own milestones with their
  frozen v1 bars (100/103/… assertions), per §4.1
  freeze-then-delete.

**SALTUARIUS IN THE LOOP (Fran, 2026-07-03).** The widen-first
loop runs CONTINUOUSLY through M2, not batched at the end — and
with a reframe: during M2, saltuarius integrations are
INSTRUMENTS for the manual bars first, features second (the F2
precedent: built as the LRU instrument, kept as a feature). The
delivery mechanism is the proven VISTA pattern (slim by-value
structs, additiones-II class): each tree tier that saltuarius can
see ships its vista same-phase, and the vista IS that phase's
API-pressure test. Governing rule: an integration lands IN-PHASE
only when it is the best instrument for that phase's manual bar;
anything merely nice goes to the saltuarius v0.1 backlog BY NAME.

Per-phase map:
- M2.0: no code — but the naming session TESTS each genus name by
  reading it as a TOC row / breadcrumb segment aloud (the names
  will be USER-VISIBLE in saltuarius, not just query syntax).
- M2a: no TOC presence (nobody navigates to the third
  multiplicative expression). OPTIONAL instrument: the node
  BREADCRUMB (cursor token → genus chain in the status line) for
  eyeballing precedence on real files — build only if the M2a
  bar wants eyes; otherwise it arrives free at M2c when
  pater-walks exist for functions anyway.
- M2b: TOC sections TYPI / DECLARATIONES via a declaration vista
  (genus, titulus, extent, linea — mirrors inclusio/ramus/macro).
  Structura grows its fourth section; the vista is M2b's
  pressure test.
- M2c: **FUNCTIONES — the original pull.** The M2c manual bar IS
  "Tab on silva_parsare.c lists every function, Enter lands on
  the definition." Signature text in rows quietly exercises
  scribere-from-subtree.
- M2d: the comprehension metric goes AMBIENT — status badge
  and/or F2 line gain the per-file ERROR-node count ("silva: N
  errores"), so ordinary daily-driver use continuously runs the
  acceptance test; incomprehension gets noticed, not audited.

Saltuarius NON-GOALS (defended boundary): statement-level
browsing (noise, not navigation); expression trees as UI; any
AST-inspector pane — structural visualization at that depth is
SOLARIUM's reason to exist; saltuarius stays at warden
granularity (files, declarations, provenance).

**Standing discipline** (unchanged, restated for the M2 sessions):
every grammar edit → ./silva/generare.sh (probatio_silva_tabulae
clamat on stale tables) → ./silva/amalgamare.sh (the suite does
NOT catch a stale amalgam); new public API → hand-extend silva.h
+ hospes call same-change; simulate contested designs before
building (the sim ledger is 5-for-5); phase-boundary full-log
audits; complexities in the four-part schema; every narrowing
named with its landing spot.

**Named risks going in** (spec §7 + graveyard): fork explosion on
typedef-dense real headers (watchdog counters + measured
ceilings, M2b); boundary-finder vs function-pointer declarators
and K&R (adversarial fixtures, M2c); annotation lock-in — v0
format meets constructs it never saw (struct members, enumerator
lists, initializer nesting; first friction = revise the FORMAT
consciously, not per-rule hacks); the coverage mountain (the map
bounds it: ~115 rules is finite and charted).

NEXT: M2.0 INTENTIO — the naming session (vocabulary doc +
grammar-file decision), with Fran at the table.

---

## M2.0 — NOMINA: INTENTIO (2026-07-03)

**The thousand-year decision, taken first.** Node genus tags
become the quaestio selector surface, solarium's query language,
and saltuarius's TOC rows — user-visible names that outlive every
implementation (genus identity in the engine is a registry index;
the STRING TAG from the grammar file is the canonical name).
Eskil's api-first rule applies to this vocabulary more than to
any C signature. Nothing is proven in this phase; everything is
named. Method (Fran signed off 2026-07-03): DRAFT-THEN-CORRECT —
Claude drafts candidates with reasoning and query-surface
consequences; Fran corrects (his Latin, his query language); each
name is tested by reading it aloud as a TOC row / selector.

**Inventory basis (extracted 2026-07-03)**:
- lapifex_c89_grammatica.c: 54 rule entries (52 unique names —
  the file holds TWO grammar strings, expression + statement/
  declaration, four rules shared), 207 productions. The
  structural map.
- arbor2 node genera: 51 ARBOR2_NODUS_* values — the semantic
  inventory of what a C89 tree needs. Both prior inventories are
  MIXED-LANGUAGE (pointer, struct_member, initializer,
  TRANSLATION_UNIT, PARENTHESIZED, INITIALIZOR) — the naming debt
  M2.0 pays down once, permanently.
- sceletum.stml PRECEDENT (kept where it holds): kebab-case genus
  tags; tok_ prefix for terminal loci; semantic child-locus names
  (sinister/dexter/internum); genus unification across
  productions (binarium); folium- prefix for literal leaves;
  structural genera in <genera-extra>.
- **SILVA_LEX_* is ALREADY COMPLETE** (95 genera shipped public
  in silva.h) — the sketch's "terminal set completion" collapses
  to mechanical <terminalis> mapping, ZERO naming decisions.
  Discovery that shrinks the phase.

**Deliverable**: silva/grammatica/genera-c89.md — the vocabulary
document, organized by tier (radix / expressiones /
declarationes / sententiae / structuralia + locus conventions),
every C89 construct owning exactly one genus tag, alternatives
and reasoning recorded so the decisions keep their why (raw-
transcripts-precede-specs culture applied to names).

**Named questions going in (the session agenda)**:
1. POLICY: latina-macro words as genus tags (si, redde, frange,
   structura, magnitudo...). Tags are STML strings and generated
   identifiers are always silva_-prefixed, so macro expansion
   cannot fire (macros expand whole tokens only) — proposal: the
   latina keyword IS the genus tag for keyword constructs (source
   reads `redde x;`, tree reads genus `redde`). One policy
   decision, not per-name. Hand-written code keeps the six-firing
   prohibition unchanged.
2. Granularity forks with selector consequences: assignatio own
   genus vs binarium-with-operator; one binarium vs per-operator
   genera; folium- prefix retention; typedef as distinct genus vs
   same-declaratio + semantic marking.
3. The barbarism budget: parametrum (Greek loan) or not; what
   replaces arbor2's INITIALIZOR; the array word; the tree-root
   name (truncus? radix per STML precedent? unitas-translationis
   for standard fidelity?).
4. Grammar-file strategy (sketch proposal, needs sign-off):
   grammatica/c89.stml FRESH with its own baked symbols;
   sceletum.stml FROZEN as generator fixture; saltuarius switches
   grammars at M2d.

**Exit criteria**: genera-c89.md reviewed and corrected by Fran;
every construct named; locus conventions pinned; grammar-file
decision recorded; RELATIO with any narrowings named. No code,
no gates — the deliverable is the vocabulary.

### M2.0 — NOMINA: RELATIO (2026-07-04) — COMPLETE

**silva/grammatica/genera-c89.md is DECISUS** — the pinned
vocabulary. ~55 genus tags across five tiers + locus conventions,
every name carrying its reasoning and rejected alternatives.
Draft-then-correct worked as designed: Fran approved the draft
wholesale ("those all look great"), then three questions that
recommendations couldn't cover got explicit sign-off via
question tool (all three landed on the recommendation):

1. **typedef = semantic mark, no distinct genus.** Grammar stays
   uniform; oracle detects TYPEDEF at commit; query surface gets
   a pseudo/header-field. Named path back: genus split is
   ADDITIVE if ever needed.
2. **c89.stml FRESH**; sceletum.stml FROZEN as generator
   fixture; saltuarius switches at M2d. (The sketch proposal,
   now decided.)
3. **Switch bodies GROUPED** — casus/ordinarius own their
   sententiae lista (tree-sitter shape, ISO shape rejected with
   previews compared). Consequences recorded in the doc:
   fall-through = empty first group; grammar mechanics =
   M2c INTENTIO item; titulatum stays ISO-shaped.

**Decisions of note beyond the map**: P1 (latina keywords ARE
genus tags — safe because tags are STML strings and generated
identifiers are silva_-prefixed; the hand-code prohibition
unchanged); P3 (one binarium, operator in token — vocabulary
stays ~55 not ~90); truncus/acies/congeries/campus/passus/
discrimen/virgula/accessus-vs-membrum all approved; arbor2's
INITIALIZOR retired; species-typi avoids the nomen landmine
entirely.

**Deferred, with owners**: folium-chorda adjacent-literal
concatenation shape (M2a, when the annotation meets it);
grouped-casus grammar mechanics (M2c); genera-extra slot-shape
verification against the M1 skeleton registry (M2/c89.stml
authoring time — c89 registry must be a SUPERSET).

Zero complexities — no code was written. NEXT: M2a INTENTIO
(expressiones, ~40 rules; first fixture set + the cast-vs-call
and sizeof fork policies).

---

## M2a — EXPRESSIONES: INTENTIO (2026-07-04)

**What M2a proves**: annotation v0 + the AMBIGUUS machinery at
real-grammar width. The sceletum exercised one designed fork in
17 productions; M2a authors ~40 rules with a whole FAMILY of
typedef-driven forks and the full C89 precedence ladder, all in
the DECISUS vocabulary (grammatica/genera-c89.md — authoring is
translation now, not naming).

**Grammar file**: grammatica/c89.stml, FRESH (M2.0 decision) —
ONE file growing across M2a→M2d; truncus's elementa alternatives
widen per phase (M2a: sententia-expressionis only). Baked as
fontes/silva_tabulae_c89.{h,c} + silva_c89_construere ("c89"
untranslated, like offset). genera-extra carried from sceletum
VERBATIM + verified against the generator's NECESSARIA registry
(the M2.0 deferral, discharged in Chunk A). sceletum.stml
untouched, frozen.

**Pinned policies (named forks — never first-parse-wins)**:
- **Fork family** (all typedef-driven, all fork + localized
  AMBIGUUS): cast-vs-call `(t)(x)`; cast-vs-binary `(t)+x`,
  `(t)-x`, `(t)*x`, `(t)&x` (the four unary/binary-shared
  operators; `(t)~x`/`(t)!x` are unambiguous casts);
  sizeof-fork `magnitudo(t)` → magnitudo-typi vs
  magnitudo-expressionis (distinct genera BY M2.0 DESIGN so
  AMBIGUUS interpretations are queryable apart). Only
  typus-nominatus forks — typus-primitivus keywords can't be
  expressions.
- **Canonical-spine default, pinned project-wide**: the
  EXPRESSION (identifier) reading is canonical when the oracle
  doesn't know the name; oracle registration flips to the type
  reading via recanonicare at commit. Rationale: unregistered
  names degrade gracefully as expressions, and the oracle only
  ever LEARNS types. (M2b declaratio forks inherit this
  default.)
- **ISO grammar shapes**: assignment LHS restricted to the
  unary tier (kills `a+b=c` at grammar level, right-assoc);
  ternary middle = full expression, right side = conditional
  tier; argumenta = separated lista at assignment tier (no
  virgula ambiguity in calls, by construction).

**Chunks**:
- **A — scala praecedentiae** (the unambiguous ladder, ~25
  rules): truncus + sententia-expressionis carrier; virgula →
  assignatio → ternarius → binarium (10 tiers, ONE genus per
  P3) → unarium → postcrementum / vocatio / subscriptio /
  accessus → folia + parenthesis. folium-chorda adjacent-literal
  decision lands here: proposed shape = hidden modus-lista rule
  collecting STRING_LIT tokens → folium-chorda genus production
  with lista-token slot (machinery believed sufficient — shift
  TOKEN values append into lista; if annotation v0 can't express
  it, that's the FORMAT-revision trigger, consciously, per the
  standing rule). Baked tables committed; **conflict count
  pinned at ZERO** (surgical-ambiguity discipline: every
  conflict named or none). Dev tree-printer
  instrumenta/principalia/arbor.c (parse arg/stdin → indented
  genus/locus dump) — the M2a manual bar's eyes; the saltuarius
  breadcrumb stays UNBUILT unless Chunk B inspection demands it
  (per-phase map: M2a has no TOC presence).
- **B — typus intrat** (casts + sizeof, the fork family):
  species-typi MINIMAL (multi-token typus-primitivus,
  typus-nominatus, pointer-chain-only declarator-abstractus —
  arrays/function pointers are M2b); conversio;
  magnitudo-expressionis + magnitudo-typi. **Conflict census
  pinned EXACTLY by production-id pairs** (sceletum precedent:
  pin by id, not state number). Fixtures prove BOTH readings of
  `(t)(x)` and `magnitudo(t)` + hand resolutor (probatio-side,
  as in Phase 4) proves the recanonicare flip in BOTH
  directions.
- **C — bars**: carried lapifex expression suite
  (probationes/probatio_lapifex_c89_expr.c: 25 PROBARE / 32
  inputs, "x" through "a||b&&c|d^e&f==g<h<<i+j*k" and
  "*p++ + sizeof(int*)") re-asserted against silva genera;
  v1 syntaxis corpus EXPRESSION half harvested (non-ERROR trees
  + genus spot-checks); incolumitas fuzz extended through the
  c89 grammar; mensura baselines (parse ms/KB + frons_maxima
  over the fixture corpus — the fork-explosion watch starts
  recording HERE, ceilings set at M2b); silva.h public surface
  for the c89 grammar (accessor analog to the sceletum bundle)
  + hospes call SAME-CHANGE; full amalgam gates.

**Standing discipline applies**: every grammar edit →
generare.sh → amalgamare.sh; roundtrip byte-exact THROUGH THE
TREE for every fixture (the ruthless oracle rides from Chunk A);
new public API → silva.h + hospes.

**Named risks**: frontier growth on operator-dense parenthesized
chains — measured not guessed; the Phase-4 evidence-gated
action-filter hook is the named remedy if numbers demand it.
Annotation-friction candidate #1 = folium-chorda token-lista.
Any UNNAMED conflict appearing in the census = stop and
disposition it, never suppress.

**Exit criteria**: suite green; Chunk A census == 0 and Chunk B
census == the named family, both pinned in probationes; both
fork fixtures prove both readings + oracle flip; 32 carried
inputs green against silva genera; syntaxis expression inputs
produce real trees; every fixture roundtrips; mensura recorded;
amalgam gates green; M2a RELATIO + **the full phase-log audit
owed from M2.0** (doc-only phase, deferred by note).

### M2a Chunk A — scala praecedentiae: COMPLETE (2026-07-04)

**Suite 27/27 (+probatio_silva_c89, 190 assertions first run);
census 0 PINNED twice** (probatio_silva_c89 + a new c89
staleness guard in probatio_silva_tabulae); tessera 5/5,
saltuarius 13/13; amalgam byte-identical (c89 tables enter the
manifest at Chunk C together with their silva.h surface).

Shipped: **grammatica/c89.stml** (49 terminals / 77 productions
/ 129 states; the full ladder expressio→primarium in the DECISUS
vocabulary; rule-name bonus: vel-bitorum (|) / aut-bitorum (^) —
classical vel=inclusive, aut=exclusive, the language itself
distinguishes the operators); baked fontes/silva_tabulae_c89
committed; generare.sh grew the c89 line. **folium-chorda
decision landed with ZERO format changes**: hidden modus-lista
rule collects adjacent STRING_LITs → one folium-chorda, tok_valor
= lista-token (fixture pins "a" "b" → one node, lista of 2,
byte-exact roundtrip with interior trivia). Separated-list
interleave pinned: f(a,b) argumenta = [folium, COMMA-token,
folium]. All 31 fixtures roundtrip byte-exact THROUGH THE TREE
incl. the ERROR segment ("int x;" — M2a honesty), comments, and
multi-statement inputs. Dev printer instrumenta/principalia/
arbor.c + silva/arbor.sh (generic registrum walk — no grammar
knowledge in the printer; the M2a manual bar's eyes).

**COMPLEXITY (the big one)** — discovered-while: first c89
generation reported 85 preserved conflicts where the INTENTIO
pinned zero. consists-in: silva_gen production-text reading
tokenized on ' ' ONLY while stml_textus_internus preserves '\n'
— a two-line production fused the last atom of line 1 with the
first atom of line 2 ("expressio@verum\nCOLON@tok_colon" = ONE
atom), so the symbol after the break VANISHED SILENTLY and the
locus name was corrupted; ternarius lost its COLON, subscriptio/
vocatio lost their closers; the mangled grammar was genuinely
ambiguous (85 real conflicts of a wrong grammar, not spurious
lookaheads — the LALR machinery was NEVER at fault; bison on the
identical ladder: 0 conflicts, the independent oracle that
localized the bug to our reader). consequences: every multi-line
production in ANY grammar silently loses structure; sceletum
never fired it (all productions single-line); lapifex likely
carried the same class invisibly — under GLR, wrong-grammar
forks die on the next token, so tests pass and only wasted work
remains (spurious-conflict class = performance bug wearing a
correctness bug's clothes). handled-by: (1) _albispatia_normare
— production text whitespace-normalized (copy, not in-place;
stml text may be doc-owned) before fissio; (2) _atomi_pars_sana
— symbol/locus atoms restricted to [A-Za-z0-9_-], anything else
CLAMAT (generator-is-schema R4: this class can never be silent
again); (3) two regression pins in probatio_silva_generare
(multi-line production parses 3 symbols; '@' inside locus =
legere NIHIL).

Minor: generare.sh had rotted since the amalgamator-mechanism
refactor (silva_amalgama.o in the instrumenta glob needs fontes
symbols the generator link never provided) — skip-line added;
first table regeneration since the refactor confirmed sceletum/
imparilis tables byte-identical.

Debug artifact worth keeping in mind: scratchpad lalr_vestigium.c
(items+lookaheads dumper per state; state-dump mode at arg
1000+N) — rebuild it if a conflict census ever surprises again.

NEXT: Chunk B — typus intrat (species-typi minimal, conversio,
magnitudo ×2, the named fork family, census re-pinned to the
enumerated set, both-readings fixtures + recanonicare flips).

### M2a Chunk B — typus intrat: COMPLETE (2026-07-04)

**Suite 27/27 (probatio_silva_c89 now 304 assertions); amalgam
gates green; saltuarius 13/13, tessera 5/5. THE FORK FAMILY
LIVES**: grammar +10 terminals (9 primitive keywords + SIZEOF) /
+22 productions (99 total, 157 states) — conversio tier spliced
between multiplicativum and unarium (unary-op operands became
conversio per ISO 3.3.3; crementa stay unarium); species-typi
{specificatores lista, declarator?}; typus-primitivus (multi-
keyword tok_verba lista — "unsigned long" is ONE node);
typus-nominatus; declarator-abstractus star-chains; magnitudo-
expressionis + magnitudo-typi (two genera BY M2.0 DESIGN — fork
interpretations queryable apart).

**Census: EXACTLY the named family, pinned exhaustively** — the
predicted shape held: the ENTIRE typedef fork family (cast-vs-
call, cast-vs-binary ±*&, sizeof-typus-vs-expressionis) lives in
ONE r/r point: typus-nominatus vs primarium-identificator after
IDENTIFICATOR inside parens, lookaheads STAR + PAREN_CLAUSA = 2
cells. probatio scans EVERY cell of the baked table: any third
cell or different production pair = loud failure. Fixtures prove
both readings ((foo)(x) → AMBIGUUS{vocatio, conversio};
(foo)+x/-x/*x/&x ambiguous; (foo)~x/!x unambiguous conversio —
GLR prunes the dead expression reading; sizeof(foo*) unambiguous
magnitudo-typi), oracle collapse in-pipeline ((foo)(x) + oracle
→ CONVERSIO, resolutiones 1), and recanonicare flips BOTH
directions with the wrapper intact.

**COMPLEXITY — canonical spine: engine order is NOT policy.**
discovered-while: pinning "expression reading canonical" per the
INTENTIO — held free for (foo)(x) (vocatio canonical) but
sizeof(foo) natively lands TYPE-canonical (drain order differs
per fork shape; it's an accident, not a rule). consists-in: the
fabrica orders interpretations by GSS drain arrival; nothing
semantic. consequences: any consumer reading canonica without
policy enforcement gets arbitrary (per-shape-stable) readings.
handled-by: the sanctioned mechanism exists and is DEMONSTRATED
in-probatio — post-commit silva_recanonicare with a policy
resolutor (victor = the non-type reading) flips canonica while
keeping both interpretations alive (no collapse, wrapper stays).
SHIPPING LOCATION for the policy call = **M2b INTENTIO item**
(the driver/oracle integration owns "who calls it when") —
named landing spot, not an assumption. Note: recanonicare
requires a non-NIHIL oraculum even when the resolutor ignores
it (empty oraculum suffices).

**Minor complexity**: arbor.sh linked its binary only when
arbor.c changed — regenerated TABLE OBJECTS never triggered a
relink, so arbor silently ran Chunk A tables ((int)x "errored";
30 minutes of engine-suspicion for a build-script bug; the
python table-walk simulation exonerated tables AND engine before
the stale link was found). Fix: always relink. Lesson recorded:
dev-tool scripts must relink on ANY object change — cheap
insurance against debugging phantoms.

**Named narrowing**: const/volatile qualifiers excluded from
Chunk B species-typi — (const char*)x is ERROR until **M2b**,
which owns order-free specifier lists (arbor2 specifiers_ordine
lesson) and extends specificatores properly. Path back: the
specificatores locus is ALREADY a lista.

NEXT: Chunk C — the bars (lapifex expression suite 25/32
re-asserted against silva genera; v1 syntaxis expression half;
incolumitas + mensura incl. frontier-width baselines; silva.h
public surface for GRAMMATICA_C89 + hospes call + amalgam
manifest entry; M2a RELATIO + the owed full-log audit).

---

## M2a — EXPRESSIONES: RELATIO (2026-07-04) — PHASE COMPLETE

**Suite 27/27 (probatio_silva_c89: 469 assertions); amalgam
gates green incl. hospes 20/20 (c89 IN the deliverable —
silva.c 838,827 octeti); saltuarius 13/13, tessera 5/5. Every
exit criterion of the INTENTIO discharged or honestly
re-dispositioned:**

- **Census**: Chunk A == 0, Chunk B == the named family (2 r/r
  cells, typus-nominatus × primarium-identificator, STAR +
  PAREN_CLAUSA), pinned by EXHAUSTIVE cell scan + the tabulae
  staleness guard.
- **Fork fixtures**: both readings proven for (foo)(x) and
  magnitudo(foo); oracle collapse in-pipeline; recanonicare
  flips BOTH directions, wrapper intact.
- **Carried bar**: all 32 lapifex expression inputs re-asserted
  against DECISUS genera (root genus + zero errors + zero
  ambigui each).
- **Syntaxis corpus DISCOVERY**: the "expression half" is
  EMPTY — the corpus is declarations wall-to-wall (expressions
  live inside initializers). M2a's expression bar is carried
  entirely by the lapifex suite; the corpus block now decodes
  the quoted lines properly (\" \\), asserts totality 125/125,
  and pins comprehension at its honest baseline ZERO — the
  counter M2b/M2c must raise deliberately (M2d's metric in
  miniature, running early).
- **Fuzz**: incolumitas runs every specimen through BOTH
  grammars (400 × 2), byte-exact roundtrip under recovery.
- **Mensura baselines (fork-explosion watch, first data)**:
  c89 over the 78-file real-C corpus: 0.5513 ms/KB parse,
  **frons_maxima 2** (no explosion), 761 ERROR nodes (sceletum:
  797 — c89 already comprehends 36 more segments), fideles
  78/78 byte-exact through trees (asserted).
- **Public surface**: silva.h + SILVA_C89_TABULA/REGISTRUM +
  construere/fabricare; manifest grew both table files; hospes
  exercises the fork THROUGH THE AMALGAM (type-drift guard).

**FULL PHASE-LOG AUDIT (owed from M2.0, executed)**: all
complexities resolved or parked WITH NAMED LANDING SPOTS. Two
M1-era dispositions REVISED (drift caught, the class this
practice exists for):
1. Phase-6 note said "M2 swaps sceletum out of the manifest" —
   M2a ADDED c89 alongside instead (sceletum's public surface
   still has consumers). The swap-out decision moves to **M2d**
   (when saltuarius switches grammars; sceletum stays the
   generator fixture regardless — whether it keeps SHIPPING is
   the M2d call).
2. "Generated per-genus accessors + genus enum in the public
   surface (M2)" — M2a shipped the BUNDLE surface only; genus
   names are already publicly reachable via REGISTRUM titulus
   strings (kebab-case tags). Full accessor/enum exposure stays
   parked **on-pull** (saltuarius M2d FUNCTIONES or solarium —
   whoever asks first).
Also verified: M2.0 deferrals all landed (folium-chorda decided
Chunk A — genera-c89.md note updated to DECIDED; genera-extra
superset carried verbatim + generation-enforced; grouped-casus
→ M2c on record); canonical-policy shipping → M2b INTENTIO;
nested-packing observation → M2b watchdog evidence; qualifiers
→ M2b. Vendored-exclusion note N/A (no new consumption). Zero
unowned complexities.

**M2a's proof, delivered**: annotation v0 + AMBIGUUS machinery
at real-grammar width — 99 productions, one designed fork
family, all of C89's expression grammar parsing byte-exact
through real trees with named, queryable genera.

NEXT: **M2b INTENTIO — declarationes + typi** (~45 rules: the
oracle dance at scale, order-free specifiers + qualifiers,
declarator recursion incl. function pointers, initializers,
struct/union/enum + campus bitfields; owns: canonical-policy
shipping location, watchdog ceilings from the Chunk C
baselines, syntaxis-corpus comprehension riser, TYPI/
DECLARATIONES saltuarius vista per the loop map).

---

## M2b — DECLARATIONES + TYPI: INTENTIO (2026-07-04)

**What M2b proves**: the M1 fork case was the GENERAL mechanism,
not a demo — the oracle dance at scale. Typedef declarations
register at segment commit and feed subsequent segments;
within-segment unknowns stay localized AMBIGUUS (both readings
retained — the wildcard-query pin WANTS them); the watchdog
counters get measured ceilings. This is the largest and
riskiest M2 chunk: ~45 rules + the first grammar-SPECIFIC
hand-written module + a driver API addition.

**Inherited by name from M2a**: canonical-policy shipping
location; watchdog ceilings (baseline frons_maxima 2);
syntaxis-corpus comprehension riser (baseline 0);
TYPI/DECLARATIONES vista (the loop map — the vista IS M2b's
API-pressure test); const/volatile qualifiers; nested-packing
observation (watchdog evidence); M1-era three-valued-oracle
paths.

**Verified going in**: the segment slicer counts PAREN depth
ONLY (silva_parsare.c:209-222) — `struct S { int x; int y; };`
would slice MID-BODY today. Brace depth-counting is a named
driver change (Chunk B). Existing suites unaffected (sceletum
inputs braceless; corpus assertions pin fidelity/ratios, not
segment counts — byte-exactness holds under any slicing).

**Chunks**:

- **0 — SIMULATIO X (the oracle dance, pretend-implemented
  against shipped code).** The culture rule: simulate contested
  designs before building; sim ledger 5-for-5. Contested here:
  (a) the driver hook — registration must run BETWEEN segments
  (segment N's typedefs shape segment N+1's parse), before
  commit (pater unset; genus+slots readable — verify); proposed
  shape: an optional per-segment callback on the parse API
  (grammar-agnostic driver, c89-specific hook), exact signature
  from the sim. (b) AMBIGUOUS-typedef registration policy:
  `typedef foo bar;` with foo unknown — the DECLARATION itself
  packs; register `bar` anyway (it names a type under EVERY
  interpretation that is a declaration) or hold? What when the
  canonical reading is the expression one? (c) the policy-
  resolutor interplay (collapse-at-commit vs flip-only
  recanonicare vs registration — three passes, one owner).
  Deliverable: sim register + decisions, interview if needed.
- **A — declaratio core (~20 rules).** Order-free
  specificatores: ONE flat lista rule over specifier atoms
  (storage classes TYPEDEF/EXTERN/STATIC/AUTO/REGISTER,
  qualifiers CONST/VOLATILE, typus atoms) — validity (one
  storage class max, etc.) is the LINT layer's job, never the
  grammar's (arbor2 specifiers_ordine lesson: hand-ordered
  categories = combinatorial explosion). Declarator recursion:
  monstrator (w/ qualificatores lista), titulus, aciei
  (mensura?), functionis (parametra — ANSI only, K&R = M2c),
  parenthesized (int (*f)(void) — the function-pointer
  adversarial fixture from spec §7), abstract forms extended to
  match. declaratores = separated lista of init-declarators
  (= initiator, expression form; congeries = Chunk B).
  elementum grows declaratio — THE SCELETUM FORK ARRIVES AT
  STATEMENT LEVEL (`foo * bar;`), plus the specifier fork
  (`foo bar;` — both typedef-name-able). Census RE-ENUMERATED
  exhaustively — the family grows; every new cell named or the
  chunk stops.
- **B — aggregates (~15 rules).** structura/unio (tok_titulus?,
  membra; membrum = specificatores + declaratores + terminator;
  campus bitfields per vocabulary), enumeratio (enumeratores,
  valor? = ternarius-tier const expr), congeries (nested, in
  initiator position), species-typi extended (structura/unio/
  enumeratio atoms — `(struct S*)p` casts). DRIVER: brace
  depth-counting in the slicer (verified need, above).
  Annotation-friction watch: struct members, enumerator lists,
  initializer nesting are the sketch's NAMED candidates —
  first friction revises the FORMAT consciously, never
  per-rule hacks.
- **C — the oracle dance.** New hand-written module (the first
  grammar-specific one; name at review — proposal
  fontes/silva_c89_oraculum.{h,c}): the REAL resolutor
  (replacing probatio hand copies), the segment-commit
  registration walk (typedef detection = TYPEDEF token in
  specificatores, per the M2.0 semantic-mark decision), the
  canonical-policy pass (expression-canonical for surviving
  ambigui — the M2a mechanism, now shipped). Driver hook from
  Simulatio X. Fixtures: typedef-feeds-next-segment
  (`typedef int foo; foo x;` → BOTH clean), unknown stays
  AMBIGUUS, ambiguous-typedef per sim policy, redefinition/
  shadowing = whatever C89 scoping demands DEFERRED BY NAME
  (block scope = M2c compound statements; M2b oracle is
  file-scope flat).
- **D — the bars.** Carried lapifex decl suite
  (probationes/probatio_lapifex_c89_decl.c — inventory at chunk
  start) re-asserted against DECISUS genera; syntaxis-corpus
  comprehension riser RE-PINNED (baseline 0 → observe; the
  corpus is wall-to-wall declarations, so this is M2b's
  headline number); mensura corpus re-baselined + WATCHDOG
  CEILINGS pinned (frons_maxima / fusiones asserted under
  measured bounds over solarium + roundtrip corpus — the
  fork-explosion risk retired with numbers, or escalated with
  numbers); SilvaDeclaratioVista in silva.h (genus + titulus +
  extent + linea, mirrors the additiones-II vista pattern) +
  hospes calls same-change; every fixture roundtrips; RELATIO
  + full-log audit.
- **E (conditional, per the loop rule) — saltuarius TYPI/
  DECLARATIONES TOC section.** silva.h now carries BOTH
  bundles, so saltuarius CAN drive the vista without switching
  its main parse. Lands in-phase ONLY if it is the best
  instrument for the D-chunk manual bar (Tab on a real header
  listing its declarations = the bar); otherwise named to the
  saltuarius backlog. Decide at D, with Fran.

**Vocabulary addenda needing Fran's eye (genera-c89.md is
DECISUS; two small gaps found at INTENTIO time)**:
1. Parenthesized declarator `(*f)` — no genus named. PROPOSAL:
   reuse `parenthesis` (identical loci shape {tok_apertum,
   internum, tok_clausum}; S20 unification across expression
   and declarator use; "find parenthesized things" queries
   uniformly). Alternative: declarator-clausus.
2. Specifier atoms in the specificatores lista: bare TOKENS
   for storage classes/qualifiers (interleaved with typus
   NODES, lista-mixta) — no wrapper nodes. Queries scan
   tokens; consistent with tok_verba precedent.

**Manual bar**: `./silva/arbor.sh -f` on a real rhubarb header
shows correct declaration trees; the typedef dance visible
(same file, names resolving as the segments advance).

**Named risks**: fork explosion on typedef-dense real headers
(THE M2b risk — watchdog ceilings are the discharge, measured
not guessed; the Phase-4 action-filter pruning hook is the
named evidence-gated remedy); annotation-format friction
(three named candidates); the declarator recursion's grammar
conflicts beyond the known family (any unnamed cell = stop);
nested-packing inflation under declaration forks (M2a
observation — watch the ambigui counts).

**Exit criteria**: suite green; census = enumerated family
(re-pinned); oracle dance proven across segments; watchdog
ceilings asserted; comprehension riser pinned at its new
number; vista + hospes; carried decl bar green; roundtrip
everything; RELATIO + audit.

### SIMULATIO X — COMPLETE (2026-07-04); register:
### project-specs/silva-simulatio-10.md

Sim ledger 6-for-6. Reversed the INTENTIO's central mechanism
(no GLR-time oracle exists → per-segment registration
unnecessary; ONE pre-commit hook suffices — X1); found the
positional-visibility hole (flat oracle wrongly resolves
typedef-after-use — the oracle must carry declaration offsets —
X3); decided ambiguous-typedef policy (conservative-exclusive —
X4); the registration walk IS the vista walk (X5); scoring
resolutor design (X6); wrapper owns the three-pass sequence and
ANSWERS the canonical-policy shipping question (X8). Post-sim
verify caught the register's own error: -Wextra fails 4-of-5
bundle initializers → all bundle sites gain explicit NIHIL
(compiler enumerates them). Fran approved all four
recommendations (bundle hook / positional oracle / exclusive /
silva_c89_oraculum). POST-SIM ADDENDUM (found at Chunk A): X10 —
the resolutor needs a COMBINATION-VALIDITY filter (C89
type-specifier combination rules: int+nominatus impossible,
two nominatus impossible) to kill semantically-impossible
readings deterministically before oracle scoring; without it
every `int f(foo);` keeps a phantom "declarator (foo) of type
int-f" reading forever. Chunk C owns it.

### M2b Chunk A — declaratio core: COMPLETE (2026-07-04)

**Suite 27/27 (probatio_silva_c89: 556 assertions); amalgam
gates + hospes 20/20; saltuarius/tessera green.** Grammar: +8
terminals (storage classes, qualifiers, ELLIPSIS) / +55
productions (154 total, 238 states): declaratio (specificatores
ordine libero + declaratores separata + initiatores expressione),
the FULL declarator recursion (monstrator cum qualificatoribus,
titulus, aciei ± mensura, functionis cum parametris + ELLIPSIS,
parenthesis REUSED for (*f) per the vocabulary addendum),
abstract declarators extended (aciei/functionis genera reused,
internum absent), species-typi upgraded to true spec-qual lists
(qualifiers in casts work), elementum grew declaratio — THE
SCELETUM FORK NOW LIVES AT STATEMENT LEVEL.

**Design that paid**: the TWO-STATE specifier lista (after a
primitive-run atom, another primitive-run cannot follow) makes
maximal-run grouping GRAMMATICAL — `unsigned long` = one
typus-primitivus (verba II) with ZERO conflicts and zero forks;
a qualifier splits runs (`unsigned const long` = three atoms,
vocabulary note recorded). declaratio-NUDA deferred to Chunk B
BY NAME — its absence keeps `int x;` and `unsigned foo x;`
runtime-unambiguous (the phantom all-specifier readings need
nuda to complete, and die in GLR without it).

**Census: NINE cells, THREE named families, exhaustively
pinned** (any unknown cell = loud failure): F1 ×4 =
typus-nominatus vs primarium-identificator (the M2a family
widened to declaration/parameter contexts); F2 ×3 =
typus-nominatus vs declarator-titulus (the ISO parameter rule
as a GLR fork); F3 ×2 = shift(IDENT-as-next-specifier) vs
reduce(specificatores-decl complete) — the specifier/declarator
boundary. Runtime behavior: plain declarations (incl. `static
const foo x;`, function pointers, arrays) parse CLEAN with zero
ambigui; `foo * bar;` = the designed statement fork (declaratio
vs sententia-expressionis, both retained); `f(a)[3];` = genuine
C double-reading, retained; `int f(foo);` = genuine double-
reading pending the X10 filter.

**COMPLEXITY (generator)** — discovered-while: first suite run
segfaulted after an S32 refusal ("signum valoris LISTA speciei
TOKEN non congruit"). consists-in: _est_lista_valens checked
DIRECT modus productions only — specificatores-decl reaches its
lista-ness through two pass-through hops (specificatores-decl →
sd-* → modus), so the registrum computed species NODUS for a
lista-carrying slot; construere's S32 check refused the store
and the GLR branch DIED SILENTLY (construere-nihil = dead
branch). consequences: worse than the crash — pre-fix trees
showed FALSE-CLEAN ambiguity counts (`int f(foo)` reported
ambigui 0 because the second legitimate reading's construction
was being killed); the S32 runtime check is what surfaced it —
the debug-assert design from spec §9.4 earning its keep.
handled-by: _est_lista_valens made TRANSITIVE through 1-symbol
pass-through productions (depth-capped against cycles); fix
regenerated all tables (sceletum/imparilis byte-identical);
the resurrected reading is now PINNED as a fixture with the
full story in its comment.

**The headline numbers**: syntaxis-corpus comprehension
0 → 69/125 lines clean (pinned; Chunk B aggregates + M2c
functions own the rest); solarium-corpus ERROR nodes 761 → 436;
**frons_maxima STILL 2** over the whole corpus — the fork-
explosion risk is so far a no-show even at declaration width
(ceilings formally pinned at Chunk D); parse 0.91 ms/KB
(bigger tables, still fast); 78/78 byte-exact through trees.

NEXT: Chunk B — aggregates (structura/unio + membrum/campus,
enumeratio, congeries, declaratio-nuda arrives WITH the
combination-validity context, species-typi struct atoms,
slicer brace-depth).

### M2b Chunk B — aggregata: COMPLETE (2026-07-04)

**Suite 27/27 (probatio_silva_c89: 635 assertions); amalgam +
hospes 20/20; saltuarius/tessera green.** Grammar: +5 terminals
(STRUCT/UNION/ENUM/BRACE ×2) / +43 productions (197 total, 305
states): structura/unio/enumeratio (P1 latina-keyword genera;
plena/anonyma/nominata forms), membrum (spec-qual specifiers —
the SAME sq lists as casts, zero duplication), campus bitfields
(± declarator), enumeratores (± valor), congeries (nested;
C89 trailing comma absorbed INTO the elementa lista as an
interleaved token — no extra locus, byte order preserved,
slot-order friction avoided). species-typi gets tags for free
via the shared atom rule ((struct S*)p, sizeof(enum E)).
DRIVER: the slicer counts BRACE depth (verified need — struct
bodies were being sliced mid-body).

**Design that paid — declaratio-nuda WITHOUT the ambiguity
plague**: ISO's constraint "a declaration without declarators
must declare a tag" made STRUCTURAL: the specifier-list
automaton grew a third state (post-tag), and nuda accepts ONLY
tag-final lists. `int x;` stays fork-free BY CONSTRUCTION (the
phantom [int, nominatus(x)] reading cannot complete), while
`struct S { int x; };` parses clean. Named divergence: `struct
S {...} const;` (tag then qualifier, nuda) is legal-C89 but
ERROR here — real-world frequency ~0, revisit on corpus
evidence.

**Census: 14 cells, same THREE families** (re-pinned
exhaustively): F1 ×4 and F2 ×3 unchanged; F3 (specifier/
declarator boundary) widened 2→7 (three list states × two list
flavors — the sq-flavor cells are the MEMBRUM boundary,
"struct S { foo bar; }").

**Honest finding — the named bitfield forks**: `unsigned a:3;`
inside a struct is genuinely ambiguous ([T]+campus(a) vs
[T, nominatus(a)]+anonymous-campus) — reading 2 is combination-
invalid, X10's Chunk-C kill; the fixture retains the wrapper
honestly and asserts the named-campus reading present. The
anonymous form `int : 2;` is unique. (Fixture initially
segfaulted by navigating as if unambiguous — lldb + the
stderr-vs-stdout lesson; no engine issue.)

**The numbers**: syntaxis comprehension 69 → **87/125**;
roundtrip-corpus ERROR nodes 436 → **62** (real headers ARE
declarations; brace-slicing + aggregates ate the rest);
frons_maxima STILL 2; 78/78 byte-exact; parse 0.82 ms/KB.

NEXT: Chunk C — the oracle dance (silva_c89_oraculum module:
Simulatio X design — bundle praecommissio hook + positional
oracle + X6 scoring resolutor + X10 combination filter + policy
pass + wrapper; bundle initializers gain explicit NIHIL).

### M2b Chunk C — saltatio oraculi: COMPLETE (2026-07-04)

**THE ORACLE DANCE WORKS, FIRST RUN. Suite 27/27
(probatio_silva_c89: 661 assertions — the entire dance block
green on its first execution); amalgam + hospes 21/21;
saltuarius/tessera green.** The Simulatio X design shipped
intact:

- **Driver** (X1/X2): SilvaGrammatica grew the optional
  praecommissio member (~6-line driver change, called between
  weave and commit on the uncommitted radix); all 14 bundle
  initializer sites gained explicit NIHIL — the compiler
  enumerated every one, exactly as the sim's post-verify
  predicted (including one the script missed in hospes, caught
  by the amalgam gate).
- **Positional oracle** (X3): oraculum entries carry the
  declaring token's byte_offset (the hash-table value slot,
  previously a dummy pointer); silva_oraculum_typum_addere_situ
  + situs_typi; preloaded types = situs 0; first situs wins on
  redeclaration. PINNED: `foo * bar; typedef int foo;` stays
  honestly ambiguous — typedef-after-use resolves nothing.
- **The module** (fontes/silva_c89_oraculum.{h,c}, the first
  grammar-specific hand-written code): declaratoris_titulus
  (the X5 walker — monstrator/aciei/functionis/parenthesis/
  campus chase, shared with the future vista), typos_registrare
  (typedef detection = TYPEDEF token per the M2.0 semantic
  mark; descends taken conditional arms; skips AMBIGUUS
  elements per X4), the REAL resolutor (X6+X10: combination
  filter hard-kills impossible readings [nominatus+type,
  tag+primitivus, tag+tag; multi-primitivus stays legal for
  qualifier-split runs] → sole survivor wins → else unique
  type-positive-at-position wins → else retention), the policy
  pass (unique pure-expression reading → canonical; same-kind
  ambigua keep engine order, documented), and silva_c89_parsare
  — ONE call owning the whole sequence.
- **Proven in fixtures**: the dance (`typedef int foo; foo *
  bar;` → zero ambigui, one resolution); X3 retention; X10
  hard-kills WITHOUT any oracle (int f(foo) → functionis;
  named bitfields collapse clean); policy through the wrapper
  ((foo)(x) retained, vocatio-canonical); dance through
  conditional arms; ERROR-segment invisibility (X9);
  multi-declarator registration; and `typedef int f; f(a)[3];`
  collapsing to a DECLARATION (array a of three f — C is
  wonderful).
- **arbor.c** switched to the wrapper — the manual bar:
  `typedef int foo; foo * bar; (foo)(x);` prints THREE clean
  trees, zero ambigui.
- **silva.h**: praecommissio mirror + positional-oracle API +
  the c89 semantic section (SILVA_C89_GRAMMATICA,
  silva_c89_parsare, resolutor, typos_registrare,
  declaratoris_titulus, politicam_imponere); manifest grew the
  module; hospes exercises the dance + situs semantics + the
  walker THROUGH THE AMALGAM.

Small frictions (all compile-caught, none silent): typedef
redefinition of SilvaPraecommissio in the amalgam TU → CADENDA
entry (silva.h owns it); i32-is-UNSIGNED mistranslated as int
in one silva.h decl (the standing latina-type-map lesson);
-Wcast-qual on a hospes string literal; a statement-before-
declaration C89 slip from a scripted edit.

NEXT: Chunk D — the bars (carried lapifex decl suite;
comprehension riser re-pinned WITH the dance — corpus lines
through silva_c89_parsare; watchdog ceilings pinned;
SilvaDeclaratioVista in silva.h + hospes; roundtrip; M2b
RELATIO + audit; Chunk E saltuarius decision with Fran).

### M2b Chunk D — the bars: COMPLETE (2026-07-04)

**Suite 27/27 (probatio_silva_c89: 826 assertions); hospes
22/22; saltuarius/tessera green.** Carried lapifex DECL bar: all
30 inputs re-asserted against DECISUS genera through the
CONSUMER path (silva_c89_parsare — dance active), every one
clean and unambiguous ("2+3" gains its terminator per the M2a
convention; "unsigned long long" parses — combination legality
is lint's). Syntaxis harness switched to the wrapper
(comprehension stays 87/125 — single-line inputs don't need
cross-segment typedefs; the remainder is M2c functions/
statements + gcc extensions). **WATCHDOG CEILING PINNED**:
frons_maxima <= VIII asserted over the corpus (observed: 2,
unchanged since M2a — the fork-explosion risk is formally
retired at declaration scale; remedy stays named if the
solarium sweep at M2d ever disagrees). **SilvaDeclaratioVista
SHIPPED** (module + silva.h + CADENDA + hospes): one row per
declarator, nuda rows titled by tag, taken arms visited,
ambigua walked through the CANONICAL spine — the fixture's
money shot: `foo * bar;` appears as a REAL "bar" row because
the dance collapsed it to a declaration first. The X5
prediction held: the vista walk is the registration walk's
sibling, built on the same declaratoris_titulus chase.

## M2b — DECLARATIONES + TYPI: RELATIO (2026-07-04) — COMPLETE

**Every INTENTIO exit criterion met**: census re-pinned (14
cells, 3 named families, exhaustive scan); oracle dance proven
across segments (in-fixture, in-hospes, in-arbor); watchdog
ceiling asserted; comprehension riser 0 → 69 → 87/125 pinned at
each step; vista + hospes same-change; carried bar green;
every fixture byte-exact through trees. The corpus headline:
**roundtrip-corpus ERROR nodes 761 → 62** across M2b; frontier
never left 2.

**FULL PHASE-LOG AUDIT (M2b boundary)**: all Simulatio X items
shipped or fixture-pinned (X1-X10 — the ledger's first 10-for-10
sim); all four M2a handoffs discharged (policy→wrapper,
ceilings→pinned, riser→pinned, vista→shipped); Chunk narrowings
all named with owners (nuda tag-final divergence — corpus
evidence owns reopening; X4's unreachable-fixture note;
qualifier-split runs in the vocabulary doc). Block-scope oracle
EXPLICITLY deferred to M2c/M2d (file-scope flat, named in
Chunk C). M1-era parks unchanged (pasta fail-loud; accessors
on-pull; sceletum manifest swap-out = M2d). The M2a
nested-packing observation: watchdog numbers show NO inflation
at declaration scale — stays a named observation, next
checkpoint = M2d solarium sweep. Zero unowned complexities.

**Two generator bugs found and killed this milestone** (both
would have corrupted every future grammar silently): multi-line
production atom loss (M2a) and non-transitive lista-valens
species (M2b — the one that silently killed GLR branches and
faked clean ambiguity counts). The S32 runtime check and the
census discipline caught what nothing else would have.

**OPEN FOR FRAN — Chunk E (the loop rule)**: the
TYPI/DECLARATIONES saltuarius TOC section. The vista API is
shipped and hospes-proven; silva.h carries both grammar
bundles, so saltuarius can drive it without switching its main
parse. Per the map this lands in-phase only if it is the best
instrument for a manual bar — M2b's bars are all discharged
without it, so the honest options are: (a) build it NOW as a
v0.1 feature (small: fourth TOC section calling the vista),
(b) name it to the saltuarius backlog and let M2c's FUNCTIONES
section (the original pull) bring both sections at once.

NEXT (after Fran's Chunk E call): **M2c INTENTIO — sententiae
+ functiones** (~30 rules: compound blocks, control flow with
grouped-switch mechanics and dangling-else policy, GOTO/labels,
function DEFINITIONS incl. K&R, the ()-before-{ boundary
fixtures; owns block-scope oracle question, comprehension
riser, FUNCTIONES TOC).

### Addendum praecompactionis (2026-07-04)

**CHUNK E DECIDED (Fran, by motion)**: proceed straight to M2c —
the saltuarius TOC integration lands ONCE at M2c, delivering
TYPI/DECLARATIONES + FUNCTIONES together (option b of the M2b
RELATIO). The (a) path stays open at zero cost: the vista API is
shipped and `./silva/arbor.sh -d <via>` demos it from the
terminal today (the -d vista mode was added POST-COMMIT while
answering "what would the rows look like" — silva.h on itself:
116 rows, 0 errores; a .c file shows only prototypes/globals
until M2c). ONE UNCOMMITTED FILE: instrumenta/principalia/
arbor.c (+arbor.sh unchanged) — the -d flag; commit with M2c or
alone.

**Re-entry for M2c (post-compaction, follow silva/CLAUDE.md
PROTOCOLLUM)**: read this log's M2b sections + the M2 sketch
(M2c paragraph) + spec-v2 §7 (boundary-finder risk). The M2c
INTENTIO is NOT yet written — it is the next act. Known scope:
statements (corpus w/ decl-before-stmt, si/alioquin w/
dangling-else DECLARED preference [generator preference
mechanism NOT built yet — M2c owns building it or forking the
conflict per policy], dum/fac-dum/per+per-clausula/passus,
commutatio w/ GROUPED casus per the M2.0 sign-off [mechanics
owed], titulatum/salta/frange/perge/redde, sententia-vacua) +
function definitions (ANSI + K&R; translation-unit; the
()-before-{ segment-boundary heuristic in the DRIVER — function
definitions have NO trailing semicolon, the slicer must end
segments at body-brace-close; adversarial fixtures named in
spec §7) + block-scope oracle question (file-scope-flat today —
decide or defer BY NAME) + carried bars probatio_lapifex_c89_
sent.c/_func.c + full v1 syntaxis harvest (87→toward 125) +
FUNCTIONES vista/TOC (row per definitio-functionis; then the
saltuarius structura pane grows BOTH sections). Corpus ERROR 62
→ expect near-0. Census will grow — enumerate exhaustively as
always; dangling-else is the named newcomer.

**Artes implementationis (M2a/M2b learnings not yet written
anywhere else — for the M2c sessions):**

1. **DESIGN HINT for the owed grouped-casus mechanics**: the
   problem ("the case group's statement list must not swallow
   the next casus/ordinarius label") is the SAME CLASS as the
   tristate specifier lista — a by-construction CFG state-split
   (the group's statement tier simply excludes case-labeled
   forms, so FOLLOW does the exclusion and no conflict/pruning
   is needed). M2b proved the technique twice (greedy primitive
   runs; tag-final nuda). Reach for it FIRST at M2c before
   inventing preference machinery for this one — the DECLARED-
   preference mechanism is still needed for dangling-else, but
   grouped-casus likely needs none.
2. **Missing-fork diagnostic rule**: if a fork you EXPECT does
   not appear (ambigui lower than designed), suspect
   CONSTRUCTION failure before grammar — a construere that
   returns nihil kills its GLR branch silently and understates
   ambiguity (the lista-valens bug's signature). First check:
   stderr for the S32 refusal line; second: the python
   baked-table walk.
3. **The python baked-table walk** (scratchpad technique, used
   twice to decisive effect): simulate shift/reduce directly
   against the generated silva_tabulae_c89.c arrays for a token
   sequence — it cleanly separates "tables wrong" from "engine
   wrong" from "build stale". Rebuild it freely; ~40 lines.
   Sibling: bison on an equivalent grammar as an independent
   LALR oracle (settled the 85-conflict question in minutes).
4. **Fixture lengths: ALWAYS strlen, never hand-counted
   literals** — Roman-numeral length constants bit twice
   (truncated input = confusing downstream failures, not
   loud ones).
5. **Scripted bulk edits on C89**: compile IMMEDIATELY after
   (two slips this milestone: a statement inserted before
   declarations; placeholder scaffolding left in a block).
   The wall catches everything, but only if run before the
   next edit buries it.

## M2c — SENTENTIAE + FUNCTIONES: INTENTIO (2026-07-04)

The last real grammar chunk; after M2c, M2d is the acceptance
wall. Two halves: the statement tier (genera-c89.md §4, all
DECISUS) and function definitions (ANSI + K&R), plus the driver's
boundary-finder upgrade, the generator's first declared-preference
mechanism, and the saltuarius TOC integration promised at Chunk E.

**Grounding (verified this session)**: all 12 statement keywords
already exist as lexeme genera in the carried lexer (SILVA_LEX_IF/
ELSE/WHILE/DO/FOR/SWITCH/CASE/DEFAULT/GOTO/BREAK/CONTINUE/RETURN)
— the grammar adds 12 terminals, zero lexer work. c89.stml's
elementum rule carries its own "M2c: + sententiae ceterae"
comment. Carried bars on disk: probatio_lapifex_c89_sent.c (30
inputs), _func.c (20 inputs). Slicer = depth-0 SEMICOLON with
PAREN+BRACE depth (M2b).

**Grammar scope (~30 rules, 12 terminals)**: sententia tier =
corpus, sententia-vacua, si (ONE genus, alioquin locus NIHIL when
absent), dum, fac-dum, per + per-clausula (real node; initium/
conditio/passus all optional, two interleaved terminator tokens),
commutatio + casus/ordinarius (GROUPED per M2.0), titulatum,
salta/frange/perge/redde; sententia-expressionis already exists.
definitio-functionis = specificatores, declarator,
declarationes-kr (lista), corpus. elementum grows sententia +
definitio-functionis (top-level statements stay lenient —
tree-sitter bar; conformance is lint's).

**Design positions going in**:

1. **Grouped casus by state-split, NOT preference** (Artes #1):
   casus/ordinarius are not sententiae — they live only in the
   switch-body rule's lista, so a group's sententiae lista ends at
   the next label by FOLLOW alone. The switch-body rule is
   grammar-internal factoring emitting genus **corpus** (P2:
   factoring never leaks). Fall-through = empty sententiae lista.
   `case` outside switch = ERROR (lenient). C89 allows
   declarations before the first label inside a switch body —
   the switch-corpus keeps the declaration tier.

2. **Dangling-else = the declared-preference mechanism, built
   this phase.** Innermost-binding is a LANGUAGE rule, not a
   style choice — forking would retain a reading C89 itself
   forbids, so deterministic table-time resolution is CORRECT
   here (contrast every previous conflict, where both readings
   are real C). Grammar element proposal: `<praelatio>` naming
   the terminal and the kept action; the generator resolves ONLY
   matching cells and — census discipline extended — **praelata
   cells become a pinned census category**: enumerated and
   asserted like conflict cells, so declared resolutions can
   never silently multiply. Generator change → sceletum/imparilis
   byte-identical gate as always.

3. **The boundary-finder** (driver; spec §7's named risk).
   Function definitions have no trailing `;`. Per-segment state
   machine: depth-0 PAREN_CLAUSA sets post-clausa; depth-0
   ASSIGNATIO clears it (initializers: `int x = g(1) + 2;` stays
   semicolon-terminated). Depth-0 BRACE_APERTA while post-clausa
   → brace-terminated segment, ends AT matching close (inclusive,
   no terminator). K&R (`int f(a) int a; { }` — depth-0 `;`
   BEFORE the body brace): at a depth-0 `;` in post-clausa with
   tokens intervening since `)`, don't finalize — remember the
   candidate, scan forward through declaration-plausible tokens;
   `{` reached → K&R confirmed, extend to brace close; EOF or
   implausible token → CUT at the remembered candidate (fallback
   preserves `dum (x) y;` and prototypes, where `;` directly
   follows `)`). Adversarial fixtures (spec §7): function
   pointers, prototype-then-definition, K&R multi-param, `struct
   S { int x; } f(void) { }` (struct brace not post-clausa;
   function brace is), old-style implicit-int `f(a) int a; { }`.

4. **Block-scope oracle: DEFER BY NAME (proposal).** Registration
   walk stays top-level (file-scope declarations incl. taken
   arms). Block-local typedefs are NOT registered; their uses
   stay honestly AMBIGUUS-retained (nothing resolves WRONG — the
   failure modes of a flat scope [leak past the body] are
   avoided by not registering at all). Named divergence:
   block-scope typedef comprehension; evidence gate = count
   block-local typedefs over solarium + rhubarb at the M2d
   sweep; landing spot = M2d or the lint layer.

**OPEN QUESTION #1 — the corpus shape (DECISUS amendment
candidate, Fran's call).** The DECISUS table gives corpus TWO
listas (declarationes, sententiae), "mirrors the standard's
grammar." Grounding analysis says this shape has a structural
consequence at block level: `{ foo * bar; baz(); }` forks into
readings that live in DIFFERENT listas (declarationes=[decl] vs
sententiae=[expr, …]) — the GLR stacks cannot re-merge until the
whole corpus reduces, so the AMBIGUUS wrapper lands on the
ENTIRE BLOCK, and the M2b resolutor (which examines DECLARATIO
readings element-wise) sees only corpus-vs-corpus and retains.
Every real function body with a typedef'd local would stay a
block-level ambiguus — the comprehension riser dies. Two exits:
(a) **AMEND the DECISUS**: corpus carries ONE lista (elementa,
mirroring the segment level exactly); the fork stays
PER-ELEMENT, the ambiguus wraps one element, the entire
X-machinery works UNCHANGED; decl-before-stmt ordering becomes a
lint query; "declarations of this block" stays a trivial
genus-filtered vista/query. The standard's two-lista factoring
leaking into tree shape is precisely what P2 warns about.
RECOMMENDED. (b) keep two listas and grow the resolutor to
descend corpus ambigua element-wise — more machinery, same
information, tree shape bought at resolver complexity. If
contested → Sim XI before Chunk A (CULTURA rule).

**Chunks**:
- **A — sententiae.** Statement tier complete minus
  definitio-functionis; grouped commutatio; titulatum;
  `<praelatio>` mechanism + dangling-else resolved; corpus shape
  per Fran's call implemented; census re-pinned EXHAUSTIVELY
  (named newcomers expected: block-item F1 widening, titulatum
  IDENT-vs-expression cells, praelata category; any unnamed cell
  = stop); carried sent bar (30) through the consumer path;
  fixtures roundtrip; interim syntaxis riser pin.
- **B — definitio-functionis + boundary-finder.** ANSI + K&R
  grammar; slicer state machine + the adversarial fixture set;
  carried func bar (20); block-scope decision RECORDED;
  registration-walk confirmation fixture (top-level only);
  roundtrip-corpus ERROR 62 → expect near-0 (pin the number);
  mensura re-baseline; frons ceiling <=VIII re-asserted.
- **C — vista FUNCTIONES + the harvest.** Vista rows for
  definitio-functionis (titulus via the X5 declaratoris_titulus
  chase, reused; signature text exercises scribere-from-subtree
  per the sketch); silva.h + hospes same-change; **full v1
  syntaxis harvest completes** (125 inputs / 234 assertions
  re-asserted against DECISUS genera); comprehension riser FINAL
  pin (gcc-extension remainder counted and named).
- **D — saltuarius TOC (the original pull) + RELATIO.** Structura
  pane grows TYPI/DECLARATIONES + FUNCTIONES sections (the Chunk
  E promise: both at once, vista-driven, silva.h carries both
  bundles); saltuarius suite green; M2c RELATIO + FULL phase-log
  audit (phase boundary).

**Manual bar**: Tab in saltuarius on silva/fontes/silva_parsare.c
lists every function; Enter lands on the definition. Terminal
sibling: ./silva/arbor.sh -f on a real .c prints clean function
trees; -d shows the function rows.

**Vocabulary addenda needing Fran** (beyond open question #1):
the preference element name — proposal `<praelatio>` (that which
is preferred/carried before); alternatives praeferentia, electio.

**Named risks**: block-item fork packing (open question #1 IS the
mitigation decision); boundary-finder K&R lookahead (adversarial
fixtures = the discharge; the fallback-to-candidate semantics
must be pinned, not assumed); praelatio generator change
regressing existing tables (byte-identical gate); titulatum
census newcomers; annotation v0 vs per-clausula's optional-heavy
shape (first friction = revise the FORMAT consciously, per the
standing rule); nested-packing inflation at statement depth (the
M2a observation's next checkpoint — watchdog numbers).

**Exit criteria**: suite green; census re-pinned exhaustively
incl. the praelata category; boundary-finder adversarial fixtures
green; carried sent + func bars green; v1 syntaxis harvest
complete; comprehension pinned near-125 with remainder named;
corpus ERROR near-0 pinned; frons ceiling re-asserted;
FUNCTIONES vista + hospes; saltuarius manual bar demonstrated;
every fixture roundtrips; RELATIO + full-log audit.

**INTENTIO RESOLUTIONS (2026-07-04, Fran, previews compared)**:
open question #1 → **ONE LISTA** (DECISUS amended — genera-c89.md
§4 AMENDED note + decisiones 11; per-element forks, oracle
machinery unchanged, decl-before-stmt = lint); preference element
= **`<praelatio>`** (decisiones 12); block scope = **DEFER BY
NAME** (decisiones 13). No Sim XI needed — the amendment removes
the contested design. Chunk A is unblocked.

### M2c Chunk A — sententiae: COMPLETE (2026-07-04)

**Suite 27/27 (probatio_silva_c89: 1,159 assertions); amalgam
gates + hospes 22/22; saltuarius 13/13 / tessera 5/5.** Grammar:
+12 terminals (statement keywords — zero lexer work, all genera
carried) / +47 productions (244 total, 397 states): the full
statement tier per DECISUS §4 (si ONE genus, dum, fac-dum, per +
per-clausula as a real node in 8 explicit optional-forms,
commutatio GROUPED, titulatum, salta/frange/perge/redde,
sententia-vacua), corpus as ONE lista REUSING the root elementa
rule — the isomorphism is literal, and `{ foo * bar; baz(); }`
proves the amendment: AMBIGUUS wraps ONE element, the neighbor
stays clean, the corpus stays clean.

**Census 14 → 16 conflict + 1 praelata, exhaustively re-pinned**:
F1 widened 4→6 (same production pair, statement-context
lookaheads), F2 ×3 and F3 ×7 unchanged, ignotae 0. TWO predicted
newcomer families never materialized, both in the good direction:
grouped-switch = ZERO cells (the ec-ante/ec-post cross-rule
state-split — Artes #1 confirmed exactly; the split also handles
the subtler stray-statement-vs-group ambiguity, which is textually
impossible after the first label and now grammatically impossible
too) and titulatum = ZERO cells (precise lookaheads: COLON cannot
follow a complete statement-position expression, so `x:` never
conflicts with expression-`x`).

**`<praelatio>` SHIPPED** (the declared-preference mechanism,
first of its kind): grammar element (terminalis + actio, only
transponere known, unknown = clamat), generator resolution pass
BETWEEN table-build and conflict-detection (cell must carry BOTH
shift and reduce; lone actions untouched), SilvaTabPraelata
emission CONDITIONAL on praelationes existing — **sceletum/
imparilis outputs verified byte-identical** — silva.h type +
extern + CADENDA (the SilvaPraecommissio precedent), and the
praelata census category pinned in BOTH suites by IDENTITY
(terminal name + removed-production id, never state numbers).
The one cell: ELSE, reduction si-sine-alioquin removed. Fixture:
`if (a) if (b) x; else y;` → else binds INNERMOST, numerus
ambiguorum ZERO — resolved in the tables, not by fork.

**COMPLEXITY (driver — statement continuation)**:
discovered-while: the dangling-else fixture failed with
numerus_errorum 1 — arbor showed `else y;` orphaned as its own
ERROR segment. consists-in: the slicer ends segments at depth-0
SEMICOLON, but statements CONTINUE past semicolons (else-arm
after an unbraced consequens; the while-tail of fac-dum); the
INTENTIO had scoped the whole boundary-finder to Chunk B
(functions), but statements needed slicer-awareness in Chunk A.
consequences: every unbraced if/else and do-while split into
tree + ERROR fragment; five fixture assertions caught it within
minutes of writing. handled-by: CONTINUATION RULE in the slicer —
a depth-0 `;` is not a boundary when the next token is ELSE, or
WHILE with a pending-fac counter (counter handles nesting:
`do do x; while (a); while (b);` slices correctly); false
suppression (stray else) yields a COARSER ERROR segment, never a
wrong tree. This was the first movement of the M2c
boundary-finder; the ()-before-{ heuristic for function
definitions remains Chunk B's, as planned.

**Carried sent bar: 30/30** through the consumer path
(silva_c89_parsare), every input clean and unambiguous against
DECISUS genera — including the dangling-else input and the
grouped switch.

**Riser finding (deliberate zero)**: syntaxis comprehension
STAYS 87/125 — inspection shows every statement line in the v1
corpus is wrapped in `void f(void) { ... }` (function
definitions, plus ~5 gcc-extension lines). The entire statement
harvest arrives WITH Chunk B's function definitions; statements
themselves are proven by the sent bar + fixtures instead. Pin
comment updated with the finding.

**Numbers**: roundtrip fixtures 74 → 98 (all byte-exact,
including comment-riddled and whitespace-warped statements);
corpus ERROR nodes 62 unchanged (they ARE the function
definitions); **frons_maxima STILL 2**; parse 1.72 ms/KB (up
from 0.82 — tables 305→397 states; measured history, complete
first).

**Eighth latina firing**: `commutatio` (= switch) as a fixture
VARIABLE name — the P1 statement genera are ALL macros; safe as
STML tags, forbidden as C identifiers. Variable renamed
selectio. (The P1 decision's safety argument held: only the
hand-written C hit it, never the generated code.)

NEXT: Chunk B — definitio-functionis (ANSI + K&R) + the
()-before-{ boundary-finder + adversarial fixtures + carried
func bar (20) + block-scope decision recorded + THE RISER
HARVEST (87 → expect ~120; corpus ERROR 62 → near-0).

### M2c Chunk B — definitio functionis + limes segmentorum:
### COMPLETE (2026-07-04)

**Suite 27/27 (probatio_silva_c89: 1,369 assertions); amalgam
gates + hospes 22/22; saltuarius 13/13 / tessera 5/5.** Grammar:
+8 productions (252 total, 412 states): definitio-functionis in
FOUR forms (ANSI, K&R, implicit-int ± K&R — C89's `f(a) int a;
{ }` parses), declarationes-kr as a NON-empty lista (the ANSI
production covers empty — the empty-lista ambiguity never
exists), enumeratores grew the C99 trailing-comma production
(robustness bar; corpus demanded it; lint's to flag). Generator
lesson: the FULLEST production must come FIRST in a genus (locus
layout order is established by first occurrence).

**Census 16 → 52 pairs / 44 cells, SIX families, exhaustively
re-pinned**: the K&R price is FOLLOW smear — after `)` a
K&R declaration can follow, so every specifier keyword entered
the reduce-lookahead of declarator states. F2 (tn vs dt)
3 → 23 cells; newcomers F4 (tn/dt/pi triple, 4 cells), F5
(dt vs pi, 1), F6 (shift vs qualificatores-reduce, 3). SAME
underlying ambiguity (the ISO param rule), more columns.
Runtime: the dance ABSORBS it — `int add(int a, int b) { }`
collapses to zero ambigui via X10 (the [int, nominatus(a)]
param reading is combination-invalid). K&R with named types
(`int f(a) int a; { }`) retains ONE ambiguus honestly — the
GENUINE ISO ambiguity (param name vs typedef name), the
wildcard pin wants it retained.

**COMPLEXITY (build hygiene — the stale-object class,
SECOND member)**: discovered-while: `int add(int a, int b)`
showed ambigui 2 with the required kill not firing; a debug
print made it work — the print forced recompilation.
consists-in: generated-header regeneration RENUMBERS the genus
enum; arbor.sh rebuilt objects only when their own .c changed,
so silva_c89_oraculum.o compared `nodus->genus` against STALE
enum values — silently wrong dispatch, no compile error, no
crash. consequences: any grammar edit invalidates EVERY .o
compiled against the old generated header; the M2a "always
relink" fix covered links, not header deps;
compile_probationes.sh already had the newest_header guard
(the suite was never wrong) — only arbor.sh lacked it.
handled-by: newest_header ported to arbor.sh; lesson appended
to the stale-binary complexity family. RULE: any script that
caches .o files MUST rebuild on newer headers — generated
headers make this existential, not cosmetic.

**The boundary-finder (driver)**: per-segment state machine —
post_clausa (depth-0 `)`; EXTINGUISHED by any non-K&R-plausible
token, so `int x = (a) + b;` never false-triggers),
modus_corporis (depth-0 `{` while post_clausa → segment ends at
matching `}`, no terminator), K&R lookahead (depth-0 `;` in
post_clausa with intervening tokens → scan through
declaration-plausible material; `{` confirms, else cut at the
candidate), `}`-boundary continuation checks (ELSE and
WHILE-with-fac-pendency — Chunk A's rules extended to braced
bodies; `do { } while (a);` consumes its pendency at the `}` so
it can't leak into a following `dum`). DESIGN PROPERTY, now
proven in fixtures: every false suspicion merges segments —
COARSER RECOVERY, never a wrong tree (multi-element segments
are legal). Adversarial set green: function pointers,
prototype-then-definition, K&R multi-param, struct-returning
definitions, initializers with parens, implicit-int K&R.

**The dance reaches into bodies** (fixture): `typedef int T;
void f(void) { T x; x = 1; }` — the block-level fork inside
corpus resolves via the SAME per-element machinery (the
one-lista amendment paying again); block-local typedefs are NOT
registered (decisiones 13) and a file-scope forking use after
the body stays honestly AMBIGUUS (`T * x;` — note `T x;` would
not fork at all: no expression reading exists).

**Carried func bar: 20/20** through the consumer path, including
multi-segment typedef dances, chained typedefs, and the
preloaded-oracle pair (size_t/ptrdiff_t at situs 0). Roundtrip
fixtures 98 → 110, all byte-exact.

**THE RISER: 87 → 123/125** — three movements: +30 (definitions
+ their statements, one stroke), +5 (HARNESS DECODER BUG: the
corpus's multi-literal lines — `"pars\n" "pars"` — were
truncated at the first closing quote since the harness was
born; artifact, not comprehension; decoder now concatenates
adjacent literals and decodes \n\t), +1 (enum trailing comma).
REMAINDER = 2, NAMED: the two __attribute__/__extension__ gcc
lines; evidence gate = a real corpus ever demanding them
(rhubarb sources do not).

**Corpus ERROR 62 → 42, with the honest story**: mid-chunk
grammar-only measured 8 — that number was mega-segment
COARSENESS (EOF-terminated segments swallowing unparseable
functions AND good declarations into few ERROR nodes), not
comprehension. The boundary-finder restored per-function
granularity: the 42 are UNEXPANDED-LATINA function bodies
(`si (x) { }` = call-expression + block, no `;` — not C89
without macro expansion; these files only truly parse with
latina.h resolved through the expander = the M2d solarium
sweep's territory, include resolution and all). Good
declarations BETWEEN those functions now parse instead of being
swallowed. Fideles 78/78 unchanged throughout. frons_maxima 2 →
3 — first movement ever, ceiling VIII holds. Parse 2.09 ms/KB
(tables 397 → 412 states; measured history).

**Amalgam friction**: helper name `_lexema_proximum` collided
with silva_lexema.c's static of the same name in the amalgam TU
→ renamed `_lexema_sequens` (the amalgamator's
duplicate-definition gate caught it — working as designed).

**Vocabulary AMENDMENT PROPOSED (genera-c89.md §3, awaiting
Fran)**: K&R identifier params ride as ordinary parametrum
readings (prototype machinery + oracle), NOT folium-identificator
as the original note said — a dedicated production would fork
every single-identifier parameter everywhere, permanent
pollution for a naming nicety. Non-empty declarationes-kr marks
K&R-ness; path back is additive.

NEXT: Chunk C — vista FUNCTIONES (rows for definitio-functionis
via the declaratoris_titulus chase; signature text exercises
scribere-from-subtree) + silva.h + hospes; then Chunk D
saltuarius TOC (TYPI/DECLARATIONES + FUNCTIONES, the original
pull) + M2c RELATIO + full-log audit.

### M2c Chunk C — vista FUNCTIONES + subscriptio: COMPLETE
### (2026-07-04)

**Suite 27/27 (probatio_silva_c89: 1,388 assertions); amalgam +
hospes 23/23; saltuarius 13/13 / tessera 5/5.**

**K&R resolution — cheaper than designed**: the plan was a
pater-walk policy rule; the tree showed the false K&R reading is
`[int, typus-nominatus(f)]`-shaped — COMBINATION-INVALID, X10's
textbook case — but _nodum_examinare's genus switch predates
definitio-functionis. ONE case label (examine the definitio's
specificatores) and both K&R forms collapse to zero ambigui
through the EXISTING kill. ISO 6.9.1 (identifier-list params ARE
names) is the semantic justification; X10 is the mechanism; no
new machinery. Chunk B's "honest retention" fixture upgraded to
assert the collapse + kr-lista navigation.

**Vista FUNCTIONES**: _vistas_ambulare gained the
definitio-functionis case — one row per definition, titulus via
the X5 declaratoris_titulus chase on the declarator locus (works
through parenthesized and K&R declarators). The walk also gained
a nodus_out parameter (threaded through all recursion sites) so
row-index → node is answerable internally.

**silva_c89_functionis_subscriptio** (new public API, silva.h +
hospes same-change): specificatores + declarator emitted
byte-exactly from the tree via silva_scribere_valorem (the M2
sketch's scribere-from-subtree exercise), skipping corpus and
declarationes-kr; implicit-int forms emit declarator alone.
LEARNED: trivia attaches TRAILING in this codebase — signatures
come out left-clean with one trailing space, the ideal TOC-row
shape ("static int *f(int a, char b) ").

**Demo (arbor -d on real files)**: hospes.c → 11 rows, 0 errors —
`fidelis` and `main` as definitio-functionis rows with line
numbers (Enter targets); the latina-canary declarations as
initiatus rows. Latina modules (silva_c89_oraculum.c) show only
their macro-free functions — the expander-integration story,
named at Chunk B, unchanged (M2d).

NEXT: Chunk D — saltuarius TOC integration (structura pane grows
TYPI/DECLARATIONES + FUNCTIONES sections via the vista + the
manual bar: Tab on a silva module lists functions, Enter lands
on the definition) + M2c RELATIO + FULL phase-log audit (phase
boundary).

### M2c Chunk D — saltuarius TOC: COMPLETE (2026-07-04)

**Saltuarius 13/13; silva 27/27 (probatio_silva_c89: 1,395
assertions); tessera 5/5; amalgam + hospes 23/23.** The Chunk E
promise delivered: structura's pane grew THREE sections at once —
FUNCTIONES / TYPI / DECLARATIONES — appended after the
preprocessor sections (INCLUSIONES first preserved; section-ORDER
preference = Fran's taste call, named).

**API-pressure finding, fixed silva-side through the gates** (the
VISIO rule working as written): the TYPI section needs
typedef-ness, which the vista did not carry —
SilvaDeclaratioVista grew **est_typedef** (filled from
_habet_typedef on the row's declaratio; definitio rows FALSUM),
silva.h + fixture + amalgam same-change.

**The integration design**: a TEMPORARY c89 parse per
Tab-opening (silva_piscina arena, destroyed after building —
_titulum copies row text into the index arena, so NOTHING
persists; no LRU interaction; parse failure = sections absent,
table lives — the every-file-opens pin). FUNCTIONES rows carry
the SUBSCRIPTIO text (byte-exact signature from the tree) with
line numbers; TYPI = est_typedef rows; DECLARATIONES = the rest.
Rows from TAKEN conditional arms only (the vista walk) — the
fixture asserts `int c;` from a taken #else appears at its line
and the untaken `int a;` does not. Jump wiring: the three new
ordo genera join the DEFINITIO/REGIO linea-jump path.

**The manual bar** (the original pull, October's TOC pressure
point): Tab on a C89 file lists every function with its
signature and line; Enter lands on the definition.
Suite-proven on the fixture (quadratum row: SALT_ORDO_FUNCTIO,
"int quadratum(int x)", :18, saltabile); the interactive form is
Fran's to savor via ./saltuarius/saltuarius.sh. NB latina
modules show only macro-free functions until expander include
resolution (M2d, named at Chunk B).

## M2c — SENTENTIAE + FUNCTIONES: RELATIO (2026-07-04) — COMPLETE

**Every INTENTIO exit criterion met**: suite green everywhere
(silva 27/27 with 1,395 c89 assertions, saltuarius 13/13,
tessera 5/5, hospes 23/23); census re-pinned exhaustively at
every step (14 → 16 → 52 pairs / 44 cells in SIX named families
+ the praelata category, ignotae 0 throughout); boundary-finder
adversarial fixtures green; carried sent (30) + func (20) bars
green through the consumer path; **v1 syntaxis harvest COMPLETE:
comprehension 87 → 123/125** with the remainder NAMED (2
gcc-extension lines, evidence-gated); corpus ERROR 62 → 42 with
the honest-granularity story; frons 2 → 3 (first movement,
ceiling VIII holds); FUNCTIONES vista + subscriptio + hospes;
saltuarius TOC manual bar; every fixture roundtrips (110).

**What M2c built**: the statement tier (15 genera), the
`<praelatio>` declared-preference mechanism (dangling-else
resolved in tables, cell pinned by identity), the one-lista
corpus (DECISUS amended — the fork stays per-element and the
oracle machinery needed ZERO changes to reach inside bodies),
definitio-functionis in four forms (ANSI/K&R × explicit/implicit
int), the statement-aware + ()-before-{ boundary-finder with K&R
lookahead, K&R-to-zero-ambigui via one X10 case label (ISO 6.9.1
justifies, existing machinery executes — decisiones 14),
SilvaDeclaratioVista rows for functions + est_typedef +
silva_c89_functionis_subscriptio (scribere-from-subtree), and
the saltuarius TOC (three sections, temp-parse design).

**FULL PHASE-LOG AUDIT (M2c boundary)**: every M2c-owned item
discharged — grouped-casus mechanics (state-split, ZERO cells),
dangling-else preference (praelatio SHIPPED), K&R params
(decisiones 14), ()-{ boundary + adversarial fixtures (Chunk B),
block scope (DEFERRED BY NAME, decisiones 13, M2d evidence
gate), comprehension riser (deliberate at every step, remainder
named), v1 syntaxis harvest (complete; the 234-assertion figure
of the sketch was superseded by the corpus-line form pinned at
M2b). The M2a breadcrumb instrument: never demanded by a bar —
NOT built; stays a saltuarius v0.1 candidate BY NAME. Parks
carried forward, each with an owner: latina-corpus expander
integration + include resolution (M2d solarium sweep — the 42
corpus ERRORs are its work order); Duff's device = ERROR
(grouped-casus price, M2d sweep evidence); gcc extensions
(evidence gate); sceletum manifest swap-out (M2d); ambient
error badge (M2d); nuda tag-final + struct-const divergences
(corpus evidence, unchanged); TOC section order (Fran's taste);
K&R-lookahead false-suspicion coarseness + fac-pendentia
merge case (benign BY CONSTRUCTION — wrong trees impossible,
documented in the slicer comments). Zero unowned complexities.

**Process notes for the M2d sessions**: the stale-object hazard
class gained its second member (generated-header enum
renumbering — newest_header now guards BOTH build scripts; the
class rule is in the Chunk B complexity); the harness-decoder
bug shows acceptance corpora need their DECODERS pinned by
fixture too (a corpus line that never reaches the parser is a
silent comprehension lie); and three separate times this
milestone, the honest-number instinct (ambigui 2? ERROR 8?
riser +0?) turned a suspicious metric into a real finding —
census discipline is not bookkeeping, it is the debugger.

NEXT: **M2d — the acceptance wall** (per the M2 sketch):
solarium 148/148 held + parse-the-monorepo sweep WITH expander
include resolution (latina files fully parse; corpus ERROR
42 → the real number), tier-1 roundtrip corpus byte-exact
through real trees (78/78 already holds), carried lapifex
integration bar, sceletum→c89 manifest swap-out for saltuarius's
MAIN parse, the ambient error badge ("silva: N errores"),
block-scope + Duff evidence counts, and the comprehension metric
goes AMBIENT. M2d INTENTIO not yet written — next act.

### Addendum post-RELATIO M2c (2026-07-04) — probatio manualis
### Fran in silva_scribere.c

Three observations, one root + one real bug (fixed):

**1. BUG (fixed): banner comments as row titles.** Trivia
attaches TRAILING, but a comment with no preceding parse-stream
token (after a directive, at segment start) attaches FORWARD to
the next token — so the subscriptio of a function preceded by a
"/* ==== */" banner began with the banner, and _titulum's
40-char window showed only that. FIX: subscriptio now TRIMS
leading whitespace + comments (it is ROW text, not roundtrip
text — documented in the function). Fixture pinned; suites +
amalgam green.

**2. Structs shown as FUNCTIONES — the unexpanded-latina
misparse, named.** `nomen structura { s32 offset; } X;` with
latina UNRESOLVED parses — grammatically correctly! — as a
function definition: specifiers [typus-nominatus(nomen)],
declarator titulus(**structura**), corpus { declarations },
plus a stray statement `X;`. Garbage in, consistent tree out.
EXPANDED, the same text is `typedef struct {...} X;` → a TYPI
row "X", which IS where struct definitions belong (typedef'd →
TYPI via est_typedef; tag-only `struct S {...};` → nuda row in
DECLARATIONES). So yes — structs are supposed to appear, in
their sections, once the TOC parse can expand latina.

**3. Missing functions — same root.** Latina bodies (si/redde/
per unexpanded) are not C89; those definitions are ERROR
segments → no rows. The two PROTOTYPES at lines 57-58 correctly
appear as declarator-functionis rows (DECLARATIONES section).
All of #2/#3 = the M2d work order already named (expander +
include resolution for the TOC parse; the saltuarius liber parse
ALREADY resolves includes via nexus — M2d should let the c89
TOC parse ride that same expanded stream instead of re-lexing
raw bytes — noted as the natural M2d design).

OPEN (Fran's taste): should PROTOTYPES join the FUNCTIONES
section (headers would then list their API there — IDE-outline
convention) or stay in DECLARATIONES (definitions-only purity)?
One strcmp either way.

### Addendum: PROTOTYPA sectio (2026-07-04, Fran)

Fourth c89 section shipped same-day: PROTOTYPA = vista rows with
genus declarator-functionis, non-typedef (typedef'd function
pointers stay TYPI). Headers now list their API under its own
heading. NB named imprecision: function-POINTER variables without
initiators (`int (*fp)(void);`) also land here — the vista
carries genus only; distinguishing needs tree access (candidate
est_prototypum vista field if it ever annoys). DECLARATIONES
excludes them. Fixture: duplum() :19. Saltuarius 13/13, silva
27/27.

## M2d — THE ACCEPTANCE WALL: INTENTIO (2026-07-04)

The honest counter flips meaning: M1 recorded ERROR nodes as
RECOVERY; M2d drives them toward zero as COMPREHENSION, over the
real monorepo, with the expander finally feeding the c89 grammar.
After M2d, M2 closes.

**Grounding**: silva_parsare_cum_contextu EXISTS (generic driver,
Phase 7) — the c89 wrapper needs only its mirror.
saltuarius_nexus.c holds the 4 sceletum references (the main-parse
swap site). The M2c corpus ERROR number (42) is entirely
unexpanded-latina bodies — the work order.

**Chunks**:
- **A — expander-fed c89 parsing.**
  silva_c89_parsare_cum_contextu (mirror of the plain wrapper:
  contextus + oraculum + dance + policy); mensura corpus runs
  WITH a latina.h-bearing contextus → corpus ERROR 42 → the real
  number, PINNED with remainder named; saltuarius TOC parse
  passes the nexus contextus → latina files fully populate all
  four c89 sections (the silva_scribere.c test Fran ran becomes
  the manual bar: structs in TYPI, functions in FUNCTIONES,
  banner-free titles).
- **B — saltuarius main-parse swap (decisiones 9 due date).**
  GRAMMATICA_SCELETI → c89 in nexus/liber; the whole saltuarius
  suite is the gate (source view, layers, origo chains must not
  notice); perf/memory measured against the 84-242MB LRU
  reality (c89 tables are 15x sceletum's states — watch arena
  peaks in F2). THEN sceletum retirement from the AMALGAM
  manifest (generator-fixture role stays in-repo; hospes
  sceletum fork block updates; the deliverable shrinks —
  amalgam gates decide).
- **C — the monorepo sweep + evidence counts.** Parse every
  rhubarb .c/.h through the full pipeline with include
  resolution; ERROR-node census recorded in mensura history
  (the headline metric); solarium 148/148 held; carried lapifex
  integration bar through the consumer path. EVIDENCE COUNTS
  with dispositions logged: block-scope typedefs (decisiones 13
  gate), Duff's device occurrences (grouped-casus price),
  struct-tag-const + nuda divergences, gcc-extension demand.
- **D — the ambient badge + M2 CLOSE.** Saltuarius status/F2
  gains "silva: N errores" per file — the acceptance test runs
  continuously in daily driving, incomprehension gets NOTICED
  not audited. M2d RELATIO + full-log audit + **M2 milestone
  close-out** (M2.0→M2d retrospective; post-M2 map: quaestio
  selectors / formator / typus+index with their frozen v1 bars
  per spec-v2 §4.1).

**Named risks**: the main-parse swap is the widest blast radius
of the milestone (everything saltuarius renders sits downstream)
— the suite + manual daily-driving are the gates, revert is
cheap (one bundle swap); include-resolution parity between TOC
parse and liber parse (same nexus contextus = same answers, by
construction); solarium byte-exactness under c89 trees; amalgam
size/perf after sceletum retirement; sweep wall-clock (2.1
ms/KB × monorepo — measure first, parallelize never).

**Exit criteria**: suites green everywhere; corpus ERROR pinned
at its real number, remainder NAMED; monorepo sweep numbers in
mensura history; all four evidence counts logged with
dispositions; badge shipping; sceletum out of the amalgam;
carried integration bar green; M2d RELATIO + full-log audit +
M2 close.

### M2d Chunk A — expansione pascitur: COMPLETE (2026-07-04)

**Silva 27/27; saltuarius 13/13 (app compiles under the full
wall); tessera 5/5; amalgam + hospes 24/24.**

**THE HEADLINE: corpus ERROR 42 → 0, PINNED.** The M2 arc
completes its number: M1 recorded 41k ERROR nodes as recovery →
M2b 62 → M2c 42 (all unexpanded latina) → **M2d 0** — every one
of the 78 real rhubarb files parses completely through the
expanded c89 grammar, 78/78 byte-exact THROUGH the expansion,
frons still 3. The zero is a CREDO assert now (murus acceptionis
— regression screams). Parse 4.04 ms/KB with expansion included
in the c89 timing.

**Shipped**: silva_c89_parsare_cum_contextu (mirror wrapper —
contextus + dance + policy; silva.h + hospes latina-source proof
"interior int quaesitum_lat(int a) { redde a + 1; }" → real
definitio row through the amalgam); mensura corpus on a shared
latina contextus (+ verbosa per-file error diagnostic, kept);
**#pragma became a true directive** (SILVA_DIR_PRAGMA:
classified, captured as a directive line for scribere, consumed
from the parse stream — it was the LAST corpus error, gluing
"#pragma once" into the next declaration; legal C89 6.8.6,
semantically inert); saltuarius structura_aedificare gained the
nexus (TOC parse rides nexus->ctx with plain-parse fallback;
fixture: "nomen structura { int n; } Probandum;" → TYPI row :20,
misparse text asserted ABSENT); arbor rides a latina contextus
too (dev-tool parity — the terminal demo of the whole fix).

**The demo number**: silva_scribere.c, the file Fran manually
tested — 19 errors / 3 phantom rows → **0 errors / 24 true rows**
(17 definitiones, 2 prototypa, typedefs, globals), every function
with its Enter-jump line.

NEXT: Chunk B — the main-parse swap (GRAMMATICA_SCELETI → c89 in
nexus/liber; saltuarius suite = the gate; then sceletum OUT of
the amalgam manifest, hospes fork block updated, generator-fixture
role stays in-repo).

### Addendum Chunk A+ (2026-07-04) — filtrum fons_princeps
### sectionum c89 (quaestio Fran)

Fran asked: once the TOC parse chases includes, won't every file
show every included function? YES — and the leak was LIVE (the
app seeds praebenda from compile_flags.txt). The pieces already
existed: tokens carry fons_index (provenance), and the
preprocessor sections have filtered by fons_princeps since D1.
SHIPPED: SilvaDeclaratioVista grew **fons_index** (the TITLE
token's file; -1 when anonymous), silva.h mirror, and all four
c89 sections filter rows to fons_princeps. Fixtures both sides:
silva (ex_amico row from a praebere'd header carries a foreign
fons_index; meus carries fons_princeps) and saltuarius (amicus.h
gained a prototype — asserted ABSENT from the TOC, joining
CELATUM).

The MACRO-EXPANSION requirement holds by construction: a
function's NAME token is textually in the current file even when
its body expands macros — the name's provenance decides, the
body is free. NAMED EDGE: functions whose NAMES are
macro-generated (X-macro patterns) follow the name token's
provenance, which may point into the macro definition — the
origo-chain machinery can refine this when real code demands it.
Fran's LAYER idea (per-stratum TOC riding the L-flip machinery —
"the TOC of layer 3") is NAMED to the saltuarius backlog: strata
are always-on, so the pieces exist; it is a view question, not a
parser question.

### M2d Chunk B — permutatio parsurae principalis: COMPLETE
### (2026-07-04)

**Silva 27/27; saltuarius 13/13 (app compiles under the wall);
tessera 5/5; amalgam + hospes 24/24. Decisiones 9 discharged.**

**The swap was ONE call site**: saltuarius_nexus's librum-parsare
now calls silva_c89_parsare_cum_contextu — the main parse gets
the full oracle dance, not just the grammar. The entire
saltuarius suite passed on the FIRST run after the swap: the
downstream surfaces (lexeme classing, origo chains, strata,
conditional regions) proved genuinely grammar-independent, as
designed.

**The swap's dividend, taken immediately**: the TOC's second
parse is GONE. structura_aedificare reads the vista straight
from liber->parsura (now c89 + contextus); Tab costs a walk, not
a parse. The nexus parameter added this morning was reverted —
the signature is (index, liber) again; a small SilvaPiscina
exists per-aedificatio only for subscriptio text (then dies).
The fons_princeps filter and all fixtures held unchanged.

**Sceletum OUT of the amalgam** (the M1-era park, discharged):
manifest dropped silva_tabulae_sceleti.{h,c}; silva.h dropped
the four sceletum declarations; hospes's fidelis() and
cum_expansione block run the c89 grammar (strictly stronger —
the recovery fixture "@ $ garbage" and the fork fixture "foo *
bar;" ride c89 recovery/retention now). Sceletum REMAINS in
fontes/ + grammatica/ as the generator fixture and the modular
suite's regression anchor — only the DELIVERABLE shed it.
Amalgam 1.47 MB → 1.52 MB (Chunk A+B additions net of sceletum
removal — honest number, tables dominate).

**Fran's manual check (named)**: daily-drive arena peaks via F2 —
the c89 tables are ~15x sceletum's states; the LRU was sized
against 84-242 MB per-root peaks. The suite proves correctness;
only real driving proves the memory envelope.

NEXT: Chunk C — the monorepo sweep + evidence counts (block-scope
typedefs, Duff's device, struct-const/nuda divergences, gcc
demand) + solarium 148/148 + carried integration bar.

### M2d Chunk C — percursus repositorii: IN PROGRESS (2026-07-04)
### — pre-compaction state

**THE HEADLINE: 31 ERROR nodes in 10 files across the ENTIRE
monorepo (721 files, 18.3 MB), totalitas TENET, 720/721
byte-exact.** From M1's 41,000. Instrument: silva/percursus.sh
(instrumenta/principalia/percursus.c — recursive sweep, latina
contextus, evidence counters, per-file apex tracking).

**FINDINGS (all named, some open)**:
1. **Arena pathology (OPEN investigation)**: dense
   data/generated-table files inflate the parse arena
   catastrophically — biblia_dr.c (5.8MB) → 49 GB apex,
   arbor2_glr_tabula.c (1.5MB) → 22.7 GB (~8-15,000x; normal
   code ~30x). First full sweep died SIGKILL/OOM. Sweep now has
   a 4MB ceiling with NAMED skips (capsula_libri.c,
   biblia_dr.c; -omnia disables). INVESTIGATE with lustrum
   subsystem telemetry — suspect per-element cost in giant
   initializer listas (GLR stack? valores copies? nodes).
2. **INFIDELIS (OPEN)**: lib/arbor2_glr_tabula.c is the ONE
   roundtrip failure in the repo — same file as the 22.7GB apex;
   suspect the pathology degrades something. Investigate after
   (or with) #1.
3. **Lexicon-latina collision (SOLVED for the sweep, REAL
   design item)**: unconditional latina lexicon corrupts
   English-C — knotapel's "double co = cos(x), si = sin(x);"
   → si becomes IF → errors. Textual "latina.h" detection FAILS
   (inclusion is transitive via piscina.h). Sweep heuristic:
   knotapel → contextus nudus, cetera → latina. TRUE FIX named
   for Chunk D consideration: include-driven expansion
   (praebere + real resolution) instead of unconditional
   lexicon; ALSO applies to saltuarius browsing knotapel files!
4. **The 10 error files**: 2×hospes.c (silva+tessera — the
   latina-canary variables "static int si = 1;" ARE the
   collision class, self-inflicted by design, would be clean
   nudo); raqiya scrutinium probationes (5 errors — likely the
   scrutinium.h Duffiana class, INSPECT); lib/fasti.c (4),
   entitas.c, probatio_actor/nuntium/entitas (1-2 each,
   INSPECT — nuntium:26 showed casus-in-errore = possible real
   Duffiana or collision).
5. **Evidence counts (dispositions PENDING full
   classification)**: typedef-in-corpore 10 (real sites exist —
   incl. knotapel demo_98:1071 + our own fixtures; decisiones 13
   gate has its number, disposition owed); Duffiana 6 (sites in
   scrutinium.h + probatio_nuntium — inspect whether real Duff
   or collision-induced); gcc-in-errore 0 (divergence FREE).
6. Perf: 1.9 ms/KB across the repo; solarium 148/148 = the
   existing probatio_silva_solarium, green in every suite run.

**REMAINING for Chunk C**:
- carried lapifex EXPANDERE bar: the VectisExp block designed
  (17 composite inputs w/ lexicon-injection cases via
  silva_contextus_lexicon_addere(ctx,via,textus,mensura)) but
  the EDIT WAS REJECTED (stale read) — NOT YET APPLIED to
  probatio_silva_c89.c; re-apply before the syntaxis-corpus
  block, then suite.
- error-file inspections (#4) + evidence dispositions (#5).
- the two OPEN investigations (#1 arena pathology, #2
  INFIDELIS) — possibly their own chunk-let; lustrum is the
  tool.
- percursus.c + percursus.sh are NEW UNCOMMITTED files.

### Addendum Chunk C — experimentum scalae arenae (2026-07-04)

Synthetic initializers ("int t[] = {1, 1, ...};"): 10KB → 852 MB
apex, 100KB → 8.5 GB apex — ratio EXACTLY 10x = **LINEAR, no
quadratic bug; the constant is ~86,000x per source byte (~258 KB
of arena PER ELEMENT)**. Time flat at ~12.5 ms/KB. 1MB run OOMs
(would need ~85 GB). Short numeric elements are WORSE than
biblia's long strings (8,000x) — element density drives it.

CONSEQUENCES: (a) hypothesis "trivia xar overhead" (~1 KB/token)
CANNOT account for 258 KB/element — something allocates orders
of magnitude more per element (GLR-layer per-reduction
allocation? per-node lista pre-allocation? piscina ladder?);
(b) FIRST RULE OUT: does piscina_summa_apex_usus count USED
bytes or total ALVEI capacity incl. the doubling ladder?
(over-count would shrink but not excuse the number);
(c) the CENSUS INSTRUMENT (per-subsystem allocation counters on
the parsura) is now REQUIRED, not optional — next act of the
investigation. Interning/hash-consing REJECTED as the fix
direction (nodes/tokens are position-full BY PIN — the win is
derivability, e.g. trivia as spans, IF the census indicts
representation rather than machinery).

### Addendum Chunk C — census arenae + sanatio: PATHOLOGIA
### RESOLUTA (2026-07-04)

**THE VERDICT: the census indicted MACHINERY, not
representation — and the machinery is fixed. Synthetic apex
88.8 MB → 14.1 MB (6.3x); biblia_dr.c 49.2 GB → 6.7 GB (7.4x);
the full-repo sweep runs CEILINGLESS for the first time: 723/723
plagulae (both named skips readmitted), totalitas TENET, the
same 31 ERROR nodes, 722/723 byte-exact, 1.37 ms/KB.** Trees are
byte-identical through both fixes — this was pure engine waste.

**Act (b) first — the counter is honest**: piscina maximus_usus
sums per-alveus OFFSETS (bytes handed out), not capacities. No
ladder over-count; 258 KB/element was real demand. (The 49 GB
apex vs 4.5 GB RSS gap = macOS compressing the mostly-zeroed
abandoned scratch; xar memsets its segments, so pages ARE
touched. The demand was real, the OS merciful.)

**The census-lite instrument**: PISCINA_DEBUG in lib/piscina.c
is now #ifndef-guarded (-DPISCINA_DEBUG=1 from the compile
line — one permanent lib improvement); a scratchpad build of
percursus + an awk histogram over the per-allocation stream.
One run on a 1 KB synthetic initializer fingerprinted the
culprit instantly: **5,959 allocations of exactly 12,416 bytes
= 84% of the apex** — the viae xar FIRST SEGMENTS (16 x
sizeof(SilvaGSSVia) = 16 x 776). _vias_enumerare created a
fresh Xar of SilvaGSSVia PER REDUCTION; a literal "1" climbs
~18 precedence-chain reductions, each costing 12,416 + 576
(header) abandoned bytes = the 258 KB/element constant, closed
exactly. SilvaNodus is 24 bytes — the TREE was ~1% of the arena.

**Fix 1 — right-size**: viae xar first segment I (one via —
reductions are almost always unambiguous), doubling only under
real forks. 88.8 → 23.4 MB.

**Fix 2 — reuse (lifetime-audited)**: glr->viae_effimerae, one
persistent xar on the engine, xar_vacare'd at the top of
_vias_enumerare (segments stay allocated — high-water reuse,
same pattern as acceptati). AUDIT: constructor consumes
valores[i] BY VALUE (generated code: nodus_ponere/lista_appendere
copies, never &valores[i]); via->basis copied out as a GSS
pointer; _nodum_processare is driven by two flat loops (frons +
reducenda FIFO — reductions QUEUE, never recurse), and multiple
REDUCERE actions in one conflict cell run sequentially — exactly
ONE viae live at a time. _vias_recursio fully writes
valores[0..gradus-1] + basis on every path it later reads, so
vacare-without-rezero is safe. 23.4 → 14.1 MB.

**Amalgamator note**: xar_creare_cum_magnitudine came OFF the
excludenda list (first silva use) — the compile clamavit exactly
as the protocol promises.

**Gates**: silva 27/27, amalgamare VERIFICATUM (hospes 24/24),
saltuarius 13/13, tessera 5/5. lib/ sweep errors unchanged
(fasti 4, entitas 1); INFIDELIS arbor2_glr_tabula.c PERSISTS
post-fix (investigation #2 stays open — pathology-degradation
hypothesis weakened now that the pathology is gone; inspect the
diff directly).

**NAMED next tiers (not approved, in descending locality)**:
(1) praedecessores xar per GSS node = 704 B (576 header + 128
first segment) for typically ONE predecessor — an inline first
slot + lazy overflow xar would cut ~35% of the remaining apex;
(2) per-step passus xars (index/reducenda/frons_nova) could
reuse via vacare, but frons_nova BECOMES the next frons —
needs a double-buffer, more surgery; (3) lib-wide: the Xar
header is 576 B because segmenta[64] is inline — 12,172 headers
= half the remaining synthetic apex; a smaller initial table
would pay across ALL of rhubarb (API-stable, blast radius =
every consumer). Dense-data files still cost (capsula_libri.c
12.8 MB claims 64 GB apex / 7 GB RSS, completes) — tier 1-3 is
where that falls next.

### Addendum Chunk C — INFIDELIS dispositum + vectis expandere
### applicata (2026-07-04)

**INVESTIGATION #2 CLOSED: the INFIDELIS was never a bug.**
arbor2_glr_tabula.c is the ONE repo file whose macros use ##
(STATUS_##n##_ACTIONES) and get expanded — scribere hits the M1
pasta fail-loud ("origo pasta/chorda/api - stratum 0 non
recuperabile, deferral nominatum") EXACTLY as designed. Parse is
clean (0 errors); only reconstruction declines. The
pathology-coincidence hypothesis is dead (big generated file
just also uses ##). Diagnosed with a scratchpad first-divergence
driver (infidelis.c — parse, scribere, print first divergent
byte ± context; revealed scriptura.successus FALSUM, not byte
divergence — candidate for instrumenta/ promotion). Pasta demand
in-repo = 1 legacy generated file; the deferral STANDS.

**Instrument honesty patch**: percursus now distinguishes
[SCRIPTURA FRACTA: causa] from [INFIDELIS octetim] — the
conflated label is what cost a full investigation slot on a
known deferral. Sweep now self-explains.

**Carried lapifex EXPANDERE bar APPLIED (the stale-read edit,
re-derived fresh)**: VectisExp block in probatio_silva_c89.c
before the syntaxis-corpus block — 17 composite inputs through
the CONSUMER path (silva_c89_parsare_cum_contextu): in-source
macros (object/function/nested/multi-element/stringify), latina
via latinam_addere (definitio, interior, nomen structura,
typedef+usus across elementa, per/frange bodies), injected
lexica via lexicon_addere (macro type, typedef-to-oracle,
composed with source macros and latina). Element counts pinned
empirically FIRST (scratchpad probe): directives create NO root
elementa; the two-declaration macro yields 2. All 17 pure
(errores 0, ambigui 0). Suite: probatio_silva_c89 1,395 → 1,554
asserts, green FIRST run; full suite 27/27.

**Chunk C remaining**: error-file inspections (current 31 nodes
/ 10 plagulae list) + evidence dispositions (typedef-in-corpore
10, Duffiana 6, gcc 0). Then Chunk D.

### Chunk C — inspectiones errorum + dispositiones evidentiae:
### COMPLETE (2026-07-04)

**Instrument promoted**: infidelis.c lives in
instrumenta/principalia + silva/infidelis.sh — per-file
diagnostic: ERROR nodes with lines + token fragments, then
fidelity (first byte divergence with two-sided context, or the
scriptura fractura CAUSA). Grew out of the arbor2 investigation;
used for every inspection below.

**THE COMPLETE ERROR CENSUS — 31 nodes / 10 plagulae = THREE
classes, every error named:**

1. **hospes.c x2 = 18 nodes — latina-collision canaries, BY
   DESIGN.** infidelis renders it self-documenting: "static int
   if = 1;" — the host-pollution canary variables (si, per,
   character, nomen, structura, redde, dum, vacuum) become
   keywords under the sweep's unconditional latina lexicon.
   These files never include latina.h; include-driven expansion
   (the named Chunk D item) makes them clean automatically.
2. **raqiya probationes x3 = 5 nodes — unexpanded SCR_WRAP_*
   invocations.** scrutinium.h is included but percursus
   resolves no includes; file-scope function-like macro
   invocations parse as nothing. SAME fix class: include-driven
   expansion. (scrutinium.h itself: CLEAN — the pre-compact "14
   errors" memory was from the broken textual-heuristic run.)
3. **LL/ULL suffix x5 files = 8 nodes — C99-ism, NEW evidence
   class.** The lexer is correctly C89: "12LL" lexes "12L" + "L"
   — and the latina lexicon then expands the orphan L to Roman
   numeral 50 ("int x = 12L 50;"). Repo demand: EXACTLY the 5
   error files (fasti 4, entitas 1, probatio_actor/nuntium/
   entitas 3), compiled with -Wno-long-long since birth.
   DECISION OWED (Fran): accept LL/ULL in the number lexer
   (robustness pin: any bytes -> tree; lint substrate later tags
   it C99) = 31 -> 23 errors, vs. hold C89 purity at the lexer.

**Dispositions:**
- typedef-in-corpore: 11 sites (5 = our own suite's Vectis
  blocks in main, 1 silva_generare, 5 knotapel) — ALL absorbed
  by the oracle dance, ZERO errors attributable. Decisiones 13
  deferral STANDS comfortably.
- Duffiana: 6 -> REAL DEMAND 0. All six casus markers sat
  inside probatio_nuntium's LL-broken main; no Duff's device
  exists in the monorepo. The grouped-switch decision costs
  NOTHING.
- gcc-in-errore: 0. Free.
- PROVENANCE ARTIFACT (noted, minor): percursus markers print
  expanded tokens' definition-side linea — "casus :26" =
  latina.h:26, "nomen :39" = latina.h:39. Markers could print
  fons; named, not fixed.

**Chunk C is now CLOSED except the two carried decisions into
Chunk D: (a) include-driven expansion (kills 23 of 31 errors +
retires the knotapel directory heuristic + fixes saltuarius-on-
knotapel), (b) the LL/ULL lexer question (kills the other 8).**
Roundtrip held through every inspected file (FIDELIS even with
errors — recovery byte-conservative, as pinned).

### M2d Chunk D — INTENTIO (2026-07-04)

The close-out chunk. Four items, in order:

**D1 — LL/ULL in the number lexer (APPROVED by Fran).** The
suffix run after digits lexes as ONE token (pp-number is greedy
in real C; the constraint is conversion-time, not lex-time —
our lex-level split was the only C89-purism in the pipeline
stricter than the standard's own layering). Robustness pin:
any bytes -> tree; the lint substrate later tags C99-isms by
QUERY. Lexer suite pins the new suffixes; expected: the 5
LL-class files (8 nodes) go clean.

**D2 — include-driven expansion in percursus (retires the
knotapel heuristic).** The machinery exists (contextus
praebenda; saltuarius seeds it from compile_flags since A+;
roundtrip through foreign-fons tokens is D1-of-M2d machinery,
proven). percursus praebet include/ + the file's own directory;
files that transitively include latina.h get latina, files that
don't (hospes canaries! knotapel! raqiya) get their REAL
lexica — scrutinium.h's SCR_WRAP expands, canaries stay
identifiers. Expected: hospes 18 + raqiya 5 go clean; heuristic
_vult_latinam DELETED; sweep perf measured (include chasing per
file — watch ms/KB).

**D3 — the ambient badge.** Saltuarius status/F2 gains
"silva: N errores" for the focused file — the acceptance test
runs continuously in daily driving; incomprehension gets
NOTICED, not audited.

**D4 — M2d RELATIO + FULL-log audit + M2 CLOSE.** The phase
boundary ritual: whole-log re-read, every complexity resolved
or parked with a named landing spot; M2.0 -> M2d retrospective;
post-M2 map (quaestio selectors / formator / typus+index with
frozen v1 bars, spec-v2 par 4.1).

**Exit criteria**: monorepo sweep 723/723 at ZERO errors with
no heuristic and no ceiling; suites + amalgam + consumers green;
badge shipping; RELATIO + audit + M2 CLOSE written.

### M2d Chunk D — D1+D2+D3 SHIPPED (2026-07-04)

**THE HEADLINE: monorepo 724/724 plagulae, inclusio VERA (no
lexicon heuristic, no size ceiling), errores 31 -> 1 — and the
one survivor is NAMED.** Duffiana 0. All gates green (silva
27/27, amalgamare VERIFICATUM + hospes 24/24, saltuarius 13/13,
tessera 5/5).

**D1 — suffixus avidus.** The integer-suffix branch in
silva_lexema.c consumes the whole [uUlL] run: pp-number is
greedy in real C, the constraint is conversion-time — the old
split was the only place the pipeline was STRICTER than the
standard's own layering. Lexer pins (12LL/0x1FULL/42ull/7LLU +
float f singular), VectisDecl integration pin. The 5 LL files
went clean immediately.

**D2 — inclusio vera in percursus.** Pre-pass praebet every
repo .h under its BASENAME (dedup primus-vincit, saltuarius
exemplar; 167 capita, 5 collisions = fixture copies LOSING,
correctly). Heuristic _vult_latinam + ctx_nudus DELETED. Two
finds on the way:
- Praebenda must root at the REPO (cwd), not the sweep radix —
  silva/fontes files include include/latina.h (16 capita -> 602
  phantom errors until fixed).
- **Basename-recidiva in the EXPANDER** (silva_expandere.c
  _includendum_processare): real sources write
  "../include/utf8.h"; exact praebenda lookup missed and the
  WHOLE chain (latina.h transitively!) stayed unresolved — 30
  new honest errors across 7 relative-include files. Fix: exact
  lookup first, basename retry second, both-miss = conservative
  pass-through unchanged. This also upgrades saltuarius (same
  praebenda-by-basename model) for free.
Cost of truth: sweep 1.37 -> 3.92 ms/KB (include chains
re-expanded per file). RSS 7.3 GB peak, no ceiling needed.

**D3 — insigne ambiens.** Saltuarius main status line shows
"err N" (alarm color) ONLY when liber->parsura carries errors —
clean files show nothing, incomprehension gets NOTICED. F2
telemetry line always carries "err N" between arena and q.
Goldens untouched (fixtures parse clean, badge absent — by
design). Named for Fran's daily driving: the badge is the
continuous acceptance test.

**The ONE remaining error**: book_assets/capsula_libri.c
(12.8MB generated embed blob) — one clean ERROR fracture whose
raw capture starts at capsula.h's enum; chain and content
bisect CLEAN at 780KB (mini-repro with the full include chain
parses); scale probe at 12.8MB pending. Roundtrip FIDELIS even
with the error. Disposition: generated-blob scale class, named;
exact trigger = open question for the lustrum/census toolbox,
not an M2 blocker. arbor2_glr_tabula.c reports SCRIPTURA
FRACTA (pasta deferral) — dispositioned Chunk C, not an error.

**Named for saltuarius v0.1 (not done here)**: nexus still
seeds latinam_addere unconditionally (saltuarius_nexus.c:282) —
correct for latina daily driving, wrong for knotapel browsing;
the include-driven model percursus now demonstrates is the fix
shape, and the basename-recidiva already helps its praebenda.

NEXT: D4 — M2d RELATIO + FULL-log audit + M2 CLOSE.

### Addendum Chunk D — ZEPHYRUM (2026-07-04)

**THE M2 NUMBER: 724/724 plagulae, 0 nodi ERROR in toto
monorepositorio.** Totalitas TENET, 723/724 octetim fideles
(the one exception = arbor2_glr_tabula.c's pasta deferral,
correctly labeled SCRIPTURA FRACTA, not an error), inclusio
vera, no heuristic, no ceilings, 3.86 ms/KB, RSS 6.8 GB.
M1 recorded 41,000 ERROR nodes. M2 closes at ZERO.

**The last error's disposition (capsula_libri.c)**: the
synthetic scale probe reproduced it exactly (content-innocent);
the trigger is SILVA_LIMEN_LEXEMATUM_DEFALTUM = 1,048,576 — the
expanded-stream ceiling, a DESIGNED graceful degradation
(fines_tactae -> expansion decides -> latina unexpanded ->
one clean fracture, roundtrip FIDELIS throughout). It was
firing SILENTLY — against the house rule. Fixed in the
instruments, not the engine: percursus prints [fines tactae]
per file + a summary count (degradatio picta, non silens), and
-omnia now lifts ctx->fines.lexemata too (it already promised
"sine tecto - cave!"); infidelis gained the same report +
-omnia. The 1M default STAYS for editors (saltuarius) — it is
the protective envelope, now visible when it fires.

REMAINING: D4 only — M2d RELATIO + FULL-log audit + M2 CLOSE.

## M2d — THE ACCEPTANCE WALL: RELATIO (2026-07-04) — COMPLETE

**Every INTENTIO exit criterion met, and the wall's number is
ZERO.** The monorepo sweep: 724/724 plagulae, totalitas TENET,
0 ERROR nodes, 723/724 octetim fideles (the 1 = the pasta
deferral, correctly labeled), inclusio vera (no lexicon
heuristic), no size ceiling, no expansion ceiling, 3.86 ms/KB.
Suites: silva 27/27 (probatio_silva_c89 1,554), saltuarius
13/13, tessera 5/5, amalgam VERIFICATUM (hospes 24/24). All
four evidence counts logged with dispositions; badge shipping;
sceletum out of the amalgam; carried integration bar green.

**The chunks, walked**:
- **A — expander-fed c89**: silva_c89_parsare_cum_contextu;
  #pragma became a true directive (the last corpus error);
  corpus ERROR 42 → 0 PINNED (murus acceptionis CREDO);
  saltuarius TOC rides nexus->ctx. **A+**: fons_princeps filter
  on all four c89 TOC sections (Fran's include-flood question —
  the leak was live).
- **B — the main-parse swap**: ONE call site; the saltuarius
  suite passed FIRST run (downstream truly grammar-independent);
  TOC second parse DELETED; sceletum OUT of the deliverable
  (generator-fixture role stays).
- **C — percursus + the census arc**: 723-file sweep instrument;
  arena pathology RESOLVED (census-lite fingerprinted 84% of
  apex = per-reduction viae scratch; two fixes, 6-7x, trees
  byte-identical; biblia_dr 49.2 GB → 6.7 GB); INFIDELIS closed
  as the pasta deferral (instrument now distinguishes SCRIPTURA
  FRACTA from octetim divergence); infidelis.c promoted to
  instrumenta; carried EXPANDERE bar applied (17 composite
  inputs, first-run green); the complete error census — every
  one of 31 errors named into 3 classes with dispositions.
- **D — the close**: LL/ULL greedy suffix run (the lexer was
  the only layer STRICTER than C's own; Fran-approved);
  inclusio vera in percursus (praebenda by basename from repo
  root; heuristic deleted) + **basename-recidiva in the
  expander** (relative-path include writings broke whole chains;
  exact-first, basename-second, conservative on both-miss —
  saltuarius upgraded free); the ambient badge (err N in the
  status line ONLY when nonzero + always in F2); the 1M
  expanded-stream ceiling made LOUD ([fines tactae]) and
  liftable (-omnia), default kept as the editor envelope.

**Dispositions of record**: typedef-in-corpore 11 sites, all
oracle-absorbed — decisiones 13 deferral STANDS. Duffiana real
demand 0 — the grouped-switch price is nothing. gcc-in-errore
0. struct-tag-const + nuda divergences: zero errors monorepo-
wide = no in-repo demand, divergences stand on evidence. Pasta:
in-repo demand = 1 legacy generated file; the M1 fail-loud
deferral stands, now with its cause printed by every instrument.

## M2 — FULL-LOG AUDIT (2026-07-04, the phase-boundary ritual)

Whole log re-read (5,205 lines, Phase 0 → M2d). Every
complexity traced to a resolution or a park with a named
landing spot. The catches (the class this practice exists for):

1. **"Config-query milestone" vs the post-M2 map — unreconciled
   names.** A dozen parks land on "the config-query milestone"
   (untaken-arm sub-parsing at taken-arm fidelity, structural
   scan of untaken arms, config-aware journal replay, lazy
   false-arm includes, named config presets, wildcard
   configuration queries, transforms-across-arms fidelity,
   macro-expanded #include operands) — but the post-M2 map
   (spec-v2 §4.1) names only quaestio / formator / typus+index.
   RESOLUTION: config-query is hereby recorded as a NAMED
   MILESTONE in the post-M2 map (position flexible — likely
   quaestio-adjacent, since wildcard queries are its face);
   its park inventory is the list above. FRAN DECIDES its
   sequencing when post-M2 planning starts.
2. **STML serialization was an orphan.** "silva_stml — own
   module, 1.0 scope" (Phase 5/7 NOT-lists) and the VISIO
   oracle code→STML→load→emit==code never entered the post-M2
   map. RESOLUTION: named into the map alongside the §4.1
   milestones — it is 1.0 scope by spec; FRAN DECIDES where it
   lands in the sequence.
3. **The stderr diagnostics hook was an orphan** ("post-M1 API
   nicety", Phase 6 — never mentioned since). RESOLUTION:
   parked ON-PULL — the first embedded host that cannot
   tolerate fprintf(stderr) owns it (solarium integration is
   the likely demander).
4. **Pater on genuinely shared subtrees** (Phase 4 Chunk C
   named limitation: single-owner by walk order, "C89-grammar
   revisit") was never explicitly revisited. Evidence: no M2
   fixture or corpus run ever surfaced a pater anomaly; the
   canonical-walked-LAST insurance held silently. RESOLUTION:
   re-parked with a real gate — the first QUERY consumer that
   walks pater across interpretations (quaestio milestone)
   owns the revisit.
5. **est_numquam as queryable genus** (Phase 7, "journal-side
   for now") lacked an owner. RESOLUTION: quaestio milestone
   (it is a selector-tag question).
6. **The v1 syntaxis suite is now deletable** per the
   freeze-then-delete rule (its bar was harvested: corpus file
   + 123/125 comprehension pin at M2c). Nobody noticed the
   rule fire. FRAN'S CEREMONY — quaestio/formator/typus/index
   v1 suites remain frozen, their milestones unbuilt.
7. **Honest status on two never-exercised engine paths**
   (Phase 4, defensively implemented): 3-way runtime AMBIGUUS
   retention (F4 triple cells EXIST in the c89 tables; a
   three-interpretation wrapper was never fixture-pinned) and
   the multi-accept root wrap (still unreachable in every
   shipped grammar). Both keep their defensive code and their
   gates; a fixture pinning F4 three-way retention is a cheap
   M3-era addition, named.
8. **Hospes backfill debt restated** (ADDITIONES I): the
   pre-existing silva.h declarations without hospes calls are
   still owed opportunistically; every NEW declaration since
   has obeyed the standing rule.

Healthy parks confirmed without action (each with its gate):
action-filter pruning hook (frons never left 2-3; mensura
gates); identity-handle refactor (eventa_marginis_novi never
fired hot); OCTETUS run-coalescing (mensura never asked);
Prosser simplification (permanent, evidence-cited); nuda
tag-final + struct-const divergences (zero-error sweep =
evidence); K&R false-suspicion coarseness (benign by
construction); accessor/enum public exposure (on-pull);
M2d-era parks as recorded in their entries (arena tiers 1-3,
saltuarius include-driven nexus, per-stratum TOC,
est_prototypum, marker provenance fons, X-macro origo
refinement, TOC section order). Zero unowned complexities.

## M2 — GRAMMATICA C89: CLOSE (2026-07-04)

**M2 IS CLOSED.** One day of sessions (2026-07-03 sketch →
2026-07-04 close), five phases (M2.0 NOMINA → M2a expressiones
→ M2b declarationes → M2c sententiae+functiones → M2d the
acceptance wall), two INTENTIO-reversing simulations (X, plus
the M2c one-lista amendment that removed a sim's need), and
the arc of the honest counter:

**ERROR nodes over the real corpus: 41,011 (M1, recovery) →
761 (M2a) → 62 (M2b) → 42 (M2c) → 0 (M2d, comprehension) —
and 0 over the ENTIRE 724-file monorepo with real include
resolution.** Comprehension 0 → 123/125 (remainder: 2
gcc-extension lines, evidence-gated). frons_maxima 2 → 3
against a ceiling of VIII — the fork-explosion risk died of
measurement. Byte-exact fidelity: 723/724 (the 1 = pasta,
labeled), through every fix, every sweep, every fuzz specimen.

**What M2 leaves standing**: ~55 permanent genus names
(DECISUS, the thousand-year vocabulary); a 252-production
grammar whose every conflict cell is named into 6 families +
1 praelata, pinned exhaustively; the oracle dance (positional,
X10-filtered, policy-passed) reaching inside function bodies;
a boundary-finder whose false suspicions are provably coarser-
never-wrong; the saltuarius TOC (4 sections) + subscriptio +
ambient badge as the continuously-running acceptance test; and
the deliverable amalgam carrying c89 ONLY (sceletum retired to
generator fixture).

**Two generator bugs killed** (multi-line atom loss; non-
transitive lista-valens) — both would have silently corrupted
every future grammar. **Two instruments born** (percursus,
infidelis) and one census technique (-DPISCINA_DEBUG histogram)
that resolved a 49 GB arena pathology in an afternoon. **The
stale-object hazard class** got its guard into every build
script.

**POST-M2 MAP (amended by this audit)**: quaestio selectors /
formator / typus+index (frozen v1 bars, spec-v2 §4.1) + the
CONFIG-QUERY milestone (park inventory above) + STML
serialization (1.0 scope, placement = Fran's call). Sequencing
decision belongs to the next planning session, with Fran, on
pull — the same rule that timed M2 correctly.

Ad numeros. Deus vult.

## Addendum post-M2 — RECENSIO POSITIONIS (2026-07-04)

Fresh-eyes positioning review with Fran (full reasoning preserved:
project-specs/silva-positio.md — the from-scratch description, the
landscape triangle [clang/tree-sitter/srcML — silva occupies the
unoccupied center: compiler-correct trees AND document-exact
surface], the Roslyn-for-C framing, honest gaps). Seven roadmap
items fell out — rough edges where a strength is 90% built. Each
now NAMED with a disposition:

**No-pull-needed tier (trust / pin-drift / foreclosure — do at the
next silva reopening, before or alongside any milestone):**
1. **Diagnostica ex tabulis** — expected-terminal sets surfaced on
   ERROR nodes (the failure state's action row already enumerates
   them; baked data, currently unread) → infidelis + badge gain
   "why". Small; ADDITIONES-class package.
7. **Incrementality foreclosure CHECK** — one page of analysis on
   absolute byte offsets vs segment-level reparse, BEFORE more
   consumers pin position semantics. Analysis only; the
   predecessors' death-class demands the path back be verified in
   writing.
3. **Comment model ownership (AUDIT REPAIR)** — the 2026-07-04
   full-log audit MISSED that the comments-as-queryable pin
   (CLAUDE.md's "WATCH THIS ONE") has no landing spot in the
   post-M2 map; consumer smoothing already began (subscriptio
   trims comments by hand). HEREBY ADDED to the post-M2 map as
   quaestio-adjacent scope ("commenta" — attachment heuristics,
   own-line/end-of-line/freestanding, doc-comment patterns).

**On-opportunity tier (cheap, named, grab when adjacent):**
2. **Politica minimalis triviorum synthesizatorum** — a default
   spacing policy (space between tokens, newline after ;/}),
   deliberately NOT the formatter: unblocks layer-N text views,
   node-inserting transforms, generated-code emission, silva.h
   generation — four parked items behind one small policy. The
   formatter milestone keeps ownership of doing it WELL.
5. **Signa normae in lexatione** — populate the token
   standard-tag AS constructs are recognized (LL/ULL = the first
   customer; the M2d D1 promise pre-paid), so lint's first query
   works on day one.
6. **Graphum inclusionum** — percursus flag dumping the
   monorepo include graph from the already-collected inclusiones
   records. Cheapest instrument available.
4. **Fuzzatio industrialis** — libFuzzer/AFL harness on the
   amalgam (single .c + binary oracle = ideal target); occasional
   overnight runs; failures harvested as fixtures. Instrument,
   not suite (the suite keeps its deterministic 400).

**POST-M2 MAP, as now amended**: quaestio selectors / formator /
typus+index / config-query / STML serialization / **commenta
(quaestio-adjacent)** — plus the seven positioning items above
(three no-pull, four on-opportunity). Sequencing stays Fran's, on
pull, at the next planning session.

## Addendum — INCREMENTALITAS: the foreclosure check EXECUTED
## (2026-07-06) — and it caught a real bug

Recensio item #7 discharged: project-specs/silva-incrementalitas.md
is the page of record. VERDICT: **nothing forecloses segment-level
incrementality.** The position-consumer census (verified against
source) shows every offset comparison is intra-parse; offsets under
an edit are a mechanical O(tokens) rebase, NOT the constraint. The
REAL constraints, ranked: (1) arena lifetime granularity — one
piscina per parse; keeping old segments' trees across reparses
wants per-segment or generational arenas; nothing forecloses this
but any arena refactor must consult it FIRST; (2) whole-file
artifacts (strata/regions/directivae) — regenerable, bookkeeping;
(3) token-pointer identity mixes generations under splicing —
noted. The journal + positional oracle already ARE the
invalidation predicate (macro state + typedef set at a segment
boundary). Invariants to preserve are listed in the doc.

**THE FINDING (the census earning its keep): the positional oracle
is FONS-BLIND — a real, live comprehension bug.** situs (typedef
byte_offset) and situs_ambigui (use byte_offset) are per-fons
rulers compared as one: an included typedef sitting LATE in its
header (offset > the use's main-file offset) is judged "declared
after use" → the fork is wrongly RETAINED, expression-canonical —
no error, no badge, no TOC row; invisible to the zero-error wall
BY CONSTRUCTION. Probe: identical include, typedef at pad.h
offset ~0 resolves, after a 3KB comment it doesn't.

**Census** (percursus grew an ambigui counter — permanent product
number, "counters are product"): baseline 17,607 retained ambigui
in 497 plagulae; positional check experimentally disabled → 15,225
in 407. **Delta = 2,382 wrongly-retained readings in ~90 files**
(upper bound for the class). Remainder = honest retention (genuine
C ambiguity + unknowable system-header typedefs). Experiment
REVERTED; suite green; the counter change is the only oraculum-
adjacent code kept... (counter lives in percursus only).

**Second finding, same family**: lexicon-path files (latinam_addere)
never register their typedefs AT ALL — lexica feed the macro table;
their non-directive content is not parsed. Include-driven contexts
are unaffected (latina.h parses when truly included), but
saltuarius's latinam_addere-only nexus never learns i8/s32 as
typedefs. Reinforces the named v0.1 include-driven-nexus item.

**FIX OPTIONS (decision owed to Fran, recorded in the doc)**:
(a) cross-fons ⇒ visible — small, fixes the 2,382, ADDITIONES-class,
pairs naturally with the table-diagnostics package; (b) stream-order
situs — principled, correct for every shape, natural at typus/index.
Recommendation: (a) now, (b) at the milestone.

## ORACULUM SANATIO — visibilitas per ORDINEM, non per situm:
## INTENTIO (2026-07-06)

The fons-blind fix, done as the C-semantic redesign (option c of
silva-incrementalitas.md §V — Fran approved): **the positional
coordinate dissolves entirely.** Registration currently runs
AHEAD of resolution, so a byte-offset comparison simulates "was
it declared yet?" — per-fons rulers, 2,382 wrongly-retained
readings. The fix: ONE pre-commit walk in stream order that
INTERLEAVES — registers typedefs as encountered, and when it
meets an AMBIGUUS examines it AGAINST THE ORACLE'S CURRENT
CONTENTS (which at that moment ARE the visible set) and stashes
the verdict. Declaration-point visibility, ISO's own semantics,
with LESS machinery than today.

**Design (source-verified)**:
- SilvaOraculum grows `Xar* responsa` ({sedes, victor} incl.
  victor -1 — "walked and retained" is distinct from "never
  walked"); vacare at walk start (oraculum reuse across parses =
  node-pointer collision hazard otherwise); ponere/quaerere pair
  in silva_commissio (linear scan; ambigua per file are dozens).
  Struct is silva.h-OPAQUE — no public surface changes.
- silva_c89_praecommissio → the interleaved walk: generic loci
  descent (slot order = stream order, the slot-order validation's
  dividend); DECLARATIO registers unless sine_registratione;
  DEFINITIO_FUNCTIONIS/CORPUS descend with sine_registratione
  VERUM (decisiones 13 unchanged); AMBIGUUS → examine-now +
  stash, then descend ALL interpretations sine_registratione
  (X4 unchanged; nested ambigua get verdicts too);
  ramus-omissus naturally skipped (cruda = tokens).
- silva_c89_resolutor = stash lookup; miss → position-free
  examination (direct-caller fallback; no in-tree path misses).
- The X10 filter, sole-survivor, unique-type-positive ladder:
  UNCHANGED — only the visibility predicate moves from
  `situs <= situs_ambigui` to walk order. situs stays recorded
  (public API compat; data, no longer consulted).
- silva_c89_typos_registrare keeps its old registration-only
  behavior (public API).

**Expected**: X3 fixture (typedef-after-use retention) passes BY
ORDER; the fons-blind class (2,382) collapses; include-after-use
correctly stays retained. New fixtures: praebere'd late-in-header
typedef resolves (the probe becomes a pin); include-after-use
honesty pin. Gates: suite + amalgam + saltuarius + tessera + the
census sweep (acceptance number ~15,225).

### ORACULUM SANATIO — RELATIO (2026-07-06): SHIPPED

**Suite 27/27 (probatio_silva_c89: 1,578 asserts — sanatio
fixtures added); amalgam VERIFICATUM (hospes 24/24); saltuarius
13/13; tessera 5/5. THE ACCEPTANCE NUMBER: monorepo retained
ambigui 17,607 → 15,229 (2,378 wrongly-retained readings FIXED),
errores still 0, fidelity unchanged.**

**The detail that proves the design**: the blunt experiment
(visibility check disabled) predicted 15,225; the real fix lands
at 15,229. The four-reading difference IS the X3 honesty the
experiment trampled — same-fons typedef-after-use shapes that
brute visibility wrongly resolved and the order-walk correctly
retains. The fix is strictly more honest than the measurement
instrument.

**What shipped**: the positional coordinate DISSOLVED —
- SilvaOraculum += responsa (verdicta praecomputata; {sedes,
  victor}, victor -1 = retention DECIDED, distinct from
  never-walked; vacare per walk — node pointers can collide
  across parses on arena reuse); ponere/quaerere in commissio.
  silva.h untouched (opaque type).
- silva_c89_praecommissio = the INTERLEAVED walk (_percurrere):
  generic loci descent in source order; DECLARATIO registers
  unless sine_registratione; DEFINITIO_FUNCTIONIS/CORPUS set
  sine_registratione (decisiones 13 intact); AMBIGUUS →
  examine-NOW against current oracle contents + stash verdict,
  then descend all interpretations sine_registratione (X4
  intact; nested ambigua get verdicts at the same stream
  position); ramus-omissus skipped (cruda).
- _specificatores_examinare: "notum ad situm" → typum_novit
  (the oracle's current contents ARE the visible set).
- silva_c89_resolutor = stash lookup; miss → position-free
  examination (direct-caller fallback; no in-tree path misses).
- situs stays RECORDED (public API compat, useful data) but is
  no longer consulted; header docs amended in both modules.
- Fixtures: late-in-header typedef via praebere RESOLVES
  (ambigui 0, DECLARATIO — the probe became a pin);
  include-after-use RETAINED (ambigui I — the honesty pin);
  X3 same-fons fixture passes unchanged (semantics preserved,
  mechanism now order).

**The whole suite passed FIRST RUN after the redesign** — the
X3/X4/X10/dance semantics carried through the mechanism swap
untouched, which is what "the ladder is unchanged, only the
visibility predicate moved" was supposed to mean, and did.

Named remainder (unchanged): the LEXICON-path finding (lexica
never register their typedefs — latinam_addere consumers) rides
the saltuarius include-driven-nexus item; stream-order situs as
a public coordinate (option b) stays named for typus/index if
scope-aware tables ever want it — likely mooted by this design.

## QUAESTIO — the selector surface: DESIGN DECISUS + milestone
## sketch (2026-07-06)

**The pull fired**: Fran chose quaestio design for the remaining
high-judgment hours. Method = M2.0 NOMINA (draft-then-correct).
**project-specs/silva-quaestio-design.md is the DECISUS** — the
selector surface's permanent names, decided with Fran (all four
questions on the recommendation):

1. Pseudos speak LATIN (:vocat/:definit/:utitur/:reddit/:habet/
   :non/:primus/:ultimus + silva-axis :ambiguum/:lectiones/
   :sumptus/:omissus; norma family :c89/:c99/:extensio gated on
   standard-tag population — recensio #5 is now PULLED by a real
   consumer).
2. Attributes = LOCUS NAMES VERBATIM ([tok_titulus="main"]) — the
   annotation format's slot vocabulary IS the attribute surface.
3. Ambigua: CANONICAL default; :lectiones opt-in widens;
   :ambiguum matches wrappers. Conditional arms: taken arms
   matched structurally (wildcard pin); untaken = config-query
   milestone BY NAME.
4. Scope: SELECTORS ONLY — transform DSL is the follow-on that
   targets them; formator rides transforms.

**Grounding done**: v1 arbor_quaestio read (model carried:
compiled query, chains, {nodus,capturae}, = ^= $= *=, custom
pseudos; four catalogued bugs DESIGNED OUT: mid-chain captures
kept, results deduped by subject, attribute paths through the
registrum tables, custom pseudos functional); the 100-assertion
v1 bar = acceptance, inputs TRANSLATED to DECISUS genera at
harvest (lapifex-bar precedent); unknown tag = LOUD compile
fractura (v1 matched silently — deliberate behavior change,
recorded). Engine is GRAMMAR-AGNOSTIC (compilare takes the
registrum; works on sceletum and c89 alike). children()/uniform
loci make v1's hand-built _iterare_liberos free.

**Chunk sketch (INTENTIO to be written at implementation start,
per protocol)**: QA parser+compilare+congruit+exsequi (genus,
space/>) → QB attributes+captures-fixed+siblings+dedup → QC
pseudos (structural, silva-axis, semantic, custom) → QD the
harvested bar + new-axis fixtures + silva.h + hospes + amalgam;
saltuarius ':quaestio mode' = v0.1 candidate BY NAME (in-phase
only if best instrument). Norma pseudos in-scope iff standard-tag
population lands cheaply (else named).

## QUAESTIO QA — parser + compilare + exsequi core: INTENTIO
## (2026-07-07)

Scope (per the milestone sketch + DECISUS doc): the selector
IR + tokenizer/parser for the QA subset (genus tags, `*`,
descendant space, child `>`, union comma), compilare against a
REGISTRUM (grammar-agnostic — the module includes NO c89
headers; unknown tag = NIHIL + causa out-param, the loud-
fracture DECISUS), congruit (full-chain test via pater —
committed trees documented as the contract), exsequi (top-down
enumerate, each node tested once as SUBJECT = rightmost
compound; ancestors matched right-to-left via pater with
recursive retry on descendant hops; AMBIGUUS descends CANONICA
only — form pre-resolved from the registrum at compilare, the
commissio pattern; results = Xar of SilvaQuaestioResultatum in
caller piscina). IR carries the QB/QC part kinds now (attributum/
pseudo/captura enum members exist unparsed) — API-first, the
extension points are in the shapes from day one.

Exit: probatio_silva_quaestio green (tags, *, both combinators,
union, unknown-tag fracture, congruit, error-genus matchability,
ambiguus-canonical descent fixture); suite green. silva.h +
manifest + hospes = QA-closing act (standing rule) — if the
session ends first, they are THE NEXT ACT by name.

### QUAESTIO QA — RELATIO (2026-07-07): core SHIPPED

**Suite 28/28 (+probatio_silva_quaestio, 38 asserts FIRST full
run).** fontes/silva_quaestio.{h,c} (~560 lines): selector IR
(partes/gradus/catenae — QB/QC part kinds present in the shapes,
unparsed), hand lexer/parser for the QA surface (tags, *, spatium,
>, virgula), compilare registrum-driven with causa out-param
(unknown tag / malformed = NIHIL + causa — six fracture shapes
pinned), congruit (right-to-left chain via pater; descendant hops
retry recursively so "a b c" cannot miss high-ancestor matches),
exsequi (top-down enumerate, each node tested once as subject —
duplicates structurally impossible; results in caller piscina).
AMBIGUUS descends CANONICA only (form pre-resolved at compilare,
commissio pattern) — pinned: "Ignotus * x;" matches binarium/
sententia-expressionis, NOT declaratio, and the wrapper itself
matches tag "ambiguus". ERROR nodes match tag "error". The module
includes NO c89 header (grammar-agnostic by construction); the
probatio drives it through silva_c89_parsare.

The i32-unsigned slip fired on its own author in the probatio
(sign-conversion wall caught it — the codebase's most reliable
trap keeps its record).

**NEXT (named, the QA-closing act)**: manifest + silva.h + hospes
for compilare/exsequi/congruit + the result struct (standing
rule); then QB (attributa [locus op "valor"], capturae $nomen
with all-hops binding, fratres +/~) → QC (pseudo-classes) → QD
(the translated v1 bar).

## OFFICINA PRE-M1 + QUAESTIO CLAUSURA QA — INTENTIO (2026-07-08)

Context: officina specs sealed (project-specs/officina-brainstorm.md +
officina-spec-v2.md — the compiler/VM/debugger toolchain, silva-fed;
medulla/machinula/vindex). The two named pre-M1 tasks land HERE
(silva-side). Folded in: the QA-closing act (quaestio manifest +
silva.h + hospes — the named next act) because it touches the same
three files; leaving its standing-rule debt open while editing them
would be false economy.

Scope, three chunks:

1. QUAESTIO CLAUSURA (the QA-closing act): silva_quaestio.h refactored
   to named-tag SilvaQuaestio (opaque in silva.h — SilvaOraculum
   precedent; SilvaQuaestioResultatum stays transparent, silva.h owns
   it); amalgamator manifest += silva_quaestio.{h,c}; CADENDA_TYPEDEF
   += SilvaQuaestio, SilvaQuaestioResultatum; silva.h quaestio section
   (compilare/exsequi/congruit); hospes REAL calls with content asserts
   (parse fixture -> compilare -> exsequi count -> congruit yes/no ->
   unknown-tag fracture with causa).

2. SUPERFICIES PUBLICA GENERATA (officina pre-M1 task 1, spec-v2 §II
   decision 1 DECISUS): coquere emits the c89 public region — genus
   enum + all accessor prototypes in VANILLA spelling (const, enum) —
   SPLICED between markers into amalgama/silva.h and into hospes.c
   (typed fn-ptr table). Internal generated enum gains a shared guard
   (SILVA_C89_GENERA_CUSTOS) so in the amalgam the internal copy
   no-ops after silva.h's copy; duplicate prototypes are legal C89 and
   expand identically. TWO PROCESS REFINEMENTS RECORDED: (a) the
   silva.h "MANU SCRIPTUM" pin gains ONE generated marked region — the
   pin's PURPOSE (vanilla C89, zero latina leakage, hospes-enforced)
   is unchanged, and drift becomes structurally impossible because the
   same generator writes both copies (hand-maintained duplication of
   58 enum constants would be silent-catastrophe drift: stale silva.h
   copy wins in the amalgam via the guard); (b) the hospes standing
   rule, for the GENERATED accessor family only, is discharged by a
   typed function-pointer table (assignment enforces exact prototype
   match at compile time = the decl/def-mismatch class the rule
   exists for; reference forces link resolution) — hand-written
   functions keep real calls with content assertions. Generator run:
   generare.sh passes the two splice paths for the c89 grammar only.

3. EXPANDER PRAEDEFINITA (officina pre-M1 task 2). Audit result:
   # stringificatio and ## pasta ARE implemented (Chunk C,
   _stringificare); __FILE__/__LINE__ are ABSENT — they pass through
   as plain identifiers. Parse-fine (that is why the corpus is green),
   execution-LIE (credo's CREDO_* macros bake __FILE__/__LINE__ into
   every assertion; officina's corpus bar would report wrong
   positions). Implement as predefined object-macros at lookup:
   __FILE__ -> STRING_LIT of the ROOT token's fons via (silva_token
   _radix = invocation site), __LINE__ -> INTEGER of root token's
   linea (correct C89 invocation-line semantics via provenance — the
   origo chain pays again), __STDC__ -> 1. __DATE__/__TIME__ DEFERRED
   BY NAME: determinism (officina time-travel/replay wants no wall
   clock inside expansion); land when pulled, likely as fixed-value
   contextus option. Byte-exact emission unaffected (scriptura emits
   from fons text); expanded strata become TRUTHFUL.

Exit: suite green; amalgamare VERIFICATUM; percursus 724/724 zero
errors preserved; generare.sh idempotent (regenerated tables byte-
identical modulo the new guard + splice regions).

### OFFICINA PRE-M1 + QUAESTIO CLAUSURA — RELATIO (2026-07-08): ALL SHIPPED

**Chunk 1 — QUAESTIO CLAUSURA QA.** silva_quaestio.h: SilvaQuaestio
refactored to named-tag (SilvaOraculum pattern); manifest +=
silva_quaestio.{h,c}; CADENDA_TYPEDEF += SilvaQuaestio,
SilvaQuaestioResultatum; silva.h quaestio section (opaque handle +
transparent result + three prototypes, vanilla); hospes: real calls
with content asserts (parse fixture, definitio-functionis count,
corpus>declaratio count, congruit yes/no, unknown-tag fractura with
causa). Suite 38/38 unchanged; the QA standing-rule debt is CLOSED.

**Chunk 2 — SUPERFICIES PUBLICA GENERATA.** coquere: internal enum
now guarded (%PRAEFIXUM%_GENERA_CUSTOS, uniform all grammars);
NEW silva_gen_splicere_publica (piscina, grammatica, praefixum,
via_capitis, via_hospitis) - emits the c89 public region VANILLA
(guarded enum + 176 accessor prototypes with const spelling) spliced
between ">>>"/"<<<" markers in amalgama/silva.h, and the
ACCESSORES[176] table into hospes.c; markers absent = loud FALSUM.
generator.c: optional argv 4/5; generare.sh passes them for c89 only.
IDEMPOTENS verified (second run byte-identical). NOTE: INTENTIO
refinement (b) - typed-table-instead-of-calls - was NOT needed:
generated accessors are genus-checked and NULL-safe (NULL ->
SILVA_VALOR_NIHIL), so hospes CALLS all 176 with NULL and asserts the
nihil valor - the standing rule holds LITERALLY. Refinement (a)
stands as recorded: silva.h carries one generated marked region;
the MANU SCRIPTUM pin's purpose (vanilla, zero latina) is untouched
and hospes still enforces it.

**Chunk 3 — EXPANDER PRAEDEFINITA.** silva_token.{h,c}: NEW
constructor silva_token_ex_praedefinito (genus-parametrized synthetic
literal; ORIGO_CHORDA with primus = invocation token - radix/
profunditas walk it). silva_expandere.c: _praedefinitum_substituere
in _generatio_interna after user-table miss (tabula usoris VINCIT);
all three names are exactly VIII chars starting "__" - cheap reject.
__LINE__ = radix->linea (invocation-line semantics inside macro
bodies via the origo chain - provenance pays again), __FILE__ =
silva_fons_via(radix->fons_index) quoted+escaped, __STDC__ = 1.
Conditio (#if) covered FREE: silva_conditio.c:734 calls
silva_expansio_expandere. __DATE__/__TIME__ deferred by name
(determinism; INTENTIO). KNOWN LIMIT recorded: defined(__FILE__) in
#if still evaluates false (defined inspects the user table only);
corpus audit found zero uses; landing spot = officina M0 if ever
pulled. Tests: +16 asserts in probatio_silva_generatio (54 total,
first-run green): direct lines, macro-body invocation line, __FILE__
literal, __STDC__, user-redefinition-wins, and the ADFIRMA
credo-shape (#x + __FILE__ + __LINE__ together - the officina corpus
bar's exact pattern).

**Gates.** Suite 28/28; amalgamare VERIFICATUM (hospes 27/27,
standalone plena severitas, nm-intersectio 0); percursus: 725
plagulae (corpus +1 since census), ERRORES 0 (wall holds), 724/725
octetim - the one is ./lib/arbor2_glr_tabula.c, the PRE-EXISTING
named pasta deferral (verified by name); ambigui 15,243. Every
credo-using file now truly expands __FILE__/__LINE__ and remains
byte-exact (emission from stratum 0 as designed).

**Officina pre-M1: BOTH named tasks DONE.** The compiler's next act
by name: M0 semantica INTENTIO (silva-side typus+index - scoped
symbol tables, types, layout, clang sizeof/offsetof oracle;
officina-spec-v2.md §VI is the grounded plan).

## M0a SEMANTICA — INTENTIO (2026-07-08)

The spec is project-specs/silva-semantica-design.md (DECISUS §I: all
eleven interview answers; V2 grounding §§VII-XI: ambigui census,
empirically-settled systema wiring, typing-walk contract; §XI all
three sign-offs APPROVED by Fran 2026-07-08). This INTENTIO is the
build plan; the design doc is the why. Officina context:
officina-spec-v2.md §VI — M0a is the compiler's first dependency AND
silva's own typus+index milestone.

SCOPE (M0a): scoped symbol tables + real types + host-faithful
layout + haruspex (clang layout oracle) + project index + the
ambiguity closure with TRIVALENT oracle + systema. M0b (expression
typing + conversions) is separate and follows.

CHUNKS:

**A — typus + forma fundamenta.** TypusC89 representation: interned
derived types (monstrator/acies/functio/qualificatus wrappers -
single-pointer handles, pointer equality), nominal tag types
(identity per declaration site), canonical primitive table (14
multisets; char/signed char/unsigned char THREE types). Declarator-
chain construction (internum unwind; abstract = bottoms at NIHIL;
parenthesis transparent). Integer constant-expression evaluator
(aestimator) sharing one context with forma (lazy layout, arm64
LP64, mutual recursion by design). Module boundaries may fold
typus/forma into one TU if the shared context demands - the API
names are the contract, not the TU count. Bar: interview-pinned
fixtures green (qualifier-through-typedef = int* const, the three
chars, incomplete-tag completion, self-referential struct, enum
constant into ordinary namespace, array-size consteval, poison-type
non-cascade).

**B — scopus + the declaration walk.** Scope stack, four namespaces
(ordinaria incl. enum constants + typedefs; tags with completion
pointers; membra; labels). Whole-file source-order walk (the sanatio
invariant: register-before-use in ONE pass, never compare
byte_offset across fontes; recurse conditionalis/ramus-sumptus
contentum, skip ramus-omissus + ERROR; AMBIGUUS descends canonica -
and canonical is the EXPRESSION reading by policy, never assume
declaration). Contract facts honored per design §IX (tok_aperta
def-vs-ref discriminator; declaratio-nuda zero declaratores;
implicit-int NIHIL specificatores; K&R park). Poison diagnostics
table. Percursus -semantica flag (DEFAULT OFF; baseline 15,243
stays pristine). Bar: corpus sweep - every file analyzed,
diagnostics 0, symbol/type totals reported.

**C — systema + oraculum trivalens + closure.** Author
fontes/systema_c89.h (the FULL ISO C89 library surface, hand-written
committed text fixture; plain .h, NO datum until machinula M2 needs
embedding). Oracle preload (typedefs via
silva_oraculum_typum_addere_literis-style table - the proven
channel; lexica channel PROVEN INERT for typedefs, design §VIII).
Channel B: parse systema once per SilvaSemantica session, harvest
declarations into semantica tables (user parses stay pristine -
systema is its own queryable layer). TRIVALENT oracle upgrade:
SilvaOraculum learns NON-types (non_typum_addere + trivalent query);
resolutor gains the kill-declaration-reading rule for known
non-types; non-type harvest from the declaration walk (registration
locus - precommit interleaved walk vs post-commit closure - is an
implementation decision inside the chunk; the bar only measures the
end state). Closure via silva_recanonicare (datum = semantica
tables; SilvaResolutor interface UNCHANGED). Bar: ambigui 15,243 ->
the named-park residue (nested-cast class ~2% + genuinely
ambiguous), measured by percursus/census_ambigui before/after;
every residual classified.

**D — haruspex + index + publica.** Haruspex instrument
(instrumenta/principalia/haruspex.c + haruspex.sh): walks the type
inventory -> ONE generated TU of compile-time asserts (sizeof +
offsetof per DIRECT member; C89 has no anonymous members, nesting
composes), partitioned by PROVENANCE (repo types = clang truth;
systema types = opacity contract only). Compile IS the verdict.
Project index API (in-memory; saltuarius type-jump = first named
consumer, wiring is saltuarius v0.1's pull). Public surface:
silva.h rows + hospes calls (standing rule) + amalgam manifest +
VERIFICATUM. M0a RELATIO + FULL phase-log audit (phase boundary).

PARKS (named, landing spots): initializer type-CHECKING -> M0b;
nested-cast resolver class (~2%, 34 instances) -> post-M0a resolver
look; K&R full typing -> on-pull (corpus cannot contain it);
block-scope haruspex coverage -> replicate-under-synthesized-tag
path back if a block-scope layout bug ever appears; semantica
lifetime/invalidation -> M4 + incrementalitas journal;
est_syntheticus reader -> when hiding synthetic decls becomes real.

RISKS WATCHED: arena scale on the corpus sweep (percursus apex
3.4GB precedent - per-file piscina destroyed per file); the
canonical-is-expression trap; lista-prospectus discipline
(silva_valor_lista_* ONLY); walk-order visibility.

Exit: chunks A-D bars + suite green + amalgamare VERIFICATUM +
percursus baseline unchanged with flags off.

### M0a CHUNK A — RELATIO (2026-07-08): typus + forma + aestimator SHIPPED

**Suite 29/29 (+probatio_silva_semantica, 88 asserts — 87/88 first
run; the one fracture was the probatio's own hand-counted fixture
length feeding a trailing NUL, i.e. silva refusing bad bytes exactly
as designed).** fontes/silva_c89_semantica.{h,c} (~1,450 lines):

- TypusC89: interned derived types (monstrator/acies/functio/
  qualificatus — single-pointer handles, POINTER EQUALITY IS
  IDENTITY, pinned by fixtures), nominal tags (identity per
  declaration site), primitive singletons with eager formae.
  Qualificatus-super-qualificatum merges vexilla; quales==0 returns
  internum (no gratuitous wrappers).
- Specificatores walk per the v2 contract: bare tokens, maximal
  primitive runs MERGED across qualifier splits (unsigned const
  long fixture), typus-nominatus through the flat typedef table,
  inline tag/enum handling, implicit int (C89 legale), mixed
  primitive+named = poison.
- Tag discrimination on tok_aperta PRESENCE (v2 contract);
  register-BEFORE-membra (self-referential struct pinned:
  membra[1].typus->monstrator.internum == the tag itself); enum
  constants into the flat constantes table via the aestimator
  (implicit counter semantics pinned: 0, 5, 6).
- Declarator unwind, C inside-out via internum chain: char *p[3]
  = acies[3] of monstratores (24/8); int (*f)(void) = monstrator
  ad functionem; parameter adjustments (acies->monstrator,
  functio->monstrator); (void) = prototypata 0; () = non
  prototypata; ELLIPSIS bare in parametra.
- Aestimator: folium-integer (dec/oct/hex + suffixa), folium-
  character (escapes), enum constants, unarium/binarium/ternarius
  (chosen-branch only), conversio (truncation to integer types),
  MAGNITUDO-TYPI through forma — the typus<->forma<->aestimator
  mutual recursion works (char b[sizeof(long)] pinned = 8).
- Forma: arm64 LP64 Apple (long double = 8 — worklog), lazy,
  in_computatione cycle guard, struct offsets/padding + union
  max (const-strip U pinned 8/8, offsets 0/0), incomplete/functio/
  vacuum/campos = FALSUM + diagnosticum.
- Poison non-cascade pinned: unknown-to-semantica typedef poisons
  ONE declaration, the next types clean; diagnostics table grows.

Parks recorded in silva_c89_semantica.worklog.md: long long as
deliberate extension (corpus s64!), sizeof(expr)-in-consteval (M0b),
\x escapes, linear interning (measure at percursus), flat-table tag
redefinition (Chunk B closes). Gates: amalgamare VERIFICATUM 27/27
(module not yet in manifest — publica = Chunk D).

**NEXT (named): Chunk B — scopus + the whole-file declaration walk**
(scope stack, four namespaces, source-order register-before-use,
conditionalis recursion, percursus -semantica flag, corpus sweep
bar: every file analyzed, diagnostics 0).

### M0a CHUNK B — RELATIO (2026-07-08): scopus + ambulatio integra SHIPPED

**Suite 29/29 (probatio_silva_semantica 112 asserts, first-compile
green). THE SWEEP: 729 files, ZERO crashes, 234,427 symbola built,
semantica cost invisible (~108s with vs ~112s baseline without —
rides free on the parse).**

- Scopus stack: ordinaria (vars/functiones/typedefs/CONSTANTES
  enumerorum — one namespace) + tags per scope, profunditas
  tracked; flat Chunk-A tables replaced (typedef/tag/constant
  lookups now scope-walks; A's flat-tag-redefinition limitation
  CLOSED). Symbols carry genus/typus/valor/repositio/profunditas/
  declarans/lexema and accumulate in sem->symbola (the index seed).
- The walk: source-order register-before-use single pass;
  DEFINITIO_FUNCTIONIS (params via _functionis_intima — innermost
  functionis layer; param adjustments; params scope + body scope
  nested); statement recursion (si/dum/fac-dum/per/commutatio/
  casus/ordinarius/titulatum/corpus); CONDITIONALIS descends
  SUMPTUS contentum, skips OMISSUS (pinned: in_omisso absent);
  AMBIGUUS via canonica; ERROR skipped. K&R = named-park
  diagnostic. Same-scope genus-conflict diagnostic (pinned).
- Shadowing pinned: y at prof 2 (int) and prof 3 (char);
  x as PARAMETRUM prof 1; static repositio on quadratum.
- percursus -semantica flag (DEFAULT OFF — baseline pristine;
  flags-off sweep numbers unchanged): per-file [semantica
  diagnostica N] + summary line.

**BAR AMENDMENT (named):** INTENTIO's "diagnostics 0" was
pre-systema optimism. Sweep: 818 diagnostica in 524 plagulis —
probe-verified ALL of class "typus nominatus ignotus" (plain
`size_t n;` declarations commit as declarations; only cast/sizeof
positions fork — census). This is THE systema gap by construction;
Chunk C's bar inherits "semantica diagnostica → ~0" alongside the
ambigui collapse. B's bar as met: every file analyzed, zero
crashes, diagnostics CLASSIFIED and owned by a named next chunk.

Gates: amalgamare VERIFICATUM 27/27. Worklog: params/body scope
model, lenient same-genus redecl, labels park.

**NEXT (named): Chunk C — systema + oraculum trivalens + closure**
(systema_c89.h authorship, oracle preload + parse-once-harvest,
trivalent upgrade + non-type resolver rule, recanonicare closure;
bar: ambigui 15,261 → named-park residue AND semantica diagnostica
→ ~0).

### M0a CHUNK C — RELATIO (2026-07-08): systema + oraculum trivalens + clausura SHIPPED

**THE DEMO NUMBERS (the census's promise, kept):**
- ambigui at commit: 15,261 → 3,401 (-78%; preload + trivalent
  precommit registration collapse most forks before retention)
- post-closure: 1,939 versae + 1,051 confirmationes; residuum
  **411 indecisa = 2.7%** of the original mass (97.3% resolved)
- semantica diagnostica: 818 → 110 in 26 plagulis (-87%)
- walls unmoved: errores 0, totalitas TENET, 729/730 octetim (the
  named pasta deferral), tempus ~111s (semantica+clausura cost
  invisible), suite 29/29, amalgamare VERIFICATUM 27/27.

**Shipped:**
- fontes/systema_c89.h — the officina PLATFORM FILE: full ISO C89
  library surface (~145 prototypes, all core types, key macros),
  vanilla C89, parses with ZERO errors and ZERO ambigui (type
  ordering deliberate); provenance-marked harvest (ex_systemate on
  types and symbols) for the haruspex partition.
- Oraculum TRIVALENS (silva_commissio): non_typi table +
  non_typum_addere[_literis]/novit; resolver kill-rule (a reading
  requiring a known NON-type as a type = impossible, same force as
  X10); precommit walk now registers non-typedef declarators AND
  function-definition names as non-types (same interleaved walk,
  same visibility semantics).
- Channel B plumbing: analysare_cum_systemate (systema harvested
  into scope 0 before the user walk; user parses stay pristine) +
  oraculum_augere (semantica symbols → oracle: typedefs as types,
  everything else as non-types — serves BOTH preload and closure)
  + ambigua_indecisa_numerare (decisum != versum: recanonicare
  counts flips only — the closure metric is undecided-count).
- percursus -semantica full pipeline: systema parsed once
  (long-lived ctx piscina), per-file preloaded oracle → parse →
  cum-systemate analysis → augere → responsa_vacare →
  recanonicare + residuum; summary "clausura: N versae; M indecisa".
- census_ambigui upgraded to full-pipeline residue mode (prints
  ONLY indecisa discriminants) — the classification instrument.

**Residuum CLASSIFIED (bar clause met):** (a) nested cast/paren
chains + GLR fork-as-reading packing — the named resolver park,
now sharply characterized with a minimal repro pinned in the
probatio (delineare 34, scrutinium family ~85...); (b) POSIX names
(reactor/tcp/dirent/termios) — systema is ISO by DECISUS, a
systema_posix supplement is named on-pull; (c) diagnostics-only:
~55 roundtrip fixtures with DELIBERATELY unknown types + ~15
praebenda basename-collision victims (harness item). Full table in
silva_c89_semantica.worklog.md.

**Found+fixed during classification — the CONTAGION bug:** the
trivalent kill leaked through nested ambigui (outer readings
absorbed non_typus from inner ALTERNATIVES → both killed → wrongful
retention). Fix: AMBIGUUS special-case in _nodum_examinare —
nominatus/ignotus merge as ANY (old semantics preserved),
non_typus merges only if ALL inner readings carry it. Probatio
pins the repro (2 of 3 forks rescued; the third = deep-chain park).

**Lesson recorded (worklog + memory-worthy): the stale-object
trap.** Two build systems write DIFFERENT object names into
silva/build (suite: <n>.o; percursus: fons_<n>.o). Hand-linked
probes silently used pre-fix objects — "the fix didn't work"
because it was never in the binary. Purge both flavors after
fontes edits when hand-linking.

Probatio: 143 asserts (was 112). **NEXT (named): Chunk D —
haruspex + index + publica** (the oracle instrument over the type
inventory w/ provenance partition; index API, saltuarius first
consumer; silva.h rows + hospes + manifest; M0a RELATIO + FULL
phase-log audit at close).

### M0a CHUNK D — RELATIO (2026-07-08): haruspex + index + publica SHIPPED

**HARUSPEX VERDICT: 177 TU, 4,801 assertiones temporis compilationis,
0 dissentientes** — clang consentit omni sizeof, ordinationi, offset
membrorum, valori enumeratoris quos semantica computavit (include/,
lib/, silva/tessera/saltuarius fontes). Design: quaeque TU plagulam
ORIGINALEM includit (inclusio textualis etiam pro .c) + assertiones
appensae; clang -fsyntax-only = iudicium; typi in .c definiti in-TU
verificati; ordinatio per dolum { char c; T t; }. Partitio
provenientiae tenuit (0 typi corporis systema per valorem
amplectuntur; systema ipsum numquam clang-compilatur — exclusum).
Praetermissa 34 = functiones/incompleta (iuste sine forma).
Instrumentum: instrumenta/principalia/haruspex.c + haruspex.sh
(TUs in build/exta/ — quid aliud?).

**Index publicus**: symbola_numerus / symbolum_per_indicem /
diagnostica_numerus / diagnosticum_per_indicem super sem->symbola
(234k+ symbola per corpus). Saltuarius saltus typorum = consumptor
primus nominatus (nexus = tractus v0.1 saltuarii).

**Publica**: manifest += silva_c89_semantica.{h,c}; CADENDA
(TypusC89 definitio plena in silva.h; SilvaSemantica opaca;
SemanticaScopus interna); silva.h sectio M0a (vanilla — i32 =
unsigned int EXACTE aut TU amalgamae declarationes pugnantes
videt); hospes: OMNES XI functiones novae VOCATAE cum assertionibus
contenti (clausura per superficiem publicam!). VERIFICATUM primo
conatu, hospes 28/28. Suite 29/29 (probatio 146 assertiones).

## M0a — RELATIO MILLIARII + AUDITUS PLENUS (2026-07-08)

**M0a CLAUSUM.** Semantica C89: typi + scopi + forma + systema +
oraculum trivalens + clausura + haruspex + index + publica. Barrae:
729/730 plagulae analysatae (0 ruinae); formae contra clang
verificatae (4,801/0); ambigui 15,261 → 411 residuum classificatum
(97.3%); diagnostica 818 → 110 classificata; suite 29/29;
VERIFICATUM. Officina M0a-dependentia soluta; M0b (typatio
expressionum + conversiones) = milliarium proximum officinae
criticum; saltuarius saltus typorum nunc DISPONIBILIS.

**AUDITUS PLENUS (regula limitis phasis; agente perlecto toto
phase-log, verificatum hic):**

CONFIRMATUM NUMERO (P2/D5 — hiatus load-bearing auditus): typedef
scopi corporis (decisiones 13): oraculum praecommissionis eos NON
registrat (retentio ad commissionem — intactum), semantica scopis
eos NOVIT → clausura DECIDIT. Fixum pineatum in probatio (retentum
>= I ad commissionem → indecisa 0 post augere). Promissum consilii
§II impletum, non silenter.

TABULA PARCARUM (sublata e worklog/consilio in phase-log — actio
auditus):
- initiatores non probantur → M0b
- magnitudo-expressionis in aestimatore → M0b
- K&R typatio plena → on-pull (corpus eam continere non potest)
- haruspex scopi corporis → replicatio-sub-tag-synthetico (via
  reditus nominata)
- parca resolutoris CATENARUM (restatuta cum magnitudine VERA —
  actio auditus D3): NON "34 instantiae" sed delineare 34 +
  familia scrutinii ~85 + elementa/pinacotheca/nuntium/cetera
  (categoria maior residui 411); repro minimale pineatum
  ("catenae nidificatae"); sedes: inspectio resolutoris post-M0a
  (resolutio coniuncta catenarum aut fixpunctum intus-primum)
- supplementum systema_posix → on-pull (officina Wave-1b/3)
- SPATIUM NOMINUM TITULORUM SALTUUM (sublatum e worklog solo —
  actio auditus D4): III spatia impleta, quartum (tituli) lint
  possidet; INTENTIO "quattuor spatia" sic amendata nominateque
- persistentia indicis (sublata e consilio solo — actio auditus
  A16) → donec consumptor frigidus existat
- vita semanticae/invalidatio → M4 + incrementalitas
- lector est_syntheticus → in necessitate vera
- fuga \x/\NNN in aestimatore → on-pull (nulla sedes prior — nunc
  nominata: M0b cum typatione expressionum)
- collisiones basename praebendorum (5) → res percursus (harness),
  non semanticae
- __DATE__/__TIME__ → contextus valore-fixo cum trahetur
- defined(__FILE__) in #if → expander, si umquam trahetur
- RESOLUTUM-MENSURA (actio auditus A10): internamentum lineare —
  Chunk B et C ambo "cost invisible"; clausum, non portatum
- RESOLUTUM: tabula plana tag (Chunk A) → scopi B

NOTAE PRO OFFICINA (cross-refs vetustae, actio auditus 6 — in
officina-spec-v2 §XII annotatae): §VI "oraculum binarium" nunc
falsum (trivalens M0a); conflatio M0/M0a+M0b; "diagnosed not
resolved" vendit minoris (97.3% resolutum); "724 plagulae" → 730.

DERIVAE RELIQUAE (LOW, notatae non actae): baselines ambiguorum
inter documenta eiusdem diei (crescentia corporis); "prope-zephyrum"
= 2.7% intra caveat consilii.

**M0a numeris finalibus: 5 chunks (pre-M1 + A/B/C/D trans dies
duos), probatio 146 assertiones, hospes 28/28, haruspex 4,801/0,
corpus 730 plagularum 0 ruinis, residuum 411 omne classificatum.**

### ADDENDUM ANTE COMPACTIONEM (2026-07-08) — M0b engine hints

Nothing in-flight (M0a CLOSED, gates green); these are the
implementation hints for whoever writes the M0b INTENTIO, recorded
before they evaporate:

- M0b = expression typing + usual arithmetic conversions as
  EXPLICIT parallel-table annotations (design DECISUS 2/3): table
  nodus -> {typus, conversio}. Bar: every expression node in the
  corpus typed; conversion fixtures pinned; demissio's input
  contract FROZEN (officina critical path).
- THE SKELETON EXISTS: the aestimator's genus dispatch
  (silva_c89_constans_aestimare) is the shape to generalize —
  folium-*/unarium/binarium/ternarius/parenthesis/conversio/
  magnitudo-* are already dispatched; M0b adds vocatio (reditus of
  FUNCTIO type; implicit extern-int for unknown callees — C89
  legale), subscriptio, accessus (member lookup = chorda compare
  over membra; . vs -> via tok_operator), postcrementum,
  assignatio, virgula, folium-identificator -> symbol resolution.
- POINTER-KEYED TABLE: tabula_dispersa keys are chordae — for the
  nodus->typus table use tabula_dispersa_creare (the GENERIC
  constructor with custom TabulaFriatio/TabulaComparatio over the
  pointer bytes) — do NOT invent a new map.
- RESOLUTION DURING THE WALK: identifier-use resolution needs LIVE
  scopes — Chunk B's walk skips expression subtrees; M0b extends
  _elementum_ambulare (+ declaration initiators) to descend
  expressions WHILE scopes are open, writing the parallel table
  as it goes (walk-order = source-order, the sanatio invariant).
- Usual arithmetic conversions: promotions (char/short -> int),
  signed/unsigned dominance, the f64 ladder; pointer arithmetic
  (T* + integer -> T*; T* - T* -> ptrdiff/long); lvalue-ness for
  assignments (lint-grade, not blocking).
- UNPARKS AT M0b: magnitudo-expressionis in the aestimator (needs
  expression type — mutual recursion already shaped for it);
  initiatores type-CHECKING; \x/\NNN escapes (folium-character
  full escapes while touching that code).
- Officina spec cross-refs to refresh while there: spec-v2 §XII.8
  list (binary-oracle claim, M0a/M0b conflation, 724->730).
- Re-entry: silva/CLAUDE.md -> this phase-log tail (M0a RELATIO +
  AUDITUS) -> silva-semantica-design.md -> the semantica worklog.

## M0b — INTENTIO (2026-07-08): typatio expressionum + conversiones

Consilium: silva-semantica-design.md DECISUS 2/3 + the M0b engine
hints (addendum supra) + **officina-simulatio-2.md** (simulation
2026-07-08 against the shipped M0a module: grammar-loci inventory +
node census + closure-mechanics grounding; Fran approved ALL SIX
recommendations as written). Officina critical path: demissio's
input contract freezes here.

**DECISUS (Fran 2026-07-08, via simulatio-2):**
1. **Annotation = per-node pair {typus naturalis, typus conversus}**,
   both TypusC89* — naturalis = the expression's own synthesized
   type, UNdecayed (arrays stay arrays, function designators stay
   functions); conversus = the context-applied conversion target
   (UAC operand conversion / array-function decay / assignment RHS /
   argument / return), NIHIL if none. THE FROZEN CONTRACT demissio
   reads (Eskil moment). Public rows: silva_c89_typus_expressionis +
   silva_c89_conversio_expressionis, both _canonicum-aware,
   NIHIL-safe. sizeof-operand: NO decay (sizeof(arr) right by
   construction). Compound assignment annotates the implied binary
   op's conversions on both operands.
2. **Typing is CANONICAL-RELATIVE; authoritative analysis runs
   POST-closure.** sem₁ cannot be skipped (closure feeds on its
   symbols via oraculum_augere) → percursus -semantica goes
   TWO-PASS: sem₁ feeds closure (annotations discarded) →
   recanonicare → sem₂ authoritative (reported, queried). Contract
   stated in silva.h: "analyze after your last recanonicare."
   Typing ALWAYS-ON in analysare (complete-first; cost invisible
   per census; if measurement later disagrees, a skip-flag for the
   feeder pass is the named path back). Probatio pins a flip
   fixture (sem₁ type ≠ sem₂ type across a closure flip).
3. **Table: pointer-keyed tabula_dispersa via the GENERIC
   constructor** (include/tabula_dispersa.h:66; custom friatio/
   comparatio over 8 pointer bytes stored in the entry; chorda
   clavis points at them). EAGER, uniform — the eager-vs-lazy fork
   was REFUTED BY MEASUREMENT (census: worst file arbor2_glr_tabula
   ≈3.7 MB annotations riding a 3.18 GB parse arena; expr nodes ≈
   57-59% of all nodes handwritten, ~80% generated).
4. **UAC adopts C99 integer conversion ranks for the long long
   extension** (C89 doesn't rank what it doesn't have; the choice
   is ours and demissio freezes it). LP64 corners pinned by
   fixture: unsigned int + long → long; unsigned long + long long
   → unsigned long long. Certified by auspex (5).
5. **AUSPEX** (haruspex's bird-watching sibling,
   instrumenta/principalia/auspex.c + auspex.sh): the integer UAC
   table exhaustively clang-certified via CONSTANT-expression
   asserts — result size (sizeof in array bound) + result
   signedness ((expr)*0-1 > 0 in array bound); -fsyntax-only =
   verdict. Known gap recorded: double vs long double identical on
   Darwin (both 8) — indistinguishable, harmless.
6. **Congeries types flow DOWN (simulatio-2 find):** annotated with
   its CONTEXT type (from the declarator); elements typed where
   correspondence is direct (fully-braced); brace-elision shapes
   get a NAMED diagnostic which IS the corpus measurement (expected
   ≈0 — generated tables fully braced); full elision walk PARKED →
   M1 demissio (static-data materialization owns the same engine).
7. **lvalue bit OMITTED from v1 annotations** (demissio derives
   lvalue-ness structurally; assignment-lvalue legality = lint).
   Named park, on-pull.

**CHUNKS + BARS:**
- **A — table + descent + primaries.** Pointer-keyed table; walk
  descends the 13 expression loci (si/dum/fac-dum conditio,
  per-clausula initium/conditio/passus, commutatio discrimen, casus
  valor, redde valor, sententia-expressionis, declarator-initiatus
  initiator, enumerator valor, campus latitudo, declarator-aciei
  mensura) with scopes LIVE; primaries: folium-identificator
  resolution (symbolum genus → typus; unknown callee → implicit
  extern-int REGISTERED per C89 6.3.2.2; unknown else → poison +
  diagnosticum), literal suffix typing (10u/10L/1.5f...),
  folium-chorda → char[N] over the fragment LIST via _fuga_decodere
  (\x/\NNN unpark REQUIRED here; shared with folium-character
  aestimator; L"..." = named-park diagnostic), parenthesis, unarium
  (INCL. prefix ++/-- — no praecrementum genus), postcrementum.
  Bar: probatio fixtures + corpus sweep "N expressionum typatae,
  0 ruinae".
- **B — UAC + operators + auspex.** Promotions + C99-rank ladder;
  binarium (12 tiers, one genus); ternarius incl. pointer
  composites (void*+T*, null-pointer-constant+T*); assignatio (11
  ops, one genus; compound implied-op); virgula; pointer arithmetic
  (T*±int → T*; T*−T* → long per systema ptrdiff_t). Bar: auspex
  0 dissentientes + LP64 corner fixtures.
- **C — postfix + unparks.** vocatio (functio/monstrator-ad-
  functionem → reditus; prototyped arg conversions annotated;
  variadic-extra + unprototyped → default argument promotions;
  vocatio.argumenta is LISTA_MIXTA — skip comma TOKENS);
  subscriptio; accessus (one genus, . vs -> per tok_operator;
  member = bare TOKEN chorda-compared over membra; basis
  qualificatores MERGE onto member type; incomplete tag = poison);
  magnitudo-expressionis UNPARK (three-way recursion
  aestimator↔forma↔typatio; operand UNdecayed); congeries context
  typing per DECISUS 6. Bar: per-file expression-coverage %
  printed by percursus.
- **D — pipeline + publica + close.** Two-pass closure in percursus
  (2); silva.h rows + hospes calls (standing rule); coverage
  counters in the summary line; officina-spec-v2 §XII.8 stale-refs
  refresh (binary-oracle claim, M0a/M0b conflation, 724→730);
  M0b RELATIO + FULL phase-log audit (milestone boundary rule).
  Bar: corpus "every expression node typed or carrying a named
  diagnostic" + the coverage number; amalgamare VERIFICATUM.

**PARKS NAMED (landing spots):** full brace elision (M1 demissio);
wide strings L"" (on-pull; corpus none); lvalue annotation
(on-pull; demissio derives); auspex double-vs-long-double blindness
(recorded; Darwin-harmless); K&R expression typing rides the
existing K&R park.

**UNPARKS EXECUTED IN M0b:** magnitudo-expressionis (C), initiator
typing per DECISUS 6 (C), \x/\NNN escapes (A).

Pleasant fact recorded (simulatio-2): implicit extern-int
SELF-HEALS the POSIX residue for typing — close(fd) → int, legal
C89, no diagnostic; M0a residue category (b) costs M0b nothing.

**NEXT (named): Chunk A.**

### M0b CHUNK A — RELATIO (2026-07-08): tabula + descensus + primaria SHIPPED

**Suite 206 asserts (was 146), 29/29; sweep 731 plagulae, 0 ruinae,
900,320 typationes; VERIFICATUM (hospes 28/28); root suite 93/94
(the 1 = pre-existing flaky probatio_tcp). AND TYPING FOUND A REAL
BUG IN LATINA.H ON ITS FIRST CORPUS CONTACT.**

Shipped per INTENTIO: pointer-keyed typationes table (simplified —
default chorda FNV-1a is binary-safe, no custom friatio needed);
walk descends all 13 expression loci with live scopes; primaries
typed (identificator resolution, literal suffix rules incl. the
hex/octal unsigned ladder, folium-chorda char[N] over fragment
lists, parenthesis, unarium incl. prefix ++/--, postcrementum);
_fugam_decodere shared decoder — **\x/\NNN UNPARKED** in both the
aestimator and string lengths; scalar-context lapsus annotated on
conditions (naturalis stays undecayed — pinned: while(buf) naturalis
char[4], conversus char*). B/C genera descend-only, silently.

**FIND 1 — libc value-macros were invisible** (first sweep:
diagnostics 110 → 18,293): NIHIL→NULL, EOF, INT_MAX lex as unknown
identifiers — declarations never contain them, so M0a couldn't see
it. Fix: systema_c89.h attached as LEXICON in percursus (-semantica
only; the #defines-only channel, empirically typedef-inert — exactly
right here). 18,293 → 1,026. Plus LP64 correction: systema's
LONG/ULONG limits carried ILP32 values.

**FIND 2 — latina.h:402 was REVERSED**: `#define FILE FILUM`
(system name on the left, unlike all 388 other defines). Real
compiles only worked because latina-before-stdio rewrote stdio's
own typedef into defining FILUM — include order was secretly
load-bearing. Silva could never see that typedef; `FILUM* f;`
retained forks whose EXPRESSION canonicals typing then walked
(pl/f/FILUM = ~700 diagnostics). Fran confirmed bug; flipped to
`#define FILUM FILE`. **Retroactive M0a improvement: ambigui
3,411→3,354, indecisa 414→355 — ~59 entries of the nested-chain
park ledger were actually this bug.** Root+silva suites green.

Residuum 245 in 28 plagulis, ALL classified (worklog table):
(a) POSIX ~120 — the systema_posix park now known to need BOTH
channels (macros via lexicon + prototypes via channel B);
(b) latina-less standalone/fixture files ~112 (existing class
extended to expressions); (c) tail 5 (typedef-in-expression 4 →
re-examine at D's post-closure pass, + 1 mensura).

Gate consequences as designed: tabula_dispersa_numerus left the
amalgamator EXCLUDENDA (first caller); hospes semantica fixture
declared its x (typing now diagnoses undeclared identifiers —
the fixture predated expression examination).

**NEXT (named): Chunk B — UAC + operators + auspex** (promotions +
C99-rank ladder, binarium/ternarius pointer composites, assignatio
compound implied-op, virgula, pointer arithmetic, auspex instrument;
bar: auspex 0 dissentientes + LP64 corner fixtures).

### M0b CHUNK B — RELATIO (2026-07-08): UAC + operatores + auspex SHIPPED

**AUSPEX VERDICT: 2,147 expressiones, 3,753 assertiones temporis
compilationis, 0 dissentientes** — omnis copula primitivorum ×
omnis operator, magnitudo ET signum, clang-certificata (eadem
expressio verbatim per fistulam nostram et per clang; TU ut C99
consulto — electio ordinum ll IPSA certificatur). Bar met.

Suite 239 asserts, 29/29; sweep **1,086,949 typationes** (+186k),
**ZERO operator-class false positives** over the million; walls
unmoved (0 errores, indecisa 355, fideles 731/732 — the +1 file is
auspex.c joining the corpus). VERIFICATUM, hospes 28/28.

Shipped: UAC engine (C99 ranks; LP64 representability = strict
size dominance; pinned corners ul+ll→ull, i+l→long); binarium all
12 tiers (shifts = promoted LEFT, non-UAC); ternarius incl.
composite pointers (qual merge, void* wins) + null-constant arms;
assignatio simple (RHS→left conversus) + compound (implied-op
conversions both operands); virgula; pointer arithmetic (T*±int,
T*−T*→long); return conversion (sem->reditus_currens); initiator
target conversion (_ad_finem_annotare shared by =/redde/init).
**INTENTIO amendment: conversio (cast) typing pulled into B** —
assigned to neither chunk; auspex required it; it IS the
conversion operator.

**FIND 3 — third real bug: uuid.c `(insignatus i8)`** = `unsigned
i8`, illegal C, hiding in the #else (Linux) arm Apple never
compiles. Silva takes that arm (__APPLE__ undefined) — typing
checked code no compiler on this machine has ever compiled, free.
Fixed (i8 alone). Config note recorded for the config-query
milestone (taken-arm evaluation models a platform-neutral config).

Diagnostic delta fully classified: +19 POSIX types in CAST
position (new sighting path via conversio typing — same
systema_posix park), +1 the uuid bug (now fixed). Instruments:
auspex.c + auspex.sh committed; known gaps recorded (Darwin
double==long double; pointer signedness non-constant; ternary
composite types → probatio fixtures carry those).

**NEXT (named): Chunk C — postfix + unparks** (vocatio w/ implicit
extern-int + arg conversions; subscriptio; accessus w/ qual
propagation + incomplete-tag poison; magnitudo-expressionis unpark;
congeries context typing w/ elision diagnostic; bar: per-file
expression-coverage % printed by percursus).

### M0b CHUNK C — RELATIO (2026-07-08): postfixa + exparcata + est_implicitum SHIPPED

**COOPERTURA 99.61%** (1,497,967/1,503,793 nodi expressionum typati,
POST clausuram contra arborem finalem — the 5,826-node gap IS the
canonical-relative flip class, Chunk D's named target; percursus
prints per-file [expr coopertura N/M] + summary %). Suite 275
asserts 29/29; typationes 1,610,983; VERIFICATUM hospes 28/28.
Bar met, with the gap it exists to measure now measured.

Shipped: vocatio (reditus; prototyped arg conversions; variadic/
unprototyped default promotions; arity = enforcement layer's, not
diagnosed); subscriptio (incl. 1[arr]); accessus (./-> one genus;
bare-token member lookup; QUAL PROPAGATION pinned — const struct →
const member; incomplete tag = poison+diagnosticum); magnitudo pair
= size_t; **magnitudo-expressionis EXPARCATUM** (aestimator↔forma↔
typatio three-way recursion; sizeof(arr)=16 UNdecayed pinned);
**congeries context typing** (DECISUS 6: type flows down, nested
recursion, member conversions annotated; **ELISION MEASUREMENT:
ZERO corpus instances** — the M1 park is free today);
**est_implicitum** (the enforcement-layer amendment: synthesized
extern-int callees flagged + declarans = CALL SITE; silva.h struct
mirrored; hospes reads it; pinned both ways).

**Retroactive improvement #2: indecisa 355 → 305** — implicit
callees feed the trivalent oracle as non-types; the nested-chain
park ledger shrinks again without touching the resolver.

**Noise class found + silenced precisely**: typing the CALLEE
position unlocked typedef-in-expression ×2,155 — all inside
retained-AMBIGUUS canonicals (the park files; real code cannot
contain typedef-as-expression). _intra_ambiguum (pater chain)
guards only the two typedef diagnostics; poison stays; ignotus
stays ungated (it found three real bugs). Post-guard arithmetic
closes exactly: 265 − 4 + 151 = **412 diagnostica in 32 plagulis,
all classified** (POSIX now shows its THIRD face: types + macros +
incomplete tag layouts = 151 accessus diagnostics; systema_posix
park owns all three).

**NEXT (named): Chunk D — pipeline + publica + close** (two-pass
closure in percursus per DECISUS 2 — the 5,826-node coverage gap
and the flip-class noise are its acceptance test; silva.h rows
typus/conversio_expressionis + typationes_numerus + hospes;
officina spec-v2 §XII.8 stale-refs refresh; M0b RELATIO + FULL
phase-log audit at milestone close).

### M0b CHUNK D — RELATIO (2026-07-08): fistula + publica SHIPPED

**COOPERTURA 100.00% (1,503,915/1,503,915)** — the two-pass
pipeline closed the 5,826-node canonical-relative gap to ZERO.
Percursus: sem₁ feeds closure (annotations discarded) →
recanonicare → sem₂ authoritative against the flipped canonicals
(reported, queried, coverage-checked). Cost invisible (113s, same
as single-pass). Diagnostics 412 → 399 (the small tail's C1
artifacts evaporated with the flips, as predicted at C).

Publica: silva.h rows silva_c89_typus_expressionis /
conversio_expressionis / typationes_numerus (vanilla, const-correct,
with the canonical-relative contract stated in the section comment);
hospes calls all three with content asserts (initiator "1 + 2" →
int, conversus long, through the public surface). VERIFICATUM,
hospes 28/28. Officina spec-v2 §XII.8 RESOLVED (§VI rewritten
trivalent + shipped-status, §XI M0 bar restated with actuals).

## M0b — RELATIO MILLIARII (2026-07-08)

**M0b CLAUSUM. Typatio expressionum + conversiones: the demissio
input contract {typus naturalis, typus conversus} is FROZEN and
shipping.** Bars, all met: **every expression node in the corpus
typed (100.00%, 1.5M nodes, 732 plagulae, 0 ruinae)**; conversion
table clang-certified (**auspex 3,753 assertiones / 0
dissentientes** — every primitive pair × every operator, size and
signedness); LP64 corners pinned by fixture; suite 275 asserts
29/29; VERIFICATUM.

Four chunks (one day): A tabula+primaria (+\x/\NNN unpark, systema
LEXICON channel), B UAC+operatores+auspex (+conversio amendment),
C postfixa+exparcata+est_implicitum (+congeries, elision measured
ZERO), D two-pass+publica.

**THREE REAL BUGS found by typing on first corpus contact**:
latina.h `#define FILE FILUM` reversed (include-order was secretly
load-bearing); uuid.c `(insignatus i8)` in the never-compiled Linux
arm; plus the value-macro invisibility class (18,293 diagnostics →
systema lexicon). **Retroactive M0a improvements**: indecisa 414 →
305 (FILUM flip −59, implicit-callee oracle feed −50) — a quarter
of the nested-chain park ledger was never a resolver limitation.

Diagnostics 399 in 31 plagulis, all classified: POSIX (three faces:
types, value-macros, incomplete tag layouts — systema_posix park
owns all) + latina-less fixtures + 1 mensura. Parks with landing
spots: full brace elision → M1 demissio (corpus measurement: ZERO
instances); wide strings L"" → on-pull; lvalue bit → ENFORCEMENT
LAYER as sole owner (demissio derives structurally in the interim —
supersedes the earlier "on-pull" phrasing, audit finding 3); auspex
gaps → Darwin double==long double (recorded), pointer-expression
signedness (PERMANENT — not constant-expressible), ternary
composite pointer TYPES (probatio fixtures carry them);
enforcement layer itself → named future (compatibility relation,
severity policy, richer diagnosticum, invalid corpus, recovery
mode — per the 2026-07-08 enforcement analysis; est_implicitum
recorded NOW because it was the one unrecoverable datum).

Officina critical path: M0 CLOSED end-to-end. NEXT officina
milestone = **M1 medulla + demissio** (op vocabulary = naming
ceremony at design time; demissio reads {naturalis, conversus} +
sym index + formae). Silva-side alternatives: saltuarius type-jump
(index + typatio now both available), quaestio QB.

**AUDITUS PLENUS (regula limitis milliarii; agente toto phase-log
perlecto + worklog + simulatio-2 + spec-v2; verificatum hic):**

VERDICT: clean on delivery — all 7 DECISUS delivered or
amended-with-recorded-reason (3: generic-creare → creare_chorda,
worklog A; +2 named additions: conversio→B, est_implicitum), all 3
unparks logged, addendum hints honored, every park owned. Two
findings ACTED ON before sealing:

- F1 RESIDUUM BASELINE RECONCILED: 411 @M0a close (730 plagulae) →
  414 with corpus growth (+1 plagula, Chunk A — the walls line
  said "unmoved" without naming the +3) → 355 (FILUM flip, −59) →
  305 (implicit-callee oracle feed, −50). END 305 agrees across
  all documents; spec-v2 §VI stale "411 → 305" CORRECTED to the
  full chain.
- F2 FLIP FIXTURE LANDED (DECISUS 2's promised probatio, absent
  through D): "fixum versionis" — sem₁ types the pre-flip vocatio
  canonical of (T)(x) as venenum; recanonicare flips ≥1; sem₂
  types the cast canonical as int. Suite 286 asserts, first
  compile. The two-pass is now unit-pinned, not only
  corpus-proven.

LOW findings, recorded as notes: lvalue park had two owners →
unified above (enforcement layer sole); two auspex gaps lifted
from Chunk-B-only into the milestone parks list above; "1.6M
expressions typed" in spec-v2 §XI = typationes (op count,
1,610,983), coverage denominator = 1,503,915 NODES — different
quantities, both true; "nested-chain park ledger" in this RELATIO
reads TOTAL indecisa (411/414/355/305 include POSIX + fixture
classes; the isolated nested-cast class is a subset, never
separately sized — its repro stays pinned); D's −13 diagnostic
drop = the named 4-item typedef tail + 9 sibling C1 artifacts in
the same fork class (not decomposed further, same cause).

**M0b final numbers: 4 chunks in one day; probatio 286 asserts;
suite 29/29; coverage 100.00% (1,503,915 nodes, 732 plagulae, 0
ruinae); auspex 3,753/0; typationes 1,609,161 post-flip; residuum
305 all classified; VERIFICATUM. Demissio's contract is frozen —
M1 medulla+demissio is unblocked.**

### ADDENDUM ANTE COMPACTIONEM (2026-07-09) — M1 engine hints

Nothing in-flight (M0b CLOSED, committed, gates green); these are
the accumulated hints for whoever designs M1 medulla+demissio,
recorded before they evaporate:

- **M1 opens with the OP-VOCABULARY NAMING CEREMONY** (DECISUS-
  grade, like the genus tags — spec-v2 §XI). Consider the proven
  arc first: /simulate-work (simulatio-3 against the SHIPPED
  typing tables — both prior sims paid) → interview → INTENTIO.
  Bar: monorepo lowers; golden dumps; textual round-trip
  byte-identical.
- **What demissio READS (all shipped, all public)**: typus/
  conversio_expressionis (the frozen pair — conversus != NIHIL =
  emit a convert op); the symbol index (est_implicitum routes
  implicit callees to machinula builtins-or-fail); formam_computare
  (sizes/offsets); symbolum->valor (enum constants pre-evaluated);
  constans_aestimare (array sizes, case labels).
- **DRIVER SHAPE IS SETTLED — copy percursus's two-pass**: parse
  w/ preloaded oracle + systema lexicon → sem₁ → augere → vacare →
  recanonicare → sem₂ → demissio reads sem₂ ONLY (typing is
  canonical-relative; the flip fixture pins it).
- **String-literal BYTES gap (named)**: _fugam_decodere is
  interior; M0b only needed LENGTHS (char[N]). Demissio needs the
  DECODED BYTES for static data materialization → export a decode
  surface (or a chorda-materialization helper) as an early M1 task
  — do NOT reimplement the decoder.
- **Static data**: congeries direct-correspondence suffices for v1
  (elision measured ZERO in corpus; full walk stays the M1 park
  only if something pulls it). Union initializers: first member.
- **Compound assignment lowering is pre-chewed**: both operands
  carry implied-op conversions → load, convert, op, convert-back,
  store. Shifts: LEFT-promoted result type (non-UAC) already
  annotated.
- **Unprototyped/variadic calls**: default argument promotions
  already annotated on args; arity mismatches are NOT diagnosed
  (enforcement layer's) — demissio must tolerate them.
- **Machinula facts banked for M2**: systema_c89.h = the builtin
  surface (same file, both channels); NULL reaches user code as
  ((void*)0) via the lexicon — the VM sees a cast, not a magic
  token; est_implicitum = builtin-routing signal.
- Instruments precedent: haruspex (layout) + auspex (conversions)
  → M1's analog is the golden-dump corpus + byte-exact round-trip
  of the textual medulla form (the bar names it).
- Re-entry: silva/CLAUDE.md → this phase-log tail (M0b RELATIO +
  AUDITUS + this addendum) → officina-spec-v2.md §VI/§XI →
  silva_c89_semantica.worklog.md. NEVER implement from a
  compaction summary alone.
  NEVER implement from a compaction summary alone.

## 2026-07-09 — OFFICINA M1a CHUNK A (exporta demissionis) — pointer entry

Silva code changed as the opening chunk of officina M1a (narrative
home: officina/phase-log.md, per the M1 interview bookkeeping
DECISUS). Summary: nexus symbolorum use-site→symbol table (recorded
at the two resolution sites; typationes pattern; rows
silva_c89_symbolum_nodi + nexus_numerus), silva_c89_chorda_decodere
(decoded string bytes), silva.h rows for the already-existing
constans_aestimare + silva_oraculum_responsa_vacare. Gates: suite
308/308, 29/29, VERIFICATUM hospes 28/28, percursus baselines
UNCHANGED (0 errores / 100.00% / 399 diagnostica / 305 indecisa).
Detail: silva_c89_semantica.worklog.md 2026-07-09 (incl. the
L-strings-are-pre-parser finding).

## 2026-07-10 — SANATIO CATENARUM (oraculum) — pointer entry

Silva code changed, pulled by officina M2c (narrative home:
officina/phase-log.md M2c addendum). The M0a nested-cast resolver
park EXPARCATUM: dual trivalent kill (known TYPE as
identifier-expression = reading impossible; umbratio protected by
the existing non-typus registration) + winner-only flag flow
through inner forks resolved on the spot (the ledger's
"resolve-inner-first fixpoint"). Chains now TRANSMUTE at precommit
when typedefs are walk-visible (whole corpus). Dead-fork finding:
the catenae-nidificatae residuum-of-1 is a fork with NO valid
reading inside a rejected parent reading — off-spine, honest
retention (named follow-up: reachability-aware indecisa metric,
no pull). Gates: suite 29/29 (+corpus-shape fixture); percursus
indecisa 305→9, retenti ~3,354→1,046, coopertura 100.00% HELD,
fideles 759/760 (pasta deferral, pre-existing), diagnostica 399→455
ALL known classes (POSIX/fixtures un-gated by decision — cause
census clean); **auspex 3,753/0 dissentientes**; amalgama
VERIFICATUM 28/28. Officina effect: fusor sistere 2,912→2,659
(vocatio-sine-typo → 1 instructione), cursor Unda-0 31→45/73.
Detail: silva/fontes/silva_c89_oraculum.worklog.md (born).

## 2026-07-10 — COMPLETIO ACIEI (semantica) — pointer entry

Silva code changed, pulled by officina M2c (narrative home:
officina phase-log). C89 3.5.7 array-completion-by-initializer
implemented at the declaration chokepoint (declarationem_tractare;
new acies node, shared types never mutated; congeries count safe
by the corpus's elision-ZERO measurement; string literal =
octeti+1; extern-sine-initiatore untouched). Gates: suite 29/29,
percursus 100.00%/0 errores/indecisa 9, diagnostica +8 classified
(POSIX-progress + stb park), **auspex 3,753/0**, amalgama
VERIFICATUM. Officina effect: fusor sistere files 68→53, cursor
Unda-0 47→52/73 (arbor2_glr 101M instr VIRIDIS; capsula gzip
tota interpretata). Detail: silva_c89_semantica.worklog.md.

## 2026-07-10 — CENSOR NATUS (instrumenta prima) — pointer entry

New silva-side instrument, zero fontes changes (narrative home:
officina phase-log, INSTRUMENTA PRIMA). censor.c = latina-landmine
lint over the public origo surface: slot inventory runtime-derived
from SILVA_C89_REGISTRUM (tok_titulus slots), latina set from the
expander's macro journal — no hand lists anywhere. First corpus
contact: 762 plagulae, 0 flagrationes (FP 0.00). amalgamare.sh
gained porta 6 CENSURA (regression probe 13/13 + fontes sweep);
VERIFICATUM "+ censura". Regression corpus:
probationes/fixa/censoris/ (fragores suffixed .fragor so percursus
baselines never move). Detail: instrumenta/principalia/
censor.worklog.md.

## 2026-07-11 — PARCA NOMINATA: "selecta" (quaestio in subsellio) — pointer entry

Parked from the M3 chunk ① debrief conversation with Fran (narrative
home: officina phase-log). The demand: a bench CLI running quaestio
selectors against a file, printing matches BYTE-EXACTLY with line
ranges (emitte's extent printer) — structure-shaped grep + ready-made
unique Edit anchors. Sketch: `./silva/selecta.sh <plagula>
'<selector>' [-intra <symbolum>]`, where -intra reuses emitte's
binding-table walk to scope the query to a named function's subtree
(papers over the missing QB name-filter for the common case).
DEMAND COUNT (rule of two): occurrence ① member-access query gap
during M3 congelatio (grep `->bloccus` — recorded in
nexus.worklog.md 2026-07-11); occurrence ② Fran's structural-
selection question (same day). The pull for QB attrs/captures is
therefore LIVE — selecta (QA + -intra glue is already useful) and
the lint-rule harness (Fran's coupled-behaviors idea, same
conversation) are QB/QC's first two named consumers. Landing spot:
between M3 chunks as a palate cleanser (similitudo precedent) or
post-M3; TITULUS = caerimonia Franis ("selecta" is a placeholder).
Path back: this entry + nexus.worklog.md 2026-07-11 + the QA API
rows (silva.h:1053-1067).

## 2026-07-11 — EXAMEN (officina M4a) — CHUNK A INTENTIO: diagnosticum v2

Books live HERE per the M0 precedent; the arc (fundamenta →
simulatio-6 → interrogatio → spec) is in officina/phase-log.md and
project-specs/officina-m4a-*.md. Chunk A = the struct and the
plumbing; no judgment yet (relation = chunk B, seats = chunk C).

**Census (this session, grounded):** 44 addere sites, 41 distinct
causae (crementum + ternarius doubled), 5 NIHIL-node sites
(c:1143/1157 specifier walk, c:2154/2190/2195 forma walk). External
consumers of the diagnostics API: auspex + hospes, numerus/per_indicem
ONLY, no field reads — struct extension is compat-safe; addere is NOT
in silva.h, so its signature change is fontes-internal.

**Design (emendationes-cum-ratione vs the spec where noted):**
1. `EXAMEN_CODEX_*` enum (41 rows) + ONE static table
   {causa, severitas} in semantica.c — codex is the single source;
   the causa field stays populated (reader compat). addere becomes
   (sem, nodus, codex); new addere_cum_socio(sem, nodus, codex,
   socius) — first user = redeclaratio (socius = prior declaration,
   chunk C wires it).
2. **EMENDATIO: severitas gains a 4th value EXAMEN_INFRA** (toolchain
   gap, never a judgment). Reason: the census contains parked-feature
   honesty rows (campi parcata, chorda lata, K&R parca, elisio parca)
   and defensive fallbacks (genus declaratoris inexspectatum) that
   are neither violatio nor suspectum nor domesticum — this
   generalizes the sealed LEXICON_DEEST decision, and the spec's
   driver verdict already says ACCIPE/REICE/INFRA.
3. **EMENDATIO: the spec's shared position resolver DISSOLVES —
   positio materializes AT ADDERE.** linea/columna need only the
   token radix walk (censor pattern; new _lexema_primum recursive
   first-token helper); via needs the parsura, which analysare
   already receives → sem stores it (sem->parsura_currens, set at
   analysare entry; diagnostic lifetime = parsura's, documented).
   Consumers just read fields; no resolver API. ~455 corpus
   diagnostics × one walk = noise.
4. provisionale = _intra_ambiguum(nodus) at addere (the existing
   c:2980 walk IS the provisional test; 128-guard included).
5. The 5 NIHIL-node sites get nodes plumbed from one frame up
   (declaration/tag-declarans in scope at all five).
6. silva.h mirror: extended struct + severitas/codex enums + the two
   new/changed internal rows stay internal; hospes gains field-touch
   coverage (standing rule).

**Severity map for Fran's eyeball (VIOLATIO unless noted):**
redeclaratio-generis, tag-sine-corpore, enumeratio-sine-corpore,
enumerator-non-constans, error-in-specificatoribus,
specificatores-mixti, multiset-primitivorum, mensura-aciei,
error-in-declaratore, species-typi-exspectata, cyclus-formae,
forma-aciei-incompletae, forma-functionis-petita, fuga-invalida,
litterale-integrum-invalidum, identificator-ignotus,
typedef-in-expressione, typedef-vocatum, operandum-unarii,
operandum-tilde, deiectio-non-monstratoris, crementum-non-scalare,
multiplicativa, non-integralia, additiva-incompatibilia,
subtractionis-incompatibilia, motus-non-integralia,
comparatio-incompatibilium, bracchia-ternarii, subscriptio,
vocatus-non-functio, sagitta, accessus-non-structurae,
accessus-incompletae, membrum-ignotum  → **VIOLATIO (35)**.
typus-nominatus-ignotus → **INFRA, classis LEXICON_DEEST** (C1:
harness excludes the class from hard verdicts; NAMED QUESTION for
chunk D: promote to violatio when the world's lexicon is loaded —
the differential will show the under-judgment on typo fixtures).
campi-parcata, chorda-lata-parca, declarationes-KR-parca,
elisio-uncorum-parca, genus-declaratoris-inexspectatum →
**INFRA (5)**. SUSPECTUM: none yet (est_implicitum materializes in
chunk C). DOMESTICUM: none yet (house lint populates later).

**Gates:** suite green + NEW probatio_silva_examen.c (positions incl.
a macro-expansion case; provisional bit on an ambiguus fixture;
table integrity — every codex has causa + severitas, table size ==
enum count); percursus: diagnostica count UNCHANGED (455 — same
firings, richer rows), coopertura 100.00% HELD, indecisa 9; auspex
3,753/0; amalgama VERIFICATUM; censor clean.

## 2026-07-11 — EXAMEN CHUNK A RELATIO: diagnosticum v2 SHIPPED

INTENTIO praestita per omnia; nulla emendatio ultra eas iam in
INTENTIO scriptas (INFRA quartus gradus; resolutor dissolutus in
additionem; Fran benedixit ante opus).

**Quod navigavit:**
- SemanticaDiagnosticum v2: {nodus, causa} + {codex, severitas,
  provisionale, via, linea, columna, socius}. Positio materializata
  TEMPORE ADDITIONIS: _lexema_primum (ambulatio locorum recursiva,
  TOKEN/NODUS/LISTA) → silva_token_radix → linea:columna radicis;
  via per sem->parsura_currens (per ambulationem ponitur in
  analysare_cum_systemate — systema, deinde usoris).
- Tabula codicum EXAMEN_CODEX_* (41 ordines) = FONS UNICUS
  {causa, severitas}; assertum staticum magnitudinis (acies
  negativa). addere(sem, nodus, codex) + addere_cum_socio; 43 sedes
  vocationis conversae (percursu pythonis, 43/43, nulla manca).
- Sedes NIHIL-nodi V sanatae: _nodus_primus_listae (specificatores
  mixti/multiset — lista ipsa) + _declarans_typi (cyclus formae/
  acies incompleta per tags; functio sine declarante = NIHIL manet,
  honeste).
- provisionale = _intra_ambiguum (ambulatio exsistens, gratis).
- silva.h: enumerationes ExamenSeveritas/ExamenCodex + structura
  extensa speculatae; amalgamator manifestum TYPI duo nomina nova.
- hospes: massa examinis nova (identificator ignotus figitur,
  omnes ordines novi asserti incl. columna exacta 16) — 29/29.
- probatio_silva_examen.c NOVA (321 asserta, 6 sectiones):
  integritas tabulae per omnes codices; positio exacta (linea 2,
  columna 16, via); MACRO ad RADICEM ancorata (sedes usus non
  definitio — praecedens indicii probatum hic); provisionale VERUM
  sub furca retenta ('Ignotum * x;'); socius; sedes NIHIL sanatae.

**Portae:** compilatio stricta primo cursu; suite silvae 30/30;
amalgama VERIFICATUM (bis); percursus coopertura 100.00% TENETUR,
indecisa 10 (basis eadem), **flagrationes diagnosticorum in corpore
praeexsistenti IDENTICAE pre/post (diff per-plagulam via stash —
delta totum = probatio nova ipsa sub capite VETERE, 31 ignoti
identificatores qui sub novo evanescunt; lectio in
silva_c89_semantica.worklog.md: corpus diff tuum ipsum continet)**;
auspex 3,753/0; officina 10/10 + hospes 78/78 (consumptores trans
proiectum intacti).

**Nota bases novas:** percursus diagnostica 588 in 45 plagulis
(recordata vetus 455+8 — incrementum = plagulae M3 novae + probatio
haec; classificatio causa→gradus = opus chunk C/D iam nominatum).

**Debrief instrumentorum:** adhibita — porta amalgamatoris (cepit
duplicationem enumerationis statim, lineis exactis: manifestum TYPI
egebat), fluxus clangd (cepit API fictam chorda_ex_litteris ante
compilationem), percursus-stash-diff (methodus honesta portae),
python ad percursum sedium (43/43). fructus — porta amalgamatoris
et diff per-plagulam quaestionem numerorum solvit cursu uno.
asperitates — NULLUS modus unius plagulae in percursu: mysterium
numerorum 4 minuta constitit quod effusor unius plagulae secundis
respondisset (demanda effusoris examinis ITERUM crescit — sedes
critica chunk D confirmata). desiderata — nihil novum ultra
effusorem iam nominatum.

**SEQUENS:** commissio Franis = sigillum chunki A; deinde chunk B
(relatio compatibilitatis — functio pura, probatio tabulata).

## 2026-07-11 — EXAMEN CHUNK B INTENTIO: relatio compatibilitatis

Functio pura — sedes NON tanguntur (chunk C eas nectet). Duo ordines
publici novi:

1. **silva_c89_typi_compatibiles(a, b)** — relatio symmetrica C89
   6.1.2.6, pura super TypusC89* internatis. Regulae: ERROR absorbet
   (compatibilis cum omnibus — regula recuperationis, numquam
   re-diagnostica); via celeris identitatis (internamentum); quales
   collecti-exuti-comparati ut COPIAE (_quales_typi), deinde recursio;
   monstrator = pointee compatibilis QUALIBUS INCLUSIS (6.1.2.6);
   acies = elementum compatibile + sentinella mensurae (-1 = ignota,
   compatibilis; ambae notae → aequales); functio = reditus
   compatibilis + (ambae prototypatae: numerus + variadica + parametra
   per paria) + (mixta: reconciliatio K&R — parametra prototypata
   promotione ordinaria immota (_promotum), non variadica); tags
   NOMINALES per identitatem (declarans intra TU — sim-6 C2);
   enumeratus ↔ int compatibiles (6.1.2.5, impl = int in LP64).
2. **silva_c89_assignationem_iudicare(sem, nodus_valoris, finis,
   valoris, codex_out)** → ExamenLicentia {LICET, LICET_CONVERSIO,
   VETITUM} — regula directionalis 6.3.16.1 (sem + nodus pro
   constante nulla). Quales summi finis exuti (conversio valoris);
   arith ← arith = LICET_CONVERSIO semper; monstrator finis:
   constans-nulla → LICET_CONVERSIO; void* utroque modo + inclusio
   qualium; pointee compatibilis (exutus) + quales finis ⊇ valoris →
   LICET (identici) / LICET_CONVERSIO; quales abiecti → VETITUM;
   pointee incompatibilis → VETITUM; arith ↔ monstrator mixti →
   VETITUM; aggregata identica → LICET; residuum → VETITUM.
   DISCIPLINA QUALIUM DIVISA (sim-6 C3): relatio possidet quales
   pointee; iudicium possidet quales summos; locus (chunk C)
   possidet mutabilitatem.

**Codices IV novi** (APPENDIX ante NUMERUS — regula stabilitatis):
ASSIGNATIO_INCOMPATIBILIS, MONSTRATORES_INCOMPATIBILES,
QUALES_ABIECTI, MONSTRATOR_INTEGER — omnes VIOLATIO; non flagrant
ante chunk C (tabula integra, probatio sectionis I eos ambulat
gratis).

**Vectis B:** probatio tabulata in probatio_silva_examen.c (sectiones
novae): ~40+ paria per omnes ramos — decipulae qualium (char* vs
const char* internati separatim: relatio numquam ==), reconciliatio
K&R, sentinella aciei, void*/nulla, tags distincti, enum↔int,
functiones prototypatae/mixtae. Fabricae publicae typos struunt;
tags per parsationem. Amalgama: ordines II + ExamenLicentia in
manifestum + hospes vocationes. Portae solitae (suite, amalgama,
percursus-firing-idem non necessarium — nulla sedes tacta, sed
percursus curritur pro cautela regressionis temporis).

## 2026-07-11 — EXAMEN CHUNK B RELATIO: relatio compatibilitatis SHIPPED

INTENTIO praestita per omnia; nulla emendatio.

**Quod navigavit:** silva_c89_typi_compatibiles (symmetrica 6.1.2.6:
identitas-primum; venenum absorbet; quales ut copiae per gradum;
pointee qualibus inclusis; sentinella aciei -1; functio prototypata/
K&R/mixta — reconciliatio per _promotione_immotum PURUM, sine sem:
char/brevis/fluitans mutantur, cetera manent; tags nominales; enum
↔ int) + silva_c89_assignationem_iudicare (directionalis 6.3.16.1:
quales summi finis exuti; arith semper; constans-nulla; void*; via
vacui + inclusio qualium; vm==f → LICET; aggregata identica; codex
per VETITUM) + codices IV novi (appendix, VIOLATIO omnes, muti ante
chunk C). Sedes INTACTAE (percursus 588/45 IDEM — chunk B nihil
flagrat per constructionem).

**Portae:** compilatio stricta primo cursu; probatio_silva_examen
sectiones VII/VIII novae — **411/411** (paria tabulata: decipula
internamenti char*/const char* per relationem numquam ==;
reconciliatio K&R int/duplex immota vs char/fluitans mutata;
sentinella aciei; dilatatio vs abiectio qualium directionalis;
void* utroque; tags forma-eadem-nomine-diverso incompatibiles;
enum↔int sed non enum↔long; venenum; constans nulla per nodum
VERUM parsatum); suite 30/30; amalgama VERIFICATUM hospes 30/30
(ordo relationis novus — typi monstratorum ex declarationibus
parsatis, NON fabricae exportatae: regula per-tractionem tenuit);
officina 10/10; percursus 100.00%/588/tempus 6.14 ms/KB (basis
6.05 — strepitus, ambulatio intacta).

**Debrief instrumentorum:** adhibita — porta amalgamatoris (cepit
fabricam monstratoris non-publicam in hospite: solutio = typi ex
parsatione, superficies publica NON crevit), python-percursus
mutationum. asperitates — nullae novae. desiderata — nihil.

**SEQUENS:** commissio Franis = sigillum chunki B; deinde chunk C
(sedes: _ad_finem_annotare trifidum + locus + aritas/redde/scalare
+ materializatio est_implicitum + V sedes vocationis novae
relationis — CAVE C4 sim-6: cauda aggregata LEGALIS).

## 2026-07-11 — EXAMEN CHUNK C INTENTIO: sedes (iudicium in ambulatione)

Sedes omnes, semper-activae (DECISUS Q7). Codices VIII novi
(appendix): ASSIGNATIO_NON_LOCUS (V), LOCUS_IMMUTABILIS (V),
NUMERUS_ARGUMENTORUM (V), REDDE_CUM_VALORE_IN_VACUO (V),
REDDE_SINE_VALORE (**SUSPECTUM** — legale C89! probatio clang huius
sessionis: clang-16 AMBOS reicit etiam nudo -std=c89, tertius
casus [-Wreturn-type] auto-promotus; demotio -Wno-error=return-type
CV verum quoque demitteret → ordo EXCLUSIO nominatus ad chunk D),
CONDICIO_NON_SCALARIS (V), VOCATIO_IMPLICITA (**SUSPECTUM primus**),
REDECLARATIO_INCOMPATIBILIS (V). Sedes operatorum codices EXSISTENTES
adhibent (comparatio/subtractio/ternarius — constrictio eadem,
probatio profundior).

Sedes: ① _ad_finem_annotare → iudicare (LICET → nihil;
LICET_CONVERSIO → _conversionem_annotare ut hodie — semitae legales
BYTE-IDEM; VETITUM → diagnosticum + NOLI annotare = conversus
fabricatus ABOLITUS); ② _est_locus + _est_locus_mutabilis
(ambulatio formae in poscente: identificator/chorda-litteralis/
deiectio/subscriptio/membrum-si-obiectum-locus/sagitta; mutabilis =
locus ∧ ¬const-summus ∧ ¬acies ∧ ¬functio ∧ completus ∧ ¬const-per-
catenam-accessus) — consumptores III: ASSIGNATIO sinister (locus
ante finem — cauda una diagnostica), &-operandum (locus AUT
designator functionis), crementum/decrementum; ③ aritas VOCATIONIS
(prototypata solum; variadica = minimum partis fixae; implicita/K&R
exemptae); ④ redde (valor-in-vacuo V; nudum-in-non-vacuo S);
⑤ condicio scalaris (_valorem_typare_scalarem); ⑥ materializatio
est_implicitum (post ambulationem usoris: symbola → VOCATIO_IMPLICITA
ad declarans, ex_systemate exclusa); ⑦ redeclaratio typo-
incompatibilis ad sedem registrationis (cum_socio = declarans prior;
implicita→realis reconciliatio per relationem — f(int) compatibilis,
f(char) non); ⑧ sedes operatorum profundatae: comparatio/subtractio/
ternarius pointee-compatibilitas ubi hodie classis sola probatur
(lectio codicis ante — semita p==0 constantis nullae NON tangenda).

CAVE C4 sim-6: cauda aggregata LEGALIS (struct a = b) → LICET per
relationem, nulla annotatio (ut hodie). CAVE conversus: semitae
legales conversus IDENTICOS dare debent (minimum-diff: annotare
vocatur ut hodie sub LICET*).

**Portae C (tres partes vectis + solitae):** (1) fusor sistere
census ante/post + cursor stdout paritas (probatio subtractionis
fabricati — corpus legale byte-idem currit); (2) percursus: delta
diagnosticorum = SOLUM codices novi; VIOLATIO novae in corpore =
inventa investiganda (exspectatur 0 post systema; sim C9);
(3) tempus percursus ante/post scriptum (iudicium semper-activum).
Suite + amalgama + hospes + officina solitae. Probatio sectiones
novae per sedem.

## 2026-07-11 — EXAMEN CHUNK C RELATIO: sedes SHIPPED (iudicium vivum)

INTENTIO praestita; emendationes duae in opere (clementiae liminum,
infra — ambae inventa corporis, non consilii).

**Quod navigavit:** ① _ad_finem_annotare = sedes iudicii (VETITUM →
diagnosticum + NOLI annotare; LICET* → annotare ut semper — probatio
mutationis intra annotare ordines hodiernos exacte reddit, etiam
lapsum aciei sub LICET); ② _est_locus (ambulatio formae C89:
identificator-obiecti [per symbolum_nodi — constantes enumeri
exclusae], chorda litteralis, deiectio, subscriptio, sagitta,
punctum-si-basis-locus, parenthesis recursiva) + _est_locus_mutabilis
(quales summi — quales basis in membra IAM propagati [inventum:
annotator id fecit]; acies/functio immutabiles); sedes III:
ASSIGNATIO sinister, &-operandum (designator functionis exemptus),
crementa ambo; ③ aritas VOCATIONIS (prototypatae; variadica minimum;
implicitae/K&R exemptae); ④ redde (valor-in-vacuo V; nudum S);
⑤ condicio scalaris in _valorem_typare_scalarem (si/dum/fac/per/
ternarius); ⑥ materializatio est_implicitum → VOCATIO_IMPLICITA
(SUSPECTUM, ad declarans, systema exclusum); ⑦ redeclaratio
typo-incompatibilis cum socio (relatio; implicita→realis per regulas
K&R mixtas); ⑧ comparatio + subtractio profundatae
(_monstratores_comparabiles: pointee exuti compatibiles aut vacuum);
TERNARIUS iam profundus erat (identitas pointee — inventum: sedes
praeexistens). Codices VIII novi; percursus CENSUM CODICUM accepit
(fons veritatis columnae verdicti futurae) + ordines publici II novi
(codicis_causa/severitas — tractio percursus, consumptor primus).

**Inventa primi contactus (lex corporis, puncta nova):**
1. Cauda residua chordam-in-aciem VETUISSET (`char s[] = "ab"`) —
   classis C4 ITERUM: tertium membrum legale in cauda "silenti"
   (aggregata, chorda-init). Sanatio: ramus aciei characterum in
   iudicare (6.5.7).
2. Probationes veteres III fractae: una per classem C4-chordae
   (sanata supra), duae per diagnostica DESIGNATA nova (vocationes
   implicitae in fontibus probationum — exspectationes renovatae
   I→II cum commentis).
3. CORPUS: censum initiale 570 novorum = 377 VOCATIO_IMPLICITA
   (suspectum; lacuna lexici POSIX percursus) + 193 VIOLATIO —
   investigatio per positiones (fructus chunk A!): (a) 8
   REDECLARATIO omnes in systema_c89.h ipso = **SUTURA PARSURARUM:
   tags nominales trans parsuras distincti — lacuna in dissolutione
   sim-6 C2 ad limen systematis**; clementia: compat redecl saltat
   vetus ex_systemate (limen systematis = limen nexus, parca
   trans-TU possidet); (b) 4 LOCUS_IMMUTABILIS = tag
   incompletus-quia-ignotus (struct termios sub lexico ISO);
   clementia: mutabilis clemens in tags incompletis (sedes vera =
   declaratio obiecti incompleti, parca #9); (c) 118+63
   MONSTRATORES/MONSTRATOR_INTEGER post clementias = OMNES
   deorsum-lexici (knotapel includit cruda; instrumenta capita
   POSIX vera; specimen verificatum: DIR* dir = opendir() in
   percursus.c ipso). NULLUM mendum domus; NULLUM falsum positivum
   sedium post clementias. Censum finale: 1146 in 77 (588 vetera +
   377 suspecta + 181 violatio-deorsum-lexici). **Chunk D: vectis
   acceptationis postulat lexicon satis plenum (systema_posix
   exsistit officina-latere) + posturam knotapel (includit cruda) —
   quaestio aperta nominata.**

**Portae:** compilatio stricta; suite silvae 30/30 (probatio_examen
553/553 — sedes omnes + custodes legalium: aggregata, chorda-init,
vacuum*, crementa, &functio, variadica, nulla-comparationes); amalgama
VERIFICATUM hospes 30/30 (ordines accessorum tacti); officina 10/10;
**fusor circuli 596/596 byte-idem, 0 fracti**; **cursor: figura stans
EXACTE — 76 praeteritae/0 sisterae/5 tempora, stdout 69 eaedem/0
DIFFERUNT** (sanatio subtractiva + sedes = invisibiles in codice
valido, per EXSECUTIONEM probatum); percursus coopertura 100.00%
TENETUR, tempus 6.19 ms/KB (basis 6.05 — iudicium semper-activum
~2.3%, scriptum per DECISUS Q7/mensura).

**Debrief instrumentorum:** adhibita — positiones diagnosticorum
(chunk A) pro investigatione corporis (probe temporalis in percursu,
5 lineae, remota); censum codicum (novum, manet); python-sweeps.
fructus — investigatio 193 violationum ex horis in minuta per
positiones + censum; sine iis, caecus fuissem. asperitates — effusor
unius plagulae ITERUM desideratus (tertia vice; chunk D eum fert).
desiderata — nihil novum.

**SEQUENS:** commissio Franis = sigillum chunki C; deinde chunk D
(effusor examen.sh + corpus invalidum + vectis differentialis;
quaestiones apertae: lexicon corporis-acceptationis, postura
knotapel, demotio return-type in oraculo).

## 2026-07-11 — EXAMEN CHUNK D INTENTIO: effusor + corpus invalidum + VECTIS

① **examen.c/examen.sh** (exemplar censor.sh; recipe percursus
FIDELE: contextus + capita praebita + lexicon systematis + oraculum
praeseminatum + BIS-ANALYSIS post recanonicare — sem2 auctoritativum).
Usus: `./silva/examen.sh <plagula.c> [-posix|-nudum] [-machina]`.
Lexicon: ordinarius = systema_c89.h; -posix = + systema_posix.h
(concatenatio, exemplar vindex_onerator; AMBAE silva/fontes!);
-nudum = sine (probat LEXICON_DEEST). Exitus: 0 ACCIPE / 1 REICE /
2 INFRA. Verdictum: REICE = violatio non-provisionalis ULLA aut
nodi erroris (syntaxis = reiectio C89); INFRA = apparatus solum.
-machina = TSV {via, linea, columna, severitas, codex,
provisionale, causa} + ordo VERDICTUM. Geminus tertius oneratorum
nascitur (migratio parca crescit).
② Effusor = instrumentum investigationis: demo_85 + tcp_posix
leguntur PRIMO (quaestio lexici corporis-acceptationis empirice
solvitur, non coniectura).
③ **Percursus fugarum oraculi** (DECISUS 9) ante benedictionem:
fixturae C99/GNU (~12 classes) contra chordam sigillatam — omnes
REICI debent; fugae → emendationes chordae (in worklog).
④ **Corpus invalidum**: silva/probationes/fixa/examinis/*.c —
sine-inclusionibus; nota `/* EXSPECTA linea:CODEX_NOMEN */`;
extractor grep/sed (numquam silva); benedictio contra oraculum.
⑤ **examen_vectis.sh**: modus ordinarius = percursus fugarum +
corpus invalidum differentialis (examen REICE ad pinnas + oraculum
REICE consentiens; versio clang scripta); modus -corpus = latus
examinis super corpus verum (columna verdicti percursus) —
postura lexici ex ② decisa. Ordines EXCLUSIO pro classibus dilatis
(vectis D conditio 5): redde-sine-valore (clang auto-promotum),
classes parcatae (switch/initiatores/...).
⑥ Columna verdicti percursus (DECISUS 12): [verdictum REICE] per
plagulam + summa.

## 2026-07-11 — EXAMEN CHUNK D RELATIO: effusor + VECTIS — BAR M4a TENET

**Quod navigavit:** ① silva/examen.sh + instrumenta/principalia/
examen.c (recipe percursus fidele: contextus + capita praebita +
lexicon + oraculum praeseminatum + bis-analysis; ISO ordinarius /
-posix concatenatus / -nudum; -machina TSV; ACCIPE/REICE/INFRA;
geminus TERTIUS oneratorum — migratio parca crescit); ② corpus
invalidum silva/probationes/fixa/examinis/ — XIV fixturae
.invalidum (suffixum extra corpus percursus — lectio corporis
sui-referentis ITERUM: fixturae in tabulam exclusionum inciderant;
praecedens .fragor censoris) cum pinnis EXSPECTA linea:CODEX_NOMEN;
XII fugae .fuga (C99/GNU contra oraculum); ③ examen_vectis.sh =
VECTIS: percursus fugarum + pinnae differentiales (mappa
nomen→numerus ex capite grep/awk — extractor numquam silva) +
modus -corpus (REICE percursus vs exclusiones.txt pinnatae);
④ columna verdicti percursus (DECISUS 12) + probatio negativa
pinnae (dens vectis probatus).

**INVENTUM CORONAE — divergentia differentialis PRIMA, in nostra
relatione:** examen demo_85 REICIEBAT (52 violationes), oraculum
ACCIPIEBAT. Classis: `const int (*)[N] ← int (*)[N]` — additio
qualium tuta PER GRADUS ACIERUM (littera C89 strictior; praxis +
clang universales). Sanatio: _pointee_assignabilis (inclusio
qualium descendens per acies, compatibilitas infra; comparabiles
quales negligit utroque modo). Post: demo_85 ACCIPE **nulla
diagnostica**; monstratores corporis 118→3; probatio 553/553 tenet
(directio abiectionis adhuc capta). Vectis suum munus fecit die
primo.

**Etiam:** oraculum eget `-x c` pro plagulis non-.c (nota
calibrationis — clang taciturne accipit suffixa ignota sub
-fsyntax-only!); fugae 0/12 (obturamentum designatoris probatum).

**VECTIS M4a (spec §IV bar D) — TENET:**
1. corpus verum: 743/782 ACCIPE; 39 REICE = exclusiones PINNATAE
   (omnes classes lexici nominatae: POSIX-profunda [rete/dirent/
   signal], knotapel includit-cruda residua, fixturae consulto
   fractae [flagrationes censoris], probationes parsatorias);
   oraculum totum corpus accipit (aedificatio domus quotidiana);
   diff exclusionum = porta stans (modus -corpus).
2. corpus invalidum: XIV/XIV REICE ad pinnas (linea, codex);
   oraculum consentit XIV/XIV.
3. percursus fugarum ANTE benedictionem: 0 fugae.
4. columna verdicti stans (+ censum codicum).
5. EXCLUSIO nominatae: exclusiones.txt (39, causis); classis
   redde-sine-valore = exclusio fixturarum nominata (examen
   suspectum-accipit, clang auto-promotus reicit — fixtura addetur
   cum demotione oraculi decisa; sedes in vectis parata).

**Portae solitae:** silva 30/30; amalgama VERIFICATUM hospes 30/30;
officina 10/10; **cursor figura stans EXACTE (76/0/5; stdout 69
eaedem/0 DIFFERUNT)** — sanatio aciei-qualium in codice valido
invisibilis, per exsecutionem.

**Debrief:** adhibita — effusor NOVUS ut instrumentum investigationis
(demo_85/tcp_posix intra minuta — desideratum tertiae vicis IMPLETUM
die suo primo); oraculum empirice (exit-post-pipe decipula: `$?`
post `| head` = exitus head!). fructus — divergentia prima inventa
et sanata die uno. asperitates — nulla. desiderata — vectis
-corpus lentus (percursus plenus ~2 min; satis pro portae stantis).

**SEQUENS: commissio Franis = sigillum chunki D. Deinde RELATIO
MILLIARII M4a (relectio plena phase-log + auditus, regula silvae)
— sessione recenti si contextus deest.**

## 2026-07-11 — M4a EXAMEN: RELATIO MILLIARII (auditus + sigillum)

**QUATTUOR CHUNKI UNO DIE.** Fistula iudicii: fons → silva →
semantica (typatio + IUDICIUM) → verdictum. Effusor
./silva/examen.sh; vectis ./silva/examen_vectis.sh; columna
verdicti + censum codicum in percursu.

**AUDITUS (spec/fundamenta/sim-6 contra rem):**
- Spec chunk A-D deliverabilia OMNIA praestita; emendationes V, omnes
  cum ratione IN LIBRIS scriptae ante aut in opere: ① resolutor
  positionum DISSOLUTUS (materializatio in additione); ② INFRA
  gradus quartus (generalizat LEXICON_DEEST sigillatum; Fran
  benedixit); ③ clementia tags incompletorum (incompletus-quia-
  ignotus); ④ clementia redecl trans limen systematis (sutura
  parsurarum); ⑤ pinnae per NOMINA codicum non numeros (mappa
  derivata; legibilitas).
- Interrogationis XV DECISUS omnes honorati. Fundamenta §III
  quaestiones XIII omnes clausae (Q3 chorda oraculi + emendatio
  `-x c`; Q4 mappa 245-residui = per constructionem: violatio extra
  exclusiones = 0).
- Sim-6 C1-C11: NULLA reversio; C3 (disciplina qualium)
  PROFUNDATA inventis (quales per gradus acierum); C4 (cauda
  legalis) membrum TERTIUM invenit (chorda-init); C2 (nominalis)
  foramen ad suturam systematis invenit.
- Vectis bar D: restatum honeste — "corpus 100% ACCIPE" = 100%
  corporis lexico-coperti; 39 exclusiones PINNATAE cum causis
  (diff = porta stans), praecedens tabulae stdout.

**LEX CONTACTUS CORPORIS, puncta huius milliarii:** cauda
chordam-in-aciem vetuisset (probationes veteres ceperunt); sutura
parsurarum in systema_c89.h ipso; incompletus-quia-ignotus in
tessera; classis aciei-qualium (INVENTUM DIFFERENTIALE PRIMUM —
oraculum et examen inter se die primo calibraverunt: 118→3
monstratores, demo_85 52→0); corpus sui-referens BIS (probatio
nova sub capite vetere; fixturae invalidae in tabula exclusionum).

**PARCAE NOVAE (cum dominis, → c89-lacunae.md):** operanda
scalaria &&/||/! non probata (sedes exsistens, tractio fixturae);
& repositionis registri (CV minima); typedef-redecl eodem scopo;
demotio return-type oraculi (implexa: -Wno-error=return-type CV
verum quoque demitteret — fixtura redde-sine-valore addetur cum
decisione); crescentia lexici (systema_rete? — 39 exclusiones
minuere); geminus tertius oneratorum (migratio tactu proximo).

**DEBRIEF INSTRUMENTORUM MILLIARII:** effusor examen = desideratum
tertiae-vicis impletum, investigator die primo (demo_85/tcp_posix
minuta non horae); positiones chunk A ubique fructiferae
(investigatio 193 violationum); censum codicum percursus = fons
veritatis; porta amalgamatoris + probationes veteres + vectis =
retia stratificata, quodque cepit quod alia non viderunt (exemplar
M3 iteratum). Asperitas una: percursus plenus 2 min in ansa
stash-diff (effusor eam iam solvit).

**M4a SIGILLATUM = commissio Franis huius RELATIO.** EXAMEN vivit:
semper-activum in analysi, effusore, vecte, columna. Consumptores
parati: M4b REPL (iudicium ante demissionem), silva-lsp (lamina
diagnostica), probatio codicis recentis. SEQUENS: M4b REPL
(on-ramp arcu probato, sessione recenti; vindex_onerator =
onerator; mundus incrementalis + reconciliatio implicita→realis =
quaestiones primae).

## 2026-07-11 — EXAMEN-CUSTOS NATUS (uncus post-editionem) — pointer entry

Sequela immediata M4a (Fran: "sooner rather than later"). Uncus
PostToolUse .claude/hooks/examen-custos.sh (praecedens
censor-custos): iudicium C89 in omnem plagulam .c modo scriptam,
canalis PUSH. Disciplina: silentium sub puritate (0.5s, vilior
censore); exclusiones lexici pinnatae TACENT (strepitus
deorsum-lexici doceret negligere); infra suppressum; scratchpad/
build/fixa/amalgama exclusa; capita .h per includentes. Probatum:
exclusa tacent, violatio flagrat cum positione exacta, purum tacet.
DUM CLANGD QUOQUE CURRIT = DIFFERENTIALIS VIVUS in sessionibus
veris — calibratio contra incumbentem ante substitutionem lsp
(scopus agentis: diagnostica push = ~95% valoris clangd; navigatio
iam per subsellium [nexus/emitte]). Armatur sessione proxima
(oneratio settings). Detail: .claude/hooks/examen-custos.sh.

## 2026-07-13 — SILVA-LSP ON-RAMP, GRADUS I: exploratores + fundamenta

Furca resoluta (officina/phase-log.md FURCA RESOLUTA): Fran
silva-lsp elegit prae M5; ratio = usus cotidianus agentis = machina
poliendi. Libri silva-latere (praecedens M4a). Exploratores IV
(superficies protocolli / daemon calidus / positio→extentum /
integratio clientis) → **project-specs/silva-lsp-fundamenta.md**
(34 facta onerifera + mensurae + R1-R7 + quaestiones apertae).

QUATTUOR REFRAMATIONES (plenae in fundamentis):
① VALOR MOTUS EST: canalis diagnosticorum LSP = editione-scopatus,
EADEM superficies quam uncus examen-custos iam possidet — v0
diagnostica-solum duplicaret canalem exsistentem. Additivum verum =
instrumentum LSP nativum (definitio/references/hover — renderer
typorum M4b consumptorem naturalem invenit) + consolidatio unci.
② CLANGD CONTRA NOS NUNC CURRIT: plugin clangd-lsp activum
globaliter; silentium = una linea settings (gratis, sine codice) —
sed differentialem vivum terminat. Decisio Franis.
③ AMORTISATIO IMPEDITA: contextus calidus TEXTUM capitum servat,
non LEXEMATA — _contextum_applicare omnia 184 capita RE-LEXAT
quaque analysi (~0.10s ex ~0.15s solo frigido) ⇒ daemon calidus
receptu immutato = ~1.4x, non ~7x. Lucrum verum = includenda
prae-lexata calida in contextu (mutatio silva-latere; fortasse
"migratio fontes-lateris tactu proximo" adveniens). R1 = DECISIO
CENTRALIS.
④ PROBLEMA JSON NON EXSTAT: lib/json.c completum (1859 lineae,
parse+scribere+evasio recta, suita 553-linearum). Uti ut est.

INVENTA CETERA: columnae silvae = OCTETI 1-basati ⇒ positionEncoding
utf-8 = conversio nulla (inversio intuitionis utf-16!); extensio
~2-linearum (radix->longitudo computatum-tum-abiectum ad
semantica.c:328 — campus + assignatio = extensio honesta omni
diagnostico semantico); honestas macrorum IAM operatur (catena
radicis originis); ERRORES SYNTAXIS SINE POSITIONE (0:0 duratum in
examen.c — lacuna maxima UX, status frequentissimus editoris vivi;
positiones in nodis erroris parsurae exsistunt = opus
parser-lateris, scopus decidendus); aedificare super AMALGAMA
(exemplar vindicis — praeparator silva.h solum in amalgamate
invenit, DECISUM non apertum); piscina ephemera per analysin =
exemplar probatum (vindex_onerator.c:110-138 = formula); OOM =
exire(I) daemonem necat (restartOnCrash clientis = mitigatio
probabilis); sessio NON necessaria (iudex-solum sine regione, UNA
SESSIO EXSECUTANS non ligat).

SEQUENS: colloquium designi cum Frane super quaestiones apertas
(centrum gravitatis diagnostica-vs-navigatio; clangd-lsp nunc?;
R1 in scopo?; dispositio unci; positiones errorum syntaxis; nomen
ceremoniae) → simulatio → interrogatio → spec.

## NOTA CONTRACTUS: symbola = registrationes (2026-07-14)

Commentarium contractus additum SemanticaSymbolo in silva.h +
silva_c89_semantica.h (mutatio nulla codicis): declaratio quaeque
symbolum PROPRIUM registrat — prototypus + definitio = symbola duo;
enumeratio omnia reddit; consumptor entia volens plicet, definitione
praelata (probatio per accessorem corporis, contractus generis
alieni). Causa: "umbra prototypi" in legato (officina/phase-log
2026-07-14) — consumptor semantica tabulae-fusae praesumpsit,
diagnosis prima falso mutationem silvae postulabat. ACTA SUNT
FRUCTUS iam macris valebat; nunc symbolis quoque EXPLICITUM.
Si consumptor SECUNDUS plicationem entium umquam eget (vindex,
saltus-typi saltuarii, :definitio quaestionis), visus entium
silva-lateris = additio nominata (acta intacta).

## NEXUS -RENOVARE CELER + SCALA CELERITATIS TSV (2026-07-14)

Fenestra post-commissionem (LEGATI gradus II caret signo dum
percursus in fundo currit, ~143s) → mensura + scala. RUNG 1
NAVIS: -renovare variante celeri (-O2 -flto, exemplar cursor
-celer; obiecta build/celer/ numquam mixta; vexillum celer_novum =
religatio cum obiecto quovis recompilato [lectio compile_tests
decorativi]; conditiones staleness plenae — recompilatio
incondicionata vetus etiam sanata). MENSURATUM: 143s → 52s (2.75x);
compilatio stricta + O2 pura; PARITAS OCTETIM ambabus tabulis
contra -O0 (barra: cursus incrementalis/optimatus tabulam
byte-idem cursui pleno reddere DEBET — facile probanda, fraudi
impossibilis). Effigies temporis: ~37s user / ~16s sys — sys =
tumultus mmap (piscinae 2×8MB creatae/destructae per plagulam
×800).

SCALA RELIQUA (parca nominata, ordine sumptus):
② reusus piscinarum trans plagulas (textus: piscina_vacare exstat;
arboris: silva_piscina_vacare exportandum = ordo unus silva.h) —
sys 16s → ~2s; ③ sem2 condicionalis: silva_recanonicare numerum
mutationum REDDIT (nunc abiectum) — 0 mutationes = sem1 auctoritas,
transitus alter omissus (semantics-praeservans constructione;
plerisque plagulis ambiguitas nulla) — fors ~30-40% user; ④ furcae
(exemplar cursor fork-per-suite; tabulae fragmentatae + concatenatio
+ sort -u inclusionibus) — ~nuclei×; ⑤ INCREMENTALE (finis):
plagulae mtime > stampa GENERATUM ∪ clausura reversa capitum
recentiorum (BFS inclusiones.tsv = computatio excubitoris!),
old-tsv stream-merge (necatio viarum re-iudicatarum/deletarum +
appendix novarum; deletio = stat existentiae per viam distinctam);
commissio typica 1-5 plagulae ≈ 1-2s — fenestra evanescit. Barra
paritatis supra pro omnibus gradibus. Convergentia AEDILIS:
computatio copiae-laboris clausurae = quod aedilis possidebit.

## SCALA TSV RUNGS 2+3 — RELATIO HONESTA (2026-07-14)

**RUNG 2 TEMPTATUM ET REVERSUM.** piscina_vacare non est reversio
vilis: _catena_alveus_vacare alvei OMNES memset-purgat (contractus
— allocare in memoriam pool-purgatam confidit; varians
non-purgans classem praesumptionis "memoria nata purgata" per
omnes vocatores frangeret = non sanatio localis). Reusus ergo =
purgatio catenae ad plagulam maximam crescentis x800 = 52s → 306s
(sys 16s → 218s) — PESSIMATIO 6x. Reversum; lectio in commentario
ad sedem. EXPORTATIO silva_piscina_vacare MANET (API legitima,
piscinis parvis; amalgama regenerata, portae VERIFICATUM 31/31 +
officina 78/78). Falsa alarma paritatis obiter: tabulas trans
ARBORES DIVERSAS comparaveram (editiones meae ipsae in ordinibus
— honestissime); barra paritatis ARBOREM EANDEM postulat.

**RUNG 3 NAVIS.** sem2 conditionale (recanonicare mutationes > 0);
PARITAS ARBORE EADEM PROBATA (varians sem2-semper ex fonte
correcto aedificata [sed prima laxa duas lineas alienas cepit —
correctio chirurgica python], ambae tabulae BYTE-IDEM).
CALIBRATIO CONTRA AESTIMATIONEM: sem2 in 1/799 plagularum SOLUM
accendit, sed lucrum ~3s (52s → 49s) non 30-40% — semantica vilis
est, PARSURA dominatur (~85% user). Scala revisa: rung 4 (furcae)
~nuclei× in 49s → ~7-10s; rung 5 (INCREMENTALE) IMMUTATUM et nunc
solum rung qui re vera valet — commissio typica 1-5 plagulae ≈
1-2s. Status: 143s (initium) → 49s (rungs 1+3); fenestra
post-commissionem iam 3x brevior.

## RUNG 5 — PERCURSUS INCREMENTALIS: INTENTIO (2026-07-14, contextu calido scripta)

*Captura conscia: haec INTENTIO plenior solito est quia sessio
calida subtilitates tenet quas sessio frigida re-derivare deberet.
NOLI implementare ex compendio compactionis — hoc introitum lege.*

PROPOSITUM: regeneratio tsv post commissionem ~49s → ~1-2s
(commissio typica 1-5 plagulae). Fenestra staleness LEGATI gradus
II evanescit. Cache residentis MANET (onus 48MB per quaestionem =
1s quam mundus calidus vitat); quod moritur est FENESTRA.

FORMA (quattuor partes):

**① ORDO CANONICUS (praecursor, pars propria).** Ambulatio
readdir NON deterministica trans mutationes directoriorum — ordo
tsv hodiernus = fortuna APFS. Lista viarum SORTETUR ante iudicia
(qsort viae). Consecutio: ordo plenus-contra-plenum stabilis in
aeternum; paritas incrementalis-contra-plenum possibilis octetim.
RE-PIN semel: tsv ordine novo (diff unica contra praeteritum).
CAVE: ordo ordinum tsv = ordo catenarum legati (sedes "prima" in
ambulationibus catenae; politica graduum plerumque vincit, sed
paria ordine solvuntur) — SUITA OFFICINA post re-pin currenda
(aureae legati sensilia ordinis detegent).

**② COPIA LABORIS (quid re-iudicandum).**
- Aqua-signum = stampa GENERATUM INTERNA tabulae veteris (numquam
  mtime tabulae — lectio venenationis debounce: mtime falsificabilis
  per touch; stampa sola cum regeneratione vera mutatur).
- (a) plagulae .c/.h quarum mtime >= stampa (aequaevum = suspectum,
  polaritas _via_recens non vigiliae; re-iudicium superfluum vile).
- (b) CLAUSURA REVERSA: includentes capitum mutatorum — BFS super
  inclusiones.tsv VETEREM inversam (ad → ex; index inversus in
  memoria). ARGUMENTUM SANITATIS GRAPHI VETERIS (scribe in codice):
  plagula G quae caput novum I per caput vetus H trahit — H mutatum
  est (mtime >= stampa) → includentes H per graphum VETEREM G
  continent (margo G→H vetus exstat). Plagula quae includes SUOS
  mutavit = mtime proprio recentior → in (a). Casus omnes tecti.
- (c) DELETAE: viae distinctae tabulae veteris quae in disco non
  exstant (stat per viam, ~800, ~ms) → ordines cadunt. Renominatio
  = deletio + nova, tecta.
- (d) NOVAE: ambulatio directoriorum (necessaria utcumque) plagulas
  ignotas tabulae veteri invenit → iudicandae.
- Externa (basenames in graphō: stdio.h...) numquam in copia.

**③ FUSIO FLUMINIS (quomodo tabula nova scribitur).**
- Tabula vetus in memoriam onerata, ordines per viam aggregati
  (48MB nihil est; piscina).
- Emissio in ORDINE CANONICO viarum: via in copia laboris → iudica
  et emitte ordines recentes; via retenta → ordines veteres
  verbatim; via deleta → nihil. PARITAS OCTETIM ita conservatur
  (non appendix-ad-finem, quae ordinem frangeret).
- DEDUP TUTUM: clavis ordinum = titulus\tspecies\tVIA\tlinea\tcolumna
  (via inclusa — collisio trans plagulas IMPOSSIBILIS; verificatum
  2026-07-14 percursus:106); clavis inclusionum = ex|ad|status (ex
  inclusum, idem argumentum). Dedup intra-copiam sufficit.
- inclusiones.tsv EODEM tractatu (ex ∈ copia∪deletae cadunt, ordo
  canonicus, stampa eadem).
- Stampa nova = tempus ANTE ambulationem captum (conservativum:
  editiones durante cursu proximo cursui cadunt).
- Scriptura atomica .nova + rename UTRIUSQUE (mos exstans).

**④ VECTES + HONESTAS.**
- Modus incrementalis = ORDINARIUS quando tabula vetus cum stampa
  legibilis exstat ET inclusiones.tsv exstat; -plenus vim facit;
  defectus quivis (stampa absens, tabula corrupta) → plenus cum
  nota stderr (numquam tacite).
- Statisticae: "incrementale: N re-iudicatae (M per clausuram
  capitum), K deletae, L novae; ordines retenti R" — lex
  nullorum-tectorum-tacitorum.
- Uncus IMMUTATUS (nexus.sh -renovare idem; debounce stampae manet
  600s — cum 1-2s cursu fors minui potest, postea).

BARRAE (omnes arbore EADEM — lectio falsae alarmae 2026-07-14:
comparatio trans arbores editiones proprias "regressionem" fingit):
① nulla-mutatio: incrementalis = transitus purus, tabula
byte-idem, <3s; ② editio unius .c → paritas octetim contra
-plenus; ③ editio CAPITIS (silva.h!) → clausura magna, paritas;
④ deletio plagulae → paritas; ⑤ plagula nova → paritas; ⑥ suita
officina viridis post re-pin ordinis canonici (sensibilitas
catenarum legati). Aestimatio: ~300 lineae in percursu; floor
temporis = systema+capita startup (~2s) + iudicia copiae.

CONVERGENTIA AEDILIS: computatio copiae per clausuram = quod
aedilis -vigila possidebit; forma huius codicis eam praefigurat.

## RUNG 5 NAVIS — PERCURSUS INCREMENTALIS: RELATIO (2026-07-14)

INTENTIO (supra) implementata sessione post compactionem — captura
contextu-calido TENUIT: reversio consilii NULLA aedificatione tota.
Partes (1)-(4) omnes navis; barrae 6/6 (omnes arbore EADEM):

① nulla-mutatio: "incrementale: nulla mutatio - tabulae intactae"
  0.3s, tabulae NON scriptae (stampa manet - residens legati non
  recargat frustra); ② editio .c unius: 0.5s, PARITAS OCTETIM
  ambabus tabulis; ③ editio capitis (legatus.h): 4 re-iudicatae =
  caput + clausura exacta (3 includentes ex grapho), 0.7s, paritas;
  reversio symmetrica; ④ deletio: greges cadunt, paritas, 0.2s;
  ⑤ nova + RENOMINATIO (mv = 1 deleta + 1 nova uno cursu), paritas;
  ⑥ suitae officina 12/12 + silva 30/30 post re-pin ordinis canonici
  (sensibilitas catenarum legati non momordit).

RE-PIN (1): ordines nexus IAM sui-attributi (796/796 cursus contigui
- fundere basenames omittens id tacite fecerat); margines RE-PINNATI
per-ex (8 margines conditionales evanidi, 8/8 ad externa = clausurae
innocui, LIMES NOMINATUS in worklog). Basename duplicata (5 paria
include/ vs fixturae) nunc deterministice include/ vincunt.

SUPRA INTENTIONEM: lapides "# vacua <via>" (plagulae vacuae ne novae
aeternae fiant - sine eis cursus quietus tabulas rescribebat et
stampam movebat = recargationes residentis frustra); custos
instrumenti in nexus.sh (obiecta celer recompilata -> -plenus
coactum: tabula mixta duorum parsatorum numquam nascitur); via
intactarum-tabularum (cursus quietus NIHIL scribit).

CALIBRATIO: floor aestimatus ~2s, mensuratus ~0.3s (praebere capitum
= repositio textus sine parsura). Commissio typica nunc sub-secundo
- fenestra staleness legati gradus II de facto EVANUIT (aestimatio
INTENTIO: "1-2s" - realitas melior 4x).

INVENTUM OBITER (praeexsistens): plagulae .c radicis repositorii
ordinibus invisibiles (via sine '/' = heuristica basename fundere).
Mos repositorii eas non habet; worklog notat.

PARCUM: debounce unci 600s minui potest (cursus 0.3s); furcae rung 4
nunc vix valent (49s plenus rarus fit - post commissiones
instrumentorum solum).

DEBRIEF INSTRUMENTORUM: adhibita examen.sh (specimen "cur sine
ordinibus?" - verdictum ACCIPE uno ictu diagnosim ad fundere vertit),
excubitor -tacitus (status pools post barras), suitae ambae, awk/comm
super tabulas (contiguitas 796/796 + diff parium marginum - quaestio
"quot cursus per clavem" instrumento proprio caret sed awk sufficit).
FRUCTUS: bench + tabulae ipsae = auditus re-pin sine instrumento novo.
ASPERITAS: nulla nova (via_activa globalis = exemplar receptoris
existens). DESIDERATUM: nihil novum - sonda una-ictus manet parcum
vetus.

## COMMENTARIUM DUCENS — INTENTIO (2026-07-14, contextu calido)

PROPOSITUM: consumptor PRIMUS pinnae "commenta sunt contenta"
(VISIO - pinna quam CLAUDE.md "facillime complanatam" vocat).
Accessor silva-lateris: commentarium ducens ("arcte-supra" ex
arbor2-comment-spec) nodi declarationis -> extenta; legatus chartas
symbolorum eo ditat (contractus in commentario capitis = quod grep
hodie invenit et instrumenta non - debrief rung 5).

FACTA RECOGNITIONIS (lege ante implementationem):
- Politica lexatoris (silva_lexema.c ~950): trivia inter lexemata
  vera DIVIDUNTUR ad primam NOVA_LINEA - ante eam ad spatia_post
  prioris (caudalia), post eam ad spatia_ante sequentis. ERGO
  spatia_ante declarationis = trivia sui-lineae SOLA; commenta
  finis-lineae enuntiati prioris iam exclusa CONSTRUCTIONE.
- NOVA_LINEA = lexema triviae PROPRIUM -> linea vacua = duae NL
  consecutivae inter commentum et ancoram (SPATIA/TABULAE inter
  eas neglectis). Regula arcte-supra = ambulatio spatia_ante
  RETRO: NL numerare (>=II frange), commenta colligere (numerum
  NL ad nihilum retexere), SPATIA/TABULAE/CONTINUATIO transilire.
- SUBTILITAS RADICIS: declarationes latinae macris incipiunt
  (hic_manens...) - lexema primum nodi = expansio SYNTHETICA
  (byte_offset -1, sine triviis). silva_token_radix -> lexema
  invocationis in fonte scripto, quod spatia_ante FERT. Idem
  ambulatus quo silva_valor_extensionem ("macros latinae!" in
  commentario eius - silva_nodus.c:~365).
- Commentum clausum multi-linea = lexema UNUM (extentum =
  byte_offset..+longitudo).

FORMA:
① silva: SilvaCommentariumVista {initium, finis (exclusivum),
  linea; -1 = absens} + silva_commentarium_ducens(nodus,
  fons_index, vista*) in silva_nodus.c iuxta extensionem
  (recursio locorum eadem, lexema minimi offsetus radice soluto,
  deinde ambulatio spatia_ante retro). silva.h manu extenditur;
  amalgamare + portae.
② legatus: charta (LegatusFunctioExtentum) tria campa nova ad
  tempus iudicii (exemplar macro-vistae: extenta tunc, octeti ad
  tempus reddendi verbatim, documento-aperto-primum); symbolum
  commentarium sub sede quaque (corpus + caput, ambo si differunt)
  reddit, tectum ~XXIV lineis ("..."). Hover LSP = vector si vilis
  in transitu, alioquin sequela nominata.

BARRAE: ① probatio silvae accessoris (attachatum simplex /
multi-commentum / linea vacua = absens / finis-lineae prioris =
absens / declaratio hic_manens-initialis [radix] / caput plagulae
sine vacua = attachatum); ② portae amalgamatis VERIFICATUM;
③ aureae legati (symbolum piscina_generare_dynamicum fert
commentarium notum ex piscina.h); ④ suitae silva + officina;
⑤ sonda viva legatus.sh -mcp.

CAVE: separatores vexilliformes (/* ==== */) supra declarationem
sine linea vacua ATTACHANTUR - honestum (regula est regula);
tectum lineae longa commenta compescit.

## COMMENTARIUM DUCENS — RELATIO (2026-07-14)

NAVIS eodem die. Accessor silva_commentarium_ducens in silva_nodus.c
(recursio locorum extensionis + ambulatio spatia_ante retro);
SilvaCommentariumVista manu in silva.h + CADENDA_TYPEDEF
amalgamatoris (lectio: typus publicus novus = introitus manifesti,
alioquin typedef duplicatum in silva.c generato). Probationes 6/6
(attachatum/vacua-linea/multi-bloccus/vacua-intra/finis-lineae/
macro-radix); amalgamare VERIFICATUM; suitae 30/30 + 12/12
(aureae legati 252).

INVENTUM PRINCIPALE (implementatio, non consilium): **declarans
prototypi = nodus DECLARATORIS** (a titulo incipit - specificatores
cum triviis EXTRA nodum), definitionis = nodus definitionis totus.
Accessor recte anchorat sed nodus datus falsus erat - casus capitis
(contractus!) invisibilis. Sanatio legatus-latere: ASCENSUS PATRIS
ad radicem ante vocationem (chartae profunditatis 0 solae, ergo
radix = declaratio continens semper - argumentum in commentario ad
sedem). Regula divisionis triviarum verificata obiter: NL primus
gap-i in spatia_post prioris cadit - regula "II NL consecutivae"
tenet quia bloccus-cum-vacua [C,NL,NL] in spatia_ante manet.

CHARTA VIVA: symbolum nexus_ordines_fundere nunc contractum capitis
fert (ipsissimum commentarium "viae sine '/' omissae" quod rung 5
hodie mane grep-avit - instrumentum factum quod deerat). Vexillum
supra definitionem linea vacua separatum NON attachatur (aurea
negativa). Corpus + caput ambo si differunt; tectum XXIV lineis.

PARCUM NOMINATUM: hover LSP idem commentarium ferre potest (charta
eadem, sedes reddendi altera) - sequela cum primo desiderio vero.
Macra sine commentario ducente manent (corpus #define iam habent).

DEBRIEF INSTRUMENTORUM: adhibita impulsus legati (×4 hoc fasce:
accessus-non-structurae ante declarationem vistae, vocatio implicita
_deb_min_tok, syntaxis fracta editionis meae pravae, arg-ordo -
SINGULA capta ad editionem), sonda amalgamatis (diagnosis reversa:
silva recta, nodus falsus), sonda -mcp viva, suitae. FRUCTUS: sonda
standalone contra amalgama = bisectio silva-vs-legatus uno ictu.
ASPERITAS NOVA: silva.h VETUS in scratchpad sessionis prioris
umbravit includem citatam ("silva.h" quaerit directorium plagulae
includentis PRIMUM) - sonda phantasma-errores dedit; sanatio
includem angulatam; lectio in debugging-lessons (varians umbrae
stalae). DESIDERATUM: nihil novum.

## MACRO DOMESTICUM IN CAPITE ALIENO — INTENTIO (2026-07-14, contextu calido; exemplar rung-5 captura-tunc-implementa)

PROPOSITUM: diagnosticum examinis quando macro DOMESTICUM in textu
capitis VENDICATI expanditur — classis "char C -> char 100"
(sqlite3.h hodie; stb ante). Subcasus tacitus quem solum origo
capit: expansio quae COMPILAT (parametrum arithmeticum -> litera
numerica = responsum pravum, non error). Censor-varians DECLINATA
ut supervacua (impulsus ad editionem = tutela vera; inventarium
dormiens nihil emit; veritas una, domus una).

REGULA (generalis, non latina-sola): expansio cuius sedes
invocationis in fonte ALIENO est (via fontis incipit "vendor/" aut
continet "/vendor/") ET cuius definitio in fonte NON alieno -> 
diagnosticum. Vendicatum-in-vendicato tacet; domesticum-in-alieno
clamat. Codices vendor numquam macros nostros consulto adhibent -
falsa positiva nulla exspectata.

DECIPULA PORTANS (hoc solum contextus calidus scit): definitio
macrorum latinae in mundo iudicis NON in include/latina.h sedet -
LEXICON latina in TEXTUM SYSTEMATIS concatenat (inventum M4b:
"canalis macrorum"; praeparator cum_latina). Ergo test definitionis
= "fons definitionis non alienus" (systema/lexicon/repositorium
omnes non-alieni), NON "fons == latina.h". Regula generalis supra
hoc gratis tractat.

FORMA:
- Sedes: examen (silva-latere, semantica/effusor M4a) - impulsus
  LSP + diagnostica MCP octetim-identica gratis.
- Ambulatio: lexemata TU cum catenis originis (exemplar
  nexus_ordines_titulus_macronis - ascensus ad invocationem);
  quaeque expansio: fons invocationis alienus? fons definitionis
  non alienus? -> collige. Emissio SEMEL per par (titulus macronis,
  via fontis alieni).
- Positio: diagnosticum in plagula iudicata 1:1 (directiva
  inclusionis linea in vista NON est - confirmatum hodie); textus
  omnia nominat: "macro domesticum 'C' in capite vendicato expansum
  (vendor/sqlite3.h:8883) - caput vendicatum ANTE latina.h
  includendum". Gradus: SUSPECTUM (significationem tacite mutat).
- Codex novus -> census codicum aureae renovandus; nomen codicis
  INSIGILLATUM (sessio implementans conventiones codicum examinis
  in fonte consulat ante cudendum).

BARRAE:
① fixtura positiva: fixa/examinis/vendor/<specimen>.h (via "/vendor/"
  continet - regula viae ita exercetur; praebere basename-clavatum
  specimen unde-cumque resolvit) cum identificatore collidenti +
  .c quod latina.h ANTE specimen includit -> SUSPECTUM emissum;
② ordo rectus (specimen ANTE latina) -> TACET;
③ corpus sweep (examen_vectis -corpus): ZEPHYRUM inventa nova
  exspectata (imago.c stb-ante-latina recta; scrinium.c
  vendor-ante-latina recta) - inventum quodvis = aut vitium regulae
  aut peccatum corporis verum (utrumque pretiosum);
④ vectis pins + suita silvae + officina intactae;
⑤ sumptus impulsus: iudicium scrinium.c (TU cum sqlite3.h 14k
  linearum) intra budget ~0.5s manet (ambulatio originis vilis -
  nexus eam per sweep totum iam solvit).

COST AESTIMATUS: fascis dimidii diei (caerimonia vectis > codex
ipse). Re-entry: hic + examen fons (effusor/relation) +
examen_vectis.sh + fixa/examinis/ conventiones.

## MACRO DOMESTICUM IN CAPITE ALIENO — RELATIO (2026-07-14)

NAVIS sessione post compactionem — captura-tunc-implementa TENUIT
TERTIO (rung 5, commentarium, hoc): reversio consilii NULLA, sed
TRES inventiones implementationis (silva_c89_semantica.worklog):
① alienitas ex GRAPHO INCLUSIONUM (via SCRIPTA directivae
"../vendor/sqlite3.h"), non ex viis fontium — viae fontium basename
SUNT (praebere basename-clavatum ambobus oneratoribus; ordines
percursus inde pendent, ne "sanes"); transitiva transitu uno (margo
parentis ante filiorum); ② SilvaFons.est_lexicon NOVUM — macros ISO
(NULL...) in textu vendicato iure stant; regula litterae INTENTIONIS
eas flagravisset (imago.c OMNI ordine!); fontes lexici + synthetici
numquam domestici; ③ mundi cum_latina (sessio/colloquium/vindex) =
LIMES NOMINATUS (latina in lexico ibi = tacet); canales portantes
(examen/legatus/percursus — cum_latina PINNATUM ZEPHYRUM in legato)
inclusione vera latinam ferunt — vigil ubi importat; via retro =
probatio nominum contra silva_latina_datum.

BARRAE 5/5: ① fixtura ordo_pravus_vendicati.invalidum per vectem
EXSTANTEM (EXSPECTA linea:codex; REICE ex syntaxi; oraculum
consentit) — effusum vivum: "macro domesticum 'C' in capite alieno
expansum (vendor/collidens_vendicatum.h:13) - caput alienum ANTE
latina.h includendum" (omnia nominata; ANTE <basename> DYNAMICE, non
fixum); ② ordo rectus TACET; ③ corpus ZEPHYRUM inventa nova (census
codicum vacuus; scrinium/imago recte ordinant — praedictio tenuit);
re-pin exclusionum 39→40 = deriva baselinae phase A (scrinium.c +
probatio_officina_legatus.c classis lexici POSIX; genera_biblia.c
SANATUM), NON hoc; ④ vectis TENET + silva 30/30 + officina 12/12
(aureae legati octetim tenent); ⑤ scrinium.c 0.23s usoris = baseline
IDEM (via celeris post scansionem marginum exit).

Probatio sectio X (579/579 examen): pravus/rectus/lexicon-fictum/
transitivum/dedup/causa-nominat. Sonda MCP viva: residens vetus
honeste vetera respondit CUM CAUTIONE staleness sui (vigilia in
productione) — post /mcp reconnect bracchium novum vivet. Codex 54
(MACRO_DOMESTICUM_IN_ALIENO, SUSPECTUM); causa in piscina structa
(contractus tabulae laxatus, commenta ambabus sedibus renovata).

DEBRIEF INSTRUMENTORUM: adhibita mcp corpus (nexus_ordines_titulus_
macronis verbatim calidum — exemplar ambulationis originis SINE grep,
primum quaesitum sessionis), mcp diagnostica (sonda vigiliae viva —
CAUTIO staleness sui functa in productione), nexus.sh, examen.sh
(baselines scrinium/fixturae), excubitor -tacitus, vectis ambobus
modis, impulsus legati (arg-counts ×4 capti AD EDITIONEM — signatura
_analysare_alienum crevit, quattuor sedes vocationis statim
nominatae). FRUCTUS: sessio integra sine grep-auditu uno — quaestio
"quid margo inclusionis fert" per lectionem fontis directam,
"ubi ambulator" per corpus. ASPERITAS: exclusiones -corpus DERIVANT
TACITE inter cursus (drift phase A latuit diem — inventum solum quia
barra ③ cursum coegit); nota, non postulatum: cursus 50s = custodia
cara, re-pin ad barras milestone sufficere videtur. DESIDERATUM:
nihil novum.

## QUAESTIO QB — INTENTIO (2026-07-17)

Pull: Fran (quaestio→legati MCP + named/parameterized selectors parked
as 01KXPV9FPK; parameterization NEEDS attributes = QB). Re-entry find:
the QA-closing act ALREADY SHIPPED 2026-07-08 (clausura RELATIO) — the
memory hint was stale; protocol paid. Scope = QB per design DECISUS:
attributa [locus op "valor"], capturae $nomen, fratres +/~.

Decisions for this build:
- IR: SilvaQuaestioPars grows {locus_titulus, attr_op, attr_valor,
  captura_titulus} (chordae COPIED into piscina — query object outlives
  the selector string). Attr ops: EXSISTIT (bare [locus]), AEQUALIS =,
  INCIPIT ^=, DESINIT $=, CONTINET *=.
- Attribute semantics: locus resolved PER-GENUS at match time (hint);
  COMPILE-TIME validation when the compound carries a genus tag — locus
  ignotus = loud fracture (bug #3 dead-attr-paths designed out at the
  strongest point available); tag-less compounds defer to runtime
  no-match. TOKEN locus compares token->valor (mensura-aware);
  LISTA locus matches if ANY token element matches (folium-chorda
  precedent); non-token loci: EXSISTIT = non-NIHIL, comparison ops =
  FALSUM (documented in header). Quoted valor = bytes to closing '"',
  NO escape sequences v1 (NAMED LIMIT; path back = lexer escape arm
  when a fixture needs an embedded quote).
- Capturae: $nomen juxtaposed as pars (space = combinator, so binding
  is tag$nomen / ]$nomen adjacency); matching-neutral; ALL HOPS bound
  (v1 bug #1): _catenae_congruit gains a vestigia out-array (slot per
  gradus, written on the success path AFTER upstream succeeds — retry
  overwrites die with their failed paths); congruit() signature stays
  boolean (vestigia NIHIL); exsequi binds. Result grows capturae Xar
  (SilvaQuaestioCaptura {titulus, nodus}; NIHIL = none — allocated
  only when the matched catena carries captures). Union: FIRST
  matching catena binds. gradus_maximi precomputed at compile;
  vestigia allocated once per exsequi.
- Fratres: siblings = elements of the node's CONTAINING LISTA in
  pater's loci (walk loci, scan listae for the node); interleaved
  token elements (congeries commas) skipped; node held in a direct
  NODUS locus has no siblings (+/~ fail honestly). + = nearest
  preceding NODUS element; ~ = any preceding.
- Public surface: SilvaQuaestioCaptura + capturae field mirrored BY
  HAND in silva.h (transparent structs live in both — the amalgam-ABI
  law: fontes and silva.h edited same-change); CADENDA_TYPEDEF +=
  SilvaQuaestioCaptura; hospes QB calls with content asserts;
  amalgamare gates.

Bar: probatio extensions — five ops incl. prefix/suffix/substring,
bare existence, compile fracture on locus-ignotus-with-tag, runtime
no-match on tag-less unknown locus, lista-token match, captures
all-hops + union-first-wins, sibling counts +/~ with interleaved
separators, congruit boolean unchanged. Suite green, hospes green,
amalgam VERIFICATUM, fidelitas untouched (no lexer/grammar edits).
QC (pseudos) and QD (translated v1 bar) stay next-by-name.

### QUAESTIO QB — RELATIO (2026-07-17): attributa/capturae/fratres SHIPPED

**Suite 30/30 (probatio_quaestio 38→86 asserta, tier temporis cursus
primo cursu viridis); hospes 32/32; amalgama VERIFICATUM.** Superficies
QB tota: attributa [locus op "valor"] (quinque operationes = ^= $= *=
+ exsistentia nuda; resolutio PER GENUS ad congruentiam; validatio
compilationis cum genus-tag praesente — "locus generi ignotus" fractura
CLARA, compositum sine tag ad tempus cursus differt; TOKEN valorem
comparat mensura-conscius, LISTA elemento ULLO congruit
[folium-chorda], non-lexema exsistentia sola); capturae $nomen
(iuxtapositae; gradus OMNES catenae ligati per vestigia — quaternio
scriptus in VIA VICTRICE SOLA post successum superiorem, retentationes
mortuae vestigia secum ferunt; insectum v1 #1 exstirpatum probatum;
unio = catena PRIMA ligat; sine capturis = NIHIL sine allocatione);
fratres +/~ (lista continens in patre, lexemata interposita
transilita — virgulae congeriei pinnatae [folium + folium = II inter
commata], nodus in loco NODO directo sine fratribus honeste).

INVENTA: (1) murus i32-insignatus TERTIO in auctorem suum — _locum_
invenire -I reddens + sedes/i decrementum sub zephyrum; functiones
index-aut-minus-unum s32 reddant SEMPER (classis eadem ac firing QA).
(2) hospes: insertio in "if (x) { }" sine else legendo — else orbum
syntaxin fregit; et sanatio prima PANICA (hack etiquettae) statim
reversa; lex: syntaxin numquam sarci, restructura. (3) arbor.sh formas
congeriei/folii-chordae ANTE asserta pinnavit — pinnae sine
coniectura, cursus primus viridis.

LIMITES NOMINATI: sequentiae fugae in valore citato ABSUNT (via
retro: bracchium fugarum in lectore cum fixtura citationem internam
petat; CONTINET interea sufficit — demonstratum in probatio
folii-chordae). Validatio loci trans-genus (compositum-tag +
attributum generis alieni) impossibilis per constructionem — locus
per genus SUI nodi semper.

RE-ENTRY FIND: actus claudens QA iam 2026-07-08 navigaverat (RELATIO
clausurae) — monitum memoriae stalum erat; protocollum "numquam ex
summario" iterum solvit.

PROXIMA BY NAME: QC pseudo-classes (:habet/:non/:lectiones/:vocat...;
monita machinae in memoria topica) → QD barra v1 translata (100
asserta, tags Anglici → DECISUS). Parcum 01KXPV9FPK (quaestio in
legati MCP + selectores nominati/parametrizati — parametrizatio EST
attributa, dependentia nunc soluta) trahitur post QD.

### Addendum prae-compactione (2026-07-17) — semina interrogationis QC

QC INTENTIO nondum scripta; rotula interrogationis brevis Frani
debetur ANTE aedificationem. Quaestiones tres in colloquio nominatae:
(1) familia normae (:c89/:c99/:extensio) - trahitne populationem
tags-normae NUNC (recensio #5, "pseudos = the pull that funds it")
an differt nominatim? (2) semantica :vocat(f) - "x CONTINET
vocationem f" (saccharum super :habet) an "x EST vocatio f"? Barra
v1 responsum fert - QD messis nos tenebit; inspicere v1 ante
decisionem. (3) registratio pseudo-classium usoris (insectum v1 #4
"made functional") - in QC an QD? Monita machinae QC in memoria
topica silvae valida manent (:lectiones = vexillum ambulationis;
:habet/:non = compilare recursivum; semantici = praedicata super
vocatio[tok_valor]). Re-entry: hoc addendum + RELATIO QB supra +
monita memoriae.

INSTRUMENTA DEBRIEF (arcus quaestio QB, more domus):
- adhibita: diagnostica legati I (ACCIPE ante compilationem);
  impulsus post-editionem (cascatae transitionales rectae; fractura
  syntaxis hospitis statim nominata - sed positione "99:1" pro
  fractura vera ~949, observatio non investigata); arbor.sh II
  (formae congeriei/folii-chordae ANTE asserta - pinnae sine
  coniectura); grep/Read pro regionibus (spec/monita lineas ferunt -
  navigatio viva minor, exemplar K4 iteratum); vigilia bis recte.
- fructus: arbor.sh = stella arcus (cursus primus viridis tier
  temporis cursus EIUS meritum); impulsus fracturam hospitis ante
  lectionem manualem nominavit.
- asperitates: (1) MAGNA - verdictum ACCIPE conversiones signi NON
  tegit: errores tres -Wsign-conversion post ACCIPE compilator
  solus cepit; classis decipulae MAXIME urens domus extra iudicium
  examinis vivit (desideratum filatum - semantica typos naturales/
  conversos IAM fert ex M0b, sedes implementationis exstat).
  (2) parva - arbor.sh fontem INLINE accipit, non viam plagulae;
  cyclum perdidi viam tradens (lexavit viam ipsam!). Vexillum
  -plagula aut detectio plagulae exsistentis = commoditas vilis.
- desiderata: gradus conversionum in examine (supra); cetera iam
  parcata (harnessum LSP interpositum, mtimes nanosecundorum).

## EXAMEN — conversio signi (gradus DOMESTICUM): INTENTIO (2026-07-16)

Origin: desideratum 01KXPXBQG7 (QB debrief — three -Wsign-conversion
compiler errors AFTER examen said ACCIPE; the i32-unsigned wall is the
monorepo's most prolific trap, ~15 historical firings, and lives
entirely outside the judgment). Goal: the trap fires IN THE PUSH at
edit time, before the compiler ever runs.

Protocol held: typing-table region of silva_c89_semantica.c read
before this document (typationes, _conversionem_annotare, UAC
helpers, diagnosticum materialization, effusor verdict logic).

### The seat — refinement of the desideratum's sketch

The desideratum sketched a post-pass over the typationes table. Code
contact found something strictly better: `_conversionem_annotare`
(silva_c89_semantica.c:4112) is a 17-line funnel through which EVERY
recorded conversion flows — ~35 call sites covering UAC (both
operands), shifts, ternary arms, the whole _ad_finem family
(assignment / init / redde / prototyped args / congeries elements),
and explicit casts. It receives exactly the judgment's trio {nodus,
naturalis, finis}, runs while parsura_currens is set (positio
materializes for free), and appends diagnostics in walk order —
deterministic, where a hash-table post-pass would not be. The "no
typing surgery" property is preserved: one additive call in the
funnel's types-differ branch.

### The judgment (new interior fn called from the funnel)

Fire EXAMEN_CODEX_CONVERSIO_SIGNI (severitas DOMESTICUM — legal C89;
house flags make it -Werror) iff:
1. both sides integer-tier after quals stripped (primitivum integer
   family; enumeratus as int per _promotum precedent; pointers,
   floats, aggregates out);
2. signedness differs (_est_insignatum_primitivum — NB plain char is
   correctly signed-tier on this platform, absent from that list);
3. range NOT preserved: signed→unsigned always fires (negatives
   lost); unsigned→signed fires only when dest size <= src size
   (u8→s32 is value-preserving → silent; subsumes "promotions never
   fire").

### Suppressions (each a named clause, in order)

- CAST EXPLICITA: nodus sits under a conversio node (pater check —
  post-commit trees carry pater; avoids threading a new param
  through 35 call sites). The writer said the conversion.
- CONTEXTUS COMPARATIONIS: pater = binarium with comparison operator
  → silent in v1. That is -Wsign-compare's domain = named phase 2.
- CONSTANS CAPIT: silva_c89_constans_aestimare succeeds AND the
  value is representable in finis → silent. Covers `i32 n = 0`,
  enum constants, `i8 m = 0xFF`. `i32 x = -I` still fires — THE
  house trap, by design.
- PER-INTERVALLUM (x & 0xff → i8, %, >>): NOT BUILT until corpus
  contact demands a specific arm — the named tail where the day
  could stretch.

### Codex + message

ONE codex for all contexts (codices are stable ABI; position shows
context). Message structured in piscina (MACRO_DOMESTICUM_IN_ALIENO
precedent) naming both types via silva_c89_typum_scribere:
"conversio signi implicita: <naturalis> -> <finis>". Needs a small
addere variant taking a built causa while reusing the standard
positio materialization + provisionale mark.

### Bars

1. Unit (probatio_silva_examen.c section): trap forms fire (redde -I
   from i32 fn; -I as prototyped i32 arg; s32→i32 assignment;
   i32→s32 same-size), suppression forms silent (cast / comparison /
   constant-fit / promotions), counts pinned.
2. CORPUS-ZERO (the free negative oracle): full percursus sweep →
   census shows zero CONVERSIO_SIGNI. The monorepo compiles under
   -Wsign-conversion -Werror, so ANY corpus firing = false positive
   BY CONSTRUCTION. Verdict column byte-identical (confirmed in the
   effusor: only non-provisional VIOLATIO flips REICE).
3. Fixture differential: NEW fixture class fixa/examinis/*.domesticum
   (suffix-per-class law): verdict stays ACCIPE + EXSPECTA pins
   present as DOMESTICUM rows + clang oracle agreement
   (-Wsign-conversion fires at the same lines). examen_vectis.sh
   gains a section. Fixtures SYNTHETIC from the named historical
   patterns (censoris precedent — all firings were fixed in-session,
   no landmine blobs exist in git).
4. Secondary (recorded, no threshold): percursus wall-time
   before/after — the aestimator now runs on constant-fit candidates.
5. Standing gates: suites, hospes, amalgamare.sh (if ExamenCodex is
   mirrored in silva.h the enum row is added by hand — twin law).

### Consumers (free, via shared judgment)

examen.sh rows + domestica count; LEGATUS DIAGNOSTICS PUSH — the
actual goal, the i32 wall caught at edit time; percursus census.
NOT v1: -domus verdict flag (visibility is the value; verdict
flipping waits for field experience).

### Named phase 2 (not now, by name)

Comparisons (-Wsign-compare incl. tautological insignatus >= 0);
narrowing (-Wconversion value truncation); float<->int. Same funnel,
same suppression machinery — each is one predicate arm + fixtures
when pulled.

### In-flight risks

- Double-annotation: some sites may annotate one node twice (UAC then
  context) — if the probatio shows doubled rows, add a once-per-node
  guard (judged on evidence, not assumed).
- Ambigui: provisionale comes free via the standard addere path
  (_intra_ambiguum) — the structured-causa variant must keep it.

### Open before code (Fran)

Q1 verdict-neutrality of DOMESTICUM in v1; Q2 structured vs static
causa; Q3 one codex vs per-context split. Recommendations inline
above.

DECISUS (Fran, 2026-07-16, all three recommendations): verdict-neutral
DOMESTICUM in v1 (-domus = named future); structured causa naming both
types; ONE codex CONVERSIO_SIGNI.

## EXAMEN — conversio signi: RELATIO (2026-07-16, same day as INTENTIO)

SHIPPED. The murus i32-insignatus is inside the judgment: firings
land in the legatus push at edit time, before the compiler.

Bars:
- Unit 641/641 (probatio_silva_examen +55 asserts): every named trap
  fires exactly once (redde -I / -I argument / assignment / UAC /
  init / char->u8), every suppression silent (cast / comparison /
  constant-fit / promotions / bigger-signed-dest / enum / shift
  count), severity DOMESTICUM + structured causa + position pinned,
  no probe noise (total-count pin).
- CORPUS-ZERO: definitive percursus -semantica sweep — census codex
  54 ABSENT over 820 files / 1,690,957 typed expression nodes;
  verdict column unchanged (one pre-existing drift row pinned:
  lib/vigilia.c = the excubitor arc's <utime.h>-outside-lexicon
  class, REICE since its birth, exclusiones.txt 40->41).
- Differential fixture conversio_signi.domesticum: clang agreement
  LINE AND COLUMN (13:12/23:13/30:13/36:12) + count parity, wired as
  vectis section ②b (verdict must stay ACCIPE + pins present as
  domesticum + oracle warns same lines + pin count == warning
  count). VECTIS TENET.
- Suites: silva 30/30 (ONE deliberate revision: the M0b UAC fixture
  contains a true conversion "ul + ll" — zero-diagnostics pin became
  one-DOMESTICUM-row pin, clang concurs); officina 12/12 (sessio
  golden transcripts intact); amalgamare VERIFICATUM, hospes 32/32.
- Timing (secondary bar, recorded): ante 155,020 ms -> post
  155,847 ms (+0.5%, noise tier) — range engine + alienity table
  invisible at corpus scale. Coverage 100.00% held.

As built (= INTENTIO except the tail, which fired exactly as named):
funnel seat _conversionem_annotare; ONE codex CONVERSIO_SIGNI
(DOMESTICUM, verdict-neutral); structured causa via
_diagnosticum_addere_plenum (addere body refactored, wrapper kept);
suppressions = cast-pater, comparison-pater (phasis II), COMPOUND-
ASSIGN LHS (calibration find), enum EXCLUDED (calibration find),
ALIEN SOURCES (_alienitates_computare factored out of
MACRO_DOMESTICUM, cached per-parsura on sem, queried by radix
fons_index — fons viae are basenames, the inclusion graph's written
paths decide), RANGE ENGINE _intervallum_expressionis {latitudo,
non_negativum} in clang's IntRange shape (constants exact via silent
probe; ternary = arm union; comparisons/logic = {1,nonneg}; bit +
arithmetic ops by widths; fallback = TYPE range from the M0b
typationes table — the table's first downstream consumer beyond
demissio).

First corpus contact: 217 rows, ALL false positives by construction
(145 vendored stb + ~53 IntRange classes + amalgam/fixture copies).
Second contact: ZERO. The INTENTIO's "per-intervallum pulled by
corpus contact only" clause was the day's best decision — the tail
was real, bounded, and arrived with classification data attached.

THE CALIBRATION LAW (three data points this arc): an oracle-paired
tier's predicate must be pinned against the LIVE oracle with
specimens BEFORE authoring — clang's suppressions are heuristic,
not mathematical (sub keeps nonneg when both operands nonneg even
though 5-10<0; compound-assign LHS silent while RHS fires;
non-negative enums typed unsigned). Two of three a-priori guesses
were wrong; specimens were minutes each.

PHASIS II BY NAME: comparisons tier (-Wsign-compare incl.
tautological insignatus >= 0, own codex); narrowing tier
(-Wconversion value truncation); float<->int; -domus verdict flag;
compound-assign-LHS as OPTIONAL stricter-than-clang house rule.

INSTRUMENTA DEBRIEF (more domus):
- adhibita: examen push (mid-refactor exp-undeclared cascade named
  instantly; the s32/i32 wall fired on the tier's own guard AND its
  own probatio — poetic, caught pre-push by compile); corpus-as-
  oracle (the arc's engine); clang specimen calibration x5 (the
  arc's second engine); percursus census column (bar instrument);
  worklogs at find-time.
- fructus: typationes table answered the range engine's fallback
  tier for free; _via_aliena + inclusion graph answered alienage;
  the diff-vs-oracle loop (examen -machina | awk vs clang) is a
  two-liner worth remembering.
- asperitates: (1) STALE-BINARY strike — silva/build/examen is
  relinked only by examen.sh; ran it directly after a fontes edit
  and chased a phantom "guard doesn't work" (bench symptom row
  exists, forgot to reach). (2) LANE CONFUSION — plain percursus
  is parse-only; the first "corpus-zero" was vacuous (judgment
  lane = -semantica). One sweep pair wasted. (3) MID-SWEEP tree
  mutation contaminated the ante baseline (sweep reads disk as it
  walks) — never touch the tree while a sweep runs.
- desiderata: none new (phasis II items named above; oracle-
  specimen calibration could become a small fixture convention if
  a third oracle-paired tier appears).

## Sequela debrief: differentia.sh + vigilia examinis (2026-07-17)

Two instruments from the conversio-signi debrief, built same-day on
Fran's pull:

DIFFERENTIA (./silva/differentia.sh <plagula> [-posix]): our
judgment vs the clang oracle on one file, three buckets per
oracle-paired tier — CONSENSUS / NOS SOLI (false-positive suspects)
/ ORACULUM SOLUM (misses or named limits). Tier table in the script
(codex 54 <-> -Wsign-conversion; phasis-II comparisons tier adds a
row). Judgment side goes through examen.sh (ALWAYS the wrapper —
the stale-binary trap is why this script exists); oracle stderr
captured correctly ONCE (2>&1 — the all-zeros-table lesson).
Verified: fixture 4/4 CONSENSUS; flatura consensus plenus; enum
named-limit specimen -> ORACULUM SOLUM row + exit 1. Eyes-tool,
not a gate (gates = vectis).

VIGILIA EXAMINIS: lib/vigilia (generalized 2026-07-16, legatus/
tabularium seed) wired into the examen binary — examen.sh writes
silva/build/examen.manifestum (aedificator-invariant: the TRUE
list it compiles: 8 lib fontes + silva/fontes *.c+*.h + examen.c +
include/*.h), binary checks fontes-superant at startup, one CAUTIO
line to STDERR naming the offender ("per ./silva/examen.sh
recurre" — the lib's resident message says /mcp reconnect, wrong
for a CLI, composed our own from vigilia_causa). -machina stdout
BYTE-IDEM under warning (diff-verified). Live bar 3/3: fresh
quiet / touched fontes warns with exact path / rebuild heals.
Link find: vigilia.o needs sigillum.o (RADIX_FONTES += vigilia,
sigillum). Fast direct-binary loops are now safe BY DETECTION.
Ceteri binaria subsellii (censor...) get the same ~20 lines on
next touch — pattern proven here.

## TOLERA — gradus severi + suppressiones commentariis: INTENTIO (2026-07-17)

Origin: parcum 01KXQ5ZKV0 (Fran's pull, same conversation as the
measurement). Premise measured before design: the sound-subtraction
rule = 17 corpus sites (one-sitting adjudication; color.c lerp may
be a REAL find). Goal: strict static analysis ABOVE oracle parity,
with in-code suppression as a first-class recorded act.

ARCHITECTURE (blessed in conversation):
- Two tiers, hard boundary: parity codex 54 stays comment-free
  forever (corpus-zero stays free); STRICT rules get own codices,
  only they are TOLERA-suppressible. A site fires 54 XOR severa —
  TOLERA can never silence what clang would reject.
- v1 strict rule: EXAMEN_CODEX_CONVERSIO_SIGNI_SEVERA (DOMESTICUM,
  verdict-neutral) = fires where the SOUND range analysis cannot
  prove safety but the clang-parity heuristic stays silent. Engine:
  ExamenIntervallum gains a second bit non_negativum_severum,
  computed in the SAME pass (sub kills it; other ops propagate);
  fire severa when heuristic-nonneg && !severum-nonneg.
- Grammar (EXSPECTA precedent): /* TOLERA <CODEX_TITULUS>: causa */
  — same line as the firing or the line above; CAUSA MANDATORY
  (house culture: record the why). Codex named explicitly, no
  wildcards.
- TOLERA table: lazy per-parsura scan of comment trivia (comments
  are content — the VISIO pin pays again), cached on sem like the
  alienity table: {linea, codex, habet_causam, usus}.
- EXAMEN_CODEX_TOLERA_IRRITUM (DOMESTICUM): fired at end of
  analysis for any TOLERA that absorbed nothing OR lacks a causa
  (reasonless = does not suppress). The eslint staleness lesson +
  the INVARIANT shape: a suppression matching nothing is a signal.
- ONE-override-philosophy note stands: TOLERA is the site-scoped
  member of the exclusiones/filter/aedilis family — vocabulary
  designed to coexist, formal unification stays at the aedilis
  on-ramp.

BARS:
1. Unit (probatio_silva_examen): severa fires on u8-u8-into-unsigned
   with parity 54 SILENT there (the XOR pin); TOLERA same-line and
   line-above suppress; TOLERA without causa does NOT suppress +
   IRRITUM; unused TOLERA -> IRRITUM; TOLERA naming codex 54 has NO
   effect (parity not suppressible) + IRRITUM (absorbed nothing).
2. Corpus adjudication = the deliverable: all 17 severa sites
   classified by hand (fix real bugs / TOLERA with reasons);
   standing bar becomes census severa = 0 UNSUPPRESSED + IRRITUM
   = 0.
3. Fixture + vectis leg: severa fixture asserts clang SILENT at
   pinned lines (the inverse oracle leg — strict means beyond
   clang), our rows present unsuppressed, absent under TOLERA.
4. Standing gates: suites, amalgam (silva.h mirror +2 codices),
   timing recorded.

RISKS: trivia access pattern for comments (read silva_token.h
first — attachment rules exist but the lookup shape is unverified);
adjudication may find real bugs (color.c) whose FIXES are the
right outcome, not TOLERA.

## TOLERA — gradus severi: RELATIO (2026-07-17, same day as INTENTIO)

SHIPPED, and the arc PAID ON ARRIVAL: adjudicating the 17 measured
sites found TWO REAL SHIPPED BUGS.

THE FINDS:
- lib/delineare.c DEAD CLAMP: cohibere(i32) — unsigned param made
  the "< 0" clamp tautologically dead since birth; negative sums
  (dark pixel + negative diffusion error / Bayer threshold in
  [-128,112]) wrapped to ~4e9 and clamped to 255: WHITE SPECKLES IN
  SHADOWS across every error-diffusion and Bayer path. The (i32)
  casts at call sites were the -Werror-silencing pattern that
  buried it. Fixed: cohibere(s32) + 9 call sites genuinely signed +
  threshold s32. All firings DISSOLVED (fixed, not tolerated).
- lib/color.c ROUNDING DRIFT: interpolate's unsigned t sent
  negative diffs (darkening lerps) through unsigned division —
  floor-vs-truncate off-by-one on non-exact steps. Fixed to
  explicit s32 arithmetic; probatio_color 63+123 green; the exact
  roundtrip-fixture harvest resynced.

ADJUDICATION LEDGER (17/17): 8 fixed (delineare 4, color 4) + 4
dissolved by fixture resync + 5 TOLERA with recorded reasons
(arbor_syntaxis 3 "custoditum - *p intra [...]", flatura 2 "larva
(1<<n)-1 non negativa" — frozen v1 code, guards/invariants the
width engine cannot see).

BARS:
- Unit 691/691 (+36): severa XOR parity pinned (u8-u8 fires severa
  ONLY), same-line + line-above suppression, sine-causa does not
  suppress + IRRITUM, unused IRRITUM, unknown codex IRRITUM with
  parity intact, plain comments inert.
- CORPUS FINAL: census 54 ABSENT + 55 ABSENT + 56 ABSENT over 820
  files / 1,692,664 typed nodes — zero unsuppressed strict
  firings, zero rotten suppressions, parity intact. Verdicts = 41
  pinned exclusions unchanged; coverage 100.00%; tempus 150.8s
  (baseline).
- Fixture gradus_severi.severum + vectis leg ②c (INVERSE oracle:
  clang must be SILENT at pins — being above the oracle is the
  pinned property; tolerata line quiet; IRRITUM 0). VECTIS TENET
  four legs.
- amalgamare VERIFICATUM (hospes 32/32); silva 30/30; officina
  12/12.

AS BUILT: ExamenIntervallum carries non_negativum_severum computed
in the same pass (identical per-op formulas; MINUS = numquam;
shift optimism SHARED v1 = named limit); severa inherits every
parity suppression by position; TOLERA table = lazy per-parsura
trivia scan (comments-are-content pin pays again), suppression =
codex+causa+fons+linea(0/-1) match, absorption marks usage;
IRRITUM fires post-walk for unused/causa-less/unknown-codex.
Codices 55+56 appended (stable ABI), silva.h mirrored.

CORPUS LAW, FOURTH STRIKE: the machinery's own doc comments
("TOLERA irrita...") parsed as directives — grammar now requires a
non-empty UPPERCASE codex word (prose continues lowercase). Pinned.

PHASIS II unchanged by name: comparisons tier (brings tautological
insignatus>=0 — delineare's dead clamp would ALSO have been caught
there), narrowing, float, sound-shift arm, -domus.

INSTRUMENTA DEBRIEF (more domus):
- adhibita: the PUSH DROVE THE FIX — changing cohibere's signature
  lit all 9 call sites positioned in the edit feedback (judgment as
  pair programmer, the strongest push moment yet); differentia.sh
  dogfooded on day one (NOS SOLI bucket = the adjudication
  worklist); percursus census = the arc's meter (17 -> 0);
  measurement-before-design (one flipped predicate + sweep) sized
  the whole proposal before commitment.
- fructus: trivia-as-content answered TOLERA lookup in ~40 lines;
  the alienity-table caching shape reused verbatim.
- asperitates: none new — the self-referential corpus law is now
  four-for-four on new judgment machinery (budget for it in every
  future tier: your own docs/fixtures WILL trip the new rule).
- desiderata: none filed (phasis II items named above).

## COMPARATIONES — phasis II conversionis signi: INTENTIO (2026-07-17)

Origin: menu analyseos staticae (parcum 01KXQ82Z9Z), Fran's pull #1;
named as phase II since the conversio-signi INTENTIO. The recipe is
now standard: seat -> CALIBRATE FIRST -> corpus census ->
adjudicate -> fixtures drop into existing vectis legs.

TWO LEGS, one seat (the comparison branch of binarium typing — the
very place the conversion tier's pater-suppression already stands):
- PARITY: EXAMEN_CODEX_COMPARATIO_SIGNORUM (DOMESTICUM,
  comment-free) = clang -Wsign-compare (house -Wextra includes it;
  corpus-zero free). Signed/unsigned comparison after UAC where the
  oracle warns. Suppression shape UNKNOWN until calibrated —
  specimens BEFORE predicate (the calibration law, now a memory
  entry: two of three a-priori guesses were wrong last time).
- SEVERA: EXAMEN_CODEX_COMPARATIO_VANA (DOMESTICUM,
  TOLERA-suppressible) = tautological zero-comparison: expression
  with SOUND non-negative range (severum bit — zero false positives
  by construction) compared < 0 (semper falsum) or >= 0 (semper
  verum), both operand orders. THE KEY INSIGHT: clang suppresses
  tautological warnings inside macro expansions and every house
  zero is ZEPHYRUM — the C toolchain is structurally BLIND to this
  class here (that is HOW delineare's dead clamp lived). We see the
  expanded tree with provenance; the inverse-oracle property is not
  incidental, it is the tier's reason to exist. The historical
  unsigned-loop-decrement firing (i >= 0 with i unsigned) is this
  class.

MACHINERY REUSED WHOLE: range engine (severum bit for VANA, 
heuristic bit for any parity range-suppression calibration finds),
TOLERA table (+1 row in _tolerabiles), alienity guard, structured
causa, vectis legs ②b/②c iterate ALL fixtures of each class — new
fixtures just drop in; differentia gains one GRADUS row
(-Wsign-compare).

BARS: unit section (both legs, both operand orders, TOLERA, XOR
with parity where applicable); corpus parity-zero + VANA census =
THE HUNT (expect more dead ZEPHYRUM clamps — adjudicate each as
bug-fix or TOLERA); fixture .domesticum (oracle agrees) + .severum
(oracle SILENT through the ZEPHYRUM blindness — pinned); suites;
amalgam; timing recorded.

RISK: clang -Wsign-compare folklore (constant handling, promoted
small types, macro contexts) — entirely calibration-first.

## COMPARATIONES — phasis II: RELATIO (2026-07-17, same day)

SHIPPED — and the VANA census surfaced the largest latent-bug
harvest of the whole lint streak.

TWO CODICES LIVE:
- COMPARATIO_SIGNORUM (parity -Wsign-compare): **CORPUS-ZERO ON
  FIRST CONTACT** — the calibration-first discipline (two specimen
  batteries BEFORE the predicate) produced zero false positives on
  its first sweep. Calibrated shape: fires when UAC commune is
  unsigned and the promoted-signed side converts (==/!= included;
  long-vs-u32 silent); suppressed by non-negative constants
  (negative constants FIRE — deliberate divergence from clang's
  inconsistent '-1 < u' silence, corpus has neither) and
  heuristic-nonneg ranges (promoted u8, masks).
- COMPARATIO_VANA (severa, TOLERA-suppressible): tautological
  zero-compares — commune-type-unsigned ((u-v) >= 0 = the broken
  underflow check!) OR sound-nonneg range (promoted u8). Sees
  through macros BY DESIGN (aestimator) — clang suppresses
  tautologicals in macro expansions and -Wtautological-* is not in
  house flags at all: this class was DOUBLY invisible (that is how
  delineare's dead clamp lived).

DECISUS mid-arc (Fran): IDIOM CUSTODIAE BILATERALIS — "X < 0 || X
>= LIMES" on plain-identifier X is the house's standard defensive
guard (~94 corpus sites), functionally safe (wrap caught by the
upper bound); the rule recognizes it structurally (pater ||/&&
chain, sibling orders the SAME identifier; composite X like u-v
never suppressed) and stays silent. 168 -> 74 firings.

THE HARVEST (census 58 = 74, STANDING as an open ledger —
quaestio 01KXQ9NF8J): spot-verification confirmed the classes are
REAL — lib/tcp_posix.c:280 'i32 n = tcp_mittere(); si (n < 0)' =
the TCP write loop's error path NEVER WORKED (-1 flows as 4e9 into
totalis); dead -1 sentinels (quaerere, sententia_fissio x5);
reverse-scan loops 'k >= ZEPHYRUM; k--' = the historical
infinite-loop trap alive in vindex/biblia_visus/selectio/stml;
delineare has 14 MORE standalone dead clamps beyond cohibere.
Adjudication = type surgery with conversion ripples — deliberately
deferred to a FRESH session (74 sites at session-end quality is
how mistakes ship). The census row is the standing meter.

BARS: unit 780/780 first-run (5 SIGNORUM fire forms, 5
suppressions, 4 VANA forms + broken-underflow + promoted-u8,
idiom pins both polarities + multi-clause + mismatched-identifier
NEGATIVE + composite-X NEGATIVE, TOLERA, VANA/SIGNORUM
exclusivity); SIGNORUM corpus-zero; fixtures
comparatio_signorum.domesticum (oracle agrees line-for-line) +
comparatio_vana.severum (oracle SILENT incl. through-macro form —
the ZEPHYRUM blindness pinned); vectis legs GENERALIZED (per-
fixture "ORACULUM -W..." directive; ②c counts pinned codices
generically) — VECTIS TENET; differentia gained the sign-compare
row; amalgamare VERIFICATUM; silva 30/30; officina 12/12.

Seat note: the judgment rides the comparison branch where the
conversion tier's pater-suppression already stood — phasis II
landed exactly where phase I said it would ("-> COMPARATIO_
SIGNORUM/VANA ad nodum comparationis").

INSTRUMENTA DEBRIEF: calibration-first = corpus-zero first contact
(the law's first full-cycle validation); the census-as-meter
pattern (a standing nonzero row = visible work ledger, verdict-
neutral) is new and deliberate; _constans_probare extracted at
third use (rule of three); no new asperitates.

### Addendum prae-compactione (2026-07-17) — re-entry for the next arcs

Session state at compaction: lint streak = FOUR tiers shipped in two
days (conversio-signi parity, TOLERA/severa, comparatio signorum,
comparatio vana) + instruments (differentia.sh, vigilia examinis) +
four real bugs (two fixed, one confirmed-live, one class standing).
All suites green, amalgam VERIFICATUM, everything filed.

NEXT PULL (Fran's stated order): **VANA ADJUDICATION** — quaestio
01KXQ9NF8J carries classes/priorities/recipe. Re-entry protocol:
(1) read the quaestio, (2) regenerate the site list (percursus
-semantica | codex 58; attribution loop in the quaestio), (3) per
site READ THE DECLARED TYPE before surgery — a firing proves
unsignedness at the comparison, the fix (s32-ification vs
restructure vs TOLERA) needs the declaration and callers, (4)
tcp_posix.c:280 FIRST (confirmed live), then sentinels, then
reverse-scan loops, then delineare clamps, (5) fixes ripple into
-Wsign-conversion — compile per batch, sweep at end (54=55=58
deltas all matter). Batch commits per class recommended.

AFTER THAT (menu park 01KXQ82Z9Z): chorda.datum->str* (house rule,
inverse oracle) -> narrowing -> commutatio fall-through -> unused.
QC quaestio pseudos still queued behind the lint streak (interview
seeds in the 2026-07-16 addendum above).

Standing counters at compaction: census 58 = 74 (the meter);
census 54 = 55 = 56 = 57 = 0. Verdicts = 41 pinned exclusions.

## VANA ADIUDICATIO: INTENTIO (2026-07-17, sessio recens)

The fresh session the addendum prescribed. Quaestio 01KXQ9NF8J is the
ledger; census regenerated at re-entry = **74, byte-stable** against
the compaction counter. Task ledger mirrors the four classes.

TWO SCOPE DISCOVERIES at re-entry (both worse than the ledger knew):
1. **lib/http.c:750 receive loop shares the trap** — `i32 n =
   tcp_recipere(...)` then `si (n > 0) memcpy(..., (size_t)n)`: a TCP
   error mid-response arrives as n=4e9, is treated as received data,
   and memcpys ~4GB out of a 4KB buffer. Not just a dead error path —
   a live crash/corruption path. (Same loop serves tls_recipere.)
2. **lib/tls_macos.m is an invisible twin** — identical API disease
   (tls_mittere returns (i32)-1; tls_mittere_omnia:415 has the same
   dead `si (n < 0)`), but .m files are outside examen's walk, so the
   census CANNOT see it. The 74 is a floor, not the class. Grep is the
   only meter for .m; noted as a standing limitation.

PLAN (class 1 = API surgery, the root cause): the tcp/tls I/O API was
DESIGNED signed (headers document "-1 si error") but TYPED unsigned.
Fix the type to match the design: tcp_mittere / tcp_recipere /
tcp_obtinere_fd / tls_mittere / tls_recipere → **s32**. Ripples:
tcp_posix.c + tls_macos.m bodies (casts, sentinels un-cast),
tcp_mittere_omnia/tls_mittere_omnia loops (guard becomes LIVE;
positive-n casts back into i32 accumulators), http.c receive loop
(n → s32), probationes ×2 (locals + CREDO _I32→_S32). Header doc
examples (tcp.h:10, tls.h:10) fixed too — they propagate the trap.
Then classes 2-4 per the quaestio (sentinels → reverse loops →
delineare clamps), READ THE DECLARED TYPE before every cut, compile
per batch, commit per class, full sweep at end (54=55=58 deltas).

BARS: census 58 → 0-or-TOLERA; census 54/55 unchanged (no new
conversions smuggled in); suites green incl. tcp/http (tcp_servus
known-flakey noted); grep-meter for tls_macos.m; RELATIO + quaestio
closed with decree.

## VANA ADIUDICATIO: RELATIO (2026-07-17)

FOUR CLASSES SHIPPED, four commits (dc2bb5a, 1a5968e, 125a4e8,
834113d). Census 58: **74 -> 46**; census 54/55/57 = 0 (no
conversions smuggled in by the surgery); REICE = 41 unchanged.

THE BUG HARVEST (every one live, none theoretical):
1. tcp/tls sentinel API (typed unsigned, designed signed) — the TCP
   write error path never worked; http.c's receive loop turned a
   transport error into a ~4GB memcpy from a 4KB buffer (crash
   vector, not just corruption).
2. tls_macos.m — the INVISIBLE TWIN (.m outside examen's walk) +
   a third bug found by reading: the read callback's partial-read
   check compared n against itself (ordering), so errSSLWouldBlock
   was never returned. Grep is the only meter for .m files.
3. quaerere backward pass: wrapped index = live OUT-OF-BOUNDS reads.
4. sententia_fissio: sentinel DELETED at the root (-1 branches were
   unreachable AND every caller clamped -1 to mensura — the function
   now just returns mensura).
5. selectio._est_ultimus_liberum: empty child list -> wrap ->
   xar_obtinere NIHIL -> NULL DEREF.
6. vindex stack walk terminated only because inspicere happened to
   fail on the wrapped index — by luck, not design.
7. delineare: wrapped radius = ~4e9-iteration HANG; midpoint circle
   error term unsigned -> `<= 0` degenerated to `== 0` = circles
   subtly misshapen since birth (outside the census — found because
   the surgery required reading the algorithm); gray-palette
   distance unsigned -> brighter entries could never win.

SCOPE NARROWED, PATH BACK NAMED: the unnamed tail = 42 source sites
across 22 files (+4 amalgam shadows that regenerate) — includes
FROZEN reference code (arbor2_glr, arbor_praeparator, lapifex) where
fixing vs TOLERA is a policy question for Fran, plus core libs
(chorda x2, chorda_aedificator x2, vim x2, sputnik x8, schirmata x4,
arx_caeli x4, officina fontes x4...). Deliberately NOT batch-fixed
at session end (the ledger's own law). The full list lives in
quaestio 01KXQ9NF8J; census row 58 remains the standing meter at 46.

INSTRUMENTA DEBRIEF: adhibita — examen -machina per-file attribution
(hand-rolled, see asperitas), the edit push (caught the biblia_dr
return conversions the instant the signature changed, the selectio
intermediate state, delineare's header/body redeclaration mismatch
— the push drove three fixes this arc), differentia not needed
(no predicate tuning). Asperitates — (1) percursus -semantica
prints only the aggregate census; corpus-wide per-file codex
attribution required a hand-rolled 812-file examen loop, whose
first (background) incarnation hung silently for 28 minutes;
desideratum filed. (2) `grep -v biblia_dr.c` also excluded
probatio_biblia_dr.c — substring filters eat sibling names; the
compile wall caught the missed caller. Fructus — calibration
categories held: every parity fix compiled clean first try against
-Werror; the vigilia CAUTIO correctly nagged resident staleness all
session. Desiderata — percursus per-file attribution flag
(-attributio?): print via:linea:codex rows during the sweep instead
of census-only.

## GRADUS CHORDA NUDA: INTENTIO (2026-07-17)

Menu pull (park 01KXQ82Z9Z item 1): chorda.datum handed to C-string
functions that assume NUL termination — the house rule clang cannot
know (pure inverse oracle; chorda is length+pointer, NOT terminated).

CALIBRATION SURVEY FIRST (the recipe, adapted — no clang oracle, so
the specimen battery is a CORPUS battery): direct str*+datum = 2
sites, BOTH RegioClicca (character[64], terminated by construction,
legitimate); printf "%s"+datum = 0 sites; chorda_ut_cstr bridge = 61
uses. TRUE MISUSE COUNT: ZERO. The discipline held — this tier is
PROPHYLAXIS, not harvest. Corpus-zero is the expected first-contact
state, and the bar.

DESIGN:
- Codex 59 CHORDA_NUDA, DOMESTICUM, verdict-neutral,
  TOLERA-suppressible (+1 _tolerabiles row) — severa-class doctrine
  (no oracle exists for house semantics).
- Predicate: call to a NUL-requiring sink; for each sink arg
  position, strip casts/parens; fire when the arg is a .datum/->datum
  member access whose BASE resolves to chorda type. Type-aware is
  mandatory: 10+ house structs carry a `datum` field (RegioClicca's
  is a terminated buffer — name-only matching would false-positive
  immediately).
- Sink lexicon v1 = UNBOUNDED readers only: strlen strcmp strcpy
  strcat strchr strrchr strstr strspn strcspn strpbrk strtok atoi
  atol atof strtol strtoul strtod sscanf puts fputs. Bounded readers
  (strncmp/strncpy/memcmp/memcpy) EXCLUDED — length-capped calls are
  the legitimate escape hatch alongside chorda_ut_cstr and %.*s.
- printf-family "%s" sink DEFERRED BY NAME to the format-signedness
  tier: both need the same format-string->vararg mapper; build the
  mapper once, both tiers ride it. (Corpus has zero %s+datum today;
  the wall arrives with the mapper.)
- Fires only at home call sites (alienity guard as usual); the sink
  being alien (libc) is the point.

BARS: unit section (fire forms: direct, cast-wrapped, ->datum,
nested parens; NEGATIVE: RegioClicca-shaped non-chorda datum,
strncmp bounded, chorda_ut_cstr result, %.*s idiom untouched);
corpus-zero sweep; fixture .severum (oracle SILENT — inverse);
vectis legs unchanged-generic; amalgam twin row; suites green.

## GRADUS CHORDA NUDA: RELATIO (2026-07-17)

SHIPPED — codex 59 CHORDA_NUDA, the first pure inverse-oracle tier
(no clang flag exists or can exist: the chorda contract is house
knowledge). Seat: the VOCATIO arg loop — callee name captured at
identifier resolution, sink mask (20 unbounded readers, arg-position
bitmask) checked per argument after the subtree is typed; the
predicate reads stored typations (never re-types — re-typing would
duplicate conversion annotations), sees through casts/parens, and
demands the BASE resolve to struct tag "chorda" (10+ house structs
have a datum field; RegioClicca's is a terminated buffer — field
name alone would false-positive immediately). TOLERA-suppressible.

THE SURVEY LIED, THE TREE DIDN'T: the calibration grep said zero
misuse sites. First corpus contact found ELEVEN — every one hidden
behind `atoi((character*)x.datum)`, where the cast's closing paren
terminates a [^)]* regex before it reaches datum. Surveys size the
work; only the parser is the truth. (Filed in the semantica worklog
next to the second strike: _diagnosticum_addere_plenum stores the
causa POINTER — structured causae must live in sem->piscina, not the
stack; the fixture printed garbage bytes until the VANA site's
pattern was copied.)

THE HARVEST (all 11 adjudicated same-arc):
- lib/biblia_visus.c x3 + lib/arx_caeli.c x3: entity-property
  chordas fed to atoi — reads past mensura until a non-digit byte
  happens; parses garbage silently when memory continues with
  digits. Fixed with chorda_ut_i32 (mensura-bounded, zero-default
  preserving old atoi-garbage semantics).
- probationes/probatio_http_parser.c x5: strstr assertions on
  serialized responses — passing only by luck of the byte after the
  buffer (incl. one NEGATIVE assert that could false-fail). Fixed
  with chorda_continet + chorda_ex_literis.

BARS: unit 10/10 probes (both operators, both strcmp args, paren
nesting, bounded-reader NEGATIVE, foreign-tag NEGATIVE, strcpy
position asymmetry, TOLERA, plain-identifier NEGATIVE); fixture
chorda_nuda.severum (pins 13/18, TOLERA'd twin quiet, oracle silent)
dropped into vectis leg ②c generically — VECTIS TENET; amalgamare
VERIFICATUM; silva 30/30; officina 12/12; corpus census 59 = 0
POST-ADJUDICATION; 58 = 46 and REICE = 41 both unchanged.

Deferred by name: printf-family "%s" sink rides the future
format-signedness tier (shared format-string->vararg mapper).

## GRADUS SIGNUM FORMATI: INTENTIO (2026-07-17)

Menu pull: printf-family format/argument signedness (codex 60
SIGNUM_FORMATI) + the shared format-string->vararg MAPPER, which
also delivers the deferred chorda "%s" sink (codex 59 extension).
One build, two walls.

ORACLE REALITY (why this is ours): clang's -Wformat already polices
SIZE mismatches (%d fed long fires under house flags - corpus has
none by construction) but is deliberately silent on same-size
SIGNEDNESS (%d fed unsigned int; -Wformat-signedness is GCC-only,
clang never implemented it). So the mapper only judges signedness
class {signatum, insignatum, aliud} - size is clang's beat. Pure
inverse oracle; severa-class doctrine (TOLERA-suppressible,
DOMESTICUM, verdict-neutral).

DESIGN DECISIONS, named up front:
- Functions: printf(fmt=0), fprintf(fmt=1), sprintf(fmt=1).
  imprimere is a macro - the aestimator sees the expansion free.
  scanf family EXCLUDED v1 (different direction, different rules;
  named future). va_list variants excluded (varargs invisible).
- Format must be a SINGLE string-literal node; anything else
  (variable, adjacent-literal concatenation if parsed multi-node)
  -> silent skip. Safe direction; named limitation.
- Star width/precision (%*d, %.*s) CONSUME an int arg but are NOT
  judged: printf("%.*s", c.mensura, ...) with i32 mensura is THE
  blessed house idiom - judging star-args would fire on every
  correct use. Named exemption.
- %x/%o EXEMPT v1: hex/octal on signed values is representation
  idiom, not arithmetic intent. %u fires. (Specimens to Fran if
  the census suggests revisiting.)
- Range suppression mirrors SIGNORUM: provably/heuristically
  nonneg unsigned in %d cannot print wrong (promoted u8 fits int)
  -> suppressed; constants likewise.
- %s + chorda.datum -> codex 59 through the mapper; %.*s + datum
  is the LEGITIMATE idiom and stays silent (precision bounds the
  read) - the mapper closing exactly the hole the chorda INTENTIO
  deferred.

BARS: unit probes (both directions, star-arg exemption, %.*s-vs-%s
on datum, %x exemption, length-modifier transparency, non-literal
format skip, TOLERA); fixture .severum (oracle silent); vectis
generic legs; amalgam twin; corpus census + adjudication with eyes;
suites green.

## GRADUS SIGNUM FORMATI: RELATIO (2026-07-17)

SHIPPED — codex 60 SIGNUM_FORMATI + the format-string->vararg
MAPPER (fragments + escape decoding via _fugam_decodere, so even a
\045-encoded '%' parses correctly; star width/precision consume
args unjudged; unknown conversion = whole format unjudged, safe
direction). The mapper also closed the chorda deferral: "%s" fed
chorda.datum now fires codex 59 through it, while "%.*s" (precision
bounds the read) stays silent — the blessed idiom recognized
structurally.

THE CALIBRATION DECISION (DECRETUM, Fran): first census = 544
firings, and attribution showed 443/443 source sites in ONE
direction — %d fed unsigned — with ZERO in the other. The %d
direction is innocuous in practice (wrong only above 2^31) and
ACCIDENTALLY DIAGNOSTIC: a wrapped i32 printed via %d shows as a
negative number, instantly visible; %u would faithfully mask it as
4e9. Decision: the %d-with-unsigned direction is NOT judged; only
%u-with-signed fires (-1 as 4294967295 = genuinely misleading
output). Post-decretum census: 60 = 0 — a pure wall on the
direction that matters.

TWO STRIKES for the book: (1) the first predicate SELF-SUPPRESSED
the whole %d direction — iv.non_negativum is vacuously true for
every unsigned expression; the exemptions had to be direction-
asymmetric (then the decretum removed that direction anyway);
(2) our own SIGNORUM tier caught a mixed s32/i32 compare in the
fix mid-edit — the walls police their own masonry in real time.
Also: editing a fixture's header comment shifts its EXSPECTA lines
(vectis caught the stale pin immediately).

BARS: unit 872/872 (both directions' probes updated to the
decretum, star exemption, %.*s-vs-%s on datum, %x exemption, %lu
transparency, non-literal skip, %% skip, \045 flex, TOLERA,
fprintf position); fixture signum_formati.severum (pin 10, %d twin
DELIBERATELY unpinned-silent as the decretum's specimen); VECTIS
TENET; amalgamare VERIFICATUM; silva 30/30; officina 12/12; census
60 = 0, 58 = 46 and REICE = 41 unchanged.

## GRADUS SENTINELLA + DEGRADATIO: INTENTIO (2026-07-17)

Two small tiers in one arc, both generalizations of specific corpses
from the VANA harvest:

CODEX 61 SENTINELLA_INSIGNATA (the producer side): a redde whose
expression is a NEGATIVE CONSTANT (seen through casts/parens) in a
function whose return type is unsigned integral. This is the tcp/
tls/biblia disease AT BIRTH: `redde (i32)-1;` in an i32 function.
The uncast form (`redde -1;`) is already codex 54 parity - clang
fires on the implicit conversion; the CAST is precisely what
silences both clang and parity, and precisely what the corpus wrote
everywhere. VANA catches the dead CONSUMERS; this catches the
PRODUCER even before any consumer checks. v1 = redde only;
assignment/initializer producers named-deferred. TOLERA-bilis
(deliberate all-bits idioms exist).

CODEX 62 COMPARATIO_DEGRADATA (unsigned <= 0): `X <= 0` / `0 >= X`
on unsigned X - NOT tautological (VANA structurally blind: it
degrades to == 0, a DIFFERENT comparison, not a constant one). The
delineare circle error-term bug wore exactly this shape. Seat:
_comparationem_examinare already receives op/sides/commune - the
two zero-position/op combinations VANA does NOT map (<=0 right,
0>= left) are exactly ours. Same sound basis (commune unsigned or
severum-nonneg range). CENSUS RISK, named up front: `mensura <= 0`
as an emptiness check may be a widespread benign idiom - if the
census is large, adjudication options go to Fran before anything
ships as standing noise (the %d-direction lesson).

BARS: unit probes both codices (cast-wrapped fire, uncast = 54 not
61, positive-constant NEGATIVE, signed-return NEGATIVE, <=0 both
operand orders, >0 NOT fired, TOLERA both); fixtures .severum
(oracle silent - both inverse); vectis generic; amalgam twin rows;
census + eyes; suites green.

## GRADUS SENTINELLA + DEGRADATIO: RELATIO (2026-07-17)

TWO TIERS, TWO OPPOSITE VERDICTS — the census-as-experiment pattern
at full power.

CODEX 61 SENTINELLA_INSIGNATA: SHIPPED AND HARVESTING. Census = 4;
two frozen (arbor_praeparator, standing by decree), TWO REAL BUGS
fixed same-arc:
- sputnik_lexema._convertere_effugium: doc said "-1 si ignotus"
  over an unsigned return (the doc-comment liar AGAIN); its three
  consumers checked `effugium >= 0` — always true — so unknown
  escapes emitted a truncated 0xFF byte instead of the literal
  character. Producer AND three dead consumers fixed in one stroke
  (VANA tail 46 -> 43).
- sententia_iterator_index: public API returning (i32)-1 for NIHIL
  iter — an indistinguishable sentinel with NO consumer checking
  yet. Exactly the "catch the producer before any consumer exists"
  case the tier was built for. Now s32.
Predicate subtlety found by calibration probe BEFORE tests: the
aestimator does NOT fold casts, and the bare form (redde -1) is
parity 54's — so the tier strips casts itself and fires ONLY when
a cast was present (the form that silences clang). No overlap, no
double-fire.

CODEX 62 COMPARATIO_DEGRADATA: BUILT, MEASURED, PUT TO SLEEP
(decretum). Census = 228: ~88 in generated tables, 18 delineare
validity guards, the rest mensura-<=-0 emptiness checks — the
idiom OWNS the shape, and the needle (arithmetic sign intent, the
circle error-term form) is statically indistinguishable from it.
The separator is the operand's DEFINITIONS (accumulation vs
assignment) = def-use = CFG stage 2. The firing sits behind
_degradata_vigil (FALSUM) with the re-arm condition written at the
seat; codex number reserved (ABI append-only); dormant specimen
fixture kept. The census WAS the experiment — we now know this
corpus's <= 0 is emptiness, which we did not know this morning.

BARS: unit 919/919; fixtures sentinella_insignata.severum (pin 7,
TOLERA twin quiet) + comparatio_degradata.severum (dormant, no
pins); VECTIS TENET; amalgamare VERIFICATUM; silva 30/30; officina
12/12; census 61 = 2 (frozen, standing), 62 = 0 (dormit), 58 = 43
(sputnik harvest), REICE = 41.

## GRADUS FLUXUS-0 (CFG + attingibilitas): INTENTIO (2026-07-17)

The biggest remaining lint-lane pull, and the first new DATA
STRUCTURE since the interval machine: a statement-level control-flow
graph per function, built silva-side on the tree (dependency law;
code-is-a-database pin — the CFG must survive into queryable form,
not be consumed and discarded inside one tier).

CALIBRATION (done first, live Apple clang 16, specimens in scratch):
- House flags: BLIND to fall-through and unreachable code (zero
  diagnostics); -Wreturn-type is in -Wall and fires on both
  missing-return-path forms (if-without-else, incomplete switch).
- -Wimplicit-fallthrough (C89 mode): fires at the RECEIVING label
  when the previous group's end is REACHABLE (path-based: `casus 0:
  si (x) frange;` falls through and fires) and the previous group is
  non-empty; label stacking (casus 4: casus 5:) is silent; comments
  do NOT suppress (GCC honors them, clang does not); the sanctioned
  suppression __attribute__((fallthrough)) is not house-writable.
- -Wunreachable-code: post-jump statements fire; return-after-
  infinite-loop needs -aggressive (DEFERRED by name, path back =
  same CFG, just an extra "no terminating edge" query).

ARCHITECTURE: new module silva_c89_fluxus.{h,c} (one unit-tested
module at a time). FluxusFunctionis per definitio-functionis:
blocks = runs of statement nodes (statement granularity), typed
edges (sequens/verus/falsus/casus/ruptura/persecutio/salta),
jump resolution (frange -> nearest breakable, perge -> nearest
loop, salta -> titulatum by name — C89 labels are function-scoped),
attingibilitas marking from entry. Blocks REFERENCE tree nodes
(every field a future query column). Piscina-allocated, caller-owned
lifetime like semantica.

SCOPE PINS (deferred use cases NAMED, paths back preserved):
- Statement granularity ONLY; no short-circuit/expression-level
  subdivision. Path back: MC/DC horizon + codex 62 stage 2 own
  expression-level flow; block struct must reference expression
  nodes intact so a later pass can subdivide WITHOUT re-shaping the
  block API.
- Conditional arms: v1 builds on the taken track (RAMUS_SUMPTUS)
  only. Path back: builder parameterized by arm selection when
  config-preset queries land (wildcard-conditional pin intact —
  the TREE keeps all arms; the CFG is a view).
- AMBIGUUS/ERROR nodes in statement position: opaque single-block
  statements, never crash (robustness bar).

ORDER OF CONSUMERS (the de-risking move): (1) SIMULATIO of the
module against shipped code BEFORE building (house law; complexity
schema); (2) build fluxus + unit suite; (3) FIRST consumer =
SEMITA_SINE_REDDITU (codex 63, -Wreturn-type PARITY) because the
whole corpus compiles under -Werror = census MUST be 0, and
differentia over 820 files becomes a free byte-exact validation
harness for the CFG's edge construction; (4) SECOND consumer =
CASUS_LAPSUS (codex 64, fall-through, house-blind oracle via flag,
TOLERA-bilis) — census + adjudication, options to Fran if large;
(5) THIRD (stretch, cheap once attingibilitas exists) =
SENTENTIA_INATTINGIBILIS (codex 65, plain -Wunreachable-code
semantics only).

OPEN QUESTION for adjudication time: honor the inherited
/* fall through */ comment idiom as suppression (comments are
content — a silva pin; GCC precedent) or TOLERA-only (uniform
house machinery)? The census will show whether the corpus already
speaks the idiom.

BARS: simulation complexities recorded; fluxus unit suite (new
probatio); parity tier differentia-validated over corpus; fixtures
.severum per tier; vectis generic legs; amalgam twins (silva.h
hand-mirror); census + eyes; suites green; codex 62 stays DORMANT
(its re-arm is stage 2 def-use, NOT this arc).

### FLUXUS-0 addendum (2026-07-17, pre-compaction)

Simulatio XI run against shipped code (project-specs/
silva-simulatio-11.md — 8 register entries; sim ledger 7-for-7)
then an 11-question interview (silva-fluxus-0-interview.md). All
recommendations accepted; consolidated into the ARC RE-ENTRY
DOCUMENT: **project-specs/silva-fluxus-0-spec.md**. Headlines:
auxilia callback seam (canonicum + aestimator are caller policy);
grammar REJECTS Duff (accepted, nota filed, flat group scan);
corpus has exactly ONE real fall-through (frozen, guarded);
VERDICT-PARITY HOLE found and verified (clang ERRORS on
frange-outside-context + salta-to-unknown-label, examen ACCIPE) —
closed in-arc as codices 66/67 VIOLATIO; 63 = SUSPECTUM; CFGs
retained in sem; 65 in-arc stretch; fluxus internal v1; TOLERA-only
suppression. Chunks: ① module+unit suite ② integration + 63/66/67 +
differentia sweep ③ 64 + census ④ stretch 65. NOTHING BUILT YET —
compaction next by decision; chunk ① starts from the spec.

## GRADUS FLUXUS-0: RELATIO (2026-07-17)

THE ARC SHIPPED WHOLE: CFG stage 0 + FIVE codices (63-67), four
chunks, each ending green, exactly along the spec's consumer order.
The simulation-first + interview + spec pipeline held: ZERO design
reversals during build - every wall the build hit had been named
beforehand except the ones the CENSUS was designed to find (which
is the point of the census).

CHUNK ①: silva_c89_fluxus.{h,c} (~750 lines) + 65-assert unit
suite, green on first run. Auxilia seam as designed (canonicum +
aestimator are caller policy); blocks by value in xar; violations
as data. One honest stumble: first Write of the .c went out
half-designed (stubbed context stack) - caught on re-read, rewritten
whole; the final stack idiom is live-depth over append-only xar.

CHUNK ②: semantica integration (sem->fluxus_functionum, retained
per Q7) + 63 SUSPECTUM + 66/67 VIOLATIO (the verdict-parity hole,
closed). THE ACCEPTANCE SWEEP: 649 files - 66/67 = 0 corpus-wide;
63 fired exactly twice = ONE real latent bug (roundtrip fixture
make_point returns struct from empty body - never compiled, clang
agrees line-exact); noreturn disagreement count = 0 (decision Q1's
branch never taken). New generic vectis leg ②b′ (*.suspectum,
oracle-paired); chorda_aequalis resurrected from excludenda.

CHUNK ③: 64 CASUS_LAPSUS. Census-taught, twice: (1) the alien
guard was missing (stb_image's three fall-throughs - DOMESTICUM
never judges alien; added); (2) the intentional +=/-= fall-through
in semantica ITSELF became the tier's first live TOLERA demand and
exposed a LATENT BUG: TOLERA above latin-keyword-led statements
NEVER absorbed (comment rides the macro invocation lexeme, which
expansion drops from the final stream; strata[0] = post-expansion,
dead end proven; fix = collector also examines silva_token_radix
per token). Affected ALL TOLERA-able tiers in Latin files since
the TOLERA arc - all 17 existing sites happened to be
identifier-led. Also documented: _tolera_legere is
single-line-strict. Census final: 1 (arbor_syntaxis:3504 frozen,
standing per Q2).

CHUNK ④: 65 SENTENTIA_INATTINGIBILIS, plain -Wunreachable-code
parity. TWO calibration surprises shaped the predicate: clang
FIRES on literal if(0) but is SILENT on if(MACRO_0) - the config
idiom exemption is macro-provenance-based, and OUR TREE KNOWS
PROVENANCE (plicatione_exemptus mark on folded-out blocks,
macro-tinged only; loop tails always exempt = -aggressive
territory, deferred by name). Skip-list redde/frange/perge-only
blocks (clang's -return/-break sub-flags = defensive-jump idiom).
Census: piscina's si(PISCINA_DEBUG) correctly exempt; final = 1
(arbor2_glr_tabula:25808, dead (vacuum)glr after redde -I, frozen,
standing).

STANDING METERS after the arc: 58=43 (VANA tail), 61=2, 63=2 (one
defect in a non-compiled roundtrip fixture, clang-confirmed -
Fran adjudicates fix-vs-stand), 64=1 (frozen), 65=1 (frozen),
REICE=41. Bars: silva 31/31, officina 12/12, probes 1043/1043,
VECTIS TENET, amalgamare VERIFICATUM, percursus 823 trees /
0 error nodes.

INSTRUMENTA DEBRIEF (adhibita/fructus/asperitates/desiderata):
- adhibita: arbor.sh (live parse dumps DROVE the simulation -
  Duff rejection + stacking shape found pre-build), differentia
  (5-file spot sweep), examen -machina + xargs -P4 recipe (three
  clean census sweeps), vectis (absorbed 4 fixtures + 1 new
  generic leg), push (caught _tolera_quaerere phantom call live),
  legati corpus (seat reads), store (all decisions ledgered).
- fructus: the census-as-experiment pattern went 3-for-3 THIS ARC
  (alien gap, TOLERA latent bug, if(0)-vs-if(MACRO) split) - first
  contact with the corpus keeps out-teaching design; twin-specimen
  bisection (C vs Latin) cracked the TOLERA bug in two probes.
- asperitates: xargs -I{} "too long" recurred (runner-script form
  is now the recipe of record); fixture line-pins shifted twice on
  comment edits (relative-pins desideratum 01KXRD8JVS still open);
  legati resident staleness nags during heavy edit runs.
- desiderata: TOLERA multi-line parsing (filed); a census
  convenience (examen -census CODEX over a file list) would
  replace the runner-script dance.

## GRADUS ANGUSTATIO + INUTILES: INTENTIO (2026-07-17)

The lint menu's LAST two items - both parity tiers on warm
machinery; after this arc the menu is EMPTY and everything left in
the lane waits on flow-analysis stage 1+.

CODEX 68 ANGUSTATIO (width-truncation, paritas -Wconversion):
implicit conversion to a NARROWER integral destination (i32->i8,
s64->s32...) - the width-direction sibling of 54's sign direction.
Widths and the interval machine already exist (conversio-signi
era); seat = the same annotation funnel. Corpus census MUST be 0
(house -Werror -Wconversion) = another free differential
validation. Calibration FIRST: clang exempts provably-fitting
constants and range-narrowed expressions (IntRange logic, same
shape 54 mimics). SEVERA direction (cast-wrapped truncation,
(i8)magnum - the form that SILENCES clang, codex 61's cast story)
is NOT pre-decided: the parity census runs first and the follow-up
is measured before built. Gradus DOMESTICUM per family precedent
(54/57), verdict-neutral. Named subtlety: differentia bucketing
must not conflate [-Wconversion] with [-Wsign-conversion] lines
(bracket-exact exemplar).

CODEX 69 VARIABILIS_INUTILIS (paritas -Wunused-variable, in -Wall)
+ CODEX 70 PARAMETRUM_INUTILE (paritas -Wunused-parameter, in
-Wextra) - numbers reserved together, built in that order, 70 only
if 69's plumbing makes it cheap this arc (parameter shapes: K&R,
prototypes - measured, not assumed). Predicate: block-scope symbol
with ZERO resolved use sites - sem->symbola knows declarations +
depth, nexus knows uses; smallest-change wins between a per-symbol
usus flag set at resolution vs a reverse tally. The house
(vacuum)x idiom counts as a use for clang AND for our resolution -
calibrate to confirm. "Set but unused" (-Wunused-but-set-variable)
is OUT of scope, deferred by name. Gradus DOMESTICUM.

BARS: calibration specimens pinned before predicates; unit probes
both codices; fixtures on the oracle-paired .domesticum leg (flags
passed explicitly); differentia rows; corpus census (expected 0 /
0 / small for 70 - the (vacuum) idiom exists BECAUSE the wall
demanded it); suites green each chunk; amalgam twins (enum rows
68-70 in silva.h).

CHUNKS: ① calibratio + 68 + census/differentia; ② 69 (+70 si
vilis) + census. Each ends green.

## GRADUS ANGUSTATIO + INUTILES: RELATIO (2026-07-17)

THE LINT MENU IS EMPTY. Codices 68/69/70 shipped in two chunks;
everything left in the lane now waits on flow-analysis stage 1+.

CHUNK ① ANGUSTATIO (68): calibration first paid immediately -
"truncation" is THREE clang sub-flags under -Wconversion
(constant/implicit-int/shorten-64-to-32; one codex ours), fitting
constants exempt, & mask credited but % NOT. Seat = the 54 funnel
(sibling function; same-sign narrowing was the exact hole in 54's
early return; cross-sign width slips 54's non-negative exemption -
gap closed with no double-fire). FIRST CENSUS = 87 rows, ALL false
positives (corpus compiles -Werror): three classes taught the
_angustatio_capit recursive fit-checker (negative constant tables
via exact aestimator range - base64's -1 in s8; ternary arms
each-fitting - selectio's ' ' : cast; structural type width).
SECOND CENSUS = 0. PURE WALL. Specimen parity 6/6 line-exact.
Differentia grew '|'-alternative bracket matching (umbrella flags).

CHUNK ② INUTILES (69/70): usus flag on SemanticaSymbolum, set at
_nexum_ponere (resolution IS the use-recorder; (vacuum)x counts for
free, same as clang). Calibration: side-effect initializers still
fire, static locals fire, extern-local declarations don't. TWO
TU-degradation lessons from the census: capsula_libri.c is a
DEGRADED TU for examen (REICE, 293 unknown named types - includes
resolve for clang but not fully for examen; pre-existing) whose
symbol layer breeds phantoms - venenata guard per symbol (error
type) PLUS a TU-level abstention (any TYPUS_NOMINATUS_IGNOTUS =>
the unused pass abstains; symbol-layer judgments need a resolved
type context). FINAL CENSUS: 69 = 35 + 70 = 8, EVERY row inside
probationes/fixa/ roundtrip parse fixtures where unused params are
LOAD-BEARING test shapes - recommendation: fixa/ is out of the
judging domain (like vendor/); in maintained code both are PURE
WALLS. Probe scaffolding: typing probes' exact-count asserts now
filter 69/70 (_ordines_praeter_inutilia) - typing asserts keep
typing meaning.

PROCESS SLIP, RECORDED HONESTLY: chunk ①'s commit gate was a shell
chain gated on grep (matched the tally line, ignored the suite's
exit) - a red suite (UAC probe, outdated count) reached a commit;
fixed forward within minutes (3aa7a2c). New discipline: gate on
script exit codes, grep only for display. The slip recurred as a
NEAR-miss in chunk ② and the exit-gate caught 4 outdated asserts
pre-commit - the lesson held.

BARS: probes 1119/1119; fixtures 3 new on the oracle-paired
domesticum leg (68/69/70, all clang-line-exact); VECTIS TENET;
amalgamare VERIFICATUM; silva 31/31 (exit 0); officina 12/12.
STANDING METERS: 54-67 unchanged; 68 = 0; 69/70 = 0 extra fixa.

INSTRUMENTA: calibration-probe discipline caught the three-subflag
split and the %-vs-& asymmetry pre-build; the census-as-experiment
pattern went 5-for-5 this arc (3 capit classes + 2 TU-degradation
guards); the differentia '|' extension is reusable machinery.

## EXAMEN -CENSUS: INTENTIO (2026-07-17)

Pull: desideratum 01KXRXW70T (census half) — codify the per-codex
corpus sweep that every tier arc has re-derived by hand (three
times in fluxus-0 alone; also where the while-read-in-background
28-minute hang lives).

Surface: `./silva/examen.sh -census [codex] [radix] [-posix]` —
pure shell orchestration around the freshly-built binary; no C
changes. Walk = .c files from radix (default .), skipping
build/.git/results/node_modules/hidden (mirrors percursus.c
_praetermittendum; percursus additionally walks .h — census is
.c-only, validated against standing meters below). xargs -P 4 over
`build/examen -machina`; raw rows land in build/census.tsv and
SURVIVE the run (immediate re-slicing without re-sweep). With a
codex: site rows via:linea:columna + causa, per-file tally, fixa/
subtotal. Without: codex × sedes × plagulae table. Verdict tally
(ACCIPE/REICE) both ways; INFRA files (exit 2) collected by NAME —
no silent drops. Eyes-tool, not a gate: exit 0 = sweep completed.

Bars: lib/-radix smoke (expect chorda ×2, chorda_aedificator ×2
from the VANA attribution table); full sweep must reproduce the
standing meters (58 = 43, REICE = 41) or the divergence gets
explained before shipping.

## EXAMEN -CENSUS: RELATIO (2026-07-17)

Shipped in one chunk: `./silva/examen.sh -census [codex] [radix]
[-posix]` — ~90 lines of shell orchestration, no C changes. Walk =
.c files (percursus also walks .h; every per-codex meter reconciles
on .c-only, so headers-standalone contributed no sites — the "820
files" historical counts were .c+.h). Raw rows land in
build/census.tsv and SURVIVE the run; re-slicing needs awk, not a
re-sweep.

THE INTENTIO BAR PAID IMMEDIATELY: the first full sweep diverged
from FIVE standing meters (54=1138!, 68=216, 64=58, 65=4, 58=46,
REICE=43). Attribution over the raw table (no re-sweep needed —
the survives-design proving itself during its own build): every
excess row was vendor/sqlite3.c. The hand indexes had been
excluding vendor/ all along — the judging-domain rule ("fixa/ is
out of the domain, like vendor/") was live practice never written
into a walk. vendor/ now pruned by name; the radix argument itself
is never pruned, so `-census 54 vendor` still sweeps it
deliberately. Second sweep: EVERY meter exact — 58 = 43 in 22
plagulis (39 source + 4 amalgam shadows), 61=2, 64=1, 65=1, 69=35 +
70=8 all-in-fixa, 54/55/57/63/68 pure walls (rows absent).

Second in-arc find: VERDICTUM rows carry no via, so the REICE
count was un-attributable from the raw table. The wrapper now
appends `VERDICTUM_PLAGULAE\tvia\tACCIPE|REICE|INFRA` per file;
the tally reads those; INFRA files are also named (nothing silently
dropped). Attribution immediately explained the last divergence:
REICE 42 vs meter 41 = lib/vigilia.c, newly POSIX-lexicon via
K2.3's stat helper (post-meter; ACCIPE under -posix). Meter
updated, not a regression. NEW REICE METER: 42.

BARS: lib/ smoke = VANA attribution table exact (33 sites/16 files,
sputnik_lexema correctly absent post-fix); full-sweep meters above;
VECTIS TENET (exit 0, gated on script exit per the new law);
single-file passthrough intact.

INSTRUMENTA: adhibita — raw-table awk was the whole diagnosis
loop; asperitates — VERDICTUM-sine-via was the one gap, closed
in-arc; desiderata — none new. The TOLERA rider on 01KXRXW70T
stands documented-as-designed (single-line official; a small
multi-line reader only if causae grow long).

## VANA CAUDA: INTENTIO (2026-07-17)

Pull: finish quaestio 01KXQ9NF8J. Fran's decretum: the frozen
parser family (arbor v1 + arbor2 + lapifex = 8 sites incl.
arbor_quaestio) stands AS-IS in the census — no fixes, no TOLERA,
no exclusion; named standing residue like codex 61. Remaining work
= 31 sites / 16 files (43 − 8 frozen − 4 amalgam shadows), core-lib
first (chorda ×2, chorda_aedificator ×2). Recipe per the quaestio:
read the declared type and intent BEFORE surgery; s32-ify or
restructure; the examen push heals -Wsign-conversion ripples at
edit time. Amalgams regenerate after (silva embeds chorda; officina
embeds indicium/machinula/aedificator shadows). Bars: touched-lib
suites + officina/silva suites green; examen -census 58 == 8
(frozen only) at close; quaestio closes with the decretum.

## VANA CAUDA: RELATIO (2026-07-17) — QUAESTIO CLAUSA

31 sites / 16 files adjudicated in one pass; census 58 = 8, all
frozen-family (arbor v1/arbor2/lapifex per Fran's decretum
01KXSAK5K7), zero in maintained code. Root tests exit 0, officina
12/12, silva 31/31, both amalgams VERIFICATUM.

LIVE BUGS KILLED (the tail was NOT innocuous):
- chorda_praecidi_laterale: ALL-WHITESPACE input = infinite loop
  with wild reads (backward scan wrapped) AND the initium default
  would have returned the untrimmed string — two bugs on one line
  in the hottest string function. Count-down idiom + not-found
  sentinel.
- sputnik slice/substring: negative-index semantics (a documented
  JS-style feature) were DEAD — slice(-2) returned empty. s32
  indices bring the feature to life.
- entitas_repositorium legere_eventus_recentes: asking for more
  events than exist returned NONE instead of all (tail-window
  subtraction wrapped).
- vim _clamp_cursor: cursor-up at top row jumped to BOTTOM (wrap
  through the upper clamp). Written intent (clamp at top) restored
  — NOTE: user-visible behavior change, flagged to Fran.
- sessio _turnum_possidentem: defensive clamp deflected to the
  LAST line instead of first when wrapped.
- schirmata_prior: cycle-to-last was correct BY DOUBLE ACCIDENT
  (wrap + upper clamp = the intended value); now honest. machinula
  ring lookback similarly luck-saved by zeroed slots; now honest
  s64.
- arx_caeli click mapping: clicks slightly left of text truncated
  to column 0 (division before sign check); signed delta computed
  first now.

PATTERNS: s32-ification where negativity is MEANT (coordinates,
sentinels, deltas, tail-windows); dead-guard DELETION where the
upper bound provably subsumes wrap (libro, indicium format fields,
entitas skip); s32-VIEW containment at chokepoints where a field
flip would cascade (vim cursor fields — 177 sites, deeper cut
named in worklog; arx trahere_grid). Two vacuous probatio asserts
made meaningful (sectio ordinatio < 32, tabula_dispersa probe
distance < capacity — a true invariant).

FOOTNOTE: capsula_libri.c flips REICE→INFRA under the parallel
census (4 × 256MB piscinae + a 128K-line TU = allocation fails
under load; solo run REICEs normally). The census NAMED it instead
of dropping it — the no-silent-drops design proving itself.

INSTRUMENTA: examen -census 58 was the worklist AND the closing
meter (built this morning, load-bearing by evening); the examen
push caught every -Wsign-conversion ripple at edit time (schirmata
margin_right UAC, sputnik temp, arx call sites) — zero compile
surprises at suite time.

## PINNAE RELATIVAE: INTENTIO (2026-07-17)

Pull: desideratum 01KXRD8JVS (pins half; vigilia half = K2.3).
Absolute EXSPECTA pins invalidated by ANY edit above them — five
recorded recalibrations in one day; the cost is the bar's signal
(red must mean the judge changed). Fix: `/* EXSPECTA-PROXIMA:
CODEX */` placed directly above its target — binds to the next
non-pin, non-blank line; pins stack; absolute grammar stays as
fallback (line-1 edge, migration safety). One shared resolver
(_pinnae_solvere awk) replaces the four grep|sed extractions in
vectis; fixtures converted wholesale by a target-keyed awk
(insert-above + drop standalone absolute pins, indentation
matched). Bars: total resolved-pin count identical pre/post
conversion; VECTIS TENET exit 0 (vectis itself is the verifier of
pin correctness — every pin found + oracle same-line + counts).
Consumer sweep: vectis is the SOLE reader (semantica hit =
EXSPECTATA substring; censoris fixture out of scope).

## PINNAE RELATIVAE: RELATIO (2026-07-17)

Shipped: `/* EXSPECTA-PROXIMA: CODEX */` is now the preferred pin
grammar — one shared awk resolver (_pinnae_solvere) feeds all four
vectis legs the same linea:CODEX pairs from either form; pins
stack, skip blanks, and bind to the next non-pin line. All 30
fixtures converted wholesale (39 pins: 38 relative + the ONE
legitimate absolute — ordo_pravus_vendicati pins LINE 1, above
which nothing can sit; the fallback grammar earned its place
before the arc even shipped). Conversion by target-keyed awk,
insert-above with matched indentation, drop-converted-standalone.

BARS: resolved-pin count 39 preserved through conversion; VECTIS
TENET exit 0; and THE USER STORY ITSELF as a test — inserted a
header line into a converted fixture, vectis STAYED GREEN, which
is the exact edit that produced five false reds in one day.
Backward compat incidentally proven live (a git checkout mid-test
restored a pre-conversion fixture; vectis accepted both grammars
side by side).

PROCESS NOTE: the multi-pin conversion first failed loudly (BWK
awk rejects newlines in -v strings) and the `&& mv` guard left
every failed file UNTOUCHED — cheap atomicity that turned a bug
into a no-op instead of corrupted fixtures. Worth keeping as the
default shape for in-place transforms.

Flow stage 1 lands on this: new fixtures are born with relative
pins, never carrying the absolute-pin debt.

## FLUXUS GRADUS 1 — CHUNK 0 (FINDERE): INTENTIO (2026-07-17)

Pull: park 01KXSETFAP (flow stage 1) → spec
project-specs/silva-fluxus-1-spec-v2.md (codebase-grounded;
calibration battery + simulatio + 12-question interview recorded
there, in silva-fluxus-1-interview.md, and in the park notae). Arc =
chunk 0 (this) → A eventa (fluxus_datorum sidecar) → B punctum fixum
(may/must) → C codices 71 LECTIO_ININITIATA / 72
ININITIATA_QUANDOCUMQUE / 73 ININITIATA_FORSITAN (dormant).

GATE SPECIMENS RAN FIRST (round 5, scratchpad calibratio-fluxus-1/,
results folded into spec §2):
- s18 sizeof: SILENT (unevaluated) — extraction skips magnitudo.
- s19 member-then-whole: silent BOTH flag sets even with a member
  never written — member/element store = FULL def of the whole
  aggregate. The v1-spec may-def rule was WRONG; sigillum's residual
  fire re-explained as zero-trip-loop, not grain. DEFINITIO_FORTASSE
  event genus DELETED from the design (YAGNI, named path back).
- s20 condition-position ||: FIRES "whenever '||' condition is
  true" in BOTH if- and while-condition seats — clang splits
  condition evaluation. Chunk-0 condition seat IN SCOPE.

CHUNK 0 SCOPE: shared splitter (working name _expressionem_findere)
for expression trees containing BINARIUM(ET_ET/VEL_VEL) or
TERNARIUS: evaluation-segment blocks, VERUS/FALSUS edges with origo
= the operator node, granules in sententiae, full statement
recoverable by pater-climb. TWO seats: statement position
(_sententiam_ambulare ordinarius: default, fluxus.c:891) +
condition position (five compound builders). Fold-first: _aestimare
on the whole condition runs before any split decision (constant
conditions keep today's folded shape); partial-fold behavior (si (0
&& f())) pinned by fixture in-arc. KNOWN-HARD SPOT: loop conditions
with short-circuits — the back edge (dum corpus→caput, per
passus→caput) must target the FIRST condition-segment block
(evaluation restarts whole); postis/exemptus wiring must survive.
Statements and conditions without the operators stay byte-identical
to stage 0.

BARS (all before chunk A): vectis TENET on script exit; census
meters byte-stable (63=0 pure wall, 64=1, 65=1, 66/67 + every
non-fluxus meter); suites green root + silva + officina; amalgams
VERIFICATUM. NAMED DELIBERATE MOVEMENT: probatio_silva_fluxus
margin-count pins (:196-298) and _bloci_mortui_pleni pins
(:352/:406) will move with the new VERUS/FALSUS edges — each moved
pin re-blessed by reading what it now pins (golden blessing law).
New fixtures for split shapes born with EXSPECTA-PROXIMA.

CONSUMER AUDIT (blast-radius pass, all in _fluxum_examinare
semantica.c:3091-3243): codex 64 label-head test unaffected (split
blocks carry titulus_dux NIHIL); codex 65 may name a granule as
culpa in unreachable split blocks (accepted — position stays valid;
RELATIO notes actual behavior); 63/66/67 untouched.

PROCESS NOTE: full phase-log re-read at this phase boundary NOT
done (484KB — the phase-log-size strategy conversation is a named
open item); tail + park + spec carried re-entry instead.

## FLUXUS GRADUS 1 — CHUNK 0 (FINDERE): RELATIO (2026-07-17)

Shipped (057d8ba): the CFG splits short-circuits. One shared
machinery — _sectionem_findere (value position: segment blocks with
own iunctio), _conditionem_findere (condition position: cascade
whose section edges ARE the branch edges, s20 parity; ! swaps
targets; deep sections under comparisons handled by the terminal via
_interna_findere), _folium_findere_aut_addere at all four statement
seats (ordinarius default, redde, per-initium, per-passus) plus the
commutatio discrimen. Edge origo = the BINARIUM/TERNARIUS node;
final-operand edges carry the compound node (s21 specimen owns
confirming clang's vocabulary before chunk C messages).

DESIGN DEVIATION FROM INTENTIO (improvement found mid-build, spec
updated): conditions are granules UNIVERSALLY — the cascade terminal
naturally emits its sub-expression into its evaluation block, so
simple conditions emit too, and chunk A's two-source extraction rule
(leaves + edge-origin conditions, switch pater-climb) DIES. Single
source: sententiae in order. Folded conditions keep their granule;
the skip-invariant drops their operands, which is CORRECT (never
evaluated) — documented in fluxus.h with the praetermissio
invariant.

BARS: 113/113 fluxus (48 new asserts — every analytically derived
margin/granule/reachability count held on FIRST run: cascade si/dum,
statement/redde/passus positions, ternary, a&&(b||c) nesting,
!-swap, fold-boundary f5), silva 31/31, VECTIS TENET, census meters
BYTE-STABLE over 626 files (58=8, 61=2, 64=1, 65=1, 63/68 walls,
69/70 in-fixa, REICE=42, INFRA=0) — the surgery is invisible to
every stage-0 judgment over a corpus full of real short-circuits.
Amalgama VERIFICATUM, officina 12/12. The INTENTIO's expected
fixture-pin movement never materialized: stage-0 fixtures contain
no short-circuits, so unaffected shapes were byte-identical exactly
as claimed.

PRE-EXISTING REDS FOUND (bisected by stash — NOT chunk 0):
probatio_officina_legatus pinned "clausura reversa (3)" for
legatus.h; sonda.c (6998d0d) is the fourth includer — pin re-blessed
to 4 (c35eb4f). probatio_officina_machinula is CWD-SENSITIVE (file
loop test opens officina/build/pa.txt RELATIVE) — the suite must run
from the repo root; ran green there.

INSTRUMENTA: adhibita — examen push healed each -Wconversion ripple
at edit time; census-as-regression-bar carried the whole corpus
claim in one sweep; stash-bisect settled causality in two minutes.
Asperitates — officina suite cwd-sensitivity cost one false alarm
(worth a cd-to-root guard in the script someday, named not built).
Desiderata — none new.

NEXT: chunk A (silva_c89_fluxus_datorum sidecar — eventa + symbol
facta seam), per spec §4. s21 specimen before chunk C vocabulary.

## FLUXUS GRADUS 1 — CHUNK A (DATORUM): RELATIO (2026-07-18)

Shipped (580977e): silva_c89_fluxus_datorum.{h,c} — the retained
dataflow tables. Per-block ordered event streams (USUS / DEFINITIO /
DEFINITIO_LOCI; variabilis -1 = def-omnia for opaque ERROR/AMBIGUUS
leaves), dense variable table, and the facts seam (FluxusDatorumAuxilia:
symbolum / parametrum_constans / canonicum callbacks — datorum never
includes semantica, XI-1 pattern; the seam returns FACTS, not the
symbol struct). Every calibration pin is a walker rule: eval-order
(RHS before LHS-def, use-then-def for +=/++), magnitudo skipped,
(vacuum)x-bare-identifier skipped, member/element store = FULL def
of the whole aggregate, member reads invisible, scalar-pointer bases
read (p[i]/p->a/*p), &x = LOCI except const-param call args (s04f),
short-circuit subtrees skipped whole (the chunk-0 praetermissio
invariant, single-source granule streams).

SEAT: _fluxum_examinare builds datorum for every function, always-on
(Q8). New substrate contract: DECLARATOR nodes now registered in the
nexus (symbolum_nodi resolves them) — with a notare_usum flag on
_nexum_ponere so declaration-registration does NOT mark usage
(codices 69/70 stay honest; their fixa meters proved it). Wrappers
compute facta from genus/profunditas/repositio and walk real
TypusC89 function types for per-position pointer-to-const.

BARS: 184/184 unit (test-local name-keyed resolver); e2e through
real semantica: shadowing yields THREE distinct identities,
peek(const int*) yields ZERO events, fill(int*) yields one — the
s04f pin proven end-to-end through actual type lookup. Silva 32/32,
VECTIS TENET, census meters byte-identical with the seat live over
the whole corpus; census wall 119s = the standing ~2min meter — the
always-on extraction is inside noise (Q8 answered: no gate needed).
Amalgama VERIFICATUM, officina 12/12.

FINDS: (1) unit-test pipeline without semantica exposed the AMBIGUUS
reality — sizeof(x), f(s), g(p[a]) all parse ambiguous without
typedef knowledge; the walker's def-omnia abstention handled them
CORRECTLY (that's what the rule is for), tests de-ambiguated their
sources and gained a dedicated def-omnia case; (2) tok_verba is a
LISTA of tokens with SILVA_LEX_VOID — first (void)x check was
shape-wrong twice; (3) the amalgamator's duplicate-definition gate
caught three cross-TU static name collisions (_canonicum,
_per_parentheses, _nodalis → _lectio_canonica, _sine_parenthesibus,
_ut_nodus) — the single-TU discipline enforcing itself; (4) I used
`nomen` as a struct FIELD name in the probatio — the oldest trap in
the book, still catching; examen push cascade named it.

NEXT: chunk B — praedecessor index + may/must bitsets + fixpoint
(spec §5); then C iudicium (71/72/73). s21 specimen before chunk C
vocabulary.

## FLUXUS GRADUS 1 — CHUNK B (PUNCTUM FIXUM): INTENTIO (2026-07-18)

Pull: spec §5 — may/must bitsets + forward fixpoint over the datorum
event streams. DEVIATION FROM SPEC WORDING (named): the fixpoint
runs SUCCESSOR-PUSH — a block whose exit changes pushes may|= /
must&= contributions along its own outgoing margines — so NO
predecessor index is needed in B (it defers to chunk C, whose
blame-edge inevitability walk actually consumes it). The
never-visited-preds meet rule becomes STRUCTURAL: unprocessed
(unreachable) blocks never push, so folded si(0) branches
contribute nothing (s04d parity, zero code).

Shape: FluxusDatorumBlocus grows four retained bitset fields
(may/must × introitus/exitus, i64 words, numerus_verborum on
FluxusDatorum); aedificare runs extraction THEN fixpoint in one
call (seat unchanged). Lattice: may init 0s (union), must init
all-ones = top (plain &= works from first contribution); introitus
seeds param bits both. Transfer: DEFINITIO/DEFINITIO_LOCI set the
bit in both; def-omnia (variabilis -1) sets ALL words; USUS inert
in B (judged by C's streaming re-walk). Monotone both directions,
worklist with in-queue flag, termination bounded.

Bars: unit probationes on block-boundary bits — if/else may-vs-must,
both-branches-must, pure-while postis may-only (s15a shape),
while+break caput must-uninit (s07b — THE blame precondition),
folded si(0) contribution invisible (s04d structural), do-while
must (s07c), param entry bits, def-omnia coverage via the ambiguous
sizeof source. Then suite + vectis + census (wall-clock = the Q8
meter again — fixpoint now runs corpus-wide) + amalgama + officina.

## FLUXUS GRADUS 1 — CHUNK B (PUNCTUM FIXUM): RELATIO (2026-07-18)

Shipped (ut supra in commissione): may/must bitsets (i64 words,
introitus+exitus per block, retained — query columns) + the
successor-push fixpoint exactly as the INTENTIO shaped it. The
deviation held: no predecessor index needed — unprocessed blocks
never push, so the folded-branch invisibility (s04d) and the
never-visited-preds meet rule are structural facts of the
algorithm, not checked conditions. Entry = param bits; def-omnia
fills all words; must starts at top (all-ones) so plain &= works
from first contribution.

BARS: 203/203 unit — the eight fixpoint pins all held first run,
notably s07b (while+break: x ABSENT from may at caput entry — the
exact must-uninit-crossing-the-edge precondition chunk C's blame
rule needs) and s04d (dead fill(&x) contributes nothing). Suite
32/32, VECTIS TENET, amalgama VERIFICATUM, officina 12/12, census
verdicts identical at 118.8s — the corpus-wide fixpoint is inside
measurement noise (Q8 holds through B).

NEXT: chunk C — s21 attribution-vocabulary specimen FIRST, then
predecessor index + streaming judgment re-walk + blame-edge rule +
codices 71 LECTIO_ININITIATA / 72 ININITIATA_QUANDOCUMQUE / 73
ININITIATA_FORSITAN (dormant) + specimen graduation to fixtures +
corpus first contact (pure wall expected).

## FLUXUS GRADUS 1 — CHUNK C (IUDICIUM): INTENTIO (2026-07-18)

Pull: spec §6 — codices 71 LECTIO_ININITIATA (definite, SUSPECTUM,
anchor at use), 72 ININITIATA_QUANDOCUMQUE (sometimes, SUSPECTUM,
anchor at blamed edge origo + socius = use), 73 ININITIATA_FORSITAN
(dormant behind _forsitan_vigil). DESIGN NOTE found in planning: the
_codices table carries ONE causa per codex — per-site "whenever X"
vocabulary is impossible in the message; attribution lives in the
ANCHOR (origo node position names the construct) + socius. s21 runs
first for the record (final-operand blame vocabulary) but no longer
gates wiring. Self-init keeps its event flag (query column) but
shares codex 71's causa — message-form parity deferred, named.

Shape: judgment in semantica (_initiationem_examinare after datorum
build): reachable blocks only, streaming may-state re-walk judging
USUS events (definite = bit absent from running may); sometimes =
may∖must + blame edge E=(P→S): (a) x absent from may_exitus(P), P
reachable; (b) U inevitable from S def-free (fixed point: I = {U-block}
grown backward via NEW pred index where all out-edges land in I and
block is def-free); E restricted to VERUS/FALSUS/ORDINARIUS-labeled
(clang's blamable forms; CASUS/LAPSUS/SALTUS → residual). One
diagnostic per use, first blamable edge in deterministic order.
Residual (may∖must, no blame edge) counted for 73; vigil FALSUM =
never fires; measurement = one documented vigil-flip census (62
pattern), expect small (lib/ baseline 3).

Bars: specimen battery as _codicem_probare e2e counts (the fast
wall); all 20 specimens graduate to fixa/examinis .suspectum with
ORACULUM -Wuninitialized + EXSPECTA-PROXIMA pins (silent ones
pin-less — verify the leg accepts zero pins); corpus first contact
census 71/72 = PURE WALL expected (any site = predicate bug or real
bug, both harvest); census wall-time meter continuity; suites +
vectis + amalgama + officina. TOLERA guards on 71/72 firings.

## FLUXUS GRADUS 1 — CHUNK C (IUDICIUM): RELATIO (2026-07-18)

Shipped (6223c07): the judgment. 71 LECTIO_ININITIATA (definite =
absent from running may; anchor at use), 72 ININITIATA_QUANDOCUMQUE
(may∖must + blamable edge; anchor at edge origo, socius = use), 73
ININITIATA_FORSITAN dormant behind _forsitan_vigil. The blame rule
shipped as specced — (a) must-uninit crossing the edge, (b) use
inevitable from the target (all-blocks fixed point; the predecessor
index died entirely, chunk B's deviation extended) — with ONE
calibration-forced amendment: clang fires PER BLAMABLE EDGE (s21b:
'if' false AND '&&' false on one line), so we emit per edge too.
s21 also pinned final-operand vocabulary = 'if' — the chunk-0
origo wiring was already clang's choice.

CORPUS FIRST CONTACT: 5322 sites in 262 files — NOT a wall, and the
whole cliff was TWO decay classes the specimens missed: (1) array
identifier in value position (sprintf(tabula,...)) = T[]->T* decay
= address-taken = LOCI, not a use; (2) member-array decay
(memset(r.c,...)) = address INTO the aggregate = LOCI of the whole
— answered by a NEW seam callback expressio_acies over typationes.
5322 -> 16 -> 2, both in fixa/roundtrip (out of judging domain).
STANDING METERS: 71 = 2-in-fixa, 72 = 0, both pure walls in
maintained code; census wall 118s unchanged (whole judgment
corpus-wide = free). 73 vigil-flip experiment: 87 residual sites
(78 knotapel matrix-init loops — the sigillum shape at scale, 6
lib/), all false-positive-flavored: dormancy vindicated.

TWO NET FINDS beyond the codices: the old purum test source read an
uninitialized whole struct (a = b) — clang flags it too (s22 pin);
and vectis' oracle-count grepped the flag substring over ALL clang
lines — -Wuninitialized is the first flag whose NOTES repeat its
name ("uninitialized use occurs here"), double-counting every
sometimes-warning; now warning:-lines only (both legs).

BARS: e2e battery 30 counted cases (all pins as counts, TOLERA
included), unit 203/203, silva 32/32, VECTIS TENET over the 12 new
.suspectum fixtures (specimina s01-s22 graduated whole — silences
ride pin-less, the count-equality contract holds them), amalgama
VERIFICATUM (hand-written silva.h enum extended — the clamat
worked), officina 12/12. TOLERA names registered (the _tolerabiles
table was a second registry the spec missed — caught by the battery).

ARC COMPLETE: chunks 0/A/B/C all shipped, 2026-07-17 evening to
2026-07-18, five commits (057d8ba, 580977e, 8d1bed4, 6223c07 +
c35eb4f pin fix). Codex 62 re-arm now UNBLOCKED: def-use substrate
exists (its named condition, decretum 01KXRBS0VQ).

## ADDENDUM 2026-07-20 — systema_posix.h Unda 2 (the hospitium pull)

The hospitium arc pulled the socket/signal/process surface through
the probationes and exposed the lexicon gap at ERROR tier for the
first time: TCP_NODELAY/IPPROTO_TCP as bare unexpanded macros =
"identificator ignotus" violatio (indistinguishable from a genuine
typo — the same day a real DXCIX typo fired the identical shape),
plus vocatio-implicita spam defeating the push's
silence-is-clean discipline on every probatio edit.

Unda 2 shipped in systema_posix.h: pid_t/ssize_t/socklen_t/
useconds_t, fork/pipe/read/write/close/_exit/usleep, kill/waitpid
+ SIGINT/SIGKILL/SIGTERM, getsockopt/setsockopt + SOL_SOCKET/
SO_REUSEADDR/SO_KEEPALIVE/SO_NOSIGPIPE/IPPROTO_TCP/TCP_NODELAY.
All values auspex_posix-certified (gate extended: real socket/
signal/unistd headers in TU verum, value+size asserts in the
shared table — both TUs CONSENTIT first run).

Hook (examen-custos): ./probationes/* now judged -posix, and for
that path the exclusiones.txt short-circuit is BYPASSED — being
posix-aware means those files can be judged honestly instead of
skipped, so real diagnostics still push (negative control pinned:
a true unknown-identifier violatio in a probationes file fires
through the hook under -posix). Differential after Unda 2:
probatio_tcp_servus/http/reactor = ACCIPE zero-diagnostic under
-posix; DEFAULT mode untouched (ISO baseline immota by design) —
probatio_tcp_servus stays REICE there (NODELAY macros), now
pinned in exclusiones.txt (41→42) for the corpus bar.

VECTIS TENET. Named residue: exclusiones graduation experiment
(would lib/tcp_posix.c etc. come clean under -posix judgment? —
could shrink the 42-file list to genuinely-un-judgeable ones);
censor-on-.m remains the other half of park 01KXZYG2SE.

## ANNOTATIONES STML — CHUNK A (COLLECTOR): INTENTIO (2026-07-21)

Pull: park 01KY3D7EJP chunk A (spec = project-specs/
annotationes-stml-spec.md §2/§4). The identity+intention layer's
collector: trivia walk recognizing annotation comments (first
non-whitespace content = STML open tag), decoration pre-pass,
stml_legere, attachment to top-level units. First paying consumer
of the "comments are content with structure" pin (VIGILA-HANC).

SEAT DECISION (Fran, 2026-07-21): stml vendored INTO the silva
amalgam — the pre-amalgam-arc cost argument died with the arc
(fontes lists aedilis-derived, excludenda compiler-harvested;
remaining hand = ~12 rename entries + silva.h reading subset +
2 policy lines). internamentum rides along (stml interns all
names). The collector CALLS stml_legere itself and returns
finished records. Public-renamed-vendored-type precedent:
SilvaPiscina/SilvaChorda/SilvaXar.

Shape: fontes/silva_annotationes.{h,c}. API:
silva_annotationes_colligere(piscina, parsura, intern /*NIHIL
licet*/) -> Xar of SilvaAnnotatio {textus (cleaned), crudum,
fons_index/linea/columna/byte_offset, modus
SUPRA|INTERIOR|PLAGULA, unitas, parsata, documentum/arbor,
status + positioned error}. Walk = TOLERA pattern (lexemata +
radix chain — the invocation-trivia lesson pinned at birth;
dedup by (fons, byte_offset)). Attachment BYTE-based per fons
(silva_nodus_extensionem, radix-resolved): SUPRA = nearest unit
minimum > offset; INTERIOR = containment (beyond-ceremony
addition, Fran blessed: enclosing unit, keeps interior
annotations lintable); PLAGULA = none. Malformed anchored =
collected with parsata FALSUM + positions — never silently
prose, structurally (everything anchored is IN the output).

Bars: probatio_silva_annotationes with 8 pinned cases (anchored /
prose / malformed / multi-line-decorated / stacked / file-scope /
INTERIOR / macro-radix) + boolean-nid attribute; suite green;
amalgamare VERIFICATUM (stml+internamentum vendored, harvest
re-run, silva.h reading subset + hospes calls per HOSPES RULE);
censor + examen clean.

Wrinkles recorded (not solved): end-of-line annotation binds to
the NEXT unit (above-target read literally); malformed source
line = annotatio.linea + linea_erroris - 1, columns shift under
decoration strip (chunk C refinement if needed).

## ANNOTATIONES STML — CHUNK A (COLLECTOR): RELATIO (2026-07-21)

Shipped, all bars green: fontes/silva_annotationes.{h,c} (collector
+ silva_annotationes_unitates public), probatio_silva_annotationes
63/63 over 9 sections (anchored / prose / malformed-never-silent /
multi-line-decorated+boolean-nid / stacked / file-scope / INTERIOR
/ macro-radix dedup / empty comments), silva suite 33/33, amalgam
VERIFICATUM (standalone + hospes 33/33 incl. new ADDITIONES block +
nm 0 + censura). stml + internamentum vendored into silva.c (~2.15
MB); selectio excluded wholesale at policy with via retro; silva.h
+ ~130 lines (renamed stml reading subset + annotation API), all
type-checked by the embedded-caput gate.

THE BUG: silva_nodus_extensionem is a CALLER-INITIALIZED
accumulator; both new call sites passed uninitialized outs. The
probatio's stack luck produced plausible extents ([0,94]); the
module's produced a split unit POINTER (min=1/max=0x50082E80 —
the decode that solved it). Every SUPRA pin initially passed BY
ACCIDENT. hospes had the contract right since LEGATUS chunk 0.
Codex-71-shaped, cross-call-boundary — the annotation type layer's
own advertisement. Detail: fontes/silva_annotationes.worklog.md.

TREE FACT: commissio radix = SILVA_VALOR_LISTA of sentence values
(not a node); unitates() accepts both shapes.

HARVEST HARDENING (tools/aedilis.worklog.md): definition-first
classifier, PROTECTA guard (non_statica inexcludable, refusal names
the seed), unneeded-internal-declaration as second signal-A class,
gyrus cap X→XXX, per-gyrus station snapshots. Cascade seed = stale
SERVANDA_FRIATIONIS (fnv1a_literis now live via stml). Fixpoint 6
gyri / 147 nomina. Plus tools/silva_fontes_generare.sh (12 launcher
snippets: live glob + persisted principalia — new-module-nobody-
includes broke principal-closure derivation).

INSTRUMENTA DEBRIEF: adhibita = examen push (ACCIPE first compile),
nexus -renovare, S41 guard (caught _est_littera live), excubitor
(STALA 2 post-regen), corpus (stml_legere read), aedilis --enumerare
(via silva_fontes_generare). fructus = the S41 + PROTECTA refusals
each pointed within one hop of the true cause. asperitates = (1)
extensionem's caller-init contract is a silent footgun — two fresh
call sites missed it in one day while hospes had it right; (2) a
failed amalgamare leaves broken committed silva.c → aedilis
bootstrap deadlock (loop-break: git checkout -- silva.c). desiderata
= filed on the board.

WRINKLES CARRIED (named in INTENTIO, unchanged): end-of-line
annotation binds to NEXT unit; malformed columns shift under
decoration strip (chunk C may refine).

NEXT: Chunk B — identitates.tsv (nexus.tsv sibling) + minting
instrument (fills boolean nids) + bench verb (##prefix → corpus).

## ANNOTATIONES STML — CHUNK B (INDEX + MINT): INTENTIO (2026-07-21)

Pull: park 01KY3D7EJP chunk B. Three deliverables: (1)
identitates.tsv — nexus.tsv sibling in root build/ (GENERATUM
header, tab columns: nid / via / linea / modus / elementum /
unitas-genus / unitas-linea), rebuilt by a walk; (2) the minting
instrument — EXPLICIT run per ceremony Q8, dry-run by DEFAULT,
--scribere applies, verify-after-write refuses loud; (3) the bench
verb: ##praefixum (>=6, unambiguous) -> resolved row + unit source
slice.

ONE MINT: scrinium_ulid() (lib/scrinium.c, PUBLIC, monotonic
within ms) — the tabularium's own generator; code and board draw
identities from the same well. aedilis derives scrinium into the
tool's fontes automatically.

Shape: fontes-side pure additions (probatio-testable):
silva_annotationes_identitates(piscina, annotatio) -> Xar of
SilvaIdentitas {elementum, valor (vacua = petitio), petitio,
insertio_offset fons-absolute, insertio_genus POST_ATTRIBUTUM |
POST_TITULUM}. Identity READ side = tree walk (document order:
<nid v="U"/>; nid="U" on any element); petitio side = boolean nid
/ boolean v on nid-element / bare <nid/>. Splice OFFSETS = textual
scan over the RAW comment bytes (in-tag word scan, quote-state
tracked, decoration skipped inline, tag-name context for the v
rule) — cleaned-text offsets don't map to fons, raw ones do.
Correspondence tree-petitio-count == textual-count or the record
carries -1 and the instrument REFUSES. Locator only runs on
parsata annotations (anchored non-parsata = lint's business, never
mint fodder).

Instrument: instrumenta/principalia/identitates.c (--renovare /
--solvere / --mintare [--scribere]); parse recipe = examen's
capita-praebere walk + parsare_cum_contextu, NO lexicon/semantica
(collection, not judgment). Launcher silva/identitates.sh: file
enumeration + cheap textual prefilter (candidate comments only —
on-demand adoption means few annotated files for a long time);
snippet identitates_fontes_generata.sh joins the
silva_fontes_generare.sh family. Mint apply = splices in
DESCENDING offset order, write temp + rename, then RE-COLLECT and
verify (petitio count -> 0, identity count grown, length delta ==
sum of inserta) — mismatch = original untouched + loud refusal.

Bars: probatio sections X (extraction: valued/boolean/standalone/
bare/v-boolean + prose-"nid"-in-text + attribute-value red
herring) + XI (splice offsets pinned exactly, incl. multi-line
decorated tag); suite green; amalgamare VERIFICATUM (fontes grew);
mint SELF-GATE on committed fixture (fixa/annotationes/mintanda.c
copied into build/, minted, verified); censor clean. silva.h
UNTOUCHED this chunk (house-instrumenta consumers only — no
HOSPES duty).

## ANNOTATIONES STML — CHUNK B (INDEX + MINT): RELATIO (2026-07-21)

Shipped, all bars green: silva_annotationes_identitates (tree-read
+ raw-byte splice locator, order-zipped with count+genus
correspondence, dissent -> -1 -> instrument refuses),
probatio_silva_annotationes 100/100 (sections X extraction + XI
splice offsets, all byte-check asserted); lib/moneta.c (ULID
extracted from scrinium - scrinium vendors sqlite, 250k lines for
26 characters was absurd; scrinium_ulid delegates; ONE mint for
board and code); instrumenta/principalia/identitates.c +
silva/identitates.sh (--renovare -> build/identitates.tsv with
byte extents so --solvere slices source with NO reparse; -mintare
dry-default, --scribere = temp + re-collect verify + rename;
-porta gate = fixture mint -> value-strip -> byte-identical +
re-dry empty). Suite 33/33, amalgamare VERIFICATUM, VECTIS TENET
(exclusiones +2: moneta.c POSIX lane, identitates.c dirent lane —
beside their siblings).

FIRST REAL IDENTITY: 01KY3TRZ2GY3HG9Y4WQRSGR2BX — the collector's
own intentio (res="01KY3D7EJP"), minted by its own instrument,
resolved by ./silva/identitates.sh '##01KY3TRZ' to annotation +
byte-exact unit source. Dogfood at birth; the porta run showed the
mint's monotonicity (...DQES/...DQET consecutive).

INSTRUMENTA DEBRIEF: adhibita = examen push (caught the i32/s32
splice arithmetic at edit time), aedilis derivation (moneta flowed
into scrinium consumers' lists via one generator run each),
silva_fontes_generare (new launcher snippet bootstrapped with a
"# principalia:" line and joined the family), -porta gate at birth
per doctrine. asperitates = (1) hook diagnostics for dirent-walk
principals fire before the exclusiones entry lands — expected
noise on every new principal, exclusiones is the cure; (2) the
first-mint adoption decision (annotating real source) is a
judgment call the instrument can't make — stayed with one tasteful
dogfood identity. NEXT: Chunk C — examen codices
(well-formedness / uniqueness / bidirectional rot), verdict-
neutral at birth.

## ANNOTATIONES STML — CHUNK C (CODICES EXAMINIS): INTENTIO (2026-07-21)

Pull: park 01KY3D7EJP chunk C. Three codices, ALL DOMESTICUM
(verdict-neutral at birth per ceremony; graduation by decretum):
- 74 ANNOTATIO_MALFORMATA: anchored comment that does not parse -
  the never-silently-prose rule gets its lint half; position =
  annotation line + linea_erroris - 1 (the pre-pass preserves
  lines), fallback comment start when stml gives no position.
- 75 NID_DUPLICATUM: same nid twice in ONE file (fires at the
  later occurrence). CROSS-FILE duplicates belong to the INDEX -
  identitates --renovare gains a seen-set + CAUTIO (the examen
  judges one file; the index owns the global truth).
- 76 IDENTITAS_INVALIDA: nid values must be EXACTLY 26 Crockford
  chars (no I/L/O/U); res= citations must be >= 6 valid chars
  (format only - RESOLUTION of citations is chunk D's, where the
  board resolver lives; the spec itself seats it there).

Seat A dividend: semantica calls the collector directly (both in
fontes) - the seam chunk C would have needed under seat C never
gets built. Emitter: annotations are TRIVIA (no node) - a raw-
position diagnostic variant (nodus NIHIL licet; MACRO_DOMESTICUM
manual-position precedent), via from silva_fons_via. Petitiones
are NOT judged (minting = the instrument's business, not a
defect). Principal-fons annotations only (provided headers get
their own judgment runs).

Bars: battery e2e counts in probatio_silva_examen (74 fires/
prose-never; 75 second-occurrence; 76 length/alphabet/res-prefix
+ valid-silent); enum+table+static-assert+silva.h extended (ABI:
tail append only); amalgamare VERIFICATUM; suites + VECTIS TENET;
corpus contact = the one real intentio stays clean.

## ANNOTATIONES STML — CHUNK C (CODICES EXAMINIS): RELATIO (2026-07-21)

Shipped: codices 74 ANNOTATIO_MALFORMATA / 75 NID_DUPLICATUM /
76 IDENTITAS_INVALIDA, all DOMESTICUM (verdict-neutral;
gradatio per decretum). _annotationes_examinare in semantica
(hooked beside TOLERA irritum pass; collector called DIRECTLY -
the seat-A dividend, no seam exists because none was needed);
raw-position diagnostic variant for trivia (nodus NIHIL,
MACRO_DOMESTICUM precedent); petitiones never judged; principal
fons only. Cross-file duplicates = identitates --renovare CAUTIO
(seen-set; summary line counts them). res= = format-only (>= VI
Crockford); RESOLUTION deferred to chunk D by the spec's own
seating. Battery 9 cases (fires + silences) 1294/1294; suite
33/33; amalgamare VERIFICATUM (enum tail-appended in silva.h -
the clamat gate held); VECTIS TENET; corpus contact clean (the
real intentio judges ACCIPE zero-diagnostic). The examen-custos
hook now pushes annotation defects on every .c/.h edit for free.

NEXT: Chunk D - TABULA: ancora genus "nid" + resolver against
identitates.tsv; board CAUTIO-si-inresolutae upgrades to true
resolution; res= citations resolve against the board (the other
half of bidirectional rot).

## ANNOTATIONES STML — CHUNK D (TABULA): INTENTIO (2026-07-21)

Pull: park 01KY3D7EJP chunk D — the board learns the identity
layer. Cross-project (silva instrument + gesta resident); arc
narrative stays HERE, pointer in gesta/phase-log.

(1) SILVA: --renovare emits a SECOND tsv build/citationes.tsv
(res-praefixum \t via \t linea, one row per res= citation found
in annotation trees) — the code->board direction's raw material.
(2) TABULARIUM: ancora genus "nid" ({genus, scopus=praefixum})
resolved against build/identitates.tsv via a lazy index (exact
twin of the nexus.tsv pattern; cfg via_identitatum/via_citationum
+ principale defaults); resolution -> "via:linea (genus
unitatis)"; absence -> CAUTIO that MEANS something (annotation
deleted or index stale — named in the message). (3) REVERSE
LINKS: res output gains "citata ex codice" lines (citationes rows
whose praefixum prefixes THIS res_id). (4) CENSUS: citation
health — resolved/unresolved counts + unresolved listed (the
code->board rot signal). Doctrine string: genus nid documented.

Bars: probatio_tabularium new section (nid anchor resolved +
unresolved-CAUTIO via fixture identitates specimen; reverse link
+ census via runtime-written citationes tsv embedding the real
res_id — static fixtures cannot know minted ULIDs); gesta suite
green; silva launcher passes both tsv paths; -porta/-renovare
green; live demo = nid anchor written onto park 01KY3D7EJP
(old resident renders it unresolved-harmless; TRUE resolution
after Fran's /mcp reconnect against the rebuilt binary).

## ANNOTATIONES STML — CHUNK D (TABULA): RELATIO (2026-07-22)

Shipped: citationes.tsv lane in --renovare (res= citations, one
row each); tabularium ancora genus "nid" (lazy identitates.tsv
index, nexus-twin; resolutio "via:linea (genus)"; CAUTIO quae
significat + mandatum renovationis; praefixum ambiguum = CAUTIO
sua); nexus reversi "citata ex codice" in lectione rei; salus
citationum in censu (inresolutae nominatae). Lectores
cache-in-successu (index post ortum apparens visibilis fit —
idemque probationem fixtura runtime-scripta possibilem fecit).
Gesta suite 220/220; launcher fontes regenerata (moneta ad
lineam nexus residentis pervenit). DEMO VIVUM POSITUM: parcum
01KY3D7EJP ancoram nid 01KY3TRZ... fert (residens vetus inertem
reddit; /mcp reconnect resolutionem veram + nexum reversum
monstrabit — anulus putredinis in historia sua ipsius clauditur).

NEXT: Chunk E — MIGRATIO familiarum veterum (aedilis verbs,
TOLERA, EXSPECTA pinnae: lectio duplex → auditor → retiratio,
chorea aedilis-arcus), Fran's pull; post-ladder items named in
spec §4.

## CONTRACTUS (desideratum 01KY3JWF): INTENTIO (2026-07-22)

Pull: the accumulator-contract family - the first SEMANTIC
annotation vocabulary, built the same day its motivating bug bit
(extensionem caller-init, chunk A). Direction recorded on the
desideratum: annotation = contract carrier, inference = auditor.

VOCABULARY (decretum-lite, Fran delegated): <contractus
param="NOMEN" modus="accumulat"/> - one element per param,
stackable; SITS ON THE PROTOTYPE IN THE HEADER (capita praebita
=> every consumer TU sees the contract in its own parse - no
index, no interprocedural plumbing; the contract lives where C
puts interface truth). accumulat = caller initializes (callee
reads pointee before writing). scribit/legit = reserved names.
Unknown modus fires (vocabulary rot never silent).

CODICES 77-79, all DOMESTICUM at birth:
- 77 CONTRACTUS_VIOLATUS: &local passed to accumulat param, local
  declared WITHOUT initializer, ZERO intervening uses
  (declaration->call straight-line). V1 = SYNTACTIC BY DESIGN -
  today's bug shape exactly, near-zero FP; full FLUXUS
  must-uninit integration = NAMED v2 (same causa, tighter
  detector).
- 78 CONTRACTUS_STALUS: declared accumulat but body never READS
  the pointee (not an accumulator any more), or modus ignotus.
  Provable contradictions only.
- 79 CONTRACTUS_ABSENS: in-out scalar-pointer param (pointee read
  AND written, no -> uses, non-const) sine contractu - advisory;
  corpus contact decides awake vs vigil-dormant (73 precedent).

Contract TABLE built during the annotation pass from ALL fontes
(the 74-76 pass filters principal; contracts must NOT - headers
carry them). Function name from the annotated unit via the
declarationes vista machinery.

Bars: battery cases incl. a VERBATIM reproduction of this
morning's bug firing 77; dogfood = silva_nodus.h extensionem
prototypes annotated (first real contracts); corpus contact
census 77-79; suites + amalgamare + VECTIS; enum/table/silva.h
tail-append.

## CONTRACTUS: RELATIO (2026-07-22)

Shipped, all bars green — and BETTER than the INTENTIO: codex 77
landed with REAL DATAFLOW (not syntactic v1): twin seam
parametrum_accumulat beside parametrum_constans in the datorum
extractor (callee+index already in hand), new event
LOCI_ACCUMULAT, judged in the codex-71 streaming walk against
running may-state BEFORE bits. 78 = provable contradictions
(never-reads / param ignotum / modus ignotus). 79 = census first
contact 147 sedes/64 plagulae (ALL true in-out shapes) -> DORMIT
behind vigil (73 pattern), census-only. Comma-token trap in the
parametra lista fixed (ordinal counts parametrum nodes only —
the extractor's own arg walk knew; mine learned). Dogfood:
silva_nodus.h extensionem prototypes carry the first real
contracts; RESURRECTION TEST = the morning's bug verbatim fires
77 x2 at the exact &-columns. Battery 1345/1345, suite 33/33,
amalgamare VERIFICATUM (clamat caught the forgotten silva.h enum
— the gate earns its keep twice in one day), VECTIS TENET.
Desideratum 01KY3JWF IMPLETUM; futures: wrapper API, scribit/
legit modi, wrapper-propagation, 79 wake.

## CHUNK E MIGRATIO: INTENTIO (2026-07-22)

Familiae veteres in grammaticam unam (spec par 4 E; parcum
01KY3D7EJP; Fran formas benedixit 2026-07-22):

- E1 EXSPECTA: `<exspecta codex="X"/>` (proxima ordinaria) /
  `<exspecta linea="13" codex="X"/>` (absoluta). Resolutor novus =
  verbum --exspecta in identitates.c (linea_resoluta + codex per
  plagulam emissa; ambulatio arboris, non awk). Vectis awk
  _pinnae_solvere retiratur. Populatio: 66 proxima + 1 absoluta
  in fixturis.
- E2 TOLERA: `<tolera codex="X" (>causa` (captura lineae pro
  causa). Passus tolerarum legit annotationes IAM COLLECTAS
  (semantica collectorem iam vocat - sutura chunk C); parser
  octetorum _tolera_legere ET ambulatio propria retirantur;
  tabula (linea, fons, codex) forma eadem. Populatio: 24 vivae +
  corpus fixturarum + battery.
- E3 AEDILIS: `<aedilis corpus="via"/>` / `<aedilis
  obiectum="via"/>` (verbum-ut-attributum). Extractor in
  tools/aedilis.c viam alteram discit (stml iam intra limitem
  puritatis - configuratio); machina INTACTA (chordas verbi
  easdem accipit). Populatio: 10 sedes.

CHOREA COMPRESSA (non trans sessiones): captura fundamenti
(fructus parseris veteris) -> via nova docta -> sedes rescriptae
-> fructus novus = fundamentum OCTETO-IDENTICUS -> parser vetus
retiratur. Familia una = commissum unum; post quamvis familiam
status purus. Portae: suites plenae + VECTIS TENET + (E3)
manifesta identica + amalgamare VERIFICATUM.

LUCRUM TUTELAE: mendum in EXSPECTA/TOLERA hodie = prosa TACITA;
in grammatica nova = ancoratum malformatum -> codex 74 CLAMAT.

CAVENDA: corpus invalidum = C89 vitiosum consulto - collector
super lexemata ambulat (parsura mala non obstat) sed CONTACTUS
CORPORIS ante omnia: verbum novum in fixtura invalida gnarissima
probare primum. Porta codicis exitus (lex): fructus in plagulas,
diff, $? - numquam grep in fistula.

## CHUNK E1 EXSPECTA: RELATIO (2026-07-22)

Navatum, portae omnes: pinnae 67/67 OCTETO-IDENTICAE (fundamentum
awk contra instrumentum novum, corpore toto), VECTIS TENET
resolutore novo vivo, probatio annotationum + casus resurrectionis,
suite silvae 33/33, amalgamare VERIFICATUM, radix 103/103.

- verbum --exspecta in identitates.c (+ -exspecta / -struere in
  launcher); awk _pinnae_solvere MORTUUM; recusatio pinnae in
  vectis fracturam ducit (awk numquam recusare poterat)
- 67 pinnae / 39 plagulae conversae: <exspecta codex="X"/> +
  <exspecta linea="1" codex="X"/> (absoluta una)
- INVENTUM MAIUS gratia portae: hiatus collectoris — annotatio
  supra #include/#define TACITE vorabatur (trivia in lexemate
  directivae consumptae equitant, fluxus expansus eas numquam
  videt). Sanatum: ambulatio parsura->directivae in colligere.
  Collocatio capitis plagulae = naturalissima; hiatus adoptionem
  momorderat. Casus resurrectionis = sectio X probationis.
- laqueus obiecti veteris in identitates.sh sanatus (nexus etiam
  cum obiectum binarium anteit)
- pinna una intra commentarium prosae latebat (ordo_pravus) - awk
  lineis legebat, grammatica commentariis ancorat; pinna nunc
  commentarium proprium (grammatica mores rectos docet)

RELIQUUM: E2 tolera, E3 aedilis.

## CHUNK E2 TOLERA: RELATIO (2026-07-22)

Navatum: `<tolera codex="X" (>causa`; parser octetorum
_tolera_legere ET ambulatio propria _toleras_ex_lexemate MORTUI -
tabula ex annotationibus collectis cribratur. LUCRUM STRUCTURALE:
ambulatio annotationum UNA communis (_annotationes_pigre, cache
per parsuram) tres consumptores pascit (74-76, tolerae,
contractus) - collector antea bis per parsuram currebat. Portae:
paritas octetorum 8/8 plagularum TOLERA-ferentium (3 vivae + 5
fixturae severae), batteria, suite 33/33, VECTIS TENET,
amalgamare VERIFICATUM, radix 103/103. Sedes conversae: 6 vivae +
5 fixturae + 16 chordae batteriae. Typographum TOLERA nunc
codex 74 CLAMAT (olim prosa tacita + diagnosticum severum
inexplicatum redibat).

RELIQUUM: E3 aedilis.

## CHUNK E3 AEDILIS: RELATIO (2026-07-22) - SCALA E COMPLETA

Navatum: <aedilis corpus=/obiectum=/vexillum=""/> (verbum-ut-
attributum); machina pura INTACTA (extractor chordas verbi easdem
retexit ex arbore per silva_stml_* amalgamatis); extractores
fixturarum intacti. TOMBSTONE primum depositum: forma vetus aut
<aedilis malformatum -> recusatio clamosa cura nominata - et
INTRA MINUTA tres stragulos vivos cepit (probationes speculum
x2 + vitrea_hospes) quos percursus lib/include omiserat; sub
parseribus tacitis corpora e clausuris tacite evanuissent.
Portae: paritas derivationum 4/4 octeto-identica, PORTA AEDILIS
119/119 OMNES BONAE, radix 103/103.

SCALA E COMPLETA: tres familiae veteres (exspecta/tolera/aedilis)
in grammaticam unam uno die migratae, parseres tres mortui,
chorea compressa (fundamentum -> via nova -> sedes -> identitas
-> retiratio) ter sine iactura tenuit.

## QUAESTIO QC PSEUDO-CLASSES: INTENTIO (2026-07-22)

Rotula interrogationis (addendum 2026-07-17) Frani reddita, tria
decreta: (1) familia normae :c89/:c99/:extensio DILATA NOMINE
(parcum 01KY47F797 - populatio tags-normae = tractus proprius;
pseudo ignotum in QC = fractura compilationis clara); (2)
:vocat(f) = semantica CONTINET (barra v1 probat:
"function-definition:calls(compute)" = functiones vocationem
CONTINENTES; QD translatio mechanica manet); (3) registratio
pseudo-classium usoris FUNCTIONALIS in QC (tabula dispatch hic
nascitur - sutura una; barra v1 API probat, QD eam postulat).

SCOPUS: parser + compilare + exsequi pro PARS_PSEUDO - sedes IR
iam ex QB exsistit ("IR eas fert, parser nondum"). Menu (consilium
DECISUS 2026-07-06, silva-quaestio-design.md):
- structurales: :primus / :ultimus (filius nodalis primus/ultimus)
- logicales: :habet(sel) / :non(sel) - compilare RECURSIVUM
  (monitum machinae: selector internus = quaestio compilata nidificata)
- semantici (paritas barrae v1, Latine): :vocat(f) CONTINET /
  :definit(x) / :utitur(x) / :reddit - praedicata super
  vocatio/declarator/folium tok_valor
- silvani: :ambiguum, :lectiones (vexillum AMBULATIONIS - descensum
  canonicum-solum laxat, contractus capitis id iam nominat),
  :sumptus / :omissus (textura conditionalium ramorum)
- registratio usoris (v1 insectum #4 functionale factum)

BARRAE: asserta QA+QB 86 viridia manent; probationes novae per
pseudo (congruentia + captura + compositum cum attributis);
contactus corporis in arbore vera ante pinnas; suite + amalgamare
+ VECTIS. LEX SUTURAE: exsequi/congruit ambulationem et compilare
LEGERE ante aestimationem :habet recursionis et vexilli
:lectiones - numquam ex summario.

POST: QD barra v1 translata (100 asserta) -> parcum 01KXPV9FPK
(quaestio in legati) -> contractus selector-scopati (01KXTX7FA3,
porta nunc sola QC).

## QUAESTIO QC: ADDENDUM SUTURAE (2026-07-22)

Seam read done per the INTENTIO's lex suturae: compilare/congruit/
exsequi read whole (silva_quaestio.c + header), plus the design doc
DECISUS section, v1's registration wiring (lib/arbor_quaestio.c),
and the grammar's genus vocabulary. Four decisions, Fran blessed:

1. REGISTRATION BEFORE COMPILE. v1's shape (registrare onto the
   already-compiled query) is INCOMPATIBLE with the fractura-clara
   decretum: compile must judge ":meus" known-or-unknown at parse
   time, so the registry must exist first. v1 accepted unknowns as
   CUSTOM with functio NIHIL -> silent zero-result queries (typo'd
   :cals(x) compiled fine); its bar never called registrare once
   (bug #4 receipt) - QD owes no registration test, QC's bar adds
   the first. Shape: opaque SilvaQuaestioPseudoRegistrum +
   silva_quaestio_registrare(registrum, titulus, functio, datum) +
   a compile variant taking the registrum (existing 4-arg compilare
   = sine-registro convenience). ONE name table: builtins resolve
   through the same table user entries extend (tabula dispatch,
   sutura una). User functio gets the arg: (nodus, arg chorda,
   datum) -> b32 - v1 parsed :custom(arg) then dropped the arg.

2. :habet CEILING. _catenae_congruit climbs ancestors unboundedly;
   CSS relative-selector semantics confine the nested chain to the
   subject's subtree. One limes param threaded through the existing
   recursion (~2 checks). Predictable counts.

3. :primus/:ultimus OFF-LIST: a node in no containing lista (direct
   NODUS locus, or root) = only child = BOTH match (CSS parity:
   root matches :first-child). _listam_continentem NIHIL = only.

4. SEMANTIC NAMES PER AMBIGUUS PRECEDENT. vocatio@functio,
   folium-identificator@tok_valor, declarator-titulus@tok_titulus,
   redde, ramus-sumptus, ramus-omissus - resolved by name from the
   registrum at compile (exemplar commissionis, like genus_ambigui);
   absent in a grammar (sceletum has no vocatio) = licet, predicate
   never matches. Grammar-agnostic law holds - no c89 caput enters
   the module.

NAMED LIMITS: :vocat(f) matches direct-identifier callees only
((*fp)(x) will not match - named in the header comment); pseudo
args share QB's escape-less quoted-value rule.

MECHANISM (seam-priced): one ':' branch in the compile loop
(_tag_legere for the name, quote-aware balanced-paren arg slice;
:habet/:non arg NUL-terminated into piscina -> recursive
silva_quaestio_compilare, union inside :non comes free); one casus
in _composito_congruit dispatching on pseudo genus; ONE boolean
early-exit subtree walker (canonical-descent-aware) serves :habet
+ all four semantic pseudos (:reddit = contains-redde etc.); :non
= congruit negated, no walker; :lectiones = b32 on SilvaQuaestio
set at compile, flips the ambiguus branch in _nodum_visitare + the
walker; :sumptus/:omissus = pater climb vs two cached genus
indices; :ambiguum = compare vs cached genus_ambigui. IR: pars
grows {pseudo_genus, pseudo_arg, pseudo_quaestio, pseudo_functio,
pseudo_datum} (memset-covered); SilvaQuaestio grows ~7 cached
indices + the lectiones flag. Fact worth keeping: ramus-omissus
has NO node children (cruda lista-token) - :omissus can only match
the arm node itself, structurally honoring the design doc's
quaestio-v1 conditional stance.

PUBLIC SURFACE: registry API + compile variant -> silva.h + hospes
same-change.

BUILD ORDER: IR + parser branch -> walker + dispatch -> registry
-> probationes per pseudo as built (congruentia + captura +
compositum cum attributis). Bars unchanged: QA+QB 86 asserta green
throughout, suite, amalgamare VERIFICATUM, VECTIS TENET.

## QUAESTIO QC PSEUDO-CLASSES: RELATIO (2026-07-22)

Navatum: pseudo-classes COMPLETAE in una sessione - IR + parser +
dispatch + ambulator + registro + barra, portae omnes virides
primo cursu. Superficies: :primus/:ultimus, :habet(sel)/:non(sel)
(compilatio recursiva, custos XVI), :vocat/:definit/:utitur/
:reddit, :ambiguum/:lectiones/:sumptus/:omissus, registratio
usoris (insectum v1 #4 mortuum - probatio registrationis PRIMA
utriusque generationis).

Portae: barra quaestionis 86 -> 160/160 (74 asserta QC nova,
NULLA rubra primo cursu - aestimationes suturae omnes tenuerunt:
fixtura conditionalium, :lectiones lectionem alteram aperiens,
limes CSS, numerator registrationis exactus); suite 33/33;
amalgamare VERIFICATUM (hospes 33/33, nm-intersectio 0); VECTIS
TENET.

Inventa aedificationis:
- radix = LISTA nuda (silva_parsare.c:244): nodi summi patre
  carent -> :primus/:ultimus AMBO congruunt in OMNIBUS nodis
  summis (extensio decreti filii-soli; PINNATUM in barra cum
  commento - quaestiones utiles sub patre scopandae, e.g.
  "corpus > declaratio:primus")
- ambulator subarboris UNUS (_liberos_quaerere/_valorem_quaerere)
  sex pseudos servit; :habet intra eum = _congruit_interna cum
  limite (param limes per _catenae_congruit totam filatus)
- :lectiones intra :habet: vexillum nidificatum QUOQUE laxat
  (:habet(declaratio:lectiones) = "in lectione ULLA")
- "registro" ablativus ut titulus parametri - "registrum"
  nominativus macro latina est (register); idem laqueus iam in
  silva_commissio.h notatus erat
- CADENDA_TYPEDEF amalgamatoris crevit: PseudoRegistrum +
  PseudoFunctio (praecedens functionum: SilvaResolutor)

Instrumenta (debrief): adhibita = corpus/inclusiones non necessaria
(sutura iam lecta), examen-custos per omnes editiones tacitus
(nulla fractura), suite-filter + amalgamare + vectis = fluxus
totus. Fructus: aestimatio suturae -> nulla retractatio. Asperitas:
nulla nova. Desideratum: nullum.

POST: QD barra v1 translata (100 asserta) -> parcum 01KXPV9FPK
(quaestio in legati) -> contractus selector-scopati (01KXTX7FA3).

## QUAESTIO QD MESSIS BARRAE V1: INTENTIO (2026-07-22)

Barra v1 (probationes/probatio_arbor_quaestio.c, ~100 asserta,
tags Anglica) in Latinum contra genera DECISUS translata -> plagula
nova silva/probationes/probatio_silva_quaestio_messis.c. Tabula:
function-definition=definitio-functionis, identifier=declarator-
titulus (nomina declarata) AUT folium-identificator (usus) - v1
utrumque uno tag tegebat, if/for/while=si/per/dum, struct/enum=
structura/enumeratio, compound=corpus, call=vocatio, integer-
literal=folium-integer, :returns/:calls/:defines/:uses=:reddit/
:vocat/:definit/:utitur. DELTA RECORDANDA: (1) tag ignotum v1
tacite vacuum -> fractura clara (iam DECISUS, hic pinnatum);
(2) translation-unit nodum non habet (radix LISTA nuda) - congruit
in nodis veris probatur; (3) utilitates introspectionis (pseudo_
nomen/combinator_nomen) non portatae - nomina in superficie ipsa
vivunt; (4) capturae v1 tabula dispersa -> Xar gradus omnes;
(5) asserta v1 mollia (">= I", ":calls sine assertione") EXACTA
facta ubi arbor silvae numerabilis. Post messem virentem: trias
v1 (lib/arbor_quaestio.c + include/.h + probatio) DELETA per
legem culturae (suite deletur post messem ad lapidem suum).
Barrae: messis viret, suite silvae tota, radix post deletionem.

## QUAESTIO QD MESSIS: RELATIO (2026-07-22) - SCALA QA-QD COMPLETA

Navatum: barra v1 tota translata -> probatio_silva_quaestio_messis
.c, 103 asserta (v1 ~100), VIRIDIS PRIMO CURSU - aestimationes
numerorum arboris omnes tenuerunt (declaratio VI, binarium IV,
folium-integer V, corpus VI in fixtura amplissima). Deltae quinque
in capite plagulae recordatae (tag ignotum fractura; translation-
unit absens - radix LISTA; utilitates introspectionis non portatae;
identifier -> declarator-titulus/folium-identificator distincti;
asserta mollia exacta facta - :vocat(compute) v1 sine assertione,
nunc EXACTUM I). Trias v1 DELETA (lib/arbor_quaestio.c 1517 +
include/arbor_quaestio.h 198 + probatio 842 lineae; dependentes
nulli - grep purus). Portae: messis 103/103; suite silvae 34/34;
radix 102 praeteritae 0 fractae (103 - probatio deleta).

Instrumenta (debrief): adhibita = grep clausurae dependentium ante
deletionem (purus), tabulae generum pro tabula translationis;
fructus = numeri exacti primo cursu iterum (bis in die - QC 74,
QD 103); asperitates = compile_tests.sh compendium finale nullum
imprimit (numeravi per grep TEST PASSED - desideratum minus:
linea summae ad finem); desiderata = nulla alia.

SCALA QUAESTIONIS COMPLETA: QA (tags/combinatores/unio) + QB
(attributa/capturae/fratres) + QC (pseudo + registratio) + QD
(messis barrae v1, trias deleta). Sequentia: parcum 01KXPV9FPK
(quaestio in legati MCP) -> contractus selector-scopati
(01KXTX7FA3, porta sola QC nunc aperta).

## SELECTA (quaestio in subsellio): INTENTIO (2026-07-22)

Parcum 01KY53MP5X tractum (postulatio 2026-07-11 harvestata: grep
structura-formatum + ancorae Edit; receptae tres - hiatus accessus-
membrorum M3, interrogatio Franis, QB=3 in officina). Instrumentum:
`./silva/selecta.sh <plagula> '<selector>' [-intra <symbolum>]
[-omnia] [-proba]` - congruentia octetim exacta cum capite
[via:linea-linea] (impressor extentuum emitte REUSUS, variante
const-tuta: series locis subiecti seminatur, nulla abiectio const).

INVENTUM SUTURAE: -intra in schizzo originali ambulationem tabulae
ligaminum emitte postulabat ("papers over the missing QB name-
filter") - QC eam DISSOLVIT: filtrum post-exsecutionis per
ascensum patrum + congruit(q_scopus) ubi q_scopus =
"definitio-functionis:definit(sym)". Zuccharum purum, nulla
ambulatio nova (gemellus sub-aestimationis iterum - sutura ante
pretium).

Posturae: selector reiectus = fractura clara cum causa (exitus 2);
arbor cum erroribus = CAUTIO + PERGE (nodi error quaeribiles -
vinculum robustitatis, dissimile emitte); exitus grep-paritate
(0 congruentia / 1 nulla / 2 fractura). PORTA IN ORTU (doctrina):
verbum -proba = fixtura inclusa + quinque probationes numeratae +
selector malus reiectus, TENET/exitus 2. Launcher = exemplar
emitte.sh (relink incondicionalis - laqueus obiecti veteris
structuraliter vitatus) + clausura AB AEDILE (semen
selecta_fontes_generata.sh -> tools/silva_fontes_generare.sh).
Futura nominata (worklog): capturae impressae, plagulae plures,
-breviter. Barrae: -proba TENET, cursus manuales contra fontes
vivos, suite intacta.

## SELECTA: RELATIO (2026-07-22) - NATUM UNO DIE

Navatum: ./silva/selecta.sh <plagula> '<selector>' [-intra sym]
[-omnia] [-proba] - grep structura-formatum vivum. Compilatio
munda primo cursu (batteria vexillorum plena); PROBA TENET
(fixtura inclusa, V numeri exacti + selector malus reiectus).
Probationes vivae: :vocat(memset) in piscina.c (corpus octetim);
accessus[tok_titulus="segmenta"] in xar.c; -intra xar_locare =
II accessus exacti (falsae vacuae DUAE = exspectationes meae
falsae, instrumentum rectum - xar_obtinere ->segmenta numquam
tangit; grep textualis "segment" 120 ictus, quaestio structuralis
II veros). Receptae postulationis 2026-07-11 solutae. Clausura
AB AEDILE (9 fontes); exclusio examinis (dirent, ut emitte).
Laqueus exitus-fistulae me ipsum momordit inter probationes -
lex memoriae re-probata. Futura in worklog (capturae impressae,
plagulae plures, -breviter). Parcum 01KY53MP5X claudendum.

## EXAMEN LEXICON: DERIVATIO + EXTERNA + MODULUS (2026-07-25..27)

Arcus trium commissorum (71e30ab, 401cd17, dd30242, e00ca6c) super
parcum 01KYB3NNR4 (varians b tum a) et decretum 01KYDMTS5N.

QUOD MUTATUM EST: tabula routing -posix unci DELETA. examen sectiones
lexici POSIX ex lineis '#include <...>' plagulae iudicatae ipsius
derivat; systema_posix.h in XIV sectiones tagis notatas divisa
(sectio caput= / poscit= / semper=). 'poscit' inventum necessarium,
non praevisum: select() timeval poscit, stat timespec - sine clausura
transitiva lexicon fractum. ISO semper integrum (decretum: motus
baselineae legibilis manere debet). '-posix' = escape totius.
Superficies quas lexicon non fert plagula ipsa fert blocis externa
(caput= = clavis promotionis): reactor.c (poll.h), tcp_posix.c
(quattuor bloci socketorum) - ex CAECIS iudicatae. Uncus exclusiones
POST iudicium consulit: exclusio strepitum silet, verdictum mundum
numquam. Logica communis extracta in instrumenta/silva_lexicon.{h,c}
(consumptor examen; percursus/legatus migrant - 'geminus tertius'
ne quartum pareret). Censura amalgamandi ad instrumenta/*.c ampliata.

MENSURAE PORTANTES: derivatio == -posix plenum in XXXII plagulis
(differentia zephyrum); VI plagulae olim routatae + VIII probationes
ACCIPE sine vexillo; inventum verum primum eodem die - tcp_posix.c
timeval sine sys/time.h (inclusio transitiva Darwin tacita).

LEX NOVA (ter uno arcu): superficies scanner-textualis - qui de tago
SCRIBIT in plagula quam scanner leget, tagum evocat. Nuntii erroris
excerptoris ipsi recusationem ciebant (examen.c se iudicare non
poterat, exitus II). Verbum nudum sine uncis; CAVE in silva_lexicon.c.

IN VOLATU AD SESSIONEM PROXIMAM:
- Design B: percursus + legatus derivationem sumant. AMBO systema
  SEMEL parsant (percursus.c:817 'systema semel parsatum', legatus
  praeparatio communis 64MB) - ergo non transcriptio examinis sed
  cache per signaturam sectionum AUT seminatio filtrata (provenientia
  adest: SilvaSymbolum.lexema->byte_offset). Consilium in notis
  01KYBAG1MJ; praeparator.c = sedes naturalis (legatus+vindex ambo
  inde); percursus prior (re-pin exclusionum retirat).
- RE-PIN exclusiones.txt (XLIII) = opus iudicii, sessione RECENTI
  aperienda: ordo motus 'mendacium sanatum' ab 'mendum captum'
  discernendus LEGENDO. vectis -corpus RUBER iam ante (01KYDPHSZR,
  duae plagulae tesserae non pinnatae; praedictum 01KXJ2R0Z1).
- Promotio blocorum externa in systema_posix.h = porta auspicis
  (formae certificandae ubi officina exsequitur) - 01KYB2Z7XA.

## DESIGN B (lexicon derivatio in percursu + legato): INTENTIO (2026-07-27)

Seams READ before pricing (percursus.c totum; praeparator.c totum;
examen.c fluxus; legatus.c 1182/1669/1761/3906/4415; vectis -corpus).
Finding that settles the design: examen and percursus have IDENTICAL
channel structure - both walk real repo headers (examen.c:409), both
feed ONE systema text into three channels (contextus lexicon /
oraculum seeding / analysare_cum_systemate). The only difference is
per-file vs once-at-startup assembly. So derivation for percursus =
per-file assembly + a preparation cache, nothing deeper.

DECISUS: preparation cache keyed by COMPOSED TEXT content.
Per file: read file FIRST (examen's reorder), derive sections +
externa via silva_lexicon, compose systema text, cache lookup.
Identical text => identical prep {ctx, systema_parsura,
systema_semantica}; empty signature => shared ISO base prep.
  - Equals examen BY CONSTRUCTION (rejected: coarse two-prep routing
    - only empirically equal, and cannot carry externa files at all;
    rejected: filtered seeding - filter seams in three channels incl.
    macro lookup in the expander hot path, and externa still needs
    per-file text anyway).
  - MEASURED (not estimated): <=21 raw include-signatures in the
    whole corpus (sectio caput-lists + poscit closure collapse
    further, ~15); dominant cluster = stdlib.h alone (182 files -
    SURPRISE: stdlib.h IS a POSIX sectio, so non-empty signatures
    are common; cache dominance unaffected). ~20 preps total.
  - Header texts read ONCE into shared pool, praebere'd into each
    new ctx from memory; per-prep pool destroyable. Summary line
    'praeparationes: N (X MB)' = the memory story stays a printed
    fact (measure from day one).
Deliverables in order: (1) silva_lexicon_componere - pure-text
composition moves into the module, examen refit to a call (twin
prevention done at the source); (2) percursus adoption; externa
fracta = loud line + counter, NEVER a clean verdict; vectis -corpus
gathers [externa fracta] lines into the diff set (a broken
annotation must not be invisible to the oracle); (3) praeparator/
legatus phase 2 - lazy prep cache, sessions never touching POSIX pay
nothing; NAMED RISK to price at that seam: the v0.2 latina PIN
(macro provenance collapses under lexicon concatenation - what
consumes the macro index / definition jumps must be read first).
EXPECTED RED: vectis -corpus diverges massively after (2) - that IS
the input to the re-pin session (separate, fresh, rows READ).

## DESIGN B PARS I (componere + percursus): RELATIO (2026-07-27)

Navatum (472160c, 9bff7ce): silva_lexicon_componere = compositio
systematis UNO loco (ISO + POSIX totum-aut-derivatum + externa,
allocatio NOVA semper - aequalitas contenti = clavis cache); examen
refit ad vocationem unam (110 lineae inline -> 1). percursus:
cache praeparationum {ctx, parsura, semantica, piscina} clave
textus compositi; capita SEMEL lecta (piscina longaeva), in
contextus praebita; contextus basis solum sine -semantica (basis
pristina intacta); fractura annotationis = INFRA alta voce +
piscina destructa, verdictum mundum numquam; vectis -corpus lineas
FRACTA in tabulam colligit (fractura oraculo invisibilis esse non
debet).

MENSURAE: praeparationes XXI (568.7 MB, linea summae impressa -
factum, non aestimatio; INTENTIO ~15-21 tenuit); XII plagulae POSIX
sanatae corpore toto, REICE novae ZEPHYRUM (asymmetria periculi
CONFIRMATA: derivatio numquam nocet); sweep 182s; examen==percursus
in lib/ (REICE {imago, iter_directoria} ambobus, dirent-classis
vera).

DIVERTICULUM DIAGNOSTICUM (dimidium sessionis, recte impensum):
tres plagulae REICE quas pinna non habebat -> hypothesis vitii mei
FALSIFICATA gemino-specimine (git stash, binarium vetus, arbor
eadem: REICE etiam vetere); contextus RECEPTUM non status (doctrina
silva_contextus.h:7 - dependentia ordinis ambulationis EXCLUSA);
determinismus probatus (sweep vetus bis, octetim aequalis, L REICE).
Conclusio: corpus post pinnam motum (villa/gesta/burndown editiones
attributae singulae in 01KYDPHSZR), relatio +II de 2026-07-25
observatio incompleta - lex truncationis, quae me ipsum intra horam
iterum momordit (~XXI ex diff partiali; verum XXXVIII).

FLAGRATIO IV legis scanner-textualis: silva_lexicon.h:66 (doc
functionis!) tagum in commentario portabat - latens quia uncus et
-census .c SOLUM iudicant; percursus .h ambulat ergo detexit.
Sanata verbo nudo; MEMORY.md renovatum.

Instrumenta (debrief): adhibita = legati corpus (structura chordae;
CAUTIO VIGILIAE ignorata recte - amalgama recentior binario, classis
nota), stash-gemini bisectio, sweeps in umbra III, examen ut
oraculum per-plagulam, nexus warmup; fructus = gemino-specimine
culpa mea intra minuta exclusa; determinismus-bis mysterium in
archaeologiam vertit; linea praeparationum impressa rem memoriae
factum fecit statim; asperitates = zsh 'echo ===' expansione
aequalis periit (=cmd quaerit); diff -corpus sine fistula in
plagulam lectus -> relationes incompletae BIS (25a et mea hodie) -
mos novus: diff in plagulam, wc ante compendium; desiderata =
nulla nova (pinnatio-cum-causa iam in 01KYDPHSZR).

IN VOLATU: pars II (praeparator/legatus, cache pigre; PERICULUM
pin latinae v0.2 - consumptores indicis macrorum legendi ante);
re-pinnatio = XXXVIII contra XLIII, sessione recenti, lineae
LEGENDAE.

## RE-PINNATIO EXCLUSIONUM (2026-07-27): RELATIO

Sessio recens post design B, ordo operis ex 01KYDPHSZR. Tabula:
sweep novus XXXVIII contra pinnas XLIII - XXXIII in ambabus, X
solvendae (ACCIPE confirmatae singulae), V novae. QUAEQUE linea
lecta: symbola culpae ex positionibus diagnosticorum extracta
(38 plagulae -> classes MENSURATAE, non aestimatae).

CLASSIFICATIO: XXXI ex XXXVIII = hiatus superficiei POSIX, quorum
XXIV UNA superficies (dirent - eadem ansa ambulationis corporis
ubique). Promotio in systema_posix.h (UNDA 3) pro pinnatione electa:
sectiones dirent/mman/termios/ioctl/utime/stdio-posix/fcntl-open/
signal-extra/errno-EEXIST + macra st_mtime (compatibilitas Darwin);
sectio select caput alterum sys/time.h accepit (mos BSD verus -
tessera_pons ita includit). Auspex_posix formas certificat (dirent
1048, termios 72, sigaction 16, winsize 8, utimbuf 16 + macra
omnia); TU verum fcntl.h carens CLAMAVIT - porta functa. I mendum
VERUM sanatum: tools/fons_merge.c signed char contra i8 (insignatus)
- violatio constrictionis C89 vera quam examen invenit (c42).

RESIDUA VI, quaeque REICE recte: imago (venditum stb), fontes_
generata.h x3 (fragmenta sine includente - .h ambulatio percursus
ea detexit), flagrationes (fixtura consulto prava), amalgamator
tesserae (ARTIFICIUM: resolutio inclusionis basinomen silvae capit,
non vicinum - desideratum 01KYJ6740K; cpp verus plicaturam
includentis primam quaerit).

FORMA NOVA PINNAE: via<TAB>causa ('#' commentaria) - decisio
recordata per lineam, non lista nuda. Consumptores III adaptati:
vectis diff (cut -f1), uncus examen-custos (columna prima), legatus
_exclusiones_onerare (clavis ad tab truncata, '#' saltatae).
Probatio legati re-directa: uuid.c (sanata) -> imago.c (pinnata
manens). Suite legati 311/311; uncus manu probatus ambobus modis.

Pinnae XLIII -> VI. Exclusio prae-iudicium legati nunc VI plagulas
tegit (reactor/tcp_posix sponte iudicium verum accipiunt post
/mcp reconnect - residens vetus adhuc currit).

Instrumenta (debrief): adhibita = examen ut oraculum per-plagulam
(~L cursus), extractio symbolorum ex positionibus diagnosticorum
(awk+sed super -machina), auspex_posix ut porta sui-corrigens,
sweeps corporis III in umbra, tabularium (desideratum + notae);
fructus = pretiatio ad suturam ITERUM vicit - 'pinna XXIV lineas
dirent' fuisset lista quae mentitur, sectio una eas sanavit (lex
subaestimationis, flagratio quarta); extractio symbolorum classes
in minutis dedit ubi lectio XXXVIII plagularum horas costasset;
asperitates = tectum VI-linearum-per-plagulam in extractione
classes secundas IV plagularum celavit (sweep eas rettulit - gemina
observationis partialis, in tabulam notata); desiderata =
resolutio inclusionis relativa includenti (01KYJ6740K, supra).
