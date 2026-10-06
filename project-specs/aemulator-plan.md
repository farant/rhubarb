# aemulator — plan (the terminal emulator core)

*Started 2026-10-06, after insula rami / vicus. Fran: a shell running in
our own window ("would our shell actually be like running in a
pictor window?"), emulator first on the house's 6x8 alphabet, the font
stack after it. Module 006 of `../terminal-planning/` (with 007 render
state and 008 PTY pons); its notes and the research notes
`ghostty-vt-architecture.md` and `app-vs-emulator-inversion.md` are the
background - read them before a task.*

## I. Decisions (Fran, 2026-10-06)

1. **Name: `aemulator`** (Cic.: imitator, rival - the root of English
   "emulator"). `terminalis` is taken by the POSIX tty layer.
2. **First milestone: the loopback core.** Enough of the VT surface to
   consume tessera's own output, judged by loopback: tessera draws ->
   memoria pons bytes -> aemulator -> cells must equal tessera's back
   buffer. No PTY yet.
3. **Restart = a fresh shell.** A shell tab is durable (it reopens in
   its place with its title) but starts a new shell; scrollback is not
   kept. The store holds small things only (title, cwd, scroll
   position); the screen lives in the core's memory.
4. **Scrollback = a fixed page pool.** Fixed-size page blocks allocated
   once from piscina; the oldest page is recycled; the limit is in
   bytes. Steady-state output allocates nothing; a hostile flood is O(1)
   per line.
5. **Our parts, Ghostty as reference.** Built on `series_terminalis`
   (tokenizer, both directions by design), `stilus_terminalis`
   (`stilus_applicare`: SGR onto a style), `runae` (widths, graphemes)
   and `codificator_terminalis` (keys out). Ghostty (MIT, `../ghostty`
   pin 12752b2, the pin the house libraries already cite) is read for
   semantics and edge cases; written fresh in C89.
6. **Oracles:** tessera loopback; **Ghostty's in-file tests as data
   vectors** (each vector cites `file:line @ 12752b2`); **esctest and
   vttest** (a network download into a pinned reference clone - named
   when it happens).
7. **Reflow: a named deferral.** Rows carry the soft-wrap flag from day
   one; resize truncates/pads. Trigger: "resizing annoys Fran".
8. **Identity: `TERM=xterm-256color`**, `COLORTERM=truecolor`,
   `TERM_PROGRAM=aemulator` and XTVERSION `aemulator <versio>` (so our
   own tessera can recognise it without probing); DA1 as a vt220-class
   terminal, like Ghostty. No terminfo to ship.
9. **Safety posture (adopted, Ghostty's defaults):** OSC 52 clipboard
   writes allowed, reads denied; the title report (`CSI 21 t`) off - it
   echoes the title back as INPUT; new behaviour only when a program
   asks (mode 2027 etc.).

## II. What exists (to read before tasks)

- `include/series_terminalis.h`: the DEC/Williams lexer from Ghostty's
  `Parser.zig`; lexemes IMPRIMERE (raw run - no UTF-8 inside), EXSEQUI,
  ESC, CSI, OSC, DCS...; bounded buffers, overlong parameter lists
  dropped whole, abrupt series surfaced (SERIES_FUGA), never silent.
- `include/stilus_terminalis.h`: `StilusTerminalis` (colours, underline
  kinds, attributes) and `stilus_applicare` (an SGR lexeme onto a
  style) - the emulator's SGR decoder already exists.
- `include/runae.h`: `runae_latitudo`, grapheme breaking
  (`runae_graphema_proximum`), policy (`RunaePolitica`) - Ghostty's
  width rule, checked against ICU on every codepoint.
- `include/codificator_terminalis.h`: keys, mouse, paste, focus -> bytes
  under modes passed in (`CodificatorModi`); the emulator will OWN those
  modes and hand them over.
- tessera: `tessera_pons_memoriae_creare/_captum` (bytes a frame
  emitted), `tessera_cellulam_legere` (the back buffer) - the two ends
  of the loopback.
- Not existing: any PTY code (`openpty` appears nowhere in lib/); a
  render-state snapshot; a fixed-block allocator (piscina is an arena).

## III. Architecture sketch (names provisional until A1)

```c
/* aemulator.h - terminalis simulatus: octeti intrant, cellulae
 * exeunt, responsa per effectus. PURUS: nulla I/O, nullum tempus. */
nomen structura Aemulator Aemulator;

nomen structura {
     vacuum* datum;
     vacuum (*responsum) (vacuum* datum, constans i8* octeti, i32 n);
     vacuum (*campana)   (vacuum* datum);
     vacuum (*titulus)   (vacuum* datum, chorda titulus);
} AemulatorEffectus;                     /* NIHIL = ignoratum */

Aemulator* aemulator_creare   (Piscina* p, i32 latitudo, i32 altitudo,
                               constans AemulatorEffectus* effectus);
vacuum     aemulator_scribere (Aemulator* a, constans i8* octeti, i32 n);
b32        aemulator_amplitudo(Aemulator* a, i32 latitudo, i32 altitudo);
/* lectio: cellula (graphema, genus latitudinis, stilus), cursor,
 * modi; effusio textus pro probationibus */
```

- **Pipeline:** bytes -> `series_terminalis` lexemes -> dispatch
  (print / execute / CSI / ESC / OSC) -> screen operations. IMPRIMERE
  runs are decoded as UTF-8 and segmented into graphemes by `runae`;
  the width decides cursor advance.
- **Cell:** a codepoint (or a grapheme reference into a side table),
  an interned style id (0 = default), a width kind (narrow, wide,
  spacer-tail, spacer-head), flags (protected, hyperlink id later).
  Rows: cells + soft-wrap flag + dirty flag; reached through a row
  indirection so scrolling rotates pointers, not cells.
- **Memory:** everything sized at `creare` (screen rows, style intern
  table, grapheme side table, later the page pool); steady state
  allocates nothing; every attacker-controlled size capped.
- **Screens:** primary and alternate (1049 with saved cursor); the
  alternate screen never scrolls into scrollback.
- **Permissive:** unknown sequences consumed and COUNTED, never fatal.

## IV. Tasks

Phase A - the loopback core (Decision 2):

**A0 - the oracle before the code.** A vector format (STML: bytes in,
expected screen out - text rows, styles where they matter, cursor),
a replayer test, and a first set of vectors hand-converted from
Ghostty's tests for phase A's surface (print and wide chars, CR/LF/BS,
cursor moves, ED/EL, SGR, alt screen), each citing `file:line @
12752b2`. The replayer fails red (no core yet).

**A1 - API and skeleton.** `include/aemulator.h` for Fran's approval
FIRST (Eskil: the API is what is hard to change), then: creation with
fixed memory, the cell and row model, print through `runae` (narrow,
wide, spacer cells, autowrap with pending-wrap at the last column),
CR/LF/BS, the text dump for tests. Glossary: aemulator.

**A2 - the loopback surface.** CUP and relative moves, ED/EL, SGR via
`stilus_applicare` with interned styles, DECTCEM, 1049 alt screen +
saved cursor, 2026 sync (mode tracked), unknown-sequence counters. All
A0 vectors green.

**A3 - loopback harness.** tessera frame -> memoria pons bytes ->
aemulator -> every cell compared with tessera's back buffer (glyph,
width kind, resolved style), cursor too; first mismatch named
(x, y, expected, actual). Over tessera's existing test frames and a
randomized property test (random cell writes, K frames, compare after
each), and the ludus_tessera replay sessions (pictor, scriba, vicus):
the emulator screen equals what tessera drew.

**A4 - RELATIO phase A.**

Later phases (re-planned after A's RELATIO):

- **B - shell-sufficient + PTY.** Scroll regions (DECSTBM),
  IL/DL/ICH/DCH/ECH, tabs, save/restore cursor, IND/RI, DA1/DSR/CPR
  answers through `responsum`, OSC 0/2 title; the PTY pons
  (`pty_posix` + `pty_memoriae`, module 008); esctest fetched (pinned)
  and its relevant subset run.
- **C - scrollback.** The fixed page pool (Decision 4), viewport
  scrolling, byte limit, eviction; reflow stays deferred (Decision 7).
- **D - full-screen v0.** Origin and insert modes, DEC special
  graphics, mouse / paste / focus / DECCKM / keypad modes handed to
  `codificator_terminalis`; the bar: vim, less, htop look right; a
  vttest session with Fran.
- **E - the host.** Render-state snapshot (module 007: dirty rows,
  shared colour resolution), a ludus component drawing the cells
  (Mandata, `fons_6x8`), a vicus kind + standalone app in both
  targets (Decision 3: the branch holds title/cwd/scroll only); Fran's
  look.
- **F - RELATIO.**

## AUDIENDA

- Where the loopback harness lives: tessera's suite would then depend
  on aemulator (the tessera amalgam must never pull it in), or a third
  integration suite (features/009 Q2). Decide in A3.
- Ghostty's tests are mostly API calls (`t.printString`,
  `t.setCursorPos`), not byte streams; how many convert to byte vectors
  cleanly is unmeasured - A0 reports the ratio.
- `series_terminalis` treats C1 bytes (0x80-0x9F) as UTF-8
  continuations (Fran's choice for input); confirm that is right for
  program OUTPUT too (xterm with UTF-8 does the same) before A2.
- Whether `fons_6x8` (ASCII + Latin-1, tofu otherwise) is enough for
  the E bar; box drawing as sprites is module 010's plan.
- Recorded-session replay (vim, htop, zsh through the PTY) was not
  chosen as an oracle now; natural once B exists.
- The fixed-block allocator question (module 006 open question 1):
  a pool carved from one piscina allocation is assumed; unverified
  against the memory apex discipline until C.
