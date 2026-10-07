# aemulator phase-log

Plan: `project-specs/aemulator-plan.md` (interview with Fran
2026-10-06: name aemulator, loopback core first, fresh shell on
restart, fixed page pool, our parts + Ghostty as reference, Ghostty
tests as vectors + esctest/vttest, reflow deferred, TERM=xterm-256color,
decision X: keep the core wasm-able). Code-adjacent finds:
`lib/aemulator.worklog.md`. Terminal-planning module 006.

## A0 — the oracle before the code (2026-10-06, ac84c1ed)

**INTENTIO.** Vectors and their replayer exist before any emulator
code, so the core is written against a judge, not toward a feeling.

Built: `probationes/fixa/aemulator/vectores.stml` (steps: bytes in,
then text/cursor/cell/mode assertions; Ghostty-converted cases cite
`file:line @ 12752b2`, house cases say why) and
`probatio_aemulator_vectores.c` (static validation with no core; the
replayer judged against a fake core - one case must pass, four must
fail on one field each; the vectors through a `Probandum` vtable).
"Red until the core exists" became DEBTS BY NAME: a case may carry
`debitum="cause"`, a paid debt FAILS the suite. Text dump = Ghostty
`plainString` exactly. 172 of Terminal.zig's 428 tests check only
externally visible state.

## A1 — API and skeleton (2026-10-06, 5a3ca048)

**INTENTIO.** The API first (Fran approved it unchanged), then cells,
rows, printing through `runae`, controls, resize, the dump.

Built: opaque `Aemulator`, configuration with defaults, effects table;
12-byte cells (inline UTF-8, width kind, style index); rows through a
pointer array; Ghostty's print rules (pending wrap, spacer-head, half-
of-a-wide overwrite); UTF-8 carried across writes, invalid -> U+FFFD;
geometric capacity, zero allocation within it and in steady state
(measured). 15 vectors green, 24 named debts. 14 plants.

## A2 — the loopback surface (2026-10-06, 684ee183)

**INTENTIO.** Everything tessera emits: cursor addressing and moves,
erase, SGR with interned styles, DECTCEM, 1049, 2026, DECSC/DECRC.

