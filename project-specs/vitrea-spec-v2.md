# vitrea + internuntius — spec v2 (SELF-CONTAINED — build from this)

*2026-07-20. Supersedes vitrea-spec.md (v1.1). v2 = v1 + hardware
verdicts from the calibration spike (project-specs/vitrea-calibratio.md,
tools/vitrea_spica.{m,sh} — marked **[CAL]**) + two codebase explorations
(fenestra/tempestivum internals **[EXP-F]**, capsula runtime **[EXP-C]**).
Every assumption herein is either verified in code (file:line) or
verdicted on this machine's WebKit (macOS 14.8.5). Names SEALED
2026-07-16: **vitrea + internuntius**; scheme **capsula://**. Board:
quaestio 01KXZR7FJT.*

## 1. Thesis

A webview application layer for rhubarb: native window + system WKWebView
+ a JS↔C bridge, in the shape of Wails/Tauri but owned. No bundled
browser engine, no localhost requirement, no Node — the frontend ships
inside the binary via capsula and is served over a custom URL scheme; the
backend is plain rhubarb C89.

macOS/WKWebView **only**. The platform file is Objective-C (`.m`),
following `fenestra_macos.m` / `tls_macos.m`; public headers strict C89.

Two libraries, split so the reusable part stays pure:

- **vitrea** — WebKit embedding: attach a WKWebView to a Fenestra, serve
  assets from a Capsula, move strings JS↔C. All WebKit contact lives here
  and only here.
- **internuntius** — transport-agnostic RPC: method registry, JSON
  envelope, dispatch, replies, C→JS event push. Knows nothing about
  WebKit; the same envelope could later run over tcp or stdio.

Permanent boundary: **vitrea is webview attachment + asset origin +
string transport, forever.** Menus, dialogs, tray, notifications,
downloads, printing — never vitrea; each a future library against the
same Fenestra handle.

Frontend posture (named house rule): the injected runtime is
dependency-free ES2017 and house frontends are **vanilla JS, no build
step** — assets are exactly what capsula embeds. WKWebView's engine is
always-current Safari; no transpiling, no npm, no rot vector.

## 2. Shape

- **lib/ additions, not a subproject**: `include/vitrea.h` +
  `lib/vitrea_macos.m`; `include/internuntius.h` + `lib/internuntius.c`
  (pure C89); `tools/salve_vitrea.c` demo; worklogs per house rule.
- **Memory**: caller passes Piscina at creation. WebKit objects are the
  only non-piscina allocations; vitrea_destruere releases them under MRC
  (see §5 discipline — [EXP-F] confirmed: no `-fobjc-arc` anywhere in the
  compile scripts; fenestra_macos.m is pure MRC, zero blocks).
- **Context struct, no globals**; `Vitrea*` opaque.
- Amalgamation: named deferral (second consumer as forcing function).

## 3. Window model — vitrea rides fenestra

vitrea does NOT create windows. `vitrea_creare(piscina, fenestra, cfg)`
obtains the NSWindow via `fenestra_obtinere_tractationem_nativam()` —
**[EXP-F] which returns the NSWindow itself** (fenestra_macos.m:837-843,
`(__bridge vacuum*)fenestra->fenestra_ns`) — casts it, and installs a
WKWebView as its contentView (v1: webview IS the content, full-window).

Reuse verified [EXP-F]: NSApplication setup, activation, delegate, and
the pump are shared untouched. The pump (fenestra_macos.m:398-613) polls
`nextEventMatchingMask` with `untilDate:[NSDate distantPast]`
(non-blocking drain-until-empty) and calls `[NSApp sendEvent:]`
**unconditionally for every dequeued event** (line 589) — this is why
keyboard/mouse reach the webview natively, and why WKWebView's
out-of-process IPC is serviced whenever the loop runs.

**Event delivery is double, not split [EXP-F].** The translator pushes
Key/Mouse/Scroll Eventus into the cauda regardless of contentView AND
sendEvent delivers them to the webview. A vitrea app therefore drains the
Eventus cauda and IGNORES key/mouse genera, acting only on window-level
events (EVENTUS_CLAUDERE etc.). Harmless; documented, not enforced.

