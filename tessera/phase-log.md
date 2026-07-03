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


## Phase A — The pure core

### INTENTIO (2026-07-03)

**What**: everything above the tty — cellula, pons vtable + memoria
pons, opus (grids + drawing + diff/emitter + fructus). Zero system
headers anywhere in Phase A code. Proves: the emitter's byte
discipline under golden tests, the per-frame zero-allocation steady
state, and the test seam that makes Phase B's tty code the ONLY
untested inch.

**Protocol reads done**: spec-v2 §1.3-1.5/§2/§3, v1 §3-§6, the salve
pledges (green).

**Design decisions (recorded before code):**

1. **Modules**: tessera_cellula.{h,c} (cell/stilus/colores/ornamenta/
   signum packing/box constants), tessera_pons.h (vtable, pure
   header), tessera_pons_memoriae.{h,c} (the test seam),
   tessera_opus.{h,c} (context + drawing + praesentare).
2. **Grid indexing uses the MAXIMUM stride** (latitudo_maxima), not
   the active width — resize changes active dims only, cells never
   move, no reallocation ever (v1 interview decision embodied).
   Grids allocated ONCE via piscina_allocare_ordinatum(…, IV)
   (i32-aligned; the byte-packed-default trap pinned by the salve).
   TESSERA_LATITUDO_MAXIMA 512, ALTITUDO 256 (2 MB/grid).
3. **Frame state resets per praesentare**: cursor-position tracker
   and SGR tracker start invalid each frame → the first changed cell
   emits CUP+SGR; runs of same-style adjacent changes share ONE SGR
   and no CUP (the coalescing promise). After emitting at x, tracked
   position = x+1; at row end → invalid (never trust terminal wrap).
4. **First frame rule**: primum → emit cursor-hide + ED clear
   ("\033[?25l\033[2J"), then paint every cell that differs from
   CELLULA VACUA (signum 0, colores nativi, ornamenta 0) — the clear
   already painted the vacua. Subsequent frames diff against frons;
   frons updated per-cell during the walk (no post-frame memcpy).
5. **SGR emission is always a full reset-and-build** ("\033[0…m" with
   attribute/color params appended) — nativus colors need NOTHING
   (reset gives terminal defaults); coalescing comes from run
   detection, not incremental SGR deltas (simple, correct,
   golden-testable).
6. **Cursor**: hidden by default; tessera_cursorem_ponere sets the
   wish; praesentare applies it at frame END (CUP + show / hide),
   tracked so unchanged wishes emit nothing.
7. **Signum**: packed UTF-8 (spec-v2 §1.3). tessera_signum_ex_octetis
   packs 1-4 bytes LSB-first; scribere walks utf8_proxima_runa;
   control bytes + invalid sequences → '?' replacement (one cell per
   byte); signum 0 emits ' '. Box constants as packed macros
   (simplex/duplex/rotundatum sets).
8. **Reading API from day one**: tessera_cellulam_legere (bounds-
   checked) — the code-is-a-database instinct, and it makes grid
   assertions direct instead of via emission.
9. **Resize seam now, signals later**: tessera_magnitudinem_renovare
   re-queries pons->amplitudo, clamps to maxima, forces primum.
   Memoria pons gets a dims setter so the resize path is TESTED in
   Phase A; Phase B's SIGWINCH handler just calls this.
10. **Fructus**: cellulae_collatae/mutatae, octeti_emissi,
    praesentationes, tempus_praesentandi_ms (clock()-based, C89).
    The apex assertion: after a 2-frame warmup, 10 further frames
    with real changes must not move piscina_summa_apex_usus — the
    aedificator is pre-sized in aperire (active cells × 20 + 1024)
    and reused via spectare+reset.
11. **≤31-char names checked at design time** (the silva S47 lesson
    landed in Chunk A of Phase 7): memoria-pons setters are
    tessera_pons_memoriae_initum/captum/purgare/amplitudo — verbs
    trimmed to fit.

**Probationes**: cellula (packing/unpacking/mensura/constants),
pons (memoria seam behaviors), opus (goldens: first frame, minimal
single-cell frame, SGR run coalescing, cursor protocol, clipping,
UTF-8 scribere via cellulam_legere, resize→full repaint, quadrum/
lineam shapes, fructus + THE APEX ASSERTION).