Built: CSI dispatch with strict rejection (intermediates, stray ':',
unknown private markers - counted); Ghostty's erase rules incl. BCE
(erased and scrolled-in cells keep only the pen's background); a fixed
512-style table with mark-compact-renumber collection and degrade-to-
default when saturated; two screens. All 24 debts paid on the first
run and the suite failed once per debt until each was promoted. 42
vectors green, 17 plants.

## A3 — the loopback (2026-10-06, 5f14091c + e690e279)

**INTENTIO.** tessera draws, its bytes go through the emulator, every
cell must equal tessera's front buffer.

Built: `ludus_tessera/probationes/probatio_ludus_tessera_reditus.c` -
hand frames, resizes, 60 random frames, the vicus app; bytes, width,
full style, cursor; zero unknown sequences per session. Found and fixed
a tessera bug (full repaint cleared under the previous frame's pen).
C1 closed (Ghostty-identical). 8 plants.

## A4 — RELATIO: phase A, the loopback core (2026-10-06)

**Phase A is done: an emulator core that consumes everything tessera
emits, cell-exact, proven against Ghostty's own test cases and against
tessera's frames.** 1269 lines of core (`lib/aemulator.c`) behind a
156-line API; 43 vectors (35 from Ghostty @ 12752b2, 8 house), a unit
suite, the loopback; 7 commits; 46 plants, every one caught or
explained.

**What exists:**
- `include/aemulator.h` - opaque core, configuration with defaults,
  effects (reply, bell, title), cursor, cells (UTF-8 view, width kind,
  full `StilusTerminalis`), modes, unknown-sequence counter, the
  `plainString` dump. Approved by Fran before implementation.
- The VT surface tessera needs: print with Ghostty's width and wrap
  rules, CR LF BS HT BEL, CUP and every relative move, EL/ED with BCE,
  SGR through `stilus_applicare`, DECTCEM, 1049, 2026, DECSC/DECRC.
- Memory: everything sized at creation; resize grows capacity by half
  and never shrinks; the style table collects instead of growing; the
  steady state allocates nothing (measured with 1000 writes and 2000
  distinct colours).

**What the work found, by weight:**
1. **A semantic loopback finds what byte goldens cannot.** tessera's
   full repaint cleared the screen under the previous frame's SGR pen;
   with background-colour erase (xterm, Ghostty, Terminal.app) every
   blank cell kept a stale background after a resize. tessera's byte
   goldens pinned the very bytes that were wrong. Fixed in tessera with
   its own regression (5f14091c).
2. **Debts by name kept the suite honest between commits.** A red
   oracle would have blocked every gate until A2; silent skips would
   have hidden progress. Named debts did neither - and the suite forced
   each paid debt to be promoted.
3. **The harness is the first suspect.** Three of the session's false
   alarms were the test's fault, not the core's: CREDO writes its
   records into the measured piscina (A1); tessera's front buffer has a
   fixed 512 stride (A3); a primary-screen resize expectation that was
   simply wrong (A2). Each is in the worklog with how it was found.
4. **Plants still teach.** A2's "collection does not renumber cells"
   survived until a test let a style die BEFORE a surviving one (order-
   preserving compaction never moved anything otherwise). A plant that
   does not compile proves nothing (A2 P6). A plant that removes a
   check survives when the system is right (A3 P8) - the plant that
   breaks the system is the one that proves the check.
5. **Ghostty as a reference, not a source.** Its tests gave exact
   semantics - `plainString`'s blank rules, pending wrap, wide-char
   overwrite, erase boundaries, BCE, xterm's 1049 sequence - while the
   code stayed ours, built on series_terminalis, stilus_terminalis and
   runae, which were written for this direction and fit without change.
6. **Purity is a list, not a promise.** `aemulator.c` itself includes
   only `<string.h>`; its link closure does not stay pure -
   `piscina.c` (stdlib, stdio) and `stilus_terminalis.c` via
   `chorda_aedificator` (stdio). That is the exact work list if the
   wasm door is opened (decision X).

**Named limits (phase A):** zero-width codepoints are dropped
(graphemes are v2); no scroll regions, origin mode or insert mode; tab
stops fixed every 8; no scrollback (scrolled-off lines vanish; a resize
below the cursor drops top rows); no query answers, title, or
clipboard; no mouse/key mode tracking for the encoder; style table 512
with degrade-to-default when that many are visible at once; unknown
sequences counted, never interpreted.

**Next: phase B - shell-sufficient + the PTY** (re-planned in the plan
after this RELATIO). It needs a short interview first: the PTY seam
has real choices (openpty vs posix_openpt, sharing with `processus`,
child reaping).

## B0 — the PTY interview (2026-10-06, ee80c89f)

Fran: `openpty`; independent of `processus`; reaping by
`waitpid(WNOHANG)` + EOF; the window wakes on the master fd (exposed
by the pons; fenestra wake source in phase E). Decisions 11-14 in the
plan.

## B1 — the shell surface (2026-10-06)

**INTENTIO.** Everything a shell and line-oriented tools need beyond
tessera's output: scroll regions and what scrolls inside them,
insert/delete lines and characters, erase characters, tab stops, LNM.

Built: terminal-wide DECSTBM region (reset on resize); region-aware
IND/LF/wrap, RI, NEL, SU/SD; margin-aware CUU/CUD/CNL/CPL; IL/DL;
ICH/DCH/ECH with Ghostty's wide-character boundary rules; tab stops
(HTS, TBC, CHT, CBT, default every 8); LNM. Vectors 127 (115 Ghostty,
80 new); the loopback unchanged and green; region scrolling allocates
nothing. 16 plants: 14 caught, one exposed redundant code (removed),
one rule invisible until reflow (named).

## B2 — answers and effects (2026-10-06)

**INTENTIO.** The terminal answers what programs ask (DA1/2/3, DSR 5,
CPR, XTVERSION) and reports titles (OSC 0/2) through the effects that
A1 already declared; identity joins the configuration (Fran approved
the header change).

Built: configured identity (copied, defaults "aemulator 0.1"); the
replies byte-exact to Ghostty's tests; OSC 0/2 titles (empty too), OSC
1 consumed, others counted; over-long OSC dropped whole like Ghostty.
The replayer asserts replies and titles. 140 vectors (7 new from
Ghostty, 6 house); the first run caught a hand-counted reply length
(DA1 sent a NUL) - every fixed reply is now measured by strlen. 15
plants, all caught. Replies allocate nothing.

## B3 — the PTY pons (2026-10-06)

**INTENTIO.** A child process on a pseudo-terminal behind a vtable
(the test seam), so the host loop of B4 can run headless against a
scripted child and for real against a shell. Header approved by Fran
with its name (`pseudoterminale`) and never-blocking writes.

Built: `pseudoterminale` - posix bridge (openpty, new session,
controlling terminal, signal reset, environment, exec failure distinct
from exit 127, non-blocking master, reaping without handlers, close
that never leaves a zombie) and a memoriae bridge. Real children are
tested in the root suite (0.65 s). 19 plants caught; two only after
fixing the TESTS: `/bin/sh` quietly acquires a controlling terminal
(the check now execs `stty -f /dev/tty` with no shell), and "lowest
free fd" cannot see leaks (now: count all descriptors). The examen
lexicon learned the PTY calls (auspex-certified); glossary entries
moved no oratio pins. Details: `lib/pseudoterminale.worklog.md`.

## B4a — the host (2026-10-06)

**INTENTIO.** The one place where the core and the child meet, as its
own library (decision 17, Fran: "could help the other parts have
better structure"), so every front end - the headless tests now,
fenestra in E, vitrea later - only reads the screen and sends keys.

Built: `aemulator_hospes` - pulse model, owns a `Pseudoterminale`
passed in, a ring queue with a reply reserve, a read cap per pulse,
bell/title forwarded to the caller. It touches the child only through
the vtable, so it is pure like the core. Tests drive it with the
memoriae child, a test-made child whose every behaviour the test sets
(the vtable as seam paid off: wrap-around, reserve, chunking, "dead
but not drained" are all exact), and a real shell (a CPR query
answered by our core and read back by the shell). examen found an
unsigned underflow in the queue clip before any test did; a test now
pins it. 16 plants, 15 caught, one redundant-by-design guard named.

## B4b — esctest (2026-10-06)

**INTENTIO.** An outside oracle that drives a real program through the
whole stack - esctest2 runs as the child of `aemulator_hospes`, asks
the terminal questions, and checks the answers.

Built: DECSTR, DECRQCRA behind `lectio_schirmi` (decision 18: off by
default - Ghostty lacks it, iTerm2 disables it), `CSI 18 t`; the runner
(`tools/aemulator_esctest.sh`, ~80 s, deterministic). 567 tests: 216
pass, 306 fail, 17 skip, 28 known. Every failure has a cause taken from
the test's own bytes, and none is unexplained: they are the features
we have not built (left/right margins, colour queries, DECRQM, window
ops, protection, VT420 rectangles...) plus our deliberate Ghostty
identity in DA. The pinned table now plays the role debts played for
the vectors: any change - a new failure or a new pass - stops it. A
DECSTR plant showed why soft reset mattered: without it one test's
scroll region broke the tests after it.

## B5 — RELATIO: phase B, shell-sufficient + the PTY (2026-10-06)

**Phase B is done: a real shell runs through our own stack - a child on
a pseudo-terminal, a host joining it to the core, the core answering
the shell's questions - and an outside test suite (esctest2) drives
all of it.** Three libraries behind Fran-approved headers
(`aemulator` grown from 1269 to 2105 lines; `pseudoterminale` 739;
`aemulator_hospes` 352), 147 vectors (121 from Ghostty, 26 house), 567
esctest results pinned with causes, 80 plants over six commits
(ee80c89f B0, 690d9e8f, fe02809d, 59e042b6, a943ac06, cbb6c826).

**What exists:**
- The shell surface in the core: scroll regions and everything that
  scrolls in them, insert/delete lines and characters, erase
  characters, tab stops, LNM (B1); answers - DA1/2/3, DSR, CPR,
  XTVERSION, `CSI 18 t` - and OSC 0/2 titles through effects (B2,
  B4b); DECSTR; DECRQCRA behind `lectio_schirmi`, off by default
  (decision 18).
- `pseudoterminale` (B3): a vtable with a posix bridge (openpty, new
  session, controlling terminal, signal reset, environment, exec
  failure distinct from exit 127, writes that never block, reaping
  without handlers, a close that never leaves a zombie) and a memoriae
  bridge.
- `aemulator_hospes` (B4a, decision 17): the one place where core and
  child meet - a pulse model, a reply reserve, a read cap, pure. Every
  front end (headless tests now, fenestra in E, vitrea later) only
  reads the screen and sends bytes.
- An outside oracle (B4b): esctest2 @ 2798f12 as a child of the host;
  216 of 567 pass, every failure named by the feature it needs; the
  table changes only on purpose (`tools/aemulator_esctest.sh
  -probare`).

**What the work found, by weight:**
1. **A seam is worth its weight when the TEST owns the other side.**
   The vtable let a test define its own child - write quotas, endless
   output, "dead but not drained", chunked reads, counted waits - and
   every queue and loop property of the host became an exact
   assertion. The memoriae child alone could not have reached the
   reply reserve or the wrap-around.
2. **Tests can be repaired by the very thing they test around.**
   `/bin/sh` quietly acquires a controlling terminal, so a shell-based
   check passed with TIOCSCTTY removed; "lowest free fd" cannot see a
   leak above a hole. Both found only because the plants survived.
   Rule kept: test the child's environment with no shell in between.
3. **i32 is unsigned, and examen knows it.** The host's queue clip
   (`limes - mensura`) underflowed once replies filled the reserve;
   examen's "comparatio vana" on the neighbouring checks led to it
   before any test did. A test now pins it.
4. **An outside oracle needs the boring parts first.** esctest could
   not run a single test until the core answered `CSI 18 t`, and its
   results meant nothing until DECSTR stopped one test's scroll
   region leaking into the next. Its cell checks exist only through
   DECRQCRA - which is why that went in behind a flag, not by default.
5. **Name every failure, or the table lies.** 306 esctest failures
   were attributed from each test's own bytes; none is unexplained,
   and the few that looked like B1 bugs all used left/right margins.
   The pinned table now does for the whole emulator what named debts
   did for the vectors.
6. **Gates owed rarely are gates nobody has run.** B3's lexicon edit
   was the first change since dispositio D0 to owe `examen-corpus`;
   it found the Clay glue rejected and unpinned. Pinned with its
   cause; nothing in B was at fault, but the commit was the one that
   paid.

**Named limits (phase B):** no scrollback (phase C); no origin mode,
insert mode, reverse wrap, alt screens 47/1047/1048, SCOSC/SCORC,
HPR/VPR, REP, DECALN, RIS; no mode or colour queries (DECRQM, DECRQSS,
OSC 4/10/11); no left/right margins or VT420 rectangle/column
operations; no protected areas; no window operations beyond `18 t`;
DA answers Ghostty's identity, not xterm's; key encoding and
mouse/paste modes are not wired (phase D); the host has no front end
yet (phase E). A title longer than 2046 bytes is dropped (Ghostty:
2047).

**Next: phase C or D - see the re-plan in the plan (Fran decides the
order).**

## C2 — history pages and scroll-off (2026-10-06)

**INTENTIO.** Rows leaving the primary screen are kept, in pages that
own their styles, up to the byte limit; the screen's style collection
never touches history.

Built: Ghostty-style pages (64 KiB of cells, one width, own style
table), copied rows with re-interned styles, a ring of pages that
grows then recycles, the history and view reads, one dump routine over
absolute rows (live, view, history+live). Ghostty's own tests moved the
scroll-off rule: a region starting at the top row feeds history, not
only the full screen - and DL never does, so the scroll function now
takes an explicit flag. Five of fourteen plants survived the first
round, each a gap in the tests (a narrow history row read past its
end landed on an empty neighbour; the alt-screen vector's primary had
no history to leak); all caught after.

## C3 — the view (2026-10-06)

**INTENTIO.** The view behaves as decided (21): it stays on what you are
reading while output arrives, it survives eviction by clamping, and a
keystroke through the host brings it back.

Built: the replayer gained a view action and view assertions; five of
Ghostty's viewport tests converted directly; the host's view move and
snap-on-input; the clamp under eviction tested at page granularity;
the full 10 MB measured - once history is at its limit, output
allocates nothing. Nine plants caught.

## C4 — ED 3 and resize (2026-10-06)

**INTENTIO.** Resize behaves like Ghostty without reflow, with history
in the picture: a shrink keeps what matters, a grow brings history
back when the cursor is at the bottom, and `clear` (ED 3) empties it.

Built: blank-row trimming before pushing into history on shrink (the
old code cut the bottom and lost text below the cursor), the pull on
grow with styles re-interned after the new size is set, ED 3 that
keeps pages for reuse. The replayer can resize; eight of Ghostty's
no-reflow resize tests converted. A plant on the pull's styles
survived because page and screen tables happened to give red the same
index - the test now forces the tables apart and compares against a
live red cell.

## C5 — RELATIO: phase C, scrollback (2026-10-06)

**Phase C is done: lines that leave the primary screen are kept, up to a
byte limit, in pages that own their styles; a view can scroll through
them without disturbing the program; resize and `clear` behave like
Ghostty without reflow - and once history is full, output allocates
nothing.** The core grew from 2105 to 2668 lines (header 239), the host
to 370; 179 vectors (142 from Ghostty, 37 house); 34 plants; five
commits (c59ae50d C0, 6c0f313b C1, 800724f1 C2, 5180678e C3, d67d8c58
C4); the esctest table unchanged throughout (216 pass, every failure
still named).

**What exists:**
- History: 64 KiB pages of one width with their own 128-entry style
  table, a ring that grows to the limit (default 10 MB, decision 19)
  and then recycles the oldest page (decision 20); rows are copied in
  and re-interned, so the screen's style collection never walks
  history.
- The scroll-off rule from Ghostty/xterm: a region that starts at the
  top row of the primary screen feeds history on index and SU; DL, a
  lower region and the alternate screen never do; ED 2 keeps nothing
  (no prompt marks).
- The view: stays put under output (decision 21), clamps on eviction,
  clamps both ways when moved; the host moves it and snaps it to the
  bottom on accepted input.
- Resize the Ghostty way (shrink trims trailing blank rows first, grow
  pulls history back only with the cursor on the bottom row); ED 3
  empties history and keeps the pages.
- Three plain dumps over absolute rows (live, view, history+live),
  matching Ghostty's three test reads; the replayer can scroll the
  view and resize.

**What the work found, by weight:**
1. **The other implementation's TESTS changed the design, not just
   checked it.** The scroll-off rule was "full screen" in the plan;
   Ghostty's `index bottom of scroll region creates scrollback` made
   it "region at the top row" - and showed DL needs an explicit no.
   Its resize code turned "cut the bottom" (which lost text below the
   cursor) into "trim blanks, then push".
2. **Ownership decides cost.** Pages owning their styles is what makes
   the screen's collection O(screen) and eviction free; the
   alternative (indices into one table) would have made a colour flood
   rewrite the whole history. The design was settled before code, with
   Fran.
3. **Order matters where collection can run.** Pulling history back
   re-interns styles, which may collect, which walks the current size -
   so the pull happens after the new size is set.
4. **Coincidence hides missing work.** A pulled row "kept" its colour
   only because both tables had given red the same index; the
   evidence became a comparison with a live red cell. Five C2 plants
   survived their first round for kindred reasons (empty neighbours,
   no history to leak).
5. **A gate that fails on untouched code is telling you about the
   environment.** Two ludus_tessera tests never closed their temporary
   volumes; after 100 runs the helper ran out of names. Fixed at the
   source; the lesson is in memory.

**Named limits (phase C):** no reflow (decision 7 - history rows keep
their width, a resize truncates); ED 2 never saves the screen (Ghostty
does at an OSC 133 prompt); no line-count limit, bytes only; no
compression of idle history (Ghostty has it); a single row needing
more than 127 styles degrades the rest to the default style; no
selection, search or absolute history addressing (feature 013).

**Next:** see "Re-plan after C5" in the plan - phase D (full-screen
programs) or a thin slice of E (a shell in a window) first.

## E2 — terminale, headless (2026-10-07)

**INTENTIO.** The terminal as a ludus app, built and checked with no
window: the host's view drawn cell by cell, keys and scroll through the
dispatcher, resize to the child, a real shell answering.

Built: `lib/terminale.c` on the approved header (private context keeps
it unchanged); key pairing that matches how real windows deliver keys;
xterm colours, inverse, bold-bright, cursor only at the live bottom.
Checked three ways: the frame's drawing commands, pixels read back from
the PNG, and my eyes on the PNG - and the third caught what the first
did not: a shared header documented the wrong RGBA byte order; the
command-level test had passed under the same wrong belief. 12 plants.

## E3 — the apps (2026-10-07)

**INTENTIO.** `terminale` as runnable apps - a window and a terminal
twin - so a real shell can be seen and typed into.

Built: both mains with their own loops (the shared loops would sleep
on the dispatcher's settle time and the terminal one ends on Ctrl-C);
the window draws only on change. Smoke: the window's last frame shows
the scripted sample exactly; the twin ran nested in our emulator and
produced the same text. Next: Fran's look at `./apps/terminale/
terminale.sh` with a real login shell.

## E4 — RELATIO: the thin E slice, a shell seen (2026-10-07)

**The slice is done: `terminale` runs a real login shell in a window
drawn entirely by the house stack - pseudoterminale, aemulator_hospes,
the emulator core with history, ludus, the 6x8 font - and in a
terminal twin. Fran: "it seems like it is all working! very cool".**
New: `lib/terminale.c` (549 lines) behind a 92-line approved header,
the two apps (369 lines), frame screenshots for every ludus app; tests
probatio_terminale (400) and probatio_ludus_imago (147); 18 plants.
Commits: 4c468a8c (screenshots), 152a2e3f (window smoke), a956a8dd
(pictor note), 6f47be82 E0, 2dc98f10 E1, 34142c3b E2, 87d406e6 E3,
c43cb5fd (Fran's look).

**What exists:**
- Screenshots of any ludus app's frame (`-imago <path>` on pictor,
  scriba, vicus, terminale) and HEADLESS rendering (a bare tabula +
  ludus_quadrum) - byte-identical to the real window's frame.
- `terminale`: the host's view drawn per cell (xterm colours, cube,
  grey ramp, inverse, bold-bright, cursor only at the live bottom);
  keys paired with their text for the codificator; scroll with a
  remainder; resize in the pulse; an in-memory state repository.
- Two apps with their own loops (draw only on change; Ctrl-C to the
  shell; the shell exiting closes the window).

**What the work found, by weight:**
1. **Looking is a test.** The first terminal screenshot showed a blue
   background as near-white. `mandatum.h` documented RGBA mandate
   colours as 0xRRGGBBAA; both rasterizers and every producer use
   frame-buffer packing. The command-level test had passed because it
   shared the wrong belief. Pixels are now asserted from the PNG - and
   the screenshot tool that made this visible did not exist the day
   before (Fran asked for it).
2. **Match the source's shape, not the event's.** Real windows send
   key down, its text, key up; the codificator wants (key, text) as
   one series. Pairing them - a pending key press - is the whole key
   design; encoding events one by one types each letter twice.
3. **A terminal can test a terminal.** With no TTY in the tool shell,
   the terminal twin ran as the child of our own headless host; its
   screen was read back cell-exact.
4. **Read the code, not the comment.** E1 concluded scroll events were
   dropped from an empty view method and a stale comment; the events
   are translated earlier. Corrected in E2 - before anything was built
   on the wrong belief.
5. **Shared loops encode one app's assumptions.** Both run loops sleep
   on the dispatcher's settle time and the terminal one ends on Ctrl-C;
   a terminal needs neither, so each main owns its loop (decision 25).

**Named limits (slice):** no window title from OSC 0/2 yet; no
selection or copy; full-screen programs wait for phase D (modes); the
16 ANSI colours are xterm's, not themed; idle polling at 16 ms (the fd
wake source, decision 14, comes later); no vicus tab kind yet;
pictor's initial canvas view is still an open question (its worklog).

**Next:** merge secunda into main (Fran's timing), then phase D.

## D1 — quick wins (2026-10-07)

**INTENTIO.** The small, common sequences full-screen programs send:
relative moves, repeat, ANSI cursor save, the older alt-screen modes,
the alignment test, full reset.

Built: HPR/VPR, REP, SCOSC/SCORC, 47/1047/1048, DECALN, RIS, each from
Ghostty's code and tests. esctest 216 -> 245 with no regressions. Two
plants survived for a reason worth more than the features: STML reads
an empty attribute as absent, so every `textus=""` in the vectors had
never been checked. An explicit empty marker fixed it; the nine
assertions it revived all held.

