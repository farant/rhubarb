# Officina M2 — diagnostic investments (idea note)

Date: 2026-07-09
Status: pre-simulatio-4 idea note (probatio-pillar precedent). Feeds the
M2 simulation brief and the eventual M2 INTENTIO. Fran approved the
direction ("really smart ideas") before the sim; nothing here is DECISUS
yet — the sim should pressure-test all of it.

## Thesis

M3 is a debugger. Diagnostic infrastructure built for M2 bring-up is not
scaffolding — it is the first draft of vindex's substrate, and officina's
product thesis (introspection over raw speed) arriving early. Every
instruction already carries origo in memory; the VM owns all memory.
Machinula can therefore produce diagnostics BETTER than native C from
day one. Invest accordingly.

House law this applies: instruments built BEFORE the mystery cost 20
minutes; built DURING the mystery, a day (three data points: M0b
haruspex/auspex, M1b fusor -causa, silva percursus/infidelis). M2's
mysteries are millions of instructions deep — the ratio only improves.

## The six investments, ranked by payoff-per-cost

1. **Flight recorder** (build WITH the interpreter core, day one).
   Ring buffer of last N executed instructions, always-on. On any
   abnormal stop (sistere fires, out-of-region access, unresolved
   symbol): dump current functio/block/instruction, register file,
   full call stack, ring tail — WITH source file:line via origo on
   every frame. To stderr (survives crashes). Every future mystery
   becomes "read the report," not "add printfs and rerun."

2. **Deterministic execution from day one.** Fix the region's mmap
   base address: identical runs → identical pointers → identical,
   DIFFABLE traces. Near-zero cost; diagnostic multiplier (bisecting a
   divergence becomes mechanical). Also the first brick of v2
   time-travel — determinism never gets given up, so never retrofitted.

3. **Memory guards + poison fill** in the region allocator (under a
   debug flag; -DPISCINA_DEBUG precedent). Every legere/scribere can
   check in-region-ness + alignment. malloc stays INDETERMINATE for
   corpus fidelity, but poison-fill (0xAA pattern) satisfies the
   semantics while making uninitialized reads instantly recognizable
   in flight-recorder output.

4. **Corpus-bar runner outputs a CLASSIFICATION TABLE, not pass/fail**
   (the M1b fusor lesson applied). Per suite, record HOW it ended:
   exit-code mismatch / sistere (with causa) / VM fault (with halt
   report) / builtin missing (with symbol). First corpus contact will
   be a wall of failures; the classification column turns the wall
   into a worklist.

5. **Cheap counters in the dispatch loop**: per-op execution counts,
   per-functio call counts, builtin call frequencies, memory
   high-water. NOT optional: the jump-table park is filed
   "M2-if-profiled" — this instrument is what makes that call
   decidable. Also locates real interpretation cost before anyone
   guesses.

6. **Full trace mode (-vestigium)** — the fusor -v/-causa analog.
   Every instruction with origo, filterable by functio. Too voluminous
   to run always; it is the escalation tool above the flight recorder.

## Design item surfaced (carry into simulatio-4)

origo is a SilvaNodus* — source-level diagnostics naively require parse
trees alive during execution, coupling VM lifetime to front-end memory
(trees are the bulk of the 9.7 GB apex). Natural move: CONEXIO distills
origo → (fons, linea) pairs at load time. Note what that is: A LINE
TABLE — indicium's core section, born a milestone early because
diagnostics demanded it. M3's format gets its first consumer for free.

## Also standing (from the M2 walkthrough, same conversation)

- Differential oracle: native clang execution IS the certifying oracle
  (third instantiation of the pattern: haruspex → auspex → differential
  runs); stdout byte-diff is the stronger bar above exit codes.
- Hand-written .medulla fixtures through the LECTOR = executable unit
  tests for the interpreter core (conexio/region/ops isolated from
  demissio).
- Recommendation to the INTENTIO: flight recorder, deterministic base,
  and classification table go in as FIRST-CLASS deliverables, not
  nice-to-haves.
