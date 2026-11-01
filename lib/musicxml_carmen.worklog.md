# musicxml_carmen worklog

## 2026-10-31 — the corpus probe and what carmen refuses

Unit tests can be green while real scores fail, so I converted the whole
OpenScore corpus (1,658 files) and read each result back with
`carmina_legere` (`build/musicxml/sonda_corpus_carmen.c` under
`tools/sonda.sh`, ASan on). First run: 0 conversion failures, but carmen
rejected 1,246. Final run: 0 rejected, no ASan/UBSan reports.

What carmen enforces that the first converter didn't respect:

- **Dynamics, per carmen part:** each `<dynamica>` is checked only
  against the PREVIOUS one of the same part. It must not fall inside the
  previous ramp (before the start of bar `usque`). Positions must not go
  backwards. Two at the same position are allowed only as a step followed
  by a ramp. MusicXML commonly writes `cresc. hairpin ... f` with the `f`
  where the hairpin ends; we move the hairpin end to the next barline
  (carmen's `usque` is a bar number), so the `f` lands inside the ramp.
  Now: collect dynamics per MusicXML part, stable-sort (step before
  ramp at the same spot), and resolve. A step inside a ramp becomes the
  ramp's target value; a ramp inside a ramp is dropped (both report
  CLIVUS_MOTUS). Repeated steps at one spot: the last wins.
  `_intensiones_scribere`.
- **Fermatas:** carmen holds a fermata until the next SOUNDING note
  onset in any part (rests and tie continuations don't count). A
  fermata that starts inside the previous one, or out of order, is
  refused. Now sorted, and one inside the previous span is dropped
  silently (same hold). `_coronas_scribere`.
- **"frustum sine notis":** a line can be created and still end up with
  no notes: `_lineam_invenire` runs before the zero-length check, and
  snapping can empty a line made of one tiny note. Lines with no
  events are now removed BEFORE naming (titles stay `piano`,
  `piano_2`… without gaps).
- **Bar lengths must be sayable by a meter.** Carmen computes bar
  positions FROM the meters; a meter is N/D with N, D ≤ 64 (D a power
  of 2). When a bar's real length was off that grid (exporter
  rounding: 7:4 32nds as 17 divisions, triplet leftovers), the meter
  was impossible, carmen reused the previous one, and from then on its
  bar positions drifted from ours. That showed up as unwritable MML
  rests (2 pulses, 2,882 pulses) and "usque post initium clivi". Now
  the bar map is built in sequence: each bar runs from the previous
  bar's end toward the absolute 60-pulse grid point nearest its real
  end, and its length is rounded to the nearest length a meter can say
  (`_longitudo_metrica`: 64ths up to one whole note, 32nds up to two,
  and so on). Drift is corrected at the next bar. Each changed bar →
  EXTRA_RETICULUM.
- **End of the piece:** after the last bar is rounded down, a note can
  run past it and was written with a dangling `&`. Events are now cut
  at the end of the last bar.
- `_mensura_ab` returns the bar's start pulse too: the old code indexed
  `mm[usque - 1]` with a bar NUMBER, but empty visits have no number,
  so number ≠ index.

Plants for each of these are in the plan's P10 note.

## 2026-10-31 — strict refusal must stay alone (found by auditio)

`auditio -musicxml gratiae.musicxml -strictus` printed a second line,
"nulla nota sonans". In strict mode `_lineas_colligere` returns at the
first approximation with the line list still empty, and the new
empty-lines check then added its structural note. The header promises
that a strict refusal's relatio carries that one approximation and
nothing else. Now the converter jumps to the relatio before the
empty-lines check whenever `cv.fractus` is set; the `strictus` section
asserts `numerus_structurarum == 0` (plant red).
