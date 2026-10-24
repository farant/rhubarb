# testa worklog

## 2026-10-24 - text clipboard service (merge of main into secunda)

secunda had scriba's register export (yy / y / dd -> system clipboard,
`clipboard_textum_ponere`) wired straight into apps/scriba and apps/vicus
`principale`. Main's testa B1 replaced those with `applicatio_initium` and
moved image paste into a testa service (`TestaAmbitus.imago_glutinata`).
The merge carries the text export the same way: `TestaTextumPonere
textum_exportare` + `textus_ctx` on `TestaAmbitus`; lib/testa.c sets
`_textum_exportare` (clipboard_platform), lib/testa_android.c sets
`_textum_nullum` (FALSUM, no Android clipboard yet, like the image stub).
Apps read it in `applicatio_initium`; a NIHIL service is safe (scriba
checks the pointer before calling). Ambitus built by hand in tests stays
zeroed = no export.
