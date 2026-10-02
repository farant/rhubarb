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

**B2 — decode tokens → Eventus (module 003's decode side).** A pure
decoder (tokens + modes → Eventus), no tessera dependency:
- legacy keys (C0, CSI `~`, SS3, xterm `1;m` modifiers) → logical key +
  runa, honestly lossy (`codex` IGNOTUS where legacy can't tell);
- kitty `CSI … u` (disambiguate, event types → PRESSA/ITERATA/SOLUTA,
  alternate keys, associated text) → codex via kitty numbers
  (`claves_physicae` gains them; Ghostty's table as the reference);
- mouse SGR 1006 → pointer in OUR pixels via the Modulus (cell centre),
  buttons, wheel as GRADATA, motion;
- bracketed paste → TEXT `origo` GLUTINATA; focus in/out;
- the first kitty sequence seen → a `FACULTATES` update (learned,
  never queried).
- **Red first:** a byte-vector table per family. **Plants:** kitty
  release read as press; mouse coordinates off by one (1-based);
  paste text decoded as keys.

**B3 — the terminal source over tessera.** The library an app uses:
declares modes (`?1003` any-motion, `?1006`, `?2004`, `?1004`, kitty
`CSI > flags u` pushed and popped), queues through `eventus_cauda`,
publishes capabilities first, owns the ESC timeout via `_pendet`,
OSC 52 clipboard write (capability FORTASSE), paste → DEPOSITIO
promotion (paths heuristic, `promota`).
- **Look:** a terminal auscultator (same STML lines as fenestra's) in
  Terminal.app and in Ghostty, if installed.
- **Plants:** modes not popped on exit (the terminal left in kitty
  mode); capabilities not first.

**B4 — the terminal runner of the SAME conformance table.** The
table's terminal column (bytes) filled for the A4 scenarios; a runner
feeds bytes through B2/B3 and compares. Capability-keyed `excusationes`
(no releases without kitty, Ctrl+I == Tab in legacy) become real.
- **Plant:** an excuse applied without its capability.

**B5 — `TesseraEventum` as a projection of Eventus.** tessera's public
type stays; its meaning is derived from B2's decode, so key semantics
live ONCE. The vectors prove nothing visible changed.

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
- Which kitty flags Terminal.app ignores silently vs echoes (B3).
- Whether X10 mouse (still in tessera's VALET set) needs a token path
  or stays a reader special case (B1b).
- The `typus` deletion inventory (D2 step 3): after B5, outside plan B.
