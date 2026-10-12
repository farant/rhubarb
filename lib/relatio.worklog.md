# relatio worklog

## 2026-10-10 — S5b: the reference system (status-plan S5b)

Fields with `referentia="ligamen/genus"` are validated against the
bound document and repaired per `orbatus` (summum = last id of the
document's ordered enumeration; nullum = remove; no orbatus = left
alone). Repairs are written as system `relatio` (S5a systemata), the
previous writer restored afterwards. Undo does NOT revert a repair
(Fran: choice a) - remembered repair is a follow-up.

Triggers:
- `historia` observer (new in historia.h, S5b): after every act, undo,
  redo. Registered ONCE per Historia; the observer walks every branch
  bound to that historia - so a layer deleted through pane A repairs
  pane B showing the same drawing (shared cache, vicus S3e).
- `relatio_ligare`: at mount (after owners) and on every rebinding
  (`pictor_picturam_ponere` - the single rebinding point;
  `pictor_picturam_condere` ends in it; vicus's shared-drawing swap
  goes through it too). Validates immediately - this also covers a
  stale value restored by S3 (restore runs before ligare).

The document library side is two adapters in pictor_applicatio.c over
the public pictor_documentum_numerus_stratorum / _stratum(k)->id
(ordered bottom first). The widget never says what a layer is.

`StatusSystemata {conservatio, relatio}` replaces the Conservatio*
parameter of pictor_montare (S5c adds popover) and is kept in
PictorMontatio for rebinding. vicus creates the bundle lazily at the
first pictor pane, like S3.

Finding: pictor's own delete already moves the selection (strata.agere
picks the layer below), so the orphans S5b fixes are the ones pictor
never saw: undo/redo, another pane, rebinding, restore. Before S5b
`stratum_currens` papered over them at READ time while the island kept
the dead id; it is now unreachable for a mounted widget (kept until
S9's typed accessors, as in S4).

Order note: implementation came before the tests this slice (historia
observer, relatio, pictor wiring) - compensated with plants: 3 for the
observer calls, 9 for relatio/pictor (incl. "repair written
anonymously" -> refused by the owner check: S5a and S5b proven
together). Lint: `observator` (real classical Latin, WORDS only knows
the verb form) got a glossary entry - oratio 19/19 unmoved;
`enumeratorem`/`revocationem` were internal names, renamed.
