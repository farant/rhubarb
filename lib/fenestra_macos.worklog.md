
## 2026-07-20 — B' (vitrea riders): expectare + menu + type-15

Blocking pump shipped as DELEGATE-BY-REPOST, simpler than the spec'd
refactor: expectare blocks once on nextEventMatchingMask with a
real deadline, and if an event arrives, reposts it atStart and
calls perscrutari — so there is exactly ONE translation path and
the resize-detection function-static never needed hoisting (the
[EXP-F] concern dissolved structurally instead of being solved).
Type-15 (ApplicationDefined) gets a `perge` case in the
translator: no Eventus, no sendEvent — the vitrea wake contract.
Default menu (App/Exire + Emendare with standard selectors)
installs once in the si(!NSApp) block; Latin item titles are free
because selectors do the routing, not names.

DEBUG FIND worth keeping: the expectare smoke first measured
0.078s for five 200ms waits and looked like a broken block. A
bare-AppKit twin with event-type printing showed clean full
timeouts — the difference was ENVIRONMENTAL (cursor movement over
the window streams mouse-moved events; real events waking the
pump is correct behavior, the loop just re-enters). With a still
mouse: 1.004s/1.0. Lesson: a blocking-pump timing smoke is
measuring the desktop too — diagnose with an event-type-printing
twin before suspecting the pump.

## 2026-07-23 — the permanently vanishing mouse cursor (NSCursor refcount leak)

Symptom (found by Fran in the forum app): cursor hides on typing
and NEVER returns on movement; maximizing (= macOS fullscreen)
makes it vanish entirely; the brokenness persists after leaving
fullscreen.

Cause: game-era policy baked into the platform layer.
windowWillEnterFullScreen called [NSCursor hide] implicitly, and
the mouse-moved handler RE-hid on every move while fullscreen
("macOS potest ostendere cursor si mus movetur violenter").
[NSCursor hide] is REFCOUNTED — N moves in fullscreen = hide
count N+1, while windowDidExitFullScreen unhides exactly once.
Net positive count = cursor hidden app-wide forever; the system's
type-to-hide auto-unhide-on-move can't beat a positive count.

Fix: both implicit hide sites removed. plena_visio flag stays
(concha reads it), explicit fenestra_occultare/ostendere_cursorem
API stays for genuinely immersive apps, and the guarded unhide on
fullscreen-exit stays as a sane default for explicit hiders.
Lesson: never put POLICY (cursor aesthetics) in the PLATFORM
layer's implicit paths — the explicit API already existed; the
implicit path is where the refcount leak hid for months, and it
only surfaced when a webview app met the green button.


## 2026-09-14 — menu applicationis: fenestra_menu_addere (plan 8, T1)

The first app-menu seam (briar-spec §4.9). `fenestra_menu_addere(fenestra,
titulus, clavis, signum)` inserts an item into the app menu above
`Exire`, with ONE separator between the added items and Exire (a second
call reuses it: `[a][b][sep][Exire]`). A click is not a C callback: a
small ObjC target (`FenestraMenuScopus`, never released — NSMenuItem's
target is assign and items live as long as the app) builds
`EVENTUS_MENU` with `datum.menu.signum = tag` and pushes it through
`impellere_eventum` into the queue of the fenestra that added the item.
The pump (`fenestra_perscrutari_eventus`) calls `[NSApp sendEvent:]` for
every event, so a click and the key equivalent both fire the action
DURING the pump; the event is in the ring before the caller drains it.

- The `Cmd+ Ctrl+ Shift+ Alt+ Opt+` loop moved out of
  `fenestra_claviarius` into `_modificantes_legere(&p)` — one parser for
  injected keys and menu equivalents, behaviour unchanged.
- Shift + a letter is written as the UPPERCASE key equivalent with Shift
  removed from the mask: the `@"Z"` form of `Iterare`, the one injected
  keys were measured to match (2026-08-15). Live proof: plan 8 T2 by
  hand, then fumus XIV.
