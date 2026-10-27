# semitae — routes as slices of the state tree

*Born 2026-10-19 in secunda, from a conversation with Fran after the
calendar week view and the volume index. Fran: "we might want to start
having canonical routes for each widget/sub-widget and views/state
within those widgets ... more aggressive remix approaches to route
driven views". Then, refining: query parameters only SET state;
absent parameters keep or reset by a declared rule; pages have nested
views that are siblings, not only parent and child; routes are
XML/STML by default, with a canonical translation to a URI or a list
of URIs.*

*Status: design decided with Fran (§IX). R0 DONE 2026-10-19 (canon
attributes, loader rules, montationes accessors - lib/organum.worklog.md).
R1 DONE 2026-10-19 (lib/semita.c, lib/semita.worklog.md). R2 DONE
2026-10-19 (sancti on routes - lib/sancti_visus.worklog.md). R3 DONE
2026-10-19 ($via, per-pane history Ctrl+O/Ctrl+I, semita_legere_plenam -
lib/vicus.worklog.md). Next R4 (calendar, nomenclator, pictor).*

## I. Doctrine (decided with Fran, 2026-10-19)

1. **A route (semita) is a partial state tree.** It is an STML
   fragment shaped like the insula itself: one element per mounted
   widget (root = the pane's genus, children = its declared
   montationes by name), attributes = fields that opted into routing.
   It is not a path: siblings and orthogonal sub-views are just
   several children.
2. **Canonical form is STML.** A URI string is a derived, canonical
   translation (§IV), not the truth. One state → exactly one canonical
   route (fields in manifest order, defaults omitted).
3. **Routes only set state.** Applying a route never runs an action
   (no "create item", no "open prompt"). Visiting a route twice equals
   visiting it once - safe to restore, share, replay, go back to.
   Side effects go through the declared `actiones` (Remix's
   loader/action split).
4. **Absent means: keep or reset, by declared scope.** A field scoped
   to the BINDING resets to its `absens` default when the binding
   changes (text page, selected item, scroll page); a field scoped to
   the WIDGET keeps its value (sort order, view mode). Present
   attributes always win.
5. **Inherited bindings never appear in a route.** A child whose
   entity comes from its parent (`<ligamen ... a="parens">`, e.g. the
   day's notes follow the calendar's day) cannot be contradicted by a
   route. A child with an independent binding carries its own.
6. **What is never routable:** entity data (documents), ephemeral
   UI state (prompt text, hover, menus), observed state (ports: time,
   window size), derived state, and kinetic state in flight. The
   manifest opts fields IN; the default is out. (R4, 2026-10-19:
   `vita` answers "restore after restart?", `semita` answers
   "addressable?" - separate questions. A transient field may be
   routable: the calendar reopens on today, yet
   `calendarium/dies/2026-10-19` addresses a day.)

## II. What exists today (cited)

- **State is already an STML tree.** Each pane mounts a branch
  (insula ramus) of typed attributes; the manifest declares them:
  `organum.canon:62-120` (campus: typus, vita, rhythmus, sedes,
  aspectus, referentia, absens, scriptor). Lifetimes and the state
  categories: `project-specs/status-genera-census.md` §II-§V, §XI.
- **Bindings exist as `ligamina`** with `a="parens"`
  (`organum.canon:36-45`, `include/organum.h:69-73`).
- **Children are declared in the canon but nowhere used:**
  `<montationes>`/`<montatio nomen genus>` (`organum.canon:118-126`);
  every manifest has `<montationes/>`; `include/organum.h` has no
  accessor. Nested widgets are mounted by hand (the calendar's notes
  charta: `lib/kalendarium.c` `notas_montare`, branch `<id>_notae`).
