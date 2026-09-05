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
