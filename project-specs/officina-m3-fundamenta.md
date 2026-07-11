# officina M3 — fundamenta (grounding synthesis)

*2026-07-11. Compiled from four parallel grounding explorers (machinula
internals, provenance chain, RDI format research, vindex consumers) at the
M3 on-ramp. This document feeds the M3 simulation and the interview; the
spec (officina-m3-spec.md) is written after both. Per the proven arc:
grounding → simulatio → interrogatio → spec. All file:line refs verified
by the explorers against the sealed instrumenta-prima tree (c6d7439).*

**The M3 bar (spec-v2 §XI):** breakpoint/step/inspect a real credo test
through the format file only; macro-aware line mapping via token origo.
**The opener:** flat instruction arrays frozen at machinula_creare
(~1.3-2x perf + stable instruction addressing for breakpoints).

---

## I. The five load-bearing facts

1. **Provenance is memory-only and the text form is the boundary that
   loses it.** MedullaInstructio carries exactly one provenance field:
   `constans SilvaNodus* origo` (officina_medulla.h:159-160) — an opaque
   AST-node pointer valid only while the silva arena lives. The textual
   .medulla form writes NO provenance and the lector restores origo as
   NIHIL (officina_medulla_textus.c:201-286; medulla.h:159-160 comment).
   **Indicium must be baked at compile time from live memory; it can
   never be reconstructed from a .medulla file.**

2. **Today's line table records the SPELLING site (macro-definition
   line), not the expansion site.** An EXPANSIO token copies linea/
   columna/byte_offset/fons_index FROM THE CORPUS (macro-body token)
   (silva_token.c:96-100), and `_lineam_colligere` uses the token's own
   fields (officina_demissio.c:4652-4684). A CREDO_AEQUALIS halt today
   points into credo.h, not at the assert line. **Both sites are
   recoverable from the origo chain** — spelling = token's own fields;
   expansion/invocation = `silva_token_radix(token)` (silva_token.c:
   264-279) walks to the FONS/API root; `silva_token_profunditas`
   gives depth; every intermediate frame carries nomen_macro + its own
   line (silva_token.h:205-228). The full macro stack is reconstructable
   — but only while parsura is alive.

3. **The line-map plumbing exists but has NEVER carried real data.**
   `demissio_lineas_colligere` (officina_demissio.c:4686-4744) requires
   the parse tree alive; interpretare destroys each TU's arbor piscina
   right after demission WITHOUT collecting (interpretare.c:179);
   `machinula_lineas_praebere` (machinula.c:1601) is called exactly once
   in the codebase — hospes.c:444, with NULL — and cursor.c never calls
   it. **Corpus code run through cursor/interpretare has no via:linea in
   halt reports today.** Collect-before-destroy + feed is the first wire
   M3 connects; it serves both honest halt reports and breakpoints.

4. **Pausing the VM is cheap; no refactor of execution state needed.**
   The instruction pointer is NOT a loop local — it is t->instructio in
   the per-frame Tabulatum (machinula.c:44-52), stored by value in
   m->tabulata; the dispatch re-fetches the frame every iteration
   (:1808-1809). Only loop-locals are per-iteration decode scratch and
   argumenta[] consumed within a single VOCARE dispatch (:2407-2444).
   **Step seam = extract the loop body (~1808-2516) into
   machinula_gradus() executing one instruction;** machinula_currere
   becomes `dum (m->currens) machinula_gradus(m);`. Call boundaries step
   correctly for free: t->instructio advances (:2456) BEFORE the callee
   frame is pushed.

5. **No stable flat instruction coordinate exists; branch targets are
   block indices.** Runtime coordinate = (modulus_index, functio,
   bloccus, instructio-in-block); SALIRE/RAMUS set t->bloccus and reset
   t->instructio to 0 (machinula.c:2300-2315). Every fetch pays THREE
   cross-TU xar_obtinere calls (frame, block via medulla_bloccum_obtinere,
   instruction; :1806-1822) — the worklog's measured dominant cost.
   **The flat-array freeze creates the coordinate that both perf and
   breakpoints need.**

## II. Machinula: the flat-array freeze (the opener)

