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


## 2026-10-08 - full screen AFTER the buffer (scale trap)

The first full-screen version passed FENESTRA_PLENA_VISIO to
fenestra_creare, which starts the transition before it returns; the
pixel buffer was created after, from the window's CURRENT (already
growing) height, so the scale came out above 2 and the logical area
shrank below the documents' size - Fran: one column too wide, one row
too tall. Now: create window, create buffer (scale exactly 2), then
`fenestra_commutare_plenam_visionem`. Fran confirmed the page fits.
Rule: anything that fixes a scale at creation must run before any
size transition starts.

## 2026-10-08 - S2b-2: a context per kind, handed to every mount

`VicusMontator` gained a final `vacuum* ctx`; `vicus_genus_addere`
takes it and `VicusGenus.ctx` keeps it; `latus_montare` passes it on.
Purpose: every scriba mount must receive the SAME page book (S2b-3);
vicus still knows no kind - the composition (vicus_applicatio) owns the
book and registers it with the scriba kind. Other kinds pass NIHIL.
Proof: the pulse test's toy kind records the ctx it was mounted with
(registered with a marker pointer, a second kind with NIHIL); plant
(vicus passes NIHIL) caught.

## 2026-10-08 - S2b-3: shared pages need two refresh points

Fran's simplification: every focus change and tab switch commits the
leaving view's gesture, so only the focused view can hold uncommitted
text - no rebase, only stale unfocused views. Refresh happens (a) in
the scriba kind's pulse (`scribam_pulsare` -> `scriba_reficere`;
returns VERUM once, so the frame repaints once) and (b) when focus
arrives (`scribae_gestum` refreshes before installing the gesture).
(b) is not redundant: a click into a stale view with no pulse in
between would otherwise type onto the old sheet and the commit would
REVERT the other view's text (plant caught).

## 2026-10-08 - S3b-1: the scriba kind's ctx is a struct now

`ContextusScribae { liber, iussa }` replaces the bare page book as the
scriba kind's ctx, so every scriba view shares one command registry as
well as one book; `scribam_montare` sets `actiones_ctx.iussa` after
`scriba_montare`. `$dies` lives here (host owns verbs). Also: a new
library called from scriba must be added BY HAND to
ludus_tessera/compile_probationes.sh (`iussum`, like `scriba_liber`
before) - the root suite's list is generated, that one is not.

## 2026-10-09 - S3c: the right-pane stack opens by command

Two traps avoided by design: (1) a command click runs INSIDE scriba's
event handler, so opening a pane there would swap registries and the
Motus gesture mid-dispatch - `vicus_acervo_aperire` only queues, and
`vicus_pulsare` applies (the next frame's EVENTUS_NIHIL recomposes,
so nothing waits for input). (2) ids were derived from position
(`<tab>_<side>_<kind>`); with several panes of one kind and reordering
on bring-forward, a derived id would hand one pane's state (branch,
pictor document, scriba visus plagula) to another after a restart -
so the layout now stores each pane's id. Old layouts without ids keep
their derived ids. Moving a pane to the front copies VicusLatus
structs within the Xar; nothing outside holds VicusLatus pointers
across a pulse (Motus keeps the branch by value and the montatio
pointer, which does not move).

## 2026-10-09 - first click runs commands (ictus_primus)

