# dispensator.worklog.md

## 2026-09-05 — natus (ludus T10)

The loop: quiet seat, derive, target, dispatch up the ascensus,
recompose. Focus and its stack live in the ephemera island as the
attributes `focus` and `focus_acervus`, so a restore restores them
(the probatio proves focus survives `insula_restituere`). Hover
(`mus_intravit`/`mus_exiit`), `focus_captus`/`focus_amissus`, and
`focus_petitus` are derived HERE, not in `derivare`, because they
need the targeting result. Staleness: recompose after every
dispatched event, including pulses, via ping-pong piscinas; frame 0
is composed at creation. Focus-petitus in P2 goes to the root; the
owner-adjusts refinement is P3. The five derived genera joined the
`fenestra.h` enum after `EVENTUS_MUS_DUPLEX`, in the order the
eventus_stml title table already carried.

Deviations from the draft:

- Every write of `focus`/`focus_acervus` goes through
  `insula_attributum_ponere`; the draft's `stml_attributum_addere`
  would have appended a new `focus` attribute on every click and
  `dispensator_focus` would have read the first one forever.
- An empty id is never looked up: `componens_invenire_per_id` with
  an empty chorda matches an anonymous node (the first frame's root
  before any focus exists would have received `mus_exiit`).
- The toy's attribute names are `character[]` statics, not literals
  (`-Wwrite-strings` + `-Wcast-qual`); its mutators use
  `insula_attributum_ponere` so the second click reads "2", not a
  duplicate "1". `toy_ictus` refers to `motus`, not the draft's `k`.
- The toy header's functions are `interior`; under `-Wall` a probatio
  that includes it must call BOTH `toy_componere` and
  `toy_registrare`, which reach everything else. Documented in the
  header.
- `destinatio_ex_componente` tolerates NIHIL; `mittere` tolerates a
  NIHIL ascensus; creation checks every piscina it makes.

Pythonica note for the enum edit: a token anchor starts at the first
token, so a `novus` that begins with indentation doubles it (the
enum's first line came back at 8 spaces; repaired byte-exact, verdict
`cosmetica`). Lead `novus` with the token, not with spaces.

Gate: `probatio_dispensator` (41). Two planted faults: the quiet
seat flushing into a NIHIL repository — red at the `pan_x` read
after the second pulse; the staleness recompose removed — red at
`toy.compositiones > n0`. Both green on revert. Regressions after
the enum change: destinatio, eventus_stml, derivare,
fenestra_tempus all green; fenestra.h keeps its pre-existing six
long-line divergences (delta 0). Examen ACCIPE on all seven files.


## 2026-09-05 — limen (ludus P3 T5)

Derived and addressed events no longer deliver mid-dispatch. Every
`mittere_ad` (hover enter/exit, focus captured/lost, focus-petitus)
enqueues a `Differendum` (id, genus, tempus) through the public
`dispensator_addressare`, the `put` seat of brainstorm §XVI §1. The
queue is drained at the LIMEN: after the current event's dispatch
and the staleness recompose, against the NEW tree, and the drain
recomposes once more. Events addressed during delivery wait for the
next boundary — the drain works on a copy taken in the per-event
scratch arena, so `addressare` from inside a handler is safe.
Focus-petitus is two boundaries: the request goes to the root at
the first, and the "still absent, clear it" check runs after that
drain.

Why: brainstorm §XVI §3–4 — a derived event should see the world
its cause produced, not the world before it; that is the one piece
of frame-level double buffering worth having now. The toy's root
handler counts deliveries and records the composition count at
delivery time; the probatio asserts it exceeds the count before the
click. The plant that restored synchronous delivery went red there.

Cost: an event that produces derived events composes twice. The toy
replay's "one composition per event" assertion became "at least
one". Measure in T10 with the rest of the frame.

## 2026-10-05 — gesture flush and `dispensator_finire` (scriba-plan S1a)

`dispensator_tractare` now flushes the app gesture (Motus S1a) when
ITS quiet interval has passed, beside the pan/zoom flush, on event
time - so a replay flushes at the same events. New
`dispensator_finire`: flushes whatever is pending without waiting
(dirty pan/zoom, dirty gesture). Before this, NOTHING flushed at exit:
both glues just left their loop (pictor never noticed - a stroke ends
on mouse-up). `ludus_fenestra_currere` and `ludus_tessera_currere` now
call it after their loop, whatever ended it.

