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