**Teardown ordering contract [EXP-F].** fenestra_destruere
(fenestra_macos.m:375-388) releases its OWN stored FenestraVisus (its
alloc at :339) and then the window — safe after vitrea's contentView
swap. But the window release drops the window's retain on the *current*
contentView (the webview). Therefore: **vitrea holds its own retain on
the WKWebView; `vitrea_destruere` runs BEFORE `fenestra_destruere`.**
After the swap, fenestra's displaced pixel view is alive-but-detached:
a Fenestra hosting a Vitrea is webview-only by contract.

Adjacent-library items (small, additive — Phase B'):

- **`fenestra_expectare_eventus(fenestra, ms_maximae)`** — blocking pump
  variant. **[EXP-F] design pinned:** parameter is **`s64` milliseconds**
  (fenestra has no ms precedent; tempestivum's idiom is s64 ms and its
  deadline feeds this argument). Implementation AS SHIPPED (B',
  2026-07-20): block once on `nextEventMatchingMask` with the real
  deadline; if an event arrives, **repost it atStart and delegate to
  `fenestra_perscrutari_eventus`** — ONE translation path, and the
  resize function-static needs no hoisting (the concern dissolved
  structurally). Idle verified live: 5×200 ms = 1.004 s with a still
  mouse; real events waking early is CORRECT (the loop re-enters).
- **[CAL] Wake contract, verified on hardware:** bridge messages arrive
  as runloop-source work; the spike proved the blocked pump services the
  script-message handler (+1.50s) yet KEEPS BLOCKING to timeout (5.0s).
  The enqueue path posts `NSEventTypeApplicationDefined` (type 15) via
  `[NSApp postEvent:atStart:NO]` — spike proved this ends the block at
  exactly message-arrival time. **[EXP-F] correction:** fenestra's
  translator does NOT swallow type 15 today — untranslated events still
  hit the unconditional sendEvent (:589). expectare's patch adds an
  explicit `casus NSEventTypeApplicationDefined: frange;` that produces
  no Eventus AND skips sendEvent for the synthetic wake.
- **fenestra grows a minimal default menu** [INT-1]: App menu (Quit) +
  Edit menu (cut:/copy:/paste:/selectAll:/undo:/redo: standard
  selectors). **[EXP-F] site pinned:** the `si (!NSApp)` init block in
  fenestra_creare (fenestra_macos.m:289-301), alongside
  setActivationPolicy. No NSMenu exists anywhere today (grep-verified) —
  without this, Cmd+C/V/X/A are dead in WKWebView fields.
- **Timers** [INT-1]: the loop computes ms-until-next-deadline and passes
  it to expectare. **[EXP-F] tempestivum reality check:** timers are
  entities (genus "System::Tempestivum") with STRING properties in an
  EntitasRepositorium; `tempestivum_processare` is a full linear scan
  re-parsing string props (tempestivum.c:259-423); no ordered deadline
  set exists, so the new aggregate next-deadline query is another linear
  scan (fine at small N — implement as `tempestivum_proxima_meta_ms()`),
  and tempestivum drags entitas_repositorium + cursus + internamentum
  with it. **Therefore tempestivum integration is OPTIONAL**: a one-clock
  vitrea app computes its own deadline and never links the stack. Note
  also tempestivum's clock is tempus.h (`tempus_nunc()`), not fenestra's
  mach ticks — one clock domain per app, don't mix.

## 4. Asset origin — capsula:// (WKURLSchemeHandler)

- Primary mode: register **`capsula://`** via
  `setURLSchemeHandler:forURLScheme:`. URLs carry the fixed dummy
  authority **`capsula://radix/...`** — **[CAL] verified:**
  `location.origin` is the well-formed `capsula://radix` and relative
  fetches resolve against it correctly (the bare `capsula://file` form
  parses the file as URL host; the authority fix is confirmed necessary
  and sufficient).
- **[CAL] Secure context: capsula:// pages ARE secure contexts** on
  current macOS WebKit — `isSecureContext=true`, `crypto.subtle` live,
  `crypto.randomUUID` live. The Tauri pain history does not apply here.
- **[CAL] Response class is load-bearing (twin-specimen verdict): the
  handler MUST reply with `NSHTTPURLResponse` status 200** (+
  Content-Type / Content-Length headers). A plain NSURLResponse delivers
  the body but `fetch()` sees `status=0` ⇒ `r.ok === false` ⇒ every
  well-written frontend breaks. Non-negotiable implementation rule.
- **[EXP-C] Path normalization is the handler's job (mandatory):**
  capsula keys are stored verbatim with NO leading slash, matched by
  exact case-sensitive byte compare (capsula.c:32-58), no index default,
  miss = `CAPSULA_NON_INVENTUM` (capsula.c:211-216). The handler strips
  scheme+authority, strips the leading `/`, and maps `""`/`"/"` →
  `configuratio.via_initialis`. `capsula://radix/index.html` ⇒ key
  `"index.html"` exactly; passing `"/index.html"` would 404.
- **[EXP-C] Serving strategy DECIDED — copy-and-reset:**
  `capsula_legere` ALWAYS allocates into the passed piscina (inflate for
  compressed entries, explicit byte-copy for uncompressed —
  capsula.c:221-257); zero-copy is not available through it. v1 rule:
  per-request `piscina_notare` → `capsula_legere` → NSData via
  **`dataWithBytes:`** (WebKit owns its copy) → respond → 
  `piscina_reficere` immediately. Two copies per request, ZERO lifetime
  coupling with the async WKURLSchemeTask — frontend assets are KBs,
  and the fructus counters (§10) will say if it ever matters. **Named
  optimization (deferred):** generate the frontend capsula with
  `compress=false` and serve via `capsula_invenire` +
  `dataWithBytesNoCopy:freeWhenDone:NO` over the static blob
  (process-lifetime stable, capsula.c:114) = true zero-copy; only valid
  for uncompressed entries.
- **MIME table**: static extension→MIME map in vitrea (html, css, js,
  mjs, json, svg, png, jpg, gif, webp, woff2, wasm, txt; fallback
  application/octet-stream). **[EXP-C] confirmed capsula stores no
  metadata** — via/offsets/sizes/compressa only.
- 404 → `didFailWithError` + `petitiones_fractae`.
- **Dev mode** (`VITREA_ORIGO_URL`): **[CAL] both ATS questions
  resolved** — bare (Info.plist-less) executables navigate
  `http://127.0.0.1` freely, and a capsula-origin page can `fetch()` a
  localhost server **when the server sends CORS headers**
  (`Access-Control-Allow-Origin`) — normal CORS applies, no
  mixed-content hard block. Servitium's dev mode must emit ACAO.
  Dev mode does not require bundling.

## 5. Transport — three WebKit primitives, one cauda

All JS↔C traffic is strings (JSON text).

1. **Inject** — one WKUserScript at document start, main frame only: the
   internuntius JS runtime (§8), a string constant in vitrea_macos.m.
2. **Receive** — one WKScriptMessageHandler (name `internuntius`). The
   handler does copy + enqueue ONLY — no dispatch inside WebKit
   callbacks, ever. The cauda **copies fenestra's ring mechanics
   verbatim [EXP-F]** (fixed 256-slot in-struct ring, caput/cauda/
   numerus, `% MAXIMUS` wrap — fenestra_macos.m:15-28, 246-275) **but
   replaces the overflow policy**: fenestra silently drops the incoming
   event with no counter; vitrea drops + bumps `nuntii_amissi` + evals
   `internuntius._superfluum(n)` (drop + loud, [INT-1]). The enqueue
   path posts the synthetic wake event (§3).
   **Message memory contract:** Vitrea owns `piscina_nuntiorum`;
   enqueued chordae are copied into it; the reset fires at the START of
   each drain burst (first `vitrea_obtinere_nuntium` call after a
   non-empty enqueue period) — **apps MUST drain to empty**; drained
   chordae are valid only until the next drain call. Tractare
   immediately, never stash. Dispatch gets its own per-call mark/reset.
   **[EXP-C]** mark/reset API verified: `piscina_notare(piscina)` →
   `PiscinaNotatio`, `piscina_reficere(piscina, notatio)`
   (piscina.h:114-126); allocations after the mark are dead after
   reficere — the discipline is vitrea's to invent (no prior per-request
   consumer exists).
