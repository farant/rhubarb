# vicus quies — idle at (almost) no cost Implementation Plan

> **For agentic workers:** this repo's standing method (Fran): tasks are
> executed INLINE in the main session, one task per turn, Fran approves
> each before the next; no implementation subagents. Steps use checkbox
> (`- [ ]`) syntax for tracking.

**Goal:** an idle vicus (nothing typed, no output streaming, nothing
animating) sleeps until something is actually due - the minute clock, a
notification, a periodic check - instead of waking ~60 times a second.
Input and terminal output still feel instant. macOS should stop calling
it out for energy use.

**Architecture:** traced 2026-10-26 (sample of a running vicus: ~3.5%
CPU idle, main thread asleep in `fenestra_expectare_eventus` most of
the time, but woken every 16 ms):
- `ludus_fenestra_currere` (`lib/ludus_fenestra.c:239`) caps the wait at
  `MORA_PULSUS` = 16 ms whenever the app has a pulse function - vicus
  always does - so the loop wakes ~60/s forever and runs `vicus_pulsare`
  each time (frames are only PAINTED when something changed; the
  wake-ups are the cost).
- `vicus_pulsare` (`lib/vicus.c` ~3925) every pulse: clock + battery
  (`horam_legere`, `pilam_legere`), host systems (admonitiones,
  terminalia, sonus), pending opens, every visible pane's pulse
  (terminal: non-blocking pty read), companion follow, and route
  bookkeeping (`semitam_restituere`/`semitam_servare`,
  `historiam_tabulae_notare` - rebuilding STML routes of all panes).
- `systema_terminalium` (`lib/vicus_applicatio.c` ~3193) every 5 s runs
  `tmux list-clients` per tmux server through the BLOCKING
  `multiplexor_clientes` -> `processus_metere` (~100 of 120 pulse
  samples; freezes the UI briefly).
- What already exists for the fix: the reactor computes "time until my
  next timer" internally (`_calculare_timer_timeout`, 0 if a task is
  pending); the pty exposes its master fd (`Pseudoterminale.fossa`,
  "so the host loop can wait on it", decision XIV); the macOS wait
  already wakes on a synthetic `ApplicationDefined` event (the WebKit
  bridge uses it); the dispensator already reports its own next timer
  (`quies_ms`, used by `ludus_fenestra_mora`); `processus_incipere`
  runs a child asynchronously.

**Tech Stack:** C89 in Latin; `reactor`, `ludus_fenestra`, `fenestra`
(macOS `fenestra_macos.m`), `vicus`/`vicus_applicatio`, `multiplexor`,
`processus`, `pseudoterminale`, credo v2.

**Background:** chat 2026-10-26 ("house libuv" discussion): stage it,
consumer-driven; no threads (blocking work -> child processes); the
reactor stays the one event library. Plan approved by Fran 2026-10-26:
"file descriptors sound good for now, i don't think we have blinking
cursors anywhere right now anyway? and cheap wins first sounds good."

## Global Constraints

- No threads. Single loop; blocking work goes to child processes.
- Android and iOS keep their own frame drivers (choreographer,
  CADisplayLink): only the macOS desktop wait changes; the shared code
  stays behind the platform layer.
- Apps WITHOUT a pulse function behave exactly as today.
- Input latency and terminal streaming must not regress (tests below
  pin both).
- Same C89/Latin/lint/examen rules; commit only on Fran's word.

## Decisions taken in this plan (check these)

1. **Deadline, not tick.** A pulse function may also report "when do I
   next need to run" (ms). The loop waits for min(that, the reactor's
   next timer, the dispensator's `quies_ms`) or an input event or a
   ready fd - no fixed 16 ms. An app that reports nothing keeps the old
   16 ms (back-compatible).
2. **Fds via the reactor; macOS wakes on them.** vicus registers pty
   fds with its reactor (it already owns one). The reactor exposes its
   registered fds (visitor) and a version number; the macOS window wait
   watches them with `CFFileDescriptor`, whose callback posts the
   existing synthetic `ApplicationDefined` event. The reactor's backend
   stays `poll()` (a kqueue backend is NOT in this plan - Open
   Question 1).
3. **Periodic jobs say when they're due.** Each vicus host system and
   the clock report a next due time; `vicus_mora_proxima` is the
   minimum. Clock: next minute boundary. Battery: next minute. tmux
   query: 5 s - only while a terminal or `$sessions` pane exists.
   Notifications: their next due time. A system that is BUSY (sound
   playing, a `$question` streaming) reports 16 ms while busy.
