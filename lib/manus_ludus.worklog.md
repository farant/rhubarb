# manus_ludus.worklog.md

## 2026-09-05 — natus (ludus T11)

The in-process hand over a dispensator: the same verbs as manus.h
(premere, clavem, existit, focus) but synchronous, so there is no
wait and no clock. Selectors run over the LOGICAL tree — `#id`,
`[partes=x]`, `[actio=x]`, `[titulus=x]` — never over mandata.
`manus_ludus_ad_schirmum` is the exact inverse of the targeting
transform in destinatio.c: local + fines, then per ancestor
`* scala + fines + translatio`. Time advances by one gradus (50 ms)
after every event the hand sends and by `exspectare`, which also
pulses the dispensator so the quiet seat can fire. Unifying this
under manus.h as a second transport is a named deferral (manus.h is
vitrea/imperium-shaped, 4,070 lines).

Deviations from the draft:

- A malformed selector (no `#`/`[`, missing `]`, unknown attribute)
  names its cause and the probatio pins three such cases; the draft
  only tested a well-formed selector that matched nothing.
- `premere` on a selector that matches nothing keeps the selector's
  own cause if one was set, so the message says which stage failed.
- `trahere` refuses `n < I` or NIHIL puncta with a cause; screen
  mapping accumulates in `s32` and casts once.
- Empty `causa` is `{0, NIHIL}`; every entry point guards `m`.
- The probatio also covers `premere_ad` on a non-focusable root
  (focus unchanged), `movere` deriving `super`, shift-Tab going
  backwards, and a short `exspectare` that must NOT flush.
- The toy has no nested transforms, so the probatio builds a
  standalone pannus (translatio 0,50; scala II) with a child and
  pins its screen point; without it the ancestor walk had no guard.

Gate: `probatio_manus_ludus` (48). Two planted faults: ancestor
scala ignored — red at the nested-transform assertion; `premere`
aiming at the corner instead of the centre — red at the `numerus`
read. Both green on revert. Examen ACCIPE; one 74-column line on
birth, fixed by rewriting `quaerere` by name.
