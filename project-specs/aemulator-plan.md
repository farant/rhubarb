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
15. **Name `pseudoterminale`** (Fran, B3 header review 2026-10-06):
    `include/pseudoterminale.h`, `Pseudoterminale` vtable (legere,
    scribere, amplitudo, finitus, fossa, claudere), posix and
    memoriae bridges in one header (no system headers in it).
16. **`scribere` never blocks:** it returns the bytes accepted (0..n,
    -1 error); the host keeps the remainder. A large paste into a
    child that is not reading cannot freeze the window.
17. **The host is its own library, `aemulator_hospes`** (Fran, B4
    header review 2026-10-06): the one place where core and child
    meet. Pulse model (the front end owns its loop); the host OWNS a
    `Pseudoterminale` passed in (posix or memoriae - the test seam);
    `AemulatorConfiguratio` embedded (the host takes the reply effect,
    bell/title pass through); an outgoing queue whose last sixteenth
    is reserved for replies (a paste never starves a query); a read
    cap per pulse. It touches the child only through the vtable, so
    it is as pure as the core. Key encoding joins in phase D,
    additively.
18. **DECRQCRA behind a flag, off by default** (Fran, B4b
    2026-10-06): `AemulatorConfiguratio.lectio_schirmi` (default
    FALSUM - Ghostty has no DECRQCRA, iTerm2 ships it disabled; a
    program could read the screen back). The esctest runner turns it
    on; it is esctest's only way to read cells.

### Phase C decisions (Fran, C0 interview 2026-10-06; Fran chose C
before D's quick wins: C is the last structural change in the core)

19. **Default scrollback limit 10 MB per terminal** (bytes, decision
    4; ~11k lines at 80 columns with 12-byte cells), configurable.
20. **The pool grows, then recycles:** pages come from the piscina as
    history fills, up to the limit; from then on the oldest page is
    recycled and nothing is allocated (a short-lived tab never pays
    the full limit).
