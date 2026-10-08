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

- Focus between the panes (S2): click to focus, a prefix key to toggle,
  and whether opening a widget moves focus right.
- Command errors (unknown verb, bad args): where are they shown?
- Whether `$verb(args)` needs escaping for a literal `$word(` in prose.
- vicus's terminal twin running terminale inside a terminal (twin
  parity: decorations and drawn glyphs are window-only today).
