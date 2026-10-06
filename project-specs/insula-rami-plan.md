# insula rami — plan (one store, branches for widgets; then schirmata)

*Started 2026-10-05 at the schirmata interview (track (c) of
ludus_tessera, after scriba). Fran: "i would see these a little bit
more like widgets on a redux store as far as state goes … some kind of
state service … they could write their state to it but in a sense it
would be like a curried branch of the parent state owner / wrapper
that's invoking them".*

## I. Decisions (Fran, 2026-10-05)

1. **One store, owned by the host; apps and widgets get a BRANCH.** An
   `InsulaRamus` = the repository + a node path; reads, writes and
   mutators are relative to that node, so an app never knows where it is
   mounted and two instances get two branches. This is the
   applications brainstorm's M1 ("verba insulae", node-path addressed -
   the keystone) and M2 (node paths), with its first real consumer.
2. **Branches first, then schirmata.** A framework track: path-scoped
   insula access, canons composed by element, owners per path, the
   dispatcher handing each action the branch of the component it fired
   on; pictor and scriba migrated to take a branch (no behaviour change -
   their suites prove it); then schirmata mounts them.
3. **Switching: Ctrl-A prefix + tab clicks.** The legacy tmux-style
   prefix (Ctrl-A, then n / p / 1-9) and clicking a tab in the tab bar.
4. **The editor keeps the name `scriba`.** lapide FR-008 also calls its
   (unbuilt) scripting library `scriba` (applications brainstorm M7);
   that library takes another name when it is built.

## II. What exists (to read before tasks)

- insula: two roots (durable, ephemeral), gates `mutare_durabile/
  _ephemera` (mutator on a duplicate of the root), one canon per root,
  owners per ROOT attribute (`insula_dominum_ponere`), reads of root
  attributes (`insula_attributum`).
- Outside the insulae, per app today: the document (volumen + act log),
  the Motus gesture slot (one per dispatcher), figurae contexts.

## III. Architecture sketch (read 2026-10-05; tasks after Fran's
answers to IV)

- **State: one store, branches.** Host roots `<documentum>` /
  `<ephemera>`; each mount a child subtree. `InsulaRamus` = repo + node
  path: `insula_ramus_attributum`, `mutare_ramum(ramus, genus, fn,
  ctx)` (the mutator gets the branch's node inside the copied root),
  attribute remove/set relative to the node.
- **The gate today** (`lib/insula.c:274`): the WHOLE root is copied
  through text, the mutator runs, owners are judged on ROOT attributes
  only (`dominos_iudicare` :223), then the root canon. Needed: owners
  per (path, attribute); canons per branch (`canon_iudicare` takes any
  element as its root - each mounted subtree judged by its app's canon,
  the host canon declares only the mount elements). Scaling: every
  write copies the whole store (fine for a handful of apps; named).
- **Only the ACTIVE app is composed.** Each frame's tree = tab bar +
  the active mount's subtree. That removes id collisions between apps
  (both have `status`, `prospectus`), figura collisions (both register
  TITULUS/PROSPECTUS with different contexts) and action collisions
  between two instances of one app (same name, different contexts):
  registries (actions, figurae) per mount, the host resolves through
  the active mount.
- **The branch travels in the app's context** (actions, componere,
  figurae already take one) - no framework signature change. The
  surface an app sees (`superficies_*`) is written by the HOST into the
  app's branch: its area (window minus tab bar); app layouts unchanged.

## IV. Decisions, second round (Fran, 2026-10-05)

5. **A mount is an element named for its app kind:** `<scriba
   id="s1" …/>`, `<pictor id="p1" …/>` under the host roots; the app's
   canon judges its element by name; two tabs of one app = two elements
   with different ids.
6. **ONE host volume.** All tabs' documents live in the host's volume,
   namespaced: historia gains a NAMESPACE (prefix on its act genus,
   `<ramus>`/`<coniunctio>` markers, `checkpoint/…` keys; the client's
   manifest key likewise). Empty namespace = today's bytes (H0 golden).
7. **Background tabs are frozen:** switching away flushes the leaving
   app's pending gesture; no events or frames until active again.
8. **The tab list is durable** (which apps, which documents, which tab
   is active survives a restart).

## V. Tasks

Track R - branches (framework; no behaviour change for standalone apps):

