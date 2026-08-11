# forum.c worklog

## 2026-08-11 — charta step 1: censum_legere (census graph serializer)

Spec: project-specs/charta-spec.md. New CHARTA section in forum.c:
`censum_legere {signum?}` → graph payload / `{mutatum:false}` /
`{vitium}`. Registered in the public method list (phone gets it free).
First real fumus: **92 nodi, 153 aristae, 68 glossae generum,
0 praetermissa** — 92 = 84 census entities + the `<% &c;>`
augmentation + 7 external stubs, which cross-checks.

Decisions and traps worth remembering:

- **signum = `mtime.size.inode` per file, three files joined.**
  Inode catches atomic saves landing within the same mtime second
  (write-temp-then-rename mints a new inode) — mtime alone would
  miss them.
- **individua.canon (846KB) parses into its own throwaway piscina**
  (8MB, created/destroyed inside `_charta_glossas_apponere`), never
  into the 8MB piscina_vocationis — the known generated-file OOM
  bite (01KYQ4H5ZG). Gloss chordae are `chorda_transcribere`d into
  the request piscina *before* the canon piscina dies; forgetting
  that copy would be the classic dangling-chorda corrupted-response
  bug already documented at the `_sententias_parsare` sigillum
  comment.
- **`nomen` and `externus` are latina.h macros** — the struct
  fields/params are `clavis` and `alienus`; only the JSON string
  keys say "nomen"/"externus".
- **Augmentation blocks**: stml parses `<% &c;>` as an element
  titled `%` with `augmentum_clavis` carrying the citation
  verbatim — the node keys off that, marked externus.
- **ChartaNodus pointers don't survive `xar_addere` on the same
  xar** (growth relocates) — the code never holds one across a
  node creation; comment on `_charta_nodum_creare` says so.
