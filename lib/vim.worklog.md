# vim.c worklog

## 2026-10-12 - yank: yy, Y, visual y

Found during status-plan S7b (shared yank register): our vim had no
`y` at all - only `dd` and visual `d`/`x` filled the clipboard, so
copying meant deleting.
- `yy` uses the multi-key prefix like `dd` (`clavis_praecedens = 'y'`;
  any other second key is consumed and does nothing - same as `d`).
  `Y` = `yy`. Only line-wise: our visual mode is line-wise (`V`), and
  vim's `y{motion}` is not implemented.
- Visual `y` copies the selected lines, returns to normal mode and puts
  the cursor on the FIRST selected line (vim behaviour; matters when
  the selection was made upward). Column is kept, then clamped.
- Neither marks `mutatus` nor pushes undo: the sheet is untouched, so
  scriba does not flush a gesture or record a history entry.
- Test gotcha: `G` goes to the last line of the SHEET (fixed height),
  not of the text - a `G V k k y` test selected three empty lines.

## 2026-10-14 - J and backspace-join wrap by word instead of truncating

Fran noticed `J` on a long lower line ate the end of the line. Verified
it was ALWAYS so (since 37e339ca, Dec 2025 - the only later touch was
formatting), and the legacy pagina app (lib/pagina.c) and scriba share
this code: both copied with `si (dest_col < latitudo)` and then deleted
the lower line, so everything past the margin vanished. Backspace at
column 0 had the identical loop. Insert mode never had the problem
(tabula_tractare_overflow cascades).

Fix (Fran chose word boundary over character boundary):
`_jungere_per_verba(tabula, superior, pos, initium, finis)` serves both.
All fits -> copy + delete lower line (as before). Otherwise only whole
words that fit move up (break = a blank cell after a non-blank one,
tabula_est_cellula_vacua); the remainder is pulled left to the lower
line's own indentation. No word fits -> nothing changes and `mutatus`
stays FALSUM (J then adds no separator space; scriba records nothing).

Two side bugs fixed with tests:
- backspace onto an EMPTY line joined at column 1: `_finis_lineae`
  returns 0 for "no content" (for cursor clamping), so pos = 0 + 1. Now
  tabula_invenire_finem_contenti (-1) directly.
- backspace onto a FULL line put the cursor at column `latitudo` (off
  the page); now clamped to latitudo - 1.

Gotchas: finem_contenti counts tabs as content but est_cellula_vacua
calls '\t' blank - the skip-blanks loop is bounded by `finis` and a
remainder of only blanks deletes the line. Test fixtures are clipped to
the sheet width on load (tabula_ex_literis_cum_dimensionibus), so a
fixture line longer than latitudo silently tests something else.

## 2026-10-22 - register 256 x 128, VimClipboard.versio

Raised VIM_CLIPBOARD_LINEA_MAXIMA 68 -> 256 and LINEAE 32 -> 128: scriba
pages are wider than 68 (half a window / 6 px), and the new export to the
OS clipboard wants whole lines. VimUndoOperatio.data uses the same constant,
so it grows with it (32 ops x 256 B). `versio` counts register writes since
vim_clipboard_initiare (yy, visual y, dd, visual d) - a host can tell a
write happened even when the text is identical (re-yank after copying
elsewhere must still export). FAQ updated (lib/vim.interrogationes.stml).
