# psalterium worklog

## 2026-11-14 — T3: how routes reach a new pane (closes spec AUDIENDA 1)

Traced lector (`lib/lector.c:3630-3680`) and vicus (`lib/vicus.c:1450-1470`):

- **`$` argument → route is the pane's job.** The pane's
  `*_argumentum_ponere` calls `semita_ex_argumento(organa, m, genus,
  arg, ...)` (`lib/semita.c:1350`): a URI of the genus ("psalterium/...")
  goes straight to `semita_ex_uri`; otherwise the manifest's
  `<cognomina>` are tried in order (fixed name, typed value substituted
  for `{}`, neutral alias for ""). Then `semita_applicare(organa, m,
  &m->ramus, semita, ...)`. The pane checks domain validity itself
  BEFORE applying (lector refuses an unknown book) — the psalter must
  refuse psalm 151 the same way, since typus `numerus` accepts it.
- **Restore is vicus's job.** At start and on history (Ctrl+O/I) vicus
  applies the saved route itself and then calls the pane's
  `facies.semitam_secuta` hook, so the pane re-syncs from its fields.
  Hence `psalterium_semitam_secuta` (re-fit the psalm from
  visus/psalmus/signum).
- Route URI = genus + the ligatio fields in manifest order:
  visus, psalmus → `psalterium/psalmus/109`, `psalterium/index`.
- "109,8 G" cannot be a typed alias (`numerus` refuses it), so the pane
  splits at the comma: the number part goes through the cognomina, the
  tone is written to `signum` (not a route field — D6).

Manifest loads: a probe linked against `build/probationes/libradix.a`
read genus "psalterium" with 2 cognomina.

One field beyond the spec table: `cifrae` (transient text) — digits
typed in the index. The plan left the display of typed digits to the
builder; house rule puts all pane state in the manifest.

## 2026-11-14 — T4: the embedded psalter; the Gloria fit

- `tools/psalterium_infigere.sh` (shape of toni_psalmorum_infigere.sh):
  one byte array of 1.txt..150.txt then LICENTIA, plus `_initia[151]`
  (start of psalm N = `_initia[N-1]`, end = `_initia[N]`, licence from
  `_initia[150]` to the array end). Action `psalterium_infixum` in
  aedificatio.stml declares the psalms as ONE input,
  `<ingressus genus="plagulae" via="musica/psalmi" suffixa=".txt"/>`,
  plus LICENTIA, instead of 151 fasciculus lines.
- Measurements (spec AUDIENDA 2, closed): see the spec. The link time
  does not move; the binary grows by the 265 KB of text. Note: a new
  lib/*.c makes the untracked corpus capsule
  (build/capsula_corpus_silicis.c) STALUM under `fabrica iudicare
  -tacta`; `./tools/corpus_infixum.sh` heals it (5.5 s).
- Tests in probatio_psalmodia.c. The planned name `_gloria_probare`
  was taken (the fitter's own Gloria test), so the new one is
  `_psalterium_gloria_probare`. It passed on first run: the Gloria
  fits all 150 psalms on all 33 endings with no warnings.
- Red: before the generator, link failure on psalterium_infixum_*.
  Plants (silva.planta, radix/psalmodia, batch in
  build/plantae_psalterium/): (1) Ps 104 byte "ejus" -> "ejut" in the
  generated copy -> `diversi == ZEPHYRUM` red; (2) "Glória" ->
  "Gloria" in lib/psalmodia.c GLORIA_PATRI -> new section red at
  `admonitiones == ZEPHYRUM` (old _gloria_probare red too); (3) Gloria
  appended only for canticles -> `sine_gloria == ZEPHYRUM` red. All
  restored (cmp against backups), green after.

## 2026-11-14 — T5: pure helpers and the index view

- lib/psalterium.c: incipit, the D8 cycle (`_circuli_terminatio`
  walks the table skipping ad_libitum/monosyllaba; label = modus +
  " " + differentia), `psalterium_signi_index` goes through
  `psalmodia_signum_invenire` and compares the returned ending
  POINTER with the cycle - so "VIII g", "8g", "8 G" all give the same
  index, and an argument's tone is stored in cycle form ("8 G").
- Index drawn in pixels with the gabc_visus geometry (limbus 6/8,
  footer 12, arrows 24 wide), rows 14 px through album (one unit per
  row; capacity = rows between limbus and footer, at least 1). Row
  "%3d  incipit" so row 109 reads "109  Dixit Dóminus Dómino meo".
  Footer "<  Psalterium  2/11  [10]   Divinum Officium · MIT  >"; the
  typed digits show as "[10]". Footer arrow nodes reuse gabc_visus's
  ids `sagitta_prior` / `sagitta_proxima` (dispatched by view).
- Rulings (in the plan note too): digits and Backspace are handled in
  agere before the key table (ten table rows would clutter the
  command list); any movement clears the typed digits, even a move
  that does nothing (Up on 1) - otherwise "151" then Up then "0"
  would select 10. "0" alone selects nothing and clears the digits.
  Enter / click write the route fields (visus, psalmus) + electus in
  T5; the psalm view's fit and drawing are T6
  (`psalterium_semitam_secuta` is a stub until then).
  `psalterium_argumentum_ponere` is complete: URI (psalm number
  checked BEFORE applying), "", "109", "109,8 G" (tone written to
  `signum`). `visus=psalmus` with a psalm outside 1-150 reads as the
  index already (Review Focus 1, tested in T6).
- All 92 checks passed on the first run, so every section got a
  plant (build/plantae_psalterium/t5.py): incipit ignores the cross;
  cycle counts monosyllaba; psalm off by one; minimum capacity 2;
  "0" alone writes electus 0 - each red in its own section, restored.
- Lint: `ima`/`imam` (footer) renamed to `pes` (gabc_visus's word;
  memory: never add imus); `cifram` -> `_cifrae_addere/_tollere`
  (the glossary's cifra is "cipher"; the T3 manifest field `cifrae`
  stays). `silva/renominare.sh` refuses files with uncommitted
  changes ("plagulae affectae sordidae") - not a no-op; I had cut
  the message off with tail -1.
