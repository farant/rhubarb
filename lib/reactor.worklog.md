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
