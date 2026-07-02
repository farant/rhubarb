# Silva — Phase Log

*Append-only. Each phase gets an INTENTIO entry (what we're implementing, what it
proves, exit criteria) written before work starts, and a RELATIO entry (how it
went, complexities discovered — discovered-while / consists-in / consequences /
handled-by) appended when it closes. English is fine, per worklog convention.
Design record: project-specs/silva-spec.md + silva-spec-v2.md (§§8–11 are the
simulation-resolved contracts; §11.2 is the M1 obligations list). This log is the
re-entry point for any session landing mid-implementation.*

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

### RELATIO — pending (Chunk D: includes, regions, guards, conditio, two-track)

---