4. **tmux query goes async** (`processus_incipere`, pipe registered in
   the reactor) - no more UI freeze.
5. **Bookkeeping only on change.** Route save/restore/history run when
   the insula repository's version moved since the last pulse, not
   every pulse.
6. **Measure before and after, in-app.** `ludus_fenestra` counts loop
   wake-ups and pulses; `vicus -quies` prints per-10-second figures to
   stderr. External check: `top`/`sample` (Claude), `powermetrics`
   (needs sudo: Fran).

## Header text (for approval)

`include/reactor.h` - additions:

```c
/* mora usque ad timer proximum (ms): 0 si labor pendet aut timer iam
 * debitus; -1 si nullus timer. Pro hospite qui ansam suam habet
 * (fenestra): exspectatio = minimum huius et suarum. */
s32
reactor_mora_proxima (
    constans Reactor* reactor);

/* fossae activae cum eventibus petitis (REACTOR_LEGERE ...): hospes
 * eas ansae suae addit (macOS: CFFileDescriptor). Versio crescit omni
 * adiectione, mutatione, remotione - hospes per eam renovat. */
nomen vacuum (*ReactorFossaVisitor)(vacuum* ctx, s32 fd, i32 eventus);

vacuum
reactor_fossas_visitare (
     constans Reactor* reactor,
    ReactorFossaVisitor visitor,
                vacuum* ctx);

i32
reactor_versio_fossarum (
    constans Reactor* reactor);
```

`include/fenestra.h` - addition:

```c
/* ut fenestra_expectare_eventus, sed fossa ulla parata (legenda) quoque
 * expergefacit (macOS: CFFileDescriptor in ansa, eventus
 * ApplicationDefined). fossae renovantur solum cum versio mutatur. */
vacuum
fenestra_expectare_cum_fossis (
           Fenestra* fenestra,
                Mora ms_maximae,
    constans s32* fossae,
                i32 numerus,
                i32 versio);
```

`include/ludus_fenestra.h` - additions:

```c
/* quies (S-quies): mora usque ad pulsum proximum necessarium (ms; -1 =
 * nullus nisi eventus) et fossae quae expergefaciant. NIHIL = mos
 * vetus (XVI ms dum pulsator adest). */
nomen Mora (*LudusMoraProxima)(vacuum* ctx);
nomen i32  (*LudusFossae)(vacuum* ctx, s32* fossae, i32 capacitas,
                          i32* versio);

vacuum
ludus_fenestra_quietem_ponere (
      LudusFenestra* lf,
   LudusMoraProxima  mora,
        LudusFossae  fossae,
             vacuum* ctx);

/* mensurae: excitationes (circuitus ansae) et pulsus (pulsator vocatus)
 * ab initio - S-quies T1 */
```

`include/vicus.h` - additions:

```c
/* quies: mora usque ad rem proximam debitam (horologium, pila,
 * systemata, latera; XVI dum systema occupatum) - ms */
Mora
vicus_mora_proxima (
    Vicus* v);

/* systema cum mora sua: ut vicus_systema_addere, et mora(ctx) =
 * proximum tempus debitum (ms; -1 = nullum; XVI = occupatum) */
b32
vicus_systema_cum_mora_addere (
                  Vicus* v,
     constans character* titulus,
           VicusSystema  systema,
                   Mora  (*mora)(vacuum* ctx),
                 vacuum* ctx);
```

## Review Focus

1. **Idle wake-ups:** before vs after, same layout (scriba + lector +
   terminal pane, 60 s untouched): wake-ups/s and CPU% (Task 1 baseline,
   Task 7 after). Target: wake-ups ~1/min-ish plus the 5 s tmux check
   only when a terminal pane exists; CPU ~0%.
2. **Nothing gets slower:** a key press is handled on the next loop
   pass (no 16 ms floor added); terminal output streams (pty fd wakes
   the loop); clock still flips at the minute.
3. **Busy stays smooth:** while sound plays or `$question` streams, the
   16 ms cadence returns, and stops when done.
4. **Other apps unchanged:** pictor/scriba standalone and every app
   without `ludus_fenestra_quietem_ponere` behave as before.

---

### Task 1: measure (baseline)