- `EVENTUS_MENU` appended last; `lib/eventus_stml.c` title "menu" (table
  order = enumeration order). Switch census (files with `casus EVENTUS_`,
  count of `commutatio (...genus)` vs `ordinarius`): tools/lector_eventuum.c,
  lib/persistentia_nuntium.c, lib/entitas_repositorium_impl.c (those two
  and the tool switch partly over the ENTITY event enum),
  lib/pictor_actiones.c, and eight probationes demos — every switch has
  an `ordinarius`, so no `-Wswitch` break (root suite 166/166). The
  serializer writes a menu event as its bare title, no signum attribute;
  nothing replays menu events.
- Gate: probatio_eventus_stml +2 assertions (18), born red 2/2; plant
  `"menu"` -> `"menus"` gave exactly 2 reds; restored.
- `menu` is IGNOTUM to the lexicon -> `ignotum-permissum` glossary line,
  as `plist` got.

## 2026-09-29 — bare letters and digits as native key presses (lapide FR-024)

`fenestra_claviarius` used to refuse a letter without a modifier, on the
reasoning that TEXT goes through `scribere` (layout-unaware). The lapide
tester had a real case the refusal blocked: single-letter shortcuts on
`document` ('a', 'n', '1'-'9') with nothing focused, where `premere-
textum` fires no keydown. A single key is not text - it is what a
finger does - so a bare letter or digit is now one native press
(keydown + keyup). Strings still go through `scribere`. A bare capital
implies Shift (a real keyboard's behaviour); Shift + digit keeps the
digit (the symbol, '!' etc., is a layout fact, not guessed).

Digits needed codes: `CODICES_NUMERORUM` (kVK_ANSI_0..9 = 29 18 19 20
21 23 22 26 28 25 - NOT sequential). `tools/claves_codices_probare.sh`
now checks all XXXVI against Carbon; a planted 5<->6 swap is named.

Verified only through a real window: briar `fumus -agere` XIV installs
a keydown listener via `manus aestimare`, sends `clavis a` and `clavis
7`, reads back. Planted (old refusal): red at `manus clavis a`. First
green attempt failed on MY check - `aestimare` prints JSON, so the value
is `"a,7"` with quotes; the keys had arrived (verified by hand: e.key a,
7; activeElement BODY, i.e. nothing focused - the tester's exact case).

Found on the way: `bin/manus` was stale since 2026-08-17 - it is built
by `tools/manus_struere.sh`, but the fumus's own hint said
`./compile_tools.sh manus_instrumentum`, which builds a DIFFERENT file
(bin/manus_instrumentum). Hint fixed. The tester's report shows the same
stale binary date.


## 2026-10-07 — focus and paste events (aemulator D6c)

`windowDidBecomeKey` / `windowDidResignKey` were empty stubs; they now
queue EVENTUS_FOCUS / EVENTUS_DEFOCUS (the window becomes key at
creation, so a FOCUS is almost always the first event). Cmd-V: the
"Glutinare" menu item has no target, so AppKit walks the responder
chain - our view is never made first responder (keys come through the
polling loop, not responders), so the window, then its DELEGATE: the
delegate implements `paste:`, reads the general pasteboard's string and
queues EVENTUS_TEXTUS origin GLUTINATA. Payload limit: the event queue's
text table is 64 KiB per read - a longer paste is truncated and flagged
(not split yet). Every fenestra app now receives these; focus lands on
tree roots (ignored unless an action is there), a paste reaches the
focus as text. Fran's run: focus reports seen in terminale.

## 2026-10-07 — drag events carry their button

Fran (terminale): dragging tmux pane dividers did nothing, and Claude
Code's split divider highlighted but would not drag. `_murem_implere`
never set `botton`, so every Left/Right/OtherMouseDragged arrived as a
plain motion: under ?1002 (tmux) motion without a button is not
reported at all, under ?1003 (Claude Code) it reads as hovering. The
event spec (D1) gives motion a button and the other sources already set
it (interpres from SGR drag codes, manus_ludus); now the window does
too, from the NSEvent type. derivare takes its button from the PRESS,
so drag recognition in other apps is unaffected. Fran verified both
cases; probatio_terminale pins the chain below the window (press,
button-held motion, release -> SGR drag report).
