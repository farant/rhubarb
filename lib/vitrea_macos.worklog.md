# vitrea_macos worklog

## 2026-07-20 — Phase B: first-fix build, first-run round trip

The whole platform file (~650 lines) compiled clean under the full
strict flag set on the FIRST attempt, and the demo's automatic
bridge smoke passed on the FIRST live run:

    [vitrea] salve vocatum
    [vitrea] confirmatum: salve, munde!

The confirmatum line is load-bearing: the page chains a second
vocare off the first reply's .then(), so its appearance proves the
COMPLETE cycle headless — capsula → scheme handler → injected
runtime → postMessage → cauda+wake → blocking-pump drain →
dispatch → reply → escaper → _accipere → promise resolution →
second round trip. Idle CPU measured 0.0% (exit bar met exactly).

Why first-fix worked: every WebKit behavior this file depends on
was pinned by the calibration spike BEFORE the file existed
(NSHTTPURLResponse-200 rule, dummy authority, wake contract), and
every codebase contact was pinned by the explorers (NSWindow from
tractationem_nativam, MRC dialect, capsula copy semantics,
PiscinaNotatio names). The file was transcription, not discovery.
Calibration-first pays off exactly here.

Decisions recorded:
- Piscina reset ON ARRIVAL TO EMPTY CAUDA (not on drain-empty):
  early-exit from the drain loop can never leak growth, and a
  non-empty cauda is never reset under a held chorda.
- Navigation policy DEFERRED to Phase C: the decidePolicy delegate
  method takes a block parameter; skipping it keeps the file
  literally block-free rather than block-literal-free.
- vitrea → internuntius include is for the escaper only; the
  dependency direction is fine (internuntius stays pure).
- Copy-and-reset serving per spec v2 §4: dataWithBytes (WebKit
  owns its copy) then immediate reficere — zero lifetime coupling
  with the async scheme task.
