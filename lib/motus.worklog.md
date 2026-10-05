# motus.worklog.md

## 2026-09-04 — natus (ludus T7)

The MOBILIS genus as a struct, not an island: `mutare_motum` is its
one gate (mutator, timestamp, `sordida`), `motus_quies` judges quiet
from event time (`sordida` AND `quies_ms` elapsed since the last
mutation; nothing dirty is never quiet), and `motus_effundere` is the
single `<quies/>` seat that writes pan and zoom into the ephemera
island through `mutare_ephemera`. The pending stroke never reaches
the island: it survives a flush and leaves through a durabilis write
in P3.

Deviations from the draft:

- Effusio uses `insula_attributum_ponere`, not `stml_attributum_addere`
  — the draft's second flush would have doubled every attribute. The
  probatio flushes twice and pins the attribute count at three.
- Header guard is `MOTUS_H` (the draft still said `KINETICA_H`, the
  unsealed name); `mutare_motum` called `fn(k, ctx)` with no `k`.
- `Punctum` is unsigned (`i32 x, y`), so pan cannot go negative yet.
  The probatio uses positive pan only; signed screen coordinates stay
  the P3 decision the plan names.
- An empty `captura` is `{ZEPHYRUM, NIHIL}` (what `chorda_vacua`
  accepts), never an allocated empty string.
- Guards on `motus`, `fn`, `repo`.

Gate: `probatio_motus` (35). Planted fault: pan_x flushed through
`stml_attributum_addere` — red at the second flush (`capere` returns
the first of the doubled attributes, the stale "40"), green on
revert. `DL` is not among latina.h's numerals (`M + D + L` is).
`<quies/>` anchor comment above
`motus_effundere` for lint L4, as `<purus/>` above `derivare`.

## 2026-10-05 — the gesture slot (scriba-plan S1a)

Motus was pictor-shaped (pending stroke points, pan, zoom). scriba
needs the same tier for an insert in progress, saved DEBOUNCED (Fran:
~1 s without a key, its own interval) and never lost on quit. Added
`MotusGestus gestus` = {status (opaque app state), effusor, ctx,
quies_ms, tempus, sordidus}, its own gate `mutare_gestum`, its own
`motus_gestus_quies` and `motus_gestum_effundere` (writer "gestus"
while the effusor runs; a refusing effusor leaves it dirty for the
next try). Separate from pan/zoom on purpose: a gesture change does
not dirty pan/zoom and vice versa, and the two quiet intervals differ.

First draft had the six fields flat in Motus; the struct could not be
aligned within 72 columns beside `tempus_ultimae_mutationis` - the
grouping was the better design anyway (one name, `motus->gestus.*`).

Plants (all compiling, re-run after the regrouping): the gesture using
the dispatcher's 300 ms; a refusal marked clean; a gesture change
dirtying pan/zoom - plus the dispatcher's two (below, dispensator
worklog). pictor does not use the slot (its stroke still lives in
`ictus_pendens`); moving it is possible later, not needed.

## 2026-10-05 — the gesture flush restores the writer (scriba S1c)

`motus_gestum_effundere` set the writer to "gestus" and then to
ANONYMOUS. From the dispatcher that is fine; from INSIDE an action
(scriba's Esc and `dd` flush at once) every later state write would be
anonymous and owned attributes would refuse it silently. It now
restores the previous writer. Red first (probatio_motus).
