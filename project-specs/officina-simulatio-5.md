# officina simulatio ⑤ — M3: congelatio plana + indicium + vindex

*2026-07-11. Gradus 2/4 of the M3 on-ramp (grounding → SIMULATIO →
interrogatio → spec). Pretend-implementation against the sealed tree
(c6d7439), ~2.5 simulated hours across four chunks: A flat-array freeze,
B line-map wiring + distiller, C indicium format, D step seam +
breakpoints. Grounding facts assumed from
project-specs/officina-m3-fundamenta.md (read it first). Three real-code
checks were made during the sim (VOCARE callee resolution
machinula.c:2450-2456; cursor fork-before-link cursor.c:879/919/942;
MedullaOperandum + per-functio operand pool medulla.h:100-145,
machinula.c:2425-2428). No shipped code was changed.*

**Outcome: NO reversal.** The opener-first sequencing held and got
stronger (the freeze turned out to be the enabling structure for
breakpoints, not just perf). Two §VI interview questions DISSOLVED
(macro-stack cap; variable extents), one got halved (global addressing),
one measured unknown was flagged (gradus call overhead at -O0), and one
new ceremony question surfaced (the pausa sentinel op).

## Chunk conclusions

- **A (freeze):** FunctioPlana {instructiones contiguae, numerus,
  blocci_initia (+1 closing sentinel), operanda plana} built as a 4th
  machinula_creare pass; keyed by the conexio function index that VOCARE
  already resolves; Tabulatum carries FunctioPlana*. Fetch drops from
  3 xar calls to 0 (with the C13 top-frame cache). IR stays immutable.
  Seal: sweep stdout-parity + probatio_vim before/after.
- **B (lines):** collect-before-destroy + praebere wiring is ~20 lines
  per consumer (interpretare, cursor-parent). Distiller change: MedullaLineae
  records the ROOT (use-site) line via silva_token_radix; full stacks go
  to indicium only. Halt reports improve immediately = early visible win.
- **C (indicium):** genus-indexed section directory; chorda-on-disk
  string sections; per-FUNCTIO forward line table (sorted intra-functio
  idx breakpoints, macro frames = consecutive equal-idx entries); per-via
  retro table indexed by ROOT lines; VARIABILIA = {source name, register
  slot, declared type} per functio; X-macro schema generates enum +
  element_size_table + size-assert probatio; lector = fread whole file
  into aligned piscina block + cast (no pointers in format); host-endian,
  this-toolchain-only, documented.
- **D (step):** MACHINULA_PAUSA halitus genus (resumable, no exitus
  packaging); machinula_pergere + machinula_gradus, currere = compat
  wrapper; breakpoints by patching the FROZEN copy (sentinel op) — zero
  hot-path cost; step-over = stay in the current forward-table index
  range with a frame-depth guard (binary search at line boundaries only).
- **Sequencing (each independently sealable):** ① freeze → ② line wiring
  + distiller → ③ indicium module → ④ pausa/gradus/breakpoint →
  ⑤ headless bar-test probatio → ⑥ TUI (saltuarius pattern).

## Complications (four-part schema)

**C1 — functio→FunctioPlana mapping at call time.**
Discovered-while: sketching the freeze pass and asking how a running
frame finds its frozen array. Consists-in: Tabulatum holds MedullaFunctio*,
not an index; frames are pushed by VOCARE and initium. Consequences:
a per-call hash lookup would tax vocare (~2.1% of executed instructions).
Handled-by: VERIFIED in real code — VOCARE already resolves
descriptor->index → conexio_functionem_obtinere (machinula.c:2450-2456)
and initium resolves the same index; freeze table = parallel array
planae[index]; Tabulatum gains FunctioPlana*. O(1), no IR touch.

**C2 — the operand pool is a second per-fetch xar tax.**
Discovered-while: reading the real VOCARE arg loop. Consists-in: args
beyond the 3 inline operands read t->functio->operanda via cross-TU
xar_obtinere per argument (machinula.c:2425-2428). Consequences: flat
instructions alone leave a per-call xar cost on the table.
Handled-by: freeze flattens the operand pool into FunctioPlana.operanda
in the same pass — free rider.

