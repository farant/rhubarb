# Charta — the census tech-tree page (forum app)

2026-08-11. Design settled in conversation with Fran; this spec is the
record. Goal: make rhubarb.census TRANSPARENT as it grows — a nested
infinite-canvas "tech tree" of its nodes and edges, so collaboration on
the census stays effective as it scales. The canvas is an INDEX INTO the
record layers, never a new writable surface.

## 0. Rulings (Fran, 2026-08-11)

- **Name approved: Charta.**
- **Nesting is an escape hatch, not v1.** One flat canvas of the whole
  census first. The hatch stays openable through payload shape alone:
  nodes keyed by nomen (the `&nomen;` citation identity — never array
  index, never synthetic id) and a scaena envelope (§3). Nothing else
  is built for it now.
- **Read-only detail panel** on click. One-way projection: the census
  file is truth, the charta never edits (same doctrine as sententiae).
- **Deterministic layout** — positions become muscle memory; the map
  must not reshuffle as entities land. (Load-bearing for hot reload:
  a landing entity is ONE new node appearing, everything else still.)
- **Glossae on demand, no seeding pass.** v1 renders everything as-is
  (Latin truth, tags/attributes verbatim); kind glosses come free
  (§9). Entity glossae get authored later through a process where
  Fran requests translations — not mechanically up front.
- **Hot reload**: the graph refreshes as rhubarb.census changes on
  disk, so growth is watchable from within the Charta tab (§8).

## 1. Where it lives

A new nav tab **Charta** in the forum app (`apps/forum/assets/
index.html` — nav is a flat list of `<a>` elements). The forum already
has the vitrea + internuntius plumbing and serves to the phone via
vitrea servus, so the charta gets all of that for free.

Precedent to follow: `sententias_parsare` — forum.c already parses a
house document server-side and returns a JSON projection. Same shape
here.

## 2. Data: `censum_legere` (forum.c)

One new method in `_methodos_praebere`:

    (vacuum)internuntius_praebere(inx, "censum_legere",
        _censum_legere, f);

Handler signature matches the house seam
(`JsonValor* f(JsonValor* argumenta, Piscina*, vacuum*, chorda* culpa)`).

Arguments: `{ "signum": "<opaque, from the previous reply>" }` —
optional; absent means "give me everything."

What it does, per request (no cache of the parse — freshness beats
cleverness; the mtime check IS the cache):

1. **Stat short-circuit**: signum = max mtime over the three source
   files (rhubarb.census, natura/cocta/semina.census,
   natura/cocta/individua.canon), rendered as an opaque string. If it
   equals the caller's signum → reply `{ "mutatum": false }` and stop.
   One stat per file, no parse.
2. Parse `rhubarb.census` via stml (pactum parseris unius).
3. Parse `natura/cocta/semina.census` for external-key resolution:
   citations that resolve there become **external nodes** (rendered
   differently, §5).
4. Parse `natura/cocta/individua.canon` for the **kind-level
   glossae** (191 `<glossa lingua="en">` already exist there,
   projected from the genera) and attach each node's genus gloss.
   Reading the generated canon is deliberate: it is the exact
   contract the census is judged by, and it is one file.
5. Emit edges from BOTH relation forms: singular = attributes
   (`proprium=`, `ex-fonte=`, `tendit-ad=`, …), multiplex = child
   elements (`natum-ex`, `informat`, `adhibet`, `invenit`,
   `minuitur-per`, …). **Edges carry `familia` only** — the
   familia→causa mapping lives in the JS view beside the
   kind→stratum table (§5), not in the serializer. (Shipped
   deviation from the first draft: reading `causa=` from natura's
   family declarations would mean a third parse plus
   underscore/hyphen name normalization in C, for a static
   eleven-row table that changes at natura-doctrine pace. Same
   lever category as the stratum table.)
6. **Parse failure is a loud, non-fatal reply**: if the census does
   not parse (likely a mid-save catch), reply
   `{ "vitium": "<message>" }` — never an RPC error that could be
   confused with plumbing failure. The client keeps its last good
   graph (§8).

