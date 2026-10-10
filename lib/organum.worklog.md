# organum worklog

## 2026-10-10 — S1: manifest loader (status-plan S1)

Built `organum` (include/organum.h, lib/organum.c) + `organum.canon`
(root; registered `<organum>` in canones.registrum) + embedded copy
`lib/organum_canon.c` (GENERATUM by tools/organum_canon_infigere.sh,
generator action `organum_canon` in aedificatio.stml - same pattern as
norma_canon: the loader never reads a file).

Two-level judgment, deliberately split:
- canon (shape): closed vocabulary, choice lists, `scriptor` must cite
  a declared `<actio>` (citatio), unique campus/actio/ligamen names.
- loader rules (what the canon's doctrine refuses: co-occurrence,
  split values): sedes=motus -> rhythmus=mobilis + vita=transiens;
  typus=series -> sedes=motus; orbatus -> referentia; referentia
  'ligamen/genus' with a declared ligamen; absens valid for typus
  (electio: among the optiones); optio only under electio, electio
  needs one.

Generated outputs:
- island canon: one `<attributum>` per campus with sedes=insula
  (motus fields excluded), root element = organum genus (per-branch
  convention), electio optiones copied (escaped).
- `<domini>`: campus with sedes=insula + scriptor + NO aspectus (an
  aspect is the system's). genus="ephemera" for now (S2 keeps the
  island split).

Evidence that it is real: probatio_organum `domini` section reads the
LIVE apps/pictor/canones/domini.stml and requires the generated owner
for each of the 11 shared attributes to match. 7 plants caught (each
loader rule family, canon skipped, motus in generated canon, aspect in
domini, base duplicates).

Naming: the lint refused `kineticus` (Greek). The house Latin word for
the kinetic tier already existed - `mobilis` (motus.h "genus MOBILE",
pictor-spec state table) - so the rhythm is `rhythmus="mobilis"`.

Follow-up: tools/organum_canon_infigere.sh is a near-copy of
norma_canon_infigere.sh - with two consumers, a generic
`canon_infigere.sh <fons> <titulus>` is due (must keep norma's output
byte-identical: generata gate).
