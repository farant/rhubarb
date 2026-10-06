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
