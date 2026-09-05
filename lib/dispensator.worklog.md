# dispensator.worklog.md

## 2026-09-05 — natus (ludus T10)

The loop: quiet seat, derive, target, dispatch up the ascensus,
recompose. Focus and its stack live in the ephemera island as the
attributes `focus` and `focus_acervus`, so a restore restores them
(the probatio proves focus survives `insula_restituere`). Hover
(`mus_intravit`/`mus_exiit`), `focus_captus`/`focus_amissus`, and
`focus_petitus` are derived HERE, not in `derivare`, because they
need the targeting result. Staleness: recompose after every
dispatched event, including pulses, via ping-pong piscinas; frame 0
is composed at creation. Focus-petitus in P2 goes to the root; the
owner-adjusts refinement is P3. The five derived genera joined the
`fenestra.h` enum after `EVENTUS_MUS_DUPLEX`, in the order the
eventus_stml title table already carried.

Deviations from the draft:

- Every write of `focus`/`focus_acervus` goes through
  `insula_attributum_ponere`; the draft's `stml_attributum_addere`
  would have appended a new `focus` attribute on every click and
  `dispensator_focus` would have read the first one forever.
- An empty id is never looked up: `componens_invenire_per_id` with
  an empty chorda matches an anonymous node (the first frame's root
  before any focus exists would have received `mus_exiit`).
- The toy's attribute names are `character[]` statics, not literals
  (`-Wwrite-strings` + `-Wcast-qual`); its mutators use
  `insula_attributum_ponere` so the second click reads "2", not a
  duplicate "1". `toy_ictus` refers to `motus`, not the draft's `k`.
- The toy header's functions are `interior`; under `-Wall` a probatio
  that includes it must call BOTH `toy_componere` and
  `toy_registrare`, which reach everything else. Documented in the
  header.
- `destinatio_ex_componente` tolerates NIHIL; `mittere` tolerates a
  NIHIL ascensus; creation checks every piscina it makes.

Pythonica note for the enum edit: a token anchor starts at the first
token, so a `novus` that begins with indentation doubles it (the
enum's first line came back at 8 spaces; repaired byte-exact, verdict
`cosmetica`). Lead `novus` with the token, not with spaces.

Gate: `probatio_dispensator` (41). Two planted faults: the quiet
seat flushing into a NIHIL repository — red at the `pan_x` read
after the second pulse; the staleness recompose removed — red at
`toy.compositiones > n0`. Both green on revert. Regressions after
the enum change: destinatio, eventus_stml, derivare,
fenestra_tempus all green; fenestra.h keeps its pre-existing six
long-line divergences (delta 0). Examen ACCIPE on all seven files.


## 2026-09-05 — limen (ludus P3 T5)

Derived and addressed events no longer deliver mid-dispatch. Every
`mittere_ad` (hover enter/exit, focus captured/lost, focus-petitus)
enqueues a `Differendum` (id, genus, tempus) through the public
`dispensator_addressare`, the `put` seat of brainstorm §XVI §1. The
queue is drained at the LIMEN: after the current event's dispatch
and the staleness recompose, against the NEW tree, and the drain
recomposes once more. Events addressed during delivery wait for the
next boundary — the drain works on a copy taken in the per-event
scratch arena, so `addressare` from inside a handler is safe.
Focus-petitus is two boundaries: the request goes to the root at
the first, and the "still absent, clear it" check runs after that
drain.

Why: brainstorm §XVI §3–4 — a derived event should see the world
its cause produced, not the world before it; that is the one piece
of frame-level double buffering worth having now. The toy's root
handler counts deliveries and records the composition count at
delivery time; the probatio asserts it exceeds the count before the
click. The plant that restored synchronous delivery went red there.

Cost: an event that produces derived events composes twice. The toy
replay's "one composition per event" assertion became "at least
one". Measure in T10 with the rest of the frame.
