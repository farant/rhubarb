# tessera — spec v1

*2026-07-02. From tessera-interview.md (all decisions there are binding
here). Status: v1 — written from the interview alone; v2 will follow after
agent exploration of the codebase (piscina/chorda/chorda_aedificator APIs,
credo patterns, amalgamator generalization, silva amalgam surface).*

## 1. Thesis

An ANSI-terminal-only (xterm-only) TUI library in C89 Latin — the
anti-ncurses. No terminfo, no autoconf, no capability negotiation: every
terminal that matters in 2026 (Terminal.app*, iTerm2, kitty, alacritty,
tmux) speaks the xterm dialect, so tessera assumes it and stays small.
Cultural reference point: termbox (~2000 lines, cell grid + input, no
widgets), not ncurses.

(*Terminal.app is the one known truecolor casualty — accepted; see §4.)

Role in the monorepo: a reusable library (SQLite-style: develop modular,
ship amalgamated tessera.c + vanilla tessera.h) whose first consumer is
**saltuarius** (proposed name), the silva code explorer — silva's real-host
integration proof and the first end-user app in rhubarb.

Permanent boundary, named now: **tessera is cells + input + drawing
primitives, forever.** Widgets, layout, focus management live in a future
second library, extracted from saltuarius once real usage shows which
abstractions earn their keep.

## 2. Shape

- **Own top-level folder** mirroring silva's conventions:
  `tessera/fontes/`, `tessera/probationes/`, `tessera/amalgama/`,
  `tessera/compile_probationes.sh`, `tessera/amalgamare.sh`.
