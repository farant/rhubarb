# tessera — spec v2 (codebase-grounded)

*2026-07-03. Supersedes tessera-spec.md (v1) where they differ; v1's
interview decisions all stand. Provenance: v1 + three parallel
codebase explorations (core-lib APIs; amalgamator mechanism/manifest
analysis; build/test/POSIX conventions) + one empirical compile probe
run this session. Every claim below carries its evidence.*

## 1. Resolved v1 open questions

### 1.1 Amalgamator: GENERALIZE (the "big one", decided by measurement)

silva's amalgamator.c (1,139 lines) is **~72-75% reusable mechanism,
~25-28% manifest data** — and the manifest is already isolated in
named file-scope arrays (lines 53-257). Two helpers are already
parameterized (`_in_indice`/`_in_servandis`); the rest read manifest
globals at exactly **9 sites** across `_lexema_emittere`,
`_plagulam_processare`, and main.

**The plan**: extract a `Manifestum` struct {file lists, rename maps
(TYPI_EXACTI + PRAEFIXA_FUNCTIONUM), CADENDA_TYPEDEF/DEFINITIO,
NON_STATICA, servanda/excludenda, verbatim-header path, prooemium,
derive-assertions[], optional latina-datum paths} and thread it
through ~6 functions — **~120-150 lines edited, the ~600-line
scanner/renamer/emitter core untouched**. Copying instead would
duplicate ~700 mechanism lines needing eternal hand-sync. Manifest
stays code (house philosophy: "manifestum EST codex"), so the shape
is: mechanism moves to `silva/instrumenta/silva_amalgama.{h,c}`
(joins the instrumenta glob); silva's
`instrumenta/principalia/amalgamator.c` shrinks to its manifest +
main; tessera gets `tessera/instrumenta/principalia/amalgamator.c`
(its manifest + main). Both amalgamare.sh scripts compile silva's
instrumenta objects as dev-time deps (the amalgamator tokenizes with
SILVA'S OWN LEXER regardless — silva_token/silva_lexema objects; a
dev-time-only cross-folder dependency, firewalled exactly like
today's bootstrap).

**Refactor safety net**: silva's own gates (standalone full-severity
compile, hospes, nm-intersection, probatio_silva_amalgama) plus one
extra check during the refactor — regenerated silva.c must be
BYTE-IDENTICAL before/after the Manifestum extraction (pure
mechanical refactor, zero output drift).

**Tessera-specific manifest content** (all small): 5 vendored file
pairs + tessera fontes; renames Piscina→TesseraPiscina,
chorda→TesseraChorda + prefixes tessera_piscina_/tessera_chorda_/
tessera_chorda_aedificator_/tessera_utf8_; fresh excludenda
(compiler-harvested fixpoint, the silva Phase 6 S44 method); CADENDA
for tessera.h-owned types; NO xar assert; NO latina-datum emission
(that's silva's parsing feature — the Manifestum makes it optional).

### 1.2 Vendored set: FIVE libs, closed dependency graph (verified)

**latina + piscina + chorda + chorda_aedificator + utf8.** Verified
closure: piscina.h → latina only; chorda.{h,c} → piscina +
aedificator + libc (chorda.c:1-7 — NO xar/tabula/friatio);
chorda_aedificator.c → own header + libc only; utf8.{h,c} → latina
only. **chorda_friare is self-contained FNV-1a** (chorda.c:1084-1104
— does NOT call the friatio lib; friatio depends on chorda, not the
reverse). xar, tabula_dispersa, friatio drop cleanly — tessera's
amalgam is materially smaller than silva's.

### 1.3 UTF-8: a decoder already exists — and it changes the cell design

`include/utf8.h` + `lib/utf8.c`: self-contained, latina-only dep,
**decode-only** (utf8_decodere, utf8_longitudo_byte,
utf8_est_continuatio, utf8_numerare_runas, utf8_proxima_runa,
utf8_prior_runa; no encoder exists anywhere — json.c:501-514 has an
inline one-off).

