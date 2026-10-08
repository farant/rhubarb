# vicus latera — plan (two panes per tab, terminale as a kind, acme commands)

*Written 2026-10-08 from Fran's answers, after the aemulator phase D
closed and while main is busy (secunda). Builds on the insula-rami arc
(`project-specs/insula-rami-plan.md`, vicus T1-T5) and the aemulator
arc (`project-specs/aemulator-plan.md`, terminale). The goal: one
window where a text editor and a terminal (or a second editor, or
pictor) sit side by side, driven by commands written in the text -
the legacy concha's design (`lib/schirmata.c`: ten screens, two
panels; `registrum_commandi`: `$date`, `$cal`… clicked in a page)
rebuilt on the insula-native apps.*

## I. Decisions (Fran, 2026-10-08)

1. **A tab is a fixed pair of panes.** LEFT = the text editor (scriba),
   stable - it is where you write and where commands live. RIGHT = the
   target pane. (Not a tmux-style tree of splits.) The left pane's kind
   is a DEFAULT, not hard-coded: another widget may live there later
   (Fran, 2026-10-08: "so we don't paint ourselves in a corner" - the
   design waits until we explore it), so the pane model holds a kind
   per pane, never "scriba" by construction.
2. **Commands are text in the page, acme style**, written
   **`$verb(args)`**: the dollar word is the click target and arguments
   sit in parentheses, so a command's extent is unambiguous inside
   prose (`$terminale`, `$scriba(notes)`, `$pictor(sketch)`).
3. **A plain click runs a command** (as the legacy concha did - not
   Cmd-click, not middle-click). A click anywhere that is not a command
   places the cursor as usual.
4. **Commands that open a widget always target the current tab's RIGHT
   pane.** Other kinds of commands exist too (they act without opening
   anything).
5. **The right pane keeps a stack, per tab.** Opening a widget puts it
   in front; the previous ones stay mounted behind it (a terminal keeps
   its shell). Opening something ALREADY in the tab's stack brings that
   one forward instead of creating a second (identity = verb + args:
   `$terminale` twice = one terminal; `$scriba(a)` and `$scriba(b)` =
   two). No back command or prefix key for the stack yet.
6. **Terminals keep running in the background.** Decision 7 of
   insula-rami ("background tabs are frozen") gains an exception: a kind
   may declare that it lives in the background - its output is read and
   its state updated, only drawing is skipped (a frozen terminal would
   stall its program once the pty buffer fills). Editors and pictor stay
   frozen.
7. **The layout is durable**: the tab list already is; each tab's
   right-pane stack is too. On restart every tab reopens its editor
   document and its stack; a terminal reopens with a FRESH shell
   (aemulator decision: fresh shell on restart).

### S2 decisions (Fran, 2026-10-08, after S1)