## 3. Payload envelope (the escape hatch's entire cost)

    {
      "scaena": "census",
      "signum": "…",                   /* echo for the next poll */
      "nodi": {
        "<nomen>": {                   /* citation verbatim: "&x;" */
          "genus":     "bibliotheca",
          "notae":     ["…", …],       /* nota children, in order */
          "glossa":    "…",            /* entity gloss, if any */
          "glossa_generis": "…",       /* kind gloss from canon */
          "externus":  false,          /* stub/augmented: minted
                                          outside the census */
          "attributa": { … }           /* non-citation attrs
                                          verbatim (incl. titulus,
                                          contentum, via, gradus) */
        }, …
      },
      "aristae": [
        { "a": "<nomen>", "ad": "<nomen>", "familia": "informat" }
      ],
      "glossae_generum": 68,           /* fumus double-entry */
      "praetermissa": 0,               /* skipped content COUNTED */
      "monita": ["…"]                  /* loud degradations */
    }

Stub nodes are minted for edge targets not declared in the census:
genus from semina when resolvable, else "genus" (dot-citation),
"locus" (#-citation), or "ignotum". A later nested view = a second
scaena requested by nomen. v1 never sends more than one.

## 4. Renderer (index.html, dependency-free)

- Canvas 2D, camera `{x, y, scala}`, world→screen transform.
- **WASD** velocity in a rAF loop, **drag-pan**, **wheel-zoom**
  (zoom toward cursor). Drag + pinch matter because telephonum has no
  keyboard. WASD listeners active only while the Charta tab is shown
  (must not fight text inputs on other pages).
- Offscreen culling from day one — trivial at 84 nodes, load-bearing
  if a scaena ever holds natura-scale graphs (463+).
- Node = rounded box: titulus + small genus tag; color by stratum
  (§5). Edge = line with arrowhead at `ad`; color by familia.
- Hit-testing in world space for hover (highlight incident edges) and
  click (detail panel).
- Gloss display: the detail panel shows the Latin truth plus any
  glosses that exist (kind gloss always, entity gloss when
  authored) — both visible, no mode. (Shipped amendment: the
  three-way Latine/Anglice/ambo toggle is deferred until entity
  glossae exist in volume — today there are zero, so the toggle
  would switch between identical views.)

## 5. Layout — the only real risk, iterate with eyes

Deterministic, computed ONCE per data load. Toggles (§6) affect edge
DRAWING only — never positions. Same input twice → byte-identical
positions.

1. **Layer assignment is semantic, not algorithmic** — a kind→stratum
   table in the view code (the census has no container elements;
   entities are typed by element name):

   | stratum (top→bottom) | kinds |
   |---|---|
   | fines | causa-finalis |
   | res | bibliotheca, instrumentum-lineae, daemon, applicatio, artefactum-aedificationis, systema-unixiforme, clavis-privata, acta |
   | lex | pactum, praeceptum, norma, peritia-communis |
   | cognitio | observatio, signum, manuale, dictionarium |
   | eventus | ambulatio |
   | custodia | uncus, periculum |
   | externi / personae | persona + semina-resolved nodes |

   Unknown kind → a labeled overflow band, never a silent drop (the
   no-silent-caps law). The table is a LEVER: strata can later be
   re-derived from the causal skeleton itself if the hand-table starts
   lying.
2. **Within-layer ordering**: barycenter passes (4 down + 4 up,
   fixed) over **all edge families with spine (causat / pendet-ex /
   dependet-ex) weighted double**. (Shipped refinement over the
   first draft's skeleton-only rule: the pacta/praecepta band's
   connective tissue is `informat` — skeleton-only would leave it
   alphabetical. Toggle-stability is preserved because the layout's
   edge set is FIXED regardless of what is drawn.) Alphabetical
   initial order; nodes without neighbors hold their slot; ties
   break by nomen — fully deterministic. Band labels draw
   screen-pinned at the left margin (world-space labels hid under
   nodes — first-screenshot lesson).
3. **Stability under growth**: a new entity inserts by its own
   neighbors and nomen; existing relative order is preserved by the
   stable sorts. Authored/pinned positions are a future option, not
   v1.

## 6. Default view + toggles

Default = the **causal skeleton** (finalis/formalis edges — the census
IS a tech tree under this lens). A legend panel lists every edge
familia present in the payload with a checkbox and its color; `causa=`
classes get filter shortcuts. Drawing all families at once is allowed
but is the user's explicit choice (hairball is opt-in).

## 7. Detail panel (read-only)

Click a node → side panel:

- titulus, genus (+ kind gloss), stratum
- prosa (nota/definitio), entity glossa if present
- attributes rendered verbatim (tags, dates, vias — as-is per ruling)
- edges grouped by familia, each row **clickable → camera pans to the
  other end** (this is the "index into the record" in miniature)
- Esc or background-click closes. No edit affordance of any kind.

## 8. Hot reload (watchable growth)

Polling with an mtime short-circuit — request/response only, fully
inside the existing internuntius seam (no push channel; vitrea's
audire/SSE gap stays untouched).

- While the Charta tab is visible: JS polls `censum_legere` every
  ~1500 ms, passing the last `signum`. `{mutatum: false}` costs three
  stats and a tiny reply. Polling pauses when the tab is hidden.
- On a changed payload: **re-run layout (deterministic → stable),
  then diff old vs new by nomen** (pure `charta_differre`, edge
  identity = `a|familia|ad`):
  - added node → green glow fading over ~3 s
  - changed node (notae/attributa/glossa, by JSON equality) →
    gold glow fading over ~3 s
  - added edge → green flash fading over ~3 s
  - removed node or edge → gone next frame, no ghost (shipped
    amendment: a removed edge cannot flash — it no longer exists
    to draw; removals are visible as absence, additions and
    changes as glow)
- **Mid-save resilience**: a `{vitium}` reply (census momentarily
  unparseable) keeps the last good graph on screen and shows a small
  badge — "census non legibilis (servatio media?)" — cleared on the
  next good poll. The canvas NEVER blanks on a bad parse.
- The camera never moves on reload — Fran is watching a place, not a
  slideshow.

## 9. Glossa admission (small, separate commit)

Entity-level glossae are REJECTED today: `individua.canon` has zero
`<liberum nomen="glossa"/>` admissions on individuum elements. Fix at
the source: the natura_canones emitter admits glossa as a liberum on
projected individuum elements → `./tools/natura_canones.sh` regen →
custos gate. The admission ships with v1 so authoring is UNBLOCKED,
but no glossae are seeded: they arrive later via a
Fran-requests-translations process (shape TBD — possibly a tabularium
processus one day). v1 ships on kind glosses alone.

## 10. Testing (gate at birth)

- **Layout is a pure function** (data in → positions out): test under
  the osascript fake-DOM harness (exemplar probatio_fori.js).
  Asserts: kind→stratum assignment; determinism (same input twice →
  identical positions); crossing count on a small fixture does not
  increase after barycenter; unknown kind lands in overflow band.
  The four ways JS tests went green while lying apply
  ([[debugging-lessons]]) — thenables synchronous, planted fault
  first.
- **Diff logic is pure too**: added/removed/changed classification
  gets the same harness treatment (fixture: two payloads, assert the
  delta sets).
- **C handler**: fumus step — node count equals census entity count,
  edge count sanity, a known edge present with its causa; signum
  round-trip (same signum → mutatum:false; touch file → full
  payload); planted parse fault → `{vitium}`, not an RPC error.
- **Real-store law**: run the serializer on the REAL rhubarb.census
  and eyeball counts before believing fixtures (fixtures share your
  assumptions).

## 11. Non-goals (v1)

- No editing, no comment-anchoring (a later arc may hang scholia off
  census entities the way they hang off sententiae points — explicitly
  deferred by ruling).
- No natura-level canvas, no scaena stack — envelope only.
- No camera persistence, no authored positions.
- No push channel / SSE — polling only.
- No mechanical glossa seeding.
- No external dependencies of any kind (house law).

## 12. Rough order of work

1. `censum_legere` serializer + signum short-circuit + fumus gate
   (≈ half day)
2. Camera + renderer + tab plumbing (≈ a day)
3. Layout + tests (1–2 days of iterate-with-eyes)
4. Hot reload polling + delta highlight (≈ half day; layout already
   deterministic by construction)
5. Detail panel + toggles + legend (≈ half day)
6. Glossa admission in natura_canones + regen (≈ an hour, separate
   commit)
