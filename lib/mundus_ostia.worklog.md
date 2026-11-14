# mundus_ostia worklog

## 2026-11-13 — born (ostium-plan T1)

Doorways from the 3D level into a scene. Spec:
`project-specs/ostium-spec.md`.

**Why an act and not a node in the level.** The first idea was a
tagged invisible node inside the level GLB. A `StatuaNodus` has no
name and its signs are three fixed words, so that meant touching the
core model struct, the GLB reader, the writer and the joiner for data
that is not geometry. The act `ostia-posita` carries the whole list
for one level; the newest readable act for that level wins. An act
that cannot be read is skipped (the earlier list stays), so one bad
write never strips a level of its doorways.

**Numbers are integer thousandths**, rounded to nearest. The same
sheet gives the same bytes, and "same list, nothing written" is an
exact comparison.

**Reachable is a circle, tested with the body's radius.** A doorway
is refused when no cell where the body can stand has its centre
within the doorway's radius. The bounding box of cells is walked and
the circle test applied inside it; the "water cross" test section
exists because a box-only test passes every simpler fixture (the
axis cell is in the box exactly when it is in the circle).

**The spot itself need not be walkable** (a door sits in a wall).
Only "inside the walk table at all" is required of the spot.

**Tool.** `bin/mundus condere` checks doorways against the level as
read back from its GLB (props already block), and writes level and
doorways in one transaction. Doorways changed alone make the level
`mutata`.

**Generated test source list:** `tools/compile_tests_fontes_generare.sh`
lists files git knows; a new lib file needs `git add -N` first or
the test fails at link.

## 2026-11-14 — ostium T4: mundus_ostia_locus_proximus

The static "is there a standable cell within the radius" became the
public `mundus_ostia_locus_proximus`, which also returns the nearest
such cell (ties: earlier in table order, z then x). The world pane
uses it to place the character on leaving a scene; `mundus_ostia_probare`
uses it for the build check, so a doorway that passes the build always
has a place to stand. The radius is a circle, measured to cell centres.
