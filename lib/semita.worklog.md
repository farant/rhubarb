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
