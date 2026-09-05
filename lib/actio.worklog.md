# actio.worklog.md

## 2026-09-05 — natus (ludus T9)

Name → (fn, ctx) registry for the actions the markup NAMES. Three
parallel Xars (names interned, function pointers, contexts) indexed
together; duplicate names refused. Both-ways resolution for lint
L10 over a componens tree: `actio_non_registratae` = names the tree
references with no handler (an error), `actio_non_relatae` =
handlers no componens references (a warning). Names are collected
once each, so a name referenced by many componentia reports once.

Deviations from the draft:

- Empty titles are refused. `chorda_internare_ex_literis` returns
  NIHIL for "" and the draft dereferenced it; the probatio pins the
  refusal. A NIHIL handler is refused too.
- Every entry point guards its pointers; the resolution functions
  return NIHIL for a NIHIL registry and an empty Xar for a NIHIL
  tree.
- Membership test factored into one helper used by the collector
  and the unreferenced-handler scan.

The `ActioFn` signature takes the repository, the motus, the
destinatio, the matching componens (which may be an ancestor of the
target), the event, and the context; VERUM = consumed, the ascent
stops. The probatio calls a registered handler through the pointer
it got back, with NIHIL for everything but the context.

Gate: `probatio_actio` (22). Planted fault: duplicate check made
unreachable — red at the `duplex recusatur` assertion, green on
revert. Examen ACCIPE; formator 0 divergences on birth.