**Explicitly NOT Phase A**: any system header (Phase B); input
parsing (B); intermittere/resumere (B — needs the posix pons);
wcwidth (named deferral); scroll-region optimization (fructus-gated).

**Exit**: suite green (salve + 3 new); golden escape bytes exact;
apex flat after warmup; RELATIO.

### RELATIO (2026-07-03) — PHASE A COMPLETE

Suite 4/4, 123 assertions — **the whole phase landed first-compile,
first-run green** (the salve's dependency pledges plus the spec-v2
exploration facts meant every integration point was known before it
was written; the one pre-compile correction was const-correctness in
scribere_literis, restructured around a const-taking worker before
the compiler ever saw it).

What exists (fontes/, ~800 lines, zero platform headers — time.h
and string.h are C89 libc, the only system includes):
- **tessera_cellula.{h,c}** — TesseraCellula (16 B, quad-i32),
  TesseraStilus + constructors + aequalis, the packed-signum trio
  (ex_octetis / mensura / scribere: LSB-first, 0 = vacuum → ' '),
  colores (NATIVUS sentinel), the safe-six ornamenta, sixteen
  box-drawing constants (simplex/duplex/rotundatum),
  TesseraLineaGenus.
- **tessera_pons.h** — the five-function vtable (pure header).
- **tessera_pons_memoriae.{h,c}** — the test seam: scripted initus
  (chunked reads, exhaustion = timeout), captured exitus
  (spectare/purgare), mutable amplitudo (the resize path testable
  without SIGWINCH), counted intrare/exire.
