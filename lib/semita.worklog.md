# semita worklog

## 2026-10-19 — R1 (project-specs/semitae-spec.md)

Library for routes as partial state trees. Header approved by Fran
(changes from spec §VII: child-branch convention, per-writer writes,
a genus -> organum callback, `organum_valorem_iudicare` in organum.h).

- **Validation before any write**: one recursive pass (`_iudicare`) over
  the whole tree: element names (root genus, children = montationes),
  attributes = routable fields, values typed (`organum_valorem_iudicare`
  shares the loader's check for `absens`), duplicate children, mounted
  children (branch present), inherited bindings. A refusal leaves the
  state untouched, root included (tested: unmounted child under a good
  root writes nothing).
- **Child branches**: montatio (nomen, genus) lives at `<genus
  id="nomen"/>` under the parent branch (insula_ramus_liberi). Today's
  hand-mounted children (calendar notes `charta` id `c1_notae`) do not
  follow it yet - R4.
- **Writes**: each field is written as its declared writer (the
  manifest's `scriptor` = insula dominus); one `mutare_ramum` per
  distinct writer. A fixture field with a second writer (`proba.ordo`)
  proves the grouping; writing everything as one writer is refused by
  the owners (plant red).
- **Reset rule**: binding "changed" compares normalized values (unset
  and empty = the field's absens), so re-applying the same binding keeps
  binding-scoped state. An explicit empty value means "back to the
  default" (`ordo=""`).
- **Reading**: canonical = routable fields only, values equal to absens
  omitted, inherited child bindings omitted, empty children omitted.
- **URI**: path = root binding fields in manifest order (trailing unset
  dropped; an unset one in the middle is written as its absens - a
  middle field without absens makes the URI fail). Query = root state,
  then children in montatio order with dotted keys (independent child
  bindings included in the single form). Escaping: every byte outside
  `A-Z a-z 0-9 - _ . ~` as uppercase %XX. The list form gives each node
  with its own binding its own URI; inherited children stay dotted in
  the parent's.
- Bug found by the tests: the dotted-key walk advanced past the last
  part before slicing the field name (negative-length slice).
- Fixture lesson: fixture manifests must take the shared base
  (`apps/organum_commune.stml`, as organum_legere_viam does) - preparing
  a branch writes base fields, refused otherwise ("attributum extra
  canonem").
- Plants 18 red (one test added first: duplicate child in a route).

## 2026-10-19 — R3: semita_legere_plenam

Full read for exact restore (vicus history): every routable field,
value or absens or "" when unset; mounted children always. Not
canonical. Test `plena`: a full read of a fresh world applied onto a
changed one brings it back to all defaults (canonical route "proba").

## 2026-10-20 — semitae R4d: binding named whole, persistent read (Fran approved)

- V-a in `_applicare_nodum`: a node with ANY ligatio attribute names
  the whole binding; unnamed ligatio fields get "" (-> absens) and count
  toward `mutata` (so binding-scoped status resets follow). A node with
  none keeps the binding.
- Canonical read forces the binding at the ROOT only (first ligatio
  field, absens or ""); children are not forced (would put
  `child.f=` in every URI).
- TRAP: STML text `visus=""` parses as an attribute with valor NIHIL.
  `_attributum` returns the value (NIHIL), so presence needs `_adest`.
  The empty-binding URI is `genus/`; `semita_ex_uri` now adds empty
  path segments as attributes (names the binding).
- `semita_legere_persistentem` = canonical minus vita transiens; the
  binding is named only if some persistent ligatio field exists.

## 2026-10-20 — semitae R4e: cognomina + symbolic values (Fran approved)

- `semita_ex_argumento`: URI of the genus first, else the manifest's
  `<cognomina>` in order - fixed (exact), typed (`organum_typum_iudicare`,
  never the empty argument, `{}` replaced by the ESCAPED argument), neutral
  (empty argument only); NIHIL = none matched (the pane tries its computed
  aliases, or refuses).
- Symbolic values (`dies`: hodie/heri/cras) pass `organum_valorem_iudicare`,
  so URIs and typed aliases carry them; `_iudicare` refuses them only when
  APPLYING (ramus given). `semita_resolvere` swaps the attribute's valor
  pointer for the resolver's concrete value; children by montatio.
- Order trap: `est_dies` accepts `2026-10` (length 7), so the calendar's
  computed `YYYY-MM` alias must run before the manifest's typed `dies`.

## 2026-10-22 — child branches "<parent id>_nomen" (D5 S3c)

The spec's open naming question (§X): vicus panes mount their notebook
children as `<scriba id="<pane id>_notae">` because scriba keys its
viewed page by id (`scriba/visus/<id>`), which must be unique in the
volume - two calendars with a child "notae" would share it.
`_ramus_liberi` now tries `<genus id="<parent id>_nomen"/>` first and
falls back to `<genus id="nomen"/>` (the old fixtures keep working).
A pane's own SemitaOrganum must also answer for its children's genus
(the calendar's resolver returned NIHIL for "scriba", so any route with
`notae` was refused). Plant red: prefixed lookup skipped.
