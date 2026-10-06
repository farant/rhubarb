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

**R3 - pictor on a branch.** Actions, componere, figurae read and
write through an `InsulaRamus` in their contexts; canons and owners
attached to the branch; `pictor_applicatio` hands the root branch.
All pictor suites, the H0 golden and the terminal replay unchanged.

**R4 - scriba on a branch.** Likewise, including the gesture flush;
all scriba suites and its replay unchanged.

Track T - schirmata (the host):

**T1 - the host store.** Host canon (roots + mount elements + the tab
list in the durable root, `activa` in the ephemeral); mounts created
from the durable tab list on open; one volume, a namespace per mount.

**T2 - composition and registries.** The tree = tab bar (a component
per tab) + the ACTIVE mount's subtree; action and figura lookup through
the active mount's registries (read first how the dispatcher and
`pingere` take a registry - a composite registry or a swap on switch);
the host writes each mount's `superficies_*` (its area).

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