**Files:** `include/ludus_fenestra.h`, `lib/ludus_fenestra.c`,
`apps/vicus/vicus.c`, tests.

- [x] `mensurae.excitationes` / `mensurae.pulsus` counters (test: N loop
  passes in a headless run count N); `vicus -quies` prints
  wake-ups/s, pulses/s, frames/s every 10 s to stderr.
- [x] Baseline: idle 60 s in the standard layout - `-quies` figures,
  `top` CPU%, a `sample`; Fran optionally `sudo powermetrics
  --samplers tasks -n 3` for wake-ups. Numbers into the worklog.
- Done 2026-10-26 (as built): vicus cannot see the LudusFenestra, so
  `-quies` prints pulses/s (= wake-ups/s: the pulsator runs every loop
  pass) and pulses-that-changed/s; frames and both counters are on the
  `ludus:` exit line. Baseline in lib/vicus.worklog.md (CPU ~5%).
  `excitationes` has no unit test (only the real window loop counts it).

### Task 2: tmux query async, only when needed

**Files:** `lib/vicus_applicatio.c` (`systema_terminalium`),
`lib/multiplexor.c` if it needs an async entry, tests.

- [x] Tests: no terminal/`$sessions` pane -> no tmux process is started;
  with one -> the query runs asynchronously (pulse returns at once,
  result applied on a later pulse).
- [x] Implement with `processus_incipere`.
- [x] Plant: blocking path restored.

### Task 2b: query tmux on foreground change (with Task 6)

Decided 2026-10-26 (Fran). The terminalia poll only keeps each terminal
pane's restore argument ("tmux <server> <session>") current;
notifications come from praeconium and the jump queries tmux live.

- [ ] Header: a pseudoterminale call returning the pty's foreground
  process group (`tcgetpgrp` on the master fd) - text for Fran first.
- [ ] Rule: hidden pane -> query only when its foreground process
  changes (catches detach / session killed while hidden); visible pane
  with a shell in front -> same; visible pane with tmux in front ->
  every 5 s (catches session switches inside tmux).
- [ ] Re-check the foreground on pty output (Task 6 puts the pty fd in
  the wait set) and on the 5 s timer for visible panes.
- [ ] Tests with the fake tmux: hidden pane, no foreground change -> no
  call; foreground change -> one call.
- Long-term alternative: tmux control mode (`tmux -C`) events, or a
  house multiplexer (no polling at all).

### Task 3: bookkeeping only on change

**Files:** `lib/vicus.c`, tests.

- [x] Tests: 100 pulses with no state change -> routes not rebuilt
  (counter); a change -> rebuilt once.
- [x] Implement (insula version since last pulse).
- [x] Plant: version check off.

### Task 4: reactor API

**Files:** `include/reactor.h`, `lib/reactor.c`, `probationes/
probatio_reactor.c`.

- [x] Tests: `reactor_mora_proxima` (none -> -1, timer in 250 ms ->
  ~250, pending task -> 0, due timer -> 0); `reactor_fossas_visitare`
  lists exactly the active fds with their events; version bumps on
  add/modify/remove only.
- [x] Implement (wrap `_calculare_timer_timeout`; fd table walk).
- [x] Plants: version not bumped on remove; task not forcing 0.
- Done 2026-10-26 (as built): visitor fd is `integer` (as
  `reactor_adicere`); the internal signal pipe IS visited (it registers
  through `reactor_adicere`, so it also bumps the version) - a signal
  wakes a sleeping host. Two sections in probatio_reactor.c.

### Task 5: deadline wait in the window loop (+ macOS fds)

**Files:** `include/fenestra.h`, `lib/fenestra_macos.m`,
`include/ludus_fenestra.h`, `lib/ludus_fenestra.c`, tests.

- [x] Tests (macOS, headless-capable where possible): a pipe registered
  as a wake fd -> writing to it ends a 5 s wait in well under 100 ms;
  no fd, deadline 200 ms -> the wait lasts ~200 ms; an app without
  `quietem_ponere` still waits <= 16 ms (old behaviour).
- [x] Implement: `CFFileDescriptor` per fd (re-synced only when the
  version changes), callback posts `ApplicationDefined`; loop wait =
  min(app deadline, dispensator `quies_ms`), no forced 16 ms.
