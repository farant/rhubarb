# mensa worklog

## 2026-08-04 — birth: the thinkboard battery (v1 slice)

Design sealed with Fran (ledger res 01KZ58K3NH): nested-desktop
paradigm ("thinkboards") — no scrolling ever (planum overflow:hidden
is LAW; cards paginate instead), %-positioned, fullscreen-first,
everything draggable, position persists across sessions. Name sealed:
**mensa**. First consumer: silex ui.

Architecture, both halves:
- **C half** (include/mensa.h + lib/mensa.c): generic internuntius
  handlers any vitrea app mounts with one call —
  `mensa_praebere(inx, piscina, via_voluminis)`. Events land in a
  VOLUMEN (genus `mensa:<genus>`, datum JSON with `id`); state =
  fold (flat per-id merge, `mensa:deletum` removes, later creatum
  resurrects — probatio pins all of it, plus tolerance of alien and
  corrupt acta). State file convention: `~/.rhubarb/<app>.volumen`
  — which turned out to ALREADY be the de facto house dir
  (forum.tessera lives there).
- **JS half** (lib/mensa_assets/fontes/*.js, concatenated by
  tools/glomerare.sh with provenance comments — deliberately NOT
  ESM: the osascript harness can't follow imports, and capsula MIME
  is a non-issue for classic scripts since mimen.c maps .js):
  `<mensa-thema>` (theme-as-component: injects ALL component CSS
  once, attributes → --mensa-* custom properties; NO shadow DOM by
  decree), `<mensa-scida>` (pointer-capture drag in % of planum),
  `<mensa-scidula>` extends scida (contenteditable + 600ms debounce
  + ×), `<mensa-planum>` (dblclick empty → new scidula),
  `persistentia.js` — the ONLY file touching internuntius;
  components emit bubbling 'mensa-actum' CustomEvents and work
  bridge-less.

Traps + notes:
- Rehydration must not re-emit: ponePositum/textumPonere are silent
  setters; only pointerup/input/dblclick emit. scidulamCreare never
  emits — the CALLER emits creatum (dblclick does, rehydration
  doesn't).
- volumen grew `volumen_acta_legere(vol, post_seq, piscina)` —
  exclusive-after semantics, deliberately the same read replication
  (G8) will want.
- WAL sidecars (.volumen-shm/-wal) appear next to the state file —
  a bare `cp` backup should happen with the app closed, or after a
  checkpoint. Fine for state; worth remembering for project volumina.
- Debt (named): no osascript probatio for the JS half yet — the C
  fold is pinned, the drag math is not. When mensa grows logic
  (pagination, thecae), wire lib/mensa_assets/probatio_mensae.js
  into compile_tests per the probatio_fori pattern.
- Next iterations by pull: thecae (folders → nested boards),
  pagination component, resize grips, z-order (bring-to-front on
  pointerdown + persisted ordo).

## 2026-08-04 — v2: nested desktops (thecae, orbis, secare/inserere)

The board renderer arrived: planum now renders the current board FROM
STATE (registry of nodes — declared cards captured at startup, created
ones born from state; navigation = re-render with a different filter).
C battery needed ZERO changes — "which board" is just a merged
property (`tabula`), a move is one `collocatum` event. The generic
fold earned its keep on its first real test.

New pieces:
- logica.js — pure, DOM-free by design: mensaFundere (JS mirror of
  the C fold — if the shapes drift, fix BOTH probationes), 
  mensaQuaeRedenda (board filter), mensaRetro (backlink derivation:
  nav stack wins, else the board where the naming theca lives, else
  radix), MensaArbiter (hold/drag/dblclick state machine).
  **probatio_mensae.js: 30 assertions, wired into compile_tests.sh**
  — the gesture-arbiter debt was NOT deferred this time.
- theca.js — folder IS-A scida; its id names the board inside it;
  dblclick enters; nominare() = inline rename at creation (rename
  later = named future). Retro theca is SYNTHESIZED (retro+fixa+
  petens attrs), never persisted — dropping a card on it moves the
  card up one level (petens is the drop target board; free feature
  from the drop rule).
- orbis.js — radial menu, two petals; planum owns open/close/Escape.
- scida.js — drag now starts after a 4pt threshold; a still click =
  selection; drop hit-test via elementFromPoint → closest theca.
- scidula.js — notes are CALM now: contentEditable only in edit
  mode (dblclick opens, blur/Escape closes, emit only if changed).
  Freed dragging from the contenteditable fight.
- ⌘X = intent (mark, dim), ⌘V = transaction (collocatum into
  current board); never pasting = never happened. Escape cascades:
  orbis → edit → cut mark → selection.
- tabula_activa persists via internum pseudo-element (genus_elementi
  'internum' — never rendered); relaunch drops you on the board you
  left, backlink still derivable with an empty nav stack.

Traps hit:
- A stray C-style label (`redde_nodum:`) leaked into JS — valid
  labeled-statement syntax, so nothing errored; caught by eye.
  Parse-check of the bundle (new Function, no execution) added to
  the smoke ritual — cheap syntax gate for DOM-extending code the
  JXA harness can't eval.
- Class-extends-HTMLElement cannot be eval'd in JXA at ALL
  (extends clause evaluates at declaration) — the customElements
  guard does not help. Hence logica.js is the deliberately DOM-free
  testable core; component behavior stays hand-tested until a
  HTMLElement shim day.

## 2026-08-04 — v2.1: the CSS-inheritance trap (Fran's hands, first catch)

**CSS does not follow JS class inheritance.** MensaScidula extends
MensaScida in JS, but `mensa-scidula` is a DIFFERENT TAG — the
`mensa-scida { position:absolute; ... }` selector never touched it.
Notes and folders had no positioning: they sat in document flow,
ignored creation coordinates, and drag wrote left/top inline styles
with zero visual effect. Exactly what Fran reported ("not sure if
notes/folders are draggable or created at the xy"). The base card
chrome is now a GROUPED selector (mensa-scida, mensa-scidula,
mensa-theca) — law for any future mensa element: add your tag to the
base group or you don't exist spatially. Also per Fran: notes now
share the standard card container (charta/margo/umbra — the sticky
palette is gone); selectability locked down (cards/thecae/orbis/
tituli user-select:none; ONLY .editans .corpus selects). Companion
fix: `datum.x || 10` falsy-zero in _nodumCreare (x=0 snapped to 10).
First red-team catch of the loop — the walkthrough found in minutes
what no suite could.

## 2026-08-04 — v2.2: drop actually drops (Fran's hands, second catch)

**elementFromPoint during a drag returns the DRAGGED CARD** — it
follows the cursor, so it's always topmost under the pointer. Drop
detection never saw the theca beneath. Cure = the classic
hide-test-restore (visibility hidden → elementFromPoint → restore,
synchronous so it never paints), extracted as scida._subTheca(ev).
Running the same test in movere gives drop-target feedback free:
theca under a drag lights up (.suscipiens — dashed accent outline +
glyph glow). Also per Fran: theca sheds the card chrome (background/
border/shadow none) — bare glyph + label, desktop-icon style; the
.electum/.suscipiens outlines still read fine on transparent.

## 2026-08-04 — v3: image cards (paste → massae, the G6 slice)

Paste an image from the clipboard → <mensa-imago> card on the board.
The storage was ALREADY THERE: volumen massae is a content-addressed
blob store, so pasting the same screenshot twice stores it once —
G6's "images as blobs in sqlite" fell out of G2's design. New:
- volumen grew BARE massa API (massam_condere → sigillum hex out,
  massam_promere by sigillum). Doctrine note in the header: a massa
  is CONTENT, not an event — truth about its USE lives in acta (the
  element's creatum carries the sigillum). Probatio: binary
  round-trip with embedded zero bytes + 0xFF (catches any text-API
  leakage), dedup same-sigillum.
- mensa.c: mensa_imago_condere {datum_b64}→{sigillum} /
  mensa_imago_promere {sigillum}→{datum_b64} — base64 over the
  bridge both ways (house base64.c). A massa:// custom scheme in
  vitrea would be the zero-copy v2 (browser caching too) — named
  future, touches platform code.
- LAW REFINED (DI): components never NAME internuntius — persistentia
  INJECTS a provider (planum.imaginesPraebere({condere, promere})).
  Bridge-less: paste inert, image frames empty. Testable with a fake
  provider.
- <mensa-imago> IS-A scida; aspect ratio preserved STRUCTURALLY:
  the card controls WIDTH only (img width:100%, height:auto) — the
  resize grip (.ansa, corner, hover-visible) adjusts latitudo alone
  and emits one 'magnitudo' event. img pointer-events:none +
  -webkit-user-drag:none so card drag wins over native image drag.
- planum: paste listener (first image item; FileReader → dataURL;
  condere then creatum with sigillum+mimen+latitudo; the just-pasted
  card fills from the LOCAL dataURL — no round trip); reddere
  applies latitudo generically and fills imago cards via provider
  (node._impleta guards refetch).
- mensa-imago joined ALL grouped selectors (the v2.1 law, applied
  at birth this time).
