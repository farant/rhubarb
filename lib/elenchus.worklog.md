# elenchus worklog

## 2026-10-14 - M2b: the context menu library

New library for agenda-plan M2b; header approved by Fran as written.
Design follows the house popover pattern instead of inventing an
overlay: the HOST declares a transient manifest field with
aspectus="popover" (set = menu open), adds the "elenchus" subtree
(PARTES_DIALOGUS + PARTES_OPTIO children) as the LAST child of its root
(destinatio walks children last-first, so it hit-tests on top, like
pictor's 'palette'), and dimissio does the closing: Esc, a click outside
(swallowed in the same organ), and a successful action of a child that
carries `claudit`. Only enabled items get `claudit`; disabled items get
the host action WITHOUT it, so the host can swallow the click while the
menu stays open (Mac behaviour). The library holds no state: items,
highlight and anchor live in the host's insula fields.

Geometry (pixels, small font 6x8; constants in elenchus.c): item row 16
(two cells, same as list rows), divider band 8 (line in its middle,
COLOR_TEXT_DIM), margin 6 each side, 12 between the longest text and the
longest key, 2 padding top and bottom, 2px flat shadow INCLUDED in the
returned bounds (body = bounds - 2) so nothing is drawn outside the
component. A divider flag on item 0 is ignored. Item rows are inset 1px
(x 1, width body-2) so the selection fill does not cover the border.

Placement: top-left at the anchor; flip left / up when it would leave
the area (Mac); then clamp right/bottom first and left/top last, so the
top-left corner always stays visible when the menu is bigger than the
area. No paging/scrolling for long menus yet (calendar menu ~8 items).

Keys: Down/j, Up/k (j/k only without Ctrl/Cmd - same rule as the
calendar's clavis_congruit), skip disabled items, wrap; from -1 Down
goes to the first enabled, Up to the last. Moving onto the same item
(one enabled item) reports NIHIL. Return = ELECTUM only on an enabled
highlighted item. Letters are matched on `clavis` (logical, ASCII for
letters) - the calendar matches on `producta`; both are set by real key
events.

Figure: one FiguraFn for PARTES_DIALOGUS draws everything (children are
not drawn separately); ctx = the host's Elenchus, which must stay alive
while drawing. Highlight only on enabled items. Text transcribed into
md->piscina.

Tests: probatio_elenchus (credo v2, 5 sections: fines, arbor, index,
claves, figura). Plants (all compile, all red then green): no flip;
claudit on every item; disabled not skipped; key text left-aligned;
divider spacing dropped; disabled text not dim; index ignoring partes.
