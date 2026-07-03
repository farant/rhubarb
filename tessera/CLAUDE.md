# tessera — orientation

*You are reading this because you touched a file in tessera/. This is
the canonical orientation: vision pins, protocol, build/test. The
silva/CLAUDE.md discipline applies here wholesale; this file carries
only what is tessera's own.*

## Quid sit

Tessera is the rhubarb monorepo's terminal library — the
anti-ncurses: **xterm-only, zero terminfo, zero capability queries**.
Cell-grid + input + drawing primitives, C89 in Latin, termbox-shaped.
Deliverable: amalgamated `tessera.c` + vanilla-C89 `tessera.h`
(silva's SQLite pattern). First consumer: **saltuarius** (the silva
explorer TUI — silva's real-host proof). A tessera is a mosaic tile;
the screen is the opus.

Design record: project-specs/tessera-interview.md (decisions) →
tessera-spec.md (v1 + API sketch) → **tessera-spec-v2.md
(codebase-grounded — the implementation truth)**.

## PROTOCOLLUM

Same as silva's (silva/CLAUDE.md): phase-log.md tail first at session
start / post-compaction; INTENTIO before a phase, RELATIO after;
complexities in the four-part schema; full phase-log audit at phase
boundaries; never implement from a compaction summary alone. Spec
section to re-read per phase: spec-v2 §5 names them.

## Build/test

`./tessera/compile_probationes.sh [filter]` — pure library, no
frameworks; deps = piscina, chorda, chorda_aedificator, utf8, credo
(dev-time; vendored set at amalgamation = those minus credo).
Dev-time mains (spectaculum demo, amalgamator manifest) live in
instrumenta/principalia/ (outside the test glob — a tty program must
never run headless in the harness), each with its own .sh runner.

## VISIO — pins with their protecting consequences

- **Grid + input, forever.** Widgets/layout/focus are a FUTURE
  SEPARATE library extracted from saltuarius usage. Box/rule
  primitives are the boundary's inside edge; nothing stateful
  crosses it.
- **Xterm-only is the thesis, not a v1 shortcut.** Zero terminal
  queries ever; wrong assumption = wrong colors, never broken
  behavior. Capability probing is the terminfo disease — refusing it
  is the product.
- **The pons vtable IS the test seam.** Everything above the pons is
  unit-tested through the memoria pons (scripted bytes in, captured
  escapes out). System headers live in tessera_pons_posix.c ONLY —
  never in public headers (tcp.h precedent; verified clean under the
  full flag set with zero feature macros, spec-v2 §1.6).
- **Cells are packed UTF-8 bytes in a u32** (spec-v2 §1.3):
  encoder-free emission, decoder-free drawing (utf8_proxima_runa
  boundary walk), ASCII-transparent (packed == codepoint < 0x80).
  wcwidth/continuation cells are NOT foreclosed — they ride
  ornamenta/sentinels, orthogonal to signum.
- **Per-frame = spectare + reset, never finire** on the frame
  buffer: chorda_aedificator_finire ALIASES the live buffer and
  growth LEAKS the old one into the arena — pre-size to the active
  grid's worst case; steady state allocates NOTHING (Phase A asserts
  this via piscina_summa_apex_usus).
- **piscina_allocare is byte-packed — grids use
  piscina_allocare_ordinatum.** (The default alignment is 1. Easy to
  forget; will not warn; will bite.)
- **Fines degrade, never break** (silva's posture inherited):
  clipping is silent, caps degrade, the crash-restore path
  (atexit + fatal signals, async-signal-safe pre-built escape
  string) is the ONE sanctioned static in a context-struct library.
- **Public API names ≤ 31 chars** (C89 significance — silva S47).
- Latin in the code; English fine in meta-documents. `nomen`,
  `registrum`, `magnitudo`, and `exire` are latina macros (typedef/
  register/sizeof/exit) — never identifiers (titulus/tabularium/
  fenestra/egredi instead; the third AND fourth fired HERE — and
  exire had silently macro-renamed a struct FIELD in every modular
  TU; only the amalgam exposed it). i32 is UNSIGNED — s32 for anything
  carrying -1 (the codebase's most reliable slip; three occurrences
  in silva).
