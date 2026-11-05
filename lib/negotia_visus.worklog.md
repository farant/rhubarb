# negotia_visus.c worklog

## 2026-11-04 — T3: project list and feature list (negotia plan)

Shape of `lib/admonitiones_visus.c` (dispositio in cells, one root
figure draws everything, praecepta table, album pages); routes as in
`lib/sancti_visus.c` (argument -> semita -> ramus, organum kept in the
figure context for `SemitaOrganum`).

- **One `Negotia*` per vicus, many panes.** `negotia_renovare` returns
  VERUM only to the first caller after a foreign write, so a pane that
  reported `mutatum` from it would leave every other open pane stale.
  Each pane keeps `versio_visa` and compares it with `negotia_versio`
  (the library's read cursor) after renovare. Plant "signum
  renovationis, non versio" proves the second-pane step goes red.
- **Hover** (`super`): handled on MOTUS / INTRAVIT / EXIIT of an item
  row; written only when it changes; EXIIT clears only if the field
  still names that row (kalendarium natalis_super rule: the dispatcher
  sends EXIIT to the old row AFTER the motion into the new one). Motion
  over a row is consumed, otherwise it bubbles to the root.
- **Single click opens** (mouse first). DUPLEX is ignored on purpose:
  the first click pushes a new pane in front, and the second click of
  a double click would land on that pane's row.
- **Esc**: index -> not consumed (vicus pops the pane). Project view ->
  back to the index only when it was opened in place (`origo index`,
  no host porta); opened by argument (the normal vicus path) -> not
  consumed, vicus pops. Fran 2026-10-22: Esc is not a universal back.
- Bare id or unique suffix as argument is resolved to the full id
  BEFORE `semita_ex_argumento` (the typed cognomen would otherwise
  store the suffix as the project id).
- **Look check** (render to PNG from a copy of the live volume, temp
  suite deleted): dim summary text on the selected row vanished into
  the selection colour; the selected row draws its second line in
  COLOR_TEXT, as admonitiones does.
- Host test gotcha: in `probatio_vicus_acervus.c` `v` belongs to
  `app_imum` (volume `vol_imum`) by section XV - writing the fixture
  project to `vol` made the pane see an empty volume.
- Fran's live vicus runs from the secunda tree (`bin/vicus -volumen
  ~/.rhubarb/paginae-proba.volumen`, cwd secunda, 11-04): the live
  check is a restart after `AEDIFICARE_SOLUM=1 ./apps/vicus/vicus.sh`.

## 2026-11-04 — T4: feature details with revisions

- **One line series, cut into pages.** Body wrapped by paginarium (one
  huge page = just the wrapped lines), then a blank line, "History"
  and one row per revision newest first, all as `LineaFacultatis`
  (genus: body / blank / heading / history). Pages are slices of that
  list sized `capacitas * LINEA / SPATIUM`; the drawer also stops at
  the component's bottom edge. History rows are never re-wrapped as
  prose (paginarium would join short lines in a prose paragraph).
- **Clip by characters, not bytes.** `praecidere` cut at N bytes: the
  "·" (2 bytes) shortened history rows and a UTF-8 title could end in
  half a character. `runas_praecidere` counts non-continuation bytes.
  Test "praecisio" (60 x "é") + plant.
- **Local time.** Act momenta are UTC with milliseconds;
  `fasti_ex_iso` refuses fractions, so the first 19 chars + 'Z' are
  parsed, `fasti_ad_unix`, then `localtime`. Without it an evening
  edit showed tomorrow's date. Fallback: the UTC text.
- **Look-check copies need the WAL.** `cp` of
  `paginae-proba.volumen` alone missed the latest acts (they sit in
  `-wal`); copy `-wal` and `-shm` too.
- No cognomen `negotia/facultas/{}` (a bare id is a project); the
  route is `negotia/facultas/<project>/<feature>`, validated (feature
  must exist and belong to that project) before semita_applicare.

## 2026-11-04 — T6: conversations in the ring, card, Resume

- **Order of preference** (Fran 11-04): a live tmux pane -> jump
  (`saltus`, same as a click in $sessions); otherwise the card, where
  Resume is one deliberate key/click (Cmd+L must stay cheap - stepping
  never starts a Claude process).
- The host opener tries `saltus` whenever the session record has a
  pane, NOT only when `panellus_vivit`: that flag is refreshed only
  while $sessions is visible. saltus asks tmux itself and returns
  FALSUM for a dead pane -> card.
- **The jump stays in the project's tab** (Fran's live check 11-04:
  the first version reused $sessions' saltus, which focuses a vicus
  terminal attached to that tmux session on ANY tab - it switched
  tabs). `saltus_modo(..., in_tabula)` skips that search and opens /
  fronts the "tmux <servitor> <sessio>" terminal in the active tab's
  stack; $sessions keeps the old behaviour. Two vicus terminals may
  then show one tmux session (tmux allows it; same window in both).
- **Resume runs inside tmux** (`colloquia_mandatum_resumendi`):
  `unset TMUX; exec tmux new-session -A -s 'claude-<8>' -c '<via>'
  '...claude --resume <id>...'` through a login shell (a Dock-launched
  vicus has a short PATH). `-A` attaches if that tmux session exists,
  so Resume twice never starts a second Claude; the resumed session is
  then jumpable like any other.
- **Directory**: the session log's cwd, else the one stored on the
  project link (`negotia involvere` records it - survives log
  rotation).
- **"May still be running outside tmux"**: known, not CLAUSUM, event
  within 15 min -> first r arms (`resumere` = armatum), second resumes.
- Card argument `colloquium/<sessio>` writes its fields directly (no
  semita); `colloquium` field is semita="status".
- Host test never opens a valid Resume terminal: it would start a real
  tmux session running claude on Fran's server. The quoting is tested
  purely in probatio_colloquia "mandatum resumendi"; the jump itself is
  the existing saltus, checked live.
