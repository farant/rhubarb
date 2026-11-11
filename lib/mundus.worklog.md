# mundus worklog

## 2026-11-10 - bodies and the step (mundus-plan T5)

A body has a place, a facing and an intent; `mundus_gradi` advances
every body by a slice of time. Nothing here knows about keys, pixels
or a clock, so the suite runs on numbers alone, on the hand-made
level in `probationes/mundus_locus_fictus.h` (new, shared; the T4
suite still carries its own copy of the same level and can move over
when it is next touched).

**Movement in pieces.** A step's travel is cut into pieces of at most
`MUNDUS_PASSUS` (1/8 unit, half a map cell). Each piece is tried
whole, then along x only, then along z only. That is what makes a
body slide along a wall and what keeps a long step from jumping over
a thin obstacle. The time step itself is capped at 0.1 s, so a stall
of seconds moves a body 0.4 units, not 20.

**Found while testing: the wall was bumpy.** Sliding along the cliff
base failed: the body stuck. `mundus_solum_stare` measured the body's
footprint from its exact position against cell centres, so whether a
wall cell fell inside the radius changed as the body moved a fraction
of a cell ALONG the wall. A straight wall became a row of teeth. Fix
(in `lib/mundus_solum.c`): measure the footprint from the centre of
the body's own cell. The verdict is then the same anywhere inside a
cell and a straight wall is straight. Cost: positions are judged on a
quarter-unit grid; nobody will see that.

**Facing.** 0 degrees looks along +z and grows toward +x (atan2 of x
over z). It turns the short way round at 720 degrees a second and
keeps turning while the body is blocked, so pushing against a wall
still shows which way the player is pushing.

**`tempus_motus`** counts seconds of unbroken movement and is the
clock for the walk clip; it returns to 0 when the body stops, so the
walk always starts from its first frame.

Plants, six red: no normalizing of the intent; no cap on the time
step; no single-axis retry; turning the long way round (this one was
green at first: the test only turned 350 -> 10, which never has a raw
difference above 180; the case 10 -> 350 was added); a blocked body
that does not turn; footprint measured from the point.
