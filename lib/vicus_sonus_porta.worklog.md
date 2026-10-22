# vicus_sonus_porta worklog

## 2026-10-22 - the real sound port (sonus S4 step 4b)

Thin glue: `cantuale_ordinarium` + `tintinnabulum_creare` whose
`TintinnabuliExitus` wraps auditus; time = `fenestra_tempus_ms`.

The one non-obvious part: `auditus_aperire` allocates its two rings
(iussa, nuntii; LXIV each - >= one jingle's 32 notes plus SISTERE for all,
and >= 2 x voices) from the piscina it is given, and tintinnabulum CLOSES
the device after TINTINNABULUM_QUIES (30 s) and reopens on the next
jingle. Passing vicus's long-lived piscina would grow it on every reopen,
for as long as vicus runs. So `exitum_aperire` creates a small dynamic
piscina per open and `exitum_claudere` destroys it after
`auditus_claudere` has waited out the audio thread.

No unit test (it would play through the speakers during gates); the
logic above it is covered by probatio_tintinnabulum (offline mixtura)
and probatio_vicus_sonus (fake port). Live check = Fran's ears.
