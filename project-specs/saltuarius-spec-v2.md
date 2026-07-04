# Saltuarius — Spec v2 (codebase-grounded)

**STATUS: v0 SHIPPED (2026-07-03).** All phases (0/A/B/C/D)
closed, all manual bars passed including the daily-driver bar
(interview Q20), suites 13/13, §2 API-pressure ledger fully
shipped in silva. The build record and the v0.1 backlog live in
saltuarius/phase-log.md (v0 RELATIO at the tail).

*Supersedes saltuarius-spec.md (v1) as implementation truth; v1
remains the intent record, saltuarius-interview.md the decision
record. This version is grounded in three agent explorations
(2026-07-03): the silva.h A–K surface map, the tessera.h UI/testing
map, and the plumbing scout's I/O survey + live timing probes.*

---

## 0. What changed from v1 (read this first)

1. **Saltuarius is the FIRST true consumer of silva.h.** Nothing in
   the repo compiles against the amalgam boundary alone — lustrum
   and every probatio reach into fontes/. The surface has never
   been exercised as a contract. The exploration produced a ranked
   **API-pressure ledger (§2)** — mostly one-line prototypes for
   functions already in the shipped amalgam, ONE genuine engine
   change (strata retention).
2. **Phases B and C are gated on small silva.h additions.** The
   phasing (§9) now interleaves two "Silva additiones" mini-
   packages. Phase A needs nothing and starts immediately.
3. **REVISION of interview Q10 (keep-all parse cache)** — measured
   reality: 84–242 MB peak arena PER PARSED ROOT (§8). Keep-all
   hits gigabytes within a dozen files. v2 elevates the LRU cap
   from v0.1 to v0 (small N, per-root piscinae make eviction
   trivial). FLAGGED FOR FRAN.
