# OSTIUM — ianua inter mundum et scaenam

*Born 2026-11-13, from Fran's question after the graphis plan
finished: "do we have the ability to trigger the view from the mundus
3d view yet? ... a floating "[E]" tooltip ... press e to enter when
the character is in an area they can interact with". Answer then: no;
`$world` and `$scene` do not know each other, and a scene exit named
`mundus` shows its text only (`include/scaena_latus.h:37`). This
closes the ledger question `01M7BBC3T43PAQFSSAAEEPQK6M`.*

**Status: AGREED** (design agreed by Fran in conversation 2026-11-13;
not built). Plan: `project-specs/ostium-plan.md`.

## I. Intent

The character walks up to a place in the 3D level, a small `[E]`
appears over it, E is pressed, and the same pane now shows a still
scene. The scene's way out puts the character back at that place.
One world with several ways of being shown ("mini-engines"), not two
panes that hand off to each other.

Goals: a doorway is one line in the level sheet; the hut in the
clearing leads to the hut scene already in Fran's volume.

Non-goals (this slice): any transition effect; doorways from a scene
into a *different* level; things in the 3D level that are examined
without entering (signs, chests); mobs. Persisting the character's
position while walking outside is a separate known gap and stays one.

Anti-goal: a second copy of the scene's drawing and input code in the
world pane.

## II. Decisions

1. **(Fran)** Entering turns the `$world` pane into the scene; the
   scene is part of the world. `$scene` stays as the browser of every
   view.
2. **(Fran)** The prompt is a floating `[E]` over the doorway while
   the character is near; E enters. Clicking the prompt does the same
   (house rule: mouse too).
3. **The level sheet** gets
   `<ostium locus="x z" radius="1.5" scaena="hut-door"/>`
   (`radius` optional, default 1.5, allowed 0.5 .. 8; `scaena` a
   scene view title by the rule of `scaena_titulus_validus`). Up to
   64 per level.
4. **Where doorways live in the volume — changed from what was said
   in conversation.** I said "inside the built level, as a tagged
   invisible node". Reading the code: a `StatuaNodus` has no name
   (`include/statua.h:60-67`) and its signs are three fixed words
   (`lib/statua_glb.c:961-965`), so that needs a new field in the
   core model struct, its GLB reader, its writer and the joiner, for
   data that is not geometry. Instead: an act of its own in the
   volume, `ostia-posita`, naming the level and listing its doorways
   (numbers as integer thousandths, so the bytes do not depend on
   float printing). The newest act for a level wins; an empty list
   clears. `bin/mundus condere` writes the level and its doorways in
   one transaction, so they change together, and an older vicus
   ignores the act. Same shape as the scene store
   (`include/scaena_repositorium.h`).
5. **Refused at build**, with the sheet's line: a doorway outside the
   level; a doorway no walkable cell lies within `radius` of (the
   prompt could never appear); a bad scene title; a bad radius; two
   doorways at the same spot.
   **Not refused:** a scene title that is not in the volume. Levels
   and scenes are built by separate tools; the pane handles it (9).
6. **Near** means the character's centre is within `radius` of the
   spot on the ground plane. With several in range, the nearest; a
   tie goes to the earlier one in the sheet.
7. **The prompt** is the text `[E]` on a small flat box, centred
   above the doorway spot: the spot at ground height plus 2.5 units
   (the character is 2 tall), projected with the world's fixed view.
   It stays fixed in the world as the character moves. If that point
   is outside the picture the prompt is clamped to the picture's
   edge, so a doorway in range always has a visible prompt.
8. **Inside**, the pane shows what `$scene` shows for that view:
   picture, description, things to touch with mouse or `j` `k` Enter,
   exits between views. Differences: no view arrows and no `[` `]`
   (views are reached by exits only); the header shows the view title
   where the character's name was; W A S D and `g` do nothing; the
   world does not step.
9. **A doorway whose scene is missing or unreadable** still enters.
   The pane says "This scene cannot be shown:" with the reason and
   "E: go back". E (or a click on that line) leaves.
10. **Leaving:** an exit named `mundus` returns to the 3D view. The
    character stands at the walkable cell nearest the doorway
    entered, facing as before. In `$scene` itself a `mundus` exit
    stays text only.
11. **Memory** (persistent pane state, `apps/mundus/organum.stml`):
    `prospectus` (view shown; empty = outside), `tactum` (as in the
    scene pane), `ostium` (the spot of the doorway entered, so
    leaving works after a restart and after the level is rebuilt).
    If that spot is no longer usable, leaving puts the character at
    the level's start.
12. **One scene implementation.** The scene pane's drawing, input and
    state handling get a hosted form that another pane can embed;
    `$scene` keeps its behaviour, tests and plants unchanged.

## III. Tests

Every behaviour above gets a test proved red by a compiling plant
(house rule). The scene pane's existing 9 sections and 17 plants are
the guard for decision 12: they must pass untouched after the hosted
form is cut out, and the plants must still go red.

Headless screenshots of the world pane (outside with prompt, inside,
missing scene) go to Fran for the look; the real window is his to
open.

## AUDIENDA (not verified)

- Whether the prompt needs an outline or shadow to read against light
  ground has not been tried; decision 7's flat box is a first guess.
- The hut's door position in the clearing has not been measured; the
  hut is not a `<res>` of `apps/mundus/loci/clearing.stml` (it is in
  the base model or absent). Task 5 finds it on the map sheet.
- No measurement of what entering costs in time (reading a view blob
  and making its picture); the scene pane does the same work today
  without complaint.