**v2 DESIGN CHANGE (supersedes v1 §3's "cell = codepoint"): the
cell's signum is PACKED UTF-8 BYTES in a u32** (1-4 bytes,
little-position packing; 0 = empty/space). Consequences:
- **No encoder needed** (emitter unpacks bytes straight into the
  output buffer) and **no decoder needed** at draw time (text
  drawing walks utf8_proxima_runa boundaries and packs byte runs;
  invalid sequences → replacement glyph per byte).
- Diff still compares u32 == u32; ASCII cells' packed value EQUALS
  the codepoint (single byte), so host code comparing `signum ==
  'a'` behaves identically — the change is invisible below 0x80.
- Box-drawing constants ship as packed-byte macros
  (TESSERA_SIGNUM_* — e.g. "─" = 0x80x94xE2 packed).
- wcwidth/continuation-cell support (named v2+ deferral) is NOT
  foreclosed — width metadata would ride ornamenta bits or a
  continuation sentinel, orthogonal to signum representation.
- If codepoint semantics are ever needed, the alternative is a ~20
  line utf8_codificare added to lib/utf8.c (a genuine lib
  improvement) — the API surface would not change either way.
FLAGGED for Fran: this is the one v1→v2 design delta; reversible
until Phase A code exists.

### 1.4 Output buffer: chorda_aedificator confirmed, with two facts that shape the emitter

- **The per-frame pattern is `_reset`, not `finire`**:
  chorda_aedificator_reset (h:213) zeroes offset but KEEPS the
  buffer — steady-state frames allocate nothing. `finire` returns a
  chorda ALIASING the live buffer (impl c:624-641, despite the
  header comment claiming destruction) — use
  `chorda_aedificator_spectare` (h:202) for the frame's byte view,
  write(2) it, then reset.
- **Growth leaks the old buffer into the arena** (doubling +
  piscina copy, no free — c:24-57): size capacitas_initialis to the
  worst-case frame (full repaint ≈ cells × ~20 bytes ≈ 512×256×20 ≈
  2.6 MB for the max grid; in practice size to the ACTIVE grid at
  aperire/resize and let one growth per resize be acceptable).
- Escape building is fully covered: appendere_character (h:58),
  appendere_s32/i32 (h:83/88, snprintf-backed), appendere_literis,
  appendere_repetita (h:108 — run-length spaces!). `ESC[12;34H` =
  character + s32 + character + s32 + character. No general
  arena-side integer formatter exists in the repo (snprintf-into-
  local-buf is the universal idiom); stdio is already a hard dep of
  the vendored libs, as in silva's amalgam.

### 1.5 piscina: two facts that prevent bugs

- **`piscina_allocare` is BYTE-PACKED — no alignment** (allocare →
  _allocare_interna with ordinatio=1, piscina.c:340-345). Cell
  grids (16-byte cells) MUST use `piscina_allocare_ordinatum(p,
  mensura, IV)` (h:50). This would have been a silent misalignment
  on some platforms — caught by exploration.
- `piscina_generare_dynamicum(titulus, n)` — n is the INITIAL chunk
  size, NOT a cap (it grows); the fixed variant is
  piscina_generare_certae_magnitudinis. `piscina_vacare` (h:77)
  resets all chunks without freeing — an alternative resize
  strategy if max-size grids are ever revisited.
  piscina_summa_apex_usus (h:98) feeds TesseraFructus metrics.

### 1.6 POSIX surface: VERIFIED clean under the strict flags (probe, this session)

A scratch TU exercising `<termios.h>` (tcgetattr/tcsetattr/ICANON/
ECHO/VMIN/VTIME), `<sys/ioctl.h>` TIOCGWINSZ + struct winsize,
`sigaction` + SIGWINCH + volatile sig_atomic_t handler, `atexit`,
`select` + fd_set + timeval, and even `cfmakeraw` **compiles clean
under the full rhubarb flag set** (-std=c89 -pedantic -Werror
-Wconversion -Wsign-conversion -Wcast-qual -Wstrict-prototypes
-Wmissing-prototypes -Wwrite-strings) **with ZERO feature-test
macros** on Darwin. This extends the tcp_posix.c precedent (POSIX
sockets, same flags, no macros, no build special-casing —
compile_tests.sh:101 treats it like any file). Convention carried:
**system headers live in the pons .c only, never in public
headers** (tcp.h:18-20 includes only repo headers — tessera.h and
tessera_pons.h do likewise).