21. **Scrolled up, the view stays put** while output arrives
    (Ghostty's default); a keystroke returns it to the bottom - that
    is the HOST's job (`aemulator_hospes_scribere`), the core never
    sees keys. If the lines in view are evicted, the view clamps to
    the oldest remaining line.
22. **ED 3 (`CSI 3 J`, clear scrollback) in C** - macOS `clear` sends
    it.

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
    B1 as built: region (terminal-wide, reset on resize) with
    region-aware index/wrap/LF, RI, NEL, SU/SD (multi-parameter SD =
    xterm mouse highlight -> unknown), margin-aware CUU/CUD/CNL/CPL,
    IL/DL (outside the region: nothing; moved rows lose soft-wrap;
    cursor to column 0), ICH/DCH/ECH with Ghostty's wide-character
    boundary rules (BCE everywhere), tab stops (default every 8,
    reset on resize; HTS, TBC 0/3, CHT, CBT), LNM (mode 20, read
    through `aemulator_modus`). 127 vectors green (115 Ghostty: 80
    new for B1). 16 plants: 14 caught; ECH's extra extension was
    REDUNDANT (the boundary split already clears the same cells) and
    was removed; DCH/IL/DL soft-wrap resets are invisible until
    reflow exists (named).
  - **B2 - answers and effects:** DA1/DA2, DSR 5 and CPR (6),
    XTVERSION, OSC 0/2 title through the `titulus` effect, replies
    through `responsum`. Identity strings join
    `AemulatorConfiguratio` (an API addition - Fran approves).
    B2 as built: `titulus`/`versio` in the configuration (default
    "aemulator" / `AEMULATOR_VERSIO` "0.1", copied at creation,
    NIHIL = default). DA1 `ESC[?62;22c` (a non-zero parameter =
    unknown), DA2 `ESC[>1;0;0c` (likewise), DA3 `DCS !|00000000 ST`,
    DSR 5 `ESC[0n`, DSR 6 CPR 1-based (during pending wrap: the last
    column), other DSR = unknown; XTVERSION `DCS >|titulus versio
    ST`; OSC 0/2 -> `titulus` effect (empty too), OSC 1 consumed
    silently, other OSC counted; an OSC body longer than the lexer's
    2048 bytes is dropped whole and counted (Ghostty drops it too;
    named divergence: Ghostty keeps titles up to 2047 bytes, we up to
    2046 because our limit includes the `2;`). No callbacks = silent.
    Replies and titles allocate nothing. 140 vectors (122 Ghostty: 7
    new); the replayer asserts `responsum`, `tituli`, `titulus`.
    15 plants, all caught.
  - **B3 - the PTY pons** (module 008, its own library): header for
    Fran's approval, `pty_posix.c` (system headers only there),
    `pty_memoriae.c` (scripted child for headless host tests); real-
    child tests outside the default glob if they need a tty (printf
    arrives, `stty size` = what we set, resize, exit code, no zombie,
    exec failure distinct).
    B3 as built: `include/pseudoterminale.h` (one header; aedilis
    convention -> `pseudoterminale_posix.c`, `<aedilis corpus>` adds
    `lib/pseudoterminale.c` = defaults, error names, memoriae bridge).
    Child: setsid, TIOCSCTTY, slave on 0/1/2, signals to SIG_DFL,
    chdir, environment changes ("NAME=value" / "NAME"), execvp; exec
    and chdir failures through a CLOEXEC pipe (EXEC, distinct from a
    real exit 127). Master CLOEXEC + O_NONBLOCK; legere by poll (0 =
    nothing, -1 = EOF/EIO); finitus by waitpid WNOHANG (code or
    signal); claudere SIGHUP -> close -> 100 ms -> SIGKILL, never a
    zombie. Real-child tests run in the ROOT suite (openpty needs no
    terminal of its own): 0.65 s. 19 plants, all caught after two
    test fixes (a shell repaired the missing controlling terminal; a
    "lowest free fd" check missed leaks). Examen lexicon + auspex +
    glossary learned the PTY names.
  - **B4 - a headless host and esctest:** core + PTY + a loop; fetch
    esctest (network: say when, pin the commit) and run its subset
    for B's surface with our core answering its queries.
    B4a as built (decision 17): `aemulator_hospes` - pulse = send
    queue, read (wait only before the first byte; cap per pulse),
    core, send replies, check exit; `finitus` = EOF AND reaped (a dead
    child's buffered output is still drained). Ring queue: user input
    up to capacity - 1/16, replies up to the whole; a reply that does
    not fit even the reserve is dropped and counted. Bell/title
    forwarded with the CALLER's datum. Pure (string.h only). Tests:
    memoriae child, a test-made "obstinate" child (write quota, endless
    output, EOF/death flags, chunking, wait counting) and real
    /bin/sh (output, a CPR round trip read back by the child, resize,
    exit code). 16 plants: 15 caught, the loop-condition cap survives
    by construction (each read also asks for at most the remainder).
    Found by examen: `limes - mensura` underflowed (i32 is UNSIGNED)
    when replies had filled the reserve.
    - **B4b - esctest** as built: `../esctest2` @ 2798f12 (Thomas
      Dickey's maintained fork; GPL-2.0 - run as a child, never
      vendored). Core: DECSTR (xterm: cursor visible, region full,
      pen and DECSC reset; cursor stays; DECAWM stays ON), DECRQCRA
      (decision 18; 16-bit codepoint sum, empty = space as xterm >=
      334, wide tail = 0), `CSI 18 t` (Ghostty csi_18_t; esctest's
      reset needs the size). Runner: `tools/aemulator_esctest.c` +
      `tools/aemulator_esctest.sh [-include RE] [-probare]`
      (--expected-terminal=xterm --xterm-checksum=334
      --max-vt-level=4), ~80 s, deterministic (two full runs equal).
      Result 567: 216 pass, 306 fail, 17 skip (VT5), 28 esctest-known;
      every failure carries a cause in
      `probationes/fixa/aemulator/esctest.tsv`, attributed from the
      test's own bytes (`--test-case-dir`): left/right margins 82,
      colour OSC 47, DECRQM 32, window ops 29, protection 24, VT420
      rectangles 14 / columns 11, reverse wrap 10, DECDSR 10, alt
      screens 47/1047/1048 9, DECRQSS 9, HPR/VPR 6, DA identity 4
      (Ghostty's, decision 5 - esctest expects xterm's), SCOSC 4,
      DECCOLM 3, REP 3, DECALN 2, RIS 2, XTSAVE 2, DECID, MoreFix,
      IRM. None unexplained. `-probare` fails on ANY change (new
      pass = promote). A tool, not a gate (like frigida_probare).
  - **B5 - RELATIO.** Done 2026-10-06: `lib/aemulator.phase-log.md`
    (B5).
- **Re-plan after B5 (PROPOSED - Fran decides order and scope).**
  The esctest table (`probationes/fixa/aemulator/esctest.tsv`) ranks
  what is missing by how many of its tests need it:
  - quick wins, small and common: HPR/VPR (6), REP (3), SCOSC/SCORC
    (4), alt screens 47/1047/1048 (9), DECALN (2), RIS (2) - ~26
    tests;
  - modes programs set: origin mode, insert mode (D already), reverse
    wrap (10);
  - queries modern programs ask: DECRQM (32 - neovim, OpenTUI), colour
    OSC 4/10/11 (47 - theme detection), DECRQSS (9), DECDSR (10);
  - VT420 and beyond, perhaps never: left/right margins (82),
    rectangle and column operations (25), protected areas (24),
    window operations (29), DECCOLM (3).
  Open questions: C (scrollback) before D as planned, or D's quick
  wins first? Which of the last group is "never"?
- **C - scrollback.** The fixed page pool (Decision 4), viewport
  scrolling, byte limit, eviction; reflow stays deferred (Decision 7).
  - **C0 - interview** - done 2026-10-06 (decisions 19-22).
  - **C1 - API** (header for Fran's approval): scrollback limit in the
    configuration; history length; viewport (scroll by lines, to top,
    to bottom, where am I); reading a history row (cells + soft-wrap
    flag) and the plain-text dump over history; the host's snap on
    keystroke.
    C1 as built (Fran approved unchanged): `historia_octeti` (0 = no
    history, default 10 MB, rounded up to whole pages);
    `aemulator_historia`, `aemulator_visus`, `aemulator_visum_movere`
    (s32 delta, clamped), `aemulator_visus_cellula`,
    `aemulator_visus_involuta`, `aemulator_visum_effundere`,
    `aemulator_historiam_effundere`; host `aemulator_hospes_visum_movere`
    and snap-on-input. The old reads stay on the LIVE screen (vectors
    and loopback untouched). Declared now, implemented in C2-C3.
  - **C2 - the pool and scroll-off:** primary-screen rows that leave
    the top of a FULL-SCREEN scroll (LF/IND at the bottom, SU, ED 2's
    xterm behaviour if any) enter history; a region scroll, IL/DL and
    the alternate screen never do. Fixed-size pages from the piscina
    up to the limit, then recycle (decision 20); styles referenced by
    history survive style-table collection. Vectors from Ghostty's
    scrollback tests first.
    C2 as built: the scroll-off rule is Ghostty's/xterm's - a region
    that STARTS AT THE TOP ROW of the primary screen (not only the
    full screen; no left/right margins exist) feeds history on index
    and SU; DL never (explicit flag on `regionem_sursum`); resize
    shrink feeds it too (its scroll passes the flag). ED 2 does not
    (Ghostty only does so at an OSC 133 prompt - we have no prompt
    marks). Pages: 64 KiB of cells (width-dependent line count), one
    width per page, own 128-entry style table (rows are COPIED in,
    styles re-interned; a full table moves the row to a fresh page
    once, then degrades to the default style); a ring of page
    pointers allocated at creation; eviction per page, oldest first.
    The view reads (`visus`, `visum_movere`, `visus_cellula`,
    `visus_involuta`, `visum_effundere`) went in with C2 - the style
    test needed them. 161 vectors (13 new: 8 Ghostty, 5 house + 1);
    section XVII (eviction counts per page, widths, styles across
    collection, table overflow, soft-wrap, alt screen). 14 plants,
    all caught after test fixes (five survived the first round).
  - **C3 - viewport and eviction:** scroll by lines/top/bottom; stays
    put under output (decision 21); clamp on eviction; steady state
    and floods allocate nothing once full (measured).
    C3 as built: the view stays put under output (each line entering
    history while scrolled up moves the view one up), clamps when its
    lines are evicted (two pages: view on line 0, page evicted -> the
    view lands on the oldest kept line, then stays there; one page:
    eviction empties history and the view returns to the bottom),
    clamps both ways on `visum_movere`. Host:
    `aemulator_hospes_visum_movere` (next pulse reports `mutatum`;
    the field is now `repingendum`), accepted input snaps the view to
    the bottom, refused input does not. Full limit measured: 10 MB at
    80x24 = 160 pages x 68 lines; past it, 3000 styled lines with wide
    characters allocate nothing. 167 vectors (6 view: 5 Ghostty). 9
    plants, all caught (P3 replanted - the first did not compile).
  - **C4 - ED 3 and resize:** clear scrollback; resize without reflow
    the Ghostty way (rows pushed off by a shrink enter history, a grow
    pulls them back).
    C4 as built (Ghostty PageList resizeWithoutReflow): shrinking
    trims trailing BLANK rows below the cursor first (never the
    cursor's row; a background colour is not text), then the rest
    pushes the top into history - text below the cursor survives,
    the cursor rises with its line (before: the bottom was simply
    cut). Growing pulls history back ONLY with the primary cursor on
    the bottom row, after the new size is set (style re-interning
    may collect, and collection walks the current size); pulled rows
    get their styles re-interned and their soft-wrap back. ED 3
    empties history (primary only), keeps the pages for reuse, view
    to the bottom. 179 vectors (12 new: 8 Ghostty no-reflow resize,
    4 house); replayer resize step; section XIX. 11 plants, all
    caught after two test fixes (three replants did not compile; the
    pulled style matched by index coincidence - now compared with a
    live red cell).
  - **C5 - RELATIO** (and the esctest table re-run). Done 2026-10-06:
    `lib/aemulator.phase-log.md` (C5); esctest unchanged (216 pass).
- **Re-plan after C5 (PROPOSED - Fran decides).** The core is now
  shell-ready (B) with history (C). Two ways forward:
  - **D first (as planned):** full-screen programs - origin and insert
    modes, alt screens 47/1047/1048, SCOSC/SCORC, HPR/VPR, REP, DECALN,
    RIS (the esctest quick wins), DEC special graphics, key/mouse/
    paste/focus/DECCKM/keypad modes handed to
    `codificator_terminalis`, mode and colour queries (DECRQM, OSC
    10/11); the bar: vim, less, htop.
  - **A thin slice of E first:** the first time a shell is SEEN - a
    vicus kind or a pictor-style window that draws
    `aemulator_hospes`'s view with the house alphabet, keys through
    `codificator_terminalis` (plain keys + Enter/Backspace/arrows),
    wake on the PTY fd or poll in the frame (decision 14 / module 008).
    No render-state module yet (draw the whole view when `mutatum`).
    Risk: D's key/mouse modes then land on a front end already in
    use - which is also how they get tested for real.
- **E-thin - a shell SEEN** (Fran 2026-10-06: E slice before D; it
  became checkable once frame screenshots existed - 4c468a8c: headless
  frames are byte-identical to the real window's).
  Decisions (E0 interview):
  23. **The app is `terminale`** (`apps/terminale/`); the libraries
      stay `aemulator` / `aemulator_hospes`.
  24. **Standalone app first**, window and terminal twins like pictor
      and scriba; a vicus tab kind comes after, reusing the component.
  25. **Poll in the frame** (module 008's first option): each frame
      pulses the host with a short wait; the fd wake source (decision
      14) comes later, measured against this.
  Tasks:
  - **E1 - design read and API:** how pictor/scriba compose
    (applicatio, dispensator, canon, figures), then the terminale
    component + figure (cells from the host's VIEW via Mandata and
    fons_6x8, styles through the theme, the cursor), keys through
    `codificator_terminalis` (text, Enter, Backspace, Tab, arrows,
    Ctrl-letters), trackpad scroll -> `aemulator_hospes_visum_movere`;
    headers for Fran's approval.
    E1 as built (Fran approved the header unchanged, 2026-10-06; it
    lands with its implementation in E2 - its corpus file does not
    exist yet): ONE library `terminale` (include/terminale.h,
    lib/terminale.c): `TerminaleApplicatio`, `terminale_argumenta`
    (-fumus: scripted /bin/sh; else `$SHELL -l`),
    `terminale_applicatio_aedificare` (in-memory insula repository -
    `insula_repositorium_creare` takes STML strings, nothing durable),
    `terminale_pulsare` (fit size from the surface, then pulse the
    host), `terminale_claudere`. Read from scriba/pictor: `componere`
    pure, figures read live data through a context pointer (here the
    host's view), actions take keys. Each main owns its loop (the
    shared window loop sleeps for the dispatcher's gesture-settle
    time - a different meaning). Keys one event at a time through
    `codificator_terminalis` (a TEXTUS event encodes as its text; a
    lone key press as itself). Colours: theme defaults, xterm 0-15,
    6x6x6 cube, grey ramp, RGB. (E1 claimed fenestra drops scroll
    events because its `scrollWheel:` view method is empty - WRONG,
    corrected in E2: scroll is translated in fenestra's polling path
    (NSEventTypeScrollWheel -> _rotulam_implere -> EVENTUS_MUS_ROTULA)
    before the view is asked; the empty method is simply unused. The
    `ludus_fenestra.h` comment that suggested it is stale.)
  - **E2 - component and figure, headless:** memoriae child and a real
    `/bin/sh`; keys via `manus_ludus`; checks cell-exact through the
    tessera target AND pixel-level through headless PNGs (looked at).
    E2 as built: `lib/terminale.c` - private context (pending key,
    scroll remainder, scratch piscina) so the approved header stayed
    unchanged; keys: a key press PENDS, its following text is encoded
    WITH it (codificator pair), anything else flushes it alone
    (`manus_ludus_scribere` warns a consumer reading both sees the
    letter twice); scroll: dy accumulates, whole cell rows move the
    view; figure: per cell, background only when not default, inverse
    swaps, bold brightens 0-7, cursor only at the bottom of the view.
    In-memory repository WITHOUT a canon (dispatcher's focus and
    surface writes land). Tests (probatio_terminale): mandata per
    cell, PIXELS read back from the PNG, keys (pair once, Enter,
    Ctrl-C), resize to the child, scroll + remainder + no cursor in
    history, real /bin/sh round trip. 12 plants caught.
    FOUND BY LOOKING: the blue background rendered as transparent
    yellow - `mandatum.h` documented an RGBA mandate colour as
    0xRRGGBBAA, but both rasterizers (delineare_mandata, tessellatio)
    and every existing producer use frame-buffer packing
    (`color_ad_pixelum`). The comment was the lie; corrected. The
    command-level test had passed with the wrong assumption - pixels
    are now asserted from the PNG.
  - **E3 - the apps:** `terminale.c` (window) + `terminale_terminalis.c`
    + scripts; one announced smoke run with `-imago`; Fran's look.
    E3 as built: `apps/terminale/terminale.c` (window, 80x24 cells at
    scale 2; own loop: pulse the host without waiting, wait for window
    events up to 16 ms, draw ONLY when something changed - an idle
    shell draws nothing; the shell exiting closes the window, as
    Ghostty; Ctrl-C goes to the shell) and `terminale_terminalis.c`
    (terminal twin: the tessera loop copied with the host pulse, no
    Ctrl-C exit, read wait capped at 16 ms, initial size via a resize
    event), scripts `terminale.sh` / `terminale_terminalis.sh`
    (AEDIFICARE_SOLUM builds only). Env TERM=xterm-256color,
    COLORTERM=truecolor, TERM_PROGRAM=terminale. Smoke (announced, in
    the approved E3): `terminale.sh -fumus -imago` -> 60 frames, exit
    0, the PNG shows the scripted sample exactly (looked at 4x). The
    twin, having no TTY here, was run NESTED inside our own emulator
    (a scratch runner on aemulator_hospes): its screen held the same
    sample - our terminal inside our terminal. Limits: no window title
    from OSC yet, full-screen programs wait for phase D.
    Fran's look (2026-10-07): "it seems like it is all working! very
    cool".
  - **E4 - RELATIO.** Done 2026-10-07: `lib/aemulator.phase-log.md`
    (E4). Next: secunda -> main merge (Fran's timing), then phase D.
- **D - full-screen v0** (D0 interview, Fran 2026-10-07):
  26. **VT420-and-beyond is LATER, not never:** left/right margins,
      rectangle/column operations, protected areas, most window ops,
      132-column mode - their esctest rows get the cause "later
      (phase F+)" so the table separates not-yet from missing.
  27. **Kitty keyboard protocol in D:** the core keeps the flag stack
      (CSI > u push, CSI < u pop, CSI = u set, CSI ? u query);
      `codificator_terminalis` already encodes kitty keys.
  28. **Colour queries answered with theme colours:** OSC 10/11
      (foreground/background) and OSC 4 (palette) from colours the
      host passes to the core.
  29. **Done bar:** headless checks (vectors, esctest table), then
      Fran uses vim, less and htop in terminale, and a vttest session
      together (vttest: Dickey's interactive VT tester - network
      fetch approved earlier; announce and pin).
  Tasks:
  - **D1 - quick wins:** HPR/VPR, REP, SCOSC/SCORC (CSI s/u), alt
    screens 47/1047/1048, DECALN, RIS; Ghostty vectors first; esctest
    rows promoted.
  - **D2 - modes in the core:** origin (DECOM), insert (IRM), reverse
    wrap (45/1045), DECCKM, keypad (DECKPAM/DECKPNM), mouse (9, 1000,
    1002, 1003, 1006, 1015), bracketed paste 2004, focus 1004, DECRQM;
    the API to hand modes to the host (header for Fran's approval).
  - **D3 - DEC special graphics:** G0/G1 designation (ESC ( 0 ...),
    SO/SI, line drawing.
  - **D4 - kitty keyboard flags** (decision 27) + its query.
  - **D5 - colour queries** (decision 28; configuration addition for
    Fran's approval).
  - **D6 - terminale wiring:** modes -> codificator (DECCKM arrows,
    keypad, kitty), mouse reporting from window mouse events, bracketed
    paste, focus events, window title from OSC 0/2.
  - **D7 - the bar:** esctest table re-pinned (causes "later" per
    decision 26), vttest fetched and pinned, Fran's session (vim,
    less, htop, vttest).
  - **D8 - RELATIO.**
- **D (old sketch).** Origin and insert modes, DEC special
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
