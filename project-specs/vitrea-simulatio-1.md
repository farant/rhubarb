# vitrea — simulatio 1

*2026-07-16. Simulated end-to-end implementation of vitrea-spec.md Phases
A+B through the salve_vitrea demo (simulate-work technique, ~2.5 simulated
hours). Grounded in real reads: json.h, compile_tests.sh, fenestra_macos.m,
capsula.h. Runtime WebKit behaviors not verifiable from the chair are
marked [VERIFY]. Interview Q&A to follow in vitrea-simulatio-1-interview.md;
deltas folded into vitrea-spec.md as [SIM-1].*

## Pre-flight facts (real, from the codebase)

- The `.m` files compile under the FULL strict flag set — `-std=c89
  -pedantic -Werror -Wconversion -Wsign-conversion -Wcast-qual ...` — so:
  no `//` comments, declarations at block tops, every NSUInteger↔i32
  crossing explicitly cast.
- fenestra_macos.m is MRC (manual retain/release, no ARC) and uses ZERO
  blocks. System headers imported first "ut vitare conflictos macros"
  (latina.h).
- Every compile script hardcodes `-framework Cocoa -framework Security`;
  compile_tests.sh keeps a hardcoded GUI-app list and OBJC_SOURCES array.
- fenestra's Eventus cauda: fixed ring of 256 in-struct, caput/cauda/
  numerus. The vitrea bridge cauda copies this pattern verbatim.

## Log

**T+0:05** — Phase A stubs. Spec correction immediately: sketch invented
`JsonNodus*`; real type is `JsonValor*`. json.h has everything (legere,
objectum_capere, builders, scribere_ad_aedificator). Handler settles:
`JsonValor* argumenta`, per-call `Piscina*`, `vacuum* datum`, returns
`JsonValor*` or writes `chorda culpa`.

**T+0:10** — Registry on tabula_dispersa. ⚠ Malformed JSON ⇒ no
recoverable `id` ⇒ culpa reply can't route to a promise. Emit id-less
culpa; JS policy for orphans = console.error.

**T+0:15** — Reply construction. Clean arena story: tractare receives the
message chorda, does its own json_legere; parse tree + reply tree +
serialized reply all live in the caller's piscina; caller resets. Lifetime
ownership pushed up into vitrea (see T+1:20).

**T+0:20** — Escaper. ⚠ chorda datum is SIGNED i8 — every byte comparison
against 0x80+ needs casts under -Wconversion. U+2028/U+2029 detected as
byte triples E2 80 A8 / E2 80 A9, emitted as \u2028 literals.

**T+0:25** — `<` always emitted as \u003C (simpler and safer than
`</script` lookahead). Escaper is a pure chorda→chorda function.

**T+0:30–0:40** — credo probationes: envelope round-trips, unknown
methodus culpa, orphan culpa, push events, escaper goldens (quotes,
backslash, control bytes, 2028/2029, multibyte passthrough, `<`). Memoria
missor = Xar of captured chordae. C89 gotcha: `nomen` is a macro — no
parameter named nomen anywhere.

**T+0:45** — Phase A exit. ~600 lines with tests. No WebKit. Predictable
half; complications so far are spec corrections, not surprises.

**T+0:50** — Phase B. Header order: Cocoa, WebKit, THEN rhubarb headers.
⚠ Build reality: `-framework WebKit` must be added to compile_tests.sh,
compile_tools.sh, compile_library.sh, compile_sputnik.sh; new .m joins
each OBJC_SOURCES array. Five files touched before line one of vitrea.
Quiet evidence for the tabularium build fold.

**T+0:55** — vitrea_creare: NSWindow via
fenestra_obtinere_tractationem_nativam, WKWebViewConfiguration +
WKUserContentController, WKWebView as contentView. MRC ceremony mirrors
fenestra_macos.m. **Decision: zero blocks** — evaluateJavaScript gets nil
completionHandler (fire-and-forget evals; envelope already assumed it).
[VERIFY] whether -std=c89 -pedantic tolerates block syntax in ObjC mode —
moot while block-free holds.

**T+1:00** — ⚠ **Spec bug found (§4).** `capsula://index.html` parses
`index.html` as the URL AUTHORITY (host), leaving an empty path —
relative URLs (./style.css) resolve against nonsense. Same reason Tauri
uses tauri://localhost and Wails uses wails://wails.localhost. Fix: fixed
dummy authority — `capsula://radix/index.html`; handler resolves the PATH
against the Capsula. → Spec delta 1.

**T+1:05** — Scheme handler `VitreaPortitor <WKURLSchemeHandler>`: NSURL
path → C string → capsula_legere → NSURLResponse (MIME table +
textEncodingName utf-8 for text types) → didReceiveData (NSData over the
static capsula blob, no copy) → didFinish; 404 → didFailWithError.
⚠ -Wcast-qual friction on i8* → const void* chains.

**T+1:10** — [VERIFY] Custom-scheme pages and SECURE CONTEXT status
(crypto.subtle etc. gated; Tauri has a pain history here). [VERIFY]
whether fetch("capsula://radix/data.json") from a capsula-origin page
routes through the handler with same-origin treatment or trips CORS.

**T+1:15** — Message handler `VitreaNuntius <WKScriptMessageHandler>`:
NSString body → UTF8String → piscina chorda → ring cauda → nuntii_recepti.
Copy + enqueue ONLY.

**T+1:20** — ⚠ **Piscina lifetime is load-bearing** (spec had punted it).
Arenas never free; hours of chatty traffic = unbounded growth. Design:
Vitrea owns piscina_nuntiorum; when the drain empties the cauda, vitrea
mark/resets it. Contract: **drained chordae are valid only until the next
drain call** — tractare immediately, never stash. Same per-call reset
inside dispatch. → Spec delta 2.

