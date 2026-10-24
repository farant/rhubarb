# concentus_auditus worklog

## 2026-10-22 - S6d step 2: extracted from vicus_sonus_porta

The ConcentusExitus over auditus.h used to be six interior functions in
lib/vicus_sonus_porta.c; auditio (-carmen) is the second consumer, so it
moved here (Fran chose extraction over a copy). The ring piscina is still
created per open and destroyed on close - the concentus closes after 30 s
of quiet and reopens on the next sound, so rings in the caller's piscina
would pile up for days. Ring capacities come from the SAME MixturaForma
the caller hands to concentus_creare (iussa / nuntii), so the port sets
the numbers in one place.

No unit test: it is pure pass-through to the real CoreAudio unit.
Checked by the vicus build (vicus + vicus_terminalis link it) and live:
Fran heard `praeco cantus -genus finis` (arpeggium), then a second one
(cadentia) fired after 35 s, i.e. after the quiet close - the reopen path
(new ring piscina) works.