4. **REVISION of preview coloring**: colored previews in the
   columns world are OFF until public lexing exists (and maybe
   forever — 44–80 ms parse per hover is sluggish; lex-only would
   be fine but isn't public yet). Previews are plain text head in
   v0; the SOURCE VIEW is where color lives.
5. **No filesystem pons needed**: the monorepo already wraps all
   file/directory I/O (iter_directoria, filum, via — §4). The
   test seam is dependency injection of listing data, not a vtable.
6. **Silva never reads disk** — the host supplies every included
   header's bytes via praebere. Saltuarius owns -I resolution and
   header feeding (§5). This is by design ("silva plagulas numquam
   aperit") and shapes the nexus module.

## 1. Ground truths from the explorations

- **SilvaExpansio is opaque** through silva.h (forward-declared,
  no body). Everything the preprocessor learned — source-file
  table, macro journal, include graph, region tree — is
  unreachable publicly. This locks the TOC lists and fons_index→
  path resolution until §2 accessors land.
- **SilvaToken is transparent and rich** (silva.h:212–226): genus,
  valor, byte_offset, longitudo, linea/columna (1-based),
  fons_index, origo (inline, never absent). SilvaOrigo is a
  transparent tagged union with datum.expansio.{corpus, invocatio,
  nomen_macro, caecatio}. The money-shot DATA is fully public;
  only the file-path lookup and convenience walkers are not.
- **parsura->lexemata is the fully-EXPANDED stream** — not the
  file as written. Layer-0 tokens have no public source today.
- **TesseraOpus is transparent**; cellulam_legere reads the BACK
  buffer — goldens assert drawn cells without calling praesentare.
- **Tessera never splits a UTF-8 rune** (decode → one cell →
  advance 1 column). 1 rune = 1 column always; column index ==
  rune index. No wcwidth (wide glyphs drift; irrelevant for C).
- **Tessera has no viewport/clip/fill primitives** — the app owns
  scroll math, gutters, interior-pane truncation, and popup
  interior fills (§6).
- **Parse cost tracks the include closure, not file size**:
  silva_parsare.c (36 KB) = 79.9 ms / 141.6 MB peak because it
  pulls 127 headers; the 460 KB amalgam standalone = 148.9 ms /
  242 MB. All probes roundtripped byte-exact through sceleton
  ERROR nodes — the degradation posture holds under fire.

## 2. THE API-PRESSURE LEDGER (silva.h additions)

The deliverable insight saltuarius was designed to produce —
arriving at spec time instead of mid-build. Ranked by blocking
severity; "declaration-only" means the function already exists in
the shipped amalgam and needs one prototype in the hand-written
silva.h (additive; all M1 gates re-run).

| # | Addition | Kind | Blocks |
|---|----------|------|--------|
| 1 | `SilvaXar* silva_lexare(SilvaPiscina*, const char* fons, unsigned int mensura, int fons_index)` **SHIPPED (additiones I, + silva_lexare_cruda)** | declaration-only | **Phase B entirely** (layer-0 token stream for coloring) |
| 2 | `const SilvaChorda* silva_fons_via(const SilvaExpansio*, int fons_index)` **SHIPPED (additiones II, + fontes_numerus)** | small accessor into opaque struct | **Phase C money shot** (every location the popup names; Enter-to-jump) |
| 3 | Strata retention: `SilvaXar* strata` on SilvaParsura **SHIPPED (additiones II — FREE: arenas already paid; always-on; strata[last] aliases lexemata)** | ~~ENGINE WORK~~ two lines | Phase C layer flips beyond 0↔final |
| 4 | Include enumeration: `SilvaInclusioVista` + numerus/vista **SHIPPED (additiones II)** | slim-view accessors | TOC includenda; unknown-header discovery loop |
| 5 | Region/arm enumeration: `SilvaRamusVista` + numerus/vista over the FLAT exp->rami; extents are BYTE offsets **SHIPPED (additiones II)** | slim-view accessors | dimmed omitted arms; TOC regiones |
| 6 | Macro-def enumeration: `SilvaMacroVista` + numerus/vista (journal semantics) **SHIPPED (additiones II)** | slim-view accessors | TOC macros |
| 7 | Helpers: `silva_token_radix` / `_profunditas` / `_est_fons`, `silva_nodus_liberi`, genus-name fns **SHIPPED (additiones I)** | declaration-only | convenience (DIY-able over transparent structs) |
| 8 | Arena telemetry: a public peak/usage accessor on SilvaPiscina **SHIPPED (additiones I: summa_usus + summa_apex_usus)** | declaration-only (piscina fns exist) | debug status metrics only |

**Design rule (cross-cutting)**: every new reader is a slim public
"vista" struct + index-addressed walk — never the internal struct
layout. silva.h is the cathedral-time contract; freezing internals
would be the over-commitment silva's CLAUDE.md warns against.

**Arm-extent caveat to pin during Silva additiones II**: SilvaRamus
corpus_initium/finis are TOKEN OFFSETS — pin which stream they
index (raw layer-0 vs expanded) before the dimmed-arm renderer is
designed.

Tessera candidates (nothing blocks; ergonomics only, decide at
Phase A close): `tessera_replere` (fill-rect — the origo popup
wants it immediately; v0 can loop cellulam_ponere) and
`tessera_scribere_praecisum` (column-budget clip write; v0
truncates app-side via utf8.h).

## 3. Architecture (revised)

    saltuarius/
      CLAUDE.md  phase-log.md  saltuarius.sh  compile_probationes.sh
      fontes/
        saltuarius_res.*        app state, mode enum, focus
        saltuarius_tabularium.* directory-listing MODEL — takes an
                                injected list of introitus records;
                                NEVER calls iter_directoria itself
        saltuarius_columnae.*   miller view-model + render
        saltuarius_liber.*      an opened file: text, line index,
                                parse handle, layer state, cursor,
                                scroll
        saltuarius_visum.*      source view render (gutter, text
                                pane, sidebar, status line)
        saltuarius_origo_tab.*  origo popup view-model + render
        saltuarius_quaestio.*   in-file '/' search state
        saltuarius_nexus.*      silva bridge (§5): contextus,
                                praebenda seeding, parse cache+LRU,
                                token↔display maps, color classes,
                                degradation classification
        saltuarius_claves.*     key/mouse dispatch per mode
      probationes/
        saltuarius_proba.{h,c}  golden helpers (§7)
        probatio_saltuarius_*.c
      instrumenta/principalia/
        saltuarius.c            thin main: posix pons, args,
                                compile_flags.txt, DISK EDGE (fills
                                tabularium lists via iter_directoria,
                                reads files via filum), event loop

- **Libraries used** (monorepo citizen): piscina, chorda,
  chorda_aedificator, xar, tabula_dispersa, **filum, via,
  iter_directoria, utf8** + the two amalgams. Includes:
  `-Iinclude -Isilva/amalgama -Itessera/amalgama` — fontes/ dirs
  of silva/tessera FORBIDDEN.
- **The disk edge lives in the thin main + nexus**, not in
  view-models: production fills tabularium from iter_directoria
  (DirectoriumIntroitus: titulus, genus FILUM/DIRECTORIUM,
  mensura); probationes fill it from literal fixture arrays. No
  filesystem vtable — injection IS the seam. One end-to-end
  integration probatio may use a real /tmp fixture tree (house
  precedent: probatio_iter_directoria builds and tears down /tmp
  trees).
- **Build**: saltuarius.sh modeled on silva/lustrum.sh —
  self-contained, project-local saltuarius/build/, mtime-cached
  lib objects, compiles both amalgams (amalgam compiles ~0.1 s;
  non-issue), execs the app. NOT the tools/ dispatcher pattern.

## 4. The nexus recipe (silva integration, grounded)

Startup (once):
1. Read radix/compile_flags.txt if present — one token per line;
   take substrings after `-I`; **resolve against the radix**, not
   cwd (rhubarb's paths are root-relative). CLI -I appends.
2. `silva_contextus_creare(piscina_contextus)` — long-lived,
   reused for every parse (lustrum-proven). SilvaContextus is
   TRANSPARENT: set caps by writing ctx->fines.* directly;
   generous defaults, 0 = unlimited.
3. `silva_contextus_latinam_addere(ctx)` — seeds latina
   definitions so expansion resolves si/per/redde (lustrum skips
   this; saltuarius must not).
4. **Pre-seed praebenda**: walk every -I dir (iter_directoria),
   `filum_legere_totum` each .h, `silva_contextus_praebere(ctx,
   nomen, textus, mensura)`. Silva resolves #include ONLY from
   praebenda — it never opens files. Rhubarb's ~105 headers ≈
   small; seed once at startup. (When ledger #4 lands, unknown
   includes — fons_ad == -1 — become visible and can be resolved
   + re-praebere'd on demand for foreign projects.)
5. **Color classes**: scan the public `silva_latina_textus[]`
   (verbatim latina.h embedded in the amalgam) for `#define`
   names → tabula_dispersa set for the latina color class. C89's
   32 keywords: hardcoded set. (Layer-0 tokens classify only
   English keywords by genus; Latin names lex as identifiers —
   coloring them is this app-side membership test, by design.)

Per file-open:
- Fresh piscina per root (lustrum model), `filum_legere_totum`,
  `silva_parsare_cum_contextu(piscina, ctx, titulus, textus,
  mensura, &GRAMMATICA_SCELETI, NIHIL, NIHIL, NIHIL)` — the
  sceleton grammatica is built from 4 public symbols exactly as
  lustrum.c:39–44 does.
- Degradation badge from public flags: parsura == NULL ||
  !successus → plain-text mode; est_intermissa, fines_tactae,
  regiones_omissae, numerus_errorum → status-line badge text.
  Non-C files skip silva entirely (extension gate).
- **Cache: LRU over N roots (default 8), per-root piscina
  destroyed on evict** — v2 REVISION of interview Q10, driven by
  the measured 84–242 MB/root. Cache-hit reopen = instant;
  evicted reopen = one parse hitch. N configurable in res.

Layer-0 text for the source view: `silva_scribere_fontem` gives
byte-exact text; tokens for coloring/hit-testing come from
silva_lexare (ledger #1) once public. Final-layer view walks
parsura->lexemata (public today). Intermediate layers arrive with
ledger #3.

## 5. Rendering truths (tessera-grounded)

- **App owns the viewport**: line→row mapping, vertical/horizontal
  scroll, gutter. Horizontal scroll = slice each line to
  [scroll, scroll+pane_width) RUNES (utf8.h walk) before drawing —
  tessera clips only at the grid edge, and a long line would
  otherwise spill across an interior pane boundary.
- **Popup recipe** (origo): draw base content; then popup border
  via tessera_quadrum_pingere; then FILL the interior explicitly
  (loop cellulam_ponere, space + bg stilus — quadrum draws border
  only, interiors are untouched; z-order is paint order); then
  rows via scribere_literis; selected row = INVERSUM.
- **Dimmed omitted arms**: OBSCURUM (SGR faint) is terminal-
  dependent; default to an explicit grey foreground (e.g.
  0x808080) with OBSCURUM as a config nicety. [Fran may veto.]
- **Focus/selection**: INVERSUM for the active selection; inactive
  pane's selection = explicit bg tint.
- **Status + search lines**: plain scribere_literis at rows
  altitudo-1 / altitudo-2 after clearing the row; the '/' edit
  buffer, caret, and echo are app state (tessera has no input
  widget).
- **Frame discipline**: repaint the whole logical screen every
  frame (praesentare's diff is O(cells) compares ≈ 10K at 200×50 —
  trivial; only changed cells emit). App-side chorda_aedificators
  (status text, TOC rows) follow tessera's own rule: pre-size +
  reset per frame, never finire. NEVER index opus->tergum directly
  (fixed 512 stride); draw only within tessera_latitudo()/
  altitudo() via tessera calls.

## 6. Input map v2 (event-exact)

- Letters (h j k l q o L n N G /): CLAVIS events, ev.runa ==
  codepoint. Shift+g arrives simply as 'G'.
- **gg**: two 'g' events — app-side pending-key state with no
  timeout (vim behavior).
- Arrows: ev.clavis SINISTRA/SURSUM/DEXTRA/DEORSUM. Enter:
  REDITUS. Tab: TABULA (Shift-Tab = TABULA + MAIUSCULA).
- ctrl-d/ctrl-u: runa 'd'/'u' + MODIFICATOR_IMPERIUM — works.
- **FORBIDDEN bindings** (classic-lossy aliasing): Ctrl+I (≡Tab),
  Ctrl+M / Ctrl+J (≡Enter), Ctrl+H (≡Backspace). Ctrl+letter
  always folds to lowercase+IMPERIUM; Shift never flags on plain
  letters.
- F-debug = TESSERA_CLAVIS_FUNCTIO + ev.numerus (F1–F12); pick F2
  for the fructus toggle (F1 often intercepted by terminal help).
- Mouse: MUS events, 0-based cell coords; ROTA_SURSUM/DEORSUM =
  wheel; PRESSUS + mus_pulsus = click. No motion/drag events
  (mouse mode ?1000+?1006), consistent with the drag deferral.
- Paste arrives as a plain rune burst (bracketed-paste markers are
  swallowed) — harmless for v0 (no text fields beyond '/').

## 7. Testing plan (grounded)

- **Golden shape**: purgare → app render → assert CELLS via
  tessera_cellulam_legere — no praesentare needed (reads the back
  buffer). Assert bytes only in rare present-path tests.
- **Helpers** (saltuarius_proba.{h,c}):
  - `proba_linea(piscina, opus, y, x0, mensura) → chorda` — pack a
    row's signa (LSB-first UTF-8 bytes; signum 0 → ' ') for
    CREDO_CHORDA_AEQUALIS_LITERIS; trim trailing blanks.
  - `proba_quaerere(opus, y, textus) → s32` — first column where a
    string appears in a row (locate, then assert styles there).
  - `PROBA_CELLA(opus,x,y)` — direct TesseraCellula for
    ornamenta/color asserts (INVERSUM on selection, grey on
    omitted arms).
  - Cursor: assert opus->cursor_x/y directly (transparent struct).
- **Scripted sessions**: tessera_pons_memoriae_initum REPLACES the
  script and resets the cursor; one buffer feeds many
  eventum_expectare calls; script dry → EVENTUM_NIHIL immediately
  (mora ignored — tests never block). Scripted resize =
  pons_memoriae_amplitudo + expectare + magnitudinem_renovare;
  scripted resume = pm->resumendum = VERUM; scripted mouse = feed
  raw SGR bytes ("\033[<0;5;3M").
- **Directory fixtures**: literal DirectoriumIntroitus arrays into
  tabularium (the injection seam) — no fake filesystem. One
  integration probatio with a real /tmp tree (house precedent).
- **Nexus tests**: parse a small fixture source through the real
  contextus recipe; assert degradation classification and color
  classes (latina set contains "si", excludes "main").

## 8. Performance + memory budget (measured)

- Parse-on-open: 44–150 ms measured on real roots — one transient
  "silva legit…" status message during parse; NO async/spinner
  machinery in v0. Cache-hit reopen instant.
- NO parse-on-preview (44–80 ms per hover = sluggish). Previews =
  plain text head via filum. Colored previews become possible
  (cheap, ~0.2 ms/KB) only after ledger #1; even then, decide by
  feel.
- Memory: 84–242 MB peak arena per root → **LRU cap N=8 in v0**
  (≈0.7–2 GB worst case; per-root piscinae make eviction one
  destroy). Ledger #8 (arena telemetry) would let the F2 line show
  it; until then the debug line shows tessera fructus only.
- Resize: AMPLITUDO event (poll-emitted, once per change) →
  tessera_magnitudinem_renovare + re-clamp scroll/cursor + full
  repaint. RESUMPTUM (Ctrl-Z return): the pons already re-entered
  raw mode — just force a full repaint. tessera_intermittere/
  resumere pair reserved for future shell-out features.

## 9. Phases v2 (silva additions interleaved)

- **Phase A — the walk** (NO silva; starts immediately): scaffold,
  columns + tabularium + type-jump, plain previews (filum), status
  line, resize/suspend, mouse; goldens via fixtures. Exit: daily-
  usable browser, suite green.
- **Silva additiones I** (in silva/, its own phase-log INTENTIO):
  ledger #1 silva_lexare (declaration-only) + #7 helpers + #8
  telemetry. Gates: amalgam regenerated, hand-written silva.h
  extended, hospes + suites + nm green. Small.
- **Phase B — the look**: source view, layer-0 coloring (lexare +
  color classes), gutter, viewport, degradation badge + plain-text
  fallback. Exit: colored rhubarb source + badge proven on non-C
  file, goldens.
- **Silva additiones II**: ledger #2 fons_via + #3 STRATA
  RETENTION (the engine-work item; own mini-INTENTIO, arm-extent
  pinning) + #4/#5/#6 vistas. Gates as above + roundtrip/suite.
- **Phase C — the magic**: layer flips (all strata) with
  token-identity cursor tracking (radix/profunditas over
  transparent origo), origo popup + Enter-jump (fons_via), region
  dimming (ramus vistas). Exit: THE MONEY SHOT on
  silva_parsare.c, goldens scripting the full chain.
- **Phase D — the polish**: navigable TOC (vistas #4/#5/#6), '/'
  search, fructus/arena debug line, LRU tuning. Exit: interview
  Q20 daily-driver bar.

## 10. v2 decisions (RESOLVED with Fran, 2026-07-03)

1. **LRU N=8 CONFIRMED** (revises interview Q10 keep-all): cache
   the 8 most-recent parsed roots, evict = piscina_destruere,
   N tunable in app state.
2. **Silva additiones as spec'd CONFIRMED**: two mini-packages in
   silva's own phase-log (additiones I before Phase B; additiones
   II incl. strata retention before Phase C), each with
   INTENTIO/RELATIO and full gate re-runs. The widen-first plan
   working as designed.
3. **Dimmed arms = explicit grey** (~0x707070 uniform foreground,
   overriding syntax color — the flat color reinforces that the
   arm is inert; taken arms keep full coloring). OBSCURUM rejected
   as terminal-dependent.
4. **tessera_replere at Phase A close**: Phase A uses app-side
   loops; the tessera 1.1 pass adds replere once real usage
   confirms the signature (eskil rule: the caller designs the
   API). Amalgam + gates re-run once.
