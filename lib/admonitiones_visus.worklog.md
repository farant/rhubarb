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

## 2026-10-15 - T4c jump to the session's terminal

- Chain: notification "<pane> <socket>" -> list-panes (pane's session)
  -> select-window + select-pane -> list-clients: a vicus terminal pane
  (ANY tab, left or any stack position) whose pty tty is a client of
  that session -> `vicus_latus_focare` (tab active, stack front, focus);
  none -> `vicus_acervo_aperire(v, "terminale", "tmux <socket|-> <sess>")`
  in the active tab (petition, next pulse). Fran 10-15: a session
  already open in another vicus tab must be reused (finite tabs) - the
  live test pins exactly that.
- tty: `ttyname(servus)` in pseudoterminale_posix before the parent
  closes the slave; copied into the arena (pons.tty, NIHIL for the
  memory bridge). Test compares with the child's own `tty` output.
- Terminal pane argument "tmux <socket> <session>" (session LAST: tmux
  allows spaces) -> `multiplexor_argumenta_adhaerendi`. The layout
  persists arguments, so these panes re-attach on vicus restart: most of
  T6 comes free.
- Deviation: terminal environment now REMOVES TMUX and TMUX_PANE - a
  vicus started from inside tmux passed them on, and `tmux attach`
  refuses to nest. Right for plain shells too (a vicus terminal is not
  inside tmux).
- Live test probatio_vicus_saltus: private server build/pmx/v.<pid>,
  windows addressed by NAME ("=alpha:prima") - the user's ~/.tmux.conf
  is loaded (base-index unknown). 15/15 stable, no socket left.
- Jump runs 3-4 tmux processes synchronously on click (~tens of ms).
- Not handled: grouped sessions (tmux new -t alpha) - different session
  name, so a new terminal opens.
- Fran's look 2026-10-15: all five steps work ("very cool") - jump opens attached terminal on the right window/pane, second jump returns to the same tab, restart re-attaches.

## 2026-10-16 - T6 re-attach on restore (incl. hand-typed `tmux attach`)

- `vicus_latus_argumentum_ponere(v, id, arg)`: changes a pane's SAVED
  argument without remounting (layout rewritten; identity genus+arg
  follows, so jumps find the pane by argument too).
- System "terminalia" (every 5 s, only after
  vicus_applicatio_admonitiones_aperire - tests never touch tmux):
  list-clients on every server the sessions report, then the default
  server LAST (so a jump terminal's full-socket argument is not
  rewritten to "-"); a terminal pane whose pty tty is a client gets
  "tmux <socket|-> <session>", a "tmux ..." pane with no client is
  cleared (you detached - don't re-attach on restore); non-tmux
  arguments are never touched.
- Restore runs `$SHELL -l -c '<tmux -u ... attach -t =s>; exec $SHELL
  -l'` (each word single-quoted): detach or missing session leaves a
  login shell - Fran's workflow (shell, then attach by hand). Applies
  to jump terminals too (Fran 10-16: consistent).
- Live test: TMUX_TMPDIR set into build/pmx/t.<pid>, so even "the
  default server" is private; it is started WITHOUT -S and asked for
  #{socket_path} (examen does not know getuid). 
- Gotcha: a tmux client exiting restores the tty with TCSAFLUSH -
  input typed while it shuts down is DISCARDED, even after the server
  has dropped the client. The test resends its probe line until the
  shell answers (a human just retypes). Passed under the runner but
  failed 3/3 run directly before that fix: timing, not logic.
- `ordinarius` is a latina.h macro (default) - bit again in a test
  identifier; renamed communis.
- Fran's look 2026-10-16: everything works (hand attach remembered, restore re-attaches, detach leaves a shell).

## 2026-10-16 - T4d Esc pops the center / sessions pane (Fran)

- `vicus_acervo_redire(v)`: drops the front pane of the active tab's
  stack (refused when it is the only one - decision VIII: never empty),
  previous pane comes to the front with focus, layout saved.
- Opt-in per genus: `VicusFacies.effugium_redit`. The pane's own
  praeceptum "Esc / Back" returns FALSE (documents the key, does not
  consume it), the key falls through to the vicus root handler, which
  pops when the focused right pane opted in. Only panes WITHOUT a
  process opt in (center, sessions): dropping a terminal would leak its
  shell - there is no unmount hook in VicusFacies.
- Test gap found by a plant: with no praeconium source the sessions
  pane's action returns before reading keys, so Esc fell through no
  matter what the pane did - the Esc section now opens the sources
  first (a pane that ignores ALL keys proves nothing about one key).
- Fran's look 2026-10-16: Esc pop works.

## 2026-10-16 - T4e read notifications hidden by default (Fran, option 1)

- Read notifications leave the list; `r` shows / hides them (field
  `lectae`, persistent, like the sessions pane's `c`). Caption "N unread,
  M read (r)" while hidden. `x` (and Enter) on a row moves the selection
  to the next visible row (else the previous) when read rows are hidden.
- j/k now walk the album rows (`vicina`), not raw indices - hidden rows
  must be skipped.
- A saved selection can point at a row that is now hidden (toggle `r`
  off after marking it read): `electa()` accepts it only if it is in the
  album, else falls back to the first visible row - plant P6 showed
  Enter would otherwise act on an invisible notification.
- PROCESS SLIP: the T4d commit's gates were running when I started this
  (silent log, no monitor event yet) and I edited files in that commit.
  Killed that commit (only this tree's commissio + fabrica; quinta's
  untouched), nothing staged; T4d and T4e land together after Fran's
  look. Memory commit-gates-freeze updated.

## 2026-10-16 - T6b stray tmux query replies in restored shells (Fran's screenshot)

- Symptom: after a restore, shell panes showed "[?62;22c[>1;0;0cP>|aemulator 0.1\" -
  our emulator's replies to tmux's attach-time queries (DA1, DA2,
  XTVERSION), arriving as typed text.
- tmux 3.4 source (tty.c TTY_QUERY_TIMEOUT 5; tty-keys.c): it waits 5 s
  for those replies, then sets TTY_ALL_REQUEST_FLAGS and forwards later
  replies to the pane as keys.
- Why late: timestamps + `sample` on Fran's launch - vicus_applicatio_
  aedificare took 9-13 s, ~89% in pictor replaying its picture's entire
  brush history pixel by pixel at mount time (parked for the pictor
  line, ledger ...GCKD0W2). Restored terminals spawn ~0.04 s into that.
- Fix: restored/jump terminals run `/bin/sh -c '<wait>; <tmux attach>;
  exec $SHELL -l'`, the wait asking the terminal DSR 5 (`ESC[5n`) and
  reading the 4-byte answer with echo off and a 5 s per-read timeout -
  attach only once vicus is actually pumping the pty. Independent of
  WHAT makes startup slow.
- Probing pitfalls hit on the way: (1) `capture-pane -t =beta` is NOT a
  pane target ("can't find pane", empty output) - `=beta:` is; my first
  red test passed vacuously because of it, now it asserts a non-empty
  capture. (2) python pty.fork gives a 0x0 window - tmux shrinks the
  session to nothing; set TIOCSWINSZ in the child. (3) a headless probe
  on a COPY of Fran's layout still attached to his real sessions - the
  saved arguments carry explicit sockets, TMUX_TMPDIR does not redirect
  them; strip tmux arguments from copies next time.
- Probe matrix (tmux 3.4, escape-time 10): whole reply prompt = clean,
  any reply after 5 s = garbage, reply split mid-sequence = garbage even
  when prompt. Our emulator flushes replies in one write - lateness was
  the cause.