**C3 — humans still want block names.**
Discovered-while: deleting t->bloccus from the hot path. Consists-in:
halt report and flight-recorder ring print @titulus positions; flat
indices erase the block coordinate. Consequences: reports would regress
to raw indices. Handled-by: binary search over blocci_initia (flat idx →
block) at PRINT time only; ring stores the flat index.

**C4 — freeze cost under cursor's fork-per-suite.**
Discovered-while: costing the freeze × 82 suites. Consists-in: cursor
forks BEFORE linking (cursor.c:879 vs :919/:942), so link AND freeze run
per-child; a full-world freeze is a per-child memcpy of every linked
instruction. Consequences: tens of ms × 82 children — likely seconds on
the sweep, plus COW memory per child. Handled-by: accept on paper,
MEASURE at implementation (the seal includes a sweep timing); named
fallback = lazy per-functio freeze on first call.

**C5 — the walk invariant (cross-phase coupling; the sim's main catch).**
Discovered-while: planning chunk C against chunk A. Consists-in: indicium
bakes intra-functio instruction indices at DEMISSION (parsura alive); the
freeze computes them at LINK. Two independent walks whose agreement is
currently "by construction", not a contract. Consequences: any future
reordering (block sorting, dead-block elision, instruction insertion)
silently desynchronizes every line table and breakpoint. Handled-by:
medulla owns ONE documented canonical enumeration (blocci in Xar order,
instructiones in Xar order); both consumers use it; a probatio pins it
(bake at demission, freeze at link, assert identical indices over a
corpus TU).

**C6 — distiller site choice: root for MedullaLineae, stack for indicium.**
Discovered-while: wiring chunk B with grounding fact 2 in hand.
Consists-in: _lineam_colligere records the spelling site; halt/step UX
wants the use-site; indicium wants the whole expansion stack.
Consequences: fixing only the format leaves halt reports pointing into
credo.h; fixing only MedullaLineae starves the format. Handled-by:
_lineam_colligere walks silva_token_radix → root line for the in-memory
map (sole consumer = halt report, strictly improves); the full
root→…→spelling stack is collected into indicium tables in the same
demission-time pass. Instructions inside credo.c's real functions are
unaffected (not expansions).

**C7 — per-functio addressing kills the global voff (architectural
insight).** Discovered-while: mapping RDI sections onto our world.
Consists-in: RDI's global address space exists because a native PC
forgets its function; our VM's frames always carry (functio, index).
Consequences: UnitVMap, ScopeVMap-by-address, and link-time renumbering
are all unnecessary; intra-functio indices are LINK-INDEPENDENT, so
per-MODULE baking (valid across all worlds that link the module) becomes
possible. Handled-by: all indicium addressing = (functio_idx,
intra-functio idx); per-module vs per-world baking promoted to a clean
interview choice (caching story vs v0 simplicity) instead of a design
mystery.

**C8 — reverse lookups across modules + root-line indexing.**
Discovered-while: designing LINEAE_RETRO. Consists-in: a header line
(credo.h) maps to instructions in many modules; users set breakpoints on
the lines they see (root lines). Consequences: a single-module retro
lookup misses sites; spelling-indexed retro tables would make header
breakpoints "work" but user-file breakpoints fail. Handled-by: retro
tables indexed by ROOT line (consistent with C6); vindex queries every
module's table for the via (linear over ~dozens of modules, fine v0).

**C9 — variable extents dissolve at v0 (lowering makes them true).**
Discovered-while: designing lexical extents for VARIABILIA.
Consists-in: demissio allocates ALL locals at function entry; in our VM
every local genuinely lives for the whole frame. Consequences: showing
all named frame registers is the TRUTH of the execution model, not an
approximation; extent machinery would encode a distinction the VM
doesn't have. Handled-by: v0 VARIABILIA = {source name (from semantica,
pre-disambiguation), register slot, declared type}; extents = named
deferral, landing spot = any future register-reuse/liveness work.

