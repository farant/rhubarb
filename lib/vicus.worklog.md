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
