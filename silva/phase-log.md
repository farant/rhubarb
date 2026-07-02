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