**T+1:25** — WKUserScript injection AtDocumentStart, mainFrameOnly. The
JS runtime as a big string constant — -Wno-overlength-strings already
applied in the relevant compiles. Small mercy.

**T+1:30** — vitrea_aestimare: escape → wrap in internuntius._accipere
→ evaluateJavaScript nil-handler. internuntius missor adapter = 5 lines.

**T+1:35** — ⚠ **Biggest find: the blocking pump has a wake-up hole.**
Bridge messages arrive as runloop-source work, not NSEvents. The runloop
services the script-message handler (message lands in cauda) but
nextEventMatchingMask KEEPS BLOCKING — no NSEvent occurred. App doesn't
drain until the mouse moves. Fix (the classic): handler posts a synthetic
NSEventTypeApplicationDefined via [NSApp postEvent:atStart:NO] to wake
the pump; pump swallows it. fenestra_expectare_eventus must specify this
synthetic-event contract; the translator ignores ApplicationDefined.
→ Spec delta 3.

**T+1:40** — Keyboard focus: [fenestra_ns makeFirstResponder:webview]
after attach or typing goes nowhere. One line; a lost day if missed.

**T+1:45** — ⚠ **Cmd+C/V/X/A are dead.** Bare executable ⇒ no menu bar ⇒
no key equivalents ⇒ WKWebView text fields can't copy/paste. Collides
with the spec's own boundary ("menus are never vitrea"). Options:
(a) fenestra grows a minimal default NSMenu (App + Edit, standard
selectors) — app-level concern, pixel apps benefit too;
(b) vitrea intercepts key equivalents and routes copy:/paste: selectors —
no menu, more code, fights AppKit;
(c) accept broken clipboard until the .app-bundle era.
→ Interview question 1.

**T+1:50** — destruere: removeScriptMessageHandlerForName: BEFORE release
(WKUserContentController retains the handler — known quirk), stop/nil
webview, release config. ⚠ vitrea displaced fenestra's FenestraVisus as
contentView; fenestra_destruere still releases its own retain safely, but
pixel calls after attach would draw to a detached view silently. Contract:
a Fenestra hosting a Vitrea is webview-only. Documented, not enforced.
→ Spec delta 4a.

**T+1:55** — VitreaLegatus <WKNavigationDelegate>:
webContentProcessDidTerminate + navigation policy (same-origin allow,
else cancel). ⚠ Interitus delivery: the cauda carries chordae, not typed
events — resolution: vitrea_obtinere_nuntium returns a genus alongside
the chorda (PONS | INTERITUS). One channel, ordered. → Spec delta 4b.

**T+2:00** — salve_vitrea demo. index.html + capsula_generare config,
three buttons (echo, addere, fructus read), C-side 1 s clock push.
⚠ Build-order friction: capsula header generated before demo compiles
(the book_assets dance); demo must join compile_tests.sh's hardcoded
GUI-app list. More build-fold evidence.

**T+2:05** — ⚠ Where does the 1 s clock fire from under a BLOCKING pump?
No frame loop to piggyback. NSTimer fires via runloop = user C code runs
inside expectare (new reentrancy the poll world never had). Demo answer:
use the timeout arm (expectare(ms)) and push from the loop body. Real
apps will want timers. → Interview question 2.

**T+2:10–2:20** — Strict-flag sweep of vitrea_macos.m: comments clean,
declarations at block tops (awkward in startURLSchemeTask: with ~8
locals), all NSUInteger↔i32 casts explicit, zero blocks. ~700 lines.
internuntius ~350 + ~250 probationes. In the estimate band.

**T+2:25** — End-to-end walkthrough: launch → creare → attach/inject/
scheme/firstResponder → load capsula://radix/index.html → relative fetch
resolves → vocare → postMessage → enqueue + wake event → pump returns →
drain → dispatch → reply eval → promise resolves → DOM updates. Clock
rides the timeout arm. Idle ~0%. Cmd+C still dead (question 1).

**T+2:30** — Stop. Phase A green; Phase B works modulo the [VERIFY]s.

## What fell out

Spec deltas (folded into vitrea-spec.md as [SIM-1]):
1. Dummy authority: capsula://radix/... — bare capsula://file parses the
   file as host and breaks relative URLs.
2. piscina_nuntiorum mark/reset contract; drained chordae valid only
   until the next drain call.
3. Blocking-pump wake contract: synthetic NSEventTypeApplicationDefined
   posted on enqueue; expectare swallows it.
4. (a) Fenestra hosting a Vitrea is webview-only by contract;
   (b) vitrea_obtinere_nuntium gains a genus (PONS | INTERITUS).
Also: handler signature corrected to JsonValor*; block-free MRC recorded
as a decision; build-system touchpoints listed under Phase B.

[VERIFY] on real hardware:
- secure-context status of custom-scheme pages (crypto.subtle etc.)
- fetch/CORS behavior for capsula:// same-scheme requests
- block syntax under -std=c89 -pedantic in ObjC mode (moot if block-free)
- WKWebsiteDataStore/localStorage persistence for bare executables

Interview questions:
1. The clipboard problem — where does the Cmd+C/V fix live? (fenestra
   default menu / vitrea key-equivalent routing / wait for bundles)
2. The timer story for blocking-pump apps (timeout arm forever / fenestra
   timer API / defer)
3. Cauda overflow leaves JS promises unsettled forever — acceptable?
