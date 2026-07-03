# Tessera — Phase Log

***Lege tessera/CLAUDE.md primum*** *— vision pins + protocol. This
log is the state re-entry point; append-only; INTENTIO before,
RELATIO after, complexities in the four-part schema (discovered-while
/ consists-in / consequences / handled-by). Design record:
project-specs/tessera-interview.md → tessera-spec.md (v1) →
tessera-spec-v2.md (codebase-grounded).*

---

## Plan of record (spec-v2 §5, revisable)

- **Phase 0 — Scaffold.** Folder + compile script + CLAUDE.md +
  this log + probatio_tessera_salve green (deliberately pins the
  dep behaviors spec-v2 relies on). Proves: the build loop exists.
- **Phase A — The pure core.** cellula (packed-UTF8 signum, stilus,
  colores, ornamenta, box constants) + opus (grids via
  allocare_ordinatum, drawing primitives, diff + SGR-coalesced
  emitter, fructus) + pons vtable + MEMORIA pons. Zero system
  headers. Exit: golden escape-byte suite green; scripted
  multi-frame movie proves steady-state allocates nothing
  (apex assertion).
- **Phase B — The posix pons + input.** termios raw + alt-screen +
  signals (WINCH/TSTP/CONT/fatal restore) + select loop; eventum
  parser (UTF-8 keys, ESC sequences, ~25ms ESC timeout, SGR mouse);
  intermittere/resumere; spectaculum demo (instrumenta/principalia/
  + spectaculum.sh, real tty, manual bar). Exit: parser suite green
  through memoria pons; spectaculum runs.
- **Phase C — The deliverable.** Step 1: Manifestum refactor of
  silva's amalgamator (mechanism → silva/instrumenta/
  silva_amalgama.{h,c}; gate: silva.c regenerates BYTE-IDENTICAL).
  Step 2: tessera manifest + hand-written vanilla tessera.h +
  amalgamare.sh (standalone compile, tessera hospes, nm-intersectio
  0). Step 3: excludenda fixpoint to zero-unused; MAP.txt entry.
  Exit: a vanilla host compiles against tessera.h alone; saltuarius
  can start.

Named deferrals (owners in spec-v2 §6): wcwidth/wide/combining;
DECSTBM scroll regions (fructus-gated); exposed-fd loop; kitty
protocol; drag/motion mouse; widgets = second library, permanently.

---

## Phase 0 — Scaffold

### INTENTIO (2026-07-03)

Create tessera/ per spec-v2 §2: fontes/, probationes/,
instrumenta/principalia/, amalgama/, build/ (gitignored globally);
compile_probationes.sh from silva's template (same 14 flags, NO
frameworks, deps = piscina/chorda/chorda_aedificator/utf8/credo,
fontes glob, RHUBARB_RADIX export, substring filter); CLAUDE.md
(pins + protocol); this log. probatio_tessera_salve does more than
wave: it PINS the four dependency behaviors the v2 design leans on —
(a) aedificator reset keeps content out but appends still work
(the per-frame pattern), (b) appendere_s32/character compose an
escape correctly, (c) utf8_decodere + utf8_proxima_runa on a 3-byte
box-drawing char (the packed-signum walk), (d) allocare_ordinatum
returns aligned storage. If a dep behavior drifts, tessera's suite
says so before tessera's own code misbehaves.

Exit: `./tessera/compile_probationes.sh` green on a fresh checkout.

### RELATIO (2026-07-03) — PHASE 0 COMPLETE

Green on the first run: 1/1 probationes, 16 assertions. The build
loop exists — five dependency objects (piscina, chorda,
chorda_aedificator, utf8, credo) compile incrementally under the
full flag set, fontes/ glob is live (empty for now), the filter and
RHUBARB_RADIX plumbing carried over from silva's template unchanged.

The salve's four dependency pledges all held exactly as the spec-v2
explorations reported:
- aedificator spectare+reset per-frame pattern (content clears,
  buffer survives, appends continue);
- ANSI escape composition via appendere_character + appendere_s32
  ("\033[12;34H" byte-exact);
- utf8_decodere/proxima_runa/numerare_runas on a 3-byte box-drawing
  char (the packed-signum walk mechanics);
- piscina_allocare_ordinatum alignment after a deliberately odd
  3-byte allocation (the byte-packed-default trap, now pinned).

Complexities: none — Phase 0 was assembly of verified parts. One
convention seeded deliberately: the salve is a DEPENDENCY-PLEDGE
suite, not a hello-world; if a root lib's behavior drifts under
tessera's feet, tessera's own suite reports it first.

Next: Phase A — the pure core (cellula + opus + diff/emitter +
memoria pons; zero system headers; golden escape bytes; the
nothing-allocated-after-warmup apex assertion). Protocol: INTENTIO
first; re-read spec-v2 §1.3-1.5 + §2 module split + v1 §6 API
sketch.
