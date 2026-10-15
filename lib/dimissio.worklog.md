# dimissio worklog

## 2026-10-10 — S5c: the popover (dismissal) system (status-plan S5c)

First INPUT-MEDIATING system (census XIII): persistence and references
react after changes; dimissio sits in the event path. Dispatcher gained
one hook pair (`dispensator_medium_ponere`): `ante` before `mittere`
(VERUM = consumed, no action runs), `post` after the action that
consumed, with that component (`mittere` now reports it). Derived
events (focus, at the threshold) bypass the hooks - dismissal is about
raw input.

Rules (Fran's decisions D1-D7, census XIII):
- Esc: closes the active branch's open popovers (motus.spatium),
  consumed - EXCEPT while a pointer gesture is in flight
  (motus.captura): see below.
- mouse-down: for each open popover field, a click whose ancestor chain
  has a DIALOGUS component of that branch (inside) or the field's
  owner action of that branch (the opener - it toggles/switches
  itself) leaves it; otherwise close; swallowed if the click is in the
  same widget, passed through if in another.
- post: an action that consumed on a `claudit` component closes its
  branch's popovers (palette options are marked; layer-panel rows are
  not, so the panel stays open while you work in it).
- popover fields are now OWNED: scriptor (opener) + systemata="dimissio"
  (S5a); nothing else may write them.
Assumption made explicit: one popover field per widget (DIALOGUS =
inside); the state<->view link is an open thread.

Esc ordering found by the regression net: probatio_pictor_strata_ui
"Esc while dragging a layer cancels the drag" broke - D4 ("popovers
take Esc first") was wrong when the transient thing is INSIDE the
popover. Rule: an in-flight gesture (captura) is inner - Esc goes to
it; the next Esc closes the popover. That is the escape stack of census
XIII arriving early, in its smallest form.

pictor: closing code removed from penicillus/aspergillum/linea strokes,
tool/colour/pattern/size picks and the Esc handler (palette_aperta
helper deleted); palette.aperire keeps its toggle/switch (owner).
Standalone pictor and vicus bind the system to their dispatchers
(vicus: created lazily by the first pictor pane, bound after the
dispatcher exists).

Plants: 10, all caught. P4 (no opener exemption) survived the TOY
suite - in a single-kind toy, "system closes" and "opener toggles"
look identical; pictor's palette-SWITCH test (colour-1 open, click
colour-2 square -> colour-2 directly) catches it. A toy can miss a rule
whose value only shows with two kinds of the same thing.

## 2026-10-14 - `post` closes on a PRESS only (calendar menu bug)

Fran: the calendar's right-click menu "disappears when I move the
mouse". Cause: the dispatcher calls `post` after EVERY consumed event,
and `post` closed the popover whenever the actor had `claudit`. The
calendar menu (elenchus, M2c) consumes EVENTUS_MUS_MOTUS over an item
to move the highlight - so hovering over any enabled item (which
carries `claudit`) "succeeded" and dimissio closed the menu. Now only
EVENTUS_MUS_DEPRESSUS or EVENTUS_CLAVIS_DEPRESSUS on a `claudit` actor
closes. pictor's palette closes on a click as before (22/22).
Missed earlier because the calendar's own test world has no dimissio
and the vicus menu test never hovered. Tests: probatio_dimissio
`claudit` (the fixture option now consumes motion as a highlight;
motion keeps the palette open) and probatio_vicus_kalendarium
`elenchus` (hover keeps the menu open with "elenchus_electa"=0, then
the click closes). Plant (old behaviour) -> red in the vicus suite.