- [x] Plants: fd callback not posting the event (the pipe test times
  out); old 16 ms cap left in -> MOVED TO T6 (the cap decision lives in
  `ludus_fenestra_currere`, which needs a real window; vicus's
  excitationes counter will show it).
- Done 2026-10-26 (as built, header approved with these changes):
  `fenestra_fossas_ponere` is process-wide (one run loop) and takes
  `FenestraFossa {fd, eventus}`; `fenestra_expectare_eventus(NIHIL, ms)`
  waits on the loop alone (tests, no window). `ludus_fenestra_
  quietem_ponere(lf, mora, ctx, reactor)` takes the Reactor directly
  (not an fd callback). Found while reading: `ludus_fenestra_mora`
  returned the dispensator's `quies_ms` (300) ALWAYS - new
  `dispensator_mora_proxima` gives the real flush deadline (-1 when
  nothing is pending), and a REFUSED flush restamps its time (retry
  after quies_ms) so quiet mode can't spin at 0 ms. If the reactor has
  > 128 fds or CF refuses, the loop falls back to 16 ms. New suite
  probatio_fenestra_fossae (first NSApp wait ~70 ms: warmed up before
  timing). Plants: 8 red (reactor T4 x2, dispensator x2, fenestra x2,
  ludus x2). android-struere + ios-struere run early: green.

### Task 6: vicus on deadlines

**Files:** `include/vicus.h`, `lib/vicus.c`, `lib/vicus_applicatio.c`,
`apps/vicus/vicus.c`, terminal pane glue, tests.

- [ ] Tests: `vicus_mora_proxima` - nothing due -> ms to next minute;
  notification due in 3 s -> ~3000; sound playing -> 16; a terminal
  pane's pty fd is among the wake fds; pty output arriving is read on
  the next pass.
- [ ] Implement: clock/battery/systems report due times
  (`vicus_systema_cum_mora_addere`), pty fds into the reactor,
  `apps/vicus/vicus.c` wires `ludus_fenestra_quietem_ponere`.
- [ ] Plants: clock deadline ignored (minute flip late), pty fd missing
  (streaming test stalls).
- [ ] Notifications log (decided 2026-10-26, Fran: option a): today
  `systema_admonitionum` opens and reads the praeconium file every
  pulse (~10% of idle awake time). Replace with a file watch that
  wakes the loop when the file is written (kqueue EVFILT_VNODE on
  macOS - `poll` reports regular files always readable), behind the
  platform layer; no timer.

### Task 6 split (decided 2026-10-26, Fran)

Unifying idea: every live part of vicus DECLARES its wake sources (a
deadline and/or an fd); anything undeclared keeps 16 ms. Each step is
safe alone; the gain shows once every pulsed part has declared (default
layout has a terminal -> visible after 6b).

Inventory (what each part needs): clock+battery -> next minute;
scriba/kalendarium/nomenclator -> nothing unless a job runs (16 while
busy); terminal (vivit_in_fundo) -> pty fd, 16 only while its input
queue is non-empty or the child exit is unreaped; volantes -> next
toast expiry; $sessions -> praeconium file watch + tmux 5 s + gate
runs; admonitiones system -> praeconium file watch; terminalia -> T2b;
sound -> 16 while playing; $question http -> reactor (T4).

- [x] 6a: framework + wiring + clock + scriba family. Header (approved):
  `VicusFacies.mora` / `.fossa` (ctx = pulsare_ctx; NIHIL -> 16 ms),
  `VicusTempus.secundum`, `vicus_mora_proxima`,
  `vicus_systema_cum_mora_addere`, `vicus_reactor_ponere`,
  `TestaForma.mora_proxima/mora_ctx/reactor`, `scriba_occupata`,
  `charta_occupata`, `kalendarium_occupatum`, `nomenclator_occupatus`.
  Done 2026-10-26 (as built): `vicus_reactorem_ponere` RENAMED
  `vicus_reactor_ponere` (lint: `reactorem` would be a new word;
  `reactor` is on the tolerated list, a glossary entry risks the English
  oracle). Also: fd panes > 64 -> 16 ms; `testa_cursus` calls
  `ludus_fenestra_quietem_ponere` on macOS only (iOS/Android read -1
  wrong). vicus already runs in quiet mode, but every undeclared part
  (terminal, volantes, $sessions, 3 systems) still says 16 - no gain
  yet, by design.