3. **Send** — `evaluateJavaScript:` wrapper, main-thread by
   construction, always nil completionHandler (MRC, zero blocks —
   [EXP-F] confirmed as the house ObjC dialect). Every send is
   `internuntius._accipere("<json-as-escaped-JS-string-literal>")`.
   **Escaper discipline**: escape `\` `"`, control chars, U+2028/U+2029
   (legal JSON, illegal raw in JS literals), and `<` as <. Pure
   function, golden credo tests.

Navigation policy v1: main-frame navigation restricted to the configured
origin; everything else refused. `webContentProcessDidTerminate` →
delivered through the same cauda as genus INTERITUS (§7); app decides
(typically reload; pending JS promises die with the page — self-healing;
the demo models the reload idiom: boot → one `vocare('status')` → C
pushes deltas thereafter).

## 6. internuntius — envelope + dispatch (pure C89)

Wire format is **json** (browser-native, Web-Inspector-debuggable);
nuntium stays the C↔C binary format.

```
JS → C   { "id": 7, "methodus": "salve", "argumenta": { ... } }
C → JS   { "id": 7, "fructus": { ... } }
         { "id": 7, "culpa": { "nuntius": "..." } }
C → JS   { "eventus": "horologium", "datum": { ... } }   (push, no id)
```

