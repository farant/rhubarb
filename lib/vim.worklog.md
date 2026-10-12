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