Tests (probatio_dispensator): no flush before the gesture's own quiet,
one at it, none again; `finire` flushes a fresh gesture and then
nothing; `finire` flushes dirty pan/zoom. Plants: no quiet flush;
`finire` skipping the gesture - caught. NOT covered headlessly: the
glues' call itself (their loops need a window / a tty) - the scriba app
proof (S3) is where it shows.

## 2026-10-05 — focus lives in the active branch (insula-rami T3a)

`attr_legere/_scribere` (focus, focus_acervus) resolve
`d->motus.ramus` (unset = root). The window surface
(`superficiem_scribere`) deliberately stays at the root - it is the
host's. Details: lib/vicus.worklog.md, same date.

## 2026-10-08 - scopes (vicus-latera S2a-1) and the focus that never clears

Scopes: a component's ACTION resolves in that component's own scope
(`componens_spatium(c)`), so a click lands in the right copy of an app
whatever pane has focus; focus, capture and Tab cycling resolve inside
`Motus.spatium` (the focused pane). Deferred derived events
(`Differendum`) now record the scope they were addressed in:
`mittere_ad` the target component's, the public `addressare` the
active one. Without it FOCUS_PETITUS addressed to the host root
('radix') was delivered to the pane root that is also called 'radix'.

Not yet scoped: hover (`d->super`) - looked up in the active scope, so
hovering the unfocused pane loses EXIIT/INTRAVIT once vicus uses
scopes. Needs `Dispensator.super_spatium` (header - proposed to Fran).

PRE-EXISTING BUG found by the scope test: the "focus requested, id
still missing -> focus cleared" branch never clears.
`dispensator_focus_ponere(d, <empty>)` -> attr_scribere ->
`insula_attributum_ponere(..., "focus", "")` -> interning "" returns
NIHIL (the interner refuses empty strings) and the old value stays.
Probe: focus 'tertia', branch taken, `post tollere 'tertia'`. No test
ever covered the branch. Fix needs a way to REMOVE an attribute (an
insula API) or an agreed empty marker - Fran's call.

Plant-runner lesson: my verdict regex `Tests Failed: \S*?[1-9]`
matched the colour code `[31m` before the 0 - two surviving plants
read as caught. Strip ANSI first; trust a plant only when a named
FRACTA line shows. A plant that breaks the LIBRARY build (unused
static under -Werror) makes the runner exit 2 = nothing ran - not
"survived".


## 2026-10-08 - focus clearing FIXED (no new API)

The "focus requested, id still missing -> cleared" branch now works:
`attr_ponere` REMOVES the attribute when the value is empty
(`insula_attributum_tollere` - it already existed, scriba uses it since
S1c; I proposed it as new without searching first). Writing "" could
never work: the interner refuses empty strings. Test: probatio_spatium
VI (scoped and unscoped); plant (branch disabled) caught by both.
Full suites unchanged (236/236, ludus_tessera 8/8): nothing relied on
the stale focus.

## 2026-10-13 — status-plan S8c-2: focus chain

Organs can mount organs (S8a/b), so focus needs to say "inside that
child". Model (approved by Fran): a branch's `focus` holds a component
id OR a child segment "elementum:id" (component ids never contain
':'). `catenam_sequi` walks from the host-set head (`Motus.ramus`,
root if none) through segments to the innermost branch and stores it in
`Motus.ramus_intimus`/`spatium_intimum`; every focus read/write goes
through it, so the chain is recomputed on each access (depth is tiny;
bounded at XVI). The head's spatium is `Motus.spatium` as the host gave
it, not `ramus.via` - probatio_spatium sets a spatium without any
branch and must keep working.

A click on a focusable component whose spatium lies under the head
calls `catenam_scribere`: each branch from the head down gets the next
segment, the component's branch gets the id. Clicks whose spatium is
NOT under the head fall back to the old single write (depth-1
behaviour, probatio_spatium IV). The "focus changed?" test now also
compares spatia: "agere" at level 1 and "agere" at level 3 are
different foci.

Segments are relative, so a stored chain survives remounting the pane
at another path. Lower branches keep their focus when a click cuts the
chain higher up - clicking back restores it.

Pan/zoom: if the click moves the innermost branch while Motus is dirty,
it is effused into the branch being left first (mirrors vicus
`motum_relinquere`); `motus_effundere` targets `ramus_intimus`. The
host's gesture slot is untouched - a nested widget with its own gesture
is deferred to A6.

