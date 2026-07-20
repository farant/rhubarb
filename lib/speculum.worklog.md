# speculum.c worklog

## 2026-07-20 — Phase A (C library + aestimator seam)

- **API**: `speculum_creare` (capsula aperire + praebere
  "speculum_obtinere" + stipes built once) / `speculum_tangere`
  (Cmd+Shift+D match → aestimator(stipes)). The aestimator seam
  mirrors internuntius's missor; vitrea gained `vitrea_aestimator` —
  a 4-line seam-shaped twin of vitrea_missor, the ONLY vitrea change,
  generic not speculum-specific.
- **Key-match facts** (from fenestra_macos.m): letters arrive as
  UPPERCASE ASCII ('D', keycode switch line ~190); `modificantes` is
  the RAW AppKit modifierFlags value, which happens to equal the
  MOD_* enum values — Cmd = **MOD_SUPER** (0x100000), NOT
  MOD_IMPERIUM (that is Control, 0x40000, despite the name).
- **Self-healing stub**: `(function(){ if (window.speculum_praesens)
  { speculum_commutare(); return; } window.speculum_stylus_fons =
  "<css>"; <speculum.js> })();` — CSS enters as a JS string literal
  via internuntius_effugere_js (it is PUBLIC, internuntius.h:122).
  No C-side page state: dev-mode reload recovers on the next press.
- **Lazy cache**: first obtinere inflates ALL capsula entries into
  speculum's piscina (retained for app lifetime); the response tree
  is rebuilt per call in the CALL piscina with json_chorda_creare
  zero-copy nodes pointing at the cached bytes — safe across pv
  resets because the bytes live in speculum's pool.
- **Parsers**: proventus key=value → flat object via
  json_objectum_ponere_chorda (takes chorda keys — no cstr dance);
  exclusa TSV → array of {via, magnitudo, digestum, causa} all as
  strings (no numeric-parse dependency; JS can Number() at will).
- **Payload reality**: hospes capsula ≈ 292 files ~6MB inflated;
  full serialize + fake-missor capture = 0.43s in the probatio.
  The fake missor and fake aestimator are the SAME capture function —
  identical seam signatures pay off in tests too.
- **Gotcha caught in review**: writing JS inside C strings invites
  Latin leaking INTO the JS — briefly wrote "redde_nihil: return;".
  Identifiers Latin, keywords JS; the two languages meet in one
  string literal and only one of them is ours.

## 2026-07-20 — Phase B (the overlay, lib/speculum_assets/)

- **Features shipped**: tabbed velamen (FONS | DE); fons-arbor with
  its own filter field + grouped tree; fons-visor with line numbers,
  copy button, stateful highlighting (block comments across lines,
  strings, C keywords, latina macros, preprocessor); full-text search
  in the header (≥2 chars, 250ms debounce, 500-match cap, jump with
  line highlight); DE = provenance table + summary + exclusa table;
  Esc clears search first, then closes; first eight --thema-* tokens
  consumed via var(..., fallback) — the theming contract is live.
- **Latina highlighting is self-referential**: the macro list is
  regex-extracted at runtime from the embedded include/latina.h —
  zero maintenance, the mirror teaches itself the dialect.
- **Custom tags WITHOUT customElements.define**: registration
  requires ES6 classes; unregistered <fons-arbor>/<fons-visor> style
  fine as block elements and keep the JS ES5-shaped. Registration
  graduates with the component library.
- **Three bugs self-caught before any run**: (1) html_effugere
  replacing chars with THEMSELVES (identity, not entities — every
  #include <...> would have vanished into the DOM as fake tags);
  (2) a literal invisible U+200B embedded in source (fixed to
  "​" via perl byte-level replace — Edit cannot match what it
  cannot see); (3) filter and search conflated into one field where
  the interview specified two features.
- **Headless overlay probatio (the find of the phase)**:
  probatio_velaminis.js runs the REAL speculum.js in JavaScriptCore
  via osascript -l JavaScript (same engine family as WKWebView, no
  node) against a ~40-line fake DOM. Two tricks: mock vocare returns
  a SYNCHRONOUS thenable (real Promises may never flush microtasks
  before osascript exits) and setTimeout stubs to immediate-call.
  17 asserts execute build/tree/open/highlight/search/jump/filter.
  Failure = throw = non-zero exit (porta per codicem exitus). Wired
  into run_speculum — every build now executes the overlay logic.
  First run's 2 failures were both ASSERTION bugs (quotes need no
  escaping in text nodes; first "redde" match is latina.h itself).
- **Chrome extension was down** — browser eyes deferred to Fran:
  open lib/speculum_assets/probatio_visus.html (mock-bridge harness,
  auto-opens overlay; hostile serif/light host page proves the
  scoped reset).
