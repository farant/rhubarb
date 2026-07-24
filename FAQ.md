
1. **Can officina compile rhubarb?**

   Partly — in two of three directions. Answered 2026-07-24.

   - **Read: yes.** All 724 files parse through silva, zero error nodes,
     byte-exact. (officina-brainstorm.md:35)
   - **Run: mostly.** Live `./officina/cursor.sh` sweep, 2026-07-24:
     **79 praeteritae / 93 suites, 0 ruinae**, 5 decipulae (unbridged
     externs — `fenestra_creare`, the GUI layer, outside the corpus by
     DECISUM), 2 sisterae, 2 vitia, 5 tempora. stdout vs pinned native
     captures: **72 identical, 0 differ, 0 ungoldened** (was 64/4/4
     before the captare.sh repair below). Corpus = root
     `probationes/` + `lib/` only (cursor.c:504,595) — officina does
     **not** yet run itself, nor silva/tessera.
   - **Compile: no, and deliberately.** Native aarch64 is M5, unstarted
     (officina-brainstorm.md:203). At the 2026-07-13 fork the choice was
     M5 vs silva-lsp; silva-lsp won, reason recorded: daily use is the
     polishing machine (phase-log.md:3060, 3097).

   So clang remains the only path to a binary. The known gate on M5 is
   Apple Silicon W^X — MAP_JIT + `pthread_jit_write_protect_np`, flagged
   *exploranda ante designum* (phase-log.md:3070).

2. **Has writing in Latin actually changed the code — structurally, not cosmetically?**

   Yes, structurally — but probably not for Whorfian reasons. Answered
   2026-07-24, n=2, held lightly.

   Control group: `../solarium` — same author, same era, same language
   (C), same genre (app/library), English-named. Two matched pairs:

   | pair | Latin | English |
   |---|---|---|
   | immediate-mode GUI | `elementa.h` (2026-01-03): **no context**, 5 named result structs (`FructusSlider{valor,mutatum,hover,dragging}`…), state passed through | `widget.h` (2026-06-23): `WidgetCtx*` threaded everywhere, returns `sol_bool`, explicit `int id` |
   | JSON | `json.h` (2025-12-26): 7 `json_est_*` predicates + 5 `json_ad_*` converters — interrogation and conversion are **separate families** | `json.h` (2026-06-04): 0 predicates, 4 fused accessors (`json_number(v, dflt)` asks + converts + defaults in one call) |

   In both pairs the Latin side is finer-grained AND ~6 months EARLIER,
   so "learned better over time" predicts the opposite of what is there.

   REFUTED on the way: the container-cluster argument (piscina/capsula/
   scrinium/internamentum/…). They are genuinely 7 distinct things, but
   English names them distinctly too (Arena/Embed/Store/Intern/…). That
   test does not discriminate. Also retired: vague-word density counting
   — it cannot separate `status`-the-status-code from `state`-the-bag,
   and `datum` is 89% chorda's struct field.

   MECHANISM (conjecture, the interesting part): probably not grammar but
   **absence of prior art**. English C offers a huge corpus to
   pattern-match onto — `json_number(v,dflt)`, `WidgetCtx *ctx`,
   `json_last_error()` are the shapes of cJSON and Dear ImGui. The name
   arrives with a design already attached. `JsonValor` has no library
   behind it, so the design must be derived from the problem instead of
   recalled from the corpus. Latin does not make you think differently;
   it **denies you autocomplete**.

   Rival non-Whorfian explanation, not excluded: **cost, not language**.
   Latin is slower per identifier; more deliberation per name would
   produce the same data with no linguistic content at all.

   Corroboration: knotapel is the one English corpus inside rhubarb, and
   it is the one domain (knot algebra) with no prior art in ANY language
   — exactly where the naming language should matter least.

   Testable prediction: the effect should be strongest where English C
   has the strongest idioms (JSON, GUI, allocators, HTTP) and weakest in
   novel domains. Currently untested beyond n=2.

3. **Is there a common failure mode across arbor, arbor2, and lapifex — and did silva actually escape it?**

   Yes, one mode, named in the repo. And yes, silva escaped it —
   structurally, not by luck. Answered 2026-07-24.

   PREMISE CORRECTION FIRST: not four attempts at one problem. All three
   "failures" fit in ONE four-week window (arbor v1 lived 3 days,
   2026-01-05→08; arbor2 four weeks →02-06; lapifex began the day arbor2
   stopped), then the repo went dormant Mar–Jun, then silva was designed
   2026-07-02 after an explicit retrospective read of all three. And
   lapifex wasn't a parser replacement at all — its brainstorm opens
   "I want to compile and execute C from within rhubarb." It was the
   COMPILER goal dragging a parser behind it. That goal was finally met
   correctly by separating parser (silva) from compiler (officina,
   2026-07-09). The lineage confusion was itself the error, and it got
   fixed.

   THE MODE (silva-brainstorm.md, verbatim): *"the corners were
   **data-model decisions that foreclosed features** … not missing
   features"* + *"you can build excellent modules and still discover
   **integration walls late**."*

   Stated once: **local decisions, each locally reasonable, that
   foreclose a global feature — where the foreclosure only becomes
   visible at integration, by which point undoing it is too expensive.**

   Instances:
   - lapifex `Lexema*` instead of `Token*` → provenance died at the
     parser boundary (discovered at M3)
   - lapifex s64-only reduction values → rich intermediates smuggled as
     casts and 0xDEAD sentinel wrappers
   - lapifex grammar written conflict-free → the GLR fork machinery
     existed but was permanently dormant; ambiguity inexpressible
   - arbor2: 189 allocation sites vs **1** reset site → memory model
     unfixable by the time it mattered
   - arbor2 dual-ordinarius class → adding a production could silently
     forget its handler
   - arbor2 had a **complete, working `#if` evaluator that was never
     wired in** — the purest instance: the hard part was built, correct,
     and orphaned

   Same feature foreclosed every time: preprocessor conditionals and
   provenance through expansion. *"The feature no attempt ever built."*

   SILVA'S ESCAPE — every counter-measure maps 1:1 onto a named prior
   failure, which is what makes it design rather than luck:

   | prior failure | counter-measure |
   |---|---|
   | integration walls found late | **walking skeleton first** — thin end-to-end thread through every layer before any layer is good |
   | data model forecloses features | maximalist from day one; `SilvaValor` tagged union, not s64 |
   | production without handler | annotated grammar; generator emits construction alongside tables |
   | 189 allocs / 1 reset | two-arena split (scratch + result), reset designed in |
   | orphaned `#if` evaluator | ported AND WIRED — `SILVA_C89_GENUS_CONDITIONALIS`, 135 corpus files carry `#if`, suites 79/79 green (verified live 2026-07-24) |

   STRONGEST EVIDENCE it escaped is not the tests: silva has since been
   integrated by THREE independent downstream consumers — officina, the
   legatus LSP, and the examen judgment tiers — and held. The mode was
   specifically "integration walls discovered late." Three integrations,
   no wall. That is three more than any predecessor survived.

   NOT EXCLUDED: silva now carries far more load than any predecessor
   (semantica, examen, dataflow, LSP, query language), so "hasn't hit it
   yet" cannot be fully ruled out. The mode is falsified for three
   integrations, not for all future ones.


## Runner-up questions

Not yet promoted to full entries.

- **Does writing the TV episode ever feed back into the library design?** "One
  C89 library = one episode" is a constraint that forces each library to be
  dramatically legible — to have a protagonist, a conflict, a turn. It would be
  unsurprising if that quietly improved the APIs.
