# tessera — interview (raw Q&A)

*2026-07-02. Context: during the silva Phase 6→7 boundary discussion, Fran
proposed replacing "compile the amalgam inside solarium's build" with a real
consumer: an ANSI-only (xterm-only) ncurses-replacement library — reusable,
amalgamated like silva — powering a TUI app for interactive silva code
exploration ("a silva REPL or terminal almost"). The Phase 7 debts half and
feeding solarium's 91 files through silva as corpus were both confirmed as
still wanted. This interview pins the library (named tessera during the
interview); the explorer app gets its own interview later.*

Format: question → chosen answer → the tradeoff context that was on the
table. All "(Recommended)" markers were the interviewer's; Fran took every
recommendation, with free-text additions noted.

## Round 1 — foundational shape

**Q: Development and distribution form?**
**A: Modular + amalgamate.** Develop as a normal rhubarb lib in its own
folder with probationes, vendoring piscina/chorda internally; ship a
generated tessera.c + vanilla tessera.h pair like silva. Reuses the proven
amalgamator pattern and pressure-tests making that tool generic.
(Rejected: header-only single file — no internal rhubarb libs without
inlining them, multi-TU duplication; plain lib only — defers the reuse goal.)

**Q: Global state (termbox tb_init style) or context struct?**
**A: Context struct.** tessera_aperire() returns a context; all calls take
it. The test backend becomes "a context wired to memory buffers instead of
the tty." Matches silva's long-lived-context philosophy.
(Rejected: global singleton, hybrid.)

**Q: Memory policy — the wrinkle is SIGWINCH resizing the grids at
unpredictable times, awkward for pure arenas?**
**A: Caller piscina + max-size grids.** Grids allocated ONCE at a fixed
maximum; resize just changes active dimensions, never reallocates. Wastes a
few MB; dead simple; arena-friendly.
(Rejected: realloc-on-resize into arena garbage; internal dedicated piscina.)

**Q: Color commitment? (iTerm2/kitty/alacritty/tmux do truecolor; macOS
Terminal.app only does 256.)**
**A: RGB API, emit truecolor.** Cells store 24-bit RGB; emitter sends
truecolor SGR. Terminal.app is the known casualty; a 256-quantizing emit
mode stays possible later behind the same API.
(Rejected: downconvert-to-256 default; indexed-256 API; 16-color brutalism.)

## Round 2 — rendering and frame model

**Q: Rendering paradigm per frame?**
**A: Full redraw + diff.** App redraws the whole back buffer every frame;
tessera_praesentare() diffs against the front buffer cell-by-cell and emits
minimal escapes. The diff IS the damage tracking; the app stays dumb and
correct. A 200x60 diff is trivially fast in C.
(Rejected: dirty-region tracking — adds invalidation bugs as a failure
class; retained cell ownership — interleaves badly with a REPL's discrete
updates.)

**Q: Who owns the main loop?**
**A: Blocking wait + timeout.** tessera_eventum_expectare(ctx, ms) blocks on
the tty fd via select(), returns key/mouse/resize/timeout events. App owns
the loop; the timeout covers ESC disambiguation and simple animation.
Other-fd integration deferred (named).
(Rejected for v1: exposed-fd primitives; both-from-start.)

**Q: Scrolling — pure diff repaints whole panes; DECSTBM scroll regions move
rows for free but are stateful and fiddly under diffing?**
**A: Pure diff in v1.** Repaint-on-scroll; ~50KB worst case per full
200x60 repaint is fine over a local tty. Scroll-region optimization is added
later ONLY if measured latency demands it (measure-first culture — the
metrics counters exist to make this decision on evidence).

**Q: Hardware cursor model?**
**A: Explicit per-frame position.** Hidden by default;
tessera_cursorem_ponere(ctx, x, y) each frame (-1,-1 = hidden), applied
atomically at the end of praesentare. The terminal's own blink blinks.
(Rejected: always-hidden fake cursor; both mechanisms.)

## Round 3 — input

**Q: Key event model — terminal input is fundamentally lossy (Ctrl+I ==
Tab, Shift invisible on printables); kitty protocol fixes it but only in
some terminals?**
**A: Classic lossy xterm.** Event = {codepoint OR special-key enum, modifier
bits where knowable}. Accept the ambiguities every terminal app lives with.
One code path, works everywhere, matches the xterm-only thesis.
(Rejected: kitty opt-in reservation — speculative generality; kitty-required.)

**Q: Bare-ESC disambiguation timeout (felt as ESC latency by vim-style
users)?**
**A: Short fixed timeout, ~25ms.** Imperceptible locally; the theoretical
mis-split on very laggy ssh is accepted — local-first is the reality.
(Rejected: configurable 50ms knob; zero-timeout burst heuristic.)

**Q: Mouse in v1?**
**A: Wheel + click in v1.** SGR mouse mode (1006); press/release/wheel with
x,y. ~100 lines of parsing; the explorer wants it immediately. Drag/motion
deferred (motion reporting is chatty and mostly for hover, which terminals
do poorly).

