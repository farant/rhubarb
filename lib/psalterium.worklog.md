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