A pane can act on the click that focused it (`VicusFacies.ictus_primus`,
called from the root handler after `vicus_focum_ponere`, so Motus is
already the pane's). Lesson: when a pane's handler is called from
INSIDE another action, the repository's writer is that other action
(`vicus.radix`), and the pane's domini refuse its writes WITHOUT an
error - the date was inserted (document commit) but cursor and message
silently stayed. Any hook that calls into a pane's action must set the
writer to that action's name for the call (scribae_ictus_primus does).
A test only caught it after its text was changed so the cursor would
actually move - the original assertion passed by coincidence.

## 2026-10-09 - Ctrl-A prefix removed, Cmd+digit switches tabs

Fran uses Ctrl-A as tmux's leader inside terminale panes, so vicus no
longer binds it: the prefix state (`praefixum` attribute, tinted tab
bar, n/p, Ctrl-A Ctrl-A for the previous tab) is gone and Ctrl-A
reaches the focused pane. Cmd+1..9 / Cmd+0 switch tabs. The routing
rule in `destinare` is NOT redundant even though a Cmd key unhandled by
scriba bubbles up to the root anyway: terminale consumes EVERY Cmd key
(window shortcuts never go to the program), so with the terminal
focused Cmd+digit would die there - a plant that removed the rule
survived until the test focused the terminal first. Terminal twin:
Cmd never crosses a terminal, so tabs switch by clicking the tab bar
(the twin replay test now clicks). `prior` is still recorded in the
ephemeral layer but nothing reads it. Test renamed
probatio_vicus_praefixum -> probatio_vicus_claves.

## 2026-10-09 - a terminal pane takes focus when vicus opens

Fran typed `tmux a` right after launching and the keys went to
scriba in normal mode (`u` undid his text). vicus knows no kinds, so
the kind says it: `VicusFacies.focus_in_apertura` (terminale sets it).
`vicus_aperire` gives each tab's focus to such a pane - left first,
then the front of the right stack - overriding the saved focus. Only
at open: switching tabs keeps the last focus (a deliberate click into
scriba in tab 1 survives Cmd+2, Cmd+1). Tests: probatio_vicus_latera
section 0; the twin replay session now clicks into scriba first.

## 2026-10-09 - S4 clock in the tab bar (Fran)

"7:52 PM" right-aligned in the tab bar, one cell margin. The vicus has
NO clock by default (`horologium` NIHIL, hora/minutum -1): every
existing test and golden composes a vicus, and a real clock would
make them change every minute. The two app mains (apps/vicus/vicus.c
and vicus_terminalis.c) install `vicus_horologium_locale`
(time/localtime, plain C89 <time.h>); tests inject a fake one.
vicus_pulsare reads the clock each pulse and returns VERUM only when
(hour, minute) changed, so the app repaints once a minute for it.
vicus_horologium_ponere reads once immediately so the first frame has
the time. Format: hour % 12 with 0 -> 12, minutes %02d, AM below 12.

## 2026-10-09 - S3e-3: $pictor-next / $pictor-prev (Fran, option A)

- vicus_acervo_mutare queues a PetitioAcervi with `mutare`: at pulse
  the front-most pane of the genus (last in the stack) gets the new
  argumentum via facies.argumentum_ponere, moves to the front, focus
  untouched, layout saved (indicem_scribere). No pane of the genus:
  falls through to the ordinary open path (focus right).
- Selection lives in vicus_applicatio (cyclus_iussum): library order,
  wrap, skip the current drawing and any drawing shown by ANOTHER
  pictor pane in any tab. A pane's drawing = its argumentum if that
  names a library drawing, else its id (old $pictor(x) identity-only
  arguments keep working).
- ONE document object per drawing: all pictor panes share the cache
  (ContextusPictoris.documenta, put into PictorMontatio.documenta at
  mount). Per-pane caches would go stale: pane A shows X and leaves,
  pane B opens X and draws, A comes back with an old history cursor
  and its next stroke would fork/corrupt X's log (plant P5).
- Known: a brand-new pane (tab without a pictor) still creates its own
  blank id-drawing before switching, so the library gains one blank
  drawing per such tab. Two clicks within one frame both compute from
  the same current drawing.

## 2026-10-09 - skip rule dropped (Fran)

With one shared document object per drawing the "skip drawings shown
in another pane" rule protected nothing: strokes commit on mouse
release, a focus/tab switch runs motum_relinquere first, and there is
one pointer and one focused pane - so all writes to a drawing go
through one history in order (Fran's reasoning). $pictor-next now
visits every library drawing except the one the pane already shows;
the same drawing can be open in several tabs or side by side, each
pane with its own tool/colour/zoom state. probatio_vicus_pictura I
draws in tab 3's pane on tab 2's drawing and sees the stroke in tab
2's document (same object).

## 2026-10-09 - S3f: $pictor-new (Fran)

New blank drawing "pictura_<n>" (first free n from 1, checked against
the library) shown in the tab's front pictor pane; no pane -> one opens.
- Creation rides the existing argumentum hook: pictor's hook is now
  "show it, or create it if the library lacks it"
  (pictor_picturam_ponere || pictor_picturam_condere). Only
  $pictor-new produces unknown names - next/prev pick from the library.
- vicus_acervo_mutare against a tab WITHOUT a pane of the genus now
  mounts and THEN calls facies.argumentum_ponere (was: mount only).
  Mounting alone left the new pane on its own blank id-drawing, which
  looks identical to the new one - plant N2 survived until the test
  asserted pictura_3 actually exists in the library.
- Size = what a fresh pane's canvas gets (compositio.fenestra_* minus
  margins and status line, same formula as pictor_montare).
- Test gotcha: every tab's left scriba shows the SAME page, so typing
  commands in tab 2 (section VIII) prefixed line II for tab 3 too and
  "$pictor-next$pictor-next" made the second '$' non-command (after
  't'). Section X writes fresh lines.

## 2026-10-10 - S4b: clock <-> date toggle (Fran)

Clicking the clock shows "Friday, October 9th, 2026" (English, as
Fran wrote it) right-aligned; clicking again returns to the time.
- VicusHorologium now fills a VicusTempus (year, month, day, weekday,
  hour, minute); the pulse compares the WHOLE struct, so the date
  updates at midnight even if hour/minute were equal.
- The click lands in radicem_tractare (bar clicks outside a tab's own
  zone fall through to the root); the hit area runs from the current
  text's left edge to the bar's right edge, so it tracks the wider
  date. Tab clicks never reach the root (their zone has its own
  action) - the meaningful negative test is a click on EMPTY bar
  space, which plant C5 needed.
- Ordinals: 11th/12th/13th before the 1/2/3 rule (x % 100 in 11..13).
- The toggle is session state (Vicus.horologium_dies), not saved.

## 2026-10-12 - S7a: the host's root on its manifest (status-plan S7a)

The root `<vicus>` is now prepared like any widget branch, from
`apps/vicus/organum.stml` via `organum_ramum_parare` (no systems yet:
every root field is transient). The hand canon
apps/vicus/canones/ephemera.canon is gone.
- Root vs branch canon: a canon attached as a BRANCH canon to the root
  ramus judges the whole tree (ramum_invenire(root) = root) and rejects
  every mounted pane. organum_ramum_parare now gives a root ramus
  (empty elementum) the repository ROOT canon, which judges
  visio_radicis - mounts delegated to their own canons. Plant P1
  (branch canon on root) broke 12 of 13 vicus suites.
- Owners: activa/prior/horologium_dies belong to `vicus.radix`
  (the root tractator's action - the dispatcher already runs the
  clock click under it); readings belong to the system `horologium`.
  Host code writing on its own behalf goes through `radicem_mutare`
  (scriptor set for the call, previous one restored) - same pattern
  as superficies_scribere's "dispensator" bracket.
- Readings: the Vicus struct no longer caches the clock/battery. Six
  number fields tempus_* (one write per change, CampiRadicis) and
  pila_centesimae/pila_oneratur; an absent field = no reading (replaces
  the -1 sentinels). No port = reading "none" = fields removed, so
  a stale reading cannot outlive its port. Change detection compares
  the new reading with the island, not a struct copy.
- v->activa remains as the host's working copy of the layout index
  (the index file stays the truth; the island copy is derived).
- probatio_terminale built its host root from vicus canons; the
  durable one had been deleted in C2 and was silently NIHIL. Now it
  prepares the root from the manifest like vicus does.

## 2026-10-12 - S7b: one yank register for the whole app (status-plan S7b)

`capsa`/`capsa_lineae` now live on the vicus root, persisted, shared by
every scriba pane.
- "Controlled" state, smallest form: the parent hands the child a
  branch (`ScribaActiones.ramus_capsae`, set after mount like `liber`
  and `iussa`). Repo NIHIL = scriba's own branch, so standalone scriba
  is unchanged (register per pane, transient).
- Systems moved from "first pane creates them" (systemata_parare) to
  vicus itself: `vicus_systemata_ponere` before `vicus_aperire`, which
  creates conservatio/relatio/dimissio right after the repository and
  prepares the root with them (status plagula `status/vicus`). The
  root needed them BEFORE any pane mounts - the lazy pane path could
  never serve it.
- Owner: the manifest checker rejects a `scriptor` that is not a
  declared `<actio>`, so the host manifest declares `pagina.clavis`
  (scriba's action) with a comment. Honest for now (the child writes
  what the parent holds); a second pane kind wanting the register
  turns this into an owner list or a system.
- Write only on change: the register gets its own mutare, skipped when
  the escaped text and line count already match. Without it every key
  would rewrite the persisted root. probatio_scriba_actiones I used to
  require `capsa_lineae="0"` after typing; it now asserts the empty
  register is NOT written (plant P2 caught there).
- Our vim has no `y`; `dd` (and visual delete) fill the register. The
  vicus test uses `dd` in tab 3 and `p` in tab 4.

## 2026-10-12 — status-plan S8c-1: spatium = branch path

A pane used to have two addresses: its id ("1_dextrum_pictor") was the
component/registry namespace, its branch path ("pictor:1_dextrum_pictor",
S8a) addressed state. Now there is one: `via_lateris` gives the path
and vicus stamps it on the pane subtree (`latus_componere`), the
registry merge and `Motus.spatium` (`motum_aptare` uses
`motus->ramus.via`). Readers that compared the namespace with
`ramus.id` (dimissio registration, pictor/scriba `focatum_est`, scriba
page click in vicus_applicatio) compare with `ramus.via`.

Persisted names are NOT namespaces and did not move: the vicus index
keeps `id`, pictor's document key in the volume is still the id
(`clavis_memoriae`), scriba's `scriba/visus/<id>` likewise. Namespaces
are runtime only, so no migration.

Tests that hard-coded the namespace changed with it (vicus, latera,
acervus, compositio, conservatio, dimissio). The dimissio test's
`ramus_nodi` turns a namespace back into a branch by stripping "pop:";
S8c-2's `insula_ramus_ex_via` is the real answer to that.

Commit side-trip: touching probatio_dimissio.c made the pre-commit
examen judge it, and credo rule 92 (credo II) rejected four
`hic_manens character attr_*[]` statics that predate the rule - non-
const statics passed to non-const params count as section writes. They
are now `constans`; the three actions that hand one to a mutator
(`vacuum* ctx`) pass a local copy (`strcpy` into `titulus`), same idiom
as probatio_organum_profundum's `scribere`. Also seen: one `generata`
gate run said the vates_responsum generator "wrote nothing" while
the generator's output was byte-identical and the gate passed alone
right after - transient, likely bin/norma rebuilt mid-gate by a
parallel gate. Not chased.

## 2026-10-17 - T4f: Cmd+] / Cmd+[ cycle the right stack

`vicus_cyclum_ponere` registers the cycle types (vicus_applicatio:
admonitiones, colloquia); Cmd+] / Cmd+[ queue a PetitioAcervi with
`gradus` +1/-1, applied in vicus_pulsare in order, so each step reads
the real stack (two presses in one frame = two steps). Home = topmost
non-cycle pane (none: the bottom one; a step to the home's own type is
skipped). Going home truncates the cycle panes above it (like Esc) and
restores `focus_domus`, which `petitionem_applicare` records whenever
a request moves the front off home - so the badge and
`$notifications` start a cycle too.

Pre-existing bug found on the way: `petitionem_applicare` called
`id_novum` AFTER `xar_addere`, and after a truncation (Esc, or the
cycle going home) the reused slot still holds the old pane - so its id
was "taken" and a reopened notifications pane became
`3_dextrum_admonitiones_2` (fresh mount, new ramus). id now chosen
before the slot is added.

Plants (silva.planta, radix filtrum vicus_acervus): 7 red (focus not
restored / not recorded, home type not skipped, direction inverted,
home always the bottom, id after xar_addere, unknown type accepted).
GREEN, unobservable: dropping the `destinare` route for Cmd+[ ] - the
focused panes (scriba, pictor) pass the chord up unconsumed and the
root handler sees it anyway; kept as the same guard Cmd+digits have.

## 2026-10-17 - T4f: agenda in the cycle (entries = type + argument)

Fran wanted the agenda as the last stop. `$agenda` is calendarium with
argument "agenda", so a type-only cycle would open the plain calendar
and count ANY calendar pane (a month view kept as home) as a cycle
pane. Cycle entries are now `LatusCycli` (genus + argumentum, the S3c
identity) everywhere: membership, the home skip, and the open request.
Plants (4, all red): identity ignoring the argument, argument not
passed to the open, `argumenta` ignored by vicus_cyclum_ponere, home
skip by type only.

## 2026-10-19 — semitae R3: per-pane history, $via (project-specs/semitae-spec.md)

- `VicusFacies.ramus` = a pane opts into routes (history, `$via`). The
  saints pane is the first; the others follow in R4.
- Recording: at the end of every `vicus_pulsare`, each opted-in pane's
  FULL route (`semita_legere_plenam`, defaults explicit - canonical
  routes omit defaults and "absent = keep", so they cannot restore
  "back to the list") is compared with the entry at the pane's cursor:
  equal -> nothing; same `historia="impelle"` fields (a "signum" of
  their values, children included) -> the entry is replaced; otherwise
  forward entries are dropped and a new entry pushed (cap C, oldest
  dropped). Reads go to `v->scheda_semitarum`, cleared every pulse;
  only recorded entries are copied into the vicus arena.
- Entries keep a DEEP COPY of the route tree (`stml_duplicare`) for
  applying, plus its text for comparing. Found by the test: STML
  writes an empty attribute value as a bare attribute and reads it
  back as boolean "true", so a text round trip turns "explicitly
  empty" (`quaestio=""`) into `quaestio="true"` - which the
  pagina_textus number check then refused. Text is fine for
  comparison (both sides lose the same way), not for applying.
- Ctrl+O / Ctrl+I in `radicem_tractare`, i.e. only after the focused
  pane did not consume the key; acts on the FOCUSED pane and only if
  it opted in (scriba keeps vim's Ctrl+O, the terminal its keys).
  Applying makes the next pulse find the cursor entry equal, so it
  records nothing.
- Plants 9 red (one test tightened first: the "calendarium: semitas
  nondum accipit" message, not just "calendarium"). Not planted: the
  early "unchanged" exit (removing it replaces an equal entry -
  harmless) and the start/end bounds (removing them crashes).

## 2026-10-20 — semitae R4d: saved route beside the argument (restart)

- `l->argumentum` is IDENTITY (acervus reuse, Cmd+]/[ cycle match
  genus+argument) - overwriting it with the route would open a second
  agenda calendar. The persistent route lives in `semita_servata`,
  saved in the index as `semita="..."`.
- Restore is LAZY: panes mount inside vicus_creare, before
  vicus_organa_ponere, so `semitam_restituere` runs on the pane's first
  pulse (history still NIHIL), then `semitam_secuta`.
- `semitam_servare` writes the index once per pulse when any pane's
  persistent route changed. First computation for a pane WITHOUT an
  argument is silent: writing it appended an act on every launch, and
  probatio_vicus_iussa / _quaestio broke because scriba's "cursor" is a
  VOLUME SEQ (they assume no other act lands between) - that was the
  symptom; the cause was a pointless startup write. Direct guard:
  probatio_vicus_semitae `initium_tacitum` (acta count unchanged).

## 2026-10-22 — D5 S3b: host shortcuts, advocare, transient argument

- **Host shortcuts need the routing step, not the root handler.** Keys
  the host wants FIRST (Cmd+1..9, Cmd+]/[, now Cmd+Shift+F) must be
  claimed in `destinare` (the DestinatioStrategia), because the
  terminal pane consumes every Cmd chord and nothing bubbles back.
  The old checks were stateless (shape of the event); a registered
  shortcut is data on one Vicus, so `DestinatioStrategia` gained a
  `ctx` (dispensator_ponere_strategiam(d, s, ctx); NIHIL strategy =
  destinatio_geometrica called directly, so its 18 test call sites
  kept their signature). manus_ludus calls the strategy directly for
  replay divergence - it got the same NIHIL/ctx handling.
  Test guard: probatio_vicus_indagatio "terminale focatum" (tab 1,
  terminal pane focused); with scriba focused the chord bubbles to
  the root anyway, so that step alone would not catch a missing
  routing check.
- Modifiers match EXACTLY over Shift/Ctrl/Alt (Caps Lock, Num Lock and
  side bits ignored); Cmd+F must not fire Cmd+Shift+F. Runa is the
  unshifted key ('f'), as fenestra delivers it.
- `vicus_advocare` = an S3c petition (genus, no argument) plus an
  `advocare` flag; after front + focus, `facies.advocatum` runs (the
  search pane opens its prompt). Applied in the pulse like every
  petition, so the hook writes outside event handling.
- `argumentum_transiens`: identity is genus + argument (R4d note
  above). `$search(words)` / `$via(indagatio?q=..)` go through
  vicus_acervo_mutare, which overwrote the argument - the next
  Cmd+Shift+F or Cmd+]/[ (both look for "indagatio" WITHOUT argument)
  would then mount a second search pane. With the flag the pane keeps
  an empty argument both in the mutare branch and after a new mount
  (montare still received the argument). Its state survives a restart
  through semita_servata, as for every pane with routes.
- Unchanged semantics worth knowing: vicus_acervo_mutare focuses a
  NEW pane (S3c path) but keeps focus when the genus is already in the
  stack - so the first `$search(words)` focuses the search pane, later
  ones leave focus where it was.
- Plants (silva.planta): see vicus_applicatio worklog (one list for
  S3b).

## 2026-10-22 — D5: one Ctrl+O timeline per tab (Fran)

Fran, after S3b: Ctrl+O could not get back from search to the
terminal underneath - history was per pane (R3) and changing the
stack's front pane was recorded nowhere. Now VicusTabula holds the
list (LocusTabulae: pane id, genus, argument, side, full route).

- Recording (historiam_tabulae_notare, per tab per pulse): every pane's
  state is read (so R4d's restore-once flag, now `historia_visa`, is
  set for hidden panes too), but only VISIBLE panes (left, stack
  front) make entries; an impelle change pushes, a substitue change
  rewrites the current entry if it is that pane's; a new front id
  pushes (front pane with its route); an empty list seeds the front.
- Stepping is a PETITION (reordering/mounting inside event handling is
  not allowed), the cursor moves at once so several presses before a
  pulse add up. Applied: pane found BY ID first (pictor's argument is
  identity and changes with $pictor-next - genus+argument would mount
  a second pictor), else reopened by genus+argument; route applied,
  semitam_secuta, focus to the entry's side; the pane's seen state and
  the tab's front id are updated so the next pulse adds nothing.
- Correction to what I told Fran: scriba does NOT consume Ctrl+O (the
  old R3 code returned FALSUM for a route-less focused pane and the key
  fell through). So Ctrl+O from the left scriba steps the tab - which
  makes "notebook hit opened on the left, Ctrl+O back to the search"
  work. The terminal does consume it.
- Tests that open a pane before the first pulse miss the starting
  entry (the first pulse records whatever is front then). The real app
  pulses before any input; probatio_vicus_semitae's mundum_struere now
  pulses once. Seeding earlier (in petitionem_applicare) would mark
  panes seen before their saved route is restored - rejected.
- Plants (silva.planta, vicus_semitae), all red: front change not
  recorded, impelle replaces, substitue not refreshing the current
  entry, futures kept, cap ignored, pane not brought forward, closed
  pane not reopened, route not applied, focus not following, seen
  state not set after a step, front id not set after a step, no
  starting entry. Not planted: hidden-pane changes making entries
  (no test drives a hidden pane), several presses before one pulse.

## 2026-10-22 — two histories per tab, one per side (Fran)

Fran wanted Ctrl+O in scriba to go back to the previous PAGE, which a
single tab timeline cannot promise (a calendar step in between would
win). Now `historia[II]` / `historia_cursor[II]`, indexed by
VICUS_SINISTRUM / VICUS_DEXTRUM; Ctrl+O steps the focused side's list
(`t->focus`). Left: route changes of the left pane (scriba page). Right:
front changes and the front pane's route changes. Each side seeds its
first entry from its current state. The left pane's saved route is now
also saved/restored (semitam_servare/restituere for sinistrum) and read
back from the index (`semita` on the left <latus> was written but never
read). Earlier note's "Ctrl+O from scriba steps the tab" is superseded:
from scriba it steps the left list.

Plants (vicus_semitae), all red: left steps filed on the right, steps
always on the right, left route never saved, no first left entry; in
scriba: page field never written, route not followed, wrong writer for
the field; vicus: scriba pane without `ramus`. The 12 single-timeline
plants above were not re-run on this shape (their anchors changed);
the same behaviours are covered by the same tests on the right side.

## 2026-10-22 — locus of a pane (comites P1)

`vicus_latus_locus` / `_titulus`: facies.locus if the pane supplies
one (calendar), else `semita_locus` over its ramus, else the genus
(notifications, sessions, terminal until P4). Title: facies hook, else
the locus itself. Views supply titles through hooks in
vicus_applicatio (search, notifications, sessions, picture) or their
own headers (calendar, contacts, saints). The calendar computes its
locus in code, not by manifest rounding: an absent day MEANS today
(not a symbolic value a resolver could see), and week rounding needs
the calendar's Sunday start. History entries renamed StatioHistoriae
(was LocusTabulae). Plants red: facies.locus ignored, title hook
ignored.

## 2026-10-22 — Cmd+T companion notebooks (comites P3)

Cmd+T is the host's own key (like Cmd+1..9 and Cmd+]/[): claimed in
`destinare`, exact modifiers (Cmd+Shift+T does nothing). vicus knows no
apps, so the left pane exposes `facies.comitem_ponere` (book prefix +
bar title) and `facies.comes_currens` (its current prefix). Follow mode
has NO extra state: a companion is active when the left book's prefix
starts with "comes/"; each pulse, if active and the right front's
"comes/" + locus differs from `t->comes_secutus`, the left pane is
re-pointed. After a restart `comes_secutus` is empty, so the first
pulse re-applies the companion with its (transient) bar title. Focus
goes left on Cmd+T both ways. Consequence found in tests: any book
whose prefix starts with "comes/" is treated as a companion and will
follow - P2's test used such a prefix by hand and now uses "alius/".
Plants red: Cmd+T not routed (terminal focused), modifiers ignored
(after adding a Cmd+Shift+T check), no follow, focus not left, title
without the location's label, toggle not returning to the home book.

## 2026-10-22 — a history per book (comites P3b, Fran)

Fran, after trying P3: Ctrl+O inside a companion walked back into OTHER
companions' pages (and switched the book while the right pane stayed).
Links (#tags, page numbers) are local to a book; history must be too.
New `facies.historiae_ambitus`: a side's history is kept per SCOPE
(scriba: its book prefix). On a scope change the current list is
stored under the old scope and the new scope's list (or none: first
entry seeded) becomes current; the change itself makes no entry.
`historia[] / historia_cursor[]` stay "the list in use", so callers and
tests did not change shape. Trap: the common book's scope is "" and
TabulaDispersa keys cannot be empty - keys are "#" + scope (plant
red). P2's test that stepped back ACROSS a book switch now checks the
opposite. Plants red: scope ignored, scope change makes an entry,
stored history not restored, empty key, scriba without a scope.

## 2026-10-22 - comites P5a: Cmd+F = aditus "quaestio"

Cmd+F (exact: no Shift/Ctrl/Alt) is a host key like Cmd+T: claimed in
`destinare`, handled in `radicem_tractare`, ALWAYS swallowed (a pane
with no `<aditus>` - the terminal - gets nothing, by Fran's decision).
`vicus_aditum_sequi` looks up the FOCUSED pane's manifest iter: a
`semita` becomes a mutare petition (genus = URI up to '/' or '?') with
`advocare` set; no `semita` calls that pane's `facies.advocatum` at
once (focus stays).

The mutare-existing branch of `petitionem_applicare` used to keep focus
and never advocated (D5 only advocated on the open/new path). With
`p->advocare` it now sets focus right and calls the front's advocatum
after `motum_aptare`. The plant that proved this needed a NEW test step
(Cmd+F from the left scriba while a search pane is already in the
stack) - every earlier step focused right first, so the focus plant
stayed green.

The D5 S3b test "Cmd+F without Shift: nothing" moved to Cmd+Alt+F
(still proves the Cmd+Shift+F brevitas needs exact modifiers).

## 2026-10-22 - P5a: Esc cancels a summoned search (one key, not Esc + Ctrl+O)

Fran: after Cmd+F from scriba, getting the old right pane back took Esc
(close prompt) then Ctrl+O. Now an advocating petition (vicus_aditum_sequi
with a semita, and vicus_advocare = Cmd+Shift+F / `$search`) records on the
tab: focus side + right history cursor BEFORE (`aditum_notare`). When it is
applied and the summoned pane's prompt is open, the tab goes PENDENS
(`aditum_pendere`, both advocatum sites in petitionem_applicare).

Detection lives in `destinare` (`aditum_observare`): it sees every event
before the panes. Esc with the summoned pane focused and its prompt open ->
REVOCANDUS (the pane still gets the Esc and closes its own prompt - no pane
code changed). Mouse down -> NULLUS. Next pulse, before petitions:
`aditum_revocare` queues a right-history petition back to the saved cursor
(only if history moved past it), state REDDENDUS; after petitions
`aditum_finire` restores focus to the saved side. PENDENS also ends when
front/focus changes or the prompt closes any other way (Enter).

Host reads the prompt through membrum__prompta's `prompta_aperta` on the
pane's ramus - vicus now includes membrum__prompta.h (the `prompta` field
convention is shared by indagatio, sancti, nomenclator).

Test trap: the D5 history test pressed Ctrl+I on a focused TERMINAL to
check history - the terminal eats Ctrl+I (Tab). Checked the forward entry
via historia_cursor instead.

## 2026-10-22 - P5b host pieces

- `facies.comitem_ponere` gained the paired route; `comitem_sequi` passes
  the front's canonical route (`semitam_lateris_legere`: semita_legere ->
  semita_ad_uri, empty for views without routes).
- `vicus_comitem_ostendere`: show a GIVEN companion (search result) - seeds
  `comes_secutus` from the current front so follow reacts only to later
  front changes; focus left is DEFERRED (`focus_differtus`) to after the
  next pulse's petitions, because opening a new pane forces focus right.
  The Esc-cancel focus restore now uses the same deferred step
  (ADITUS_REDDENDUS removed).
- `facies.iter_semita`: a view computes an iter's route in code (same
  pattern as facies.locus); empty = the manifest's semita.

## 2026-10-26 — vicus quies T1: idle baseline (before any change)

Plan: project-specs/vicus-quies-plan.md. Measured on Fran's running
vicus (pid 54176, `-volumen ~/.rhubarb/paginae-proba.volumen`, layout
with a terminal pane), untouched:

- `top -l 7 -s 5 -stats cpu,idlew,power`: CPU 4.6-5.1%, POWER ~5,
  IDLEW 0 (top's idle-wakeup column does not see the 16 ms loop - use
  `vicus -quies` / the exit line for the wake-up rate instead).
- `sample 54176 10 1` (scratch copy vicus_quies_ante.txt): main thread
  7642 samples, 7202 (94%) waiting in fenestra_expectare_eventus, ~440
  awake - i.e. ~0.44 s CPU per 10 s. Of the awake time:
  vicus_pulsare 401, of which terminale_pulsare 163 (pty poll every
  pulse), insula_ramus_attributum 89 + semita_legere_plenam 57 +
  semita_legere_persistentem 45 (per-pulse bookkeeping, Task 3),
  systema_terminalium 58 (tmux list-clients via multiplexor_clientes ->
  processus_metere 52, Task 2), admonitiones 28 + praeconium 19.
  ludus_quadrum 7: almost no painting - the cost is waking 60x/s and
  re-checking everything, not drawing.

Counters added for the after-measurement: `mensurae.excitationes`
(loop passes) and `mensurae.pulsus` (pulsator calls), both on the
`ludus:` exit line; `vicus -quies` prints pulses/s and pulses that
changed something every 10 s to stderr.

## 2026-10-26 — vicus quies T2: tmux client check async, only with a terminal

`systema_terminalium` used to run `tmux list-clients` (one per session
server plus the default) with the blocking `processus_exsequi` every
5 s, even when no terminal pane existed. Now:

- No mounted, unfinished `terminale` pane -> return before the 5 s
  stamp (no tmux; a pane opened later is checked on the next pulse).
- Due -> `multiplexor_clientes_incipere` per server (new in
  multiplexor.h), handles kept in `ca->terminalia_quaestiones`
  (QuaestioClientium {servitor, processus}); the pulse returns.
- Later pulses `processus_pulsare` every handle (never blocks); when
  all are PARATUS: metere, `multiplexor_clientes_legere`, then
  `terminalia_applicare` (the old pane loop, moved verbatim).
- `scheda_terminalium` is only vacated when nothing is in flight - the
  handles and servitor strings live in it.

Tests: probationes/probatio_vicus_quies.c with a FAKE tmux first on
PATH (shell script in the credo area: logs "$1 $2", sleeps, cats a
clients file) - no real server needed. The layout is seeded via
`volumen_praesens_ponere("vicus/latera", ...)` because the default
layout has a terminal on tab 1. Plants: immediate processus_metere
after incipere (async test red: 900+ ms pulse) and the terminal check
neutralized (`&& FALSUM`; deleting it made the helper unused ->
-Werror, plant refused by the compile pre-check).

## 2026-10-26 — vicus quies T3: route bookkeeping only after an insula write

Every pulse used to rebuild, for EVERY pane of every tab, the full
route (`statum_lateris_legere`, for tab history) and the persistent
route + URI (`semitam_servare`) - ~40% of idle awake time. Routes are
a pure function of insula state (organa manifests are static;
`facies.iter_semita` is only used when jumping to an iter), and vicus
has one repository whose `versio` bumps on every successful write.

- `VicusLatus` gained `semitae_lectae / semitae_versio /
  semitae_lectiones`. `semitae_legendae`: never read, or the repo
  version moved. A pane without a ramus is read once, then never.
  `latus_montare` resets `semitae_lectae` (new ramus).
- vicus_pulsare per tab: version captured BEFORE semitam_restituere
  (a restore write in this pulse -> one more read next pulse, never a
  missed one); servare + the history loop skip unchanged panes;
  `semitas_notare` marks them afterwards.
- The post-loop history paths (front pane changed, empty history)
  read directly and are not gated.
- Coarse by design: any write (clock once a minute) re-reads every
  pane once. Per-branch versions would need insula changes.

Measured with build/sonda/sonda_quies.c (scratch) on a `sqlite3
.backup` COPY of Fran's paginae-proba.volumen, fake tmux first on PATH
(copied terminal panes must not attach to the real sessions): idle
pulse 675 us -> 213 us (-68%); insula versio +0 over 600 idle pulses,
so nothing in the real layout writes state at idle.
