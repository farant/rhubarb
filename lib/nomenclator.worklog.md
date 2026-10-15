# nomenclator worklog

## 2026-10-14 - P2a: the contacts list in vicus

nomenclator-plan P2a; manifest (apps/nomenclator/organum.stml) and
header approved by Fran (one Latin fix in the manifest note: "indici"
-> "indicis"). Shape copied from the calendar on purpose:
- dispositio tree in 6x8 cells (root column with a one-cell margin:
  "titulus" 3 lines, caput row 2 lines = "caput" + "pagina-prior" +
  "index_paginae" + "pagina-proxima", then the list region);
- ONE root figure (PARTES_TABULA) draws everything in root space
  (title, count, arrows, letter bands, rows) - the same convention the
  calendar's tests rely on (text found by the child's root-space box);
- rows from `album`: letter headings "l<L>" ("l_" = '#') are
  ALBUM_TITULUS, so a page starting inside a letter repeats it
  ("lR-c"); contacts "p<id>" are ALBUM_RES; 2 lines each;
- commands from `membrum__praecepta` (j k [ ] - one view for now);
  selection writes persona_electa AND the page of that contact (album_
  pagina_rei), so j across a page boundary turns the page; [ ] select
  the first contact of the new page.
The layout glue (LocusComponentis/Disponens/locum_addere/
componentem_locare, ~60 lines) is a COPY of the calendar's - second
consumer, so a candidate for a membrum__ library (noted in the plan).

Mistake on the way: I used `nomen` as a variable (twice: lib and
test) - it is the latina.h macro for typedef (CLAUDE.md says so).
Renamed appellatio / cognomen.

vicus: ContextusNomenclatoris opens ONE personae document per volume
(spatium "personae") lazily, like the agenda; genus "nomenclator";
`$contacts` via aperiens_fixum_registrare (no fixed argument).
Opening the same genus twice in one tab returns the existing pane - the
shared-document test opens the second pane in tab 2.

Tests: probatio_nomenclator (6 sections; plants: no letter headings,
k moving forward, page not following the selection, page turn without
selecting, row click ignored, tags not drawn, selection not drawn,
heading not repeated - all red) and probatio_vicus_nomenclator (2
sections; plants: document per mount, $contacts unregistered - red).
Screenshot (headless probe, deleted; copy in $TMPDIR/imago): list with
letter bands B C E G R S, tags right, selection. The business marker
(5px diamond) is barely visible and lost on the selected row - revisit
in P5.