- **Registry**: `internuntius_praebere(inx, "salve", tractator, datum)`
  — tabula_dispersa, name → fn + userdata.
- **Dispatch**: `internuntius_tractare(inx, nuntium)` — parse, look up,
  call, serialize, hand the reply to the **mittere callback** given at
  creation (the transport seam; a probatio's missor appends to a list).
- Replies route by id: the envelope permits delayed/out-of-order replies,
  so v1's sync handlers are an implementation detail, not an API
  commitment — async C handlers later change no wire format.
- **Push**: `internuntius_eventum_mittere(inx, eventus, datum_json)`.
- Malformed JSON ⇒ id-less culpa + `culpae` bump; unknown methodus ⇒
  culpa reply. Never a crash.
- `internuntius_methodi(inx)` — registry introspection (list of method
  names); trivial now, seed of the TS/JSDoc binding deferral.

## 7. API sketch (C89 — API-first; ≤31-char names)

```c
/* ===== internuntius.h (pure) ===== */
nomen structura Internuntius Internuntius;

nomen JsonValor* (*InternuntiusTractator)(
    JsonValor* argumenta, Piscina* piscina,
    vacuum* datum, chorda* culpa_exitus);

nomen vacuum (*InternuntiusMissor)(vacuum* datum, chorda textus);

Internuntius* internuntius_creare  (Piscina* piscina,
                                    InternuntiusMissor mittere,
                                    vacuum* mittere_datum);
b32  internuntius_praebere         (Internuntius* inx,
                                    constans character* methodus,
                                    InternuntiusTractator tractator,
                                    vacuum* datum);
/* AS-SHIPPED (Phase A): tractare/eventum_mittere take an EXPLICIT
 * call piscina — a pure library owns no hidden growing arena; the
 * caller (vitrea's drain loop) holds the mark/reset. Missor
 * contract: textus valid only inside the missor call — copy to
 * retain. */
vacuum internuntius_tractare       (Internuntius* inx, chorda nuntium,
                                    Piscina* piscina_vocationis);
vacuum internuntius_eventum_mittere(Internuntius* inx,
                                    constans character* eventus,
                                    JsonValor* datum,
                                    Piscina* piscina_vocationis);
Xar* internuntius_methodi          (Internuntius* inx, Piscina* piscina);
InternuntiusFructus internuntius_fructus(constans Internuntius* inx);
chorda internuntius_effugere_js    (chorda textus, Piscina* piscina);

/* ===== vitrea.h ===== */
nomen structura Vitrea Vitrea;

nomen enumeratio {
    VITREA_ORIGO_CAPSULA,
    VITREA_ORIGO_URL
} vitrea_origo_t;

nomen structura {
    vitrea_origo_t      origo;
    Capsula*            capsula;       /* ORIGO_CAPSULA */
    constans character* via_initialis; /* "index.html" */
    constans character* url;           /* ORIGO_URL */
    b32                 inspectabilis; /* Web Inspector; macOS 13.3+ */
} VitreaConfiguratio;

Vitrea* vitrea_creare    (Piscina* piscina, Fenestra* fenestra,
                          constans VitreaConfiguratio* configuratio);
vacuum  vitrea_destruere (Vitrea* vitrea);   /* ANTE fenestra_destruere */

nomen enumeratio {
    VITREA_NUNTIUS_PONS,
    VITREA_NUNTIUS_INTERITUS
} vitrea_nuntius_genus_t;

b32     vitrea_obtinere_nuntium (Vitrea* vitrea, chorda* nuntium,
                                 vitrea_nuntius_genus_t* genus);
vacuum  vitrea_aestimare (Vitrea* vitrea, chorda javascript);
vacuum  vitrea_recargare (Vitrea* vitrea);

/* ===== fenestra.h additum ===== */
vacuum  fenestra_expectare_eventus (Fenestra* fenestra, s64 ms_maximae);

/* ===== tempestivum.h additum (optionale) ===== */
s64     tempestivum_proxima_meta_ms (AdministratorTempestivi* adm);
```

Canonical loop:

```c
dum (!fenestra_debet_claudere(fenestra)) {
    fenestra_expectare_eventus(fenestra, ms_proximae);
    dum (fenestra_obtinere_eventus(fenestra, &ev)) {
        /* window-level solum; key/mouse ignorantur (§3) */
    }
    dum (vitrea_obtinere_nuntium(vitrea, &nu, &genus)) {
        si (genus == VITREA_NUNTIUS_PONS) {
            internuntius_tractare(inx, nu);
        } aliter {
            vitrea_recargare(vitrea);
        }
    }
}
```

## 8. JS runtime sketch (the injected ~80 lines)

```js
window.internuntius = {
  vocare(methodus, argumenta)  → Promise   /* id++, pending map */
  audire(eventus, functio)                 /* push subscription */
  _accipere(textus)                        /* resolve/reject or fan out */
  _superfluum(n)                           /* console.error: n dropped */
};
```

- Latin API; rejected promise carries culpa.nuntius.
- Dependency-free ES2017; no transpiling.
- v1.5 named: client-side flow control — cap in-flight vocare at N,
  queue the rest in JS (page memory, not arena) — turns legitimate
  bursts from drop events into throttling; cauda depth then only
  matters for runaway bugs.

## 9. Threading & robustness

Single thread by construction: pump, drain, dispatch, eval — all main
thread. Workers never touch vitrea; they hand results to the main loop
(actor.c) which pushes via internuntius. Script-message handler does
copy+enqueue only. Dispatch survives malformed anything.

## 10. Metrics — fructus counters

Vitrea: `nuntii_recepti`, `nuntii_amissi`, `aestimationes`,
`petitiones_capsulae`, `petitiones_fractae`. Internuntius:
`vocationes_tractatae`, `culpae`, `eventus_missi`. Perf decisions (incl.
the zero-copy optimization, §4) get made on these numbers.

## 11. Testing

- **internuntius: full credo suite, zero WebKit** — envelope
  round-trips, registry hit/miss, culpa paths, push, methodi, replies
  via memoria missor.
- **Escaper goldens**: quotes, backslash, control bytes, U+2028/2029,
  `<`, UTF-8 multibyte passthrough. (chorda datum is SIGNED i8 — every
  0x80+ comparison casts, under -Wconversion.)
- **vitrea: the last inch is manual** — `tools/salve_vitrea.c`: capsula
  index.html, buttons (echo, addere, fructus read), 1 s clock push via
  the timeout arm, inspector on. Exit: visible round trip; idle ~0% CPU.
- The calibration spike (tools/vitrea_spica.{m,sh}) stays as the
  re-runnable oracle pin — re-run after macOS updates.

## 12. The .app bundle question

v1 runs as a bare executable — **[CAL] verified**: WKWebView, the
capsula scheme, Web Inspector, http loopback all work unbundled, and
webview storage (localStorage) persists at
**`~/Library/WebKit/<process-name>`** across runs. Bundling still buys:
menu-bar app name, Dock icon, Info.plist keys, file associations. Bundle
tool (`instrumenta/fasciculus`) deferred but anticipated — nothing in
vitrea may fight relocation into a bundle.

## 13. Phasing

- **Phase A — internuntius (pure).** Envelope, registry, dispatch,
  culpa paths, push, methodi, missor seam, escaper, fructus, credo
  suite. No WebKit. Independent of every [CAL] verdict — can start
  immediately. Exit: suite green under the strict flags.
- **Phase B' — fenestra riders (independent small pull).**
  expectare_eventus (s64 ms; shared resize static; type-15 swallow),
  default menu in the `si (!NSApp)` block, tempestivum_proxima_meta_ms.
  Testable against existing pixel apps before vitrea exists.
- **Phase B — vitrea attach + capsula origin.** vitrea_macos.m:
  contentView swap (+ retain/teardown contract §3), user-script
  injection, script-message → cauda + wake, aestimare, capsula://
  handler (normalization + NSHTTPURLResponse-200 + copy-and-reset +
  MIME table). Build touchpoints: `-framework WebKit` + OBJC_SOURCES in
  compile_tests/tools/library/sputnik scripts; demo joins the GUI-app
  list. **[EXP-C] simplification: no build-order dance** — capsula
  generation is offline; generated capsula_*.{h,c} are committed and
  linked as ordinary sources (book_assets precedent,
  compile_tests.sh:148-149).
- **Phase C — polish + posture.** Dev-mode URL origin (+ servitium ACAO
  emission), inspectabilis, interitus/reload idiom, navigation policy,
  worklogs, MAP.txt. Exit: a second toy app builds against the two
  headers alone.

## 14. Named deferrals

- subview/hybrid embedding; multiple webviews/windows
- native dialogs, drag-drop, downloads, printing, notifications, menus
  beyond the fenestra default — each its own library
- `aperire externum`; async/streaming handlers; cookies/dataStore
  management; snapshot API in vitrea proper
- TS/JSDoc binding generation from the registry (methodi is the seed)
- JS-runtime unit harness via JavaScriptCore (system framework — same
  engine; only if the runtime grows past ~150 lines)
- zero-copy capsula serving (uncompressed + capsula_invenire — §4)
- **vitrea-sonda** (observation channel): CDP does not exist in WebKit
  (protocols forked with Blink 2013; the inspector protocol is private;
  Playwright drives only its own patched builds). Agent eyes/hands ride
  owned primitives — aestimare to drive + query, window snapshot for
  pixels ([CAL] proven end-to-end: takeSnapshot → PNG → read by
  Claude). Future shape: `-sonda` stdio/MCP side channel, sonda.sh
  lineage. Safari Web Inspector via `inspectabilis` = human devtools.
- cross-platform backends (WebView2, WebKitGTK) — the .m seam
- amalgamated deliverable

## 15. Verificanda — NONE OPEN

All v1 ⚠/[VERIFY] items are decided (vitrea-calibratio.md): secure
context ITA; same-scheme fetch same-origin with NSHTTPURLResponse-200
required; wake hole + synthetic wake proven; ATS non-issue unbundled;
localStorage persists (~/Library/WebKit/<process>); block-syntax
question moot (block-free MRC is the dialect, [EXP-F] confirmed).
Interview verdicts [INT-1] carried: fenestra default menu; timeout arm +
tempestivum query (now optional-dependency, [EXP-F]); overflow
drop + loud.
