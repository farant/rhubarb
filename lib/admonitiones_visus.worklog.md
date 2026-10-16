# admonitiones_visus worklog

## 2026-10-15 - T4a vicus side (colloquia-plan)

Center pane (genus "admonitiones") + log follower, wired into vicus via a
new generic hook (vicus_systema_addere / vicus_radicem_scribere). Built on
the nomenclator template minus menus, prompts and notes.

Deviations from the approved headers (reported to Fran):
- `electa` is the notification ID (textus), not an index: indices shift
  every time a newer notification arrives, so a numeric selection would
  silently move to a different row (and Enter would jump to the wrong
  pane). Test `stabilitas` pins this.
- `lectae_ad` lives on the ROOT (`admonitiones_lectae_ad`, persistent,
  owner `admonitiones.agere` declared in the root manifest, same pattern
  as `capsa` / `pagina.clavis`), not on the center's branch: the badge
  count must be right at startup even if no center pane is open.
- the follower takes the index as a parameter instead of creating it:
  vicus creates the shared index at build time (centers mount against it)
  but must NOT read the real ~/.rhubarb log there - a dozen vicus tests
  call vicus_applicatio_aedificare, and reading the live log in gates
  would make them depend on a changing file. So the follower starts only
  via `vicus_applicatio_admonitiones_aperire(app, praeconium_via(...))`,
  which apps/vicus/vicus.c and vicus_terminalis.c call.
- `nunc` porta on the montatio (ages "now/5m/3h/2d"); NIHIL = no ages.
- `admonitiones_lectas_notare_ad` added (X in the center; vicus applies
  the restored root watermark once at startup).

Non-obvious bits:
- follower start: positio = file size measured BEFORE reading the tail,
  so lines written in between are read by the next pulse (duplicates are
  dropped by ID). A half-written last line stays unread until its '\n'.
- qualification is checked with an empty title first: titles cost a
  256 KB transcript tail read, PreToolUse spam must not trigger that.
- titles cached per session only when found in the transcript; the
  directory-name fallback is NOT cached (ai-title appears after the
  first prompt, so an early permission prompt would freeze "rhubarb").
- the system runs every pulse without a throttle: praeconium_legere_ab is
  fopen + ftell + fclose unless the file grew.
- single `x` marks are in memory only (lost on restart); `X` persists via
  the root watermark. Good enough for T4a.
- the follower's long-lived arena grows slowly with every notification
  (evicted copies are not reclaimed) - a few hundred bytes each.

Test gotcha: probationes/fixa/colloquia/eventa.jsonl ends with
"linea mala" WITHOUT '\n'; appending to a copy glued the first appended
event onto it. Copies in tests append "\n" first (a real log is always
whole lines: one write() per event).

Fran approved after the look (2026-10-15: "that looks good to me!").

## 2026-10-15 - T4b toasts (admonitiones_volantes) + badge

- Generic vicus layer `vicus_stratum_addere(v, genus)`: one mount of a
  registered genus, composed LAST (drawn on top, hit first), in every
  tab, pulsed every pulse, full surface. Root of the layer is 0 x 0 at
  the origin: hit-testing (destinatio.c `invenire`) walks children last
  to first and only returns a container when the point is inside it, so
  clicks between toasts fall through to the panes.
- Host `destinare` redirects a click in a non-focused space to the root
  (that's how clicking a pane focuses it). The layer is exempt by its
  path: vicus alone assigns ids "stratum_<genus>", so `spatium_strati`
  checks the first id segment. Clicking a toast never moves focus.
- A fake genus in tests needs its own canon before
  insula_ramum_initiare ("liberum hic non licet" otherwise): real genera
  get it from organum_ramum_parare.
- Toast visibility is DERIVED (unread, not dismissed, age < 8 s NUNTIUS /
  30 s ATTENTIO, 3 newest) - no timers in state. `dimissae` is pruned on
  every dismissal to ids that could still fly.
- Badge in the tab line reads root `admonitiones_non_lectae`; click
  opens genus "admonitiones" (small accepted coupling; Fran 10-15).
- Card widened 40 -> 49 cells after the headless render: "...use Edit"
  was cut. Suppressing toasts while the center is visible: deferred.
- One UNREPRODUCED red run of probatio_vicus_admonitiones by hand right
  after the width change (output lost - the next run overwrote it); 40
  direct + 6 runner + 120 runs (half CREDO_ORDO=mixtus) all green, no
  leaked volumes. If it recurs in a gate, mensor names the section.
- vicus tests never click a toast BODY: the real porta would run tmux
  select-pane on the default server.
- Fran's look 2026-10-15: "worked!" - toasts triggered from a terminale INSIDE the vicus run (admonere.sh), badge, x, body click, center via badge.
