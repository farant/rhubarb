# reactor.c worklog

## 2026-07-20 — hospitium Phase 0: fd-slot reuse

reactor_adicere always appended at fd_numerus and removere only
flips activus, so add/remove churn permanently exhausted the fd
table — a long-running server would hit the wall after
REACTOR_FD_MAXIMA distinct connections EVER, not concurrent.
Fix mirrors the timer-slot pattern: scan [0, fd_numerus) for the
first !activus slot, extend (with the capacity check) only when
none is free.

Audit verdict that held: _rebuilere_poll_fds and the dispatch
loop needed ZERO changes — both compact by activus in index
order, and alignment is recomputed every poll. The reentrancy
one-turn-deferral guarantee also survives slot reuse: a callback
that removes itself and adds a spare refills its OWN slot, but
dispatch already consumed that slot's activus check, so the swap
takes effect next poll (probatio_callback_reentrans pins this).

Differential proof the new churn test bites: reverted reactor.c
to HEAD → probatio_fd_churn_reuse FRACTA at cyclus == M + CC
(exhaustion mid-run); restored fix → 102/102. The bar is not a
blind spot.

## 2026-10-14 - dispatch safe under mutation (http-vocatio T1, spec II.0)

Route B puts several independent clients (DNS, TCP, TLS, timers) on one
reactor, and their callbacks add and remove fds and timers while the
reactor is dispatching. Reading the code for the plan found three
hazards; `probationes/probatio_reactor_mutatio.c` pins them (red before
the fix: 5 of 6 sections).

- **Rebuild during dispatch dropped events.** `reactor_adicere`,
  `_modificare` and `_removere` called `_rebuilere_poll_fds`, which
  reset every `revents` to 0. One `reactor_removere` inside a callback
  silently lost every later fd's event in that pass (level-triggered
  poll re-reports next pass, so it was a delay, plus a lost event for
  anything that changed in between). The rebuild calls are gone:
  `reactor_poll` rebuilds at the start of every pass anyway.
- **Dispatch now walks a snapshot.** `_rebuilere_poll_fds` records for
  each `poll_fds[k]` the entry index and that entry's `generatio`; the
  generation bumps on every new registration and every removal. A
  callback may add/remove anything: removed entries and REUSED slots
  (same index, new registration) are skipped for the rest of the pass;
  fds added mid-pass are first dispatched next pass. (Section `locus
  reusus` passed on old code by accident - the rebuild zeroed the
  revents - so it is guarded by a plant, not born red.)
- **One-shot timers were switched off after their callback.** A
  callback that cancelled itself and added a new timer got the same
  slot back, which was then deactivated: the new timer never fired.
  Now the state (inactive / next deadline) is set before the callback.
- **Timers born in a pass wait for the next pass** (`id >= limes`,
  limes = next id at pass start). Otherwise a callback that always adds
  a 0 ms timer never lets `reactor_poll` return. Two other mechanisms
  already cover most cases - a one-shot parent frees its slot before
  the callback (the child reuses it, behind the loop) and the loop
  bound is the pass-start `timer_numerus` (a child above it is never
  visited) - so limes is load-bearing only when the child lands in a
  HOLE: a freed slot above the parent and below the high-water mark.
  The `limes` plant stayed green twice until section `timer natus` was
  rebuilt as exactly that layout (repeating parent, cancelled slot, a
  live timer above).
- **The early return counted the high-water mark.** `timer_numerus`
  never shrinks, so after the first timer ever, `reactor_poll(r, ms)`
  with nothing active really slept `ms` inside a `poll()` with no fds.
  It now counts active timers (`reactor_numerus_timer`).

Test gotcha: the generation plant made the suite HANG for 10 minutes,
not fail - a misdispatched "readable" event made `_notare` do a
blocking `read()` on an empty pipe. The test's pipes are now
O_NONBLOCK (`_pipam`), so a misdispatch counts instead of blocking.
A plant must make the test fail, not wait; non-blocking fixtures are
part of that. Second gotcha: deleting the `limes` check leaves the
variable unused, which -Werror refuses - the plant is `(... && FALSUM)`.

## 2026-10-16 - cooperative jobs and signals (hospitium v3 T1)

- **Jobs** (`reactor_laborem_adicere`): one step per `reactor_poll` cycle,
  after fds and timers; any pending job forces the poll wait to 0, and
  the "nothing registered -> return 0" check counts jobs too. A job added
  during a pass runs from the NEXT cycle - decided by the cycle it was
  born in (`cyclus_natalis`), which covers both an appended job and one
  that reuses a slot freed earlier in the same pass (section
  `locus_reusus`). The loop's snapshot bound is therefore redundant
  (removing it is an equivalent mutation - plan R1).
- **Signals** (`reactor_signum_adicere`): self-pipe. The handler only
  writes the signal number byte (errno saved); the read end is an
  INTERNAL fd entry - not counted by `reactor_numerus_fd` (tests assert
  0 fds after requests) but it keeps `poll()` blocking for a reactor
  whose only registration is a signal. One process-wide pipe and owner
  (signals are process-wide); `reactor_destruere` restores SIG_DFL and
  closes the pipe so another reactor can take over.
- **EINTR finding:** a signal interrupts `poll()` with EINTR, and
  `SA_RESTART` does NOT restart `poll()` (Darwin and Linux). Without
  care the signal waits a whole cycle; now the pipe is drained right
  there (section `signum_solum`, plant red).
- examen's POSIX surface (`silva/fontes/systema_posix.h`) lacked
  SIGUSR1/SIGUSR2/SA_RESTART - added (Darwin values, like its neighbours)
  rather than tolerated.