- **Planted faults, both RED**: truncated census → `{vitium}` →
  `fumus charta FRACTUS: vitium` exit 1; empty-but-valid census →
  zero nodes → FRACTUS exit 1. Proven by running `bin/forum -fumus`
  from a scratch cwd containing the broken `rhubarb.census` (the
  charta paths are cwd-relative; the daemon connection is TCP so
  the rest of fumus doesn't care about cwd).
- **Edges carry `familia` only** — the familia→causa map is a
  static ~11-row table that belongs in the JS view next to the
  kind→stratum table (spec §2.5 records the reasoning). Avoided a
  third parse + underscore/hyphen normalization in C.
- The zsh `===` separator trap bit again during the planted-fault
  run (equals-expansion ate the second test) — quoted separators,
  always.

## 2026-08-11 — charta step 2: the Charta tab (camera + renderer)

New nav tab + `visus-charta` section + `#charta-tela` canvas, and a
`/* ---- charta ---- */` JS section in index.html: camera
{x, y, scala}, WASD velocity in a rAF loop, drag-pan, wheel-zoom
toward cursor, single-finger touch pan + two-finger pinch, hover
highlight, click-select (detail panel = step 5), offscreen culling,
first-load fit-to-graph centering. Provisional deterministic layout
(stratum bands, alphabetical, rows of 8, centered) — step 3 replaces
within-band ordering with barycenter over the causal skeleton.
`CHARTA_STRATA` (kind→band) and `CHARTA_SPINA` (skeleton families)
are the view-side tables the spec assigns here.

Notes:

- **The rAF loop self-terminates when `visus !== 'charta'`** and
  restarts on entry — no vertere() exit hook needed. Repaints every
  frame while the tab is visible; 92 nodes is nothing, revisit only
  if a scaena ever holds thousands.
- **Harness discipline**: all canvas/rAF calls live inside functions
  never invoked at load; `charta_pingere` additionally guards
  `tela.getContext` absence. The fake DOM's auto-creating
  `getElementById` covers the top-level nav wiring.
- **Global `document.onkeydown` is charta's** — nothing else in the
  file used it (grepped before taking it); handler exits unless the
  charta tab is visible and no input/textarea has focus.
- `body.latus` (90em) for the charta page, same as codex/lector.
- `.charta` the CSS class = "card", pre-existing and unrelated —
  the page's ids are `#charta-*`; a comment at the CSS block warns.
- **Harness planted-fault mechanics**: probatio_fori.js reads
  `apps/forum/assets/index.html` relative to CWD, not to the script
  — a red-test copy must run from a dir with that subtree in place.
  Proven red: inverted "fines supra res" → FRACTA (1), exit 1.
  Green: 233 assertions (225 + 8 charta layout/sigil).

## 2026-08-11 — charta step 3: barycenter + band labels

Fran's first screenshot (layout.jpg) was the input: bands read
well, but alphabetical order scattered every parent's children
across its band (dominium's ten causat targets = a fan of crossing
lines), and the band labels were invisible (world-space drawing
put them under nodes / off-viewport at fit zoom).

- **Barycenter over ALL edge families, spine weighted ×2** —
  refinement over the spec's skeleton-only rule, recorded in spec
  §5.2: the pacta band's connective tissue is `informat`;
  skeleton-only would leave it alphabetical. Toggle-stability
  holds because the layout's edge set is fixed regardless of
  drawing toggles. 4 down + 4 up passes, alphabetical seed,
  neighbor-less nodes hold their slot, nomen tiebreak — fully
  deterministic (asserted twice-identical WITH edges).
- **Inner-function scoping bug caught before it shipped**:
  `positus_scribere`'s `for (f = …)` reused the CALLER's `f`, so
  the pass loop would have ended after one band — silently, as a
  layout that "worked" but never iterated. Inner helpers now own
  their locals; comment in code warns.
- **Band labels screen-pinned** (transform reset, left margin,
  projected band y, clipped to viewport).
- Harness: 235 assertions; crossing-reduction fixture
  (alpha→secundus / beta→primus uncrossed after passes) proven
  red-capable via inverted copy.

## 2026-08-11 — charta step 4: hot reload

`charta_interrogare` polls `censum_legere` with the last signum
every 1500ms while the tab is visible (self-clearing interval on
tab exit, same pattern as the rAF loop; `document.hidden` skips).
`charta_recipere` is the testable core: `{vitium}` keeps the last
good graph and shows the badge, a good reply clears it,
`{mutatum:false}` touches nothing, a full payload swaps
datum+signum, re-runs layout, diffs, and NEVER moves the camera
(asserted). `charta_differre` is pure: node identity by nomen,
change by JSON-stringify equality (safe because the C serializer
emits attributes in file order — determinism end to end), edge
identity `a|familia|ad`.

- Spec amendment recorded (§8): removed edges don't flash —
  they no longer exist to draw. Removals are visible as absence;
  additions glow green, changes gold, 3s fade (free animation:
  the rAF loop repaints every frame anyway).
- Glow state lives OUTSIDE the payload (`charta_fulgores`,
  keyed by nomen) so a reload can't clear another reload's glow
  mid-fade; entries self-delete at age > 3s during paint.
- Harness: 245 assertions (+10: diff classification ×3, receive
  flow ×7). Planted fault: wrong camera expectation → FRACTA (1).
- End-to-end (edit census while the tab is open, watch the glow)
  is eyes-territory — the signum-change path in C is logically
  covered (equality proven by fumus round-trip; inequality falls
  through to the default full-parse path, also proven).

## 2026-08-11 — charta step 5: tabella + legenda + cribrum

Detail panel (read-only, spec §7): titulus, genus + stratum +
externum badge, kind gloss (green italic), notae, entity gloss,
attributes verbatim, edges grouped by familia with direction
arrows — each edge row clicks through: selects the other end and
GLIDES the camera there (charta_iter lerp in the rAF loop; WASD,
drag, and Esc all cancel it; reload never sets it, so
camera-never-moves-on-reload still holds). Esc or background click
closes. Reload refreshes an open panel; a vanished node closes it.

Legend (spec §6): families present in the payload, grouped under
CHARTA_CAUSAE_ORDO headers (finalis/formalis/materialis/
efficiens/incerta); clicking a causa header toggles its whole
group. Default = the skeleton: spine + finalis + formalis causae
(charta_familia_ordinaria) — adhibet/custodit/invenit etc. are
opt-in. User checkbox state survives reloads (only unseen
families get defaults). Colors deterministic: spine gold, informat
green, the rest from a fixed palette in sorted-name order.

- **Filter bypass rules** (charta_arista_visibilis, tested):
  a selected node's incident edges always draw (white), and a
  glowing new edge draws even in a hidden family — then fades
  away, which reads as "something landed here" and is exactly
  right.
- **Confession**: step 4's aristam_pingere rewrite dropped the
  ux/uy unit-vector computation — arrowheads NaN'd silently from
  e71e13b until now (visual-only; no gate renders). Restored in
  this step's rewrite. Lesson: a replace-edit that reflows a
  function must re-check every downstream use of locals the
  old text defined.
- Spec §4 amended: Latine/Anglice/ambo toggle deferred until
  entity glossae exist in volume (today: zero — the toggle would
  switch between identical views). Panel shows both where present.
- Harness: 254 assertions (+9). Planted fault (inverted
  default-visibility) → FRACTA (1). Full smoke green.

## 2026-08-11 — charta step 6: entity glossae unblocked

Canon side in natura.worklog.md (emitter grants universal glossa
liberum + definition; red-green proven; chain regenerated). Forum
side: the fumus gained a NON-GATING probe line — `glossa entis
piscinae ADEST/abest` — because the serializer's entity-glossa
branch had never run against a real glossa (none existed until
&piscina; got the first one today). Probe, not gate: glossae come
and go at authoring pace, and a gate would break the fumus the day
Fran rewords the census. All six spec steps now shipped.