The repo has ZERO existing termios/signal/atexit code (verified
grep) — tessera's pons is greenfield, but the scaffolding around it
is all precedent.

## 2. Folder scaffold (mirrors silva, minus what doesn't apply)

```
tessera/
  CLAUDE.md                    orientation (VISIO pins + protocol, silva pattern)
  phase-log.md                 append-only INTENTIO/RELATIO log
  compile_probationes.sh       silva template: same 14 flags, NO -framework
                               (pure library), RADIX_FONTES=(piscina chorda
                               chorda_aedificator utf8 credo), fontes/*.c glob,
                               RHUBARB_RADIX export, filter arg
  amalgamare.sh                deps + silva instrumenta objects (mechanism +
                               lexer) + tessera manifest main → amalgama/ +
                               gates (standalone, hospes, nm-intersection)
  spectaculum.sh               demo runner (compiles + execs against real tty)
  fontes/                      tessera_*.{h,c} modules
  probationes/                 probatio_tessera_*.c (credo suite)
  instrumenta/principalia/     amalgamator.c (manifest+main), hospes.c,
                               spectaculum.c (interactive demo — OUTSIDE the
                               test glob, harness never runs a tty program)
  amalgama/                    tessera.c (generated) + tessera.h (hand-written
                               vanilla C89) — committed deliverables
  build/                       gitignored
```
NOT replicated from silva: generare.sh/grammatica (no codegen),
lustrum.sh (no corpus). MAP.txt gets a tessera entry in the silva
multi-line format (START AT tessera/phase-log.md); the update-map
skill maintains it.

Module split (fontes/):
- `tessera_cellula.h` — TesseraCellula {signum, color_litterae,
  color_fundi, ornamenta} (16 B), TesseraStilus, colores (0x00RRGGBB
  + TESSERA_COLOR_NATIVUS sentinel), ornamenta bits (the safe six),
  signum packing helpers (bytes↔u32), box-drawing constants.
- `tessera_pons.h/.c` — the vtable {datum, legere, scribere,
  amplitudo, intrare, exire}; `tessera_pons_memoriae.c` (scripted
  input + captured output, the TEST SEAM); `tessera_pons_posix.c`
  (the ONLY file including system headers: termios, ioctl, signal,
  unistd, select; raw mode, alt-screen, mouse on/off, SIGWINCH flag,
  SIGTSTP/SIGCONT, fatal-signal restore via pre-built static byte
  string + saved termios, atexit).
- `tessera_opus.h/.c` — TesseraOpus (context): grids
  (allocare_ordinatum), active dims, aedificator (pre-sized, reset
  per frame), cursor state, TesseraFructus; aperire/claudere/
  intermittere/resumere; drawing primitives (purgare,
  cellulam_ponere, scribere/scribere_literis via utf8_proxima_runa
  walk, quadrum/lineam_pingere, cursorem_ponere); praesentare (diff
  + SGR-run-coalesced emission + one scribere through the pons).
- `tessera_eventum.h/.c` — TesseraEventum, the input parser (pure
  function over bytes: UTF-8 keys via utf8_decodere, ESC sequences,
  ~25ms ESC timeout via pons legere timeout, SGR mouse), eventum_
  expectare loop (select in the posix pons; EINTR → resize check).

## 3. Test plan grounded in credo

Canonical main shape (probatio_piscina.c pattern): piscina →
credo_aperire → sectioned blocks with imprimere headers → 
credo_imprimere_compendium → exit by credo_omnia_praeterierunt.
Directly useful macros: CREDO_CHORDAE_AEQUALES / CREDO_CHORDA_*
(escape-byte golden tests compare `chorda_aedificator_spectare`
views against expected byte strings), CREDO_AEQUALIS_I32 (cells,
counters), CREDO_VERUM/memcmp for grids. Everything below Phase B's
last inch runs through the memoria pons:
- emitter golden tests (grid state → exact escape bytes; SGR
  coalescing; cursor-move minimization; NATIVUS colors; packed
  UTF-8 signum emission),
