# The Testing Pillar — probatio as first-class officina concern (idea note)

Date: 2026-07-08
Status: Recorded idea, later project (post-M3 consumer of the VM edge).
Origin: Fran, bounced in conversation after officina-spec-v2 sealed.
Related: officina-brainstorm.md, officina-spec-v2.md, tessera (pons
memoriae precedent), silva roadmap item #4 (libFuzzer harness).

## Thesis

**The VM boundary is a universal test seam.** Everything a program cannot
compute for itself — memory, time, files, entropy, network, windows,
input — crosses machinula's builtin edge, and builtins are swappable.
Test infrastructure that normally requires per-library discipline
(dependency injection, tessera's pons memoriae, mock frameworks) becomes
a PROPERTY OF THE PLATFORM. Owning parser + compiler + VM doesn't just
permit a testing pillar; the VM's edge is shaped like one.

The Wave 2 deterministic time/entropy stubs (spec-v2 §III) are the seed
instance of the general mechanism.

## The four applications

1. **Fuzzing (differentiated, not just straightforward).**
   - Coverage-guided without instrumentation: machinula IS the
     instrumentation — edge coverage from watching interpreter branch
     decisions (libFuzzer needs a compiler pass for this; we get it free).
   - Region memory model = built-in memory-error oracle, on a platform
     where valgrind is dead and ASan needs clang flags.
   - Grammar-aware fuzzing via silva: mutate TREES, not bytes — every
     input parses, so you fuzz deep logic instead of error paths.
   - The killer loop: fuzzer finds crash → deterministic replay →
     VINDEX STEPS BACKWARD from the crash site. Fuzz-to-time-travel.

2. **Unit testing beyond credo.**
   - FAULT INJECTION: the VM owns the call instruction — "make malloc
     fail on the Nth call" is trivial. Allocator-failure paths (the most
     famously untested code in C) become testable.
   - COVERAGE with zero gcov ceremony, mapped through indicium to
     source; saltuarius could render it as a layer.
   - PROPERTY-BASED TESTING: semantica knows signatures + types →
     derive input generators QuickCheck-style. Type tables → generators
     is the reflection C never had.
   - Isolation: fresh VM region per test; leaks detectable by region
     accounting.

3. **Snapshot testing, structural not byte-level.**
   - Semantica's type + layout tables let the VM walk the heap TYPED —
     a snapshot is a structural serialization of live data structures,
     diffable semantically, stable across layout changes.
   - Same mechanism = generic serialization of arbitrary C structs
     (reflection-derived, nuntium-adjacent).
   - "Run to point X, serialize the world, diff against golden" as a
     one-liner.

4. **Headless GUI testing (the strongest case, not the weakest).**
   - The Tier-2 platform boundary (fenestra_* etc., spec-v2 §II.2) IS a
     builtin boundary: the VM routes those calls wherever we say.
     Substitute a virtual framebuffer + synthetic event queue → headless
     GUI testing BY CONSTRUCTION — pons memoriae generalized, enforced
     at the platform edge instead of by library discipline.
   - The VM owns the clock → timing/animation flake dies of determinism.
   - Snapshot the DRAW-COMMAND STREAM rather than pixels (non-brittle
     goldens).
   - Ceiling: record an interactive session, replay headless as a
     regression test forever.
   - Honest limit: you test YOUR commands, not Apple's rendering of
     them — the right boundary anyway.

## The caveat that is a feature

All of this lives in interpreted mode; native output (M5) loses the
seam. But tests are exactly the workload where peak performance doesn't
matter — the project premise ("not competing with LLVM") is load-bearing
here, not a concession. Develop and test in machinula, ship native: the
Smalltalk/Lisp workflow reborn with C89 semantics.

## Sequencing

Kind: nothing here needs anything beyond what M0–M3 already build. Every
item is a CONSUMER of the VM edge — fault injection, coverage, typed
heap walks, world-substitution are features of machinula + semantica
tables + indicium, added when pulled. No officina milestone changes.

## Open

- Latin name for the pillar (Fran's ceremony). The house test framework
  is credo; this is the platform layer beneath/around it.
- Which application gets pulled first (likely whichever the corpus-bar
  work makes cheap: coverage and fault injection fall out earliest).
