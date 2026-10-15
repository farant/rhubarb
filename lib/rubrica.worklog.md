# rubrica worklog

## 2026-10-14 - P1a: shared tag rules

nomenclator-plan P1a, header approved by Fran. The tag rules lived
twice (agenda.c: valid / compare / parse / join; kalendarium.c: its own
`rubrica_valida`, an item-shaped `rubricam_habet` and a join wrapper);
contacts would have been the third copy. Moved verbatim from agenda.c;
`rubricam_habet` now takes the SET (Xar of chorda) instead of an
AgendaRes, so the calendar passes `r->rubricae`. One change on the way:
`rubricas_comparare` no longer calls memcmp with length 0 (both empty)
- harmless before, but memcmp on a NIHIL datum is formally undefined.
The calendar keeps its inline-#tag helpers (rubricas_extrahere,
rubricas_miscere): calendar behaviour, not tag rules.
Tests: probatio_rubrica (valida, ordo, legere, habere). Plants: capitals
allowed; duplicates kept; length order reversed; habet always true -
all red. ludus_tessera hand list gained "rubrica" (0/8 at link until
then - vicus links agenda and the calendar).
