# eventus — plan B (the terminal source)

*2026-10-02. Spec: `project-specs/eventus-spec.md` (D8 decided by Fran
2026-10-02: (a), tokenizer first). Phase A's RELATIO: the end of
`lib/eventus.phase-log.md`. Module designs: terminal-planning
`modules/002-sequence-tokenizer.md` (B1), `modules/003-input-vocabulary.md`
(B2), `modules/005-tessera-reshaped.md` (B1b, B5). Worktree
`../rhubarb-secunda`. Executed INLINE, one task per turn, Fran approving
each. Red first; plants that COMPILE, caught by name; tasks with a
terminal step end with Fran's look before the commit. Freeze the tree
while any commit's gates run.*

## 0. Facts this plan rests on (read 2026-10-02)

- **tessera's reader** (`tessera/fontes/tessera_eventum.c`, 860 lines)
  is hand-rolled: `_csi_parsare` / `_regimen_parsare` / `_parsare` over a
  64-byte gestation buffer, mapping bytes straight to the LOSSY
  `TesseraEventum` (classic xterm: Ctrl+I == Tab, `\r` and `\n` both
  Enter, no releases). The ESC timeout (25 ms) and bracketed paste live
  in the reader.
- **Its test bar:** `tessera/probationes/vectores_initus.h`. VALET
  vectors (1,162 assertions) must keep passing; 27 DEBT vectors (108
  shapes) name their causes (`CAUSA_H2` OSC/DCS/APC, `H7` partial after
  timeout, `H8` mouse split across a timeout, `ALIENA` foreign terminals,
  `FUGAE_PRAEFIXUM` ESC ESC [ A). The harness FAILS with `debitum solutum
  - promove` when a debt starts passing, so success is measured.
  `probatio_tessera_eventum` (68) must pass unchanged.
- **The reference:** Ghostty (MIT, `../ghostty`, pin 12752b2).
  `src/terminal/Parser.zig` is the vt100.net state machine: 14 states
  (ground, escape, escape_intermediate, csi_entry/param/intermediate/
  ignore, dcs_entry/param/intermediate/passthrough/ignore, osc_string,
  sos_pm_apc_string), `MAX_PARAMS = 24`, `MAX_INTERMEDIATE = 4`, 24
  tests. `parse_table.zig` builds the transition table. ~1,560 lines
  together.
- **Phase A's reusable parts:** `eventus_cauda` (the queue every
  source shares), `claves_physicae` (gains kitty key numbers), the
  conformance table (its terminal column is empty), the notarius and
  `manus_ludus_iterare` (B6).
- **House word for "token" is *lexema*** (glossary; silva and stml use it
  in 36 files). The terminal tokenizer needs its own name (§3).

## 1. Tasks

