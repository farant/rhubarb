# destinatio.worklog.md

## 2026-09-05 — natus (ludus T8)

The default targeting strategy over the LOGICAL tree, never the
mandata: children walked last-to-first (painter's order is z-order),
`sectio` prunes a subtree at its parent's fines, translatio and
scala inverted per level, the punctum locale written in the target's
own space. Capture in `Motus` overrides geometry for positioned mouse
events; keys route to focus; the ascensus is target-first, root-last.
Tab order is a depth-first collection of focusables with wrap.
Polygon and mask predicates are P3 pulls (rect and proprius only;
proprius means the punctum locale is authoritative).

Deviations from the draft, all forced:

- The draft's "outside everything" point (190,90) was inside the
  pannus, which spans x 70–200. The probatio uses (65,90) instead,
  and the pannus is 70 tall so tabula's screen extent (y 50–90) is
  genuinely clipped: (125,80) resolves to radix with sectio and to
  tabula without it — the first test that shows sectio doing work.
- `EVENTUS_MUS_ROTULA` is routed to focus, not geometry. `Eventus`
  carries wheel deltas in a union member that overlays `datum.mus`;
  reading `mus.x` on a wheel event reinterprets float bits. A
  positional wheel needs the platform layer to carry x/y beside the
  deltas — a fenestra pull, not a strategy decision.
- Every coordinate in the house is unsigned `i32`, so the child-space
  point is computed in `s32` and a negative axis skips the child walk
  outright (no child has negative fines). The draft cast the
  negative back to `i32` and relied on `fines_continet` failing on
  the wrapped value. Scala 0 is treated as 1.
- Unknown capture id falls back to the geometric target (probatio
  pins it); unknown focus falls back to the root; empty `chorda`s are
  `{0, NIHIL}`, never allocated.
- Rename leftovers fixed: guard `DESTINATIO_H`, `&motus` not `&k`,
  `destinatum` not `destinatio` as the local.

Focus order: proximum from an unknown or empty focus is the first
focusable, praecedens the last; both wrap.

Gate: `probatio_destinatio` (40). Planted fault: children walked
first-to-last — the original geometry had no overlapping siblings,
so that plant would have stayed green; a `b3` overlapping `b1` was
added first, then the plant went red at the z-order assertion and
green on revert. Examen ACCIPE; formator one 73-column comment on
birth, split through `Editio`.
