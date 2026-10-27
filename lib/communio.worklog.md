# communio worklog

## 2026-10-27 - communio_transferre (phase 1 T3, quarta)

New library: moves one volume's OWN acts (`volumen_acta_propria_legere`,
replica = own) after a local-seq cursor into another open volume via
`volumen_actum_importare`. The same export/import the phase-2 network
protocol will carry. Rules:
- Foreign acts are never re-exported (own replica only), so relays never
  bounce acts back.
- Blobs first: every run of exactly 64 lowercase hex chars in an act's
  datum that names a massa in the source is copied if the destination
  lacks it (and the re-computed sigillum must match). Generic - no app
  format; covers plagula-condita's sigillum and pictor's
  `<imago massa=...>`; a false match costs one extra blob.
- Derived data stays home (genera-datorum VIII-b): plagula-condita with
  origo ending `:checkpoint`, plagula-remota of a `checkpoint/<n>` path.
  The cursor still passes them.
- The cursor only advances past acts that imported or were already there;
  the first refusal stops the batch with the cursor just before it (a
  retry stops at the same act - corruption never gets skipped).
