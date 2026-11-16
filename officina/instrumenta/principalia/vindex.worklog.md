# vindex — worklog

## 2026-07-11 — natus (M3 chunk ⑥)

App on THREE amalgams (officina + silva + tessera; saltuarius build
model — amalgam objects in, fontes include dirs deliberately
absent). Parts: vindex_onerator.{c,h} (instrumenta/ — the app-side
loader EXTRACTION: systema+capita+demittere-with-lineae+indicium+
nectere over PUBLIC surfaces only; ARCHITECTURE NOTE: two loader
families exist — fontes-side (cursor/interpretare/bar, house types,
own copies) and app-side (vindex now, M4 REPL next); this extraction
prevents a NEW twin chain; fontes-side twins migrate on next touch);
vindex_visum.{c,h} (conspectus = plain snapshot + paint over
tessera.h — never Machinula*/lector; the saltuarius seam); vindex.c
(main: loader → machinula → indicium bake-write-REOPEN → event loop;
ALL position display through the LECTOR, never in-memory lineae —
format-first made visible); officina/vindex.sh.

Bars this side: cell probatio 24/24 FIRST RUN (title bar, source
markers */>, line numbers, right column stiva+variabilia, position
line with macro stack, command line + cursor position, anulus tab);
-imago smoke (memoriae pons, one painted frame → stdout as text)
at probatio_piscina.c:37: full TUI with LIVE paused values (usus
unsigned long 0, piscina struct Piscina* with real region pointer,
shadowed p_1..p_N honestly typeless — the blessed gap, visibly
noisy: filter = a v0.1 candidate on Fran's call); second smoke at
piscina_summa_usus (function-name punctum): cross-module pause,
source pane auto-switched to lib/piscina.c, two-frame stack.
Suite 10/10; sweep parity EXACT; hospes 78/78 (anulum row).

Notes at the reach:
- machinula_anulum_inspicere born (ring was machinula-private;
  vindex's anulus pane is its first external consumer).
- The -imago position line at a CREDO pause shows "...:37 per
  CREDO_AEQUALIS_I32 <- ...:37" (same file twice) when the paused
  instruction evaluates a macro ARGUMENT (spelled at the invocation
  line, inside the expansion) — correct, not a bug; body
  instructions show credo.h. Recorded because it will look odd
  until you think about it.
- vindex.sh compiles the officina amalgam with -Isilva/amalgama
  (the ONE documented external dep of officina.c).
- compile_probationes.sh gained tessera amalgam + vindex_visum.o
  as standing objects (unused in other probationes — harmless).
- Q12 note for stepping UX: step-over-LINE through a CREDO line =
  hundreds of gradus calls, imperceptible; perge across the whole
  suite from a mid-suite punctum ~1s at -O0. Fine for daily use.

## 2026-07-11 — position-line compression (Fran's find, first session)

The argument-token frame ("...:37 per CREDO_AEQUALIS_I32 <- ...:37")
printed the same via:linea twice — data-honest, display-noisy. Fix
in the DISPLAY layer only (_conspectum_aedificare): a deeper frame
whose (via, linea) indices equal the previous frame's prints just
"per MACRO", no position. The format keeps recording the frame
(the "inside this expansion" fact is real); cross-file frames
(credo.h bodies) still render in full — the compression triggers
only on exact position repetition. Format untouched: presentation
decisions live in presentation.

## 2026-07-11 — stdout-scroll ghosts (Fran's find #2, with screenshot)

Symptom: stepping with 's', the current line "doubles" and the
source pane accumulates shifted duplicates (two '>' markers, lines
appearing twice). Root cause read straight off the screenshot: the
mess begins at imprimere lines — THE INTERPRETED PROGRAM'S printf
WRITES TO THE SAME TTY THE TUI DRAWS ON. Each interpreted newline
physically scrolls the terminal; tessera's diff (frons vs tergum)
has no idea the screen moved, so subsequent frames repaint only
"changed" cells over vertically-shifted ghosts — compounding with
every printing step. (Verified tessera itself is sound: purgare
fills vacuum cells, praesentare emits ' ' for glyph→vacuum — the
diff never ghosts on its own; only external writes to the tty can.)

Fix: capture the debuggee's fds around VM execution (TUI mode
only). _vm_capere/_vm_solvere: fflush + dup2 fd 1 AND 2 to
officina/build/vindex.stdout (O_TRUNC per session) around every
VM-running site (gradus/super/intra/fini/pergere); restore + fflush
after. Status line names the capture file. stderr captured too:
interpreted fprintf(stderr) and machinula halt reports would
scroll the same way; they land in the capture, readable after.
-imago mode deliberately uncaptured (headless smoke wants the
output visible). Future pull: an effusio PANE tailing the capture
file (the gdb-TUI answer); v0 = file + another terminal.

## 2026-11-12 — launcher links again; two limits found on the way

`./officina/vindex.sh` had not linked since 8bbf97f7: praeparator.c calls
`nexus_ordines_capitis_gradus` (silva/instrumenta/nexus_ordines.c) and the
launcher, which compiles its helper objects by hand, never got a block for
that file. Same bug colloquium.sh had until 2026-11-02 and sonda.sh; same
fix (block copied). Nobody saw it because no gate ran the launcher - the
officina suite builds the vindex unit test from its own source list.
Now `probatio_officina_lanceatores` has a section "vindex imago"
(`-plagulae chorda,credo -imago credo_suitam_currere` -> exit 0, header
`VINDEX  lib/credo.c  [PAUSA]`); with the block removed: FRACTA.

What works headless: breakpoints by lib function name and by lib
`via:linea` pause for real (credo_suitam_currere -> lib/credo.c:2390), and
`probationes/probatio_chorda.c:1984` (the line in main) pauses too. The
source panel reads through lectiones_fopen (sweep site, now exercised).

Two limits, NOT fixed (both older than this work):
1. Any credo v2 suite traps before its first section: DECIPULA
   "decipula vocata: getpid" at lib/credo.c:2447 (machinula has no
   getpid). `cursor.sh -sola probatio_chorda` shows the same trap, rc 1.
   So a breakpoint inside a lib function called from a section is never
   reached - the run ends in [DECIPULA] at main, still exit 0.
2. Breakpoints inside the suite's own section functions do not resolve:
   `-imago probationes/probatio_chorda.c:73` and `-imago _sectio_tertia`
   -> "punctum -imago non positum" (exit 2), while line 1984 (main) in
   the same file resolves. Cause not investigated (static functions
   reached only through the SECTIONES table?).
Also: `-imago` on a comment line gives the same "non positum" - my
earlier differential used probatio_chorda.c:60, a comment.