- **Context struct, no globals** (one documented exception: the crash
  handlers' static context pointer, §8). `tessera_aperire()` →
  `TesseraOpus*`; every call takes it.
- **Vendored deps (amalgam)**: latina + piscina + chorda +
  chorda_aedificator. (No xar/tabula/friatio expected — grids are flat
  arrays. Confirm at v2.)
- **Memory**: caller passes a Piscina at aperire. Front/back grids
  allocated ONCE at fixed maxima — proposal `TESSERA_LATITUDO_MAXIMA 512`,
  `TESSERA_ALTITUDO_MAXIMA 256` (512·256 cells · 16 B · 2 grids = 4 MB).
  Resize (SIGWINCH) only changes the active dimensions; no reallocation
  ever. Output buffer: one chorda_aedificator, reset per frame, ONE write()
  per praesentare.

## 3. Cell model

```c
nomen structura TesseraCellula {
    i32 signum;          /* codepoint UTF-32; 0 = spatium */
    i32 color_litterae;  /* 0x00RRGGBB; TESSERA_COLOR_NATIVUS = default */
    i32 color_fundi;
    i32 ornamenta;       /* fasciculus bitorum */
} TesseraCellula;        /* 16 octeti */
```

- **Codepoint cells, width 1 assumed.** UTF-8 is decoded by tessera at the
  draw-call boundary (chorda in, codepoints into cells). Box-drawing and
  Latin accents just work; CJK misaligns its own row (cosmetic).
  **wcwidth/wide-char support is a NAMED v2 item** — the cell struct and
  diff engine must not foreclose a continuation-cell scheme.
- Control codepoints (< 0x20, 0x7F) render as a replacement glyph; text
  layout (tabs, newlines) is the app's job.
- **Ornamenta — the safe six**: CRASSUM (bold), OBSCURUM (dim), CURSIVUM
  (italic), SUBLINEATUM (underline), INVERSUM (reverse), TRANSFIXUM
  (strikethrough). Each a single universal SGR code.
- **Color**: 24-bit RGB in the API, truecolor SGR (38;2 / 48;2) emitted.
  `TESSERA_COLOR_NATIVUS` (sentinel, e.g. 0xFF000000) = terminal default
  (SGR 39/49). A 256-quantizing emit mode is a possible later addition
  behind the same API; not v1.

## 4. Rendering

- **Full redraw + diff.** The app clears/redraws the back buffer every
  frame; `tessera_praesentare()` diffs back vs front cell-by-cell, emits
  minimal cursor-moves + SGR runs (state-tracked: only emit SGR when style
  changes between emitted cells), copies back→front.
- **Pure diff in v1** — no DECSTBM scroll regions. The metrics (§9) exist
  so that optimization is a measured decision later.
- **Cursor**: hidden by default. `tessera_cursorem_ponere(opus, x, y)`
  each frame (−1,−1 = hidden), applied atomically at the end of
  praesentare.
- Alt-screen (`ESC[?1049h/l`) always; entered at aperire, left at
  claudere/intermittere/crash.

## 5. Input

- **Event API**: `tessera_eventum_expectare(opus, &eventum, ms)` — blocks
  on the tty fd via select() with timeout; returns exactly one event.
  Genera: `CLAVIS`, `MUS`, `AMPLITUDO_MUTATA` (resize), `NIHIL` (timeout).
- **Classic lossy xterm key model.** Event carries: codepoint (printables,
  UTF-8 decoded), or a special-key enum — SURSUM/DEORSUM/SINISTRA/DEXTRA,
  DOMUS/FINIS, PAGINA_SURSUM/PAGINA_DEORSUM, FUGA (ESC), REDITUS (enter),
  TABULA, RETRORSUM (backspace), DELETIO, FUNCTIO 1–12 — plus modifier bits
  where knowable (IMPERIUM = ctrl, ALTERUM = alt/meta via ESC-prefix,
  MAIUSCULA = shift, only on special keys). Ctrl+I == Tab etc. accepted.
- **Bare-ESC disambiguation: fixed ~25 ms** follow-byte timeout.
- **Mouse v1: wheel + click.** SGR mode 1006 (+ basic 1000); press/release/
  wheel events with x,y and button. Drag/motion deferred.
- **Suspend**: SIGTSTP/SIGCONT handled (restore ↔ re-raw + full-repaint
  flag) AND public `tessera_intermittere()/tessera_resumere()` for
  shell-outs ($EDITOR). Resize while suspended handled on resume.

## 6. Drawing API (sketch — API-first, review before building)

```c
TesseraOpus*  tessera_aperire   (Piscina* piscina, TesseraPons* pons);
vacuum        tessera_claudere  (TesseraOpus* opus);
vacuum        tessera_intermittere (TesseraOpus* opus);
vacuum        tessera_resumere  (TesseraOpus* opus);

i32           tessera_latitudo  (constans TesseraOpus* opus);
i32           tessera_altitudo  (constans TesseraOpus* opus);

vacuum tessera_purgare          (TesseraOpus* opus, TesseraStilus stilus);
vacuum tessera_cellulam_ponere  (TesseraOpus* opus, s32 x, s32 y,
                                 i32 signum, TesseraStilus stilus);
vacuum tessera_scribere         (TesseraOpus* opus, s32 x, s32 y,
                                 chorda textus, TesseraStilus stilus);
vacuum tessera_scribere_literis (TesseraOpus* opus, s32 x, s32 y,
                                 constans character* textus,
                                 TesseraStilus stilus);
vacuum tessera_quadrum_pingere  (TesseraOpus* opus, s32 x, s32 y,
                                 s32 latitudo, s32 altitudo,
                                 i32 genus_lineae, TesseraStilus stilus);
vacuum tessera_lineam_pingere   (TesseraOpus* opus, s32 x, s32 y,
                                 s32 longitudo, b32 verticalis,
                                 i32 genus_lineae, TesseraStilus stilus);
vacuum tessera_cursorem_ponere  (TesseraOpus* opus, s32 x, s32 y);

vacuum tessera_praesentare      (TesseraOpus* opus);

TesseraEventumGenus tessera_eventum_expectare (TesseraOpus* opus,
                                 TesseraEventum* eventum, s32 ms);
```

- **Per-call `TesseraStilus {color_litterae, color_fundi, ornamenta}`** by
  value — stateless; a syntax theme is an array of stili.
- All drawing clips silently at the active grid bounds.
- Line art: genus_lineae ∈ {SIMPLEX, DUPLEX, ROTUNDATUM}; box-drawing
  codepoint constants public.
- Public API names ≤ 31 chars (C89 significance — same rule as silva).

## 7. Pons — the platform layer IS the test seam

```c
nomen structura TesseraPons {
    vacuum* datum;
    s32  (*legere)   (vacuum* datum, i8* buffer, i32 capacitas, s32 ms);
    b32  (*scribere) (vacuum* datum, constans i8* octeti, i32 numerus);
    b32  (*amplitudo)(vacuum* datum, i32* latitudo, i32* altitudo);
    b32  (*intrare)  (vacuum* datum);   /* raw + alt-screen + mouse on  */
    b32  (*exire)    (vacuum* datum);   /* restore everything          */
} TesseraPons;
```

- `tessera_pontem_posix_creare(piscina)` — termios raw mode
  (tcgetattr/tcsetattr), TIOCGWINSZ, select() with EINTR-aware resize flag,
  the only file that includes POSIX headers. This is rhubarb's
  platform-independence seam for terminals.
- `tessera_pontem_memoriae_creare(piscina, scriptum_initus)` — scripted
  input bytes in, captured output bytes readable by probationes. Full fake
  tty: diff engine, emitter, input parser, and whole event loops get true
  unit tests. Grid-snapshot assertion helpers ride alongside.
- aperire(piscina, NIHIL) = posix pons by default.
- **Zero terminal queries, ever.** TIOCGWINSZ for size; xterm+truecolor
  assumed. Wrong assumption = wrong colors, never broken behavior.
- Non-tty stdout (piped/CI) with the posix pons: aperire fails loudly.
  Headless = the memoria pons, explicitly.

## 8. Robustness

- **Crash restoration: atexit + fatal signals.** atexit cleanup AND
  handlers for SIGSEGV/SIGABRT/SIGBUS/SIGFPE/SIGTERM/SIGINT that write()
  the fixed restore sequence (async-signal-safe: pre-built static byte
  string — leave alt-screen, show cursor, SGR reset, mouse off) then
  re-raise. The terminal survives any death. Documented impurity: the
  handlers need one static TesseraOpus pointer (or just the static restore
  string + saved termios); set at aperire, cleared at claudere.
- SIGWINCH: handler sets a volatile sig_atomic_t flag; select() wakes via
  EINTR; eventum_expectare re-reads TIOCGWINSZ, clamps to maxima, delivers
  AMPLITUDO_MUTATA. App responds by redrawing (full redraw model = resize
  is free).
- Partial write()/EINTR handled in the posix pons write loop; SIGPIPE →
  the write fails, praesentare reports it (b32 or fructus flag — decide at
  v2), app exits cleanly.

## 9. Metrics — counters as product

`TesseraFructus` on the context, updated every praesentare:
`cellulae_collatae` (diffed), `cellulae_mutatae`, `octeti_emissi`,
`praesentationes`, `tempus_praesentandi_ms` (cumulative). Saltuarius can
show its own render cost in a status line; the scroll-region decision (§4)
gets made on this evidence. Same philosophy as silva's fructus counters:
measurement is product, not debug.

## 10. Testing

- credo suite in tessera/probationes/, everything through the memoria pons:
  - emitter golden tests (grid state in → exact escape bytes out; SGR
    run-coalescing; cursor-move minimization; NATIVUS colors),
  - diff correctness (change one cell → minimal output; resize → full
    repaint; style-only change),
  - input parser as pure function (byte scripts → event sequences: UTF-8
    keys, arrows, F-keys, alt-prefix, SGR mouse, split escape sequences
    across reads, bare-ESC timeout path),
  - suspend/resume state machine, crash-restore string content,
  - fructus counter values.
- The last inch (real tty behavior) is manual: a small demo tool in
  tessera/instrumenta/ (colors, attributes, input echo, resize torture).

## 11. Amalgamation

Ship `tessera/amalgama/tessera.c` + hand-written vanilla `tessera.h`,
produced by the amalgamator pattern proven in silva Phase 6: vendored libs
prefix-renamed (tessera_piscina_* …) and static-internalized, CADENDA rule
for host-facing types, zero-unused end state, verification gates in
amalgamare.sh (standalone full-severity compile, host-pollution test with
latina-keyword variable names, nm-intersection 0 vs rhubarb objects).

**Open question for v2 (the big one)**: generalize silva's amalgamator
(manifest as input) vs copy-and-edit amalgamator.c for tessera. Second
consumer = the natural forcing function for generalization, but don't
gold-plate before the second data point is in hand.

## 12. Saltuarius (the explorer app — brief; own interview later)

- Own top-level folder; consumes silva ONLY via silva/amalgama/ and
  tessera ONLY via tessera/amalgama/ — it IS the real-host proof for both
  deliverables (replaces "compile inside solarium's build" as silva's
  Phase 7+ integration target).
- **v0 spine: ranger/joshuto-style miller columns** (parent / current /
  preview panes, vim-ish keys) where the preview of a C file is the
  silva-powered view.
- **v0 feature: layer/provenance explorer** — cursor on a token shows its
  origo chain (the macro expansion story), toggle between expansion
  generations. The "layers are the product" VISIO pin on an actual screen;
  fully real with today's silva (sceletum grammar is no constraint on the
  preprocessor story).
