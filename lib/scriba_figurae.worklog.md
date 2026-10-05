# scriba_figurae (+ scriba_componentia) worklog

## 2026-10-05 — S2: the page drawn and laid out

**Tree (`scriba_componere`)**, pictor's shape: a column in CELLS via
dispositio - viewport (`PARTES_PROSPECTUS`, grows, clips) over a
one-row status (`PARTES_TITULUS`); in the viewport the page
(`PARTES_CAMPUS`) one cell in from the edge (the frame shows), action
`pagina.clavis`, focusable. Figure data rides in the tree the way
pictor's pending stroke does (`PRAEDICATUM_PROPRIUS`): `puncta[0]` =
cursor, `puncta[1]` = visual anchor, `titulus` = mode. The sheet text
comes through the figure's context (the working sheet) - as pictor's
document image does - so insert mode shows uncommitted text.

**Scrolling without state:** when the sheet plus its one-cell margin
overflows the viewport, the view centres on the cursor, clamped to the
sheet's edges - a pure function of the cursor (a 56-line sheet in a
24-row terminal; 70 columns in a 320-px window). Default, veto-able
(the alternative is paging).

**Look:** paper COLOR_BACKGROUND on the desk COLOR_SUPERFICIES with a
COLOR_BORDER frame; text COLOR_TEXT, trailing spaces dropped, tabs and
control bytes drawn as spaces; a steady block cursor (no blink - one
clock) with its character redrawn in the paper colour; visual selection
as whole lines (as the legacy page), text inverted; status "MODUS l:c"
(1-based), the mode word in its status colour.

**The cursor colour was invisible per mode.** First draft: insert =
COLOR_STATUS_INSERT, otherwise COLOR_CURSOR. A plant ignoring the mode
SURVIVED - in the theme both are PALETTE_BRIGHT_GOLD. Now the cursor
takes its mode's STATUS colour (normal grey, insert gold, visual leaf -
COLOR_STATUS_VISUAL existed and was unused), matching the status word;
the test asserts insert != normal so a theme change cannot merge them
silently. Paper (warm grey) and the normal cursor (medium grey) differ
(checked).

Tests (probatio_scriba_figurae): tree bounds and data, pixels through
`pingere` -> `delineare_mandata` (desk, paper, frame, text, cursor per
mode, inverse character, selection, status colours), scrolling
including both clamps. Plants: page not offset; cursor colour
ignoring mode (after the fix); no inverse character; no selection; no
upper clamp; status colour fixed - all caught.

## 2026-10-05 — the frame belongs in the margin cells (Fran's S3 look)

Fran: "the border is also part of the editable buffer so in the
terminal you can type over the border and it disappears". Cell dump of
the terminal frame: row 0 (margin) plain desk; the box-drawing frame in
the sheet's FIRST column and row, under the text. Cause: tessellatio
maps a rectangle to cells by rounding each edge to the NEAREST cell
boundary and draws an unfilled rectangle on the inner ring of that box;
a frame 1 px outside the paper rounds onto the paper's own edge cells.

Fran chose the frame in the margin cells (not insetting the editable
area, which would change the document to 66×54): the frame rectangle is
now one full cell out, `(-cw, -ch, w + 2cw, h + 2ch)`. Window: the 1-px
line sits ~5 px out from the paper (a mat of desk between). pictor had
the same cause (its frame covered the drawing's edge cells in the
terminal): `PictorFigurae` gained the cell size (0 = the old 1 px),
set by `pictor_applicatio`; its golden `pictor_prima.png` regenerated -
the difference image showed only the old frame lines gone and the new
right line one cell out - inspected, promoted.

Pinned in the terminal replay (probatio_ludus_tessera_scriba): the
sheet's first column is a space, the margin cells hold frame
characters; the old placement fails exactly there. The S2 pixel test
moved the frame check to x = 0 and asserts desk at the old x = 5.
