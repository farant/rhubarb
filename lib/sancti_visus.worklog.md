# sancti_visus worklog

## 2026-10-19 - V1: the saints pane ($saints)

Built in the shape of admonitiones_visus (dispositio in cells, one root
figure, praecepta table, album pages) with nomenclator's search prompt
(membrum__prompta) and paginarium for the record text. Header and
manifest approved by Fran before code.

- One row per saint (Fran): first feast = general, then US, then
  Martyrology (`sancti_festum_primum`, in lib/sancti.c with
  `sancti_aetas_textus` - "1762–1804", "c. 1225–1274", "d. 304",
  "4th century", "10 BC–30").
- Sorts: calendar (Jan->Dec, default; opens on the first saint on or
  after today), name, lifetime (death year, then birth, then
  century midpoint; unknown last). `s` cycles.
- Defaults are WRITTEN, not just computed: `t`, a new sort, a committed
  or cleared search all write `electus` (the tests read the field; a
  computed-only default left the field empty).
- Record view: compact header (empty lines skipped), header and body
  on 10 px lines (8 px on an 8 px font was cramped - seen in the first
  headless screenshot), Sources list with every locus (Holy See
  sources printed "Holy See · Holy See" without it).
- Corpus: vicus opens sancti/ (or RHUBARB_SANCTI) once, lazily, on the
  first mount; vicus.sh cds to the repo root, so a launch from a tree
  reads that tree's sancti/. Missing -> "No saints found (sancti/)".
- Headless screenshots: a throwaway probatio built the real vicus with
  the real corpus (tabula_pixelorum_creare_nuda + ludus_fenestra) -
  recipe kept in the session scratchpad, not committed.
- Plants 16 red (helpers 5, pane 9, wiring 1, + record-paging plant
  re-run). Three were green first and showed test gaps: 11th-century
  ordinal, record paging past the end (the drawing clamps, so only the
  drawn "k/n" indicator shows the stored index running away), the
  ambiguous anchor in vicus_applicatio (re-planted whitespace-tolerant).
