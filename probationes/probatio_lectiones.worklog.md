
## 2026-11-10 - fixtures moved into the credo area (fabrica-plan-8 T5)

The shadow pass over all nine composites found exactly one member where
the walk and the walk-free judgement disagree: this one. The test points
the channel at a private book, so its book (`build/probatio_lectiones.tsv`)
and the fixture `build/probatio_lectiones_s.txt` were raw writes into
build/ that neither the member's own book nor the tracked snapshot could
see - a debt and a "wrote outside its place" invalidation visible to the
walk only.

Fix: the private book and the d/s/w fixtures live under `credo_area(c)`
(`_in_area`), which sits under the member's TMPDIR, i.e. inside its
place. Expected S lines are built from the same paths (`_linea`). After:
vestigia_umbra.txt and vestigia_collata.txt carry only `cursus`,
debita.txt has no paths, and the member passes under
FABRICA_VESTIGIUM=liber.

Not changed: the test still unsets FABRICA_LECTIONES for the rest of the
process, so the member's real book stops after section I.
