# indagatio_visus worklog

## 2026-10-21 — S3a: the search pane (genus "indagatio")

Built on the sancti pane's shape (dispositio in cells, one root figure,
membrum__praecepta / __prompta / __album). Header and manifest text
approved by Fran 10-21; spec project-specs/indagatio.md §VII.

**Two forced deviations from the approved text:**
- `historia="impelle"` on `q` and `genus`. Route status fields default
  to *substitue*; without it a committed search never becomes a pane
  history entry, which the approved header promises ("Enter q
  committit (semita, historia lateris)").
- `semita_applicare` writes only the fields PRESENT in a route (the
  sancti tests clear with `quaestio=`). A search link is the whole
  search, so `indagatio_visus_argumentum_ponere` clears q / genus /
  pagina absent from the route (and electus), and closes an open
  prompt - a link shows results.

**One result list for everything.** The fructus is cached in the
figure context per (verba, genus, pagina, capacitas) and the index
version; componere, pingere and agere all call `_fructus` and get the
same object - one FTS query per change, not three per frame.
`indagatio_renovare` runs first so the version compared is the
post-drain one (an act between frames shows: tested with a new item).
The overview is a flat row list (cluster header, hits, "+ k more")
paged by membrum__album, which repeats the cluster header at a page
break; `electus` indexes selectable rows only. Cluster mode pages
through indagatio itself (pagina, capacitas from the pane height).

**Looked at it before planting** (scratch harness rendering the pane
to PNG, not committed): dim text vanished on the selected row (dim ==
selection-ish), the match highlight vanished there too (same colour
as the selection), and the context was cut mid-word. Fixed: selected
rows use COLOR_TEXT, matched parts on the selected row get an inverted
(background-colour) box, the title keeps its width and the context
takes the rest with "…". The corpus of a contact can start with blank
lines (empty optional fields), so the snippet line drops leading
spaces.

Plants (silva.planta, all red at their own assertions): 12 - route
not whole, commit keeps the page, results not live, "+ k more"
missing, gate given the wrong route, j does not cross a page, k lands
on the first row of the previous page, Esc clears q before the
cluster, matched parts dropped, cache key without genus, index
version ignored, Tab dead. Not plantable here: Esc with nothing left
must NOT be consumed (vicus pops the pane) - the dispatcher returns
void; S3b's vicus test covers it.