Test: probatio_catena_foci, toy `ludibrium` three deep mounted through
`organum_ramum_parare` (so the generated canon judges the segment value
in `focus` - textus, accepted). The effusion section uses bare branches
because the toy manifest has no pan/zoom fields (manifest text is
Fran's call, not worth changing for this). Plant: `catenam_scribere`
refusing every nested spatium -> catena section red, reverted green.

## 2026-10-13 — A6d-2: Tab between levels, Esc leaves a nested widget

The two items S8c-2 deferred "until a real widget needs it" (the
calendar's day notes are that widget). Fran approved them as GENERAL
dispatcher behaviour:
- Tab: the native circuit now runs over the focusables of the PANE's
  namespace (radix of motus.spatium), nested namespaces included
  (`tabulam_circumire`, collecting Componens* - ids alone are ambiguous
  across levels: the toy's three levels all say "agere"). The current
  focus is matched by id AND namespace = spatium_intimum; landing in
  another namespace writes the chain (`focum_ad_componentem` ->
  catenam_scribere, fallback focus_ponere). Plain panes: pane root ==
  innermost root, so nothing changes (all focus/Tab suites green).
- Esc that nobody consumed, while focus is nested: focus moves to the
  first focusable of the NEAREST ENCLOSING level - a namespace that
  contains spatium_intimum without being it, longest wins
  (`gradum_ascendere`). First version looked the parent up by its stamp
  and failed when the parent is the host itself (unstamped, spatium "")
  - "contains, longest" handles both.
The calendar side: its action ignores key events while focus is inside
its notes (so unused keys reach these rules instead of list commands),
and scriba passes an IDLE Esc (normal mode, no pending key, no 'fd') -
like its existing Tab rule. Printable characters reach scriba as
EVENTUS_TEXTUS, not key presses (my first test sent keys and scriba
rightly ignored them).
Plants: Tab ring back to the innermost root -> red; Esc pop off -> red.

## 2026-10-15 - focus chain into a hidden nested space (Fran's bug)

Symptom (Fran, vicus): calendar Agenda tab, "+ New" - the "new:" prompt
appears but typed characters never show; minutes later (after some
other click) or after a restart it works. Headless reproduction
(manus_ludus through the real vicus) worked on every route until the
route went THROUGH the day view's notes: click into the notes (focus
chain: calendar branch `focus` = "scriba:<id>_notae"), then the Agenda
or Month BUTTON (buttons are not focusable - focus unchanged). The
notes are not composed outside the day view, but `catenam_sequi` only
asked whether the child BRANCH exists in the island (it always does),
so the chain still ended in the notes space and every key went to the
invisible scriba (scriba is modal: 'a'/'i' would start inserting into
that day's notes). A trace showed the calendar never received a key.

Fix (header text approved): `catenam_sequi` also requires the child's
space to have a root in the composed tree; if not, the chain is cut
there and that level's `focus` is rewritten to the root of its own
space (here the calendar root) - writing an empty focus is not enough:
an empty or unknown focus sends keys to the WHOLE tree's root
(destinatio), which in vicus handles nothing. `attr_ponere` /
`attr_scribere_in` moved above `catenam_sequi` (helpers before
callers). A calendar-side guard (skip the notes key pass-through
outside the day view) was tried first, proved insufficient (the keys
never reached the calendar), and was reverted rather than kept as an
unguarded second mechanism.

Tests: probatio_vicus_kalendarium `visus_post_notas` - notes ->
Agenda -> + New -> "pane" Enter creates the item; notes -> Month -> L
moves to November. Plants red: no cut; cut with empty focus.

## 2026-10-26 - dispensator_mora_proxima + refused-flush retry (vicus quies T5)

`ludus_fenestra_mora` used to return `quies_ms` (vicus: 300) every
pass, pending or not - a ~3/s wake floor once the 16 ms cap goes.
`quies_ms` is really "flush pan/zoom 300 ms after the last movement",
so the honest deadline is: motus sordida -> tempus_ultimae + quies_ms;
gestus sordidus -> gestus.tempus + gestus.quies_ms; neither -> -1.

Trap found on the way: `motus_effundere` / `motus_gestum_effundere`
clear the dirty flag only when the write succeeds. A refused flush left
the deadline at 0 forever - harmless under the old 16 ms cap, a CPU
spin in quiet mode. `dispensator_tractare` now restamps the time on
refusal (retry after quies_ms). Plant: restamp removed -> red l.387.
