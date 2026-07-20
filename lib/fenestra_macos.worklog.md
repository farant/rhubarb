
## 2026-07-20 — B' (vitrea riders): expectare + menu + type-15

Blocking pump shipped as DELEGATE-BY-REPOST, simpler than the spec'd
refactor: expectare blocks once on nextEventMatchingMask with a
real deadline, and if an event arrives, reposts it atStart and
calls perscrutari — so there is exactly ONE translation path and
the resize-detection function-static never needed hoisting (the
[EXP-F] concern dissolved structurally instead of being solved).
Type-15 (ApplicationDefined) gets a `perge` case in the
translator: no Eventus, no sendEvent — the vitrea wake contract.
Default menu (App/Exire + Emendare with standard selectors)
installs once in the si(!NSApp) block; Latin item titles are free
because selectors do the routing, not names.

DEBUG FIND worth keeping: the expectare smoke first measured
0.078s for five 200ms waits and looked like a broken block. A
bare-AppKit twin with event-type printing showed clean full
timeouts — the difference was ENVIRONMENTAL (cursor movement over
the window streams mouse-moved events; real events waking the
pump is correct behavior, the loop just re-enters). With a still
mouse: 1.004s/1.0. Lesson: a blocking-pump timing smoke is
measuring the desktop too — diagnose with an event-type-printing
twin before suspecting the pump.