8. **Always two panes**, split in half (amends decision 5's "empty
   stack"): a tab never shows one pane - an app would be clipped
   when the second appears. A right pane with nothing opened shows a
   scriba. Full-screen-one-pane waits for real pane-management UI.
9. **Exactly ten tabs, fixed slots** (no add, no remove): tab 1 =
   scriba | terminale, tab 2 = scriba | pictor, tabs 3-10 = scriba |
   scriba. Changes to panes are durable and restored on restart.
10. **scriba views share pages**: the text lives in PAGES (shared);
    a scriba view holds its own current page and cursor. In a fresh
    volume every view shows the same page. Pages are NAMED and picked
    later by command (S3, `$scriba(name)`); the legacy app's page
    management is the starting point when we get there.
11. **Focus by click only** for now. (Shift-Tab rejected: programs in
    the terminal use it; Ctrl-A is Fran's tmux leader.)
12. **Layout target = the maximized window** (Fran works maximized /
    full screen): the window opens filling the screen's usable area.
    On this machine 1920 x 1200 points at SCALA II = 960 x 600 of our
    pixels: a half pane ~480 x 592 px = 80 x 74 cells.
13. **New documents are sized to their pane** (pictor canvas, scriba
    page) at creation; existing documents keep their size.

## II. What exists (read 2026-10-08)

- **vicus** (`include/vicus.h`, `lib/vicus.c`, `lib/vicus_applicatio.c`,
  `apps/vicus/`): one store (`InsulaRepositorium`), one volume, a tab
  bar row, kinds registered by `main` (`vicus_genus_addere`: title,
  mount size, `VicusMontator`, `VicusDescriptor` → `VicusFacies` with
  actions, figurae, componere, image source, gesture). One tab = one
  mount = the whole area. Ctrl-A prefix + tab clicks
  (`vicus_dispensatorem_ligare`). The host writes each app's surface
  size into its branch (`superficies_*`) - the seam a pane rectangle
  needs.
- **scriba** (`lib/scriba_*`, `apps/scriba/`): vim page on an act log;
  no `:` command line, no clickable commands.
- **terminale** (`include/terminale.h`, `lib/terminale.c`): a
  `TerminaleApplicatio` (dispatcher, figurae, host, cell size) with its
  own pulse; not a vicus kind yet.
- **Legacy** (`lib/schirmata.c` ~:200, `lib/registrum_commandi.c`): a
  click on a page region of genus "command" ran the registered
  function with the page and position as context.
- **Actions**: every app and the host register named actions in an
  `ActioRegistrum` - the natural target for commands (one verb, many
  callers: prefix keys, clicked text, a palette later).

## III. Slices (proposed - order Fran's)

**S1 - terminale as a vicus kind.** Register `terminale`; mount =
`TerminaleApplicatio` in the sedes; the branch holds title (+ later
cwd/scroll) only (aemulator decision 3); the BACKGROUND PUMP (decision
6) as a kind flag the host honours; works as a whole-tab app at first.

S1 split (Fran approved headers 2026-10-08): S1a terminale on a branch,
S1b `terminale_montare` + public `terminale_componere` (environment list
moves into the library), S1c vicus pulses living kinds
(`VicusPulsus`/`VicusPulsator`, `VicusFacies.pulsare/_ctx/
vivit_in_fundo`, `vicus_pulsare`, pulse hooks in both shared loops).
No new key: the DEFAULT tab list gains a terminal tab `t1` (existing
volumes keep theirs). Shell exit: the tab stays with its last screen
and "[exited]" in its title until S2 brings a close command.

S1a as built: `TerminaleApplicatio.ramus`; both surface reads through
it; standalone = root branch; test XIII (another store's `<terminale
id="t1">`); three plants.

S1b as built: shared builder (standalone vs host repo); embedded canons
(vicus's root canon declares no children - a mount needs its own);
environment list in the library; test XIV on vicus's real canons with a
real shell; five plants.

S1c-1 as built: `LudusPulsator` + `ludus_fenestra_pulsum_ponere` /
`ludus_fenestra_pingendum` (+ the twin's), `versio_picta`: a living
app is pulsed every <= 16 ms and the window repaints only on events,
pulse change or a store-version change since the last frame (the
dispatcher recomposes on every event, so its counter cannot signal).
Eight plants.

S1c-2 as built: `vicus_pulsare` (active always, background only for
`vivit_in_fundo` kinds, finished never; a background change does not
ask for a frame, a tab FINISHING does - the bar changes); `finita` is
host memory only - the bar draws `title [exitus]` and the hit zone
widens with it (one width function for both), the durable index never
sees it, so a restart reopens the tab with a fresh shell. terminale
registered in `vicus_applicatio` (mount ignores volume and path root -
canons embedded; pulse with no wait, the loop's 16 ms is the wait);
default index gains `t1` (existing volumes keep theirs);
`terminale_ambitus()` replaces the two apps' copies; both vicus mains
install the pulse; the vicus twin turns `ornamenta_pixelorum` off on
its terminals as terminale's twin does. Window scale (Fran
2026-10-08): vicus, pictor, scriba open at SCALA II like terminale
(buffer stays logical height; one of our pixels = two window points).
Test `probatio_vicus_pulsus` (toy kinds, then the real terminale with
/bin/sh: echo read in the background, `exit` finishes the tab).
Eleven plants.

S2a-1 as built (scope in core, headers approved 2026-10-08):
`Componens.spatium` marks a scope root (inherited down the tree);
action and figura registries carry a scope per entry (`""` = host;
lookups STRICT; plain register/find/miscere = scope "" or the entry's
own scope); `pingere` carries the scope down; the dispatcher resolves
a component's action in the COMPONENT's scope, focus / capture / Tab
cycling inside `Motus.spatium`, and deferred derived events in the
scope they were addressed in (internal `Differendum.spatium` - the
host root and a pane root can share an id like 'radix'). Test
`probatio_spatium` (one toy app twice, same ids/actions/figures);
sixteen plants. Found on the way: the dispatcher CANNOT clear focus -
`dispensator_focus_ponere(d, "")` interns "" (refused, NIHIL) and the
old value stays (pre-existing; see lib/dispensator.worklog.md).
Pending Fran: `Dispensator.super_spatium` (hover across panes).

S2a-2 as built (headers approved 2026-10-08): `VicusLatus` (one
mount) and `VicusTabula` = left pane + right stack + focus;
`vicus_latus`, `vicus_latus_focatum`, `vicus_focum_ponere`;
`vicus_tabulam_addere` gone (slots fixed); `Dispensator.super_spatium`.
Pane id = `<tab>_<side>_<kind>` (canon `nomen` refuses dots) = branch
= scope. Layout in plagula `vicus/latera`; ten default tabs from
vicus_applicatio; Ctrl-A 0 = tab 10. Left = half width rounded down to
a cell; 1 px divider; titles derived (id + right kind if different +
"[exitus]"). Click into the other pane only focuses it (strategy sends
the press to the host root). Tests: all vicus tests converted; new
`probatio_vicus_latera` (keys to the focused pane, first click
focuses / second acts in pictor, typed text handed over on a pane
switch, Motus follows, hover scope, focus durable, Ctrl-A 0);
fourteen plants. Parked: formator false positive inside a macro
expansion (terminal-planning parks/014).

S2c as built: `fenestra_spatium_utile` (approved header) - vicus opens
filling the screen's usable area (here 960 x 573 of our pixels;
`-fumus` keeps the fixed size); new scriba pages and pictor canvases
take the size of the surface they are mounted on (floors 20 x 10 and
64), existing documents keep theirs. Tests: probatio_fenestra_spatium
(plausibility, NIHIL refused), probatio_montatio (sizes, floors,
remount keeps size); five plants.

**S2 - two panes.** A tab = left editor + right stack (decision 5);
each pane's rectangle written to its branch; focus (clicking a pane
focuses it; opening a widget focuses the right pane - to confirm);
durable stack (decision 7); both targets (window + terminal twin).

**S3 - acme commands.** scriba recognises `$verb(args)` tokens (and
draws them distinctly - a figura concern); a plain click on one runs
the named action through the host's registry; the open-widget verbs
(`terminale`, `scriba(doc)`, `pictor(doc)`) push onto the right stack
with the identity rule; a first non-widget command or two.

## AUDIENDA

- **vicus's Ctrl-A prefix (T3b) collides with Fran's tmux leader**: tmux
  inside a vicus terminal pane never sees Ctrl-A. The prefix needs
  another key before tmux is used inside vicus.
- **Two panes of the same kind collide** (found 2026-10-08 planning
  S2a): figura registry keyed by (partes, thema) - miscere refuses
  collisions; action registry keyed by name; component ids ('pagina')
  repeat, and focus / lookups by id find the first. Needs a subtree
  scope (see S2a proposal).

- Focus between the panes (S2): click to focus, a prefix key to toggle,
  and whether opening a widget moves focus right.
- Command errors (unknown verb, bad args): where are they shown?
- Whether `$verb(args)` needs escaping for a literal `$word(` in prose.
- vicus's terminal twin running terminale inside a terminal (twin
  parity: decorations and drawn glyphs are window-only today).
