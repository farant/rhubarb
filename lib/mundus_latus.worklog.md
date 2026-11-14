# mundus_latus worklog

## 2026-11-10 - the pane (mundus-plan T9)

Modelled on `lib/statuae.c` (mount, tick, `mora`, image source, header
row, volume poll). What differs:

**Held keys.** W A S D need press AND release, so they are handled
directly in `agere` before the command table (which is press-only and
carries `[` and `]`). Four flags; the intent is rebuilt from them on
every tick, screen-relative, turned into world x/z by the view's yaw
(right = (cos g, sin g), up = (sin g, -cos g); at 45 degrees W is
+x -z). A key with Cmd, Ctrl or Alt is not a walking key; Shift is
allowed.

**Releases that never come.** `mundus_latus_focus_amissus` (wired to
`VicusFacies.focus_amissus`, T1) clears the flags. The plan's extra
guard ("no key event for a second") was dropped: T1 showed releases
are delivered, and a timer would stop a character whose key is
legitimately held on a machine where the OS does not send repeats.

**The first step after standing still.** A standing pane is ticked
once a second. A key pressed 700 ms after the last tick would have
made the first step cover 700 ms, capped to 100 ms: a visible lurch
of 0.4 units. On the press that starts movement, `hora_gradus` is set
to the press's own time. (The first version of the test did not let
time pass between the last tick and the press, so the plant that
removes this line was green; the test now advances the clock.)

**Only the tick advances the world.** The compositor refreshes the
frame (an event may have changed the character) but never steps the
world, so composing twice cannot move a body twice.

**Frame key** = character digest, body position (mm), facing (tenths
of a degree), moving flag, walk time (ms), size. A standing character
yields the same key every tick and costs nothing.

**Outside the level** the frame is transparent (index 0xFF from
`mundus_quadrum_pingere` becomes alpha 0), and the theme background
drawn under the picture shows through, as in `$models`.

**Default character:** the status field `persona`; else the first
title containing "mushroom" (Fran: "the mushroom guy is best to
start"); else the first.

**Place:** the first entry of kind LOCUS. Its arena also holds the
walk map and the ground regions, so replacing the place frees them
all at once. A place whose map has no standable cell (any character
model added with `-locus`) says "nowhere to walk".

Not done: a pinned picture of the start view (plan T9 "imago"); the
headless screenshot in T10 is where Fran first sees the pane. Idle
clips are still unexercised (no model has one).

Plants, six red: releases ignored; focus loss does not clear the
keys; first step measured from the last tick; screen-up inverted;
modifiers not checked; default character not the mushroom man.

## 2026-11-10 - ground dither toggle

`g`, or the button at the right end of the header, switches the
ground between Atkinson and exemplaria. Status field `humus`
("exemplaria"; absent = Atkinson), persistent like `persona`. The
ground now lives in its own arena (`piscina_humi`); `humum_curare`
empties it and makes a new humus when the place is new or the mode
differs from the status, and clears the frame key. The character
stays where it is.

Test section "humus": the frame changes, the character does not move,
the new ground has made the same number of regions (old ones were
dropped, not piled up), and switching back gives the first frame
again byte for byte. Plants, four red (mode ignored in the picture
library; mode change does not remake the ground; `g` never returns to
Atkinson; the button does nothing).

Headless picture: `build/statuae/vicus_mundus_humus_exemplaria.png`.

## 2026-11-13 — doorway prompt (ostium-plan T3)

The pane reads the level's doorways (`mundus_ostia.h`) and shows
`[E]` over the nearest one in range.

**Doorways are re-read with the indices**, in `indices_relegere`, on
every volume sequence change - not in `locum_curare`, which only
runs when the level's seal changes. `bin/mundus condere` can change
doorways and leave the level's bytes alone. The list lives in
`piscina_indicis`, so a re-read can never leave a stale pointer.

**One function says where the prompt is.** `mundus_latus_signum`
computes the box from the present state; the figure draws it and the
compositor puts the click area `ostium` on the same rectangle, so
they cannot disagree. The pulse alone decides WHICH doorway
(`ostium_curare`, after the world step); the guard `m->positum` in
its loop matters: a level with nowhere to walk leaves the body at
the origin, and a doorway near (0, 0) would otherwise count as near.

**Height.** The prompt floats 2.5 above the ground AT THE DOORWAY
(`mundus_solum_altitudo`), which can differ from the character's
(ledge). A door in a wall has no walkable ground under it; then the
character's height is used.

**Test lesson.** `planum.glb` is not flat. A test that reused one
ground height for four points saw the prompt 160 pixels off. Each
point asks the walk table for its own height. Also: the fixed view
is inclined, so 8 units "up" or "down" the screen stay inside a
450-pixel pane; the top and bottom clamps are tested in a pane 100
high.

Screen directions in world terms (for the next person writing such
a test): right = +x +z, left = -x -z, up = +x -z, down = -x +z.