- **Freeze point:** machinula_creare (machinula.c:1520-1599) — after
  conexio_nectere, before currere; already does 3 world-build passes
  (lineae init, decipulae→aedificata rewrite, errno cell). The bake is a
  4th pass: per conexio_functionem_obtinere(k) → ConexioFunctioNexa
  {functio, modulus_index} (conexio.h:75-78), memcpy each block's
  instruction Xar into one contiguous MedullaInstructio* array +
  per-functio block-index → flat-start table. Allocate from m->piscina.
- **The IR stays IMMUTABLE.** Flat arrays are a derived machinula-side
  structure, honoring the conexio DECISUS ("nulla instructio
  rescribitur", conexio.h:5-7). Demissio, medulla_textus, the canonical
  dump, byte-exact round-trip: all untouched. origo rides by value in
  each ~80-byte instruction — flat memcpy preserves provenance free.
- **Safe:** nothing holds a MedullaInstructio* across loop iterations —
  no aliasing hazard on the hot path.
- **Remap list (all VM-side):** dispatch fetch → flat-index load;
  SALIRE/RAMUS targets translated through the freeze table (NOT
  pre-resolved in demissio); Tabulatum.instructio → flat index (bloccus
  retained for reporting or derived via reverse map); flight-recorder
  ring entries (:54-59, :1826-1835); halt report _positionem_imprimere
  (:220-259) needs flat → (block, instr-in-block) reverse or block id
  carried alongside.
- **Census says branches pay double:** ramus 18.5% + salire 7.2% ≈ 25.7%
  of executed instructions, each forcing a fresh block lookup on the next
  fetch. Flat indices delete that second lookup on the highest-frequency
  pattern. (officina/build/census/, 82 suites, regen via `cursor.sh
  -census`; distribution reliable, absolutes inflated by lapifex suites.
  Top: movere 19.7, ramus 18.5, addere 12.3, legere 9.2, salire 7.2.)
- **Perf frame:** the worklog decomposition holds (flight recorder FREE —
  never touch it for perf; ~1.3-2x grounded estimate for flat arrays in
  the daily -O0 build where cross-TU inlining never happens).

## III. Provenance: what indicium must carry

- **In-memory chain today:** demissio `_em()` stores the AST node
  verbatim (officina_demissio.c:622-642); multi-token expressions get
  the whole expression node; implicit conversions reuse the triggering
  node (:755); synthetic instructions (prologue ARCA/SCRIBERE
  :4296-4307, implicit salire :661) get NIHIL — principled gaps.
- **MedullaLineae today:** sparse origo-pointer → (via_index, linea) map
  (medulla.h:305-309; LineaIntroitus medulla.c:630-634), keyed on opaque
  pointer bytes (survives arena free; the TOKENS do not). One line, no
  column, no site distinction, not PC-ordered. Consumer: halt report
  only (_positionem_imprimere → medulla_lineam_quaerere).
- **Register → variable names exist but are implicit:** every register
  is named (MedullaFunctio.registra, Xar of chorda); params and locals
  get their C names from semantica symbols (demissio :4235-4236,
  :4344-4357), temporaries "tN", shadowing appends _N ("x_37",
  _registrum_unicum :580-606). Names survive the text form as register
  spellings. But: no types (MedullaParametrum {index, typus} is params
  only), unreliable under shadowing, no structured table.
- **Lexical scope is erased by lowering:** all locals are allocated at
  function entry (`locales` loop :4314-4362); MedullaBloccus is a
  control-flow block, not a scope. "Which variables are visible here"
  is unanswerable from medulla. Scope depth lives only in semantica
  (SemanticaSymbolum.profunditas), freed after demission.
- **Net-new for indicium:** (a) a position table distilled from the
  origo chain — use-site line + macro expansion stack, not just
  def-site — built during demission while parsura lives; (b) a variable
  table (name, declared type, register index, lexical extent) sourced
  from semantica at the same moment; (c) column if wanted (nothing
  records it today).

## IV. Vindex: consumer requirements

- **Step semantics defined by measurement:** probatio_piscina = 46,521
  instructions / ~2,100 calls for a ~500-line test; one CREDO assert
  line lowers to dozens-hundreds of instructions (macro expands to
  _credo_notare*(...) with ~7 args + inline expression; credo.h:72-346).
  **"Step over line" = single-instruction steps until the distilled
  origo-line changes AND frame depth ≤ entry depth** (depth =
  xar_numerus(m->tabulata)); step-into = same without the depth guard.
- **Inspection on a paused VM is trivial:** frame K = xar_obtinere(
  m->tabulata, K); register N = t->registra[N] raw i64 (reinterpret via
  the _f64_de_verbo union pattern :128-135); names via
  medulla_registrum_titulus; guest memory via the existing
  _memoriam_probare bounds path (:1735-1755) — interpreted pointers ARE
  host pointers into the owned region. NOTE: machinula.h:14 claims the
  halt report dumps "registra" — it never has (doc drift; _relationem_
  imprimere :263-316 prints stack + ring + counts only). Vindex writes
  the first register dump.
- **Breakpoints need the REVERSE map** — (via, linea) → instruction
  site(s) — which does not exist in any form. Forward-only today.
  Building it on current MedullaLineae would inherit the spelling-site
  bug (§I.2): breakpoints would land on macro-definition lines. The
  indicium line table fixes direction and site together.
- **Load path = interpretare.c steps 1-6 verbatim** (contexts → system
  seed → header praebere → per-TU parse/semantica/demissio → wrapper →
  regio/conexio/nectere; interpretare.c:482-848), swapping step 7's
  currere for the step-granular entry. Plus the §I.3 wire: collect
  lineae per-TU before arena destruction, feed after machinula_creare.
- **TUI skeleton = saltuarius verbatim** (saltuarius.c:653-815):
  tessera_pons_posix → aperire → opus + lector; frame loop = draw-all →
  praesentare → eventum_expectare(250ms) → event→command mapper →
  per-mode handlers → view-model mutation. No widget library — panes
  (source, registers, stack, memory, breakpoints) drawn by hand from
  cells/boxes/text. **Headless test seam: paint functions take a PLAIN
  view-model snapshot struct, never the live Machinula\*** —
  tessera_pons_memoriae + PROBA_CELLA asserts on literal fixtures
  (probatio_saltuarius_visum.c pattern). App-side chorda_aedificator:
  pre-size + reset per frame, never finire (saltuarius lore).

## V. RDI: instincts adopted, complexity skipped

From the live EpicGamesExt/raddebugger source (rdi.h encoding v23,
rdi_parse.c). Full report in the explorer transcript; the distilled
instincts:

**Steal:**
1. Header = magic + version + directory offset/count; hard-reject
   version mismatch; O(1) parse (validate + point, never walk).
2. Section directory = enum-indexed fixed array — **section kind IS the
   array index**; element_size_table[kind] gives count = size/elem.
3. Flat typed arrays + u32 index cross-refs, zero file pointers — mmap
   and cast. Our "voff" analog = VM instruction index.
4. String section = data blob + u32 offset array, string i =
   [off[i], off[i+1]) — length-delimited, not null-terminated.
   **This is chorda on disk.** Index 0 = nil.
5. Two separate sorted line structures, one per direction: forward =
   sorted instruction-index breakpoints ∥ line records (binary search;
   range [voff[i], voff[i+1]) → line[i]); reverse = per-file sorted
   line numbers → ranges → instruction indices ("first index for line
   N" = the breakpoint primitive). Don't force one table to serve both.
6. **Macro sites modeled exactly like RDI inline sites: consecutive
   line entries sharing the same instruction index, shallowest-first;
   debugger walks the run.** The single most transferable idea — our
   expansion stack drops into their encoding without invention.
7. VMap pattern: any "which entity owns this address" = standalone
   sorted (index, entity_idx) array + binary search.
8. Scopes as index tree (parent/first-child/next-sibling) + (local_first,
   local_count) + address range; single [first, opl) range to start.
9. One shared u32 IndexRuns pool for all variable-length index lists,
   referenced by (first, count).
10. Type nodes: one indexed array, kind-discriminated union, kinds
    partitioned by numeric range (classify by comparison). Prune to C89:
    void/ints/float/double/ptr/array/function/struct/union/enum/alias/
    bitfield.
11. Generate struct defs + enums + element_size_table from one schema
    (their rdi.mdesk; we lean nuntium-style codegen) — stops
    producer/consumer drift.
12. Name lookup = sorted-by-string array + binary search (skip their
    hash buckets at our scale).

**Skip (with reasons):**
- The entire PDB/DWARF/CodeView ingest + loose-builder (RDIM) + bake/
  flatten/parallel-orchestration pipeline — we emit directly from our
  own compiler.
- Per-section compression, checksums, separate-binary identity matching
  (exe_hash/guid) — same toolchain, we own source and build.
- The 9 machine LocationKinds, x64 register codes, CFA/unwind, and the
  53-op location bytecode VM — our locals live in fixed per-frame
  register slots; location collapses to ~(kind, slot_index).
- C++ machinery (namespaces, methods, member-pointers, UDT containers)
  and non-C89 type kinds.
- A distinct inline-site record — stacked line entries (+ optional tiny
  macro-name side table) fully serve the macro analog.
- Byte-swapping: RDI assumes host endianness, same-toolchain. Same call
  for indicium — document as host-endian, this-toolchain-only.

## VI. Open questions for the interview

1. **Indicium unit of baking: per-TU or per-world?** Medulla modules are
   produced per-TU at demission (when parsura is alive — the only moment
   provenance is harvestable), but flat instruction indices are frozen
   per-WORLD at machinula_creare (post-link). RDI's global voff space
   has no direct analog. Candidate: indicium baked per-module with
   (functio, intra-functio index) addressing, resolved to world-flat at
   load — but this is exactly the kind of decision the simulation
   should pressure-test.
2. **Who writes the file, when?** Demissio grows an indicium emitter?
   A fusor flag (-indicium)? Every cursor/interpretare run? Cost of
   always-on baking vs opt-in.
3. **v0 scope of the variable table:** names + register slots only
   (register tituli already carry names — cheap), or the full table
   (types + lexical extents) that fixes shadowing honestly? Lexical
   extents must be recorded DURING lowering (scope is erased after) —
   demissio change either way.
4. **Type info depth for v0 inspect:** raw i64/f64 reinterpretation
   (free), declared-type strings (cheap), or real C89 type nodes
   (RDI-style, full)? The bar says "inspect" — how typed must v0 be?
5. **Macro stack depth in the line table:** full stack per instruction
   (RDI-style stacked entries) vs top+root only? Census-of-provenance
   question the sim can answer: how deep do real corpus chains go
   (CREDO ~2; worst case?).
6. **The name.** "Indicium" has been the floated placeholder since the
   brainstorm (spec-v2 open q. 3). Fran's ceremony — needs sealing
   before goldens pin the header magic.
7. **Vindex location:** officina/instrumenta/principalia/ vs sibling
   project (spec-v2 open q. 6, deferred to M3 = now).
8. **Step/breakpoint UX surface for v0:** file:line breakpoints +
   step-over/step-into/finish/continue is the classic minimum; is
   run-to-cursor or watch-on-memory in scope? What does the halt report
   become inside vindex (the ring pane)?
9. **Does the flat-array freeze land first as its own chunk** (perf win
   + coordinate, testable via cursor sweep parity) **with indicium
   building on it, or do they co-design?** (Current lean: opener first,
   sealed by sweep parity + perf measurement.)
10. **Hex-dump for aggregates in interpretare** (sealed deferral) — M3's
    variable table + type nodes would subsume it; fold in or keep
    deferred?

## VII. Corrections to record (small, from grounding)

- machinula.h:14 says the halt report includes "registra" — it does not
  (never has). Fix the comment when machinula is next touched, or let
  vindex's register pane make it true.
- The worklog says the flight-recorder ring is 128KB; it is 4096 × 16B
  = 64KB (machinula.c:34, :54-59). Conclusion (L2-hot, free) unchanged.
- Census lives at officina/build/census/ (not build/census/), 82 suites,
  live via `cursor.sh -census`.