**B1a — the tokenizer (module 002).** A pure library, no time, no UTF-8.
- Pull API as in module 002's sketch: `_proximum(lx, &ptr, finis,
  &lexema)` returns the next token; split sequences stay in state;
  `_pendet` / `_evacuare` let the READER own the ESC timeout.
- Bounded always: params 24, intermediates 4, string payloads 2048
  (`truncatum`), overflow → `csi_ignore`. Colon sub-parameters kept
  (`separatores` bitset).
- The transition table is BUILT AT CREATION by a small rules function
  that reads like the vt100.net diagram (no generator, no generated
  file).
- **Red first:**
  - Ghostty's 24 Parser.zig tests ported (sequence → expected tokens);
  - a SPLIT-POINT SWEEP: every test sequence fed whole, in every 2-way
    split, and byte by byte, asserting identical tokens;
  - hostile input: 10k params, an unterminated OSC, random bytes →
    bounded memory (`piscina` apex) and no crash.
- **Plants:** a param cap off by one; `:` treated as `;`; split state
  lost between calls; OSC terminated only by BEL (not `ESC \`).

**B1b — tessera's reader on the tokenizer.** `tessera_eventum.c`'s
parsing becomes tokenizer + key semantics; the timeout and paste
collector stay in the reader.
- **The bar:** every VALET vector and `probatio_tessera_eventum`
  UNCHANGED; the debt shapes that the tokenizer fixes (H2, H7, H8,
  ALIENA, FUGAE_PRAEFIXUM) are promoted as the harness demands, each by
  name.
- **Look:** Fran in a tessera demo (folium or musivum): keys, mouse,
  paste behave as before.
- **Plant:** the reader bypassing the tokenizer for one sequence kind
  (a debt must stay unpaid → the harness says so).

**B2 — `interpres_terminalis`: tokens → Eventus (module 003's decode
side).** Name, split and legacy policy SEALED by Fran 2026-10-02. A PURE
decoder: `SeriesLexema` in, Eventus pushed into an `EventusCauda`
(phase A's queue: text copied, motion merged). Token-level state only
(the alt prefix: a lone ESC before the next token; kitty learning in
B2b). The PIPELINE (timeouts, stashed partials, raw side channels: X10
bytes, paste body, foreign tails) stays in the source (B3; today in
tessera's reader, B1b), which hands the decoder tokens, a `post_moram`
flag for evacuated sequences, and raw-channel entry points
(`_x10`, `_glutinum`).

**B2a — legacy.**
- printable bytes: per rune KEY PRESSA (clavis: letters UPPERCASE as in
  fenestra; runa: letters lowercase; codex IGNOTUS; NO shift bit, since
  legacy can't tell Shift from Caps Lock) + TEXT (SCRIPTA), the text
  carrying the case. No TEXT under alt or ctrl.
- controls HONEST: `\r` Enter, `\t` Tab (= Ctrl+I, capability
  `tabula_distincta` FALSE), DEL Backspace, `\n` = Ctrl+J, 0x08 =
  Ctrl+H, other C0 = Ctrl+letter / Ctrl+symbol. tessera's lossy merges
  (`\n`→Enter, 0x08→Backspace) become ITS projection (B5).
- alt: a lone ESC (FUGA) before the next key = MOD_ALT; ESC dispatch =
  alt+char; after a timeout: ESC = Escape, ESC ESC = two, `ESC x` = alt+x.
- CSI/SS3 keys: arrows, Home/End, PgUp/PgDn, Insert/Delete, F1-F12,
  Shift+Tab (CSI Z); xterm modifier param (shift 1, alt 2, ctrl 4,
  meta 8 → super). Codex SET where the sequence names the physical key
  (arrows, navigation, F-keys, Shift+Tab); IGNOTUS where legacy is
  ambiguous (Enter = Ctrl+M, Tab = Ctrl+I).
- mouse SGR `CSI < b;x;y M/m` → pointer at the CELL CENTRE in our pixels
  (the cell size is given at init); press/release/motion (buttons held
  or hover); wheel → ROTULA GRADATA (one notch = one cell height);
  modifiers 4/8/16. X10 via `_x10`. Focus `CSI I` / `CSI O`.
- paste via `_glutinum` → TEXT origo GLUTINATA (copied, truncated).
- **Red first:** byte vectors through `series_terminalis` (input mode)
  + the decoder into a queue. **Plants:** 1-based mouse coordinates
  not shifted; alt prefix not cleared; text emitted under ctrl.

**B2b — kitty.** `CSI keycode[:shifted[:base]] [;mods[:event]]
[;text] u` and event-type subfields on legacy-form keys → PRESSA /
ITERATA / SOLUTA, side-less modifiers + super/hyper/meta/caps/num,
associated text; the BASE-LAYOUT key → `EventusCodex` (the closest kitty
gets to a physical position); kitty's functional key numbers (57344+)
learned by `claves_physicae`; the first kitty sequence seen updates the
capabilities. **Plants:** a release read as press; the base key
ignored.

**B3 — `rivus_terminalis`, the terminal source.** SEALED by Fran
2026-10-02: the name (*rivus*, a stream) and architecture (A): ONE
pipeline, MOVED out of tessera's reader into `lib/rivus_terminalis`,
with tessera's reader becoming at once a PROJECTION of its Eventus
output (B5 folded in). tessera's 1,534 vector assertions guard the move.

rivus is PURE (it lives in lib/, below tessera, so it can't read
through tessera's bridge). The caller pushes bytes (`rivus_tradere`),
asks how long to wait (`rivus_mora_ms`: 0 nothing pending, 25 escape,
3000 paste), reports silence (`rivus_moram`), and pulls events
(`rivus_eventum`, decoding lazily, so motion is NOT merged for the
projection). It owns: the raw buffer, `series_terminalis` (input mode),
`interpres_terminalis`, the queue, the timeout rules and stashed
partials (H7/H8), X10 bytes, foreign tails, the paste collector.

**B3a-i — a vocabulary gap + two decoder fidelity fixes** (found while
designing the projection):
- `datum.rotula` gains `x`, `y` and `modificantes`: shift+wheel,
  ctrl+wheel and the pointer position exist in BOTH sources (tessera's
  vectors expect them; fenestra had them and dropped them). The
  eventus_stml writer is sparse, fenestra fills them, the decoder fills
  them.
- The decoder's `typus` carries the ACTUAL character (fenestra's
  meaning: characters[0]), so a projection can tell alt+`A` from
  alt+`a`.
- motion+wheel (SGR 96/97) is dropped, as tessera does.

**B3a-ii — the pipeline moved + tessera as a projection.** Eventus →
`TesseraEventum`: Ctrl+J → Enter, Ctrl+H → Backspace (the lossy merges),
pixels → cells (cell centre / cell size), botton unknown → pulsus 3,
hover/focus/capabilities/typed text dropped, pasted text → GLUTINUM.
The amalgam vendors the Eventus stack (`eventus_cauda`, `interpres`,
`claves_physicae`, `rivus`). **Bar:** every vector and
`probatio_tessera_eventum` unchanged. **Plants:** the projection
merging motions; the cell conversion off by one.

**B3b — the source as a source.** An eager read for Eventus consumers
(motion merged per read, as in fenestra); mode declaration (`?1003`,
`?1006`, `?2004`, `?1004`, kitty `CSI > 31 u` pushed and `CSI < u`
popped); capabilities first; OSC 52 clipboard write; paste → DEPOSITIO
promotion (paths heuristic). **Look:** a terminal auscultator (the same
STML lines as fenestra's) in Terminal.app and Ghostty. **Plants:** modes
not popped on exit; capabilities not first.

Split by Fran's approval (2026-10-02) into three:
- **B3b-i — the pure source surface.** `rivus_eventum_coalitum` (decode
  everything present when the queue is empty, then extract: motion
  coalesces per read); `rivus_modos_intrare` / `_exire` (bytes
  returned, not written; exit = exact reverse; `?1003` only when SUPER
  is declared, Q24); FACULTATES first in the stream and again on
  declaration. **Plants:** kitty not popped; capabilities not first.
- **B3b-ii — OSC 52 write + paste → DEPOSITIO promotion** (declared,
  like hover; the heuristic is text-only, no filesystem). As built: OSC
  52 is its own module `copia_terminalis` (output, not input; keeps
  base64 out of tessera's amalgam). Drop position = the last pointer
  cell the decoder saw, 0,0 if none (Fran, 2026-10-02).
- **B3b-iii — raw mode as a platform layer** (`lib/terminalis_macos.c`:
  raw on/off, read with a timeout, write; Fran approved the lib/ layer
  over tessera's bridge) + the terminal auscultator + **Look** in
  Terminal.app and Ghostty (answers the two B3 AUDIENDA).

**B4 — the terminal runner of the SAME conformance table.** Needs a
pty harness (`posix_openpt` & co. are not in the house lexicon yet; a
lexicon change owes glossary entries + `officina/auspex_posix.sh`
assertions). The same harness gives `terminalis_posix.c` its first
headless test (B3b-iii's bar was the look).

CORRECTED + split by Fran (2026-10-02). The conformance runner needs NO
pty: rivus is pure, so the runner pushes each scene's bytes into it,
reports silence with `rivus_moram`, reads with the coalescing read and
compares. (The pty harness stays a separate, later item for
`terminalis_posix.c`.)
- **B4a — the comparator.** Excuses keyed by capability (scene
  attribute `excusationes`, capability names; valid ONLY when the
  stream's last FACULTATES deny the capability; no FACULTATES = nothing
  excused); a three-state verdict (FRACTA / CONFORMIS / EXCUSATA);
  modifiers compared on VOCABULARY bits only (Fran: AppKit's Function
  bit 0x800000 is a platform leak); a new capability
  `modificantes_textus` (Fran: an honest name over reusing
  `tabula_distincta`). fenestra stays 8/8.
- **B4b — the terminal column + runner.** `<terminalis profilum
  octeti>` per scene (legacy, kitty), cells of 1x1 pixel so the mouse
  expectations hold, the runner as a suite test. The
table's terminal column (bytes) filled for the A4 scenarios; a runner
feeds bytes through B2/B3 and compares. Capability-keyed `excusationes`
(no releases without kitty, Ctrl+I == Tab in legacy) become real.
- **Plant:** an excuse applied without its capability.

**B5 — folded into B3a-ii** (Fran's architecture (A)).

**B6 — cross-target replay.** Record a session in the terminal (the
notarius), replay it SEMANTICALLY in fenestra (and back), divergences
reported. The Modulus converts cells ↔ pixels.

**B7 — RELATIO.** Phase log, module 002/003/005 statuses, merges, briar
MUTATIONES.

## 2. Gates per task

- tessera suite (all of B); root suite when `include/` changes;
  briar + generata for corpus headers; oratio when the glossary grows.
- B1b/B5: `probatio_tessera_vectores` + `probatio_tessera_eventum`
  are THE bar.
- B4: `tools/conformitas.sh` (fenestra column must stay 8/8).

## 3. Decisions (SEALED by Fran, 2026-10-02)

1. **Name: `series_terminalis`** (*series* = chain/sequence; tessera's
   comments already say *series fugae*). Header `series_terminalis.h`;
   types `SeriesLexema` (one token) and `SeriesLector` (parser state).
   Every identifier is checked against latina.h and the lint first.
2. **DCS in v1: whole and truncated** (one token, payload capped at
   2048, `truncatum`). Streaming (hook/put/unhook) is a later,
   API-compatible addition for the emulator.
3. **C1 8-bit controls: NOT recognized.** 0x80–0x9F are UTF-8
   continuation bytes and pass through as printable bytes.
4. **A separate library** (the emulator, module 006, will consume it).
5. **The transition table is built at creation** by a rules function
   that reads like the vt100.net diagram; no generator, no generated
   file.

## 4. AUDIENDA (carried, with the task that measures each)

- What Terminal.app sends for a file drop beyond pasted path text (B3).
  ANSWERED B3b-iii (Fran's look): nothing beyond it - Ghostty and
  Terminal.app both send a bracketed paste of the path; promotion
  works. The position is the pointer BEFORE the drag (the terminal is
  blind during it): park 008, contract documented, decision (a).
- Which kitty flags Terminal.app ignores silently vs echoes (B3).
  ANSWERED B3b-iii: the auscultator pushes `CSI > 31 u` in both
  terminals; Fran's look found everything working (Terminal.app stays
  legacy, nothing leaks to the shell after exit).
- Whether X10 mouse (still in tessera's VALET set) needs a token path
  or stays a reader special case (B1b).
- The `typus` deletion inventory (D2 step 3): after B5, outside plan B.
