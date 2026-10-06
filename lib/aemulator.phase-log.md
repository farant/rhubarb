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