- **Four hand-rolled argument languages** (one per pane), each its own
  parser: `kalendarium_argumentum_ponere` (`lib/kalendarium.c:4144`:
  "YYYY-MM", "YYYY-MM-DD", "agenda"), `nomenclator_argumentum_ponere`
  (`lib/nomenclator.c:2873`: person id), `sancti_visus_argumentum_ponere`
  (`lib/sancti_visus.c:2085`: saint id), pictor's
  (`lib/vicus_applicatio.c:1085`: picture name). Wired through
  `VicusFacies.argumentum_ponere` (`include/vicus.h:117-121`).
- **Navigation exists:** `vicus_acervo_mutare(v, genus, argumentum)`
  (`lib/vicus.c:2724`) - "bring this genus to the front with this
  argument". Cross-pane links already use it (calendar → saint,
  birthday → contact, contact → calendar day).
- **History is only a stack of panes** (`acervus`; Esc pops), not of
  states within a pane.

## III. The model

**Field roles** (new, per campus in the manifest):

| role | meaning | in URI form | when absent |
|---|---|---|---|
| `ligatio` | the binding: which entity / scope this view shows | path segments | keep current (navigation within the widget) |
| `status` | optional widget state a caller may set | `?key=value` | keep (widget scope) or reset (binding scope) |
| (none) | not routable | - | untouched |

**Reset scope** for `status` fields: `ambitus="ligatio"` (reset to
`absens` when any `ligatio` field of the same widget changes) or
`ambitus="organum"` (default: kept).

**Children:** `<montatio nomen="notae" genus="charta"/>` declares a
child slot. A route child element `<notae .../>` addresses it. If the
child's binding is inherited (`ligamen a="parens"`), its `ligatio`
fields are refused in a route; otherwise they are allowed.

**Worked example** (calendar, after R4):

```xml
<!-- canonical route -->
<calendarium visus="dies" dies_electus="2026-10-19">
  <index res_electa="17"/>
  <notae pagina="2"/>
</calendarium>
```

`visus` and `dies_electus` are `ligatio`; `res_electa` (`status`,
`ambitus="ligatio"`) resets when the day changes; `notae` inherits the
day from its parent, so only its `pagina` appears.

## IV. URI forms (derived, canonical)

- **Single URI:** root genus, then the root's `ligatio` values in
  manifest order as path segments; then every `status` value of the
  whole tree as a query key, dotted by montatio names from the root;
  keys in tree order, defaults omitted.

  ```
  calendarium/dies/2026-10-19?index.res_electa=17&notae.pagina=2
  sancti/sanctus/thomas-aquinas
  sancti/index?quaestio=therese&ordo=congruentia
  ```

- **List of URIs:** one URI per node that has its OWN binding (the
  root and every child with an independent `ligatio`), each with its
  own status keys - for indexing, logs, "what entities is this view
  showing", and as projection keys later (§VIII). A tree whose
  children all inherit their binding yields a list of one.
- **Escaping:** values percent-escape `/ ? & = % #` and spaces;
  `.` is reserved in keys. Parsing a URI yields the same STML tree the
  canonical form gives (round-trip both ways, tested).

## V. Semantics

