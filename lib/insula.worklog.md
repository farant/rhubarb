# insula.worklog.md

## 2026-09-04 — natus (ludus T6)

Three-genera repository: DURABILIS and EPHEMERA as STML trees, one
write gate per genus (`mutare_durabile`, `mutare_ephemera`), MOTUS
deliberately absent (T7 keeps it as a struct and flushes here in
quiet). Ping-pong piscinas per genus: a gated write serializes the
live tree into the other piscina, re-parses it there, runs the
mutator on the duplicate, judges it against the genus canon if one
is set, then swaps. The round trip through text IS the rehydration
discipline — a tree that cannot circuit cannot be written. Restore
compares the in-memory serialization with the last honest text;
mismatch = mendacium, then rebuilds from the honest text. A lie is
not a write: `versio` and the actarius are untouched by it.
`mendacium` is per-restore, not sticky.

Deviations from the draft, all forced by the substrate:

- `insula_attributum_ponere` is new. stml has no replace primitive:
  `stml_attributum_addere` appends unconditionally and
  `stml_attributum_capere` returns the first match, so the draft's
  mutator would have produced a duplicate attribute and read back the
  OLD value. The helper substitutes in place or appends; it keeps
  `stml_attributum_addere`'s shape so a mutator passes its arguments
  through. T7's `motus_effundere` needs exactly this.
- The toy canon declares its attributes. `CANON_ATTRIBUTUM_IGNOTUM`
  exists, so `<elementum nomen="ephemera" radix="verum"/>` alone
  would have rejected the honest write too.
- `causa` on a canon refusal carries `canon_nuntius` of the first
  vitium after the prefix, allocated in the repository's own piscina
  (arena semantics: a failure path leaks its message; small).
- Every entry point guards `repo` and the genus range; the draft
  trusted both.
- The probatio passes mutator contexts as `character[]` locals, not
  string literals: house flags carry `-Wwrite-strings`, so
  `(vacuum*)"impletio"` is a `-Wcast-qual` error.

Facts worth keeping: `insula_attributum` returns a pointer into the
interner (the caller's piscina), so it outlives the swap — the header
still calls it read-only, and writing through it is the lie the
probatio commits on purpose. An empty attribute value interns to
NIHIL (`chorda_internare_ex_literis` returns NIHIL for ""); remove
rather than blank. Each repository owns four dynamic piscinas of
XVI·M initial bytes.

Gate: `probatio_insula` (49). Planted fault: mendacium detection
inverted → red at the `CREDO_FALSUM(insula_restituere)` of the lying
writer, green on revert. Examen ACCIPE; formator 0 divergences on
birth (the T2 lesson held).


## 2026-09-05 — domini (ludus P3 T6)

Reads global, writes owned (brainstorm §XVI §2). A repository now
carries, per genus, a table of `InsulaDominus` (attribute → the one
scriptor allowed to change it) and a current `scriptor` name that
the dispatch context sets: the dispensator puts the action's name
around each handler call and `"dispensator"` around its own focus
writes; `motus_effundere` puts `"motus"` around the quiet flush.
Enforcement is a diff at the gate: after the mutator runs on the
duplicate, every owned root attribute that was added, changed, or
removed is checked against the scriptor, and a mismatch refuses the
write with `causa` naming the attribute, its owner, and the
offender. No static analysis, one attribute walk per write.
Handlers keep their signature — ownership is a property of the
dispatch context, not of the code that writes. Unowned attributes
are anyone's; children (the `strata` list) are unowned in v1.

The real canons live in `apps/pictor/canones/` (spec §4):
`ephemera.canon`, `durabilis.canon`, and the owner table
`domini.stml` (eleven ephemera rows, one durabilis row). The flow
idiom of §XVI §1 is reserved there as `cursus`/`gradus`, owner
`cursus`, unused until P5. `zoom`, `pan_x`, `pan_y` belong to
`motus`; `focus` and `focus_acervus` to `dispensator`;
`instrumentum` to `instrumentum.eligere`. The probatio `pictor_canones`
loads all three from disk, judges a pictor-shaped island pair,
refuses an undeclared attribute, and proves only `motus` may write
`zoom`.

`insula_attributum_tollere` joined `insula_attributum_ponere` (stml
has no removal primitive; swap-remove, order is not semantic).
Gate: `probatio_insula` (+15) and `probatio_pictor_canones` (12);
dispensator, motus, and the toy replay unchanged. Planted fault: the
owner compared against the attribute name instead of the scriptor —
red at the owner's own write, green on revert.