**Q: Suspend and shell-out (Ctrl-Z / run $EDITOR and resume — same
restore/re-init mechanism)?**
**A: Both in v1.** SIGTSTP/SIGCONT handled correctly AND a public
tessera_intermittere/resumere pair. A TUI that mangles the terminal on
Ctrl-Z feels broken.

## Round 4 — text, unicode, drawing

**Q: Cell content model — source files can contain any UTF-8; wide chars
(CJK) occupy 2 columns?**
**A: Codepoint cells, width 1.** Cell = one u32 codepoint, everything
assumed width 1; box-drawing and Latin accents just work. A CJK line
misaligns (cosmetic, that row only). wcwidth handling is a NAMED v2 item.
(Rejected: built-in wide-range table now; ASCII-only bytes.)

**Q: Styling at the draw-call level?**
**A: Per-call style struct.** TesseraStilus {fg, bg, ornamenta} by value to
every draw call. Stateless, explicit; a syntax theme = an array of stili.
(Rejected: stateful pen; both.)

**Q: Which text attributes?**
**A: The safe six.** bold, dim, italic, underline, reverse, strikethrough —
one flags byte, all single SGR codes, all render in modern terminals.
(Rejected: minimal three; kitty curly-underline extension — first
non-universal escape, declined.)

**Q: Line-art helpers, or is that app-land (first test of the no-widgets
line)?**
**A: Box + rules in v1.** quadrum (single/double/rounded borders), linea
horizontal/vertical, named constants for box-drawing codepoints. Still pure
drawing — no layout, no focus, no state.
(Rejected: constants-only; nothing.)

## Round 5 — robustness and testing

**Q: Crash restoration paranoia level?**
**A: atexit + fatal signals.** atexit cleanup AND handlers for
SIGSEGV/SIGABRT/SIGBUS/SIGFPE/SIGTERM/SIGINT that write() the restore
sequence (async-signal-safe) then re-raise. The terminal survives ANY death.
The impurity (signal handlers need a static context pointer) is accepted
and documented.
(Rejected: atexit-only; explicit-claudere-only purism.)

**Q: The test seam — how do probationes exercise tessera without a
terminal?**
**A: Full fake tty.** The platform layer is a small vtable (read, write,
size, raw-mode enter/exit); probationes open contexts over memory buffers —
scripted input bytes in, captured escape bytes out, plus grid-snapshot
assertion helpers. Diff engine, input parser, and emitter all get true unit
tests. This decision shapes the platform layer's design (pons).
(Rejected: output-capture-only; grid-level-only trust-the-emitter.)

**Q: Startup terminal interrogation (capability queries)?**
**A: Zero queries, assume xterm.** TIOCGWINSZ for size, truecolor assumed
per the thesis. Startup synchronous and instant; wrong assumption = wrong
colors, not broken behavior.
(Rejected: DSR sanity ping; capability probing — "the terminfo disease
reinvented".)

**Q: Instrumentation from day one (silva fructus-counter culture)?**
**A: Counters as product.** TesseraFructus on the context, updated every
praesentare: cells diffed/changed, bytes emitted, present duration. The
explorer can show its own render cost; the scroll-optimization decision
later gets made on evidence.

## Round 6 — boundaries, the app, naming

**Q: Where does tessera permanently stop?**
**A: Grid forever; widgets = a second library.** tessera is permanently
cells + input + drawing primitives. Widget/layout patterns get extracted
from the explorer into a separate library LATER, once real usage shows
which abstractions earn their keep. Boundary NAMED now (the silva lesson).

**Q: Explorer app v0 — what should the first usable version do?
(multi-select)**
**A: Layer/provenance explorer + file picker/multi-file.** Fran's note:
*"i think file picker kind of like ranger or joshuto could be nice"* —
miller-column navigation (parent / current / preview panes) as the app's
spine, where the preview pane of a C file is the silva-powered view.
Layer/provenance = cursor on any token shows its origo chain, toggle
between expansion generations — the "layers are the product" VISIO pin on
an actual screen, 100% real with today's silva.
(Not selected for v0: conditional region navigator, tree inspector — both
noted as natural v0.5/v1 growth, tree views shallow until M2 anyway.)

**Q: Where does the explorer app live?**
**A: Own top-level folder.** A peer of silva/ — first end-user application
in the monorepo, with its own probationes and build script. Consumes silva
ONLY via amalgama/silva.h (proving the deliverable) and tessera the same
way.
(Rejected: silva/instrumenta — would link fontes and muddy the firewall;
apps/ tier — premature with n=1.)

**Q: Naming — the mosaic metaphor (Roman mosaics = tesserae; the technique
= opus tessellatum)?**
**A: Library = tessera; context struct = TesseraOpus** (the mosaic work
itself); a cell = TesseraCellula. API: tessera_aperire(),
tessera_praesentare(), TesseraStilus, TesseraFructus. Explorer app name:
**saltuarius** (forest warden — the keeper of the silva) pitched and not
objected to; treated as proposed, confirm at app-spec time.

## Sequencing (from the surrounding conversation)

silva Phase 7 (debts + solarium corpus feed) proceeds as planned; tessera
is prepared "for later" by this interview. The explorer app replaces
"compile inside solarium's build" as silva's real-host integration proof.
