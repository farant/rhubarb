# speculum — spec v1 (2026-07-20)

*Status: post-interview, pre-codebase-exploration. v2 will add
codebase-specific planning after agent exploration. Interview raw:
project-specs/speculum-interview.md.*

## I. Purpose

Speculum ("mirror") is the standard debug mode for vitrea apps: a summonable
in-app overlay in which the running program shows you **its own complete
source code** — every house file that was compiled into it, the worklogs of
those files, and full provenance of how the binary was made.

House binaries carry their own source, unconditionally. This is the first
step of the self-embedding-binaries direction (Oberon/Smalltalk lineage;
eventual horizon: self-edit + recompile). It is also, deliberately, the seed
of two other projects: the first house **web components** (<fons-arbor>,
<fons-visor>) and the first consumer of the **theming token contract**.

## II. Shape (one paragraph)

A build-time generator (`speculum_generare`) — invoked by the app's build
script with the exact source list being compiled — computes the true header
closure via `clang -MM`, gathers worklogs and provenance, and emits a capsula
as a generated `.c/.h` linked into the binary. A small C library
(`speculum.{h,c}`) registers one bridge method on the app's internuntius that
returns the whole payload as JSON. A front-end bundle
(`lib/speculum_assets/`) provides the overlay: Cmd+Shift+D summons a tabbed
panel (FONS: tree + reader + search; DE: provenance) drawn over the live app
page without disturbing its state.

## III. Decisions of record (from interview)

1. **Separate library.** Vitrea's API does not grow. App opts in with ~2
   lines: generator call in its build script + `speculum_praebere(...)`.
2. **Scope = rebuild set minus vendor.** App .c + all house lib/*.c on the
   link line + true include closure (clang -MM) + latina.h + the build
   script itself. Vendor bodies excluded but **listed** (via, size, hash) in
   the manifest's `exclusa` section.
3. **In-page overlay**, injected; app state preserved; works in both origin
   modes (capsula:// and URL/dev).
4. **Full provenance**: git commit + dirty flag, compiler version + complete
   flag list, build timestamp, exact compile command(s).
5. **CSS isolation by class-prefix discipline** (.speculum-* under one root,
   scoped reset). NO shadow DOM — this is the doctrine's first exercise.
6. **Summon = JS keydown listener**, default Cmd+Shift+D, registered by the
   speculum bootstrap. No native menu plumbing in v1.
7. **Data path = one dump.** Single bridge call at first summon returns
   manifest + provenance + all bodies (~1–2MB JSON), held client-side.
8. **Reader v1 = all four**: line numbers, file-name filter, full-text
   search (client-side, file:line jump links), basic regex highlighting
   (comments/strings/C keywords/latina.h macros).
9. **Name: speculum.** speculum.h, speculum_* prefix, .speculum-* CSS.
10. **Assets live in lib/speculum_assets/** — components graduate to a
    shared home only when a second consumer pulls them (mimen pattern).
11. **Front-end language: full Latin, snake_case** (matches C side; custom
    elements <fons-arbor>, <fons-visor>).
12. **Manifest derived by the build** (generator consumes the build script's
    own source list; clang -MM closure). Hand-maintained lists forbidden.
13. **Theme seed: global tokens with fallbacks** — ~8 flat roles consumed as
    `var(--thema-*, <default>)`. No machinery; contract born working.
14. **Tabbed overlay from day one, shipping FONS + DE.** Future panels
    (fructus, bridge log, piscina) join without refactor.
15. **Lore: worklogs ride along.** lib/x.worklog.md embeds for every
    embedded lib/x.c; shown as `documenta`, visually distinct.
16. **Unconditional embedding.** No strip flag, ever, including releases.
17. **Quine handling: list, don't embed.** The generated capsula .c appears
    in `exclusa` marked "generatum — continet hos ipsos fontes"; the
    generator's inputs (file list) and speculum_generare itself embed
    instead.
18. **Strictly read-only.** Copy-to-clipboard allowed; no editor bridges.
19. **First wearer: probatio_vitrea_hospes** (richest tree, proves overlay
    over a live composed app).

## IV. Components

### A. speculum_generare (build-time tool)

Input: the app name + the exact list of .c files the build script compiles
(passed from the script's own variable, not re-derived) + the build script
path + the flag list.

Work:
1. `clang -MM` over the .c list → true header closure (house headers only;
   vendor and system headers routed to `exclusa` / dropped respectively).
2. Collect lib/x.worklog.md for every embedded lib/x.c that has one.
3. Provenance: `git rev-parse HEAD` + dirty check; `cc --version`; flags;
   timestamp; the literal compile command.
4. Emit capsula (via the existing capsula generation mechanism) as e.g.
   `speculum_fontium.{h,c}` in the app's asset/build area, containing:
   fontes (bodies), documenta (worklogs), the manifest JSON (or data from
   which the C side builds it), and speculum's own viewer assets
   (speculum.js/css) so the overlay works in URL-origin mode too.
5. Quine entry: itself listed in exclusa with size + hash.

Open for v2: exact integration points in compile_tools.sh /
compile_tests.sh; whether manifest is a JSON blob in the capsula or built
C-side; where generated files land and .gitignore policy.

### B. speculum.{h,c} (runtime C library)

API sketch (subject to v2):

    /* registrat methodum pontis "speculum_obtinere" in internuntio;
       capsula fontium + piscina pro responsis */
    vacuum speculum_praebere(Internuntius* inx,
                             constans SpeculumCapsula* fontes,
                             Piscina* piscina);