- diff correctness (single-cell delta → minimal bytes; style-only
  change; resize → full repaint),
- input parser pure-function tests (byte scripts → event sequences;
  split escape sequences across reads; bare-ESC timeout; SGR mouse;
  UTF-8 keys incl. invalid sequences),
- suspend/resume state machine; crash-restore string content;
  fructus counters; alignment of grid allocations.
Manual last inch: spectaculum (colors, attributes, input echo,
resize torture) — human-run via spectaculum.sh.

## 4. Work inventory (carry / extend / new)

- **CARRY unchanged**: compile script template (silva's, minus
  frameworks), credo, hospes gate pattern, the S44
  excludenda-fixpoint method, CADENDA type-ownership rule, the
  ≤31-char public-name rule.
- **EXTEND (the one shared-infrastructure change)**: the Manifestum
  refactor of silva's amalgamator (§1.1) — done as tessera Phase C
  step 1, guarded by silva's byte-identical regeneration.
- **NEW**: everything in fontes/ (§2), ~5 probationes, spectaculum,
  the tessera manifest, hand-written vanilla tessera.h.
- **NOT needed after all** (v1 uncertainty resolved): xar, friatio,
  tabula_dispersa vendoring; a UTF-8 encoder; any feature-test
  macro plumbing.

## 5. Phase plan (sharpened from v1 §13)

- **Phase 0 — scaffold.** Folder + scripts + CLAUDE.md + phase-log
  + probatio_tessera_salve green. (Half a session.)
- **Phase A — the pure core.** cellula + stilus + signum packing;
  opus grids + drawing primitives + diff/emitter; pons vtable +
  memoria pons; fructus. Zero system headers anywhere. Exit: golden
  escape-byte suite green; a scripted "movie" (draw→present→assert
  bytes ×N frames) proves reset-reuse allocates nothing after warmup
  (apex assertion via piscina_summa_apex_usus).
- **Phase B — the posix pons + input.** termios/alt-screen/signals
  (WINCH/TSTP/CONT/fatal restore)/select; the event parser + ESC
  timeout + SGR mouse; intermittere/resumere. Exit: parser suite
  green through memoria pons; spectaculum runs on a real tty
  (manual bar); suspend/resume works.
- **Phase C — the deliverable.** Step 1: Manifestum refactor in
  silva (byte-identical gate). Step 2: tessera manifest + hand
  tessera.h + amalgamare.sh with all three gates + tessera hospes
  (vanilla C89, latina-keyword variable names). Step 3: excludenda
  fixpoint to zero-unused; line-art polish; worklog; MAP.txt entry.
  Exit: a vanilla host compiles against tessera.h alone; saltuarius
  can start.

## 6. Named deferrals (carried from v1, unchanged owners)

wcwidth/wide/combining (v2+; packed signum doesn't foreclose);
DECSTBM scroll regions (evidence-gated on fructus); exposed-fd
event loop (when saltuarius needs async); kitty protocol / curly
underlines / 256-quantizing emit; drag/motion mouse; widgets =
second library, permanently.

## 7. Risks & notes register

- aedificator growth leak → pre-size to active-grid worst case;
  apex metric watches (Phase A assertion).
- piscina alignment → allocare_ordinatum everywhere a struct array
  lives; note added because the default is byte-packed.
- finire aliasing → emitter uses spectare + reset; finire never
  called on the frame buffer.
- Darwin-only verification → the probe ran on this Mac only; the
  pons is the isolation seam if another platform ever appears
  (v1 §7 unchanged).
- The fatal-signal restore path must be async-signal-safe: pre-built
  static escape string + write(2) + saved termios in a static —
  the ONE sanctioned static in an otherwise context-struct library
  (v1 §8's documented impurity).
