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
10. **Keep the core wasm-able** (Fran: a terminal in a vitrea app or a
    web version may come). Not built now - the door is kept open by
    construction, Ghostty's way (its core ships as wasm because it is
    pure):
    - aemulator's closure is `<aedilis nexus="purus"/>` (like
      series_terminalis and stilus_terminalis): no framework, no
      system headers beyond `<string.h>`, no clock, no threads, no
      mutable statics (two instances never share state), all memory
      from the caller's piscina, every output an effect callback;
    - the render-state snapshot (phase E, module 007) is plain
      relocatable data - offsets, not pointers - so it crosses the
      vitrea bridge or a wasm boundary by copy;
    - two topologies, in this order: (a) the core native in vitrea's
      C host, the page draws the snapshot (module 009 - no wasm);
      (b) the core compiled to wasm in a plain web page;
    - for (b) later: `piscina_ex_memoria(buffer, n)` (a pool over a
      host-given region, no malloc) rather than a wasm libc; and a
      probe that compiles the closure for `wasm32` once a wasm-capable
      clang exists (Apple clang has no wasm backend; Homebrew LLVM 17
      is installed but broken - missing libz3 - 2026-10-06).

### Phase B decisions (Fran, B0 interview 2026-10-06)

11. **`openpty`** (BSD `<util.h>`; one call, Ghostty's choice; Linux
    links `-lutil`) - system headers only in the platform file.
12. **Independent of `processus`:** the fork/exec/CLOEXEC-error-pipe
    pattern (~30 lines) is written again in the PTY library; the two
    contracts (separate pipes + deadline vs one merged PTY +
    long-lived child) must not drift together.
13. **Reaping by `waitpid(WNOHANG)`** when asked (as processus), with
    EOF on the master fd as the usual "shell exited" signal - no
    signal handlers, no global state.
14. **Wake on the fd:** the pons exposes its master descriptor; in
    phase E fenestra's wait gains a file-descriptor wake source
    (CFFileDescriptor) so an idle shell costs zero CPU. The fd is
    therefore part of the pons API from B3. The examen lexicon
    (`silva/fontes/systema_posix.h`) learns the PTY calls (glossary +
    auspex assertions) in B3.

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
A0 as built: `probationes/fixa/aemulator/vectores.stml` - 38 cases
(31 from Ghostty, 7 house cases marked `fons="domus"` with a reason),
steps of bytes in (escapes `\xHH \n \r \t \\`) then assertions:
`textus` (Ghostty `plainString` exactly - see the worklog), cursor x/y,
`pendens`, `visibilis`, `altera`, `ignota`, per-cell `textus`/`vacua`/
`latitudo`/`stilus` (canonical SGR of `stilus_codificare`), `modus`.
`probationes/probatio_aemulator_vectores.c`: 0 - static validation of
every vector (runs with no core); I - the replayer judged against a
fake core (one case must pass; four must fail, each on ONE field);
II - the vectors against the core through a `Probandum` vtable (A1
binds aemulator). "Red" became DEBTS BY NAME (tessera's vectores_initus
pattern): no core = every case a debt, counted; later a case may carry
`debitum="cause"`, and a debt that passes FAILS (promote it) - so the
root suite stays green between commits without hiding anything. Seven
plants caught; the debt-promotion path is planted at A1 (needs a core).