- **Applying a route** (`semita_applicare`): validate the WHOLE tree
  against the manifests first (element names = genus/montationes,
  attributes = routable fields, values = declared types and options);
  any error refuses the route and names the element and attribute.
  Then ONE write through the gate (scriptor = the widget's action):
  `ligatio` fields, then binding-scoped resets, then present `status`
  fields. Children are applied after their parent (an inherited
  binding is in place before the child's state).
- **Reading a route** (`semita_legere`): current branch → canonical
  STML (routable fields only, defaults omitted).
- **Idempotent and total:** applying `semita_legere(x)` to state `x`
  changes nothing; any valid route is applicable from any state.

## V-a. The binding is named whole (R4d, Fran 2026-10-20)

A route **names the binding** when its node carries any `ligatio`
attribute (even an empty one). Then every `ligatio` field it names is
set, every `ligatio` field it does not name resets to `absens` (or is
removed), and the binding-scoped `status` resets follow as before. A
node with **no** `ligatio` attribute keeps the current binding and sets
only the `status` it names - the "caller sets state on whatever is
showing" form.

| route | effect |
|---|---|
| `calendarium/dies` | day view, **today** (`dies_electus` reset) |
| `calendarium?rubrica_filtrum=domus` | current view and day kept, filter set |
| `sancti/index` | index, open saint closed |
| `sancti` | pane brought forward, nothing changed |
| `nomenclator/` | index (trailing `/` = binding named, all at defaults) |

**Canonical reading** names the binding even when every binding field
is at its default: the first segment is written (empty if it has no
default). `calendarium/mensis` (was `calendarium`), `nomenclator/`.
So a URI read from a state and applied anywhere reproduces that
state's binding (closes the R1 AUDIENDA for bindings; `status` at
default is still omitted and kept when absent). ROOT only (R4d): a
child's binding is named in the canonical form only when it differs
from its defaults - forcing it would put `child.field=` into every
URI, and no pane routes a child yet. In STML text an empty attribute
(`visus=""`) parses as an attribute WITHOUT a value; it still names the
binding (semita checks presence, not value).

## V-b. Aliases and symbolic values - resolve at the boundary (R4e)

**Rule:** an alias is resolved ONCE, where intent enters (a command
argument, `$via`, a link, the saved restart route). Everything
downstream - the applied state, history, canonical URIs, projection
keys (R5) - sees only concrete values. History never stores "today";
a bookmark may.

**Symbolic values per type.** A type may declare a few; the first is
`dies`: `hodie`, `heri`, `cras`. `organum_valorem_iudicare` accepts
them; `semita_resolvere` turns them concrete with a resolver the caller
supplies (the calendar's clock port); `semita_applicare` REFUSES an
unresolved symbolic value, so forgetting to resolve fails loudly.

**Aliases in the manifest** (`<cognomina>`), data only:

```xml
<cognomina>
  <cognomen nomen="agenda" semita="calendarium/agenda"/>   <!-- fixed -->
  <cognomen typus="dies" semita="calendarium/dies/{}"/>    <!-- typed -->
</cognomina>
```

A fixed alias matches the bare argument exactly; a typed alias matches
a bare argument that is a valid value of the type (its symbolic values
included) and substitutes it for `{}`; tried in manifest order; a URI
argument (`<genus>`, `<genus>/...`, `<genus>?...`) is never an alias.
Aliases that need computation stay code in the pane's argument
function (calendar `YYYY-MM` -> today if inside that month; pictor
create-on-unknown). A typed alias never matches the EMPTY argument
(only a neutral `<cognomen semita=.../>` does). A computed alias that
overlaps a type is tried FIRST: `dies` accepts the shape `2026-10`, so
the calendar checks `YYYY-MM` before the manifest's aliases (R4e).

| pane | aliases |
|---|---|
| sancti | `""` -> `sancti/index`; typus `textus` -> `sancti/sanctus/{}` (R4e: ids carry hyphens, `nomen` refuses them) |
| calendarium | `agenda`; typus `dies` -> `calendarium/dies/{}` (so `hodie`); `YYYY-MM` stays code |
| nomenclator | `""` -> `nomenclator/`; typus `numerus` -> `nomenclator/{}` |
| pictor | none declared; create-on-unknown stays code |

## VI. History (per pane)

Each routable field declares `historia="impelle"` (push: view or
binding changes) or `historia="substitue"` (replace: stepping a day,
moving a selection, typing a filter); default `substitue`. A pane keeps
its own back/forward list of canonical routes. Keys: OPEN (§IX).

**Amended 2026-10-22 (Fran, genera-datorum D5): two lists per TAB,
one per side - not per pane.** Per-pane lists could not go back across
panes (terminal -> Cmd+Shift+F -> search -> Ctrl+O stayed in search).
Ctrl+O / Ctrl+I step the FOCUSED side's list. LEFT: an `impelle`
change of the left pane pushes (scriba's page is now route state,
`scriba/<pagina>`: next/prev, #links, search hits all push). RIGHT: an
`impelle` change of the stack's front pane pushes, and so does a change
of the front pane itself (open, cycle, Esc, $via, a search hit).
`substitue` replaces that pane's current entry; a focus click does
nothing. A step brings the entry's pane to the front (reopens it if
closed) and applies its route. The terminal consumes Ctrl+O itself.
lib/vicus.worklog.md.

## VI-e. Restart: the saved argument is the route (R4d)

Vicus saves each pane's argument and reopens with it. Until R4d that
was the argument the pane was OPENED with - stale after navigation or
history. Now, after any refresh in which a route-taking pane's
**persistent route** changed (the canonical route with
`vita="transiens"` fields dropped), vicus stores it BESIDE the pane's
argument (index attribute `semita`); a restart mounts the pane with its
argument, then applies the saved route (then `semitam_secuta`). The
argument stays the pane's IDENTITY (`vicus_acervo_aperire` reuse, the
Cmd+]/[ cycle match genus + argument) - overwriting it would make the
cycle open a second agenda calendar (found 2026-10-20 before coding).

- `vita` keeps deciding what survives a restart: the calendar on day
  view saves `calendarium/dies` and reopens on today; contacts saves
  only its list mode and filter (`persona_aperta` is transient), so it
  reopens on the index - a person opened by a link no longer comes
  back after a restart while one opened by Enter didn't (now neither
  does).
