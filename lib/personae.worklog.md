# personae worklog

## 2026-10-14 - P1b: the contacts document

nomenclator-plan P1b, header approved by Fran. Built on agenda.c's
shape (one historia log, genus "personae", pools status / scheda /
acta, projection + canonical encoding for seal and checkpoint, typed
acts that refuse bad input and return seq 0). Tags via include/rubrica.h.

Forced deviation (reported): phone/email values were planned as plain
attributes `<telephonum valor="..."/>`. A value containing '"' vanished
on reopen - a plain STML attribute cannot represent a double quote
(the agenda header notes it for titles). Values now use the element
attribute form like names: `<telephonum><@valor=>...</></telephonum>`;
the header's act listing was updated to match.

Decisions inside the approved surface:
- Kind toggle never rewrites names; display name / sort key fall back
  to the other kind's name when the current one is empty.
- Names are trimmed for display, key and initial (the stored text is
  as typed). Sort key lowercases ASCII only (accents deferred, plan
  decision 12).
- Birthday "--MM-DD" (no year) validated against a leap year, so
  29 Feb is allowed; with a year, fasti_dies_valida (29 Feb 2023
  refused). Age is -1 when there is no birthday, no year, or "today"
  is before the birth.
- personae_rubricae joins every set and re-parses with rubricas_legere
  (sorted, deduped) instead of a third insertion loop.

Tests: probatio_personae (credo v2, 9 sections) incl. reopening TWICE
from the volume (checkpoint interval 3) with '"', '<', '&' in names and
values. Plants: key not lowercased; key first-name-first; age ignoring
the day; 29 Feb refused without year; kind ignored for display; deleted
contact still accepting acts; unstable order - all red (the order plant
only after a tie case was added: the first fixture had no equal keys).
Not observable (documented): the writer also drops empty list values,
but the reader drops them too, so only the stored text would differ.
