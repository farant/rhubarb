
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

3. **Is there a common failure mode across arbor, arbor2, and lapifex — and did silva actually escape it?**


## Runner-up questions

Not yet promoted to full entries.

- **Does writing the TV episode ever feed back into the library design?** "One
  C89 library = one episode" is a constraint that forces each library to be
  dramatically legible — to have a protagonist, a conflict, a turn. It would be
  unsurprising if that quietly improved the APIs.