**C10 — format portability posture.**
Discovered-while: speccing the lector. Consists-in: mmap vs fread,
endianness, struct alignment from a raw buffer. Consequences:
over-engineering here is pure waste at our scale. Handled-by: fread
whole file into a 16-aligned piscina block + cast (no pointers in the
format); sections 8-aligned; host-endian this-toolchain-only, documented
(RDI makes the same call); POSIX mmap = named later; X-macro schema
generates enum + element_size_table + a size-assert probatio (stops
producer/consumer drift the house way).

**C11 — breakpoint patching is legal on the frozen copy (+ one ceremony
flag).** Discovered-while: costing a per-step breakpoint check in chunk
D. Consists-in: a per-step set-membership test taxes every instruction
forever; but the frozen arrays are machinula-owned COPIES — patching
them violates no IR immutability. Consequences: classic debugger
patching (swap op for a runtime-only sentinel; restore-and-step to
continue) gives ZERO hot-path cost for run-to-breakpoint. Handled-by:
adopt patching; the sentinel never appears in text and never parses, but
it lives in the op enum's semantic space — how it is spelled
(MEDULLA_OP_NUMERUS+n vs a flag bit vs a proper vocabulary entry) is
Fran's ceremony call (interview).

**C12 — who writes the file, when (the bar's self-honesty).**
Discovered-while: rehearsing the bar end-to-end. Consists-in: "through
the format file only" requires an actual file, but provenance is only
harvestable during vindex's own load. Consequences: without discipline,
vindex could quietly consume in-memory tables and the format would never
be battle-tested. Handled-by: vindex's loader bakes + WRITES the file
for the world it just linked, then REOPENS it and drives only through it
(M1 round-trip tradition); standing-artifact production (fusor flag,
per-module cache) = interview.

**C13 — top-frame pointer cache (minor).**
Discovered-while: counting residual xar calls in the new fetch.
Consists-in: the dispatch still fetches the top Tabulatum from the Xar
each iteration. Consequences: one avoidable indirection per instruction.
Handled-by: cached tabulatum_summum pointer updated at push/pop — safe
because Xar element pointers are stable (2026-07-02 Correctio).

**Measured unknown (flagged, not resolved):** machinula_gradus() as a
separate function costs one non-inlined call per instruction in the
daily -O0 build. Could offset part of the freeze win. MEASURE during
chunk ④; fallback options recorded (pergere keeps an internal loop
sharing the switch; vindex-only stepping loop).

## Effect on the interview (fundamenta §VI)

- Q1 (per-TU vs per-world baking): SHARPENED by C7 — both are now cheap;
  choose on regeneration/caching story.
- Q2 (who writes, when): SHARPENED by C12 — bar answer fixed
  (bake-write-reopen); standing artifact still open.
- Q3 (variable table scope): DISSOLVED for v0 by C9 — names+slots+types,
  no extents.
- Q4 (type depth): OPEN — declared-type strings are nearly free; real
  type nodes are not; still Fran's call.
- Q5 (macro stack cap): DISSOLVED — variable-length stacked entries,
  no cap.
- Q6 (the name): OPEN — now URGENT-ish; the header magic pins it.
- Q7 (vindex location): OPEN, unaffected.
- Q8 (UX surface): PARTIALLY SHAPED — breakpoint/continue/step-over/
  step-into fall out of the design; finish/run-to-cursor cheap; watch =
  defer.
- Q9 (opener first?): CONFIRMED — the freeze is load-bearing for
  breakpoints (C11), not just perf.
- Q10 (aggregates hex-dump): unaffected; VARIABILIA + declared types
  would subsume the scalar half.
- NEW Q11: the pausa sentinel op's spelling (ceremony sensitivity, C11).
- NEW Q12: does the -O0 gradus overhead change the pergere API shape
  (measured unknown above)?
