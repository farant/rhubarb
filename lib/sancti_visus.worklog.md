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

## 2026-10-19 — V2: fuzzy live search, calendar link (Fran: "yes, those all sound like good approaches")

- **Live search.** While the `/` prompt is open the index reads the
  prompt's text (`_quaestio`: prompt text if open, else the committed
  `quaestio`), so the list filters per keystroke. Esc in the prompt
  restores the committed query; Enter commits. Matching is
  `sancti_congruentes` (lib/sancti.c): fzf subsequence via quaerere.h,
  each name and the id folded through `runae_basis` + ASCII lowercase,
  query split on spaces (every term must match ONE name; scores add),
  best name wins, ties by id.
- **Best-match order** = new `ordo` value `congruentia`, effective only
  while a query is active (`_ordo` falls back to calendar otherwise).
  `/` writes it; `s` cycles best match -> date -> name -> lifetime;
  Esc clearing the search and an empty commit write `calendarium`.
  I first also reset `electus` on every prompt edit - dead code: `/`
  already clears it and typing never sets it. Removed.
- **origo** (transient): `sancti_visus_argumentum_ponere` writes
  `argumentum` for an id, `index` for an empty argument. Esc in the
  record with origo=argumentum returns FALSUM, so vicus pops the pane
  (effugium_redit) and the calendar is back on the same day.
- **Calendar link** lives entirely in vicus_applicatio.c: a third
  fasti source "sancti" (`sanctos_legere` / `sanctum_aperire`), genus
  "sanctus" (unknown to kalendarium = plain row with the title),
  title "Thomas Aquinas · c. 1225–1274", referentia = corpus index.
  No kalendarium change. Double-click opens - the calendar's keyboard
  selection covers agenda items only, not source rows (birthdays have
  the same limit); Enter on a source row would be a kalendarium change.
- A same-date general+US dedupe was written and removed: no record in
  sancti/ has one (scanned), so it was untested code.
- probatio_vicus_kalendarium now sets RHUBARB_SANCTI to the fixture:
  with the real corpus Oct 15 (Teresa of Avila) would break the
  existing "no row on the 15th" birthday check.
- Test trap: `vicus_acervo_aperire(v, "calendarium", "2026-01")` kept
  day-of-month 14 from earlier state, not today; the ISO day argument
  ("2026-01-25") is the documented path (diem_in_calendario).
- ludus_tessera hand list needed `quaerere` too (sancti.c uses it) -
  the same trap that failed the V1 commit's first gate run.
