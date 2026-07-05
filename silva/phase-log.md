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