- **Pictor's `pictura` becomes `vita="persistens"`** - the picture is
  the pane's document, not a passing view; a restart reopens the last
  picture shown.
- The saved index is written only when the persistent route changes
  (stepping days writes nothing). The FIRST computation for a pane
  with no argument is recorded silently (reopening with no argument
  gives the same state) - otherwise every launch appended an act;
  a pane opened WITH an argument saves at once (the argument may carry
  transient state: `calendarium/dies/2026-10-19`).

## VI-f. Regular state vs modal state; what Esc means (Fran 2026-10-22)

- **A reopened pane inherits the closed pane's regular state, per
  tab.** Closing a pane (Esc at its top level, T4d) and reopening its
  genus in the same tab gives the same pane id and the same insula
  state - the last saint, the last list mode. Intended.
- **Modal state is the exception: it lives on the HOST** (the tab or
  pane record in vicus), never in the pane's insula state, so it is
  never saved or rehydrated by construction. Today the only modal is
  the summoned prompt (Cmd+F / Cmd+Shift+F / `$search`, comites P5a):
  the open prompt IS the visible modal, so "Esc: cancel" is honest;
  Esc cancels the whole summon (right history back, focus back).
- **Esc is not a universal back button.** Inside a widget Esc goes UP
  (saint -> list, day -> month, contact -> list, close prompt, clear
  filter); at a widget's top level it closes the widget (T4d). Going
  back to the PREVIOUS widget is Ctrl+O. The saints field `origo`
  (Esc closed a saint opened by the calendar or `$saints(id)`) was a
  modal flag kept in pane state: it leaked into a reopened pane (a
  `$saints` over contacts showed an old saint and Esc dropped to
  contacts), and the key hint "Esc: Back to list" lied. Removed: Esc
  in a saint always goes to the list.

## VI-a. Actions declare their result route (POST-redirect-GET as contract)

On the web the result of a POST has no address - hence "Confirm Form
Resubmission" and the redirect-after-POST workaround. Here an action
may declare the route it lands on; its placeholders are filled from
the action's declared OUTPUTS (never from expressions - canon has
none):

```xml
<actio nomen="calendarium.nova">
  <exitus nomen="id" typus="numerus"/>
  <semita><index res_electa="{id}"/></semita>   <!-- relative to self -->
</actio>
```

The host runs the action, then applies the result route with history
`impelle` - so "back" returns to before the creation, and the created
item has an address immediately. Actions without `<semita>` leave the
view where it is.

