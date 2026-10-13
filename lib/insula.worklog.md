# insula.worklog.md

## 2026-09-04 — natus (ludus T6)

Three-genera repository: DURABILIS and EPHEMERA as STML trees, one
write gate per genus (`mutare_durabile`, `mutare_ephemera`), MOTUS
deliberately absent (T7 keeps it as a struct and flushes here in
quiet). Ping-pong piscinas per genus: a gated write serializes the
live tree into the other piscina, re-parses it there, runs the
mutator on the duplicate, judges it against the genus canon if one
is set, then swaps. The round trip through text IS the rehydration
discipline — a tree that cannot circuit cannot be written. Restore
compares the in-memory serialization with the last honest text;
mismatch = mendacium, then rebuilds from the honest text. A lie is
not a write: `versio` and the actarius are untouched by it.
`mendacium` is per-restore, not sticky.

Deviations from the draft, all forced by the substrate:

- `insula_attributum_ponere` is new. stml has no replace primitive:
  `stml_attributum_addere` appends unconditionally and
  `stml_attributum_capere` returns the first match, so the draft's
  mutator would have produced a duplicate attribute and read back the
  OLD value. The helper substitutes in place or appends; it keeps
  `stml_attributum_addere`'s shape so a mutator passes its arguments
  through. T7's `motus_effundere` needs exactly this.
- The toy canon declares its attributes. `CANON_ATTRIBUTUM_IGNOTUM`
  exists, so `<elementum nomen="ephemera" radix="verum"/>` alone
  would have rejected the honest write too.
- `causa` on a canon refusal carries `canon_nuntius` of the first
  vitium after the prefix, allocated in the repository's own piscina
  (arena semantics: a failure path leaks its message; small).
- Every entry point guards `repo` and the genus range; the draft
  trusted both.
- The probatio passes mutator contexts as `character[]` locals, not
  string literals: house flags carry `-Wwrite-strings`, so
  `(vacuum*)"impletio"` is a `-Wcast-qual` error.

Facts worth keeping: `insula_attributum` returns a pointer into the
interner (the caller's piscina), so it outlives the swap — the header
still calls it read-only, and writing through it is the lie the
probatio commits on purpose. An empty attribute value interns to
NIHIL (`chorda_internare_ex_literis` returns NIHIL for ""); remove
rather than blank. Each repository owns four dynamic piscinas of
XVI·M initial bytes.

Gate: `probatio_insula` (49). Planted fault: mendacium detection
inverted → red at the `CREDO_FALSUM(insula_restituere)` of the lying
writer, green on revert. Examen ACCIPE; formator 0 divergences on
birth (the T2 lesson held).


## 2026-09-05 — domini (ludus P3 T6)

Reads global, writes owned (brainstorm §XVI §2). A repository now
carries, per genus, a table of `InsulaDominus` (attribute → the one
scriptor allowed to change it) and a current `scriptor` name that
the dispatch context sets: the dispensator puts the action's name
around each handler call and `"dispensator"` around its own focus
writes; `motus_effundere` puts `"motus"` around the quiet flush.
Enforcement is a diff at the gate: after the mutator runs on the
duplicate, every owned root attribute that was added, changed, or
removed is checked against the scriptor, and a mismatch refuses the
write with `causa` naming the attribute, its owner, and the
offender. No static analysis, one attribute walk per write.
Handlers keep their signature — ownership is a property of the
dispatch context, not of the code that writes. Unowned attributes
are anyone's; children (the `strata` list) are unowned in v1.

The real canons live in `apps/pictor/canones/` (spec §4):
`ephemera.canon`, `durabilis.canon`, and the owner table
`domini.stml` (eleven ephemera rows, one durabilis row). The flow
idiom of §XVI §1 is reserved there as `cursus`/`gradus`, owner
`cursus`, unused until P5. `zoom`, `pan_x`, `pan_y` belong to
`motus`; `focus` and `focus_acervus` to `dispensator`;
`instrumentum` to `instrumentum.eligere`. The probatio `pictor_canones`
loads all three from disk, judges a pictor-shaped island pair,
refuses an undeclared attribute, and proves only `motus` may write
`zoom`.

`insula_attributum_tollere` joined `insula_attributum_ponere` (stml
has no removal primitive; swap-remove, order is not semantic).
Gate: `probatio_insula` (+15) and `probatio_pictor_canones` (12);
dispensator, motus, and the toy replay unchanged. Planted fault: the
owner compared against the attribute name instead of the scriptor —
red at the owner's own write, green on revert.

## 2026-10-05 — branches: `InsulaRamus` (insula-rami-plan R1)

Fran's idea at the schirmata interview: one store owned by the host,
apps and widgets given a "curried branch" of it (redux-like). A branch
= repo + (element, id) - a child of the root by kind and `id`, or the
root itself. Reads and the gate are relative to the branch's node:
`mutare` now takes a target; after the whole root is copied (as
before) the mutator receives the BRANCH's node in the copy; a missing
branch refuses with a named cause. `mutare_durabile/_ephemera` are
root-branch writes - unchanged behaviour (root 201/201).

**Owners per branch:** `InsulaDominus` gained (element, id); judging
compares the attribute on the owner's branch node before and after
(`ramum_invenire` on both roots; a missing node counts as no
attributes). Every write still judges every owner, so a write through
one branch cannot change another branch's owned attribute.

**Canons per branch:** each registered branch canon judges its node
(`canon_iudicare` accepts any element as root - but compares the
element's NAME with the canon's root: a mountable app's canon must name
its root element after the app kind; R3/R4). The ROOT canon judges a
shallow copy of the root whose child list omits branches that have
their own canon (`visio_radicis`) - the host's canon never needs the
apps' vocabulary, while a non-delegated child is still judged (an
unknown element under the root is refused).

