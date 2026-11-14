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

## 2026-11-14 — T2: extraction done

- antiphonale.c 1381 -> 538 lines; gabc_visus.c 1168. Guard held:
  probatio_antiphonale_latus 11/11 and probatio_vicus_antiphonale 3/3
  before and after, both files untouched (`git diff --stat
  probationes/` shows only the new probatio_gabc_visus.c).
- **Plan step 3 did not go red.** The plan expected the new test to
  fail at link before git knew lib/gabc_visus.c; it passed, because the
  build resolves the body through the header's aedilis tag
  (`<aedilis corpus="lib/gabc_visus.c"/>`), not through git's file
  list. The red came from the plant instead: `continuare` ignored
  (`si (visus->sonat && continuare == II)`) -> FRACTA at
  probatio_gabc_visus.c:189 "continuare FALSUM: sonus sistit"; restored
  -> green (silva.planta).
- Small widening of the approved header: `gabc_visus_componere` also
  sets the root's fines (the surface), since the surface rule lives in
  the view now; the shell only creates the root, focus and actio.
- Shell's relegere reads into a scratch arena and destroys it; the
  view copies the text into the version's own arena. The mount arena
  does not grow per re-read ("directorium absens" test unchanged).
- Empty text handed to the view sets banner "empty text" and keeps the
  version; the antiphonale never hands it empty text (it says "empty
  file" itself).
- First commissio refused: ludus_tessera/compile_probationes.sh keeps a
  hand source list; antiphonale.c now needs gabc_visus.c, so 8 tessera
  tests failed to link. Added "gabc_visus" next to "antiphonale" there
  (the known trap, memory "new include in an old lib file"). T5 adds
  psalterium.c, which vicus_applicatio.c will include in T7: add it to
  the same list then.

## 2026-11-14 — psalterium T6: scriptor copied once

`numerum_scribere` did `chorda_ex_literis(visus->actio, visus->piscina)`
on every "pagina"/"spatium" write - the mount arena grew a few bytes
per page turn (inherited from antiphonale.c). Now `visus->scriptor` is
copied once in `gabc_visus_creare`. Found while writing the psalter's
Review Focus 5 test (200 re-fits, mount arena flat); a plant putting
the per-write copy back turns that test red.

## 2026-11-14 — banner below margo_supra

The banner (parse error, "not playable", "can't play: sound muted")
used to sit at the top limbus, i.e. on top of the psalter's header row
(margo_supra 16 px), hiding its buttons whenever sound was muted. It
now starts at y_paginae = limbus + margo_supra. The antiphonale has
margo_supra 0, so its banner is where it was (its suites unchanged).
