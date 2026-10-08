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

## TESSERA 1.2 — VECTORES INITUS (2026-09-28)

### INTENTIO (T1: harness + test pons + seed vectors)

Plan: `project-specs/tessera-vectores-plan.md` (terminal-planning
features/003). Port OpenTUI's input-parsing test DATA into a C89 table,
replayed through a pons, with NO library change. T1 builds the harness
and proves it on tessera's OWN known-good eventum cases, re-expressed as
vectors.

- `probationes/vectores_initus.h`: the record (`VectorInitus`,
  `EventumExspectatum`), shorthand macros, the seed table, and the
  provenance header (MIT, © 2025 opentui, 7581976f; the seed section is
  tessera's own).
- `probationes/probatio_tessera_vectores.c`: `PonsFrustorum`, a
  test-local `TesseraPons` delivering scripted chunks (an empty chunk =
  timeout, `legere` returns 0 once). The memoria pons stays untouched,
  since it's in the hand-written amalgam header. Runs every vector in
  four shapes: INTEGRA (one read), BIPARTITA (every two-way split),
  SINGULA (byte at a time), SEQUENS (+ 'a').
- Refinement of plan D5, decided while writing: debt is per SHAPE, not
  per vector (`formae_debitae` bitmask). A vector that parses whole but
  breaks byte-at-a-time keeps its INTEGRA assertion as a real test
  instead of hiding it behind vector-level debt. Owed shapes print
  `debitum manet`; an owed shape that starts passing FAILS with
  `debitum solutum - promove`.
- Expectation: SINGULA fails on every sequence of three or more bytes
  (H1: the reader retries exactly once, tessera_eventum.c:597–633).
  Measure first, then mark exactly the observed failures as owed, each
  checked against H1's mechanism.

**T1 FACTUM.** Harness green: 34 seed vectors, 118 shape runs, 165
assertions, 21 owed shapes (all SINGULA, all H1, measured before
marking). Suite 6/6. Both planted-fault directions caught by name.
Detail: `fontes/tessera_eventum.worklog.md`.

**T1b FACTUM (H1 fix, inserted before T2 with Fran's approval).** The
reader loops reads while bytes arrive within the ESC timeout; only an
empty read times out. 21 debts promoted (186 assertions, 0 debts);
eventum suite unchanged; amalgam VERIFICATUM + idempotent; saltuarius
13/13; compiling plant → exactly the 21 SINGULA red. Detail:
`fontes/tessera_eventum.worklog.md`.

**T2 FACTUM (keypress vectors).** 86 vectors from parse.keypress.test.ts:
same-as-OpenTUI, documented losses, silent noise, 18 named debts (H2
OSC, the ESC-prefix design call, H6). **H6 found:** a pending lone ESC
waits the caller's timeout before the escape timeout, so a quick next
key merges into alt+key (saltuarius: 250 ms). Fix proposed as T2b.
Planted data fault caught in all four shapes. Suite 6/6.

**T2b FACTUM (H6 fix).** A pending partial sequence skips the caller's
timeout and gets only the escape timeout. 2 debts promoted (16 remain:
H2, ESC prefix; the latter's desired semantics DECIDED by Fran, alt+key).
Amalgam VERIFICATUM + idempotent; saltuarius 13/13; compiling plant →
exactly the 2 H6 shapes red.

**T3 FACTUM (mouse vectors).** 35 vectors from parse.mouse.test.ts; 10
VALET held; 25 debt vectors, each verified to fail for its predicted
reason: mouse modifiers dropped, wheel release = second scroll,
horizontal wheel reported vertical, H4 motion as press/scroll, H3 X10 →
phantom keys (incl. two phantom Backspace at coordinate 94). Two design
calls for Fran: horizontal wheel genera; X10 parse vs swallow. Suite 6/6.

**T3b FACTUM (mouse fixes).** One classifier for SGR + X10: modifiers,
wheel release swallowed, horizontal wheel as new genera (Fran),
unrequested motion swallowed, X10 parsed (Fran). 100 shapes promoted
(868 assertions; 16 debts remain). Public header gained two enum values
(appended). Amalgam VERIFICATUM + idempotent; saltuarius 13/13; three
compiling plants, each caught exactly.

**T4 FACTUM (framing vectors).** 83 vectors from stdin-parser.test.ts;
harness gained MORA (mid-stream timeout marker; builder refactor proven
868/868 first, guarded by a discriminating vector + plant). All VALET
held; 26 debt vectors verified: H2 widened (DCS, APC), H7 partial
sequences replayed as phantoms on timeout, H8 mouse split across a
timeout, SS3 swallowing an embedded ESC, foreign forms with phantom
tails, CSI parameter overflow (UB; wraps into a false ctrl). Suite 6/6,
1,258 assertions, 120 named debts.

**T4b FACTUM.** CSI parameters capped (no signed overflow on hostile
input; a huge parameter is invalid) and SS3 aborts on an embedded ESC.
12 shapes promoted; 108 debts remain, all for the tokenizer (002).
Amalgam VERIFICATUM + idempotent; saltuarius 13/13; two compiling
plants, each exact.

### RELATIO (T1–T5, 2026-09-28)

**Landed.** OpenTUI's three input-test files (MIT, 7581976f, read in
full: 5,144 lines) ported as C89 data: 238 vectors in four tables
(SEMEN 34, CLAVIUM 86, MURIUM 35, FRUSTORUM 83), each run in four
shapes (integra, bipartita = every split, singula = byte at a time,
sequens = + 'a'), plus MORA timeouts mid-stream. Input suites: 68 →
1,338 assertions (eventum 68 unchanged + vectores 1,270). 8 work
commits on rhubarb-secunda (938c0df4 … 7a25a0bd), + this RELATIO. Every library change:
failing vectors first, amalgam VERIFICATUM + idempotent, saltuarius
13/13, a planted fault that COMPILES, caught exactly.

**Nine reader bugs found and fixed** (tessera 1.2 input):
H1 split sequences (T1b), H6 pending-ESC merge (T2b), mouse modifiers,
wheel release, horizontal wheel (new genera), unrequested motion,
X10 phantoms (T3b), SS3 embedded ESC, CSI parameter overflow UB (T4b).

**Remaining: 27 debt vectors = 108 shapes, all tokenizer (modules/002)
territory:** H2 OSC/DCS/APC (8), H7 partial-on-timeout (11), H8 mouse
across timeout (3), foreign forms (3), ESC prefix (2). Fran's decided
semantics are in each causa and in the tabularium (decretum …QRYT);
the tokenizer's acceptance bar is desideratum …E4Q.

**Skipped with named reasons** (vector-file headers): kitty and
modifyOtherKeys as KEYS (tessera never requests them; asserted as
silent noise), bracketed paste (needs the GLUTINUM genus of features/
001), OpenTUI protocol-context probes (tessera never queries), OpenTUI
parser-object API, button-tracking drag state.

**Complexities (four-part schema):**
1. *Discovered-while:* T1's first run, all seed vectors VALET. *Consists-
   in:* debt per VECTOR would hide the passing shapes of a vector that
   fails only byte-at-a-time. *Consequences:* 60+ real assertions would
   have been silent. *Handled-by:* debt per SHAPE (`formae_debitae`
   bitmask), decided before marking anything.
2. *Discovered-while:* T1 debt marking. *Consists-in:* a transform of
   a file piped into `scribe` of the same file truncated it; scribe
   wrote 0 bytes and examen said ACCIPE. *Consequences:* an untracked
   file silently emptied. *Handled-by:* rebuilt from the conversation;
   rule "transform to scratch, then scribe"; quaestio …FBRPJ.
3. *Discovered-while:* T1b and T3b plants. *Consists-in:* a plant that
   does NOT compile hid its build error behind the next (restored)
   green run. *Consequences:* a false "gate born red" twice in one day.
   *Handled-by:* every plant run captured to its own log, rc and
   `error:` count checked before reading a verdict.
4. *Discovered-while:* T2's SEQUENS shape. *Consists-in:* H6, a real bug
   no hypothesis predicted; the four-shape design caught it for free.
   *Consequences:* the shapes are worth more than their cost. *Handled-
   by:* T2b fix; kept as a general argument for shape-sweeps.
5. *Discovered-while:* T4 debt verification. *Consists-in:* no VALET
   vector depended on a mid-stream MORA, so the new chunk builder was
   unguarded. *Consequences:* a harness bug in MORA would pass silently.
   *Handled-by:* a discriminating vector (ESC, mora, b) + a plant.
6. *Discovered-while:* every debt table. *Consists-in:* a debt that
   fails proves only "not the desired output", not "fails for the named
   reason". *Handled-by:* a diagnostic run per table (debts temporarily
   VALET) printed tessera's ACTUAL output; every causa matched.

**Next** (Fran chooses): the tokenizer interview (modules/002; 108
shapes waiting), or the remaining tessera 1.2 features (bracketed
paste 001, sync output 002, `?1002` drags), or unicode width
(modules/001 = desideratum …XBWP).

## TESSERA 1.2 — PASTE, SYNC, DRAG (2026-09-28)

Plan: `project-specs/tessera-1-2-plan.md` (terminal-planning features/
001, 002 + `?1002` for modules/013). Decisions D1–D5 approved by Fran
2026-09-28 as proposed (one paste event, 64 KiB cap + truncated flag;
mode strings in one internal header + structural probatio; drag genus
`TESSERA_MUS_TRACTUS`, `?1002` only; 3 s silence timeout ends an
unterminated paste; sync unconditional, `?2026l` in the restore string).

### INTENTIO (T1: mode strings in one place + structural probatio)

Move `INTRANDI`/`EXEUNDI` from `tessera_pons_posix.c:22–23` into
`fontes/tessera_modi.h` (internal; the posix pons still uses them as
static strings for the signal path). New `probatio_tessera_modi.c`:
every `?Nh` in INTRANDI has its `?Nl` in EXEUNDI (and no stray `?Nl`),
plus the expected mode set. NO behavior change: the posix pons writes
identical bytes. The aedilis-derived manifest list
(`fontes_generata.h`) must be regenerated for the new header; the
amalgam's porta 0 (manifest staleness) proves it. Planted fault: an
EXEUNDI missing one `l`.

**T1 FACTUM (mode strings in one place).** `fontes/tessera_modi.h` now
holds INTRANDI/EXEUNDI (internal; the posix pons still uses them as
static strings for the signal path). `probatio_tessera_modi.c` (15
assertions) parses both strings and asserts the law of pairs: every mode
entered is left; nothing is left that wasn't entered; EXEUNDI leaves in
REVERSE order (a stack: mouse modes off before the alternate screen);
the base modes 1049/1000/1006 present; cursor ?25 exempt. Two compiling
plants caught (EXEUNDI without ?1000l → named "intratus, numquam
relictus"; reversed order → the order assertions). The aedilis-derived
manifest (`fontes_generata.h`) regenerated: +tessera_modi.h exactly;
porta 0 green. Amalgam VERIFICATUM + idempotent; escape bytes in the
amalgam identical to before and defined once. Suite 7/7, saltuarius
13/13. T2–T5 add their modes to this header, and the probatio guards
each addition.

### INTENTIO (T2: synchronized output ?2026)

`tessera_praesentare` wraps every NON-empty frame in `?2026h … ?2026l`;
a no-op frame stays ZERO bytes (existing golden). The markers get named
constants in `tessera_modi.h` (`SYNCHRONIA_INITIUM`/`_FINIS`, unsealed):
they're PER-FRAME, never in INTRANDI, so the law-of-pairs probatio gains
rule V (every per-frame mode is left in EXEUNDI, the crash-safety
guarantee) and rule II exempts them. `?2026l` goes FIRST in EXEUNDI (end
any open frame before leaving modes), and `tessera_intermittere` emits it
before handing the screen to `$EDITOR`. Byte goldens change by exactly
the wrapper: each checked by hand (opus, eventum, salve, pons, hospes),
never bulk-replaced. Terminal step: spectaculum in Terminal.app (the
likeliest non-supporter) + a supporting terminal; Fran looks before the
commit.

**T2 FACTUM (synchronized output).** Non-empty frames wrapped in
?2026h…?2026l, no-op frames still 0 bytes; ?2026l first in EXEUNDI and
in intermittere; modi rule V. 9 goldens + saltuarius's pledge updated
explicitly (the pledge was missing from the plan's list; saltuarius
caught it). Three compiling plants caught. Fran's terminal look: all
good. Suite 7/7, saltuarius 13/13, amalgam VERIFICATUM + idempotent.

### INTENTIO (T3: drag events ?1002)

`INTRANDI` gains `?1002h` after `?1000h` (button-event tracking: motion
reported only while a button is held); `EXEUNDI` gains `?1002l` in
reverse position, and the law-of-pairs probatio guards both.
`TESSERA_MUS_TRACTUS` is APPENDED to `TesseraMusGenus` (source header and
the hand-written amalgam header; existing values unchanged).
`_murem_classificare`: bit 32 with button 0–2 → TRACTUS, `mus_pulsus` =
held button, modifiers kept, the M/m final IGNORED for motion (xterm
always sends M; OpenTUI's vectors use m and still call it motion).
Stateless: tessera trusts the button bits and keeps no pressed-set
(OpenTUI keeps one to tell drag from move; under ?1002 the terminal
only reports motion with a button held, so the bits suffice). Still
swallowed: 35 (motion, no button = ?1003 hover, never requested) and
96/97 (motion + wheel bits, URxvt noise). The X10 path gets the same
rule for free (it calls the same classifier).

Red first: the H4 vectors that announced this change flip their
expectations (32 alone, two drags in one chunk, press+drag+release, the
"tractus tacitus" stream vector), plus new drag vectors (middle, right,
ctrl+drag, X10 drag, a full press / drag ×2 / release stroke); the
harness runs each in all four shapes. saltuarius: TRACTUS falls to
`ordinarius` (nothing happens), pinned by one assertion. spectaculum:
a drag moves the cursor like a press does, so the look shows it.
Terminal step: drag in spectaculum; Fran looks before the commit.

**T3 FACTUM (drag events).** `?1002` in the mode strings (enter 1049
1000 1002 1006, leave in stack order; modi rule IV requires 1002).
`TESSERA_MUS_TRACTUS` appended in both headers. Classifier: bit 32 +
button 0–2 → TRACTUS (button + modifiers kept, M/m final ignored), 35/39
and 96/97 still swallowed; X10 drags ride the same rule. 4 H4 vectors
flipped, 7 added; red first = exactly the 10 drag vectors × 4 shapes +
modi. Three compiling plants caught by name. saltuarius pins "a drag does
nothing"; spectaculum's cursor follows drags. Suite 7/7, saltuarius
13/13, amalgam VERIFICATUM + idempotent. **Fran's terminal look
(2026-09-28): all working.** Hazard logged in the eventum worklog:
spectaculum.sh ignores header changes (not fixed here).

### INTENTIO (T4: paste API + failing vectors)

(Written after the work, in the same turn; the ritual puts it first.)
API only, no reader change: `TESSERA_EVENTUM_GLUTINUM` appended to the
genus enum; `TesseraEventum` gains `chorda glutinum` (a view into the
lector's collector, valid until the next `tessera_eventum_expectare`)
and `b32 glutinum_truncatum`; public constants
`TESSERA_GLUTINUM_CAPACITAS` 65536 and `TESSERA_MORA_GLUTINI_MS` 3000
(D1, D4). Both headers, plus `_eventum_vacare` zeroes the new fields.
The harness's `EventumExspectatum` gains the body (bytes + length +
truncated flag), and it COPIES an observed body the moment it arrives,
honoring the view contract. OpenTUI's paste vectors plus tessera's own
become a new table as debts (`CAUSA_GLUTINUM`).

**T4 FACTUM (paste API + debts).** New table GLUTINA, 17 vectors:
- 10 from OpenTUI: simple, empty, newlines, tabs, ESC in body, near-match
  202~, doubled ESC before the end marker, a key after, back-to-back,
  UTF-8. The split-marker and split-UTF-8 tests are covered by the
  BIPARTITA and SINGULA shapes.
- 4 of tessera's own: CR, ctrl-c and a mouse sequence stay body bytes,
  and keys before and after a paste.
- 2 for D4 silence: an unterminated paste becomes a truncated event, and
  silence mid-paste truncates, with the rest arriving as keys.
- 1 already true (a stray 201~ is swallowed).
The old FRUSTA vector "200~ alone" now expects a truncated empty paste.
Debts 108 → 176 (+17 × 4). Skipped with a reason: OpenTUI's 100k and
6000-byte pastes don't fit the 256-byte table and go to a dedicated T5
probatio (cap, truncation, time).
Two debts were flipped to VALET temporarily to prove they fail for the
right reason: `hello` arrives today as five keystrokes. Two
header-drift plants in the hand-written amalgam header were caught by
the amalgam gate by name (a missing field → compile error; a wrong
capacity → macro redefinition).
Harness fix riding along: T3 left `MURIUM_TITULI` without "tractus",
an out-of-bounds read in the failure print for any TRACTUS event.
It was visible and missed: T3's red run printed the expected drags as
`MUS(nulla 12,5 p0)` (the read landed in the adjacent CLAVIUM_TITULI). Suite 7/7, saltuarius 13/13, amalgam
VERIFICATUM + idempotent.

### INTENTIO (T5: paste implementation)

- Mode strings: `?2004h` last in INTRANDI, `?2004l` right after the
  frame-close in EXEUNDI (stack order); modi rule IV requires 2004.
- Lector: a 64 KiB collector (`TESSERA_GLUTINUM_CAPACITAS`) allocated at
  creation (both headers; the amalgam header mirrors the struct);
  steady state allocates nothing.
- Reader: `CSI 200 ~` (exactly one parameter) → the CSI parser reports
  a paste start. `_parsare_plene` then runs the collector, which ALWAYS
  yields one GLUTINUM event, so no call site in `expectare` changes.
- The collector drains the 64-byte buffer byte by byte, matching the end
  marker `ESC [ 2 0 1 ~` with a prefix counter carried across reads.
  On a mismatch the matched prefix is body, and the byte is re-tried as
  a new ESC. Bytes after the marker stay in the lector buffer for
  normal parsing.
- Reads inside a paste use `TESSERA_MORA_GLUTINI_MS` (3 s), NOT the
  caller's timeout. An empty read = silence (D4) or a dead terminal:
  the paste ends truncated, and a dangling marker prefix becomes body.
- Past the cap, bytes are dropped (flag set) but the reader keeps
  draining to the marker. Reads stay 64-byte staged (the body never
  lives in that buffer); the throughput cost is measured, not assumed.
- Red first: a new `probatio_tessera_glutinum.c` covers 100k (cap,
  truncation, the key after), exact-cap and cap+1 boundaries, 1000 × 6
  bytes in 6-byte reads (the reader's paste timeout value recorded), a
  1 MB timing, and zero steady-state allocation. Plus saltuarius end to
  end: `ESC[200~ jjG\rq ESC[201~` through the amalgam's memoria pons →
  one GLUTINUM, tradere FALSE; the same bytes unbracketed DO issue
  commands (the gap, shown). Then the 68 paste debt shapes get promoted.
- Terminal step: paste multi-line text containing commands into
  saltuarius (nothing runs) and into spectaculum (status shows the
  paste length).

**T5 FACTUM (bracketed paste).** `?2004` in the mode strings (enter
1049 1000 1002 1006 2004; leave 2026 2004 1006 1002 1000 1049; modi rule
IV requires 2004). Each lector gets a 64 KiB collector at creation.
`CSI 200 ~` starts the collector, which always yields ONE GLUTINUM
event: the end marker is matched across reads, a false prefix becomes
body, reads inside a paste wait 3 s (D4), silence truncates, and past
the cap the reader drains but keeps nothing.
- Red first: the new `probatio_tessera_glutinum.c` (100k, cap and cap+1,
  1000 × 6-byte reads with the timeout counted, 1 MB timed at ~2.7 ms,
  zero steady-state allocation) and saltuarius end to end (bracketed
  `jjG\rq` → one event, tradere FALSE; unbracketed → 5 commands incl.
  quit).
- The 17 paste debts (68 shapes) promoted and `CAUSA_GLUTINUM` retired
  (debts back to 108, all tokenizer territory). 2 new pins (a dangling
  prefix on silence; `200;5~` is not a paste).
- The vector harness rolls its piscina back per run (64 KiB per lector).
- Six compiling plants caught by name.
- Suite 8/8, saltuarius 13/13, amalgam VERIFICATUM + idempotent.
**Fran's terminal look (2026-09-28): all good** (saltuarius runs nothing
on paste, spectaculum shows the paste, the shell is clean after exit).

### RELATIO (tessera 1.2, T1–T6, 2026-09-28)

**Landed.** Three terminal modes, each one tessera SETS and never
queries (so the thesis holds: a terminal that doesn't know a mode
ignores it):
- `?2026` synchronized output: every non-empty frame wrapped, empty
  frames still zero bytes.
- `?1002` drags: `TESSERA_MUS_TRACTUS`, stateless.
- `?2004` bracketed paste: one `TESSERA_EVENTUM_GLUTINUM` per paste,
  64 KiB cap + truncated flag, 3 s silence ends an unterminated paste.
The mode strings live in one internal header (`tessera_modi.h`), guarded
by a structural law-of-pairs probatio (rules I–V: pairing, no strays,
stack order, required modes, per-frame modes first in the crash string).
The safety gap that motivated features/001 is closed and pinned end to
end: saltuarius no longer executes pasted text.
5 work commits on rhubarb-secunda (9a8d51ab, cbc10f3b, 59fa1121,
f436b629, 44c05466) + this RELATIO. Suites: tessera 7 → 8 probationes
(+ `probatio_tessera_glutinum`), vectors 238 → 264 (+7 drag, +19
paste); debts back to exactly 108 (all tokenizer). saltuarius 13/13
throughout. Fran looked in the terminal after T2, T3 and T5: all good.
Ledger: nota …6BSG7G (sequitur the vectors nota); desiderata
…K14Y34 (paste into saltuarius search) and …ETHAJYNGYXF
(spectaculum.sh ignores headers); a note on the tokenizer desideratum
…E4Q (its acceptance bar now includes the paste vectors).

**Complexities (four-part schema):**
1. *Discovered-while:* T2's saltuarius run (12/13). *Consists-in:* the
   plan listed tessera's byte goldens but missed saltuarius's pledge
   golden, which pins tessera's first-frame bytes from outside.
   *Consequences:* a plan's "goldens to update" list is a guess until a
   sweep proves it. *Handled-by:* updated explicitly; a
   `grep -F '\033[?25l'` sweep confirmed nothing else pins frame bytes.
2. *Discovered-while:* T2's commit. *Consists-in:* the Latin lint
   rejected "synchronia". *Handled-by:* renamed to
   `QUADRUM_INITIUM`/`QUADRUM_FINIS` (tessera's own word for a frame),
   suites rerun after the rename.
3. *Discovered-while:* preparing T3's terminal look. *Consists-in:*
   `spectaculum.sh` rebuilds an object only when its `.c` is newer; it
   ignores headers, and plants leave planted objects in the shared
   build dir. *Consequences:* a look could run stale mode strings.
   *Handled-by:* a rebuild after every plant plus a `strings` check of
   the posix object before each look; the fix is filed as a desideratum
   (…ETHAJYNGYXF).
4. *Discovered-while:* T4 (reading T3's red log). *Consists-in:* the
   harness's enum-indexed `MURIUM_TITULI` wasn't grown with the enum;
   the failure print read out of bounds and printed "nulla", visible and
   missed. *Handled-by:* fixed; worklog rule: when an enum grows, check
   every table indexed by it.
5. *Discovered-while:* T5 red run. *Consists-in:* credo writes its
   failure records into the test's piscina, so a zero-allocation check
   spanning several CREDOs measured credo, not tessera. *Handled-by:*
   measure around the one call, assert afterwards.
6. *Discovered-while:* T4 design. *Consists-in:* the frustorum pons
   ignores the requested timeout, so a harness `MORA` inside a paste
   means D4 silence, never a short gap. *Consequences:* the 3 s timeout
   value can't be checked by vectors. *Handled-by:* a counting pons in
   `probatio_tessera_glutinum` asserts every read inside a paste uses
   `TESSERA_MORA_GLUTINI_MS` (a plant proved it).
7. *Discovered-while:* T5. *Consists-in:* every lector now allocates
   64 KiB, and the vector harness creates one per shape run (thousands).
   *Handled-by:* per-run `piscina_notare`/`piscina_reficere`; the binary
   peaks at 1.8 MB.

**Design choices worth remembering:** paste reads stay staged through the
64-byte buffer (bytes after the end marker need no hand-off), measured
at ~2.7 ms per MB through the memoria pons; inside a paste the caller's
timeout yields to the 3 s paste timeout (documented in the header).

**Next** (Fran chooses): unicode width (modules/001, desideratum
…XBWP), the terminal as a ludus render target (modules/012/013, now
unblocked by `?1002`), or the tokenizer (modules/002, 108 shapes
waiting).

## WIDE CELLS (runae U5, 2026-09-28)

Plan: `project-specs/unicode-width-graphemes-plan.md` U5 (terminal-planning
features/004). Width comes from `runae_latitudo` (Unicode 15.1, Ghostty's
rule, ICU-verified); tessera's own "latitudo 1 praesumpta" ends here.

### INTENTIO

- **Markers ride `ornamenta`** (features/004; signum has no spare bits):
  `TESSERA_ORNAMENTUM_LATUM` 0x40 = first cell of a 2-cell rune,
  `TESSERA_ORNAMENTUM_CONTINUATIO` 0x80 = its second half (signum 0, never
  emitted). They are NOT SGR: masked out of every style a caller passes
  (`TESSERA_ORNAMENTA_STILI` 0x3F) and out of the style comparison in
  emission; they DO count in cell equality (a marker change repaints).
- **Drawing rules live in `tessera_cellulam_ponere`** (the one primitive
  every drawing call uses, so `tessera_replere` and boxes inherit them):
  writing over a continuation blanks its start; writing over a start
  blanks its continuation; a width-2 rune whose second half would fall
  outside the grid becomes a space (never split, never wraps); a width-2
  rune fully placed writes start + continuation (same style, so a
  background colour spans both).
- **`_octetos_scribere` advances by width.** Width-0 runes (combining
  marks, ZWJ, variation selectors) are DROPPED from the grid for now:
  a cell holds one codepoint, and giving a mark its own cell is the
  misalignment this task removes. That's a known loss of fidelity (é
  written as e + U+0301 shows as e) until D7 (clusters in tessera),
  which the plan decides after this task with the corpus showing it.
- **Emission** (`tessera_praesentare`): continuation cells are skipped
  (their front-buffer copy is still updated); a start cell repaints if
  its continuation changed; after emitting a wide cell, `pos_x = -1`
  forces an explicit cursor position before the next cell. That's the
  containment: a terminal that disagrees about a width damages one cell,
  never the rest of the row.
- **Dependency:** tessera now uses `runae` (+ `runae_tabulae`): the
  test runner's list, spectaculum's generated list, and the amalgam's
  vendored set (renamed `tessera_runae_*`, like `utf8`). The amalgam's
  own verification says what else it needs.

Red first: `probatio_tessera_latae.c` (cells of `a中b`; its first-frame
bytes with a cursor position before `b`; both overwrite cases; the last
column; `tessera_replere` over half; a wide rune over a wide rune shifted
by one; a combining mark dropped; an unchanged second frame writes
nothing). Plants: the containment removed, the start-blanking removed,
the last-column rule removed. Terminal step: spectaculum with CJK,
emoji and a Hindi line, in more than one terminal.

**U5 FACTUM (wide cells).**
- Markers `LATUM`/`CONTINUATIO` (0x40/0x80 in ornamenta, never SGR,
  masked from every style), the drawing rules in
  `tessera_cellulam_ponere`, advance-by-width in `_octetos_scribere`
  (width-0 runes dropped), and emission (continuations skipped; a start
  repaints if its continuation changed; `pos_x = x + 2` models an
  agreeing terminal and containment overrides it with a CUP).
- Red first: `probatio_tessera_latae.c`. Green: tessera 9/9, saltuarius
  13/13, amalgam VERIFICATUM + idempotent with runae vendored
  (`tessera_runae_*`, tables renamed `TESSERA_RUNAE_GRADUS_*` after `nm`
  showed them leaking).
- Plants: containment (it was dead code until the `x + 2` model; the
  first plant attempt didn't compile, redone), start-blanking,
  last-column rule, each caught by name.

**Fran's terminal look (2026-09-28), Terminal.app and Ghostty:**
- Layout correct in both: the `|` column aligned on all five rows; CJK,
  emoji and mixed rows spaced right.
- Terminal.app has NO 24-bit colour: the gradient bar and the red
  background behind 中 are missing, strikethrough too; it misreads
  `48;2;R;G;B` (the trailing 34 turns the text blue). Pre-existing
  tessera behaviour (spec-v2 §6 defers a 256-colour emit), and exactly
  the thesis's "wrong colours, never broken layout". Ghostty shows the
  red spanning both halves.
- The D7 evidence: dropping width-0 runes CHANGES Hindi text (the virama
  goes, न्द splits: हिन्दी reads hinadī) and é becomes e. Spacing marks
  (Mc) in their own cells stay aligned, because Ghostty clusters ह+ि into
  exactly the two cells we gave it. Recommendation recorded in the plan:
  tessera cells hold grapheme clusters (features/005), as U5b.

## GRAPHEME CELLS (runae U5b, 2026-09-28)

Decision D7 = (a) (Fran): a per-opus cluster table.

### INTENTIO

- **Cell encoding:** a cluster of ONE rune keeps today's packed signum;
  a cluster of several runes is interned once in the opus's table, and
  the cell's signum holds its ID with `TESSERA_ORNAMENTUM_GRAPHEMA` 0x100
  (not SGR, masked from styles like LATUM/CONTINUATIO). Equal clusters →
  equal IDs, so the frame diff stays an integer comparison.
- **Table** (flat fields appended to `TesseraOpus`, mirrored in the
  hand-written `tessera.h`): arena bytes, an ID → (offset, length) map,
  and an open-addressing index (FNV-1a, linear probing; slots hold
  ID+1). Allocated at `tessera_aperire` with fixed caps:
  `TESSERA_GRAPHEMATA_MAXIMA` 16384 clusters,
  `TESSERA_GRAPHEMA_OCTETI_MAXIMI` 64 bytes per cluster, a 256 KiB
  arena (~450 KB per opus, next to 4 MB of cells). Grow-only: IDs are
  never reused, because frons and tergum may both still hold them.
  Beyond a cap (table full or cluster too long) the cell degrades to
  the cluster's first rune alone (the U5 behaviour), never breaks.
- **Writing:** `_octetos_scribere` walks clusters with
  `runae_graphema_proximum` (control bytes and invalid UTF-8 still
  become '?'); width-0 clusters (a lone mark with no base) are dropped;
  a cluster's width is Ghostty's cluster width (Mc widens, VS16 after a
  base widens, ZWJ sequences 2).
- **Emission:** a GRAPHEMA cell writes its stored bytes; after ANY
  multi-rune cluster, containment (CUP before the next cell), because
  clusters are where terminals disagree most (research note).
- **API:** `tessera_cellulae_octeti(opus, x, y, exitus, capacitas)`
  returns a cell's UTF-8 bytes whichever encoding it uses (tests need
  it, and so will consumers like saltuarius and the viewer).

Red first: `probatio_tessera_graphemata.c` (e+U+0301 as one width-1
cell; हिन्दी as two width-2 clusters, matching Ghostty; ❤️ and a ZWJ family
as one wide cluster; equal clusters share an ID; the first-frame
bytes with a CUP after the cluster; a cluster over 64 bytes degrades to
its base; a lone mark is dropped; redrawing known clusters allocates
nothing; overwriting a cluster with ASCII clears the marker). Plants:
interning off (a new ID each time), the containment after clusters
off, the length cap off. Terminal step: spectaculum's Hindi, accent,
emoji and new Arabic/Yoruba rows.

**U5b FACTUM (grapheme cells).**
- Per-opus cluster table (interned; ID in signum + `GRAPHEMA` 0x100;
  16384 clusters / 64 bytes each / 256 KiB arena; past a cap → first
  rune), writing by `runae_graphema_proximum`, emission of the stored
  bytes with containment after every multi-rune cluster, the accessor
  `tessera_cellulae_octeti`, and blanking a wide cluster's start clears
  `GRAPHEMA` (else signum 0 reads as cluster ID 0).
- Red first: `probatio_tessera_graphemata.c`; U5's "mark dropped"
  expectation updated.
- Green: tessera 10/10, saltuarius 13/13, amalgam VERIFICATUM +
  idempotent.
- Four compiling plants caught by name (interning, containment after
  clusters, length cap, GRAPHEMA on blanking).

**Fran's terminal look (2026-09-28):**
- **Ghostty:** every row right and every bar aligned: हिन्दी with its
  conjunct, café, ❤️ as colour emoji, Arabic with all harakat (shaped,
  LTR since nothing does bidi), Yoruba tone marks, the ZWJ family as one
  glyph.
- **Terminal.app:** text right (café, Arabic, Yoruba, ❤️, CJK), but two
  WIDTH disagreements:
  - हि drawn 1 cell where Ghostty's rule says 2: it sums codepoint
    widths, spacing mark = 0.
  - The ZWJ family isn't joined (👨 👩 👧, 6 cells vs our 2).
  - In both rows everything to the right is DRAWN one cell left (our bar
    and even the frame border), though the cursor is right. Containment
    protects the cursor, not a terminal's own rendering of the rest of
    a line it measured differently.
- Follow-up (Fran agreed): U5c, a width policy chosen from the
  ENVIRONMENT (not a query): `TERM_PROGRAM=Apple_Terminal` → clusters
  measured as the sum of per-codepoint widths, as OpenTUI's `WidthMethod`
  does.

## WIDTH POLICY FROM THE ENVIRONMENT (runae U5c, 2026-09-28)

### INTENTIO

**What Terminal.app does, from Fran's U5b screenshots (3 data points):**
- हि is 1 cell: spacing marks (Mc) count 0 there, not 1.
- the ZWJ family is three separate emoji: no pictograph join across ZWJ.
- ❤️ is 2 cells: VS16 IS honoured.
So it isn't a plain codepoint sum (that would make ❤️ 1). The policy that
fits all three is **SIMPLEX**: Ghostty's cluster rule with two changes,
(1) a spacing mark does not widen its cluster, (2) ZWJ does not join
pictographs (GB11 off, so each emoji becomes its own cluster and every
cluster stays ≤ 2 cells, which the cell model can hold). Everything else
(VS16/VS15, modifiers, RI pairs, Mn/Me zero) is unchanged. Flags are
untested so far (the look adds a flag row).

**runae:** `RunaePolitica` { `RUNAE_POLITICA_GRAPHEMATUM` (Ghostty, the
default), `RUNAE_POLITICA_SIMPLEX` } and
`runae_graphema_ex_politica(initium, finis, politica, &latitudo)`;
`runae_graphema_proximum` becomes its GRAPHEMATUM wrapper.
`runae_rumpitur` stays pure UAX #29.

**tessera:** its own `TesseraPolitica` (runae's enum isn't public in the
amalgam), an opus field (default GRAPHEMATUM), `tessera_politicam_ponere`,
and `tessera_politica_ambitus()`, which reads `TERM_PROGRAM` (ISO C
`getenv`, not a query): `Apple_Terminal` → SIMPLEX, else GRAPHEMATUM.
**No auto-detection in `tessera_aperire`**: the test suite would then
behave differently depending on which terminal runs it. Apps opt in with
one line (spectaculum, saltuarius). Set the policy right after opening:
cells keep the widths they were drawn with.

Red first: SIMPLEX cases in `probatio_runae_graphemata.c` (क+ि = 1; the
family's first cluster is 👨+ZWJ, width 2; ❤️ still 2; 👋🏿 2; a flag 2)
and in `probatio_tessera_graphemata.c` (हि a 1-cell cluster; the family
as three wide cells). Plants: SIMPLEX's Mc exception off, its ZWJ break
off. Terminal step: spectaculum in Terminal.app (policy from the
environment) with a flag and a skin-tone row added; Ghostty unchanged.

**U5c FACTUM (width policy).**
- `RunaePolitica` / `runae_graphema_ex_politica` in runae; tessera's
  `TesseraPolitica`, the opus field, `tessera_politicam_ponere`,
  `tessera_politica_ambitus` (`TERM_PROGRAM=Apple_Terminal` → SIMPLEX;
  no auto-detection, apps opt in: spectaculum, saltuarius).
- Red first; plants caught.
- **Fran's Terminal.app look REVISED the hypothesis:**
  - The ZWJ rule was confirmed: the family row aligned.
  - The spacing-mark rule was REFUTED: हि squeezed to 1 cell made the
    ि vanish (overdrawn), so Terminal.app gives हि 2 like Ghostty. In
    U5b the shift came from the conjunct न्दी, not हि; I had
    misattributed it.
  - SIMPLEX now differs from Ghostty in the ZWJ rule ONLY.
  - Conjuncts, flags and skin tones draw narrower in Terminal.app (it
    renders rows as shaped text); that's recorded as its limit in the
    research note, not modelled.
- Ghostty (GRAPHEMATUM) looked great with the new rows.
- Suite 10/10, saltuarius 13/13, amalgam VERIFICATUM + idempotent.

## PLACE ONE UNIT (runae U6c, 2026-09-29)

**INTENTIO (Fran approved the U6 re-split).** saltuarius paints with a
style per byte offset, so it cannot hand tessera a whole line; it
needs "place ONE unit here, tell me how wide it was". Multi-rune
clusters can only be placed by tessera (the intern table is private —
the amalgam boundary is the API), so the primitive belongs here:

```c
constans i8*
tessera_graphema_ponere (opus, x, y, initium, finis, stilus, &latitudo);
```

It places the first DRAWABLE UNIT of `[initium, finis)` — the unit
rule written in `runae.h` (U6b): a C0/DEL byte or an invalid byte →
`?`, one byte, 1 column; otherwise a grapheme under the opus's policy;
a zero-width grapheme draws nothing — and returns the pointer after it
with the UNIT's width (the caller advances x by it even when the cell
is clipped: a wide unit at the last column still returns 2, the cell
becomes a space). `_octetos_scribere` is now a four-line loop over it,
so `tessera_scribere` and any consumer share one path. The stale "width
1 assumed" comment on `tessera_scribere` (the line the width ledger
item quoted) is gone.

Intern-table overflow (a cluster > 64 bytes, or the table full) still
falls back to the first rune, but now pads the unit's remaining columns
with spaces, so the measured width and the drawn cells agree (before, a
`ka + i + 30 accents` cluster would leave its second column stale).

**Red first:** a stub (returns `initium`, width 0) — every case red by
name. **Green:** 11/11 suites, including the unchanged wide-cell and
cluster suites (the refactor is invisible to them). The new
`probatio_tessera_ponere` also CROSS-CHECKS tessera's own copy of the
unit rule against runae's: all 35 Lapide samples, both policies, the
sum of returned widths == `runae_latitudo_textus` and the walk ends
exactly at the end.

**Plants (compiling), caught by name:** invalid bytes per series
(ponere), control not `?` (ponere + opus), policy ignored (graphemata +
ponere), overflow padding off (ponere), the writer advancing by 1
(graphemata). Amalgam VERIFICATUM + idempotent, header mirrored
(`tessera_graphema_ponere` exported), saltuarius 13/13 on it.

## FOLIUM, THE CORPUS VIEWER (runae U7b, 2026-09-29)

A new instrumentum beside spectaculum: `./tessera/folium.sh` pages the
35 Lapide samples (or any UTF-8 files) wrapped to the terminal width.
Its pure part lives in `instrumenta/folium/` (not `fontes/`, not the
amalgam — layout is not tessera's); `compile_probationes.sh` links it
for `probatio_tessera_folium`. Narrative, tests and the findings table:
`lib/runae.phase-log.md` ("U7b").

## COLOUR DEPTH FROM THE ENVIRONMENT (quadrans Q4, 2026-09-30)

`TesseraColores` (PLENI 24-bit / CCLVI 256) chosen by
`tessera_colores_ambitus` (COLORTERM, then TERM_PROGRAM=Apple_Terminal),
quantized at emission only. Undefers spec-v2 §6's "256-quantizing emit".
Fran's look: Terminal.app finally shows spectaculum's colour bar and the
red background, no blue text. Narrative: `lib/quadrans.phase-log.md`
(Q4).

## SGR THROUGH THE SHARED CODEC (module 004 T3, 2026-10-03)

`_stilum_emittere` converts `TesseraStilus` losslessly to
`StilusTerminalis` (six ornaments; sublineatum = single underline) and
calls `stilus_codificare(aed, NIHIL, …)`; `TesseraColores` picks the
codec's PLENA/CCLVI; `_cclvi` and `_colorem_emittere` deleted (the
quantizer lives on as `stilus_quantizare`). Bytes identical. The amalgam
vendors the codec (prefix `tessera_stilus_terminalis_`, see
tools/amalgamatio.worklog.md for the harvest bug it exposed). Finding:
no tessera test had ever pinned ORNAMENT bytes — a plant dropping
strikethrough passed 15/15; probatio_tessera_colores IV now pins each
ornament and the 17-parameter maximum (verified against the pre-codec
code too). Narrative: `lib/stilus_terminalis.phase-log.md` (T3).

## PEN RESET BEFORE THE CLEAR (aemulator A3, 2026-10-06)

The emulator loopback (`ludus_tessera/probationes/
probatio_ludus_tessera_reditus.c`: tessera frame -> captured bytes ->
aemulator -> every cell compared) found a real bug: a FULL repaint
(first frame, resize, resume) began `ESC[?25l ESC[2J` with whatever
SGR pen the PREVIOUS frame left - tessera never resets the pen at
frame end, and ED paints the current background (BCE: xterm, Ghostty,
Terminal.app). The repaint then skips cells equal to the empty cell
("2J already painted them"), so every blank cell kept the previous
frame's last background. pictor and scriba hid it by painting nearly
every cell; the loopback showed it after a resize following a coloured
fill. Fix: `ESC[?25l ESC[0m ESC[2J`. Byte goldens updated (opus,
graphemata, eventum, latae, hospes, saltuarius salve); new regression
"calamus nativus ante 2J" in probatio_tessera_opus (red before the
fix). Plant (fix reverted): the regression AND the loopback go red.
