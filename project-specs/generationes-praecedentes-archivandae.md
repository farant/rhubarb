# Generationes praecedentes — quid archivari possit (analysis)

2026-08-20. Measured, not assumed. Prompted by Fran: "arbor (and
arbor2) were the original attempts at C parsing that became silva…
at some point it might make sense to archive them."

The lineage is right — arbor v1 (recursive descent) → arbor2
(hand-written GLR) → lapifex (generated LALR) → silva (GLR,
generated tables), per silva/CLAUDE.md. But the archiving question
has a surprise in it, and one non-obvious blocker.

## 1. Footprint, measured

- **90 tracked files** match `arbor|arbor2|lapifex`.
- **78,300 lines** across `lib/` (34 files) + `include/` (19).
- **24 predecessor test mains**: 13 `probatio_arbor*`, 11
  `probatio_lapifex*` (silva has 42 for comparison).
- The **four slowest tests in the whole root suite** are lapifex
  (~6.7s each, of a 193s run).
- 29 files in `project-specs/` — these are the DESIGN RECORD, not
  code. **Keep.** silva/CLAUDE.md instructs future sessions to
  re-read `silva-brainstorm.md` and the arbor brainstorms whenever
  the maximalist rule starts feeling negotiable.

## 2. THE SURPRISE: lapifex is not dead

`lib/nuntium_schema.c` includes `lapifex_generare.h`,
`lapifex_lexere.h`, `lapifex_parsere.h`.

**Nuntium is a live house library** (root CLAUDE.md: "we also have
our own version of protobuf basically in nuntium.h"). It has four
test suites — `probatio_nuntium{,_generare,_rotunda,_schema}.c` —
and `nuntium_schema` appears in the generated suite manifest.

So lapifex is not a superseded C parser sitting in the attic: it is
the **parser generator underneath nuntium's schema compiler**. It
was superseded *as silva's ancestor* while remaining load-bearing
*as nuntium's substrate*. Those are different roles and the file
tree does not distinguish them.

**Consequence: `lib/lapifex_*.c` and `include/lapifex_*.h` CANNOT
move** without breaking nuntium.

Note the separable part: the 11 lapifex test mains are named
`probatio_lapifex_c89_*` — they exercise lapifex as a **C89
parser**, which is the retired role. Nuntium's use is covered by
nuntium's own suites. So the *suites* are archivable even though the
*engine* is not — which is also where the 4 slowest tests live.

## 3. Consumers of arbor v1 / arbor2

Beyond their own 13 suites, four files reference them:

| File | Uses | Built? |
|---|---|---|
| `lint/lint_probatio.c` | `arbor.h` | NO — `lint/` holds this one file; no binary in `build/` |
| `tools/glr_quaestio.c` | `arbor.h` | NO |
| `tools/glr_debug.c` | `arbor2_*` | NO |
| `probationes/probatio_compound.c` | `arbor2_*` | NO — not in the suite, no binary |

All four are tracked but unbuilt — orphans. They move WITH the
generation they depend on, or they become dangling references.

## 4. The generated manifest

`compile_tests_fontes_generata.sh` (GENERATUM, from
`tools/compile_tests_fontes_generare.sh` via aedilis) currently
lists **15 arbor + 8 arbor2 + 6 lapifex** library sources, because
it is the union of closures over the suite's mains. Any move
requires regenerating it — and unlike silva's manifests (gated as of
7a9c0c5), **this one has no freshness gate**. Same class of trap,
different manifest.

## 5. The rule that already governs this

spec-v2 §4.1, "freeze-then-delete refinement" — worth obeying
because it was written against a specific mistake:

> engines may be deleted at substrate parity, but each v1 downstream
> SUITE is deleted only after its bar has been harvested into silva/
> at its milestone. **A deleted test suite is a test suite nobody
> ports.**

Harvest status of the four Tier-3 bars, measured:

