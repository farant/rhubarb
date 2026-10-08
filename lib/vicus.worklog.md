# vicus worklog

## 2026-10-05 — T1b: the host store (insula-rami-plan)

The host library is `vicus` (Fran: a Roman street of insulae;
`schirmata` is the legacy concha screen system and stays). One store
with roots `<vicus>` (host canons in `apps/vicus/canones/`: the durable
root declares nothing - every child is a delegated mount; the ephemeral
root declares `activa` and the dispatcher's own attributes), one volume.
The library knows no app: the host's main registers kinds (name, the
SIZE of the kind's mount struct, a mount function) and vicus allocates
each tab's mount storage.

The tab list is a manifest entry `vicus/tabulae`
(`<tabulae activa="…"><tabula id genus titulus/>…</tabulae>`) -
configuration, overwritten on change, not history (decided at T1,
veto-able); the active tab lives there too, so it survives a restart
(decision 8). Open: read the list (or write the given default), mount
each tab in order, set `activa` (the stored one, else the first). An
UNKNOWN kind in a stored list (a volume from a newer host) is kept but
not mounted, with a named cause; ADDING a tab of an unknown kind, a
duplicate id, or a title with `"` (manifest attributes are raw) is
refused.

Plants: adding without saving the list - SURVIVED at first, because the
next step (`vicus_activam_ponere`) rewrote the list anyway; the test now
reopens right after adding; the active tab not saved; an unknown kind
aborting the open; a duplicate id accepted - all caught. Forbidden
word on the way: a field named `magnitudo` (the latina `sizeof`) -
renamed `mensura`. Glossary: montator.

## 2026-10-05 — T2a: the host's registries follow the active tab

The dispatcher holds ONE action-registry pointer and each glue ONE
figura-registry pointer + one image source; registering a duplicate
name/kind is refused. So vicus owns one `ActioRegistrum` and one
`FiguraRegistrum` (handed once - the pointers never change) and on open
and on every switch empties them and copies in the active mount's
entries (`actio_registrum_vacare/_miscere`, `figura_registrum_vacare/
_miscere` - new; a merge with ANY collision adds nothing). Only the
active app is ever in the registries, so two scriba tabs never collide
on `pagina.clavis`: each resolves to its own context.

Kinds now also register `describere(montatio, &VicusFacies)` (actions,
figurae, componere + ctx, image source + ctx); the wrappers live in the
host's main (here: the test), so vicus and the apps stay unaware of
each other. `vicus_imago_fons` delegates to the active tab's source.

Plants: switching without refilling; refilling without emptying; the
composite image source ignoring the active tab - all caught.

## 2026-10-05 — T2b: host composition and mount surfaces

`vicus_componere` is a plain `Componere`: root `vicus` carrying the
action `vicus.magnitudo`, one tab-bar component (`vicus.tabulae`,
`PARTES_INDEX`, 8 px tall), then the active app's own tree with
`fines.y += 8` on its root. `fines` are parent-relative (componens.h,
destinatio subtracts each ancestor's origin), so shifting one root
moves both painting and hit-testing for the whole subtree - the test
clicks at raw screen coordinates (`manus_ludus_premere_ad`) rather than
`#pagina`, because the selector form inverts whatever transform exists
and would pass with a wrong offset too.

Surfaces: each app reads `superficies_*` from its OWN branch, so the
host writes (W, H - 8) into every mounted branch: once in `aperire`,
and on resize. Resize is not focal or positional, so `destinatio`
sends it to the root, after the dispatcher has written the ROOT's
surface (the host reads that for its own size, falling back to the
creation size). The apps' `domini.stml` give `superficies_*` to writer
`"dispensator"`; the host saves `repo->scriptor`, writes as
`"dispensator"`, restores. Plant P4 (writer "hospes") proved this is
load-bearing: the canon/owner check silently refuses and the branch
keeps no surface.

Bar figura: background rect, the active tab's cell span in
`COLOR_SELECTION`, titles at one cell in; the active title in
`COLOR_BACKGROUND` (inverse). Tab width = (title length + 2) cells.

Plants (8, all caught by name): no offset; bar not subtracted; resize
ignored; wrong writer; active not highlighted; no initial write; first
tab composed instead of the active; title not inverted. The last one
SURVIVED my first assertion ("some background pixels in the active
title" - trivially true, the bar's background supplies them); now it
also asserts ZERO text-coloured pixels there.

## 2026-10-05 — T3a: the active branch lives in Motus

Framework writes went to the store ROOT: the dispatcher's `focus` /
`focus_acervus` (`attr_legere/_scribere`) and Motus's idle flush of
`pan_x/pan_y/zoom`. In a host that is wrong twice: focus is one value
for all tabs, and the vicus root canon has no `pan_x`, so the flush is
refused and stays dirty. My first claim ("after any pictor stroke the
flush retries every idle tick") was wrong and the red test said so: a
FINISHED stroke clears `sordida` itself (pictor_actiones, "a finished
stroke does not touch the ephemera"). The real case is an idle pause
MID-drag - mouse held still past the 300 ms quiet - which flushes to
the root, is refused, and retries on every event until release. The
test now holds a stroke across a pulse.

Fix: `Motus.ramus` (an InsulaRamus; zeroed = root, so every standalone
app is byte-for-byte unchanged). Motus is the right carrier: it already
holds the per-app gesture slot, and every action receives `Motus*`, so
T3b's switch action can move it without reaching the dispatcher. The
window surface stays at the root - it is the host's.

Switch protocol (`motum_relinquere`, then `motum_aptare`):
1. leaving gesture flushed into the STILL-active branch; a dirty
   gesture that will not flush refuses the switch (text would be lost)
   with a named cause;
2. pan/zoom flushed, and dropped if refused - otherwise the next idle
   flush would land the leaving app's pan in the arriving branch;
3. capture and pending stroke dropped (a drag cannot cross tabs);
4. branch := arriving tab's, gesture slot := its `gestum_ponere` (or
   empty).
Per-tab focus then needs no save/restore: it lives in each branch.
Mounts now write their kind's default focus (`pagina`, `tabula`) into
the branch at mount time.

`VicusFacies` is memset before `describere`, so kinds that ignore the
new gesture fields get "no gesture", not garbage.

Plants (10, all caught): flush to root; focus read from root; focus
written to root; no default focus; leaving gesture not flushed; gesture
not installed; capture kept; branch not set; bind without apply;
`motus_initiare` not zeroing the branch - that last one is caught by
the EXISTING probatio_motus (its stack Motus holds garbage), the same
class the aedilis gate caught in R3.

## 2026-10-06 — T3b: Ctrl-A prefix, tab clicks, tint

Routing is the crux. Keys go to the focus (`pagina`), and scriba's
`pagina.clavis` would eat Ctrl-A and the following letter. So vicus
installs its own `DestinatioStrategia` (now inside
`vicus_dispensatorem_ligare`, which replaced `vicus_motum_ligare` -
binding Motus and installing the strategy must not be separable). A
strategy has no ctx, so pending state is read from the TREE:
`vicus_componere` gives the root (and the bar) titulus "praefixum"
when the host ephemeral `praefixum` is "1". Pure: the tree reflects
the store; the strategy reads the tree. The root is recomposed after
every event, so the next event sees the new mark.

Key + text pairing: both real sources send a printable as key-down
then TEXTUS (terminal: `_runae_clavem` then `textum_impellere` when
no modifier; window: COMMISSUM at the same time; manus_ludus_scribere
mimics it). So a pending printable key-down is swallowed and the
switch happens on the TEXTUS. A key-down with Ctrl/Alt/Cmd, or a
non-printable (Esc), cancels - the terminal sends no TEXTUS for those,
so waiting would leave the prefix stuck. Key-ups while pending are
swallowed; the letter's key-up after the TEXTUS cleared the prefix
reaches the app, which ignores key-ups.

Ctrl-A Ctrl-A = previous tab (`prior`, written alongside `activa` on
every real switch; never written empty). n / p cycle over mounted
tabs; 1-9 index (unmounted or absent = no-op, prefix still cleared).

Tabs are `PARTES_NULLUM` hit zones (no figura; the bar figura already
paints titles) with `titulus` = tab id; the click action reads the
titulus, not the component id ("vicus.tabula.<id>"). One shared
`latitudo_tabulae` for painting and zones, so they cannot drift.

Tests end-to-end through manus: insert "ab", Ctrl-A n, back with
Ctrl-A p, type "c": the line is "abc" - neither the prefix letter
leaks nor does the insert session break across the switch.

Plants (13, all caught): strategy not installed; pending not routed;
printable key-down cancelling; TEXTUS not clearing; p as n; no
wrap-around; Ctrl-A Ctrl-A not going back; prior not recorded; no
tint; no tab zones; click using the id instead of the titulus; digits
off by one; key-downs bypassing the pending route (Esc reaches scriba,
which leaves insert mode - caught by the "zy" line).

Lint: `ctrl` is not a word here - `est_imperium_a` / `imperium_a`
(MOD_IMPERIUM). `renominare.sh` refuses a dirty file (git is its
undo) - fine for a rename inside the change being made.

## 2026-10-06 — T4: the app, and a replay proof across both targets

`lib/vicus_applicatio` is the shared composition (the pattern of
scriba/pictor_applicatio): kinds registered with their wrappers (which
lived in each test until now), default index, dispatcher over the
host store, `vicus_dispensatorem_ligare`. The glues get the HOST's
figura registry and `vicus_imago_fons` with ctx = the Vicus - pointers
handed once; switching refills the registry behind them.

Window is 480 x 488 so each app keeps its standalone 480 x 480 below
the 8 px bar. Terminal: the bar is the first row.

The replay proof records a window-path session through manus and
replays it through the real terminal encoder. It found the manus
button bug (lib/manus_ludus.worklog.md): every synthetic press was
dropped by the encoder. Debugging recipe that worked: print what the
transit RECEIVES, then hang a notary on the TERMINAL dispatcher to see
what ARRIVES - the press was present in one list and absent in the
other, so the loss was in the encoder; then print the encoder's bytes
(empty for the press, botton=0).

Plant lesson: "no tab hit zones" survived the first proof - nothing in
the session clicked a tab, and the bar is painted by its figura
either way. The session now ends with a click on the scriba tab at a
cell centre on row 0, through the encoder.

## 2026-10-08 - vicus-latera S1c-2: living tabs, terminale as a kind

- `vicus_pulsare` returns "draw a frame" only for the ACTIVE tab's
  change or for a tab that just finished (its title in the bar
  changes even when it is in the background). A background terminal's
  output is read but asks for nothing - the frame skip in the loop
  (S1c-1) then keeps the window idle while a hidden `top` runs.
- `finita` is host memory: computed into the bar at draw time, never
  written to the index. `latitudo_tabulae` is the single width
  function for the drawn tab AND its hit zone; the suffix goes in
  there, or clicks on " [exitus]" would hit the next tab.
- The toy-kind test needs the finishing tab LAST in the bar: before
  it finishes, the region where the suffix will be drawn must be
  empty, or the "no text there yet" check would see the next tab's
  title.
- Adding `vicus_applicatio.h` to a root test pulled
  `lib/vicus_applicatio.c` into the root suite: the source list is
  GENERATED (`./tools/compile_tests_fontes_generare.sh`); the
  ludus_tessera suite's list is by hand and needed terminale +
  aemulator_hospes + pseudoterminale(_posix) + glyphae_ductae.
- Every vicus built from the default index now spawns a shell. Tests
  set `SHELL=/bin/sh` first (no login shell with the tester's
  dotfiles); the two ludus_tessera vicus tests too.

## 2026-10-08 - vicus-latera S2a-2: ten tabs, two panes each

- A tab is now a left pane + a right STACK (front = last, never empty:
  a missing stack repeats the left kind). A pane is one mount; its id
  is the path `<tab>_<side>_<kind>` and is at the same time the store
  BRANCH id and the tree SCOPE (S2a-1). Underscores, not dots: the
  apps' canons type `id` as `nomen` (letters, digits, `_`, `*`) - with
  dots every mount was refused ("montatio defecit"); found by probing
  `vicus_causa` after the first converted test went red everywhere.
- Layout lives in plagula `vicus/latera` (new key; old `vicus/tabulae`
  volumes just get the default). At most VICUS_TABULAE (10) are read.
  The default (1 scriba|terminale, 2 scriba|pictor, 3-10 scriba|scriba)
  is vicus_applicatio's, vicus knows no kinds.
- Geometry: left = half the width rounded DOWN to a 6 px cell, right =
  the rest; each pane's surface written to its branch. A 1 px divider
  component after the panes (host scope "", PARTES_NULLUM figure that
  draws only for id `vicus.divisor` - the root gets the same figure
  and draws nothing, and pane roots never see it: lookups are strict).
- Click focus: the strategy compares the pressed component's scope
  with Motus.spatium; another pane -> the press goes to the host root,
  whose handler focuses the side under x. FIRST CLICK ONLY FOCUSES.
  The strategy has no ctx, so the decision is made where the data is
  (Motus) and the action where the Vicus is (root handler) - no
  dispatcher hook needed.
- Motus follows the focused pane: ramus, spatium, gesture; switching
  pane = same handover as switching tab (motum_relinquere).
- Titles are derived: id, + front kind when it differs from the left
  kind, + " [exitus]" when a visible pane finished. One function for
  the drawn title and the hit zone.
- scriba does not handle mouse presses at all (no cursor move on
  click): the "first click only focuses, second acts" proof uses
  pictor (stroke count). House vim's Esc leaves the cursor where it
  is (real vim steps back one).
- Plant lesson again: the gesture-flush plant survived while the test
  typed `iuno` + Esc - Esc commits scriba's text by itself. The test now
  types WITHOUT Esc before switching panes.
- `vicus_latus` takes a mutable `VicusTabula*` (approved header said
  constans; returning a mutable pane from a const tab is a cast-qual
  error and vicus_tabula hands out mutable tabs anyway).

## 2026-10-08 - vicus-latera S2c: window fills the screen, new docs fit their pane

- `fenestra_spatium_utile` (new, fenestra_macos.m): the primary
  screen's visibleFrame through `contentRectForFrameRect:` for the
  ordinary window style = the content rect of a titled window filling
  the usable area. Here: 0,0 1920x1147 points -> 960x573 of our pixels
  at SCALA II -> panes 480x565 (80 x 70 cells).
- vicus main asks BEFORE building the composition (mounts take their
  size, new documents their size from the mount); `-fumus` keeps the
  fixed size so screenshots stay deterministic. The window opens at
  the usable rectangle; the twin and the standalone apps unchanged.
- New documents from the mount size (scriba_montare, pictor_montare -
  standalone mounts too, a window is a pane): scriba page = cells
  minus margins (and the status line in height), floor 20 x 10;
  pictor canvas = prospect (status line off the height), floor 64.
  Existing documents keep theirs (tested by remounting at another
  size). `i32` is unsigned: the subtraction is done in s32.
- The window main itself has no test (it needs a window); the query,
  the size rules and their floors do (5 plants).

## 2026-10-08 - S2c follow-up: open in FULL SCREEN

Fran went full screen after launch: the window grew 573 -> 600 of our
pixels but new documents had been sized for 573 (scriba ~4 rows,
pictor ~27 px short). Fran works full screen, so vicus now opens in
it: `fenestra_spatium_schirmi` (new) = the screen's frame minus the
notch (`safeAreaInsets.top`, macOS 12+) - here 1920 x 1200 -> mounts
and new documents get 960 x 600. The window starts at the usable
rectangle with FENESTRA_PLENA_VISIO; its height is a whole number of
OUR pixels (573 x 2 = 1146 points) because the buffer's scale is fixed
at creation (window height / buffer height) and kept on resize:
1147/573 = 2.0017 would make full screen 599, a pixel shorter than the
documents. Checked headless first: at 960 x 573 page and canvas fit
exactly - the mismatch was only the later full-screen growth.

