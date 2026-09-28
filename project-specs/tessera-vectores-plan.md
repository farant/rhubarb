# tessera vectores — plan (input test vectors ported from OpenTUI)

*2026-09-28. Plan for terminal-planning features/003 ("Input test
vectors"), the first tessera 1.2 step. Worktree `../rhubarb-secunda`,
branch `rhubarb-secunda`. Executed INLINE, one task per turn, Fran
approving each. **STATUS: PERFECTUM 2026-09-28** (T1, T1b, T2, T2b, T3,
T3b, T4, T4b, T5; commits 938c0df4 … 7a25a0bd): 238 vectors, 1,270
assertions, nine reader bugs fixed, 108 debt shapes handed to the
tokenizer (tabularium desideratum …E4Q). RELATIO: tessera/phase-log.md. Names marked (unsealed) are working names; Fran names.*

## 1. Goal

Port OpenTUI's input-parsing test DATA (bytes in, meaning out) into
tessera's suite as a C89 table, replayed through a pons. **No library
code changes.** The point is a much stronger oracle *before* tessera's
reader is refactored (tokenizer extraction, terminal-planning
modules/002) or extended (bracketed paste, drag events, tessera 1.2).

Source, pinned: `~/Documents/projects/opentui` @ 7581976f, MIT,
"Copyright (c) 2025 opentui":

| file | lines | what we take |
|---|---:|---|
| `packages/core/src/lib/parse.keypress.test.ts` | 2,020 | legacy key sequences (the kitty and modifyOtherKeys sections are out of scope) |
| `packages/core/src/lib/parse.mouse.test.ts` | 545 | SGR mouse; X10 as debt |
| `packages/core/src/lib/stdin-parser.test.ts` | 2,579 | framing: splits, UTF-8 splits, embedded ESC, timeouts, OSC/DCS/APC garbage, paste |

Take the data, not the parser. Many vectors will give a DIFFERENT
answer under tessera, whose model is deliberately lossy
(`tessera_eventum.h`: `\r` and `\n` → REDITUS, 0x08 and 0x7F →
RETRORSUM, Ctrl+I == TABULA, shift invisible on letters). Those are
ported as **documented-loss** vectors that assert tessera's answer,
with a comment giving OpenTUI's.

## 2. Design decisions (proposed; Fran approves at T1)

**D1: a test-local pons, not a memoria change.** The split and timeout
shapes need a pons that delivers bytes in scripted chunks, with
scripted timeouts between them. `TesseraPonsMemoriae` is part of the
hand-written amalgam header (`amalgama/tessera.h:124`), so changing it
would force an amalgam regeneration and its three gates for a test's
sake. Instead the probatio implements its own `TesseraPons` vtable
(`PonsFrustorum`, unsealed): a list of chunks, each returned by one
`legere` call; an empty chunk is a timeout (`legere` returns 0 once).
The pons is public and is the house test seam, so this is the seam
used as designed.

**D2: data in a header, one new probatio.** `tessera/probationes/
vectores_initus.h` (unsealed) holds `hic_manens constans` tables and
carries the provenance header (MIT, © 2025 opentui, pinned commit, the
source file of each section). `tessera/probationes/
probatio_tessera_vectores.c` (unsealed) is the harness. The runner's
glob discovers it; `probationes/` is already on the include path. The
existing `probatio_tessera_eventum.c` is untouched. The tables are
plain C89 data, so they can move to the tokenizer's suite later with
no edits (features/003 open question 1).

**D3: the vector record.**

    titulus      what it tests (Latin, short)
    octeti       the bytes (may contain \033, \x7f, NUL via explicit length)
    mensura      byte count (explicit, so NUL is allowed)
    eventa[]     expected events, in order, up to VIII (genus, clavis,
                 runa, modificatores, numerus, mus_genus, mus_x, mus_y,
                 mus_pulsus)
    status       VALET (must pass) or DEBITUM (known gap)
    causa        for DEBITUM: which future work fixes it ("ad 001
                 glutinum", "ad 002 lexemator", "ad ?1002")

**D4: every VALET vector runs in four shapes.**
1. whole, in one read
2. every two-way split, both chunks arriving within the timeout
3. byte at a time, all within the timeout
4. followed by `a` (proves nothing after it is swallowed)

Shapes with a timeout *between* chunks are separate, explicit vectors,
because a timeout changes the meaning (a lone ESC becomes FUGA).

**D5: DEBITUM is tolerated BY NAME** (the census-gate pattern:
"omnia==II, each vitium named"). A DEBITUM vector carries the DESIRED
events. The harness runs it, prints `debitum manet: <titulus> (<causa>)`
when it still mismatches, and **fails** with `debitum solutum -
promove: <titulus>` when it starts matching. So the debts are the red
tests of the future work: fixing bracketed paste turns its vectors
green, and the harness forces the promotion to VALET. Every vector,
VALET or DEBITUM, must run without a crash and with bounded memory.

## 3. Hypotheses the port should confirm or refute (from reading tessera_eventum.c)

- **H1: multi-chunk arrival breaks.** `tessera_eventum_expectare` reads
  once, and on INCOMPLETUM reads exactly once more before treating the
  sequence as timed out (`tessera_eventum.c:597–633`). A sequence split
  into three or more reads, all within the timeout (shape 3 on anything
  three or more bytes long), will misparse: `\033[A` byte at a time
  gives FUGA, `[`, `A`. That's a real-world case (ssh splits packets).
- **H2: OSC/DCS/APC become phantom keys.** Only `ESC [` and `ESC O` are
  framed. `ESC ]` falls into the ALTERUM branch: alt+`]`, then the
  payload as runes.
- **H3: X10 mouse payload bytes become phantom keys.** `ESC [ M` is a
  CSI with final `M` and no parameters, consumed as unknown; the three
  raw payload bytes that follow are read as keys.
- **H4: drag motion is reported as a press.** An SGR button code with
  bit 32 set (motion) is classified by `& III` and `M` → PRESSUS.
- **H5: the documented losses hold** (`\r`/`\n`, 0x08/0x7F, Ctrl+I/Tab,
  shift on letters, ESC ESC).

H1–H4 become DEBITUM vectors if confirmed. None is fixed in this plan
(see T5).

## 4. Tasks

Each ends green, with a planted fault shown red, a worklog note
(`tessera/fontes/tessera_eventum.worklog.md`, created in T1), and a
commit through `silva.commissio` run in the worktree.

**T1: harness + test pons + seed vectors.** `PonsFrustorum`, the
four-shape runner, DEBITUM accounting, the provenance header. Seed the
table with tessera's OWN existing eventum cases re-expressed as
vectors (~40): they're known-good, so they prove the harness, and
shapes 2–4 immediately test H1. INTENTIO in `tessera/phase-log.md`
before the first line. Planted fault: flip one expectation; the harness
names it.

**T1b: the H1 fix (inserted 2026-09-28, Fran).** T1 measured H1 (21
owed SINGULA shapes), so the fix came before T2: otherwise every ported
vector would carry the same SINGULA debt and hide new findings. The
reader loops while bytes arrive within the timeout. This is the plan's
one library change (`tessera_eventum.c` + amalgam).

**T2: parse.keypress (legacy).** Sections: basic letters, numbers,
special keys, ctrl+letter, raw ctrl control bytes (± meta), ctrl/alt
+ space, meta+character, function keys, arrows, navigation, modifier
combinations, delete, special characters, meta space and escape, rxvt
arrows with modifiers, ctrl+option+letter, "filters out" terminal
responses and mouse, double ESC, non-BMP Unicode, SS3 keypad. Kitty and
modifyOtherKeys sections: skipped (a comment names features/008).

**T3: parse.mouse (SGR).** Press/release, wheel (including left/right
66/67, which tessera doesn't model: DEBITUM or loss, decided in the
task), modifiers, coordinates (large values, 1→0 based), multi-event
chunks, motion/drag (H4). X10 mode: DEBITUM (H3).

**T4: stdin-parser (framing).** Control characters, lone ESC with
timeout, SS3 interrupted by ESC, meta combinations, UTF-8 split at
every boundary (2/3/4-byte), invalid UTF-8, embedded ESC abort, OSC /
DCS / APC / generic CSI partials and timeouts (H2), multi-event
interleaving, chunk-shape invariance. Bracketed paste: DEBITUM "ad 001
glutinum". Protocol-context sections (capability probes, CPR, pixel
responses): skipped, since tessera never queries (a comment says why).

**T5: RELATIO.** Phase-log RELATIO with the counts (vectors, VALET,
DEBITUM by causa, skipped by reason). Findings to the worklog and to
`../terminal-planning/worklog.md`. Each confirmed hypothesis becomes a
named follow-on:
- H1 → a small reader fix (loop reads while bytes keep arriving within
  the timeout), red already present as DEBITUM
- H2/H3 → the tokenizer extraction (modules/002)
- H4 → `?1002` (tessera 1.2)
The fixes are separate plans; this plan changes no library code.

## 5. Names to seal (Fran)

`vectores_initus.h`, `probatio_tessera_vectores.c`, `VectorInitus`
(the record), `EventumExspectatum` (one expected event),
`PonsFrustorum` (the test pons; `frusta` = pieces), `VECTOR_VALET` /
`VECTOR_DEBITUM`, `causa`. All checked against `latina.h`: none is
a macro.

## 6. AUDIENDA

- The exact vector count after porting is unknown. Estimate: 150–250.
- Whether some OpenTUI vectors depend on OpenTUI-specific parser state
  (its "protocol context", its `mouseButtonsPressed` tracking) that has
  no tessera meaning. Those are skipped with a reason, not forced.
- H1–H4 are readings of the code, not measurements. T1–T4 measure them.
