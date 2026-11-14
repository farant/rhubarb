# gabc_visus worklog

## 2026-11-14 — T1: what moves out of antiphonale.c (the split map)

Plan: project-specs/psalterium-plan.md T1-T2; spec psalterium-spec.md
D4, §II. Every function/static in lib/antiphonale.c (1381 lines at
0c6c96e84), marked VIEW (moves to gabc_visus.c) or FILE (stays in the
antiphonale shell).

| antiphonale.c | goes to | note |
|---|---|---|
| attributum_s32, spatium_currens, ParCampi, campum_ponere, numerum_scribere | VIEW | write "pagina"/"spatium" under the shell's actio as scriptor |
| superficies, altitudo_paginae, latitudo_paginae | VIEW | altitudo_paginae also subtracts margo_supra |
| versionem_destruere, componere_ad, compositio_currens, pagina_currens | VIEW | |
| carmen_aedificare, nexus_ad_pulsum, syllaba_ad_pulsum, nexus_syllabae | VIEW | |
| relegere | SPLIT | FILE: exists?/read/"empty file"/"missing:"; VIEW: parse, vitia banner, same-text check, new Versio, G18 |
| speculam_claudere, speculam_aperire | FILE | |
| viam_ponere | FILE | calls gabc_visus_vacuare |
| paginam_movere, spatium_movere, syllaba_currens, ad_syllabam_ire, fac_canere (body) | VIEW | exported as gabc_visus_* actions |
| fac_proximam/priorem/maius/minus/canere/priorem_syllabam/proximam_syllabam | FILE | thin wrappers in the shell's key table |
| TABULA_PRAECEPTORUM, requisita_nulla | FILE | names/keys/titles byte-identical |
| agere | SPLIT | keys stay in shell; mouse branch -> gabc_visus_premere |
| titulum_capere, antiphonale_titulus | FILE | uses gabc_visus_titulus, then file name |
| textum_pingere, figura_antiphonalis | VIEW | -> gabc_visus_pingere; shell passes nuntius_vacuus only when via is "" |
| antiphonale_componere | SPLIT | shell makes root "antiphonale"; view adds sagitta_prior/_proxima, globulus_canere and runs the lazy layout |
| antiphonale_montare, _argumentum_ponere, fossa | FILE | argumentum_ponere's "pagina 0" -> gabc_visus_paginam_ponere |
| antiphonale_pulsare | SPLIT | watcher FILE, sound-follow VIEW (gabc_visus_pulsare) |
| antiphonale_mora | SPLIT | via "" -> -1; else gabc_visus_mora (33 while sounding), else specula ? -1 : 250 |

Subtleties T2 must keep (behaviour the tests pin):

- **Every re-read rebuilds the banner from scratch.** relegere starts
  with an empty banner, so a good re-read (even same text) clears ANY
  banner, including "not playable" and "can't play". That is why the
  header says "textus bonus quilibet (etiam idem) vexillum tollit".
- After a good parse the G18 branch may set "not playable: ..." or the
  muted banner AFTER the clear: order matters.
- The test files reach into the public structs:
  `probatio_antiphonale_latus.c:46-49` embeds AntiphonaleMontatio and
  AntiphonalePorta; `probatio_vicus_antiphonale.c:142-144` reads
  `montatio->porta.incipere/pulsus`. So the shell keeps its own `porta`
  copy in AntiphonaleMontatio (the view copies it too), and
  AntiphonalePorta becomes `nomen GabcVisusPorta AntiphonalePorta;`.
  The unchanged-tests guard covers BOTH files.
- Only lib/vicus_applicatio.c, lib/antiphonale.c and those two tests
  include antiphonale.h.

`continuare` FALSUM is new: stop the sound, selection -1 (a new melody
starts from the top on Space).