- Named growth (not v0): conditional region navigator (deepens when Phase
  7 threads CONDITIONALIS into trees), tree inspector (shallow until M2),
  selector-query REPL line (query milestone).

## 13. Phasing proposal

- **Phase A — the pure core.** Cell grid, stilus, draw primitives, diff +
  emitter, memoria pons, fructus. Everything unit-tested; no tty code at
  all. Exit: golden escape-byte tests green.
- **Phase B — the posix pons + input.** termios raw, alt-screen, signals
  (WINCH/TSTP/CONT/fatal), select loop, escape-sequence input parser
  (keys + mouse), intermittere/resumere. Exit: interactive demo tool runs;
  parser probationes green.
- **Phase C — the deliverable.** Line-art polish, amalgamation +
  verification gates, committed amalgama/ artifacts, worklog. Exit:
  a vanilla-C89 host compiles against tessera.h alone.

Then saltuarius starts (own interview/spec first). Sequencing vs silva:
silva Phase 7 (debts + solarium corpus) proceeds first; tessera is queued
behind it ("so we have that for later" — Fran).

## 14. Named deferrals

- wcwidth / wide chars / combining marks (v2; cell struct must not
  foreclose continuation cells)
- DECSTBM scroll-region optimization (evidence-gated on fructus numbers)
- exposed-fd / multi-source event loop (when saltuarius needs async parse
  or timers beyond the timeout)
- kitty keyboard protocol, curly underlines, 256-color quantizing emit
  mode (all behind the existing API if ever)
- drag/motion mouse events
- widgets/layout/focus — permanently out; future second library
