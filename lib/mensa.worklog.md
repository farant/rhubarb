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