## VI-b. Relations: universal verbs, per-view answers

Precedent: HTML link relations (`rel="next" prev up search canonical
alternate"`, the IANA registry), the macOS responder chain (`copy:`,
`find:` implemented per view), Emacs generic commands. A small
registry of UNIVERSAL relations lives in `organum_commune.stml`
(name, description, parameters); each organum declares which it
answers and how. The host binds relations to global keys and lists
the available ones in a "Go" menu; links may be relative
(`rel:hodie`).

First registry (proposal):

| relatio | meaning | parameters |
|---|---|---|
| `proxima` / `prior` | next / previous at this view's natural step | - |
| `superior` | up: the containing view (record -> list, week -> month) | - |
| `index` | the widget's home view | - |
| `hodie` | the view's "now" (today's week, today's saint) | - |
| `quaestio` | search within this widget | `q` |
| `alterna` | another view of the same entity (week <-> day) | - |

A view answers a relation in one of three ways (no expressions):

```xml
<relationes>
  <relatio nomen="index"><sancti visus="index"/></relatio>        <!-- static -->
  <relatio nomen="quaestio"><sancti visus="index" quaestio="{q}"/></relatio>
  <relatio nomen="proxima" functio="sancti.proxima"/>             <!-- computed -->
</relationes>
```

**First slice shipped (comites P5a, 2026-10-22):** Fran named the
element `<aditus>` with `<iter nomen="..." semita="URI"
advocare="verum"/>` children (not `<relationes>`/`<relatio>`); only
`quaestio` exists so far, bound to Cmd+F. `semita` is a plain URI (any
genus - the calendar's search answer is `indagatio?genus=res`); absent
`semita` means "this pane itself", answered by `facies.advocatum`.
`{q}` parameters and `functio` answers are not built yet. See
`project-specs/comites.md` P5a.

A `functio` is registered like an actio but RETURNS a route (pure:
reads state, writes nothing); the host applies it. So "routes only set
state" holds for relations too. Undeclared relation = not offered
(the key does nothing, the menu omits it).

**Opt-in, not a contract (Fran, 2026-10-19):** the registry is a
STANDARD LIBRARY of relation types, not an interface every page must
implement. Most pages declare a few (a record view: `superior`,
`proxima`/`prior`; a calendar: also `hodie`, `alterna`). What the
library buys is CONSISTENCY: the same relation always has the same
key, the same control and the same menu entry, generated from the
declarations - a shared membrum (like `membrum__praecepta` for key
tables) renders the controls of the declared relations (arrows,
"Today", "Up"), so widgets stop hand-drawing their own navigation
buttons. Adding a relation type to the library is a deliberate act
(a second widget wanting it), like promoting a state category.

## VI-c. Versions and renames

Routes carry the manifest version on the root (`<calendarium
versio="2" ...>`; URI form `calendarium@2/...`, omitted when current).
A manifest that renames a routable field or option declares it:

```xml
<renominationes>
  <renominatio versio="1" campus="dies" novus="dies_electus"/>
  <renominatio versio="1" campus="visus" optio="hebdomas" novus="septimana"/>
</renominationes>
```

Applying an older route maps through the declared renames (chained
across versions); anything undeclared is refused, naming field and
version. Bookmarks and links in notes survive refactors without
redirect tables kept by hand.

## VI-d. Parked: "as of" routes (`@seq`)

Unique to event sourcing: a route plus a volume sequence number could
open the view AS OF that point (documents can project at any seq).
Time-travel permalinks. Parked until projections exist (R5+): the
pane would have to render read-only from a historical projection.

## VII. Text for approval

**Canon** (`organum.canon`, campus attributes; montatio gains nothing
new - it exists):

```xml
<attributum nomen="semita"  genus="electio">
  <optio>ligatio</optio>
  <optio>status</optio>
</attributum>
<attributum nomen="ambitus" genus="electio">
  <optio>ligatio</optio>
  <optio>organum</optio>
</attributum>
<attributum nomen="historia" genus="electio">
  <optio>impelle</optio>
  <optio>substitue</optio>
</attributum>
```

Loader rules (organum_legere, not canon): `semita` never on
`sedes="motus"` nor `vita="probatio"` (R4 - was "only persistens" in
R0); `ambitus` and `historia` only with `semita`;
`ambitus` only with `semita="status"`.

**Header** `include/semita.h` (new library, lib/semita.c):

```c
/* semita.h - semitae: viae ut segmenta arboris status (project-specs/
 * semitae-spec.md). Semita = fragmentum STML formae insulae: elementum
 * per organum montatum (radix = genus, liberi = montationes nomine),
 * attributa = campi manifesti 'semita' declarati. Forma canonica
 * STML; URI derivatum. Semita statum SOLUM ponit (numquam actionem). */

/* semitam applicare ramo: arbor tota primum iudicatur (nomina,
 * campi, typi); vitium = nihil scriptum, *vitium nominat elementum et
 * attributum. Deinde scriptura UNA: ligatio, resetum ambitus
 * ligationis, status; liberi post parentem. */
b32
semita_applicare (
        constans Organum* organum,
              InsulaRamus* ramus,
        constans StmlNodus* semita,
                  Piscina* piscina,
                   chorda* vitium);

/* semita canonica status currentis: campi 'semita' soli, absentes
 * (valor = absens) omissi, ordine manifesti */
StmlNodus*
semita_legere (
        constans Organum* organum,
        constans InsulaRamus* ramus,
                  Piscina* piscina,
      InternamentumChorda* intern);

/* URI canonicum (forma IV): genus/ligatio...?clavis.punctata=valor */
chorda
semita_ad_uri (
        constans Organum* organum,
      constans StmlNodus* semita,
                  Piscina* piscina);

/* URI -> semita (eadem arbor quam forma canonica); NIHIL + *vitium
 * si malum */
StmlNodus*
semita_ex_uri (
        constans Organum* organum,
                   chorda  uri,
                  Piscina* piscina,
      InternamentumChorda* intern,
                   chorda* vitium);

/* URI singula per nodum ligationis propriae (radix et liberi non
 * hereditarii) - Xar de chorda */
Xar*
semita_ad_uri_lista (
        constans Organum* organum,
      constans StmlNodus* semita,
                  Piscina* piscina);
```

(Signatures take the root organum; child organa are found through the
manifests' montationes - the loader needs `organum_montatio_*`
accessors, R1.)

## VIII. Phases

| Phase | Content |
|---|---|
| **R0** | This spec approved; canon attributes; loader rules; `organum_montatio_*` accessors (montationes read at last). |
| **R1** | `semita.h` + lib: apply / read / URI both ways / URI list, against a FIXTURE manifest with a child and an inherited binding; round-trip and refusal tests (red first, plants). |
| **R2** | Migrate the simplest pane: sancti (`visus`, `sanctus_apertus` ligatio; `ordo`, `quaestio` status). Its `argumentum_ponere` becomes "parse URI, apply route"; `$saints(x)` keeps working. |
| **R3** | vicus: `vicus_acervo_mutare` accepts a route; `$genus/...` commands; per-pane history (§VI). |
| **R4** | Calendar: montationes declared (`notae`), notes binding inherited; migrate its argument language; then nomenclator, pictor. The four hand parsers go away. |
| **R4d** | V-a (binding named whole; canonical first segment) and VI-e (persistent route = saved argument; pictor `pictura` persistent). |
| **R4e** | V-b (symbolic `dies` values + `semita_resolvere`; `<cognomina>` fixed and typed; the panes' hand aliases shrink to the computed ones). |
| **R5** | Projections (separate spec, `proiectiones`): a route's URI list = the projection keys a view needs ("loaders"). |
| **R6** | Action result routes (VI-a), relations registry + first answers (VI-b: sancti, calendar), host keys and "Go" menu. |
| **R7** | Versions and renames (VI-c) - before the first manifest change that breaks a stored route. |

## IX. Decisions (Fran, 2026-10-19)

1. **History keys:** Ctrl+O (back) / Ctrl+I (forward), per pane (vim's
   jump list) - per SIDE of a tab since 2026-10-22 (§VI amendment). Cmd+[ / Cmd+] stay the pane cycle.
2. **Commands stay:** `$saints(thomas-aquinas)` remains an English
   alias that compiles to the route `sancti/sanctus/thomas-aquinas`;
   `$genus/...` route commands are added beside them (R3).
3. **Scope:** one pane's tree first (R1-R4); whole-layout routes later.
4. **Names approved:** library `semita`; campus attributes `semita`,
   `ambitus`, `historia`; manifest elements `relatio`, `renominatio`,
   `exitus`.
5. **Relation keys - layered, defined in ONE place.** The relation
   registry (`organum_commune.stml`) carries each relation's universal
   chord, so a change is one edit and menus / help lines / controls
   follow:

   | relatio | chord | precedent |
   |---|---|---|
   | `proxima` / `prior` | Option+Down / Option+Up | |
   | `superior` | Cmd+Up | Finder "enclosing folder" |
   | `alterna` | Cmd+Down | Finder "open" |
   | `hodie` | Cmd+T | |
   | `quaestio` | Cmd+F | universal "find" |

   A widget MAY also bind its natural single letter to a relation it
   declares (calendar `t` = `hodie`, `Esc` = `superior`), declared in
   its manifest, so both keys show in menus and cannot drift apart.
   Selection movement (`j`/`k` in a list), edits and actions stay
   local keys - relations are navigation between views only.
   Checked 2026-10-19: the fenestra menu bar claims Cmd+Q Z Shift-Z X
   C V A only (`lib/fenestra_macos.m:698-720`); vicus claims
   Cmd+digit and Cmd+[ ]. Cmd+T, Cmd+F, Cmd+Up/Down are free.
6. **Interception follows declaration.** The host intercepts a
   relation chord ONLY when the focused pane declares that relation;
   otherwise the chord passes through. A terminal pane declares none,
   so Option+arrows still reach the shell (the terminal encoder
   forwards Option/Cmd: `lib/codificator_terminalis.c:16`).
7. **Registry = standard library, opt-in per page** (VI-b): pages
   declare the few relations that fit; a new relation type joins the
   library when a second widget wants it.

## X. AUDIENDA

- Not verified: that every insula branch name maps 1:1 to a montatio
  name - today children are mounted by hand with ids like
  `<id>_notae`; R0/R4 must reconcile the naming. **Resolved 2026-10-22
  (D5 S3c):** the route library looks for `<genus id="<parent id>_
  nomen"/>` first, then `<genus id="nomen"/>` (`include/semita.h`);
  the calendar and contacts declare `<montatio nomen="notae"
  genus="scriba"/>` - `calendarium/dies/2026-10-19?notae.pagina=2`.
- Not measured: cost of applying a route (expected trivial: one branch
  write).
- Not designed: routes for documents themselves (a picture's layer, a
  scriba page's line) - entity addressing is the projections' and
  documents' business, not the pane's state.
- The URI escaping set (§IV) is a first guess; the round-trip tests
  decide. (R1: every byte outside `A-Z a-z 0-9 - _ . ~` is %XX.)
- **Found at R1, for Fran:** a route READ from state X and applied to
  a different state Y does not reproduce X exactly. Reading omits
  values equal to the default, and "absent" means keep (widget scope),
  so a widget-scoped field that is at its default in X keeps Y's value.
  Fine for restoring a fresh pane (it starts at defaults) and for
  navigation; not a faithful "copy this view onto that pane". If that
  is ever wanted: a "full" read that writes defaults explicitly.
