# vitrea + internuntius — spec v1

*2026-07-16. From chat discussion of a Wails/Tauri-shaped webview layer for
rhubarb. Status: v1.1 — drafted from conversation + a first codebase read
(MAP.txt, fenestra.h, capsula.h, fenestra_macos.m pump, tessera-spec.md for
house style); deltas from vitrea-simulatio-1.md folded in, marked [SIM-1];
interview verdicts marked [INT-1]. Names SEALED 2026-07-16: **vitrea +
internuntius**; scheme **capsula://**. v2 after agent exploration of
fenestra's Eventus cauda internals, capsula runtime, and servitium (for
dev mode).*

## 1. Thesis

A webview application layer for rhubarb: native window + system WKWebView +
a JS↔C bridge, in the shape of Wails/Tauri but owned. No bundled browser
engine, no localhost requirement, no Node — the frontend ships inside the
binary via capsula and is served over a custom URL scheme; the backend is
plain rhubarb C89.

macOS/WKWebView **only**, matching the monorepo's current platform reality.
The platform file is Objective-C (`.m`), following the established
`fenestra_macos.m` / `clipboard_platform_macos.m` / `tls_macos.m` pattern —
no objc_msgSend ceremony. The public headers are strict C89.

Two libraries, split so the reusable part stays pure:

- **vitrea** (glass panes; sits next to fenestra the way stained glass sits
  in a window frame) — WebKit embedding: attach a WKWebView to a Fenestra,
  serve assets from a Capsula, move strings JS↔C. All the WebKit contact
  lives here and only here.
