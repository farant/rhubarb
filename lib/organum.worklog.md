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

## 2026-10-10 — S2: pictor loads its manifest

apps/pictor/organum.stml (real manifest) + apps/organum_commune.stml
(base: id, focus, focus_acervus, superficies_*). pictor_montare reads
both, merges the base, GENERATES one canon (set on BOTH islands while
the split lasts - slightly looser per island than the two old canons;
the decision point after S5 settles the split) and the owner table.
apps/pictor/canones/ (durabilis.canon, ephemera.canon, domini.stml)
deleted; the old owner table frozen as
probationes/fixa/organum/domini_ante_s2.stml.

No behaviour change, checked three ways:
- types: every attribute shared by the old canons and the manifest has
  the same type (script diff at S2; only the dead declarations differ).
  `instrumentum` stays `nomen` (tests use "nihil"; electio = a
  tightening for later).
- owners: probatio_organum `pictor_vivus` - the live manifest's 16
  generated owners equal the frozen old table; the only old owners
  absent are the dead ones (cursus, gradus, sigillum).
- suites: organum, 19 pictor, 12 vicus green.

Differences from the S0 draft, on purpose: semen stays on the island
(written there today; moves to motus later), latitudo/altitudo kept
(Fran: until a clean migration), dead declarations dropped.

Gap found by plants and closed: nothing tested that the MOUNT applies
canon and owners (planting "skip the ephemera canon" stayed green - and
before S2 a missing canon file was silently tolerated too). New
probatio_pictor_montatio goes through pictor_applicatio_aedificare:
unknown attribute refused on both islands, wrong type refused, zoom
owned by motus, focus by dispensator (base), palette ownerless (aspect);
missing manifest = mount refused (two guards: no organum, then no
canon). Gotcha: an owner check needs a value that CHANGES - writing
the current value is a no-op the gate lets through.

Now pictor refuses to mount without its manifest (before: missing canon
files = mounted without a canon). Fail-loud, intended.

Follow-up: the "read manifest + base, generate, set on branch" code
lives in pictor_applicatio.c and a test helper
(probationes/pictor_organum_probationis.h); scriba (S6) is the second
consumer - then it becomes a generic function (header to Fran).