- [x] 6b: terminal pty fd + mora; T2b foreground check.
  Done 2026-10-26 (as built): `Pseudoterminale.grex_frontalis`
  (tcgetpgrp; examen's POSIX surface + an ignotum-permissum glossary
  line, oratio 19/19), `aemulator_hospes_mora/_grex_frontalis`,
  `terminale_fossa/_mora/_grex_frontalis`. The tmux check keys on a
  SIGNATURE of all terminal panes' foreground groups. Found by
  probatio_vicus_saltus: `tmux attach` is in front slightly BEFORE its
  client registers with the server, so the query on that change misses
  it and nothing changes again -> every foreground-triggered query gets
  ONE confirmation query 1 s later (plant: vicus_saltus red). Also
  fixed a pre-existing flake in `sine_exspectatione` (fake tmux slept
  0.8 s under the multiplexor's hard 1 s limit; now 0.4 s).
  `terminale_adest` removed (the signature counts panes).
- [x] 6c: praeconium file watch (kqueue) for admonitiones + $sessions.
  Done 2026-10-26 (as built): new lib `specula` (aedilis variants
  specula_macos.c kqueue / specula_posix.c NIHIL), `vicus_systema_
  fossam_ponere`; the admonitiones system drains the watch every pulse
  and keeps its cheap incremental log read (cost was the 60/s rate, not
  the read). $sessions reads the same log in its pulse, which now runs
  on the wake - its own timers are 6d. Side fixes: auspex_posix was RED
  at HEAD under clang 21 (OFF() not a constant -> __builtin_offsetof);
  kqueue surface added to silva/fontes/systema_posix.h (kevent 32 B
  certified); 6 ignotum-permissum glossary lines (oratio 19/19);
  compile_tests_fontes_generata.sh regenerated for the new lib. Moved
  to variant files because examen does NOT judge `#ifdef __APPLE__`
  branches (probe: an undefined call there passed).
- [x] 6d: volantes expiry, $sessions timers, sound.
  Done 2026-10-26 (as built): `volantes_mora`, `colloquia_visus_mora`,
  `VicusPortaSoni.mora` (real port keeps concentus/tintinnabulum agere
  results; open-and-quiet -> until CONCENTUS_QUIES 30 s after the last
  busy agere, then 16 until it closes). End-to-end test: the test app
  world (scriba layout, no clock) reports `vicus_mora_proxima` = -1
  idle - every pulsed part declared. The real port's deadline has no
  headless test (CoreAudio) - 6e measures it.
- [x] 6e: measure (excitationes/s), the 16 ms-cap plant moved from T5.
  Done 2026-10-26: idle 1 pulse per 14-19 s, CPU 0.0-0.1% (was 60/s,
  ~5%). Planted old cap -> 54/s, ~6% (the measurement catches it).
  Activity is event-driven: each hook event in the log = a brief wake;
  a chime = ~2 s at 16 ms. Table in lib/vicus.worklog.md.

### Task 7: measure after, look, docs, commit

- [ ] Same idle measurement as Task 1; table before/after in the
  worklog. Fran: use vicus normally for a while (typing, terminal,
  lector, `$question`), report anything that feels slower; Activity
  Monitor's Energy tab.
- [ ] Worklogs (ludus_fenestra, reactor, vicus), memory, ledger; lint;
  commit on Fran's word.

## Open Questions (answered 2026-10-26)

Fran: fd list now (Q1, as recommended); no blinking cursors exist (Q2 -
checked: `STILUS_NICTANS` is the SGR 5 text attribute, stored and
re-emitted, never animated - no blink timer anywhere); cheap wins first
(Q3: order Tasks 1, 2, 3, then 4-6, 7).

1. **How the reactor's fds reach Cocoa:** this plan exposes the fd list
   (keeps `poll()`, no behaviour change for servers: hospitium,
   http_vocatio, mcp, nomina). Alternative: a kqueue backend on macOS
   whose single fd is handed over - cleaner hand-off, but every reactor
   user then runs on the new backend. Recommendation: fd list now,
   kqueue later only if a server needs it.
2. **Cursor blink:** if the scriba or terminal cursor blinks while idle,
   that's a ~2/s wake-up floor. Many terminals stop blinking after a few
   seconds without input; do we want that, or no blink at all, or keep
   blinking?
3. **Order:** Tasks 2-3 (cheap, local wins) before the loop change
   (4-6), so each step is measurable on its own - or straight to the
   deadline loop?
