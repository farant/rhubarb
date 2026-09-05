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