- **internuntius** (the go-between; deliberately NOT "pons," which is
  already tessera's platform-seam term) — a transport-agnostic RPC layer:
  method registry, JSON envelope, dispatch, replies, C→JS event push. Knows
  nothing about WebKit. Could later run the same envelope over tcp or stdio.
  Name is nuntium-adjacent on purpose: it carries messages; but the wire
  format is json, not nuntium (see §6).

Permanent boundary, named now: **vitrea is webview attachment + asset
origin + string transport, forever.** Native menus, dialogs, tray icons,
notifications, downloads, printing — none of that is vitrea; if wanted,
each is its own future library against the same Fenestra handle.

## 2. Shape

- **lib/ additions, not a subproject.** vitrea leans on fenestra, capsula,
  json, chorda, piscina — vendoring all that into an amalgam buys nothing
  while the monorepo is the only consumer. Files:
  - `include/vitrea.h` + `lib/vitrea_macos.m`
  - `include/internuntius.h` + `lib/internuntius.c` (pure C89)
  - `tools/` demo (see §11) + worklogs per house rule.
- **Amalgamation: named deferral.** If vitrea ever wants to ship as a
  standalone deliverable, that's the moment to revisit — second consumer as
  forcing function, same rule as tessera §11.
- **Memory**: caller passes Piscina at creation, house style. WebKit
  objects are the only non-piscina allocations; vitrea_destruere releases
  them (follow fenestra_macos.m's retain/release conventions exactly).
- **Context struct, no globals.** `Vitrea*` opaque; every call takes it.

## 3. Window model — vitrea rides fenestra

vitrea does NOT create windows. `vitrea_creare(piscina, fenestra, config)`
obtains the NSWindow via `fenestra_obtinere_tractationem_nativam()` and
installs a WKWebView as its contentView (v1: the webview IS the content —
full-window). Consequences, all good:

- NSApplication setup, activation, window delegate, close/resize/focus
  events, and the manual pump (`fenestra_perscrutari_eventus` →
  `nextEventMatchingMask` + `sendEvent`) are reused untouched. The pump
  already forwards unhandled NSEvents, so keyboard/mouse reach the webview
  natively; the app's Eventus cauda keeps carrying window-level events
  (EVENTUS_CLAUDERE etc.), which is exactly the split we want.
- WKWebView is out-of-process; its IPC is serviced whenever the runloop
  runs, which `nextEventMatchingMask` does. **No new loop machinery.**
- The subview-hybrid door (web panel over pixel-buffer UI, or beside it)
  stays open structurally — it's all NSViews — but is a named deferral.

A webview window needs no frame pacing: nothing to redraw from C. Named v1
items on adjacent libraries (small, additive; verdicts from
vitrea-simulatio-1-interview.md marked [INT-1]):

- `fenestra_expectare_eventus(fenestra, ms)` — a blocking pump variant
  (`untilDate:` with timeout) so a pure vitrea app idles at ~0% CPU
  instead of polling. Pixel-buffer apps ignore it.
- [INT-1] **fenestra grows a minimal default menu** (App menu with Quit +
  Edit menu with standard selectors cut:/copy:/paste:/selectAll:/undo:/
  redo:). Without it, a bare executable has no key equivalents and
  Cmd+C/V/X/A are dead inside WKWebView fields. The §1 boundary holds:
  the menu is fenestra's, an app-level concern — never vitrea's — and
  pixel-buffer apps benefit too.
- [INT-1] **Timers: timeout arm + tempestivum.** The loop computes
  ms-until-next-deadline and passes exactly that to expectare (~ms
  precision, no polling granularity). tempestivum gains one aggregate
  next-deadline query to feed that argument. No NSTimer machinery in
  fenestra, ever — the codebase already made this call when tempestivum
  was built loop-processed.

[SIM-1] **Wake contract.** Bridge messages arrive as runloop-source work,
not NSEvents — a blocked `nextEventMatchingMask` services the script
message (it lands in the cauda) and then KEEPS BLOCKING until a real
NSEvent arrives. The enqueue path therefore posts a synthetic
`NSEventTypeApplicationDefined` via `[NSApp postEvent:atStart:NO]` to wake
the pump; fenestra's translator swallows ApplicationDefined events. Any
future producer that feeds the main loop from a runloop source or thread
must use the same wake.

[SIM-1] **Webview-only contract.** vitrea_creare displaces fenestra's
pixel view as contentView. fenestra_destruere remains safe (it releases
its own retain), but pixel-buffer calls on a Vitrea-hosting Fenestra draw
to a detached view, silently. Rule: a Fenestra hosting a Vitrea is
webview-only. Documented, not enforced.

## 4. Asset origin — capsula:// (WKURLSchemeHandler)

- Primary mode: register the custom scheme **`capsula://`** [INT-1 sealed]
  via `setURLSchemeHandler:forURLScheme:` on the WKWebViewConfiguration.
  [SIM-1] URLs carry a **fixed dummy authority** — `capsula://radix/...` —
  because in `capsula://index.html` the file parses as the URL *host*,
  leaving an empty path and breaking every relative URL in the page (the
  same reason Tauri uses tauri://localhost and Wails wails://wails.localhost).
  The handler resolves the URL *path* against a caller-provided `Capsula*`:
  `capsula://radix/index.html` → `capsula_legere(cap, "index.html",
  piscina)` → didReceiveResponse (with MIME + textEncodingName utf-8 for
  text types) + didReceiveData + didFinish. 404 → didFailWithError.
- **MIME table**: small static extension→MIME map in vitrea (html, css, js,
  mjs, json, svg, png, jpg, gif, webp, woff2, wasm, txt; fallback
  application/octet-stream). Not a library; a table.
- Custom scheme sidesteps file:// sandbox restrictions AND App Transport
  Security entirely — this is why it's the primary mode.
- **Dev mode**: `configuratio.origo = VITREA_ORIGO_URL` loads an http URL
  (a local `servitium` instance or vite-style dev server) instead, for
  edit-refresh loops against the same C backend. ⚠ VERIFY at v2: ATS
  treatment of plain-http loopback for a non-bundled executable (no
  Info.plist to grant exceptions). If blocked, dev mode may itself be the
  first reason to run bundled (§12).
- Per-request piscina marks (PiscinaNotatio) or a per-request scratch
  piscina — decide at v2 after reading capsula/flatura allocation behavior;
  scheme requests arrive on the main runloop, so no locking either way.
- [SIM-1] ⚠ VERIFY on hardware: (a) whether custom-scheme pages count as
  **secure contexts** (crypto.subtle and friends are gated on it; Tauri
  has a pain history here); (b) whether `fetch()` against capsula:// from
  a capsula-origin page routes through the handler with same-origin
  treatment or trips CORS.

## 5. Transport — three WebKit primitives, one cauda

All JS↔C traffic is strings (JSON text). vitrea's whole transport surface:

1. **Inject** — one WKUserScript at document start, main frame only: the
   internuntius JS runtime (§8), a string constant compiled into vitrea.
2. **Receive** — one WKScriptMessageHandler (name: `internuntius`). JS
   calls `window.webkit.messageHandlers.internuntius.postMessage(textus)`.
   The handler does the minimum: copy the NSString into a piscina chorda
   and push it onto a **fixed ring cauda** (house pattern — "eventus in
   cauda"). It does NOT dispatch. This kills reentrancy: C handlers never
   run inside the pump; the app drains explicitly (§7) on its own turn of
   the loop, always on the main thread.
   Cauda overflow [INT-1]: **drop + loud.** Bump `nuntii_amissi` (fructus
   philosophy, §10) AND eval `internuntius._superfluum(n)` at drop time —
   the dropped request's id is unreadable (copy-only discipline) so its
   promise never settles, but the JS runtime console.errors loudly and
   the mystery hang becomes a labeled one. Proposed depth: 256 messages;
   bounded + drop stays the rule at any depth (unbounded growth under
   runaway JS = unbounded memory).
   [SIM-1] **Message memory contract:** Vitrea owns a dedicated
   `piscina_nuntiorum`; enqueued chordae are copied into it; when a drain
   empties the cauda, vitrea mark/resets it. Therefore **drained chordae
   are valid only until the next drain call** — tractare immediately,
   never stash pointers. Dispatch likewise gets a per-call mark/reset for
   parse tree + reply. Without this, arena allocation grows unboundedly
   under chatty traffic.
3. **Send** — `evaluateJavaScript:` wrapper. Main-thread-only per WebKit;
   trivially satisfied since the app loop IS the main thread. Every send is
   of the form `internuntius._accipere("<json-as-JS-string-literal>")` —
   the payload crosses as ONE escaped string literal and is JSON.parse'd on
   the JS side. **Escaping discipline (the classic bug):** escape `\` `"`,
   control chars, and — the ones people miss — `\u2028`/`\u2029` (legal in
   JSON, illegal raw in a JS string literal) and any `</script`-shaped
   sequence. This escaper is a pure function with golden credo tests (§11).

Navigation policy v1: main-frame navigation restricted to the configured
origin (scheme or dev URL); everything else refused (target=_blank etc.
dropped; `aperire externum` via system browser is a named deferral).
webContentProcessDidTerminate (WebKit's child process died) → delivered
through the same cauda as a distinct genus (§7 [SIM-1]); app decides
(typically: reload).

[SIM-1] **MRC / block-free discipline.** vitrea_macos.m uses zero blocks,
matching house ObjC: `evaluateJavaScript:completionHandler:` always gets
nil (evals are fire-and-forget — the envelope already assumes it), and
all WebKit contact is delegate-based. Manual retain/release per
fenestra_macos.m; `removeScriptMessageHandlerForName:` is called before
release in destruere (WKUserContentController retains the handler — known
quirk). `makeFirstResponder:` on the webview at attach, or keyboard input
goes nowhere.

## 6. internuntius — envelope + dispatch (pure C89)

Wire format is **json** (not nuntium): the other end is a browser, JSON is
native there, and traffic is human-debuggable in the Web Inspector. nuntium
stays the binary format for C↔C.

Envelope (field names Latin, like everything else):

```
JS → C   { "id": 7, "methodus": "salve", "argumenta": { ... } }
C → JS   { "id": 7, "fructus": { ... } }            (success)
         { "id": 7, "culpa": { "nuntius": "..." } }  (failure)
C → JS   { "eventus": "horologium", "datum": { ... } }   (push, no id)
```

- **Registry**: `internuntius_praebere(inx, "salve", tractator, datum)` —
  name → function pointer + userdata, in a tabula_dispersa. Handler
  signature sketch: takes the parsed json argumenta node + a per-call
  piscina, returns a json node (fructus) or sets culpa. Sync in v1; every
  call gets a reply (fire-and-forget is just an ignored reply).
- **Dispatch**: `internuntius_tractare(inx, chorda nuntium_textus)` —
  parse, look up, call, serialize reply, hand the reply string to a
  **mittere callback** provided at creation. That callback is the
  transport seam: vitrea's is "escape + evaluateJavaScript"; a probatio's
  is "append to a captured list." internuntius never includes WebKit
  headers — this seam is its memoria-pons (tessera §7, same move).
- **Push**: `internuntius_eventum_mittere(inx, "horologium", datum_json)`.
- Unknown methodus / malformed JSON → culpa reply (with id when
  recoverable), `culpae` counter bump, never a crash.

## 7. API sketch (C89 — API-first, review before building)

```c
/* ===== internuntius.h (pure) ===== */
nomen structura Internuntius Internuntius;

nomen JsonValor* (*InternuntiusTractator)(
    JsonValor* argumenta, Piscina* piscina,
    vacuum* datum, chorda* culpa_exitus);        /* [SIM-1] real json.h types */

nomen vacuum (*InternuntiusMissor)(vacuum* datum, chorda textus);

Internuntius* internuntius_creare  (Piscina* piscina,
                                    InternuntiusMissor mittere,
                                    vacuum* mittere_datum);
b32  internuntius_praebere         (Internuntius* inx,
                                    constans character* methodus,
                                    InternuntiusTractator tractator,
                                    vacuum* datum);
vacuum internuntius_tractare       (Internuntius* inx, chorda nuntium);
vacuum internuntius_eventum_mittere(Internuntius* inx,
                                    constans character* eventus,
                                    JsonValor* datum);

/* ===== vitrea.h ===== */
nomen structura Vitrea Vitrea;

nomen enumeratio {
    VITREA_ORIGO_CAPSULA,
    VITREA_ORIGO_URL            /* dev mode */
} vitrea_origo_t;

nomen structura {
    vitrea_origo_t      origo;
    Capsula*            capsula;      /* ORIGO_CAPSULA */
    constans character* via_initialis;/* "index.html" */
    constans character* url;          /* ORIGO_URL     */
    b32                 inspectabilis;/* Web Inspector; macOS 13.3+ */
} VitreaConfiguratio;

Vitrea* vitrea_creare    (Piscina* piscina, Fenestra* fenestra,
                          constans VitreaConfiguratio* configuratio);
vacuum  vitrea_destruere (Vitrea* vitrea);

/* [SIM-1] drain the bridge cauda; VERUM dum manent nuntii.
 * genus distinguishes bridge traffic from vitrea's own events.
 * Drained chordae live in piscina_nuntiorum: valid only until the
 * next drain call (§5) — tractare immediately, never stash. */
nomen enumeratio {
    VITREA_NUNTIUS_PONS,          /* JS → C bridge message  */
    VITREA_NUNTIUS_INTERITUS      /* web content process died */
} vitrea_nuntius_genus_t;

b32     vitrea_obtinere_nuntium (Vitrea* vitrea, chorda* nuntium,
                                 vitrea_nuntius_genus_t* genus);

/* raw eval (internuntius rides this via its missor) */
vacuum  vitrea_aestimare (Vitrea* vitrea, chorda javascript);

vacuum  vitrea_recargare (Vitrea* vitrea);   /* reload */
```

Canonical loop (a vitrea app):

```c
dum (!fenestra_debet_claudere(fenestra)) {
    fenestra_expectare_eventus(fenestra, X);          /* blocking pump */
    dum (fenestra_obtinere_eventus(fenestra, &ev)) { /* window events */ }
    dum (vitrea_obtinere_nuntium(vitrea, &nu, &genus)) {
        si (genus == VITREA_NUNTIUS_PONS) {
            internuntius_tractare(inx, nu);
        } aliter {
            vitrea_recargare(vitrea);     /* interitus */
        }
    }
}
```

Public API names ≤ 31 chars (C89 significance rule, per silva/tessera).

## 8. JS runtime sketch (the injected ~60 lines)

```js
window.internuntius = {
  vocare(methodus, argumenta)  → Promise   /* id++, pending map, postMessage */
  audire(eventus, functio)                 /* push-event subscription */
  _accipere(textus)                        /* JSON.parse → resolve/reject
                                              pending id, or fan out eventus
                                              to audire listeners */
  _superfluum(n)                           /* [INT-1] console.error: n
                                              messages dropped; some calls
                                              will never settle */
};
```

- Latin JS API to match the house (`internuntius.vocare("salve", {...})`).
- Rejected promise carries culpa.nuntius as the Error message.
- The runtime is a static C string constant in vitrea_macos.m; its content
  is spec'd here and golden-tested only indirectly (via the escaper +
  demo). Keep it dependency-free ES2017-ish; WKWebView's JS engine is
  always-current Safari, no transpiling.

## 9. Threading & robustness

- **Single thread, by construction.** Pump, cauda drain, dispatch, eval —
  all main thread. WebKit's main-thread-only rules are satisfied
  structurally, not by policing. If a C backend ever wants worker threads,
  the rule is: workers never touch vitrea; they hand results to the main
  loop (actor.c exists) which pushes via internuntius. Documented, not
  enforced.
- Script-message handler does copy+enqueue only (§5) — no user code inside
  WebKit callbacks, ever.
- webContentProcessDidTerminate → VITREA_EVENTUS_INTERITUS (§5).
- JSON from the webview is untrusted-ish input (it's our own frontend, but
  bugs happen): dispatch survives malformed anything (§6).

## 10. Metrics — fructus counters (house pattern)

On Vitrea: `nuntii_recepti`, `nuntii_amissi` (cauda drops),
`aestimationes` (evals), `petitiones_capsulae`, `petitiones_fractae` (404s).
On Internuntius: `vocationes_tractatae`, `culpae`, `eventus_missi`.
Measurement is product; the cauda-depth and any perf decisions get made on
these numbers.

## 11. Testing

- **internuntius: full credo suite, zero WebKit.** Envelope round-trips,
  registry hit/miss, culpa paths (bad JSON, unknown methodus, handler
  culpa), push events, replies captured via a memoria missor. This is the
  pure core and should be tested like one.
- **The escaper: golden tests.** Byte-exact expected output for quotes,
  backslashes, control chars, U+2028/U+2029, `</script`, UTF-8 multibyte.
- **vitrea: the last inch is manual**, same posture as tessera's real-tty
  inch. `tools/salve_vitrea.c`: capsula-embedded index.html with buttons
  that vocare C methods (echo, add, read a fructus counter), a C-side
  timer pushing an eventus, inspector enabled. Exit criterion: round trip
  visibly works; process idles near 0% CPU on the blocking pump.

## 12. The .app bundle question

v1 runs as a bare executable — WKWebView works, Web Inspector works, the
capsula scheme works. Named for later (likely alongside vitrea Phase C or
first real app):

- **`instrumenta/fasciculus`** (or tool in tools/): generate
  `Nomen.app/Contents/{MacOS/,Resources/,Info.plist}` from a small config.
  Mostly mkdir + plist emission; xml.c or a template suffices. No
  codesigning in scope (ad-hoc signing note at v2 if Gatekeeper bites).
- What bundling actually buys, when we want it: proper menu-bar app name
  (bare executables show the binary name), Dock icon, Info.plist keys —
  including ATS exceptions if dev mode needs them (§4 verify item), file
  associations, Retina/display declarations, and a sane home for
  WKWebsiteDataStore persistence (localStorage etc. — v1 explicitly makes
  no promises about where webview storage lands for a bare executable).
- Decision recorded: bundle tool is deferred but ANTICIPATED — nothing in
  vitrea may assume argv[0]-relative resource paths or otherwise fight
  relocation into a bundle. (Assets are in the binary via capsula, so this
  is nearly free; keep it true.)

## 13. Phasing proposal

- **Phase A — internuntius (pure).** Envelope, registry, dispatch, culpa
  paths, push, missor seam, escaper, fructus, full credo suite. No WebKit,
  no .m file. Exit: suite green under the strict flag set.
- **Phase B — vitrea attach + capsula origin.** vitrea_macos.m: WKWebView
  as contentView on a Fenestra, user-script injection, script-message →
  cauda (+ wake event, §3), aestimare, capsula:// handler + MIME table,
  fenestra_expectare_eventus addition. [SIM-1] Known build touchpoints:
  `-framework WebKit` + OBJC_SOURCES entries in compile_tests.sh,
  compile_tools.sh, compile_library.sh, compile_sputnik.sh; the demo joins
  compile_tests.sh's GUI-app list; capsula header generation precedes the
  demo compile (the book_assets dance). Exit: salve_vitrea demo round-trips
  vocare and eventus; idle CPU ~0%.
- **Phase C — polish + posture.** Dev-mode URL origin (+ ATS verdict),
  inspectabilis wiring, interitus handling, navigation policy, worklogs,
  MAP.txt entry. Exit: a second toy app builds against vitrea.h +
  internuntius.h alone with no reference to the demo.
- Bundle tool (§12): scheduled when the first app that ships wants it.

## 14. Named deferrals

- subview/hybrid embedding (web panel inside pixel-buffer UI) — door open
  via NSView reality, not v1
- multiple webviews per window / multiple windows per Vitrea
- native file dialogs (NSOpen/SavePanel), drag-drop, downloads, printing,
  notifications, menus — each its own future library, never vitrea (§1)
- `aperire externum` (target=_blank → system browser)
- async/streaming handlers (v1 handlers are sync; long work → worker +
  actor + push, §9)
- TS/JSDoc binding generation from the C registry (silva/officina synergy —
  genuinely attractive, genuinely later)
- cookies/WKWebsiteDataStore management, snapshot API
- cross-platform backends (WebView2, WebKitGTK): the platform seam is the
  .m file boundary itself; nothing in vitrea.h names WebKit
- amalgamated deliverable (§2)

## 15. Verificanda + open interview items [SIM-1]

From vitrea-simulatio-1.md. Verify on real hardware, fold verdicts at v2:

- secure-context status of custom-scheme pages (crypto.subtle etc.)
- fetch/CORS behavior for same-scheme capsula:// requests
- block syntax under -std=c89 -pedantic in ObjC mode (moot while
  block-free discipline holds)
- WKWebsiteDataStore / localStorage persistence for bare executables
- ATS treatment of plain-http loopback, non-bundled (§4, pre-existing)

Interview verdicts [INT-1] (2026-07-16, Q&A in
vitrea-simulatio-1-interview.md, folded above):

1. **Clipboard** → fenestra grows the minimal default menu (§3).
2. **Timers** → timeout arm + tempestivum next-deadline query (§3).
3. **Overflow** → drop + loud via _superfluum(n) (§5, §8).