**R1 - `InsulaRamus`.** A branch = repo + path (root, or a child
element of the root by kind + id). Reads (`insula_ramus_attributum`),
the gate (`mutare_ramum(ramus, genus, fn, ctx)` - the mutator receives
the BRANCH's node inside the copied root), set/remove relative to the
node; owners per (branch, attribute) and canons per branch, judged on
the branch's node. A ROOT branch behaves exactly like today's repo
calls - standalone apps use the root branch. Tests with a toy store:
two branches of one kind isolated; owners and canons per branch
(a refused write in one branch leaves the other untouched); root
branch == today. Plants: owner judged on the root only; canon of one
branch applied to the other; mutator handed the root.

R1 as built: `InsulaRamus` in insula (`insula_ramus`, `_radix`,
`_nodus`, `_attributum`, `mutare_ramum`, owners and canons per branch);
the root canon judges a view without delegated branches. Convention
found: a mountable app's canon names its root element after the app
kind (`canon_iudicare` checks the element name). Five plants.

**R2 - historia namespace.** `historia_creare/_aperire` take a
namespace (empty = today); pictor_documentum and scriba_documentum pass
theirs through (and prefix their manifest). Two documents in one
volume replay independently (undo in one never touches the other).
H0 golden byte-identical with the empty namespace.

R2 as built: a `spatium` parameter after the volume in
`historia_creare/_aperire`, `pictor_documentum_creare/_aperire`,
`scriba_documentum_creare/_aperire` ("" = old bytes; H0 golden
identical). Prefix `s/` on genus, markers, checkpoint keys, manifest.
Three plants (bare markers caught only on reopening).

**R3 - pictor on a branch.** Actions, componere, figurae read and
write through an `InsulaRamus` in their contexts; canons and owners
attached to the branch; `pictor_applicatio` hands the root branch.
All pictor suites, the H0 golden and the terminal replay unchanged.

R3 as built: `InsulaRamus` in `PictorActiones` / `PictorCompositio`
(unset = the given repo's root); canon roots renamed `pictor` (+ `id`);
`probatio_pictor_ramus` (two mounts, real canons/owners on p1). Three
plants.

**R4 - scriba on a branch.** Likewise, including the gesture flush;
all scriba suites and its replay unchanged.

R4 as built: `ScribaActiones.ramus`, `ScribaCompositio.ramus`; helpers
take the branch; canon roots `scriba` (+ `id`); `probatio_scriba_ramus`
(two scribae, one store, one volume). Found: the undo position is not
persisted (historia reopens at the end of the log) - open question.
**Track R done.**

**R5 - persist the undo position (Fran, 2026-10-05: "persist as an
act").** `historia_revocare/_reficere` append a historia marker
`<cursor ad="seq"/>` (genus `cursor`, namespaced); `historia_aperire`
reopens at the last marker unless a client act or `<ramus>` followed
it (then at the end, as today). Append-only; redo works after
reopening. The H0 golden CHANGES on purpose (its session undoes):
inspect the diff (only cursor markers added, sigilla per state
unchanged) and re-pin.

R5 as built: `cursorem_notare` in revocare/reficere; `aperire` honours
the last non-stale marker. H0 golden re-pinned after inspection (only
markers added, seqs shifted, sigilla unchanged). Three plants.

Track T - schirmata (the host):

**T1 - the host store.** Host canon (roots + mount elements + the tab
list in the durable root, `activa` in the ephemeral); mounts created
from the durable tab list on open; one volume, a namespace per mount.

T1 split (2026-10-05, after reading both app assemblies):
- **T1a - mounting per app.** `pictor_montare` / `scriba_montare`
  (repo + id + namespace): open/create the document in the namespace,
  register the app's canons and owners on its branch FIRST (so the host
  canon never sees an undelegated mount), create the mount element
  with the app's initial attributes if absent, build the contexts
  (branch inside) and the app's own registries (actions, figurae).
  Standalone = a store whose root IS the app element + one mount on the
  root branch + a dispatcher (no behaviour change). Test: both apps
  mounted into one store.
  T1a as built: `insula_ramum_initiare`; `pictor_montare`,
  `scriba_montare` (+ `…Montatio`); standalone rebuilt on them;
  `probatio_montatio`. Plants: owners before initiation; namespace
  ignored; initiation appending a duplicate - caught.
- **T1b - the host store.** Host canon; the durable tab list as a
  MANIFEST entry in the volume (`schirmata/tabulae`, overwritten on
  change - configuration, not history; decided here, veto-able); app
  kinds registered by the host's main (kind -> mount function), so the
  host library knows no app; open = read the tab list, mount each tab;
  fresh volume = a default list.

  T1b name (Fran, 2026-10-05): the host library is **`vicus`** (a
  Roman street of insulae) - `schirmata` is the legacy concha screen
  system and stays. Tab list manifest `vicus/tabulae`
  (`<tabulae activa><tabula id genus titulus/>…</tabulae>`; the active
  tab persists - decision 8); app kinds register mount size + mount
  function; host canons in `apps/vicus/canones/`.

  T1b as built: `include/vicus.h`, `lib/vicus.c` (kinds by size +
  mount function; tab list + active tab in `vicus/tabulae`; unknown
  kinds kept unmounted), `probatio_vicus`. Four plants (one needed a
  reopen right after adding).

**T2 - composition and registries.** The tree = tab bar (a component
per tab) + the ACTIVE mount's subtree; action and figura lookup through
the active mount's registries (read first how the dispatcher and
`pingere` take a registry - a composite registry or a swap on switch);
the host writes each mount's `superficies_*` (its area).

T2 design (agreed with Fran, 2026-10-05), two commits:
- **T2a - registries and the kind's description.** vicus owns ONE
  `ActioRegistrum` and ONE `FiguraRegistrum` (handed once to the
  dispatcher / glue - pointers never change); on open and on switch it
  empties them, re-registers its own entries, then copies in the active
  mount's (`actio_registrum_vacare/_miscere`, `figura_registrum_
  vacare/_miscere`) - two tabs of one app never collide. Kind
  registration gains `describere(montatio, &VicusFacies)` (actions,
  figurae, componere + ctx, image source + ctx) - wrappers in the
  host's main, so vicus and the apps stay unaware of each other. A
  composite image source delegates to the active mount's.
  T2a as built: registries emptied + merged on open/switch; `VicusFacies`
  + `describere`; `vicus_imago_fons`. Three plants.
- **T2b - composition and surfaces.** Host `componere`: root = a
  one-row tab bar (a component per tab, a click action) + the active
  app's tree (its own componere and ctx) offset below the bar. Each
  app keeps reading `superficies_*` from its branch; a host action on
  resize (and on open) writes each mount's area (window minus tab bar)
  into its branch - recorded, replayable, componere pure.
  T2b as built: `vicus_componere` (ctx = Vicus*): root `vicus`
  (action `vicus.magnitudo`), ONE bar component `vicus.tabulae`
  (`PARTES_INDEX`, full width x 8 px) whose host figura draws every
  title and inverts the active one, then the active app's tree with
  its root's `fines.y` += 8 (children are parent-relative, so the
  whole subtree - painting and hit-testing - moves). Host entries are
  registered BEFORE the active mount's are merged. Surfaces: window
  minus the bar, written into every MOUNTED branch on open and on
  `EVENTUS_MUTARE_MAGNITUDINEM` (the dispatcher routes it to the root,
  after writing the root's own surface), as writer `"dispensator"` -
  the apps' `domini` give `superficies_*` to that writer, so the host
  writes under that name (saved and restored). Deviation from the
  design: tab components and the click action move to T3 (switching),
  where the click has something to do. Eight plants.

**T3 - switching.** Ctrl-A then n / p / 1-9 (the pending prefix is host
ephemeral state), clicking a tab; leaving flushes the app's gesture and
swaps the Motus gesture slot to the arriving app's.

**T4 - the app.** `apps/schirmata/` in both targets, mounting pictor
and scriba; replay proof (a session that types, switches, draws,
switches back - the same store and volume through the terminal path);
Fran's look.

**T5 - RELATIO.**

## AUDIENDA

- Whether `canon_iudicare` on a non-root element checks that element's
  name against the canon's radix (needed for per-branch canons).
- How the dispatcher, focus and `pingere` would take per-mount
  registries (T2 reads before designing).
- The gesture slot is one per dispatcher: swap on switch (T3) vs one
  per mount.
- Every write copies the whole store through text: cost with N tabs
  (measure at T4).
- **Decided (Fran):** persist the undo position as an act (R5).
- Framework writes still go to the ROOT: Motus's pan/zoom flush
  (`motus_effundere`), the dispatcher's `focus` and `superficies_*`.
  In a host they must reach the active mount's branch (T2/T3).
