# speculum — spec v2 (2026-07-20)

*Status: post-exploration. Supersedes speculum-spec-v1.md where they differ.
Sources: three exploration reports (vitrea/capsula/internuntius surfaces;
build tooling; runtime payload path) — all claims below are file:line
verified by them. Interview record: speculum-interview.md.*

## I. Amendments to v1 (what exploration changed)

1. **Summon path (amends interview decision #6).** "JS-side shortcut" has a
   bootstrapping hole: a page keydown listener requires the bootstrap
   already injected, but vitrea has NO generic user-script API (one
   hardcoded WKUserScript, VITREA_CURSUS_JS, lib/vitrea_macos.m:135-182;
   WKWebViewConfiguration immutable after creare, :473), and eager eval at
   load has no "page loaded" signal to hook. Meanwhile key events are
   DOUBLE-DELIVERED: the C loop sees every keydown via
   fenestra_obtinere_eventus AND the webview gets it via [NSApp sendEvent:]
   (lib/fenestra_macos.m:491-508, 659) — and Cmd+Shift+D is unclaimed by
   menu equivalents (:391-431). **Resolution: native summon + eval
   delivery.** The app's existing event drain calls
   `speculum_tangere(spec, &eventus)`; on Cmd+Shift+D speculum evals a
   self-healing stub through an aestimator seam. UX identical to the
   interview decision (press Cmd+Shift+D in the window); JS owns only Esc
   dismissal (never Cmd+Shift+D — double-handling would toggle twice).

2. **json serialization prerequisite: dissolved.** The house json lib
   serializes: `json_scribere`, `json_scribere_ad_aedificator`
   (json.h:221-239). Additionally `json_chorda_creare` stores string nodes
   ZERO-COPY (points at existing bytes, json.c:1400-1401) — the manifest
   tree can wrap capsula-resident source bodies without duplication.

3. **Payload size: confirmed safe, with a budget.** No hard ceiling
   anywhere: every pool on the path is piscina_generare_dynamicum, which
   grows by chaining chunks and never fails short of OOM (piscina.c:145-190)
   and RETAINS grown chunks across reficere (piscina.c:476-504 —
   self-tuning after first call). The real cost is escape scratch:
   appendere_evasus_json allocates worst-case 6× per string body, retained
   until pool reset (chorda_aedificator.c:452-472) → a ~2MB payload costs
   ~12-20MB transient in the call-piscina, plus a second full JS-literal
   escape pass in vitrea's own piscina_aestimationis (vitrea_macos.m:604-617).
   Fine — but the wearer should give piscina_vocationis a generous initial
   chunk (16MB+) to avoid first-call malloc churn. The 256-entry inbound
   ring (VITREA_MAXIMUS_NUNTIORUM) limits JS→C message COUNT only — not
   relevant to this C→JS payload.

4. **Generated-file home (amends the capsula precedent — DECISION FOR
   FRAN, §VI).** Existing generated capsulae are committed + judged
   normally. But speculum_fontium embeds per-build provenance (timestamp,
   commit hash) — committing it means every build dirties the tree and the
   embedded hash is always one commit behind. Recommendation: generate
   every build into `build/` (gitignored, pruned by census find
   (examen.sh:127-132) and the examen-custos hook (*/build/* exclusion) —
   sane lint treatment for free). This deviates from the committed-capsula
   precedent for a principled reason: those embed stable ASSETS, this
   embeds BUILD PROVENANCE.

5. **Compression is mandatory, not optional.** The generated .c costs
   ~6.6 chars per blob byte (hospes: 1581B → 10509B). Uncompressed 2MB of
   source → ~13MB generated file. Capsula supports per-entry gzip
   (`<name>_compress`, inflated on read via flatura_gzip_inflare,
   capsula.c:221-236); C source gzips ~4-5×. Compressed: blob ~400-500KB,
   generated .c ~3MB, binary +~500KB. Acceptable.

6. **The `generare:` directive system is NOT the vehicle** — it passes
   only literal string args (tools/generare.c:300 exec of comment
   directives) and cannot receive the build's live SOURCE_FILES array.
   Integration is a direct pre-compile step in compile_tests.sh's GUI-app
   path, mirroring the existing per-app capsula_caudae_adiungere post-link
   hook (compile_tests.sh:383-408).

## II. Architecture (concrete)

### A. Build-time: speculum_generare

New tool `tools/speculum_generare.c` → `bin/speculum_generare`. Invocation
(from compile_tests.sh, per app in a SPECULUM_APPS list, before
compile_gui_app):

    ./bin/speculum_generare <app_titulus> <exitus.c-via> <fabrica-via> \
        -- "${SOURCE_FILES[@]}" <app.c>

Work:
1. **Closure**: one `clang -MM $INCLUDE_FLAGS <fontes>` (the blessed shape:
   gesta/tabularium.sh:54-77 / officina/legatus.sh:53-79 — awk joins
   continuations, keep house paths, sort -u; on empty output: verbose
   fallback to the full include/*.h glob, "garrulum, numquam caecum").
   Vendor paths routed to exclusa (size + causa "vendor"), system headers
   dropped.
2. **Lore**: for each embedded lib/x.c, embed lib/x.worklog.md if present.
3. **Scripta**: the build script itself + speculum's own front-end assets
   (lib/speculum_assets/speculum.js, speculum.css) — so the overlay works
   in BOTH origin modes (capsula:// subresources do NOT resolve in URL
   mode — scheme handler only installed for VITREA_ORIGO_CAPSULA,
   vitrea_macos.m:464-468; everything must travel by bridge/eval).
4. **Provenance** (net-new, no house precedent to honor): `git rev-parse
   HEAD`, dirty via `git status --porcelain`, `cc --version`, the flag
   list, `date -u +%Y-%m-%dT%H:%M:%SZ`, the literal compile command.
   Written as capsula entry `proventus.json`.
5. **Manifest**: capsula entry `index.json` — fontes/documenta/scripta
   listings + `exclusa` (vendor entries; the quine entry for the emitted
   file itself: causa "generatum — continet hos ipsos fontes"; digestum
   deferred to the future sha256 library — v1 ships magnitudo only).
6. **Emit** the capsula blob (format is simple + documented: 12B header
   magic "CAPS", 20B TOC entries, string table, gzip bodies —
   capsula.c:136-141) as `speculum_fontium_<app>.{h,c}` hex-array C
   (BYTES_PER_LINE pattern, capsula_generare.c:36) with
   `constans CapsulaEmbed capsula_speculi_<app>`. Either by invoking the
   blob/emitter logic shared with capsula_generare (small refactor) or
   self-contained (~100 lines; needs flatura's gzip COMPRESS entry point —
   verify name, open item §VII.1).

### B. Runtime: speculum.{h,c}

    nomen structura Speculum Speculum;

    /* aestimator: seam qua speculum JS in paginam evaluat.
       Vitrea praebet vitrea_aestimator (novum, §V) — aut app adaptorem
       scribit. Idem mos ac missor internuntii. */
    nomen vacuum (*SpeculumAestimator)(vacuum* datum, chorda js);

    Speculum* speculum_creare(Piscina* piscina,
                              constans CapsulaEmbed* fontes,
                              Internuntius* inx,
                              SpeculumAestimator aestimator,
                              vacuum* aestimator_datum);

    /* in effusione eventuum ansae app vocanda; VERUM si consumptus */
    b32 speculum_tangere(Speculum* s, constans Eventus* eventus);

- `speculum_creare`: capsula_aperire on the embed; registers ONE bridge
  method `speculum_obtinere` via internuntius_praebere; builds the eval
  stub ONCE into its piscina (static — reused every press).
- `speculum_tangere`: matches Cmd+Shift+D from the Eventus stream; on
  match, evals the self-healing stub:
  `if (window.speculum_praesens) { speculum_commutare(); } else { <bootstrap: inject CSS, define components, build overlay, set speculum_praesens> }`
  Self-healing means a dev-mode page reload (which wipes page JS but not C
  state) recovers on the next press with zero C-side bookkeeping.
- `speculum_obtinere` handler: on FIRST call, capsula_legere every entry
  into speculum's own piscina (lazy — apps that never summon pay only the
  blob bytes in the binary; gzip inflates once, cached). Builds the
  response tree with `json_chorda_creare` (zero-copy) over the cached
  bodies:

      { "proventus": {…}, "fontes": [{via, magnitudo, corpus}…],
        "documenta": […], "exclusa": […] }

  internuntius serializes it in the call-piscina and vitrea_missor carries
  it to the page (~4 copies total, both escape passes correct for source
  bytes: json escaper passes UTF-8 through, chorda_aedificator.c:229-232;
  JS-literal escaper handles `<` and U+2028/9, internuntius.c:347-432).

### C. Front-end: lib/speculum_assets/

`speculum.js` + `speculum.css`, full Latin, snake_case, no shadow DOM,
.speculum-* prefix + scoped reset. Custom elements `<fons-arbor>`
(tree, grouped lib/ include/ app scripta documenta, type-to-filter) and
`<fons-visor>` (line numbers, scroll-to-line). Tab chrome: FONS | DE.
Esc dismisses. First eight --thema-* tokens consumed via
`var(--thema-*, <fallback>)` — exploration confirmed NO CSS custom
properties exist anywhere yet; this genuinely births the contract.
Highlighting: comments/strings/C-keywords regex; the latina.h macro list
can be EXTRACTED AT RUNTIME from the embedded latina.h source itself
(it's in fontes) — self-referential, zero-maintenance.
Page API as shipped: `internuntius.vocare('speculum_obtinere', {})` →
Promise (vitrea_macos.m:141-149); reject path `.catch(e => e.message)`.
Dev workflow: the overlay page can be developed standalone in a browser
against a mock payload JSON before touching the app.

### D. Wearer: probatio_vitrea_hospes

    #include "speculum_fontium_hospes.h"          /* generatum */
    ...
    spec = speculum_creare(piscina, &capsula_speculi_hospes, inx,
                           vitrea_aestimator, vitrea);
    ...
    dum (fenestra_obtinere_eventus(fenestra, &eventus))
    {
        speculum_tangere(spec, &eventus);          /* erat: ';' */
    }

Plus: bump piscina_vocationis initial size (currently 4MB,
probatio_vitrea_hospes.c:91-92) to 16MB for first-call hygiene (§I.3).

## III. Testing

- **probatio_speculum.c (headless — no GUI)**: copy the fake-missor
  pattern from probationes/probatio_internuntius.c exactly (MissorMemoriae
  capture struct :21-24, _missor_memoriae copies+stores :26-54,
  _tractare_literis :154-159, strstr asserts on captured JSON, two-pool
  split 16MB/1MB :185/:233). Add a fake AESTIMATOR the same way (captures
  evaled chordas). Asserts: praebere registers; tangere on Cmd+Shift+D
  evals a stub containing "speculum_praesens"; non-matching events pass;
  obtinere response contains proventus fields, a known source line, the
  quine exclusa entry, documenta. Fixture: a small committed fixture
  capsula generated over 2-3 tiny files by the real generator.
- **Generator probatio**: run bin/speculum_generare on the fixture set;
  capsula_aperire the emitted embed; assert entry count, closure includes
  a header only reachable transitively, gzip round-trip byte-identity,
  provenance keys present. Dirty-flag both ways = manual [CAL] item.
- **GUI smoke (self-announcing)**: `-speculum-fumus` flag makes the wearer
  eval the bootstrap + a probe that reads a known line of its own source
  from the payload and echoes it through `confirmare` — prints or it
  didn't happen. GUI-smoke plumbing law applies (stdout → file, sleep,
  read, kill).
- credo API: credo_aperire(piscina) / CREDO_VERUM / CREDO_AEQUALIS_I32 /
  credo_imprimere_compendium / exit on credo_omnia_praeterierunt.

## IV. Phases

- **0 — generator**: flatura-compress check; tools/speculum_generare.c;
  compile_tests.sh wiring (SPECULUM_APPS list + pre-compile step + the
  generated .c into the app's object set); generator probatio green.
- **A — C library**: speculum.{h,c}; headless probatio (fake missor +
  fake aestimator); the vitrea_aestimator seam (§V) if chosen.
- **B — overlay**: speculum_assets; components; reader features;
  standalone-browser iteration against mock payload.
- **C — wearer**: probatio_vitrea_hospes integration, both postures
  [CAL]; self-announcing fumus; worklog + board seal.

## V. The one vitrea touch (DECISUS 2026-07-20: option a)

The aestimator seam needs a `vacuum (*)(vacuum*, chorda)` shape.
`vitrea_aestimare(Vitrea*, chorda)` (vitrea.h:109-112) is not seam-shaped.
Options:
- **(a) Add `vitrea_aestimator(vacuum* datum, chorda js)` to vitrea** — a
  4-line seam-shaped twin, exactly the precedent of vitrea_missor
  (vitrea_macos.m:592, written seam-shaped for internuntius). Generic
  capability, not a speculum hook; doctrine-compatible. RECOMMENDED.
- (b) Zero vitrea change: each wearer writes a 3-line adapter function.
  Purer, but every app repeats it forever.

## VI. Open decisions — RESOLVED (Fran, 2026-07-20)

1. §I.4 — generated-file home: **build/ + gitignore +
   regenerate-every-build.** DECISUS.
2. §V — **vitrea_aestimator seam** (generic seam-shaped twin, the
   vitrea_missor precedent). DECISUS.
3. Regeneration cadence: n/a — build/ won.

## VII. Small open items (verify during Phase 0)

1. flatura gzip COMPRESS entry point name (inflate is
   flatura_gzip_inflare; capsula_generare compresses today — find/share
   its path).
2. Eventus key-event field names for the Cmd+Shift+D match in
   speculum_tangere (fenestra.h claves/modificatores shape).
3. Include path for the generated header from the wearer if build/ home
   wins (-Ibuild addition to INCLUDE_FLAGS; check examen's view of it —
   stale-header-shadow class).
4. Whether capsula blob dedups identical entries (speculum.js embedded
   per-app ×N apps is fine either way — small).
