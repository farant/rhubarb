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