- **tessera_opus.{h,c}** — the context + the emitter. Grids at
  MAXIMUM stride (512×256, allocare_ordinatum IV — cells never
  move); drawing primitives (purgare/cellulam ponere+legere/
  scribere via utf8_proxima_runa with '?' replacement/quadrum/
  lineam/cursorem); praesentare = diff + CUP-minimized,
  SGR-run-coalesced emission + ONE pons scribere; fructus counters
  incl. clock()-based timing; claudere leaves the terminal reset;
  magnitudinem_renovare (Phase B's SIGWINCH just calls it).

The golden facts now pinned by exact byte assertions:
- first frame = "\033[?25l\033[2J" + painted cells only (vacua
  skipped — ED painted them);
- one changed cell = ONE CUP + ONE SGR + the glyph, nothing else;
- adjacent same-style changes share one CUP + one SGR;
- a frame with no changes emits ZERO bytes (pons scribere not even
  called);
- cursor protocol: wish-based, applied at frame end, idempotent
  (unchanged wish + unpainted frame = nothing);
- claudere = "\033[0m\033[?25h" + exire.
- **THE APEX ASSERTION**: after a 2-frame warmup, 10 further
  changing frames move piscina_summa_apex_usus by ZERO bytes —
  the spectare+reset frame discipline is now a standing oracle,
  not a design intention.

Complexities: none — the exploration-first method (spec v2) is the
story of this phase; every trap it flagged (alignment, finire
aliasing, growth leak, hex-escape munch in test literals) was
designed around rather than debugged out.

Next: Phase B — the posix pons + input (termios/alt-screen/signals/
select in tessera_pons_posix.c ONLY; eventum parser with ESC
timeout + SGR mouse; intermittere/resumere; spectaculum demo).
Protocol: INTENTIO first; re-read spec-v2 §1.6 + v1 §5/§7/§8.


## Phase B — The posix pons + input

### INTENTIO (2026-07-03)

**What**: tessera_pons_posix.{h,c} (the ONLY file with platform
headers, ever), tessera_eventum.{h,c} (the input parser + lector),
intermittere/resumere on opus, the spectaculum demo + runner.
Proves: the last untested inch is exactly the tty syscalls; every
byte of parsing logic runs through the memoria pons in the suite.

**Design decisions (recorded before code):**

1. **Resize needs NO new pons contract**: the lector polls
   pons->amplitudo at each expectare and compares against its noted
   dims — SIGWINCH merely interrupts select (handler installed
   WITHOUT SA_RESTART; EINTR → legere returns 0 → lector re-polls).
   Works identically for the memoria pons (the dims setter makes
   AMPLITUDO events testable). App handler for AMPLITUDO =
   tessera_magnitudinem_renovare + repaint.
2. **Resume gets ONE new optional vtable slot**:
   `b32 (*resumptum)(vacuum*)` — poll-and-clear, NIHIL allowed
   (Phase A code predates it; pre-1.0 vtable change is sanctioned).
   SIGCONT sets the posix flag; the lector emits
   TESSERA_EVENTUM_RESUMPTUM; app handler = same as AMPLITUDO
   (renovare forces primum unconditionally, so one handler serves
   both). Memoria pons gets a settable flag — testable.
3. **Input runa = DECODED codepoint** (not packed): apps compare
   ranges and do case math on input; cells pack for emission. The
   name split makes it unconfusable: eventum.runa (decoded) vs
   cellula.signum (packed).
4. **TesseraLector owns a 64-byte carry buffer** — sequences split
   across reads accumulate; the parser is a pure consume-from-front
   function. ESC disambiguation: lone ESC + 25ms follow-up read
   returning 0 → FUGA. Partial UTF-8 at buffer end waits the same
   25ms, then consumes 1 as skip.
5. **Classic lossy mappings documented in the header**: \r AND \n →
   REDITUS; 0x08 and 0x7F → RETRORSUM; Ctrl+I == TABULA; Ctrl+letter
   → runa+IMPERIUM; CSI 1;m modifiers (shift/alt/ctrl bits); SS3
   (ESC O) arrows + F1-F4; CSI ~ codes (Insert 2, Delete 3, PgUp/Dn
   5/6, F1-F12 = 11-15,17-21,23,24); CSI Z = shift-tab; SGR mouse
   (<btn;x;y M/m, wheel = btn&64, coords 1-based → 0-based).
   Unknown CSI sequences are consumed silently (byte-conservative
   for DISPLAY is silva's rule; for INPUT, unknown control noise
   must not become phantom keys).
6. **Raw mode via cfmakeraw + VMIN=1/VTIME=0 + select** (the probe
   verified cfmakeraw compiles; select supplies all timeouts). ISIG
   off means Ctrl+C arrives as key 0x03 — SIGINT stays in the fatal
   set for kill(2) only.
7. **The fatal-restore static**: saved termios + a fixed restore
   byte string ("mouse off, alt-screen off, SGR reset, cursor
   show"), written via write(2) + tcsetattr (both async-signal-safe)
   from SIGSEGV/BUS/FPE/ABRT/TERM/INT handlers, then re-raise;
   atexit for clean paths. The ONE sanctioned static (CLAUDE.md
   pin).
8. **intermittere/resumere live on opus** and speak only through
   the vtable (reset escapes + exire; intrare + primum) — fully
   testable against the memoria pons; the posix SIGTSTP handler is
   the same dance in async-safe form.

**Chunks** (one pass, module order): pons.h vtable slot + memoria
update → eventum → opus intermittere/resumere → posix pons →
probatio_tessera_eventum (parser through scripted bytes: ASCII,
UTF-8, arrows, SS3, ~-codes, modifiers, alt-prefix, ctrl, mouse
press/release/wheel, unknown-CSI skip, buffered multi-event, lone
ESC via timeout, AMPLITUDO, RESUMPTUM, intermittere counts +
resume full-repaint golden) → spectaculum + spectaculum.sh.

**Exit**: suite green (parser fully covered via memoria pons);
posix pons compiles under the full flag set (its runtime = the
manual bar); spectaculum runs on a real tty (Fran's eyes); RELATIO.

### RELATIO (2026-07-03) — PHASE B CODE COMPLETE
### (manual bar PENDING: ./tessera/spectaculum.sh awaits Fran's eyes)

Suite 5/5, 191 assertions. Everything above the tty is suite-proven;
the posix pons compiles under the full flag set and correctly
refuses a non-terminal ("terminal verum requiritur"); the
INTERACTIVE verification is the one bar only a human can hold.

What landed:
- **tessera_eventum.{h,c}** — the lector (64-byte carry buffer, pure
  consume-from-front parser) + the full classic-lossy key model:
  runae (DECODED codepoints — the runa/signum name split), regimen
  (REDITUS/TABULA/RETRORSUM/Ctrl+littera), CSI (arrows, H/F, Z,
  ~-codes incl. F1-F12 + Insert, 1;m modifiers), SS3, ALTERUM
  prefix (incl. alt+UTF-8 and ESC ESC), SGR mouse (press/release/
  wheel, 1-based→0-based), lone-ESC via the 25ms follow-up read,
  half-rune waits, invalid-byte discard. AMPLITUDO by dimension
  polling (design decision 1 — no pons contract change; SIGWINCH
  just interrupts select) and RESUMPTUM via the new optional
  vtable slot (decision 2).
- **tessera_pons_posix.{h,c}** — the machine, all of it: cfmakeraw +
  VMIN=1/VTIME=0 + select timeouts (EINTR = timeout, EOF = error);
  write loop with partial-write/EINTR handling; TIOCGWINSZ with
  80x24 fallback; alt-screen + SGR-mouse enter/exit strings; the
  sanctioned static (saved termios + restore escapes) serving
  SIGSEGV/BUS/FPE/ABRT/TERM/INT handlers (restore, re-raise),
  SIGTSTP (restore, default, re-raise) / SIGCONT (re-raw,
  re-enter, set resumptum flag, reinstall TSTP), and atexit.
  isatty-gated creare: no terminal → NIHIL (headless = memoria
  pons, by design).
- **intermittere/resumere on opus** — vtable-only (reset escapes +
  exire; intrare + forced primum), golden-tested against the
  memoria pons including the full-repaint-after-resume byte
  assertion.
- **spectaculum** (instrumenta/principalia/ + spectaculum.sh) —
  colors, all six ornamenta, three box genera, arrow-driven cursor,
  key/mouse echo, resize handling, 'i' = intermittere/resumere
  round trip, 'q' quits to a fructus summary line.

Complexities (two, both caught by the first test run / compile):
- **CSI private-parameter bytes** — discovered-while: the
  unknown-sequence fixture (\033[?1049h) leaked "1049h" as phantom
  keys; consists-in: the first scanner accepted only digits and ';'
  as parameters, but CSI grammar puts ?, >, =, : in the parameter
  range 0x30-0x3F (and intermediates 0x20-0x2F); consequences:
  every private-mode report a terminal might echo would type
  garbage into the app; handled-by: full-grammar scan to the final
  byte, private/intermediate marks → whole sequence consumed
  silently. The probatio pins it.
- **`magnitudo` is a latina macro (sizeof)** — the THIRD
  macro-identifier landmine (nomen, registrum, now magnitudo):
  a struct winsize local named magnitudo expanded to `sizeof` and
  produced parse errors; renamed fenestra. The forbidden-identifier
  list in CLAUDE.md gains its third entry.

Named forward: the spectaculum run on Fran's terminal closes the
phase (colors/attributes render, keys/mouse echo, resize repaints,
Ctrl-Z restores and resumes); anything his eyes catch lands here as
addenda. Then Phase C — the deliverable (Manifestum refactor first,
silva byte-identical gate).

### Addendum — the manual bar caught one (Fran, 2026-07-03)

**Ctrl-Z arrived as `runa 'z' +imperium` instead of suspending.**
- discovered-while: Fran's first spectaculum run (everything else
  rendered and echoed correctly — the manual bar earning its place
  on its first outing).
- consists-in: cfmakeraw clears ISIG, so the terminal GENERATES no
  SIGTSTP — Ctrl-Z is just byte 0x1A. I had documented this exact
  consequence for Ctrl-C in the INTENTIO ("arrives as key 0x03")
  and failed to apply it to Ctrl-Z, where the interview decision
  explicitly requires suspension to work.
- consequences: the whole TSTP/CONT handler dance was unreachable
  from the keyboard; Ctrl-Z typed a key like any other.
- handled-by: ISIG re-enabled with VINTR/VQUIT individually
  DISABLED (_POSIX_VDISABLE) — Ctrl-Z alone generates real SIGTSTP
  (handler restores, SIGCONT re-raws + RESUMPTUM event) while
  Ctrl-C/Ctrl-\ remain ordinary app keys. modus_crudus saved AFTER
  the tweak, so SIGCONT restores the corrected raw mode. Suite
  stays 5/5 (0x1A-as-key remains the correct parse for ISIG-less
  ponses — the memoria pons path is unchanged and still pinned).
  eventum.h documents the split. Re-verification: Fran's terminal.

**MANUAL BAR PASSED (Fran, 2026-07-03): "yup! that works
perfectly."** Colors, ornamenta, boxes, cursor, keys, mouse, resize,
and the corrected Ctrl-Z suspend/resume all verified by human eyes
on a real terminal. PHASE B COMPLETE.


## Phase C — The deliverable

### INTENTIO (2026-07-03)

**What**: three steps, in dependency order. **Step 1 — the
Manifestum refactor** of silva's amalgamator (the spec-v2 §1.1
decision, evidence-based): mechanism (~75% of the file) extracted to
silva/instrumenta/silva_amalgama.{h,c}; the manifest data becomes an
AmalgamaManifestum struct; silva's amalgamator.c shrinks to manifest
+ main; **GATE: silva.c and silva_latina_datum.{h,c} regenerate
BYTE-IDENTICAL** (pure mechanical refactor, md5-verified), silva
suite + amalgam gates stay green. **Step 2 — tessera's amalgam**:
tessera manifest main (tessera/instrumenta/principalia/
amalgamator.c), hand-written vanilla tessera.h, tessera hospes
(latina-keyword pollution + equivalence smoke through the memoria
pons), tessera/amalgamare.sh with the three gates (standalone
full-severity, hospes, nm-intersection 0). **Step 3 — closure**:
excludenda fixpoint to zero-unused (the S44 compiler-harvest
method), MAP.txt entry, worklog/RELATIO.

**Design decisions:**
1. **Manifestum shape**: plagula lists (4), CADENDA typedef/
   definitio, NON_STATICA, rename maps (exact + prefixes),
   verbatim-header path, output prooemium, post-vendored-capita
   assertion text (NIHIL licet — the XAR assert becomes silva
   data), optional latina-datum emission block (paths/guard/symbol
   names; tessera: off). The manifest pointer joins the existing
   Amalgamator state struct — the functions already threading that
   struct need no signature changes where possible.
2. **The mechanism stays silva-hosted** (silva/instrumenta/ — it
   tokenizes with silva's lexer; a dev-time cross-folder dependency
   for tessera's amalgamare.sh, firewalled exactly like today's
   bootstrap; instrumenta/*.c glob picks it up in silva's builds).
3. **Tessera vendored set** (spec-v2 §1.2 closed graph): latina
   verbatim + piscina + chorda + chorda_aedificator + utf8.
   Suspicion to verify at fixpoint: chorda.c may exclude down to
   ZERO functions (tessera touches only the chorda TYPE; whether
   aedificator.c calls any chorda_ function decides it).
4. **The posix pons SHIPS in the amalgam** (system includes hoisted
   to the top of tessera.c): tessera IS a terminal library — a
   POSIX-requiring amalgam is honest; the memoria pons ships too
   (hosts test with it; saltuarius will). The pons vtable remains
   the seam if another platform ever appears.
5. **tessera.h ownership (CADENDA)**: full transparent defs for
   TesseraChorda/Cellula/Stilus/Pons/PonsMemoriae/Fructus/Opus/
   Eventum/Lector + enums + macros (colors/ornamenta/signa/claves);
   forward-only for TesseraPiscina + TesseraChordaAedificator
   (hosts hold pointers, never deref). Public vendored surface =
   piscina generare_dynamicum/destruere (NON_STATICA), nothing
   else.
6. **31-char note**: renamed internals like tessera_chorda_
   aedificator_appendere_literis exceed 31 chars but are STATIC
   (internal linkage) — same situation silva ships today; house
   precedent, noted not changed.

**Exit criteria**: silva byte-identical gate passes + silva suite
26/26 + silva amalgam gates green (the refactor is invisible);
tessera/amalgama/{tessera.c, tessera.h} committed artifacts;
tessera amalgamare.sh gates green (standalone, hospes,
nm-intersectio 0, zero-unused); tessera suite 5/5 green; MAP.txt
entry; RELATIO.

### RELATIO (2026-07-03) — PHASE C COMPLETE; TESSERA 1.0 EXISTS

**tessera/amalgama/{tessera.c (102 KB), tessera.h} — committed,
verification-gated, idempotent.** 32 external symbols, every one
tessera_-prefixed (zero pollution surface). Both suites green
(tessera 5/5, silva 26/26); spectaculum builds and refuses headless.

**Step 1 — the Manifestum refactor, gated as promised**: mechanism
(~700 lines) extracted to silva/instrumenta/silva_amalgama.{h,c};
the AmalgamaManifestum struct threads through the Amalgamator state
(9 global-read sites became field reads; the latina-datum emitter
fully parameterized incl. basename-derived content so silva's
output reproduces exactly); silva's amalgamator.c is now 310 lines
of manifest + main. **THE GATE HELD: silva.c AND both
silva_latina_datum files regenerated BYTE-IDENTICAL** (md5), silva
suite 26/26, all silva gates green. One build-plumbing catch: the
verification object and the mechanism object both wanted
build/silva_amalgama.o — the stale full-amalgam object linked into
every probatio (102 duplicate symbols); verification object renamed
amalgama_verificatio.o.

**Step 2 — tessera's amalgam**: manifest (~200 lines) + hand-written
vanilla tessera.h (~290 lines) + hospes (7/7: latina-keyword
pollution vars, golden first-frame bytes THROUGH the amalgam,
zero-byte empty frame, packed signum, arrow-key parse) +
amalgamare.sh (mechanism compiled from silva/ — the dev-time
bootstrap dependency, documented). Vendored set shrank ONCE MORE at
implementation: chorda.{h,c} dropped entirely (tessera calls zero
chorda functions; aedificator calls zero; tessera.h alone owns
TesseraChorda) — final set = latina verbatim + piscina +
chorda_aedificator + utf8. NO derive-assertions needed: tessera.h
reuses the internal macro NAMES, and identical macro redefinition
is legal while DIVERGENT redefinition is a compile error — free
drift-guarding the XAR assert had to hand-build.

**Step 3 — fixpoint + closure**: exclusions converged in three
rounds (28 → 4 → 1 → 0); pleasing detail: aedificator finire and
destruere are among the dead (the spectare+reset frame discipline
made them unreachable). Zero suppressions in the standalone gate.
MAP.txt gained the tessera entry (+ silva's updated to M1-closed).

Complexities (all caught by the gates, all instructive):
- **The S41 detector's second prey**: both pons implementations
  used identical static helper names (_legere/_scribere/_amplitudo/
  _intrare/_exire/_resumptum) — invisible modularly, duplicate
  definitions in one TU. Posix set renamed *_posix. (Silva Phase 6
  caught three of these; the pattern is now twice-proven: modular
  builds HIDE cross-file static collisions until amalgamation.)
- **The FOURTH latina landmine, and the subtlest yet: `exire` is a
  latina macro (exit)** — as a vtable FIELD name it had been
  silently macro-renamed to `exit` in every modular TU
  (consistently, hence green suites!); the amalgam's assembly order
  (vanilla tessera.h BEFORE latina.h) broke the spell:
  `pons->exire` in bodies expanded to `pons->exit` against a struct
  whose field was really named exire. Renamed egredi. The
  forbidden-identifier list is now nomen/registrum/magnitudo/exire
  — and the REAL lesson is sharper: latina macros can silently
  rename STRUCT FIELDS in modular builds and only the amalgam's
  header/macro ordering exposes it. A latina-collision check
  belongs in every hand-written vanilla header's future (the
  silva.h pollution probatio pattern, worth porting to tessera's
  suite eventually — noted, not built).
- **Excluded functions need their PROTOTYPES excluded too** (spec
  §11.1.2 said so; the tessera manifest initially attached
  excludenda only to corpora rows → static-injected prototypes
  without definitions → -Wunused on declarations. Header rows carry
  the same lists, mirroring silva).

TESSERA IS COMPLETE (1.0): Phase 0 scaffold → A pure core (golden
bytes, apex oracle) → B posix pons + input (manual bar passed) → C
deliverable (gates + idempotence). Suite 5/5, 191 assertions; a
vanilla C89 host compiles against tessera.h alone. **Saltuarius can
start.** Named deferrals unchanged (spec-v2 §6): wcwidth, scroll
regions (fructus-gated), exposed-fd loop, kitty, drag, widgets =
second library.

## TESSERA 1.1 (2026-07-03) — tessera_replere

First post-1.0 addition, exactly by the book: saltuarius Phase A
(the first true amalgam host) confirmed the signature through real
usage (selection bar = altitudo-1 fill; origo popup interior =
the rect case, Phase C). tessera_replere(opus, x, y, lat, alt,
signum, stilus) — uniform fill, silent grid clipping. Added to
fontes (opus.{h,c}) + probatio_tessera_opus (replere section:
fill, style, outside-intact, cross-bounds clip) + hand-written
amalgama/tessera.h. Amalgam regenerated; ALL GATES green
(standalone full-severity, hospes 7/7, nm-intersectio 0); suite
5/5; saltuarius adopted it, suite 6/6. The eskil loop: caller
designs, library gains, both prove.