**A1 - API and skeleton.** `include/aemulator.h` for Fran's approval
FIRST (Eskil: the API is what is hard to change), then: creation with
fixed memory, the cell and row model, print through `runae` (narrow,
wide, spacer cells, autowrap with pending-wrap at the last column),
CR/LF/BS, the text dump for tests. Glossary: aemulator.
A1 as built (API approved by Fran unchanged): opaque `Aemulator`; cell
= inline UTF-8 (4 bytes) + length (0 = never written) + width kind +
style index; rows through a pointer array (scroll rotates pointers,
clears the recycled row) with the soft-wrap flag; print per Ghostty
(pending wrap at the last column, wide at the last column leaves a
spacer-head and wraps, overwriting half of a wide char clears the
other half); UTF-8 split across writes carried, invalid bytes ->
U+FFFD, a split rune interrupted by a sequence -> U+FFFD; BEL effect;
default tab stops every 8; resize truncates/pads, keeps the cursor on
screen by dropping top rows, capacity grows by half and never shrinks
(within capacity: zero allocation - measured); the dump writes into a
piscina buffer (no chorda_aedificator). Zero-width codepoints are
DROPPED (graphemes = v2, named). Vectors: 15 green, 24 named debts
(each `A2: CUP/EL/ED/SGR/...`). probatio_aemulator: limits, UTF-8,
unknown counting, BEL, tabs, spacer head, overwrite, scroll clearing,
resize, steady state allocates nothing. Fourteen plants (incl. debt
promotion). PURITY CHECKED: aemulator.c includes only house headers +
`<string.h>`; its LINK closure is not pure - piscina.c (stdlib, stdio)
and stilus_terminalis.c (+ chorda_aedificator: stdio) - the exact list
for the wasm door (decision 10).

**A2 - the loopback surface.** CUP and relative moves, ED/EL, SGR via
`stilus_applicare` with interned styles, DECTCEM, 1049 alt screen +
saved cursor, 2026 sync (mode tracked), unknown-sequence counters. All
A0 vectors green.

A2 as built: CSI dispatch (intermediates, or ':' outside SGR, or an
unknown private marker -> counted unknown): CUP/HVP, CUU/CUD/CUF/CUB,
CNL/CPL, CHA/HPA, VPA (parameter 0 = 1, all clamped, pending wrap
cleared); EL/ED per Ghostty (right starts at a wide head under a tail,
left includes the tail of a wide head, right/complete clear the row's
soft-wrap; ED below/above = EL + whole rows; ED 3 = nothing until C);
erased cells and rows scrolled in at the bottom take ONLY the pen's
background (Ghostty blankCell; BCE - confirmed by "index bottom of
primary screen background sgr"); SGR via `stilus_applicare` onto the
pen with unknown parameters counted; styles interned in a fixed table
(512) - when full, a collection marks styles held by cells, pens and
saved cursors, compacts in order and renumbers; still full -> default
(degrade, never break); DECSET 7/25/1049/2026; 1049 per Ghostty's
xterm reading (save, switch, erase alt, copy cursor + pen; exit
restores, or homes with no save); ESC 7/8 (position, pen, pending
wrap). Two screens resize together (each keeps its cursor row on
screen). Vectors 42 green (35 Ghostty), 0 debts; 17 plants caught -
P10 (cells not renumbered) SURVIVED until the test made a style die
BEFORE a surviving one.

**A3 - loopback harness.** tessera frame -> memoria pons bytes ->
aemulator -> every cell compared with tessera's back buffer (glyph,
width kind, resolved style), cursor too; first mismatch named
(x, y, expected, actual). Over tessera's existing test frames and a
randomized property test (random cell writes, K frames, compare after
each), and the ludus_tessera replay sessions (pictor, scriba, vicus):
the emulator screen equals what tessera drew.

A3 as built: `ludus_tessera/probationes/probatio_ludus_tessera_
reditus.c` in the ludus_tessera suite (it builds tessera from source
and holds the replay sessions; the tessera amalgam stays free of the
emulator - Fran agreed). Each frame: praesentare -> captured bytes ->
aemulator -> purge; every cell of tessera's FRONT buffer (stride =
TESSERA_LATITUDO_MAXIMA, not the width - the harness's own first bug)
against the emulator: UTF-8 bytes (blank = empty or one space on both
sides), width kind, full style (colours PLENI, so RGB is exact), and
the applied cursor. Sessions: hand-drawn frames + diff + no-change; two
resizes; 60 random frames (sors, seed 0x5EED: ASCII, box drawing,
blocks, CJK, emoji, random RGB/default, attributes, fills, cursor); the
vicus app (scriba typing, Ctrl-A n, pictor's block-character image, a
stroke). After every session the emulator saw ZERO unknown sequences.
FOUND: a tessera bug - a full repaint cleared with the previous frame's
pen (BCE) and skipped blank cells; fixed in tessera (pen reset before
`2J`, own regression, goldens, amalgam) - tessera/phase-log.md. Eight
plants: seven caught; "harness skips the style check" survives by
construction (P2 shows that check is what catches SGR bugs).