One bridge method: `speculum_obtinere` → JSON:

    {
      "proventus": { "commissum": "...", "sordidum": false,
                     "compilator": "...", "vexilla": "...",
                     "tempus": "...", "mandatum": "..." },
      "fontes":    [ { "via": "lib/chorda.c", "magnitudo": N,
                       "corpus": "..." }, ... ],
      "documenta": [ { "via": "lib/chorda.worklog.md", "corpus": "..." } ],
      "exclusa":   [ { "via": "vendor/sqlite3.c", "magnitudo": N,
                       "digestum": "...", "causa": "vendor" },
                     { "via": "speculum_fontium.c", "causa":
                       "generatum — continet hos ipsos fontes" } ]
    }

Open for v2: whether the house json lib can serialize (vs parse only);
bridge payload size limits; UTF-8/escaping of source bytes through
internuntius; chorda handling of megabyte bodies; hash choice for digestum
(pre-sha256-lib, may be a simple checksum in v1 with an upgrade note).

### C. lib/speculum_assets/ (front-end)

- `speculum.js` — bootstrap: registers the keydown listener; on first
  summon calls `internuntius.vocare('speculum_obtinere')`, builds the
  overlay, defines the components. Full Latin, snake_case.
- `speculum.css` — .speculum-* rules; consumes global theme tokens with
  fallbacks.
- Components (light DOM, no shadow):
  - `<fons-arbor>` — file tree grouped by directory (lib/, include/, app,
    scripta, documenta), type-to-filter.
  - `<fons-visor>` — reader: line numbers, highlighting, scroll-to-line.
  - Panel chrome: tabs (FONS | DE), close, search field.
- Delivery into the page: bootstrap must load in BOTH origin modes. Exact
  mechanism (script tag from capsula, user-script injection at vitrea
  level, or bridge-delivered eval) is the **top v2 exploration question** —
  constraint: vitrea stays speculum-unaware, so any vitrea change must be a
  generic capability, not a speculum hook.

### D. Theme token seed (the contract's first eight)

    --thema-fundus         (panel background;   fallback #26201a)
    --thema-fundus-altus   (raised surfaces;    fallback #1a1512)
    --thema-littera        (primary text;       fallback #efe2dc)
    --thema-littera-hebes  (secondary text)
    --thema-accentus       (accent/active;      fallback #d4a48f)
    --thema-margo          (borders;            fallback #54402c)
    --thema-electio        (selection/match highlight)
    --thema-monospatium    (code font stack)

(Defaults drawn from the existing hospes page palette so the family
resembles itself. Names final in v2.)

## V. UI

FONS tab: left = <fons-arbor> (filter box above), right = <fons-visor>.
Search field spans the top; results list shows via:linea + context line;
selecting jumps the visor. DE tab: provenance stamps as a definition list +
exclusa inventory table. Overlay covers most of the window with the app
visible at the edges (it is an overlay, not a takeover); Esc or Cmd+Shift+D
dismisses; all app state persists.

## VI. Non-goals (v1)

No editing or self-recompile affordances beyond copy. No silva-grade
highlighting. No shadow DOM. No strip flag. No hospitium dependency (works
in serverless capsula apps). No menu-bar integration. No multi-window.

## VII. Milestones (sketch — v2 will firm)

- **Phase 0**: speculum_generare + build integration; capsula on disk
  verified; provenance correct (dirty flag proven both ways).
- **Phase A**: speculum.{h,c}; bridge method returns full payload;
  pump-test with a fake missor (no GUI) asserting JSON shape.
- **Phase B**: overlay assets; components; reader features; smoke via the
  GUI-smoke pattern (stdout → file, sleep, read, kill).
- **Phase C**: first wearer probatio_vitrea_hospes, both postures; the
  self-announcing smoke: page summons speculum via injected keydown,
  reads a known line of its own source through the bridge, echoes it.

## VIII. Open questions for v2 exploration

1. Bootstrap delivery into the page in both origin modes (the vitrea
   user-script question; generic capability only).
2. json lib: serialization capability + performance at ~2MB.
3. capsula API: enumeration of entries; binary-vs-text handling; does it
   compress (if not, is ~1–2MB acceptable — likely yes).
4. internuntius: max message size, escaping path for source bytes.
5. compile_tools.sh / compile_tests.sh integration shape; where generated
   speculum_fontium.{h,c} live; gitignore + examen/censor treatment of
   generated files (exclusiones? vendor-style pruning?).
6. Digestum choice pre-sha256 (adler/crc in-house? defer hashes to the
   sha256 library and ship "magnitudo only" in v1?).
7. Keydown capture inside WKWebView: does Cmd+Shift+D reach page JS in
   vitrea as configured (type-15 swallow etc. — check fenestra riders)?
