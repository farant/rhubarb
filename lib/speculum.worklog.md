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
