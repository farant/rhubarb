# tessera 1.2 — plan (bracketed paste, synchronized output, drag events)

**PERFECTUM 2026-09-28** (T1 9a8d51ab, T2 cbc10f3b, T3 59fa1121, T4
f436b629, T5 44c05466; RELATIO in tessera/phase-log.md).

*2026-09-28. terminal-planning features/001 (bracketed paste), 002
(synchronized output), and the `?1002` drag events that ludus-on-tessera
(modules/013) needs. Worktree `../rhubarb-secunda`. Executed INLINE, one
task per turn, Fran approving each; tasks with a terminal step end with
Fran's own look before the commit. Names marked (unsealed) are working
names; Fran names.*

## 1. Goal

Three additions, each a mode tessera SETS (never queries, so the thesis
holds: a terminal that doesn't know a mode ignores it):

| mode | gives | today |
|---|---|---|
| `?2004` bracketed paste | a paste arrives as ONE event, not as keystrokes | pasted text IS keystrokes. In saltuarius (vim-style: bare letters are commands) **a paste executes commands** |
| `?2026` synchronized output | the terminal shows each frame whole | one `write` per frame, but large frames can still tear mid-paint |
| `?1002` button-event tracking | motion while a button is held = drag events | no drag at all; pictor strokes, sliders, selections impossible |

Every change goes the way of T1–T4 of the vectors plan: failing
vectors (or goldens) first, amalgam VERIFICATUM + idempotent, saltuarius
green, a planted fault that COMPILES, caught exactly.

## 2. What exists (read 2026-09-28)

- `tessera/fontes/tessera_pons_posix.c:22–23`: `INTRANDI` =
  `?1049h ?1000h ?1006h`, `EXEUNDI` = `?1006l ?1000l ?1049l` + SGR reset
  + cursor show. `EXEUNDI` is written from `_egredi_posix`, from the
  fatal-signal and SIGTSTP handlers (async-signal-safe, the one
  sanctioned static), and `INTRANDI` again from the SIGCONT handler. **These strings sit below
  the test seam**: no memoria-pons golden can see them.
- `tessera_praesentare` (`tessera_opus.c`): builds the frame into one
  aedificator (pre-sized worst case + 1024 slack), ONE `pons->scribere`;
  a no-op frame writes ZERO bytes (golden). `tessera_intermittere`
  writes `\033[0m\033[?25h` then `egredi`.
- Byte goldens that will change with sync output: `probatio_tessera_opus`
  (7 string assertions), `probatio_tessera_eventum` (the
  intermittere/resumere one), `probatio_tessera_salve` (3),
  `probatio_tessera_pons` (2), and **hospes** in the amalgam gate
  (`instrumenta/principalia/hospes.c:69`, "golden first frame"). Each is
  checked by hand, not bulk-replaced.
- The reader classifies motion (bit 32) as swallowed since T3b; the
  table says the expectations change when `?1002` arrives.
- OpenTUI's bracketed-paste vectors (`stdin-parser.test.ts:1699–1847`)
  were skipped in T4 because no paste genus existed.

## 3. Decisions to make before T1 (proposed)

**D1: paste event shape.** ONE event per paste:
`TESSERA_EVENTUM_GLUTINUM` (unsealed) with `chorda glutinum` (a view
valid until the next `tessera_eventum_expectare`, the vitrea
"handle it now, never store it" contract) and `b32 glutinum_truncatum`.
The collector is pre-sized at lector creation (default 64 KiB, unsealed);
past the cap it keeps draining to the end marker and flags truncation.
Steady state allocates nothing. The alternative, a chunk stream
(INITIUM / chunk / FINIS), needs no cap but makes every consumer
reassemble. OpenTUI does one event.

**D2: where the mode strings live.** Proposed middle path: move
`INTRANDI`/`EXEUNDI` into one internal header (`fontes/tessera_modi.h`,
unsealed), still used by the posix pons (the signal path needs them as
static strings), and add a STRUCTURAL probatio: every `?Nh` in
`INTRANDI` has its `?Nl` in `EXEUNDI`, and the expected set of modes is
present. That's headless, catches the "entered a mode, forgot to leave
it on crash" bug the feature notes warn about, and needs no pons
refactor. The alternative (escapes in the opus, pons = termios only)
still needs a static copy for crash restore, so it's two sources of
truth.

**D3: drag genus.** `TESSERA_MUS_TRACTUS` (unsealed), appended to
`TesseraMusGenus`, `mus_pulsus` = held button. The name deliberately
avoids "motus" (sealed in ludus for the kinetic state struct). Only
`?1002` (motion WITH a button held). `?1003` (all motion, hover) stays
off: it costs bytes on every mouse move and has no consumer yet. So code
35 (motion without a button) stays swallowed, and 96/97 stay swallowed.

**D4: an unterminated paste.** OpenTUI waits forever for the end
marker. Proposed: a silence timeout (3 s, unsealed) ends the paste and
delivers it as truncated. A buggy terminal or a dropped marker then
can't freeze input for good.

**D5: sync unconditional.** `?2026h`/`?2026l` around every non-empty
frame, no probe (terminals ignore unknown private modes; OpenTUI probes
with DECRQM, tessera refuses to). `?2026l` is added to `EXEUNDI` so a
crash mid-frame can't leave a supporting terminal frozen. An
environment opt-out only if a terminal turns out to misbehave (not
built).

## 4. Tasks

**T1: mode strings in one place + the structural probatio (no behavior
change).** `fontes/tessera_modi.h` with `INTRANDI`/`EXEUNDI`; posix
pons includes it; `probatio_tessera_modi.c` asserts pairing and the
mode set. Planted fault: an `EXEUNDI` missing one `l` → red by name.
Amalgam regenerated (internal header joins the amalgam; output bytes
identical).

**T2: synchronized output (`?2026`).** `tessera_praesentare` wraps
non-empty frames; a no-op frame stays ZERO bytes; `EXEUNDI` gains
`?2026l`; `tessera_intermittere` emits `?2026l` too (cheap insurance
before `$EDITOR`). Goldens updated one by one (listed in §2), hospes
too. **Terminal step:** `spectaculum` in Terminal.app (the likeliest
non-supporter) and one supporting terminal: no escape garbage, no
freeze across Ctrl-Z / `fg`. Fran looks before the commit.

**T3: drag events (`?1002`).** `INTRANDI` `?1002h` (with `?1000h`),
`EXEUNDI` `?1002l`; `TESSERA_MUS_TRACTUS` appended (source header and
hand-written amalgam header); `_murem_classificare`: bit 32 with button
0–2 → TRACTUS, button 3 / 96 / 97 still swallowed. H4 vectors'
expectations change as announced; new drag vectors (press, drag ×2,
release, in every shape). saltuarius's `ordinarius` ignores TRACTUS
(checked). **Terminal step:** drag in spectaculum; its status line shows
the genus.

**T4: paste API + failing vectors.** `TESSERA_EVENTUM_GLUTINUM`,
`glutinum`, `glutinum_truncatum` in `TesseraEventum` (both headers);
the harness's `EventumExspectatum` gains the body bytes; port OpenTUI's
paste vectors (simple, empty, newlines, tabs, ESC in body, split start
marker at every boundary (a shape), split end marker (a shape),
near-match `\033[202~` in body, doubled ESC before the end marker,
trailing keys after, back-to-back pastes, UTF-8 body split) as DEBITUM
(`CAUSA_GLUTINUM`, unsealed). Nothing implemented yet: red first.

**T5: paste implementation.** `?2004h`/`?2004l` in the mode strings;
the reader enters paste mode on `CSI 200 ~`, collects into the pre-sized
aedificator with an end-marker tail scanned across reads (the collector
never uses the 64-byte lector buffer for the body), leaves on
`CSI 201 ~`, delivers one GLUTINUM event; timeouts don't end a paste
except D4's silence timeout; the cap sets `truncatum` and keeps
draining. Debts promoted. A saltuarius probatio: paste bytes in → no
command issued (the safety gap closed). **Terminal step:** paste a
multi-line text with `dd` in it into saltuarius: nothing executes.

**T6: RELATIO.** Phase-log; tessera-spec-v2 §6 deferral list updated
(paste, sync, drag no longer deferred); terminal-planning features/001,
002 → done; ledger; merge to main.

## 5. Names to seal (Fran)

`tessera_modi.h`, `TESSERA_EVENTUM_GLUTINUM`, `glutinum`,
`glutinum_truncatum`, `TESSERA_MUS_TRACTUS`, `CAUSA_GLUTINUM`, the
64 KiB cap and 3 s silence timeout. None is a `latina.h` macro
(checked).

## 6. AUDIENDA

- Whether macOS Terminal.app ignores `?2026h` silently (expected, not
  verified). T2's terminal step measures it.
- Whether any terminal sends motion with `?1002` but WITHOUT `?1006`
  honored (i.e. X10 drags). The X10 path handles bit 32 the same way,
  so it should just work. Unverified.
- Paste throughput for a large paste through the 64-byte read loop: one
  `legere` per chunk, unmeasured. T5 records the time for a 1 MB paste
  through the memoria pons.
