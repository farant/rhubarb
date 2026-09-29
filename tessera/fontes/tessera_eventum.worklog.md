# tessera_eventum.worklog.md

## 2026-09-28: input vectors, T1 (harness + seed); H1 measured

`probationes/probatio_tessera_vectores.c` + `vectores_initus.h` (plan:
`project-specs/tessera-vectores-plan.md`). A test-local pons
(`PonsFrustorum`) delivers scripted chunks, and an empty chunk is a
timeout. Every vector runs four shapes: INTEGRA, BIPARTITA (every split),
SINGULA (byte at a time), SEQUENS (+ 'a'). The memoria pons is untouched
(it's in the hand-written amalgam header).

**H1 confirmed by measurement.** The first run of tessera's own 34 seed
cases, all marked VALET: 21 failures, ALL in shape SINGULA, ALL on
sequences of 3+ bytes. INTEGRA, BIPARTITA and SEQUENS all green;
two-byte sequences (`é`, alt+b, alt+Enter) green in every shape. Cause:
`tessera_eventum_expectare` reads once, and on INCOMPLETUM reads exactly
ONCE more (the ~25 ms `TESSERA_MORA_FUGAE_MS` read, line 597) before
treating the sequence as timed out. Two faces:

- an ESC sequence in 3+ reads → FUGA, then the rest as phantom keys
  (`\033[A` → fuga, `[`, `A`)
- a 3/4-byte UTF-8 rune in 3+ reads → **silently lost** (lead byte
  dropped by the timeout path, continuation bytes dropped as invalid).
  Worse than the ESC face, because nothing shows.

Real-world: ssh and slow links split writes. The fix belongs to its own
task (loop reads while bytes keep arriving within the timeout, and treat
only a 0-byte read as the timeout). The 21 owed shapes are its red tests
already.

**Design refinement (plan D5):** debt is per SHAPE (`formae_debitae`
bitmask), not per vector. Vector-level debt would have hidden the
INTEGRA/BIPARTITA/SEQUENS assertions of these 21 vectors, and those are
real, passing tests.

Planted faults, both directions: a flipped expectation (sursum →
deorsum) failed by name in exactly integra, bipartita 1 and sequens (the
singula shape stayed "debitum manet", correctly); a passing shape falsely
marked owed failed with `DEBITUM SOLUTUM - promove: "a" [integra]`.

**Tool incident, not tessera:** a python transform of `vectores_initus.h`
piped into `./silva/scribe.sh` writing the SAME file truncated it (the
classic `cat f | … > f` race). scribe then wrote 0 bytes and examen said
ACCIPE. Recovered from the conversation (the file was untracked). Rule:
transform to a scratch file, then scribe from it. Filed as a question in
the tabularium (scribe should refuse empty stdin).

## 2026-09-28: T1b, the H1 fix (reader loops while bytes arrive)

`tessera_eventum_expectare`: after INCOMPLETUM the reader now reads
**while** bytes keep arriving within `TESSERA_MORA_FUGAE_MS`. Only an
EMPTY read is the timeout (then the old path: lone ESC → FUGA,
otherwise drop one byte). The loop is bounded: each iteration adds bytes,
and the 64-byte buffer is finite (a full buffer is handled as a timeout).
The three copies of "parse, skipping consumed noise" became one helper,
`_parsare_plene`.

Proof, in order:
1. The 21 owed SINGULA shapes all went to `DEBITUM SOLUTUM - promove`,
   and nothing else moved: the red-before-green was T1's debt list.
2. Promoted to VALET (186 assertions, 0 debts). The existing eventum
   suite (68) is unchanged and green: behavior-preserving elsewhere.
3. Amalgam regenerated: VERIFICATUM (standalone full severity, hospes
   7/7, nm 0, censura), idempotent (md5 identical on a second run);
   only `tessera.c` changed. saltuarius 13/13 against it.
4. Planted fault that COMPILES: a `frange` at the end of the new loop's
   body (= the old one-extra-read behavior) → exactly 21 failures, all
   SINGULA. My FIRST plant (`dum` → `si`) did NOT compile (`frange`
   outside a loop), and the pipeline hid the build error behind a later
   green run. That's the house rule "gates born red by a plant THAT
   COMPILES", earned again: capture each run's log and check rc before
   reading a verdict.

Unchanged and still named for later: an overlong sequence that fills the
64-byte buffer is still flushed as a timeout (FUGA + the rest as phantom
keys). That's the tokenizer's job (bounded CSI with `csi_ignore`,
terminal-planning modules/002).

## 2026-09-28: T2, keypress vectors (OpenTUI parse.keypress.test.ts)

86 vectors in `VECTORES_CLAVIUM` (the file read in full, 2,020 lines).
Suite: 120 vectors, 434 shape runs, 656 assertions, 18 named debts.
Every prediction from reading `tessera_eventum.c` held except one (H6
below).
- **Same answer as OpenTUI:** letters/digits/symbols, `\x1c`–`\x1f` →
  ctrl+`\ ] ^ _`, NUL → ctrl+space, alt prefixes including alt+ctrl,
  F-keys with modifiers, every `1;m` combination, delete with modifiers,
  BEL = ctrl+g, 3- and 4-byte UTF-8 (U+4E2D, U+1F44D).
- **Documented losses, asserted:** shift invisible on letters; LF =
  Enter; the super bit (8) dropped; old-style `ESC F`/`ESC B` = alt+letter
  (not arrows); ESC ESC = two FUGA (not meta+escape); rxvt `CSI a`,
  `CSI 2^`, `SS3 a`, keypad `SS3 p`/`SS3 M` and a lone 8-bit meta byte
  (0xA0) consumed silently.
- **Noise consumed silently (no phantom keys):** CSI t size replies,
  CPR, DA, DECRPM (`$` intermediate), focus I/O, paste markers
  200~/201~, kitty `u`, modifyOtherKeys `27;m;c~`.
- **Debts:** OSC replies (H2, ×2 vectors, all shapes); ESC-prefixed
  sequences (`ESC ESC [A` → spurious FUGA + unmodified key; desired
  alt+key is a DESIGN CALL for Fran, recorded in the causa).
- **Skipped with a reason in the header:** rxvt `\033[2$` (`$` is an
  intermediate per Williams, so the sequence swallows the NEXT key;
  expected behavior depends on the tokenizer → T4); mouse (T3); partial
  and ESC-less continuations (T4).

**H6, found by the SEQUENS shape (new):** a lone ESC left pending as the
LAST byte of a read (after `ESC ESC`, or `x` + ESC in one read) waits the
CALLER's timeout (`mora_ms`) in the next call's first read, and only then
the escape timeout. A key arriving within `mora_ms` + ~25 ms is merged
into alt+key. saltuarius passes 250 ms, so Esc, Esc, `j` typed quickly
gives FUGA, alt+j. The fix is in the same function: when the buffer
already holds INCOMPLETUM data on entry, skip the caller-timeout read and
go straight to the escape-timeout loop. Two vectors own it (SEQUENS
debt): the ESC ESC loss vector and a direct `x`+ESC one. Proposed as T2b.

## 2026-09-28: T2b, the H6 fix (a pending ESC gets only the escape timeout)

The caller-timeout read (`mora_ms`) now happens ONLY when the buffer is
VACUUM after the first parse. Bytes already pending (INCOMPLETUM, e.g.
a lone ESC that was the last byte of the previous read) go straight to
the escape-timeout loop. Semantics: `mora_ms` means "how long to wait
for NEW input"; a partial sequence already in hand gets ~25 ms to
complete.

Proof: exactly the two owed SEQUENS shapes went to `DEBITUM SOLUTUM`
and nothing else moved; promoted (16 debts remain: H2 ×8, the ESC
prefix ×8). Suite 6/6. Amalgam VERIFICATUM + idempotent (only
`tessera.c`), saltuarius 13/13. A compiling plant (`fructus ==
PARS_VACUUM` → `!= PARS_COMPLETUM`, the old unconditional read) failed
exactly the two H6 shapes; rc checked this time, not a build error.

Also recorded: Fran DECIDED (2026-09-28) that an ESC-prefixed sequence
(`ESC ESC [A`) means key + ALTERUM, consistent with ESC + key = alt.
The causa now says so. The fix still belongs to the tokenizer (002).

## 2026-09-28: T3, mouse vectors (OpenTUI parse.mouse.test.ts)

35 vectors in `VECTORES_MURIUM` (file read in full, 545 lines). Suite:
155 vectors, 574 shape runs, 768 assertions, 116 named debts (29 debt
vectors). All 10 VALET held: SGR press/release of three buttons, wheel
up/down, 1→0 coordinates incl. 500,300, three-wheel chunk.

**Debts, each VERIFIED to fail for the predicted reason** (a diagnostic
run with the mouse debts temporarily VALET printed tessera's actual
output, then restored):
- **modifiers dropped** (`CAUSA_MUS_MODIFICATORES`): SGR bits 4/8/16
  are never copied into `modificatores`; shift/alt/ctrl+click = a plain
  click. Obvious mapping (4→MAI, 8→ALT, 16→IMP), small fix.
- **wheel release** (`CAUSA_ROTA_SOLUTA`): `64…m` reported as a second
  wheel-up (bit 64 tested before M/m).
- **horizontal wheel** (`CAUSA_ROTA_LATERALIS`): 66 → wheel-UP, 67 →
  wheel-DOWN (`pulsus & 1`). A sideways trackpad swipe scrolls
  vertically. Desired is PROVISIONAL (nothing): a design call for Fran
  (new genera `ROTA_SINISTRORSUM`/`DEXTRORSUM`, or ignore).
- **H4 motion** (bit 32): drag → press/release events, motion+wheel
  (96/97) → wheel events. Desired: consumed (tessera doesn't request
  motion); genus MOTUS arrives with `?1002` (terminal-planning 013).
- **H3 X10:** `ESC [ M` consumed as an unknown CSI, three payload bytes
  → phantom keys. Worse than predicted: at column/row 94 the payload
  byte is 0x7F → **two phantom Backspace** events. A click can delete
  text. Desired is PROVISIONAL (parse like SGR; release = pulsus III,
  unknown button): a design call for Fran (parse vs swallow).

Skipped with reasons in the header: OpenTUI's button-tracking state
(drag vs move needs state tessera doesn't keep), incomplete-SGR framing
(T4), the JS utf8 `toString` limit (not a protocol matter).

## 2026-09-28: T3b, mouse fixes (modifiers, wheel release, horizontal wheel, motion, X10)

One classifier, `_murem_classificare(ev, pulsus, x, y, solutio)`, serves
SGR and X10. It writes event fields ONLY on COMPLETUM, so a swallowed
motion event leaves nothing stale for the next key event.
- **modifiers:** bits 4/8/16 → MAIUSCULA/ALTERUM/IMPERIUM on MUS events.
- **wheel release** (`64…m`): swallowed (a wheel has no release).
- **horizontal wheel, DECIDED by Fran 2026-09-28:** new genera
  `TESSERA_MUS_ROTA_SINISTRORSUM` (66) / `DEXTRORSUM` (67), APPENDED to
  `TesseraMusGenus` (existing values unchanged), in both
  `fontes/tessera_eventum.h` and the hand-written `amalgama/tessera.h`.
  saltuarius's switch has an `ordinarius` (horizontal ignored there,
  correctly); spectaculum compares with PRESSUS only.
- **motion (bit 32, incl. 96/97):** swallowed. Tessera doesn't request
  motion; a MOTUS genus arrives with `?1002` (terminal-planning 013), and
  the H4 vectors' expectations will change then (noted in the table).
- **X10, DECIDED by Fran 2026-09-28 (parse):** `ESC [ M` + three raw
  bytes (+32/+33/+33) straight after `M`; INCOMPLETUM until 6 bytes
  (works with byte-at-a-time since T1b); a payload below the offsets is
  consumed as malformed; release = button 3 → SOLUTUS, `mus_pulsus` III
  (unknown button). No more phantom keys, including the two phantom
  Backspaces a click at coordinate 94 used to produce.

Proof: 25 debt vectors × 4 shapes = exactly 100 `DEBITUM SOLUTUM`,
nothing else moved; promoted (868 assertions; 16 debts remain, H2 + ESC
prefix). Amalgam VERIFICATUM (hospes 7/7 against the CHANGED public
header) + idempotent; saltuarius 13/13. Three compiling plants, one per
mechanism, each failing exactly its own vectors: motion (7), X10 (9),
mouse ctrl (3).

## 2026-09-28: T4, framing vectors (OpenTUI stdin-parser.test.ts)

83 vectors in `VECTORES_FRUSTORUM` (file read in full, 2,579 lines).
Suite: 238 vectors, 902 shape runs, 1,258 assertions, 120 named debts.

**Harness extension: `MORA`** (`"\xFE"`, never valid UTF-8) inside a
vector = a timeout at that point. The test pons strips it and inserts
an empty chunk, and the shapes cut around it. `_pontem_parare` became a
general chunk builder (cut points per shape + mora points). The
refactor reproduced 868/868 with 16 debts BEFORE any new vector used it.
A discriminating VALET vector (ESC, mora, b → FUGA, b; without the mora
it's alt+b) guards the builder; a plant that ignores markers failed it
in all four shapes (and falsely "solved" the SGR-split debt, which the
DEBITUM SOLUTUM check also caught).

**VALET (all predictions held):** full ctrl-letter table, symbol runs,
multi-rune UTF-8, alt+DEL/BS/digit/`!`, F2–F11 in both forms,
modifier-arrow grids, foreign forms already consumed (CSI E, SS3 E, rxvt
7~/8~/b-d/^), kitty `:`/u and modifyOtherKeys noise, long DA1, DECRPM,
focus between keys, SGR/X10 chunks (drag and motion silent), CSI aborted
by an embedded ESC (tessera does this right), ESC chains across
timeouts, every mixed-stream pair across every split, `[<35;5m` without
ESC = literal keys, invalid UTF-8 leads dropped (OpenTUI's 8-bit meta
is a loss).

**Debts, each verified by a diagnostic run (debts temporarily VALET):**
- **H2 widened:** DCS (`ESC P`) and APC (`ESC _`) as well as OSC →
  alt+`]`/`P`/`_` + payload as keys (long replies overflow the 16-event
  capture).
- **H7 (new):** a PARTIAL sequence that times out is replayed as FUGA +
  phantom keys (`\033[123` → fuga `[ 1 2 3`; x10 partial; OSC/DCS/APC
  partials as alt+X + keys). Proposed policy (Fran to confirm): discard
  silently, except the 2-byte `ESC O` / `ESC [` → alt+O / alt+[ (xterm
  sends ESC O for Alt+O).
- **H8 (new):** a mouse report split across a timeout (slow ssh) →
  FUGA + phantoms, and the continuation phantoms too. OpenTUI holds
  mouse partials and recovers ESC-less continuations after a flushed ESC.
- **SS3 swallows an embedded ESC** as its "final": `ESC O ESC O A` →
  `O A` phantoms. CSI aborts correctly on ESC; SS3 doesn't.
- **Foreign forms with a phantom tail:** Linux console `\033[[A` → `A`,
  putty `\033[[5~` → `5~`, rxvt `\033[2$` + timeout → fuga `[2$`.
  Proposed: consume silently like the other foreign forms.
- **CSI parameter overflow = undefined behavior:** parameters accumulate
  in an unbounded `s32`. Hostile input overflows (UB), and in practice
  `1;4294967301A` wraps to 5 → **ctrl+up**. Fix: cap each parameter.

Skipped with reasons in the header: bracketed paste (needs a GLUTINUM
genus, feature 001), OpenTUI's protocol-context probes (tessera never
queries), parser-object lifecycle API.

## 2026-09-28: T4b, CSI parameter cap (UB) + SS3 aborts on an embedded ESC

- **Parameter cap:** `PARAMETRUM_MAXIMUM` = X * M (10,000). The digit
  accumulator stops multiplying once past the cap, so a parameter can
  never exceed 100,009 and the signed-overflow UB on hostile input is
  gone BY CONSTRUCTION. A value above the cap = "huge" = invalid:
  `_modificatores_csi` returns no modifiers for it, so
  `1;4294967301A` is a plain up-arrow, not ctrl+up. Tilde codes and
  mouse coordinates degrade naturally (no match, or a clamped
  coordinate). The no-UB property itself isn't observable by a test; a
  sanitizer build (-fsanitize=undefined) would be the oracle for it,
  not done here.
- **SS3 + embedded ESC:** a third byte of 0x1B aborts SS3. `ESC O` is
  dropped silently and the ESC starts a fresh sequence, as CSI already
  did (`ESC O ESC O A` and `ESC O ESC [ A` → up).

Proof: exactly 12 `DEBITUM SOLUTUM` (3 vectors × 4 shapes), nothing
else moved; promoted, 2 causes retired (1,270 assertions; 108 debts
remain, all tokenizer territory: H2, H7, H8, foreign forms, ESC prefix).
Amalgam VERIFICATUM + idempotent; saltuarius 13/13. Compiling plants:
SS3 abort disabled → exactly the 2 SS3 vectors; huge-modifier check
disabled → exactly the overflow vector.

## 2026-09-28: tessera 1.2 T2, synchronized output (?2026) (opus, not eventum, but the log lives here)

`tessera_praesentare` prepends `QUADRUM_INITIUM` and, only if
anything followed, appends `QUADRUM_FINIS` and writes; a no-op frame
still writes ZERO bytes. `?2026l` is FIRST in EXEUNDI (crash safety) and
in `tessera_intermittere`. `probatio_tessera_modi` gained rule V
(per-frame modes: never in INTRANDI, always first in EXEUNDI). Goldens
updated by hand as explicit `"\033[?2026h" "<old>" "\033[?2026l"`: opus
×6, eventum ×2, hospes ×1, AND saltuarius's pledge golden
(`probatio_saltuarius_salve.c:88`), which the plan's list MISSED. It
surfaced as saltuarius 12/13; a `grep -F '\033[?25l'` sweep then
confirmed nothing else pins tessera's frame bytes. Three compiling
plants caught (EXEUNDI without ?2026l → rule V by name; empty frame
written → the zero-byte assertions; close marker dropped → all 8
goldens). **Fran's terminal look (2026-09-28): everything works**
(spectaculum; no escape garbage, redraws, intermittere, Ctrl-Z/fg,
quit).

## 2026-09-28: tessera 1.2 T3, drag events (?1002)

- `INTRANDI` = 1049 1000 **1002** 1006; `EXEUNDI` leaves 2026 1006
  **1002** 1000 1049 (stack order, law-of-pairs probatio green; rule IV
  now requires 1002).
- `TESSERA_MUS_TRACTUS` APPENDED to `TesseraMusGenus` (both headers), so
  no existing value moved.
- Classifier rule, stateless: bit 32 + button 0–2 → TRACTUS,
  `mus_pulsus` = the held button, modifiers kept, and the M/m final
  IGNORED for motion. OpenTUI's own vectors encode drags with `m`
  (their encoder's `press=false`) and still call them motion; xterm
  always sends `M`. OpenTUI keeps a pressed-button set to tell "drag"
  from "move"; tessera doesn't need one, because under ?1002 the
  terminal reports motion ONLY while a button is held. Swallowed: button 3 +
  motion (35/39, what ?1003 hover would send, never requested) and
  motion + wheel bits (96/97, URxvt). X10 drags (`ESC [ M @ ..`) come
  through the same classifier and work for free.
- Vectors: 4 H4 expectations flipped (announced in the HISTORIA note
  since T3b), 7 added (middle/right/ctrl drags, shift+motion 39 swallowed,
  a full press / drag ×2 / release stroke, 2 X10 drags). Red first was
  exactly the 10 drag vectors × 4 shapes + modi's 1002.
- Plants (all compiling, each caught by name): TRACTUS branch → swallow
  = exactly the 10 drag vectors; `botton == III` guard dropped =
  exactly 35/39/x10-35; `?1002h` dropped from INTRANDI = modi rules II,
  III, IV by name ("?1002 relictus, numquam intratus").
- HAZARD found: `tessera/spectaculum.sh` rebuilds an object only when
  its `.c` is newer. It ignores headers (compile_probationes.sh has
  `newest_header`, spectaculum.sh doesn't). A change to `tessera_modi.h`
  alone would leave spectaculum running the OLD mode strings; it's
  only fresh here because the test runner shares `tessera/build/`. A
  planted run also leaves planted objects behind, so rebuild after
  plants (done here). Not fixed in T3.

## 2026-09-28: tessera 1.2 T4, paste API + debts

- The paste body is a VIEW (`chorda glutinum`) into the lector's
  collector: valid until the next `tessera_eventum_expectare`. The
  vector harness honors this by copying the body into its piscina at
  observation time. Without the copy, back-to-back pastes would both
  point at the collector and show the second body twice. Any consumer
  that queues events must do the same.
- The harness's pons ignores the timeout the reader asks for, so a
  `MORA` inside a paste means silence (D4's 3 s), NOT a 25 ms gap. The
  D4 vectors are written that way. A short mid-paste gap can't be
  expressed in the table; a T5 probatio can check the timeout value
  the reader passes.
- Public macro values must be literal (`65536`, not `LXIV * MXXIV`):
  the hand-written amalgam header redefines them, and redefinition has
  to be token-identical (that's the drift guard; plant II proved it).
- T3 slip: `MURIUM_TITULI` in the harness lacked "tractus" (index 6),
  so the failure print read out of bounds for TRACTUS. Fixed here. An
  enum-indexed name table should be checked whenever the enum grows.

## 2026-09-28: tessera 1.2 T5, bracketed paste implemented

- Shape: `_csi_parsare` reports `PARS_GLUTINUM` for `CSI 200 ~` (exactly
  one parameter; `200;5~` stays swallowed noise). `_parsare_plene` runs
  `_glutinum_colligere` at once, and it ALWAYS sets one GLUTINUM event, so
  none of `expectare`'s three parse sites changed. A paste can't span two
  `expectare` calls, so the lector carries no paste-mode flag, only the
  collector pointer.
- End-marker match: a prefix counter over `ESC [ 2 0 1 ~`. On a mismatch
  the matched prefix goes to the body and the byte is retried only if
  it's ESC (the marker has ESC only at its head, so no fuller KMP is
  needed). The counter lives across reads. On silence, a dangling prefix
  becomes body (`ab ESC[20` → body "ab\033[20", truncated).
- Reads stay staged through the 64-byte lector buffer; the body is
  copied to the collector. Bytes after the marker are simply still in
  the lector buffer, so normal parsing resumes with no hand-off. The
  cost: 1 MB through the memoria pons takes ~2.7 ms (16k `legere` calls).
  The posix pons adds a select+read per 64 bytes; if a real 1 MB paste
  ever feels slow, read straight into the collector and move the tail
  back.
- Inside a paste the reader ignores the caller's timeout and reads with
  `TESSERA_MORA_GLUTINI_MS`. A caller polling with a short timeout can
  therefore block up to 3 s per silent gap mid-paste. That's deliberate
  (D4) and documented in the header.
- Every lector now allocates 64 KiB at creation. The vector harness
  creates a lector per shape run (thousands), so it now rolls its piscina
  back per run (`piscina_notare`/`piscina_reficere`); the binary peaks
  at 1.8 MB.
- Test trap: the zero-allocation assertion first compared
  `piscina_summa_usus` across several CREDOs, but credo writes failure
  records into the same piscina. Measure around the ONE call, assert
  after.
- Plants (six, all compiling): start ignored / no ESC retry / truncation
  flag never set / caller's timeout inside a paste / dangling prefix
  dropped / `?2004l` missing. Each caught exactly by its own vectors or
  assertions.

## 2026-09-28: runae U5, wide cells (opus, logged here with the rest of 1.2)

- **A plant found dead code.** My first emission set `pos_x = x + 1`
  after a wide cell, then skipped its continuation. So the next emitted
  cell was never contiguous and got a CUP anyway, and the explicit
  containment line (`pos_x = -1`) did nothing: removing it left the
  golden green. Fix: model the agreeing terminal honestly (`pos_x = x +
  2` after a wide cell), so containment is the ONLY thing producing that
  CUP. Now the plant turns the first-frame golden red.
- **A plant that didn't compile.** `pos_x = pos_x;` is -Wself-assign →
  error; its red meant nothing (the house rule caught it: check the
  `error:` count). A compiling no-op (`(vacuum)ZEPHYRUM;`) did the job.
- **The amalgam leaked the width tables.** `RUNAE_GRADUS_PRIMUS/SECUNDUS`
  are DATA; the amalgamator's function prefixes don't touch them, so
  they were exported under their plain names (nm), and a host linking
  both tessera and runae would hit duplicate symbols. The verification
  passed only because hospes doesn't link runae. Fixed with exact
  renames (`TYPI_EXACTI`) to `TESSERA_RUNAE_GRADUS_*`. Any future
  vendored library with global data needs the same.
- **examen and pythonica disagree on the amalgamator** (pre-existing): an
  ABSOLUTE path makes examen resolve `#include "fontes_generata.h"` to
  silva's file of the same name. `Editio.applicare` therefore reports
  REICE for amalgamator.c while `examen.sh` with a relative path says
  ACCIPE. Filed as quaestio …0VV1K (residue of 01KYJ6740K's fix).
- **Editio + comments:** a `tolerans='verba'` replace inside a comment
  REFLOWS it (my two comment lines were joined into one 124-column line,
  with a stray leading space). Check `formator -vitia` after such an
  edit.
- The amalgam grew 230 KB → 372 KB (the width table, ~40 KB of data as
  decimal source). The unused-function harvest settled at 4 rounds;
  tessera uses only `runae_latitudo`.

## 2026-09-28: runae U5b, grapheme cells (opus)

- Blanking bug caught at design time: `_dimidium_solvere` sets signum 0 on
  a wide start; if that start was a GRAPHEMA cell the marker survived,
  and signum 0 then reads as cluster ID 0 (someone else's bytes). Blanking
  now clears GRAPHEMA; a test and a plant pin it.
- Editio refuses a multi-line anchor whose replacement has a different
  line/token shape ("forma perderetur"); plants must keep the shape (swap
  one operand, e.g. `| ZEPHYRUM`).
- The emission buffer is still pre-sized at ~20 bytes per cell; a screen
  dense with long clusters can grow it once (a bounded arena leak). Noted,
  not tuned.
- Terminal.app vs Ghostty measured (phase-log U5b FACTUM): Terminal.app
  sums codepoint widths (Mc = 0) and doesn't join ZWJ families. Its
  rendering of the REST of such a row shifts one cell even though CUP puts
  the cursor right: containment can't fix a terminal's own row layout.
