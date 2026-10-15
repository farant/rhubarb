# membrum__charta worklog

## 2026-10-15 - U5: extracted from kalendarium (A6d/A6e)

The calendar's day-notes machinery (notarum_area, notarum_mensurae,
librum_diei_aperire, notas_montare, librum_aptare, notas_componere, the
shadow in figura_tabulae, the focus check in the agere wrapper) moved
here behind a KEY: the host builds the volume namespace prefix
("dies/2026-10-15", "persona/7"), the library opens/swaps the book.

Split of responsibilities:
- host: the key, where the locus is (componens lookup of its "notae"
  child), and the EXPECTED locus size before layout (the calendar
  derives it from superficies_*; nothing is laid out at mount time).
- library: page size of NEW pages from that locus size (area with an
  unbounded page, minus scriba margins II and status line I - so a new
  page never scrolls), the cell-exact area at compose/draw time
  (clamped to the current page), registry merge into the host's
  registries in the scriba branch's space, gesture flush on a key swap.

Deviation from the calendar: the focus test is now "intimate space
under the SCRIBA branch" instead of "anything nested under the host
branch". Equivalent while the notes are the only nested space; it stops
being accidentally true when a host nests something else.

`volumen NIHIL` = not mounted, VERUM: every other call is a no-op (the
calendar test worlds without a volume rely on this).

Cell size CHARTA_LITTERA/LINEA (6x8) is scriba's glyph cell, duplicated
from KALENDARIUM_LITTERA/LINEA - if scriba ever exports its cell size,
point these at it.

Plants (silva.planta, all red on the intended assertion): page clamp,
same-key no-op, registry merge, focus argument order, compose space,
shadow when not mounted.