**A4 - RELATIO phase A.** Done 2026-10-06: `lib/aemulator.phase-log.md`
(A4). Phase A: 43 vectors, the loopback, 46 plants; a tessera bug found
and fixed on the way.

Later phases (re-planned after A's RELATIO):

- **B - shell-sufficient + PTY** (re-planned after A4):
  - **B0 - interview** - done 2026-10-06 (decisions 11-14). Was (the
    PTY seam): `openpty` (BSD, `<util.h>`) vs
    `posix_openpt`/`grantpt`/`unlockpt` (POSIX, the Linux door);
    share an argv/exec/error-pipe helper with `processus` or stay
    independent; child reaping (`waitpid(WNOHANG)` polling as
    processus does, or a self-pipe); the host loop (poll in the frame
    first - module 008 says measure before a reader thread).
  - **B1 - the shell surface in the core** (vectors first, Ghostty's
    convertible tests): DECSTBM scroll regions and everything that
    scrolls inside them (LF/IND at the region bottom, RI at the top,
    NEL, SU/SD), IL/DL, ICH/DCH/ECH, tab stops (HTS, TBC, CHT, CBT),
    LNM. ESC 7/8 already exist (A2).
  - **B2 - answers and effects:** DA1/DA2, DSR 5 and CPR (6),
    XTVERSION, OSC 0/2 title through the `titulus` effect, replies
    through `responsum`. Identity strings join
    `AemulatorConfiguratio` (an API addition - Fran approves).
  - **B3 - the PTY pons** (module 008, its own library): header for
    Fran's approval, `pty_posix.c` (system headers only there),
    `pty_memoriae.c` (scripted child for headless host tests); real-
    child tests outside the default glob if they need a tty (printf
    arrives, `stty size` = what we set, resize, exit code, no zombie,
    exec failure distinct).
  - **B4 - a headless host and esctest:** core + PTY + a loop; fetch
    esctest (network: say when, pin the commit) and run its subset
    for B's surface with our core answering its queries.
  - **B5 - RELATIO.**
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
  cleanly is unmeasured - A0 reports the ratio. **Measured (A0, keyword
  heuristic):** Terminal.zig 428 tests, 361 assert observable state,
  172 ONLY externally observable (text, cursor, cells) - those convert
  mechanically; stream_terminal.zig 134, 117 byte-fed but mostly
  effects/queries (phase B), 26 observe the screen.
- `series_terminalis` treats C1 bytes (0x80-0x9F) as UTF-8
  continuations (Fran's choice for input); confirm that is right for
  program OUTPUT too (xterm with UTF-8 does the same) before A2.
  **Confirmed (A3):** Ghostty decodes UTF-8 first - a raw 0x80-0x9F
  byte is invalid UTF-8 (replacement), UTF-8-ENCODED C1 controls are
  ignored (stream.zig:744); ours prints U+FFFD for the raw byte and
  drops U+0080-U+009F as width-0 controls. Same behaviour.
- Where the loopback harness lives: **decided (A3, Fran):** the
  ludus_tessera suite.
- Whether `fons_6x8` (ASCII + Latin-1, tofu otherwise) is enough for
  the E bar; box drawing as sprites is module 010's plan.
- Recorded-session replay (vim, htop, zsh through the PTY) was not
  chosen as an oracle now; natural once B exists.
- Purity of the closure is promised, not yet checked mechanically:
  A1 should verify aemulator's include closure (legati inclusiones /
  nexus graph) holds no system header beyond `<string.h>`;
  `lib/piscina.c` itself includes `<stdlib.h>` and `<stdio.h>` - what
  stdio is used for there is unread.
- The fixed-block allocator question (module 006 open question 1):
  a pool carved from one piscina allocation is assumed; unverified
  against the memory apex discipline until C.