| Bar | Silva counterpart | Status |
|---|---|---|
| v1 quaestio (100 assertions) | `probatio_silva_quaestio{,_messis,…}` | **harvested** — `messis` = harvest |
| v1 formator (103) | `probatio_silva_formator` + formator milestone complete | **harvested** |
| v1 typus | *none* | **BLOCKED** — type-resolver milestone not reached |
| v1 index | *none* | **BLOCKED** — project-index milestone not reached |

So `probatio_arbor_typus.c` and `probatio_arbor_index.c` are still
holding bars for milestones that have not happened. Archiving them
is fine (they stay readable); **deleting** them is what the rule
forbids. Fran's move-don't-delete instinct is exactly right here.

## 6. What is actually archivable

**Movable:**
- `lib/arbor*.c` + `include/arbor*.h` (v1 and v2 engines)
- the 13 `probationes/probatio_arbor*.c`
- the 11 `probationes/probatio_lapifex_c89_*.c` (suites only)
- the 4 orphaned consumers (§3), moved with their generation
- `probationes/fixa/arbor_index/` (5 files) — verify no other user
  first

**Must stay:**
- `lib/lapifex_*.c`, `include/lapifex_*.h` — nuntium's substrate
- `probationes/fixa/roundtrip/` — **shared v1+v2 corpus that silva
  adopted**; `probatio_silva_fidelitas` runs it today and arbor's M1
  gate is about to run it again. Moving it breaks live tests.
- the 29 `project-specs/` documents

## 7. Downsides of the move (Fran asked)

1. **The lapifex split is not obvious from the file tree.** After
   the move, `lib/lapifex_*.c` sits in `lib/` with its C89 test
   suites in an archive folder. Someone will eventually read that as
   "half-archived" and finish the job, breaking nuntium. **Mitigation:
   a header comment in the lapifex sources naming nuntium as the
   live consumer.** Cheap, and it is the only thing standing between
   a future reader and a broken build.
2. **Regeneration required**, and that manifest is ungated (§4).
3. **Unbuilt orphans stay unbuilt** — moving them changes nothing
   functionally, but it does mean nothing will ever tell us if they
   rot. They are already rotting silently; the move just relocates
   that.
4. **Harvest becomes harder, slightly**: an archived suite that no
   longer compiles is harder to mine for the typus/index bars later.
   Mitigation: move engine + suite together so the pair could be
   restored as a unit.
5. **Not a downside**: git history survives `git mv` with rename
   detection, and the frozen-reference purpose in silva/CLAUDE.md is
   *better* served by a readable archive folder than by deletion.
6. **The move would silently reverse a standing decretum.** Found by
   the ledger's duplicate-guard, not by reading code — decretum
   **01KXSAK5K7HW** (Fran, 2026-07-17) rules that the 8 codex-58
   (VANA) sites in the legacy parsers are NOT fixed, NOT `TOLERA`-ed,
   and NOT excluded: they "stand in the census as named frozen
   residue," with the metric pinned at **census 58 = 8**. That
   visibility is the whole point of the ruling. If the files move out
   of the swept paths, the count silently drops to 0 — which reads as
   an improvement and is actually the loss of a deliberate choice.
   **Before moving: decide whether the pin follows the files, or the
   decretum is superseded.** Sibling decretum: 01KXR5ZMAWV0.

## 8. Recommended procedure

1. Decretum in the ledger recording §5's harvest table — the
   expensive-to-reconstruct fact.
2. Add the nuntium-consumer comment to `lib/lapifex_*.c` FIRST
   (§7.1) — before any move, while the reason is in view.
3. `git mv` the §6 movable set into `archivum/` preserving relative
   layout.
4. `./tools/compile_tests_fontes_generare.sh` to regenerate.
5. Full `./compile_tests.sh` — expect the suite to LOSE ~24 mains
   and get materially faster; expect nuntium's 4 suites to stay
   green (that is the lapifex check).
6. Measure the suite time before and after; record it.
