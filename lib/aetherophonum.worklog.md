# aetherophonum worklog

## 2026-10-22 - S5c: the theremin pane

A vicus pane kind (`$theremin`). Root "aetherophonum" (PARTES_TABULA,
draws everything) + child "campus" (the playing area above the status
line; same actio). x -> note linearly in semitones across `octavae`
octaves from `nota_infima`; y -> gain 1 (top) .. 0 (bottom). Sounds only
while the button is held (Fran 10-22): press = capture + `incipere`, each
motion = `ponere` with a short glide (XV ms; XL ms when snapping, so a
semitone step does not click), release or Esc = `dimittere`.

Seams:
- The pane knows no sound: it gets an `AetherophoniPorta` (incipere /
  ponere / dimittere) from the host. vicus_applicatio's wrappers check the
  MASTER mute and the port at call time (a port set after mounting still
  works); the notification mute does not silence an instrument.
- Persistent preferences live in the pane's branch (manifest
  apps/aetherophonum/organum.stml - EMBEDDED: run tools/organa_infigere.sh
  after editing). The voice itself (handle, current note) is kinetic.
- First click: vicus's first click on an unfocused pane only focuses
  unless the facies has `ictus_primus`; the pane provides
  `aetherophonum_ictus_primus` so the theremin sounds on the first press
  (only inside "campus" - the status line just focuses).

Test lessons (21 plants, 6 green at first): in the standalone harness the
pane IS the whole tree, so "drag outside" never tested capture - moved to
the vicus suite (drag over the left scriba pane); `$theremin` opens the
pane focused, so the first-click hook was never exercised until the test
focused scriba first; the hover guard is visible only in the montatio's
current note. vicus opens panes on the NEXT pulse (pending requests) - a
test must pulse after clicking `$theremin`. Pane-local fines are not
screen coordinates: screen = local - destinatio_ad_locale(k, origin).
Forbidden identifier bit again: `nomen` (= typedef) as a local buffer.
