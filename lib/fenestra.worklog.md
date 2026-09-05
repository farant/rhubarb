
## 2026-09-04 — Eventus.tempus (ludus T1)

Added `s64 tempus` (milliseconds) to Eventus and `fenestra_tempus_ms()`
(pulsus * 1000 / frequentia, f64 math — frequentiam returns f64).
Stamped ONCE, in `impellere_eventum`: it is the single enqueue path for
the NSEvent construction sites and both immittere functions alike, and
it stamps only when the caller left tempus at ZEPHYRUM, so an injected
or replayed event keeps its own time. Reason: ludus dispatch must be
replayable — time is data in the event, never a clock call below
fenestra (project-specs/ludus-brainstorm.md §XIV). The old widget
manager's tempus_nunc() in dispatch is what this retires (T4 derivare).
Probatio: probatio_fenestra_tempus (window-free; the enqueue stamp is
exercised by T12's live run). Refinement parked: the :484 site could
carry NSEvent.timestamp for sub-ms fidelity — same clock family.
