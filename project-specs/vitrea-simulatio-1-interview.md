# vitrea — simulatio 1 interview

*2026-07-16. Q&A following vitrea-simulatio-1.md. Verdicts folded into
vitrea-spec.md as [INT-1].*

## Q1 — The clipboard problem

**Q:** Cmd+C/V/X/A are dead in a bare executable (no Edit menu → no key
equivalents → WKWebView fields can't copy/paste). Where does the fix live?
(a) fenestra grows a minimal default menu (App + Edit); (b) vitrea routes
key equivalents manually, no menu; (c) live with it until the .app bundle
era.

**A (Fran):** (a) — fenestra grows a minimal default menu (App + Edit).

Notes: keeps the "menus are never vitrea" boundary intact — the menu is
fenestra's, an app-level concern; pixel-buffer apps benefit too. Standard
selectors (cut:/copy:/paste:/selectAll:/undo:/redo:) + App menu with Quit.

## Q2 — The timer story

**Q:** The blocking pump kills the frame loop apps piggybacked timers on.
Timeout arm vs a fenestra timer API?

**Fran asked for pros and cons first.** Summary of the analysis given:

- Timeout arm: loop computes ms-until-next-deadline, passes exactly that
  to expectare — precision ~ms, not "polling granularity." Control flow
  fully visible (house posture), pure C, portable, no reentrancy, fake-
  clock testable. Cons: a three-line idiom per app; discovered mid-answer
  that **tempestivum already exists** (timers as entities, processed from
  the loop, delivering via cursus) — needs only an aggregate next-deadline
  query; drags entitas_repositorium + cursus as dependencies (heavy for a
  one-clock app).
- fenestra NSTimer API: uniform Eventus delivery, zero app bookkeeping.
  Cons: duplicates tempestivum in platform ObjC; fires inside expectare so
  the callback needs the enqueue-only + synthetic-wake dance; forces
  id/cancel API design including the ghost-tick problem (cancelled timer's
  already-enqueued event still arrives); MRC ceremony; unportable. Delay
  under load identical in both (single thread).
- Framing: the codebase already made this decision once when tempestivum
  was built loop-processed instead of wrapping NSTimer.

**A (Fran):** Timeout arm + tempestivum (add a next-deadline query).

## Q3 — Cauda overflow policy

**Q:** Overflow drops a request and its JS promise never settles.
Acceptable?

**Fran asked for the overflow scenarios first.** Summary given:

- Mechanics: single-threaded ⇒ enqueues happen only while expectare
  services the runloop; nothing arrives mid-drain. Overflow = >256
  messages delivered in ONE runloop-servicing window.
- Scenarios: (1) runaway JS (vocare loop, WebKit IPC floods in);
  (2) legitimate-but-unbatched burst (500 rows, one call each);
  (3) slow C handler + chatty JS (everything queued during the crunch
  floods in afterward).
- Only REQUESTS drop (replies/pushes are evals) ⇒ symptom is always a
  silently hanging vocare promise. At drop time vitrea can't read the id
  (copy-only discipline) but CAN eval internuntius._superfluum(n) so the
  JS runtime console.errors loudly. Depth is nearly free (~16 B/slot) but
  unbounded growth under a runaway bug = unbounded memory; bounded + drop
  stays the honest answer at any depth.

**A (Fran):** Drop + loud — eval _superfluum(n), console.error in JS.

## Q4 — Sealing the names

**Q:** Library names — vitrea + internuntius as drafted, or specularia
instead of vitrea?

**A (Fran):** vitrea + internuntius, as drafted.

**Q:** URL scheme — capsula:// or app://?

**A (Fran):** capsula:// — the honest name.

## Verdicts

1. Minimal default menu (App + Edit) lands in **fenestra**, v1.
2. Timers: **timeout arm + tempestivum**; tempestivum gains an aggregate
   next-deadline query to feed expectare's ms argument.
3. Overflow: **drop + loud** — _superfluum(n) added to the JS runtime;
   nuntii_amissi counter stays.
4. Names SEALED: **vitrea + internuntius**; scheme **capsula://**.
