# Officina M2 interview — raw Q&A

Date: 2026-07-09
Context: post-simulatio-4 DECISUS conversation (project-specs/
officina-simulatio-4.md §V + the forks beneath). 16 questions, 4
batches; ALL recommendations accepted by Fran. Spec:
project-specs/officina-m2-spec.md.

## Batch 1 — value representation & linking

**Q1. FILE* representation in stderr/stdout/stdin cells (and fopen
returns at Wave 1)?**
A: OPAQUE HANDLES. Cells hold small ints (0/1/2; fopen returns 3+);
bridge maps handle→host FILE*. Region stays closed to foreign
pointers, runs stay deterministic, handle table = the v2 record/replay
seam. (Corpus never dereferences FILE* — grounded.)

**Q2. Unresolved-symbol policy at conexio?**
A: LAZY TRAPS EVERYWHERE. Every unresolved symbol binds to a trap stub
that halts with the symbol name IF CALLED. Load never fails;
sistere's fault-if-executed posture extended to the link seam.
Classification gains "builtin deest".

**Q3. Function descriptor shape?**
A: MINIMAL 16B {s64 tag; functio reference}. Tag = the M5
interpreted-vs-native seam designed in now. Layout is VM-internal —
grow later when M5/vindex state real needs.

**Q4. Register width convention (the named DECISUS)?**
A: CANONICALIZE ON WRITE. Every op writes a full 64-bit word, extended
per the op suffix's signedness (movere.s32 sign-extends, legere.i8
zero-extends; f64 = bit-pattern). Reads take low bits. Variadic bridge
reads plain words — zero per-call analysis. (Forced by grounding:
operands carry values only, registers untyped.)

## Batch 2 — pipeline & bars

**Q5. Moduli residency / lowering pipeline?**
A: LOWER ONCE, CACHE, FREE TREES. Per TU: parse → semantica →
demissio → distill origo line-table → free front-end arenas. Moduli +
line tables resident (~100MB-scale); parse trees never accumulate
(avoids the 9.7GB-apex shape). Suite run = conexio merge from cache.

**Q6. Interpreted wall-clock posture?**
A: ACCEPT; MEASURE; DECIDE LATER. Minutes-scale Wave 0 acceptable for
v1. Per-suite timing column day one; dispatch counters find hot ops;
jump-table park waits for that evidence. (C89 -pedantic forbids
computed goto — switch dispatch is the ceiling until M5.)

**Q7. Chunk split?**
A: FOUR CHUNKS per sim R8. M2a conexio+regio+descriptors+loader (bar:
world links, $main found, hand-written .medulla executes) → M2b
interpreter+flight recorder+Wave-0 floor builtins (bar:
probatio_piscina GREEN interpreted) → M2c Wave-0 sweep+classification
runner (bar: Wave 0 green) → M2d Wave 1+systema_posix (bar: Waves 0+1
= v1 SHIPPED).

**Q8. stdout differential bar?**
A: STANDING SECOND COLUMN. Runner byte-diffs interpreted stdout
against pinned native capture (pass-path proven deterministic +
address-free). Catches exit-0-but-wrong bugs (the float-"100" lesson).
Legitimately nondeterministic suites get named exclusions.

## Batch 3 — engine internals & posture

**Q9. Region sizing?**
A: BIG RESERVATION, LAZY COMMIT. One large fixed-base mmap (16-64GB
address space); pages commit on touch (macOS native behavior).
Contiguity + determinism guaranteed; no growth machinery ever.

**Q10. Stack overflow posture?**
A: EXPLICIT CHECK AT VOCARE. Frame push checks remaining stack space;
breach = honest halt with flight-recorder report. Portable C89,
deterministic, no signals. One compare per call = noise.

**Q11. Builtin table architecture?**
A: STATIC NOW, PUBLIC ROW AT M4. M2 ships a static internal
name→bridge table. The REPL (M4) is the first real consumer of
user-registered builtins — the public row + hospes call arrives there,
designed against an actual need. (Eskil rule: interfaces are the
expensive part.)

**Q12. Time-travel (v2) foreshadowing?**
A: NAME THE SEAM, BUILD NOTHING. Every builtin call routes through ONE
chokepoint function, documented as the future record/replay seam.
Zero cost now; v2 finds a single door waiting.

## Batch 4 — naming & instruments

**Q13. Module naming?**
A: THREE MODULES + CURSOR. officina_conexio.{h,c} (linker/loader),
officina_regio.{h,c} (owned region: placement, malloc, guards — its
own unit-testable module), officina_machinula.{h,c} (interpreter core
+ builtins). Corpus runner = instrumenta/principalia/cursor.c +
cursor.sh (cursor = runner, literally).

**Q14. Hand-written .medulla fixture scope?**
A: PER-OP-FAMILY, ~8-12 FILES. arithmetica, bitalia, comparationes,
conversiones, memoria (arca/legere/scribere), fluxus, vocationes
(incl. indirect via descriptor), data+relocatio, sistere. Each asserts
exit values. Covers what the corpus exercises without op-by-op
exhaustiveness (the differential bar has clang as oracle for that).

**Q15. Cursor vs fusor?**
A: SEPARATE INSTRUMENTS. Fusor stays the fast standing lowering gate
(every demissio change); cursor runs suites (lower-from-cache →
conexio → execute → classify) as the milestone bar. Different bars,
different cadences.

**Q16. Flight recorder default?**
A: ALWAYS ON. Fixed-size ring (1-4K entries of
functio/bloccus/instr-index), unconditional; halt report always
available. Matches the provenance vision pin ("never optional, never
debug-only"). A knob can grow later if evidence demands.