Plants, all compiling (one first did not - an unused static; redone):
owners on the root only; mutator handed the root; branch canons
skipped; root canon seeing delegated branches; the view dropping every
child. All caught.

## 2026-10-05 — `insula_ramum_initiare` (insula-rami-plan T1a)

Mounting an app needs "make sure my branch exists with my initial
attributes": for a child branch, append the element (parsed in the
gate's own arena) if absent - an existing one is left untouched; for
the ROOT branch, add the element's attributes the root lacks (how a
standalone app now starts: a bare `<scriba/>` root, then a root-branch
mount). Through the gate, so canons and owners judge it. Order that
matters, now in the header: register the branch canon BEFORE (the host
canon never sees an undelegated mount), owners AFTER (scriba's `modus`
is owned by `pagina.clavis`; an anonymous initial write would be
refused - a plant moving owners first makes `scriba_montare` fail).

## 2026-10-10 — S5a: system writers in the owner table (status-plan)

`InsulaDominus.systemata`: an owner entry may also list SYSTEMS allowed
to write that field (space-separated, whole-word match - a prefix like
"rel" or the whole list as one writer is refused). Read from
`<dominus ... systemata="relatio"/>`; the refusal message names the
systems. Census X: owners for value fields, systems for cross-cutting
aspects - this replaces the "set repo->scriptor to the owner's name"
workaround (domini-scriptor-silent-refusal memory), which lied in the
log about who wrote. organum generates systemata="relatio" for fields
with a `referentia` (S5b's reference system will repair them).
Popover fields stay ownerless until S5c moves closing into the system
(an owner now would refuse the seven closing actions).

## 2026-10-10 — C1: durable island drained (status-plan, one-island collapse)

Plan reordered on contact: "one tree inside insula, API unchanged" would
need MERGING canons (scriba and vicus still have two hand canons with
closed vocabularies - each would refuse the other's attributes in one
tree). Instead: drain first. The only durable contents were pictor's and
scriba's latitudo/altitudo (copies of drawing/page size); terminale and
vicus durable trees were empty. Both moved to the ephemera island (init,
writer, reader; scriba's two canon attributes folded into
ephemera.canon). Durable branches now carry only `id`
(probatio_montatio asserts it; 3 plants caught). 10 test fixtures that
built islands by hand with sizes in the durable string were updated.
Next: C2 removes the durable genus from the API; C3 saved-state v2.

Pre-existing finding (not fixed here): apps/scriba and apps/vicus canons
fail `bin/canon_examen` - dialectus "scriba-ephemera" has a hyphen, the
canon-of-canons types dialectus as nomen. Runtime loading accepts them.
They disappear in C2 (durable) and S6/S7 (manifests).

## 2026-10-12 — C2: one island (genera removed) + saved-state v2

`InsulaGenus` (DURABILIS/EPHEMERA) removed from the API: one tree per
repository. `insula_repositorium_creare(p, in, stml)`; single gate
`insula_mutare` (was mutare_durabile/_ephemera); every reader/writer
lost its genus argument; `InsulaActarius(radix, ctx)`. Owner lines:
`genus` attribute now IGNORED (old domini.stml files still read).
Lifetime lives in the manifest (vita per field), not in trees.

Callers: pictor/scriba/terminale/vicus mount with one canon, one
initiation, one owner read; durable canons deleted (scriba, vicus,
terminale's infixed one); ludus_fenestra uses one version; motus flush
and dispensator unchanged except the argument. relatio lost its
per-island loop. conservatio writes versio 2 (`<status versio="2"
genus=... field="..."/>`) and MIGRATES versio 1 by merging
<durabilis> then <ephemera> (ephemera wins a conflict) - tested on a
real two-child v1 save; unknown versions still skipped. (C3 folded in:
removing genera forces the format change in the same commit.)

Tests: mechanical pass (regex: drop the genus argument; merge the two
creation strings by root+children union - ElementTree, ids as keys) +
hand rewrites where tests asserted two-tree semantics: probatio_insula
(one root carries all four attributes; durable-gate section removed -
counts after it unchanged), probatio_insula_ramus (root canon now
judges the initiation that used to go to the canon-less second tree:
canon gains `nova`, and "activa unchanged" is asserted - the real
point), dispensator ("not yet written" = version unchanged since the
mouse-down), toy snapshot (union of the two old trees, identical
values). 10 toy fixtures used "<documentum/>","<ephemera/>" - two
different roots, refused by the auto-merge as they should be;
collapsed to "<insula/>". 276/276 root tests, ludus_tessera 8/8, plants
5/5. briar's smoke builds project-specs/exempla/salutatio.thistle -
updated too.

## 2026-10-12 — status-plan S8a: branches at any depth

`InsulaRamus` gained `via`: the interned path from the root, segments
`elementum:id` joined by '/' ("" = root; canon `nomen` allows only
[A-Za-z0-9_*], so ':' and '/' are safe). `elementum`/`id` stay = the
last segment (pictor/scriba/dimissio/organum_montatio read them). Value
type, so a branch still copies by assignment; interning makes equal
paths share a pointer and keeps the per-event `insula_ramus` calls from
growing the arena (paths are built in a stack buffer, copied only when
new). `insula_ramus_liberi(parens, e, id)`: a widget mounting children
hands over ITS branch; `insula_ramus` = liberi of the root.

Depth-1 was in four places, all in insula.c: ramum_invenire (now walks
the path), the root canon's view (visio_radicis -> visio_nodi), owners
(InsulaDominus.via) and initiation (adds under the PARENT node; an
absent parent is refused BEFORE the gate with a named cause - nothing
written). Branch canons (CanonRami.via) are judged on a view too now:
every canon judges its node minus descendant branches that have their
own canon, nearest wins. The view copies only nodes on the way to an
excluded branch (shallow copies with a new child list); untouched
subtrees are shared pointers. With no branch canons it is the node
itself (old fast path).

Cost note for S8d: the view is rebuilt per canon per write (relative
suffixes of the canon paths below it, matched segment by segment) -
same order as before at depth 1, but now also for branch canons.

Plants (silva.planta, function gate on probatio_insula_profunda): view
recursion off (depth-1 behaviour) -> `canones` red 6/9; initiation to
the root instead of the parent -> tree cannot be built (parent check of
c fails). Both reverted -> green. Existing insula, organum, pictor,
scriba, vicus, terminale, conservatio, relatio, dimissio, motus,
dispensator suites and ludus_tessera 8/8 unchanged.

## 2026-10-13 — status-plan S8d: write cost measured (park 010 again)

Tool: `tools/insula_mensura.c` (build: `./bin/aedilis
tools/insula_mensura.c && bash build/aedilis/insula_mensura/struere.sh`;
`-ansa <genus> <n>` loops 20 s for `sample`). Per-write cost, min of 3
runs x 2000 writes, Apple silicon, default flags:

| case | n | branches | store B | per write |
|---|---|---|---|---|
| vicus (focus via dispatcher) | 2 | 4 | 1666 | 58 us |
| | 3 | 6 | 2488 | 76 us |
| | 6 | 12 | 4954 | 149 us |
| | 10 | 20 | 8244 | 284 us |
| toy chain, write at the bottom | 1 | 1 | 43 | 1.8 us |
| | 6 | 6 | 215 | 14 us |
| | 11 | 11 | 394 | 36 us |
| | 22 | 22 | 999 | 114 us |
| toy parent + k rows, write one row | 11 | 12 | 437 | 35 us |
| | 50 | 51 | 1850 | 415 us |
| | 100 | 101 | 3651 | 1.5 ms |
| | 200 | 201 | 7351 | 5.1 ms |

vicus opens at most 10 tabs (Cmd+1..0) - park 010's "11 tabs" row was
10 too. vicus at 10 tabs is 5x T5 (52.7 -> 284 us): the store is 8x
bigger (one island: every pane's whole state, not just ephemera) and
every write now also persists.

Profiles (`sample`):
- 200 rows: ~85% `ramum_invenire` from `mutare` - `dominos_iudicare`
  does TWO path lookups (before/after) per owner entry, every branch
  has several entries, each lookup scans siblings linearly: quadratic.
  `visio_nodi` 8% (the parent's view excludes 200 children, each
  matched against 200 paths), `reliqua_sub` 3%, canon judging and the
  text copy under 2% each. Park 010 suspected canon judging; at size
  it is the lookups.
- vicus 10 tabs: lookups ~38%; persistence actarius ~29%
  (`statum_scribere` re-serializes EVERY registered branch's state on
  every write, 19%; `volumen_plagulam_condere` pwrite+fsync of the
  changed one, 10%); text copy (scribere + legere) ~12%; canon judging
  ~7%.

So the dominant costs are all "the whole store, every write" in the
JUDGING and PERSISTING layers, not the copy. Options for S8e are in
status-plan (S8d).

## 2026-10-13 — S8e-1: path index (no API change)

`mutare` now resolves branches through an `IndexViarum` per tree
(old root and the copy), allocated in the write's arena:
- `ramum_per_viam`: lazy `tabula_dispersa` path -> node. The first
  query under a parent indexes ALL its children in one pass; a key is
  inserted only if absent (first match wins, like `ramum_invenire`).
  Children `ramum_invenire` could never match are skipped (title with
  ':' or '/', id with '/') - same results, no key collisions.
- `ramum_quaerere`: a pointer-keyed memo in front (`friatio_sedis`
  hashes `via.datum`, compares pointer + length). Owner, canon and
  target paths are interned, so a branch's several owner entries hit
  the memo. Without it the chain case REGRESSED (22 levels 114 -> 183
  us): `friatio_fnv1a` over ~300-byte paths, once per lookup, was the
  top of the profile. Non-interned paths just miss the memo.
- `visio_nodi`: excluded paths grouped by first segment in a table
  (`GrexReliquorum`) - one probe per child instead of children x
  excluded.

`insula_ramus_nodus` and initiation still use `ramum_invenire` (reads
on the live tree, one lookup each); conservatio's `statum_scribere`
lookups are S8e-3's job.

| case | S8d | S8e-1 |
|---|---|---|
| vicus 2 / 10 tabs | 58 / 284 us | 60 / 194 us |
| chain 1 / 22 | 1.8 / 114 us | 2.8 / 113 us |
| rows 50 / 100 / 200 | 415 us / 1.5 ms / 5.1 ms | 84 / 192 / 482 us |

## 2026-10-13 — S8e-2: scoped judging + guard

`mutare(repo, via, ...)` with a non-empty path now:
- **Guard first.** The copy is parsed from `textus`, so the target
  node's `positus_initium/finis` are its span in the OLD text; the new
  text comes from `stml_scribere_sedibus`, whose entry for the same
  node is its span in the NEW text. Bytes before and after must match
  (two memcmp). Escape -> "mutatio extra ramum: ...". The new text is
  reused as `textus_ultimus` (no second serialization). Root writes
  skip the guard (no sedes collected).
- **Owners** only where `insula_via_continet(via, d->via)`.
- **Canons**: the nearest `CanonRami` whose path contains `via`
  (longest), else the root canon; plus every branch canon strictly
  below `via`. Nothing between the nearest canon and `via` has a
  canon, so it sees the change; ancestors above it exclude it from
  their view (`visio_nodi`), so they cannot see it.
- `InsulaActarius` gets `via` (conservatio ignores it until S8e-3).

Behaviour change, deliberate: state made invalid OUTSIDE the gate's
view (a canon installed after the node already broke it) no longer
blocks writes elsewhere - only writes that canon can see. Tests in
probatio_insula_profunda: custodia (plant: guard off -> red),
actarius, scopus_canonum (invalid sibling ignored; invalid root
ignored under a branch canon but still judging a canon-less branch;
canon on level 1 judges a write at level 3). Test bug on the way:
b's canon does not allow child c, so c needs its own canon for the
root-skip case (cause printed, not guessed).

Numbers: rows 50/100/200 = 52/103/184 us (S8e-1: 84/192/482), chain
22 = 86 us (113). vicus barely moved (10 tabs 195 us) and its small
rows got noisier: `sample` at 2 tabs shows 72% in
`actarius_conservationis` -> `volumen_plagulam_condere` -> SQLite
commit (WAL pwrite) - every focus write is a transaction. The gate
itself is ~11%.
