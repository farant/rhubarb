# vicus worklog

## 2026-10-05 — T1b: the host store (insula-rami-plan)

The host library is `vicus` (Fran: a Roman street of insulae;
`schirmata` is the legacy concha screen system and stays). One store
with roots `<vicus>` (host canons in `apps/vicus/canones/`: the durable
root declares nothing - every child is a delegated mount; the ephemeral
root declares `activa` and the dispatcher's own attributes), one volume.
The library knows no app: the host's main registers kinds (name, the
SIZE of the kind's mount struct, a mount function) and vicus allocates
each tab's mount storage.

The tab list is a manifest entry `vicus/tabulae`
(`<tabulae activa="…"><tabula id genus titulus/>…</tabulae>`) -
configuration, overwritten on change, not history (decided at T1,
veto-able); the active tab lives there too, so it survives a restart
(decision 8). Open: read the list (or write the given default), mount
each tab in order, set `activa` (the stored one, else the first). An
UNKNOWN kind in a stored list (a volume from a newer host) is kept but
not mounted, with a named cause; ADDING a tab of an unknown kind, a
duplicate id, or a title with `"` (manifest attributes are raw) is
refused.

Plants: adding without saving the list - SURVIVED at first, because the
next step (`vicus_activam_ponere`) rewrote the list anyway; the test now
reopens right after adding; the active tab not saved; an unknown kind
aborting the open; a duplicate id accepted - all caught. Forbidden
word on the way: a field named `magnitudo` (the latina `sizeof`) -
renamed `mensura`. Glossary: montator.

## 2026-10-05 — T2a: the host's registries follow the active tab

The dispatcher holds ONE action-registry pointer and each glue ONE
figura-registry pointer + one image source; registering a duplicate
name/kind is refused. So vicus owns one `ActioRegistrum` and one
`FiguraRegistrum` (handed once - the pointers never change) and on open
and on every switch empties them and copies in the active mount's
entries (`actio_registrum_vacare/_miscere`, `figura_registrum_vacare/
_miscere` - new; a merge with ANY collision adds nothing). Only the
active app is ever in the registries, so two scriba tabs never collide
on `pagina.clavis`: each resolves to its own context.

Kinds now also register `describere(montatio, &VicusFacies)` (actions,
figurae, componere + ctx, image source + ctx); the wrappers live in the
host's main (here: the test), so vicus and the apps stay unaware of
each other. `vicus_imago_fons` delegates to the active tab's source.

Plants: switching without refilling; refilling without emptying; the
composite image source ignoring the active tab - all caught.

## 2026-10-05 — T2b: host composition and mount surfaces

`vicus_componere` is a plain `Componere`: root `vicus` carrying the
action `vicus.magnitudo`, one tab-bar component (`vicus.tabulae`,
`PARTES_INDEX`, 8 px tall), then the active app's own tree with
`fines.y += 8` on its root. `fines` are parent-relative (componens.h,
destinatio subtracts each ancestor's origin), so shifting one root
moves both painting and hit-testing for the whole subtree - the test
clicks at raw screen coordinates (`manus_ludus_premere_ad`) rather than
`#pagina`, because the selector form inverts whatever transform exists
and would pass with a wrong offset too.

Surfaces: each app reads `superficies_*` from its OWN branch, so the
host writes (W, H - 8) into every mounted branch: once in `aperire`,
and on resize. Resize is not focal or positional, so `destinatio`
sends it to the root, after the dispatcher has written the ROOT's
surface (the host reads that for its own size, falling back to the
creation size). The apps' `domini.stml` give `superficies_*` to writer
`"dispensator"`; the host saves `repo->scriptor`, writes as
`"dispensator"`, restores. Plant P4 (writer "hospes") proved this is
load-bearing: the canon/owner check silently refuses and the branch
keeps no surface.

Bar figura: background rect, the active tab's cell span in
`COLOR_SELECTION`, titles at one cell in; the active title in
`COLOR_BACKGROUND` (inverse). Tab width = (title length + 2) cells.

Plants (8, all caught by name): no offset; bar not subtracted; resize
ignored; wrong writer; active not highlighted; no initial write; first
tab composed instead of the active; title not inverted. The last one
SURVIVED my first assertion ("some background pixels in the active
title" - trivially true, the bar's background supplies them); now it
also asserts ZERO text-coloured pixels there.

## 2026-10-05 — T3a: the active branch lives in Motus

Framework writes went to the store ROOT: the dispatcher's `focus` /
`focus_acervus` (`attr_legere/_scribere`) and Motus's idle flush of
`pan_x/pan_y/zoom`. In a host that is wrong twice: focus is one value
for all tabs, and the vicus root canon has no `pan_x`, so the flush is
refused and stays dirty. My first claim ("after any pictor stroke the
flush retries every idle tick") was wrong and the red test said so: a
FINISHED stroke clears `sordida` itself (pictor_actiones, "a finished
stroke does not touch the ephemera"). The real case is an idle pause
MID-drag - mouse held still past the 300 ms quiet - which flushes to
the root, is refused, and retries on every event until release. The
test now holds a stroke across a pulse.

Fix: `Motus.ramus` (an InsulaRamus; zeroed = root, so every standalone
app is byte-for-byte unchanged). Motus is the right carrier: it already
holds the per-app gesture slot, and every action receives `Motus*`, so
T3b's switch action can move it without reaching the dispatcher. The
window surface stays at the root - it is the host's.

Switch protocol (`motum_relinquere`, then `motum_aptare`):
1. leaving gesture flushed into the STILL-active branch; a dirty
   gesture that will not flush refuses the switch (text would be lost)
   with a named cause;
2. pan/zoom flushed, and dropped if refused - otherwise the next idle
   flush would land the leaving app's pan in the arriving branch;
3. capture and pending stroke dropped (a drag cannot cross tabs);
4. branch := arriving tab's, gesture slot := its `gestum_ponere` (or
   empty).
Per-tab focus then needs no save/restore: it lives in each branch.
Mounts now write their kind's default focus (`pagina`, `tabula`) into
the branch at mount time.

`VicusFacies` is memset before `describere`, so kinds that ignore the
new gesture fields get "no gesture", not garbage.

Plants (10, all caught): flush to root; focus read from root; focus
written to root; no default focus; leaving gesture not flushed; gesture
not installed; capture kept; branch not set; bind without apply;
`motus_initiare` not zeroing the branch - that last one is caught by
the EXISTING probatio_motus (its stack Motus holds garbage), the same
class the aedilis gate caught in R3.
